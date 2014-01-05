#include "pathalgorithms.h"
#include <math.h>
#include <queue>
#include <list>
#include "../db/node.h"
#include "../db/Relations/waysegment.h"
#include "../db/database.h"


float PathAlgorithms::EarthRadius = 6372.8;
double PathAlgorithms::pi = 3.14159265358979323846;




PathAlgorithms::PathAlgorithms()
{
}
double PathAlgorithms::deg2rad(const double &deg){
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double PathAlgorithms::rad2deg(const double &rad){
  return (rad * 180 / pi);
}
float PathAlgorithms::calculateDistancePoints(boost_xy_point &a, boost_xy_point &b){
    //Haversine formula
    //http://en.wikipedia.org/wiki/Haversine_formula
    //http://andrew.hedges.name/experiments/haversine/

    //we convert everything to radians
    double a_lat = deg2rad(a.y());
    double a_lon = deg2rad(a.x());
    double b_lat = deg2rad(b.y());
    double b_lon = deg2rad(b.x());

    //calc sin values
    double u = sin((b_lat - a_lat)/2);
    double v = sin((b_lon - a_lon)/2);

    return (2.0 * EarthRadius * asin(sqrt(u*u + cos(a_lat)*cos(b_lat)*v*v)));
}
//Calculates air distance between two nodes
float PathAlgorithms::calculateDistancePoints(Node* &A, Node* &B){
    return calculateDistancePoints(A->getGeoPosition(),B->getGeoPosition());
}
float PathAlgorithms::calculateDistancePoints(vector<Node*> &points){
    float c = 0.0f;
    //we always look at pair of points for distance, so we dont have to take last point
    for(vector<Node*>::iterator it = points.begin();(it+1)!=points.end();it++){
        c+=calculateDistancePoints((*it)->getGeoPosition(),(*(it+1))->getGeoPosition());
    }
    return c;
}
void PathAlgorithms::safelyDeletePath(Path* &p){
    safelyDeletePath(*p);
    delete p;
}
void PathAlgorithms::safelyDeletePath(Path &p){
    for(vector<PathSegment*>::iterator it = p.getPathSegmentsBegin();it!=p.getPathSegmentsEnd();it++){
        delete (*it);
    }
}
boost_xy_point& PathAlgorithms::projectPointToSegment(boost_xy_point &A,boost_xy_point &B, boost_xy_point &C){
    double x1 = B.x()-A.x();
    double y1 = B.y()-A.y();
    double x2 = C.x()-A.x();
    double y2 = C.y()-A.y();

    //t = (B-A) . (C-A) / ||(B-A)||
    double t = (x1*x2+y1*y2)/(x1*x1+y1*y1);

    if(t>1){
        t=1.0;
    }
    else if(t<0){
        t=0.0;
    }
    //D = A + t*(B-A)
    boost_xy_point *D = new boost_xy_point(A.x()+t*(x1),A.y()+t*(y1));

    return *D;
}

//Find closest way segment and point of contact to a point A
bool PathAlgorithms::findClosestWaySegment(Database &d,boost_xy_point &A,ns_permisions::transport_type &tt, WaySegment* &closestSegment, boost_xy_point &pointOfContact){
    /*
     * Finding the segment is done by finding a segment with shortest distance between point A and point of contact (perpendicular projection on the segment).
     * We exam all segments in some neighborhood that point A lies in their bounding box
     * If there are no segments in the area we increase the size of the area
     *
     */
    //Borders of the neighborhood for search
    boost_xy_point min(A.x(),A.y());
    boost_xy_point max(A.x(),A.y());
    vector<WaySegment*> segments_in_area;
    //Step with which we increase the search area
    float closest_search_step=0.0002;
    int closest_search_max_steps = 20;

    int i=0;
    do{
        min.set<0>(min.x()-closest_search_step);
        min.set<1>(min.y()-closest_search_step);
        max.set<0>(max.x()+closest_search_step);
        max.set<1>(max.y()+closest_search_step);
        d.searchWaySegmentsInArea(min,max,segments_in_area,tt);
        i++;
    }while(segments_in_area.empty() && i<closest_search_max_steps);

    //return false if no segments are in the area after max_steps
    if(i==closest_search_max_steps)
        return false;

    float min_distance=-1;
    boost_xy_point min_proj;
    WaySegment* min_segment;

    //For each of the segments in the area we calculate the projection of the point onto them
    for(vector<WaySegment*>::iterator it = segments_in_area.begin();it!=segments_in_area.end();it++){
        //Projection of the point
        boost_xy_point proj = projectPointToSegment((*it)->getPointA()->getGeoPosition(),(*it)->getPointB()->getGeoPosition(),A);
        //Distance between our point and projection
        float d = calculateDistancePoints(proj,A);
        //Remember the closest one
        if(d<min_distance || min_distance==-1){
            min_distance = d;
            min_segment = *it;
            min_proj.set<0>(proj.x());
            min_proj.set<1>(proj.y());
        }
    }

    closestSegment = min_segment;
    pointOfContact.set<0>(min_proj.x());
    pointOfContact.set<1>(min_proj.y());
    return true;
}



void PathAlgorithms::constructBackShortestPath(PathSegment* &best_path,Node* &element, Node* &start){
    //traverse back to the start node
    if(element->getPrevWaySegment()->getPointA()!=start){
        Node* n = element->getPrevWaySegment()->getPointA();
        constructBackShortestPath(best_path,n,start);
    }

    //Copy data for each node and insert it in PathSegment
    Node *nA = new Node(element->getPrevWaySegment()->getPointA());
    Node *nB = new Node(element->getPrevWaySegment()->getPointB());
    WaySegment *ws = new WaySegment(nA,nB,element->getPrevWaySegment()->getWay());
    float cs = element->getPrevWaySegment()->getCost();
    ws->setCost(cs);

    best_path->addSegment(ws);
}



/*
 * SHORTEST PATH ALGORITHMS A->B
 */

//Find shortest path between two nodes spcified by pointers to nodes
PathSegment* & PathAlgorithms::findShortestPath(Node* &start, Node* &end,ns_permisions::transport_type &tt){
    //Best path
    PathSegment* best_path = new PathSegment(0);

    //Openset for nodes to be
    priority_queue<Node*, std::vector<Node*>, CompareNodes> open_set;
    set<unsigned long int> open_set_elements;

    //<id, node pointer of the node>
    map<unsigned long int,Node*> closed_set;

    //We add the first point to open set
    start->setGscore(0.0);
    start->setFscore(calculateDistancePoints(start,end));

    open_set.push(start);
    open_set_elements.insert(start->getId());

    //set g_score and f_score

    Node* current;
    while(!open_set.empty()){
        //get the element from open_set with the smallest f_score
        current = open_set.top();

        if(current->getId()==end->getId()){
            //we came to end node so all we have to do is to reconstruct the path
            constructBackShortestPath(best_path,current,start);
            return best_path;
        }

        //erase the first current node
        open_set.pop();
        open_set_elements.erase(current->getId());

        //add current node to close set
        closed_set.insert(make_pair<unsigned long int,Node*>(current->getId(),current));

        //For each of the neighbors of current we check:
        for(vector<WaySegment*>::iterator it = current->getOutcomingEdgesBegin(); it!=current->getOutcomingEdgesEnd();it++){
            //check if it is for the right type of road
            
            WaySegment* neigh_edge = *it;
            if(neigh_edge->isPermitedOnThisRoad(tt)){
                Node* neigh = neigh_edge->getPointB();

                //calculate potential scores for neighbor
                float temp_g_score = current->getGscore() + neigh_edge->getCost();
                float temp_f_score = temp_g_score + calculateDistancePoints(neigh,end);

                //if it is already in closed_set and with smaller f_score
                //we already checked the better value so we just skip
                if(closed_set.find(neigh->getId())!=closed_set.end() && neigh->getFscore()<temp_f_score){
                    continue;
                }

                //if node is not in open_set or the temp_f_score is smaller than f_score of node
                set<unsigned long int>::iterator it_set = open_set_elements.find(neigh->getId());
                if(it_set==open_set_elements.end() || temp_f_score < neigh->getFscore()){
                    //we set where did we came from
                    neigh->setPrevWaySegment(neigh_edge);
                    //set g and f score of the neighbor
                    neigh->setFscore(temp_f_score);
                    neigh->setGscore(temp_g_score);
                    if(it_set==open_set_elements.end()){
                        open_set.push(neigh);
                        open_set_elements.insert(neigh->getId());
                    }

                }
            }

        }

    }
    return best_path;
}

//Find shortest path between two nodes specified by id from database
int PathAlgorithms::findShortestPath(Database &d,unsigned long int &A, unsigned long int &B,ns_permisions::transport_type &tt,Path &best_path){
    //returns 0 - if everything ok
    // 1 - if node A not ok
    // 2 - if node B not ok
    // 3 - no path found
    Node* nodeA = d.getNodeById(A);
    Node* nodeB = d.getNodeById(B);
    if(nodeA && nodeB){
        PathSegment* path_segment =  findShortestPath(nodeA, nodeB,tt);
        if(!path_segment->isEmpty())
            best_path.addSegment(path_segment);

        if(best_path.isEmpty())
            return 3;
        return 0;
    }
    else if(nodeA){
        return 2;
    }
    else
        return 1;
}

//Find shortest path between two points on map with specified closest waysegments
PathSegment* & PathAlgorithms::findShortestPath(Node* &node_start,WaySegment* &start_segment, Node* &node_end, WaySegment* &end_segment, ns_permisions::transport_type &tt){
    //Create way segment between start node and nodes on the closest segment
    //Add connection:   Start -> startNodeA
    //                  Start -> startNodeB
    if(start_segment->getWay()->getOnewayType()==ns_oneway::yes || start_segment->getWay()->getOnewayType()==ns_oneway::opposite){
        //Check if way is oneway
        //add connection Start->startNodeB
        WaySegment* wsA = new WaySegment(node_start,start_segment->getPointB(),start_segment->getWay());
        node_start->addOutcomingConnection((WaySegment*)wsA);
    }
    else{
        //add connection Start->startNodeB
        WaySegment* wsA = new WaySegment(node_start,start_segment->getPointB(),start_segment->getWay());
        node_start->addOutcomingConnection((WaySegment*)wsA);
        //add connection Start->startNodeA
        WaySegment* wsB = new WaySegment(node_start,start_segment->getPointA(),start_segment->getWay());
        node_start->addOutcomingConnection((WaySegment*)wsB);
    }

    //Create way segment between nodes on the end segment and end node
    //Add connection:   endNodeA -> End
    //                  endNodeB -> End
    if(end_segment->getWay()->getOnewayType()==ns_oneway::yes || end_segment->getWay()->getOnewayType()==ns_oneway::opposite){
        //Check if way is oneway
        //add connection EndNodeA -> End
        WaySegment* wsA = new WaySegment(end_segment->getPointA(),node_end,end_segment->getWay());
        end_segment->getPointA()->addOutcomingConnection((WaySegment*)wsA);
    }
    else{
        //add connection EndNodeA -> End
        WaySegment* wsA = new WaySegment(end_segment->getPointA(),node_end,end_segment->getWay());
        end_segment->getPointA()->addOutcomingConnection((WaySegment*)wsA);
        //add connection EndNodeB -> End
        WaySegment* wsB = new WaySegment(end_segment->getPointB(),node_end,end_segment->getWay());
        end_segment->getPointB()->addOutcomingConnection((WaySegment*)wsB);
    }


    PathSegment* p_segment = findShortestPath(node_start, node_end,tt);

    //remove waysegments created in node_start
    if(!(start_segment->getWay()->getOnewayType()==ns_oneway::yes || start_segment->getWay()->getOnewayType()==ns_oneway::opposite)){
        delete node_start->removeLastOutcomingConnection();
    }
    delete node_start->removeLastOutcomingConnection();

    //end point
    if(!(end_segment->getWay()->getOnewayType()==ns_oneway::yes || end_segment->getWay()->getOnewayType()==ns_oneway::opposite)){
        delete end_segment->getPointB()->removeLastOutcomingConnection();
    }
    delete end_segment->getPointA()->removeLastOutcomingConnection();

    return p_segment;
}


//Find shortest path between two lon-lat points
int PathAlgorithms::findShortestPath(Database &d, boost_xy_point &A, boost_xy_point &B,ns_permisions::transport_type &tt,Path &best_path){
    //returns 0 if everything ok - path found
    // 1 - node A doesnt found segment
    // 2 - node B doesnt found segment
    // 3 - no path found

    //Find closest point on road to point A
    WaySegment* start_segment;
    boost_xy_point pointOfContactStart;
    //If we cant find closest segment we return empty path
    if (!findClosestWaySegment(d,A,tt,start_segment,pointOfContactStart)){
         return 1;
    }
    Node *node_start = new Node();
    node_start->setGeoPosition(pointOfContactStart.x(),pointOfContactStart.y());

    //Find closest point on road to point B
    WaySegment* end_segment;
    boost_xy_point pointOfContactEnd;

    //If we cant find closest segment we return empty path
    if (!findClosestWaySegment(d,B,tt,end_segment,pointOfContactEnd)){
        return 2;
    }
    Node *node_end = new Node(1);
    node_end->setGeoPosition(pointOfContactEnd.x(),pointOfContactEnd.y());


    /*
    * SHORTEST PATH SEARCH A->B between two nodes
    */
    PathSegment* p_segment = PathAlgorithms::findShortestPath(node_start,start_segment,node_end,end_segment,tt);
    if(!p_segment->isEmpty())
        best_path.addSegment(p_segment);

    //we delete all the waysegments at start point and start point
    delete node_start;
    delete node_end;

    if(p_segment->isEmpty())
        return 3;
    return 0;
}




int PathAlgorithms::findPathsInRadius(Database &d, boost_xy_point &A, POICategory* &p_cat, float &max_radius, ns_permisions::transport_type &tt,set<Path*,ComparePaths> &all_paths){
    //returns 0 - if everything ok - paths found
    // 1 - node A doesnt found segment
    // 3 - no path found

    //Find closest point on road to point A
    WaySegment* start_segment;
    boost_xy_point pointOfContactStart;
    if (!findClosestWaySegment(d,A,tt,start_segment,pointOfContactStart)){
        return 1;
    }
    Node *node_start = new Node();
    node_start->setGeoPosition(pointOfContactStart.x(),pointOfContactStart.y());


    for(vector<POIPoint*>::iterator it = p_cat->getPOIPointsBegin();it!=p_cat->getPOIPointsEnd();it++){
        //Find closest point on road to the POI
        WaySegment* end_segment;
        boost_xy_point pointOfContactEnd;

        if (!findClosestWaySegment(d,(*it)->getGeoPosition(),tt,end_segment,pointOfContactEnd)){
            continue;
        }
        Node *node_end = new Node(1);
        node_end->setGeoPosition(pointOfContactEnd.x(),pointOfContactEnd.y());
        node_end->setPOI((*it));


        //if air distance is bigger than max radius we can have smaller distance
        if(max_radius!=0){
            float air_distance = calculateDistancePoints(node_start->getGeoPosition(),node_end->getGeoPosition());
            if(air_distance>max_radius){
                delete node_end;
                continue;
            }
        }

        //////////////////////////////////
        //Search algorithm
        //////////////////////////////////

        PathSegment* path_seg = PathAlgorithms::findShortestPath(node_start,start_segment,node_end,end_segment,tt);
        if(!path_seg->isEmpty()){
            Path* cur_path = new Path();
            cur_path->addSegment(path_seg);
            float current_distance = cur_path->getCost();
            if(current_distance<max_radius || max_radius==0){
                all_paths.insert(cur_path);
            }
        }
        delete node_end;
    }

    //we delete all the waysegments at start point and start point
    delete node_start;

    if(all_paths.empty())
        return 3;
    return 0;

}


void printVectorContent4(std::priority_queue<vector<Node*>, vector<vector<Node*> >,CompareVectorNodes > paths){
    int ii=0;
    while(!paths.empty()){
        vector<Node*> p (paths.top());
        vector<Node*>::iterator it_r = p.begin();
        float dist_r=0.0f;
        for(;(it_r+1)!=p.end();it_r++){
            dist_r+=PathAlgorithms::calculateDistancePoints((*it_r),*(it_r+1));
        }
        cout<<"Path "<<ii<<": "<<dist_r<<endl;

        for(int i=0;i<paths.top().size();i++){
            if(i==0){
                cout<<"Start"<<endl;
            }
            else if(i==(paths.top().size()-1)){
                cout<<"End"<<endl;
            }
            else{
                cout<<paths.top().at(i)->getPOI()->getName()<<endl;
            }
        }
        cout<<"--------------"<<endl;
        paths.pop();
    }
}

vector<vector<Node*> > createPOIList(Database &d,vector<unsigned int> &category_list){
    vector<vector<Node*> > poi_list;
    //go through all category ids and get related POI
    for(vector<unsigned int>::iterator it_cat=category_list.begin();it_cat!=category_list.end();it_cat++){
        //Put POIs in the list and add to global list
        vector<Node*> points;
        POICategory* p_cat = d.getPOICategoryById(*it_cat);
        for(vector<POIPoint*>::iterator it_poi = p_cat->getPOIPointsBegin();it_poi!=p_cat->getPOIPointsEnd();it_poi++){
            Node* n = new Node(1000);
            n->setPOI((*it_poi));
            n->setGeoPosition((*it_poi)->getGeoPosition().x(),(*it_poi)->getGeoPosition().y());
            points.push_back(n);
        }
        poi_list.push_back(points);
    }

    return poi_list;
}
list<vector<Node*> > PathAlgorithms::streachPaths(vector<vector<Node*> > &poi_list,vector<vector<Node*> >::iterator cur_poi_category){
    list<vector<Node*> > all_paths;
    //If this category is not last in the set:
    if((cur_poi_category+1)!=poi_list.end()){
        //Get points on path from next category
        list<vector<Node*> > past_paths = streachPaths(poi_list,cur_poi_category+1);
        //add all the paths from previous categories to EACH of poi in this category
        for(vector<Node*>::iterator it_poi = ((*cur_poi_category).begin());it_poi!=((*cur_poi_category).end());it_poi++){
            for(list<vector<Node*> > ::iterator it = past_paths.begin();it!=past_paths.end();it++){
                vector<Node*> p;
                p.push_back((*it_poi));
                p.insert(p.end(),(*it).begin(),(*it).end());
                all_paths.push_back(p);
            }
        }
    }
    else{
        //last category in the set
        for(vector<Node*>::iterator it_poi = ((*cur_poi_category).begin());it_poi!=((*cur_poi_category).end());it_poi++){
            vector<Node*> p;
            p.push_back((*it_poi));
            all_paths.push_back(p);
        }
    }
    return all_paths;
}


int PathAlgorithms::BicycleSearch(Database &d,boost_xy_point &A,boost_xy_point &B, vector<unsigned int> &category_list,float max_radius,bool direction,ns_permisions::transport_type &tt,Path &final_path){
    //returns 0 if everything ok - path found
    // 3 - no path found
    vector<vector<Node*> > pois_list = createPOIList(d,category_list);

    //Add start and end node to every vector of nodes
    Node *startN = new Node();
    startN->setGeoPosition(A.x(),A.y());
    Node *endN = new Node(1);
    endN->setGeoPosition(B.x(),B.y());

    //Construct all possible paths
    list<vector<Node*> > all_paths = streachPaths(pois_list,pois_list.begin());

    //Go through all the paths and add to queue the ones that in best option (air distance) is smaller than radius
    std::priority_queue<vector<Node*>, vector<vector<Node*> >,CompareVectorNodes >queue_paths;
    //Add start and end node to every path
    for(list<vector<Node*> >::iterator it = all_paths.begin();it!=all_paths.end();it++){
        vector<Node*> p(*it);
        p.insert(p.begin(),startN);
        p.insert(p.end(),endN);
        if(PathAlgorithms::calculateDistancePoints(p)<max_radius || max_radius==0){
            queue_paths.push(p);
        }
    }

//    cout<<"---------------------"<<endl<<"PATHS priority"<<endl<<"-------------------------"<<endl;
//    printVectorContent4(queue_paths);
//    cout<<"----------------------------------------------"<<endl<<endl;


    //go through each (in min distance order) and calculate real path
    //when our real calculated value is smaller than next smallest air distance of path we are done -> it cant be smaller
    int n_path=0;
    Path* min_path=NULL;
    while(!queue_paths.empty()){
        vector<Node*> vec_points(queue_paths.top());
        //if min possible distance (air distance) is bigger than min distance of path
        //end search...all after that will be just bigger
        if(min_path!=NULL && PathAlgorithms::calculateDistancePoints(vec_points)>min_path->getCost()){
            break;
        }
        Path* path = new Path();
        bool complete_path=true;
        for(vector<Node*>::const_iterator it = vec_points.begin();(it+1)!=vec_points.end();it++){
            int found = PathAlgorithms::findShortestPath(d,(*it)->getGeoPosition(),(*(it+1))->getGeoPosition(),tt,*path);
            if(found!=0){
                complete_path = false;
                break;
            }
            else
            {
                if(max_radius!=0 && path->calculateCost()>max_radius){
                    complete_path = false;
                    break;
                }
            }
        }
        if(complete_path){
            //It is complete and in radius
            if(min_path==NULL){
                min_path = path;
            }
            else if(min_path->getCost()>path->getCost()){
                PathAlgorithms::safelyDeletePath(min_path);
                min_path = path;
            }
        }
        else{
            //not complete because of different reasons
            //delete path and path segments
            PathAlgorithms::safelyDeletePath(path);
        }
        queue_paths.pop();
        n_path++;
    }

    //delete all created nodes
    for(vector<vector<Node*> >::iterator it=pois_list.begin();it!=pois_list.end();it++){
        for(vector<Node*>::iterator it_p = (*it).begin();it_p!=(*it).end();it_p++){
            delete *it_p;
        }
    }
    //delete start and end node
    delete startN;
    delete endN;

    if(min_path==NULL){
        //we didnt find any paths
        return 3;
    }
    else{
        final_path.addSegments(*min_path);
    }
    return 0;
}






