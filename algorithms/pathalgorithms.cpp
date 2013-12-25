#include "pathalgorithms.h"
#include <math.h>
#include <queue>
#include <set>
#include "../db/node.h"
#include "../db/Relations/waysegment.h"
#include "../db/database.h"


float PathAlgorithms::EarthRadius = 6372.8;
double PathAlgorithms::pi = 3.14159265358979323846;

struct CompareNodes
{
    bool operator()(Node* l, Node* r) {return l->getFscore() > r->getFscore();}
};


PathAlgorithms::PathAlgorithms()
{
}
double PathAlgorithms::deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double PathAlgorithms::rad2deg(double rad) {
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
float PathAlgorithms::calculateDistancePoints(Node* A, Node* B){
    return calculateDistancePoints(A->getGeoPosition(),B->getGeoPosition());
}

boost_xy_point& PathAlgorithms::projectPointToSegment(boost_xy_point &A,boost_xy_point &B, boost_xy_point &C){
    float x1 = B.x()-A.x();
    float y1 = B.y()-A.y();
    float x2 = C.x()-A.x();
    float y2 = C.y()-A.y();

    //t = (B-A) . (C-A) / ||(B-A)||
    float t = (x1*x2+y1*y2)/(x1*x1+y1*y1);

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

bool PathAlgorithms::findClosestWaySegment(Database *d,boost_xy_point &A,ns_permisions::transport_type &tt, WaySegment* &closestSegment, boost_xy_point &pointOfContact){
    boost_xy_point min(A.x(),A.y());
    boost_xy_point max(A.x(),A.y());
    vector<WaySegment*> segments_in_area;
    float step=0.0002;
    int i=0;
    while(segments_in_area.empty() && i<20){
        min.set<0>(min.x()-step);
        min.set<1>(min.y()-step);
        max.set<0>(max.x()+step);
        max.set<1>(max.y()+step);
        d->searchWaySegmentsInArea(min,max,segments_in_area,tt);
        i++;
    }
    if(i==20)
        return false;

    float min_distance=-1;
    boost_xy_point min_proj;
    WaySegment* min_segment;

    for(vector<WaySegment*>::iterator it = segments_in_area.begin();it!=segments_in_area.end();it++){
        boost_xy_point proj = projectPointToSegment((*it)->getPointA()->getGeoPosition(),(*it)->getPointB()->getGeoPosition(),A);
        float d = calculateDistancePoints(proj,A);

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



void PathAlgorithms::constructBackShortestPath(vector<WaySegment*> &best_path,Node* element, Node* start){
    if(element->getPrevNodeSegment()->getPointA()!=start){
        constructBackShortestPath(best_path,element->getPrevNodeSegment()->getPointA(),start);
    }

    //COPY DATA and PUT IT IN BEST_PATH
    Node *nA = new Node(element->getPrevNodeSegment()->getPointA());
    Node *nB = new Node(element->getPrevNodeSegment()->getPointB());
    WaySegment *ws = new WaySegment(nA,nB,element->getPrevNodeSegment()->getWay());
    float cs = element->getPrevNodeSegment()->getCost();
    ws->setCost(cs);

    best_path.push_back(ws);
}

void PathAlgorithms::safelyDeletePath(vector<WaySegment*> &path){
    for(vector<WaySegment*>::iterator it = path.begin();it!=path.end();it++){
        delete (*it)->getPointA();
        delete (*it)->getPointB();
        delete (*it);
    }
    path.clear();
}



/*
 *Find Shortest Path Algorithms *
 */

//Search shortest path by nodes
bool PathAlgorithms::findShortestPath(Node* start, Node* end,ns_permisions::transport_type &tt,vector<WaySegment*> &best_path){
    //Openset for nodes to be
    priority_queue<Node*, std::vector<Node*>, CompareNodes> open_set;
    set<unsigned long int> open_set_elements;

    //<id, node pointer of the node>
    map<unsigned long int,Node*> closed_set;

    //We add the first point to open set
    //open_set.insert(make_pair<float,Node*>(0.0,start));
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
            return true;
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
                    neigh->setPrevNodeSegment(neigh_edge);
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
    return false;
}

//Search shortest path by node IDs
bool PathAlgorithms::findShortestPath(Database *d,unsigned long int A, unsigned long int B,ns_permisions::transport_type &tt, vector<WaySegment*> &path,float &total_distance, float &total_time){
    Node* nodeA = d->getNodeById(A);
    Node* nodeB = d->getNodeById(B);
    if(nodeA && nodeB){
        return findShortestPath(nodeA, nodeB,tt,path);
        total_distance = distanceToTravelPath(path);
        total_time = timeToTravelPath(tt,path);
    }
    return false;
}
//Find shortest path by any two positions on the map
bool PathAlgorithms::findShortestPath(Database *d, boost_xy_point &A, boost_xy_point &B,ns_permisions::transport_type &tt, vector<WaySegment*> &path,float &total_distance, float &total_time){

    //Find closest point on road to point A
    WaySegment* start_segment;
    boost_xy_point pointOfContactStart;
    if (!findClosestWaySegment(d,A,tt,start_segment,pointOfContactStart)){
        return false;
    }
    Node *node_start = new Node();
    node_start->setGeoPosition(pointOfContactStart.x(),pointOfContactStart.y());

    //Create waysegment with nodes on closest waysegment
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


    //Find closest point on road to point A
    int flag_end_way=0; //0 - twoway, 1 - oneway or opposite
    WaySegment* end_segment;
    boost_xy_point pointOfContactEnd;

    if (!findClosestWaySegment(d,B,tt,end_segment,pointOfContactEnd)){
        return false;
    }
    Node *node_end = new Node(1);
    node_end->setGeoPosition(pointOfContactEnd.x(),pointOfContactEnd.y());

    //Create waysegment with nodes on closest waysegment
    //Add connection:   endNodeA -> End
    //                  endNodeB -> End
    if(end_segment->getWay()->getOnewayType()==ns_oneway::yes || end_segment->getWay()->getOnewayType()==ns_oneway::opposite){
        //Check if way is oneway
        //add connection EndNodeA -> End
        WaySegment* wsA = new WaySegment(end_segment->getPointA(),node_end,end_segment->getWay());
        end_segment->getPointA()->addOutcomingConnection((WaySegment*)wsA);
        flag_end_way=1;
    }
    else{
        //add connection EndNodeA -> End
        WaySegment* wsA = new WaySegment(end_segment->getPointA(),node_end,end_segment->getWay());
        end_segment->getPointA()->addOutcomingConnection((WaySegment*)wsA);
        //add connection EndNodeB -> End
        WaySegment* wsB = new WaySegment(end_segment->getPointB(),node_end,end_segment->getWay());
        end_segment->getPointB()->addOutcomingConnection((WaySegment*)wsB);
    }

    //////////////////////////////////
    //Search algorithm
    //////////////////////////////////


    bool found = findShortestPath(node_start, node_end,tt,path);

    total_distance = distanceToTravelPath(path);
    total_time = timeToTravelPath(tt,path);

    //we delete all the waysegments at start point and start point
    delete node_start;

    //end point
    if(flag_end_way==0){
        delete end_segment->getPointB()->removeLastOutcomingConnection();
    }
    delete end_segment->getPointA()->removeLastOutcomingConnection();
    delete node_end;


    return found;
}

float PathAlgorithms::timeToTravelPath(ns_permisions::transport_type &tt, vector<WaySegment*> &path){
    float time=0.0f;
    for (vector<WaySegment*>::iterator it=path.begin(); it!=path.end(); it++){
        float d=(*it)->getCost();
        (*it)->getWay()->getWayType();
        float v = getSpeedFromWayType(tt,(*it)->getWay()->getWayType());
        time += (d/v);
    }
    return time;
}

float PathAlgorithms::distanceToTravelPath(vector<WaySegment*> &path){
    float cost=0.0f;
    for (vector<WaySegment*>::iterator it=path.begin(); it!=path.end(); it++){
        cost +=(*it)->getCost();
    }
    return cost;
}



