#include "database.h"
#include "db_definitions.hpp"
#include "commonFunctions.hpp"
#include <iostream>

#include "../algorithms/pathalgorithms.h"

#include "poi/poicategory.h"
#include "poi/poipoint.h"

#include <iomanip>

using namespace std;

Database::Database()
{
}
Database::Database(QString path)
{
    build(path);
}
Database::~Database()
{
    //Delete all buildings
    for(map<unsigned long int,Building *>::iterator it = all_buildings.begin();it!=all_buildings.end();it++){
        delete it->second;
    }
    //Delete all ways
    for(map<unsigned long int,Way *>::iterator it = all_ways.begin();it!=all_ways.end();it++){
        delete it->second;
    }

    //delete all nodes -> deletes all waysegments
    for(map<unsigned long int,Node *>::iterator it = all_nodes.begin();it!=all_nodes.end();it++){
        delete it->second;
    }

    //delete all POI categories
    for(map<unsigned int,POICategory *>::iterator it = all_poi_categories.begin();it!=all_poi_categories.end();it++){
        delete it->second;
    }

    //delete all POI categories
    for(map<unsigned int,POIPoint *>::iterator it = all_poi_points.begin();it!=all_poi_points.end();it++){
        delete it->second;
    }

}
bool Database::isWaysBuild(){
    return ways_build;
}

bool Database::isPOIBuild(){
    return poi_build;
}

boost_xy_point Database::getMapMinBound()
{
    return min_bound;
}

boost_xy_point Database::getMapMaxBound()
{
    return max_bound;
}

int Database::build(QString path){
    ways_build = false;
    QFile file(path);
    cout << "Building data structure..." << endl;

    QDomDocument doc( "OSM" );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ways_build = false;
        return -1;
    }

    if( !doc.setContent( &file ) )
     {
       file.close();
       ways_build = false;
       return -2;
     }

    QDomElement root = doc.documentElement();
    if( root.tagName() != "osm" )
      return -2;

    QDomNode n = root.firstChild();
     while( !n.isNull() )
     {
           QDomElement e = n.toElement();
           if( !e.isNull() )
           {
             if( e.tagName() == "bounds" )
             {
                 if(e.hasAttribute("minlat") && e.hasAttribute("minlon") && e.hasAttribute("maxlat") && e.hasAttribute("maxlon")){

                     double min_lat = e.attribute("minlat").toDouble();
                     double min_lon = e.attribute("minlon").toDouble();
                     double max_lat = e.attribute("maxlat").toDouble();
                     double max_lon = e.attribute("maxlon").toDouble();
                     setBounds(min_lon,min_lat,max_lon,max_lat);
                 }
             }
             else if(e.tagName() == "node"){
                if(e.hasAttribute("id")){
                    unsigned long int id;
                    getValueFromString( e.attribute("id").toStdString(), id );

                    Node *n = new Node(id);

                    if(e.hasAttribute("lat") && e.hasAttribute("lon")){
                        double lat = e.attribute("lat").toDouble();
                        double lon = e.attribute("lon").toDouble();
                        n->setGeoPosition(lon,lat);
                    }
                        //Insert node in vector of all nodes
                        insertNewNode(n);
                }
             }
             else if(e.tagName() == "way"){
                 if(e.hasAttribute("id")) {
                     QDomElement elem = e.firstChildElement("tag");
                     while( !elem.isNull() )
                     {
                         if(elem.attribute("k")=="highway"){
                             //It is a road
                             Relation * rel = processWay(e,elem.attribute("v").toStdString());
                             insertNewWay((Way*)rel);
                             break;
                         }
                         else if(elem.attribute("k")=="building" && elem.attribute("v")=="yes"){
                             //It is a building
                             Relation * building = processBuilding(e,elem.attribute("v").toStdString());
                             insertNewBuilding((Building*)building);
                             break;
                         }

                         elem = elem.nextSiblingElement("tag");
                     }
                 }
             }
           }
           n = n.nextSibling();
     }
     file.close();
     ways_build = true;
     return 0;
}



int Database::buildPOIs(QString path){
    poi_build = false;
    QFile file(path);
    cout << "Building POI structure..." << endl;

    QDomDocument doc( "POI" );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        poi_build = false;
        return -1;
    }

    if( !doc.setContent( &file ) )
     {
       file.close();
       poi_build = false;
       return -2;
     }

    QDomElement root = doc.documentElement();
    if( root.tagName() != "data" )
      return -2;

    QDomNode n = root.firstChild();
     while( !n.isNull() )
     {
         QDomElement e = n.toElement();
         if( !e.isNull() )
         {
           if( e.tagName() == "category" )
           {
               unsigned int id;
               getValueFromString( e.attribute("id").toStdString(), id );
               //Create new category
               POICategory *poi_c = new POICategory(id,e.attribute("name").toStdString(),e.attribute("icon").toStdString());
               insertNewPOICategory(poi_c);
           }
           else if(e.tagName() == "poi"){
               unsigned int id,cat_id;               
               getValueFromString( e.attribute("id").toStdString(), id );
               getValueFromString( e.attribute("cat_id").toStdString(), cat_id );

               POICategory *p_cat = getPOICategoryById(cat_id);
               //Check if category exists
               if(p_cat!=0){

                   double lat = e.attribute("lat").toDouble();
                   double lon = e.attribute("lon").toDouble();
                   POIPoint *poi = new POIPoint(id,lon,lat,e.attribute("name").toStdString(),e.attribute("addr").toStdString(),e.attribute("photo").toStdString(),e.attribute("user").toStdString());
                   poi->setCategory(p_cat);
                   p_cat->addPOI(poi);
                   insertNewPOIPoint(poi);
               }
           }
         }
         n = n.nextSibling();
     }
    file.close();
    poi_build = true;
    return 0;

}

int Database::savePOIs(QString path){
    QFile file(path);
    cout << "Saving POI structure..." << endl;

    /*open a file */
    if (!file.open(QIODevice::WriteOnly))
    {
    /* show wrror message if not able to open file */
        //QMessageBox::warning(0, "Read only", "The file is in read only mode");
    }
    else
    {
        /*if file is successfully opened then create XML*/
        QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
        xmlWriter->setDevice(&file);
        xmlWriter->setAutoFormatting(true);
        xmlWriter->writeStartDocument();
        /* Writes a start element with name,*/
        xmlWriter->writeStartElement("data");

        //Write all the category data
        for(map<unsigned int,POICategory *>::iterator it_cat = all_poi_categories.begin();it_cat!=all_poi_categories.end();it_cat++){
             xmlWriter->writeStartElement("category");
             xmlWriter->writeAttribute("id", QString::number(it_cat->second->getId()));
             xmlWriter->writeAttribute("name", QString::fromStdString(it_cat->second->getName()));
             xmlWriter->writeAttribute("icon", QString::fromStdString(it_cat->second->getIcon()));
             xmlWriter->writeEndElement();
        }
        //Write all poi data
        for(map<unsigned int,POIPoint *>::iterator it_cat = all_poi_points.begin();it_cat!=all_poi_points.end();it_cat++){
             xmlWriter->writeStartElement("poi");
             xmlWriter->writeAttribute("id", QString::number(it_cat->second->getId()));
             xmlWriter->writeAttribute("cat_id", QString::number(it_cat->second->getCategory()->getId()));
             xmlWriter->writeAttribute("lat", QString::number(it_cat->second->getGeoPosition().y(),'f',6));
             xmlWriter->writeAttribute("lon", QString::number(it_cat->second->getGeoPosition().x(),'f',6));
             xmlWriter->writeAttribute("name", QString::fromStdString(it_cat->second->getName()));
             xmlWriter->writeAttribute("addr", QString::fromStdString(it_cat->second->getAddress()));
             xmlWriter->writeAttribute("photo", QString::fromStdString(it_cat->second->getPhotoPath()));
             xmlWriter->writeAttribute("user", QString::fromStdString(it_cat->second->getUser()));
             xmlWriter->writeEndElement();
        }
        /*end data tag
        xmlWriter->writeEndElement();
        /*end document */
        xmlWriter->writeEndDocument();
               delete xmlWriter;
    }
    file.close();
    return 0;
}

void Database::setBounds(double &min_lon, double &min_lat, double &max_lon, double &max_lat){
    min_bound.x(min_lon);
    min_bound.y(min_lat);
    max_bound.x(max_lon);
    max_bound.y(max_lat);
}

bool Database::checkIfInBoundsOfMap(boost_xy_point p){
    if(p.x()>=min_bound.x() && p.x()<=max_bound.x()){
        if(p.y()>=min_bound.x() && p.y()<=max_bound.y()){
            return true;
        }
    }
    return false;
}


Relation * Database::processWay(QDomElement &e, string t){
    unsigned long int id;
    getValueFromString(e.attribute("id").toStdString(), id );

    Relation * rel = new Way(id);
    ((Way*)rel)->setWayType(getWayTypeFromString(t));

    QDomElement child = e.firstChildElement();
    while(!child.isNull()){
        if(child.tagName()=="nd"){
            getValueFromString( child.attribute("ref").toStdString(), id );
            Node * n = getNodeById(id);
            ((Way*)rel)->insertNode(n);
            n->addRelation(rel);
        }
        else if(child.tagName()=="tag"){
            string tag_type = child.attribute("k").toStdString();
            if(tag_type=="oneway"){
                ((Way*)rel)->setOnewayType(getOnewayTypeFromString(child.attribute("v").toStdString()));
            }
            else if(tag_type=="access"){
                ((Way*)rel)->setAccessType(getAccessTypeFromString(child.attribute("v").toStdString()));
            }
            else if(tag_type=="name"){
                ((Way*)rel)->setStreetName(child.attribute("v").toStdString());
            }
            else if(tag_type=="junction" && child.attribute("v")=="roundabout"){
                ((Way*)rel)->setAsRoundabout(true);
            }
            else if(tag_type=="bridge" && child.attribute("v")=="true"){
                ((Way*)rel)->setAsBridge(true);
            }
        }
        child = child.nextSiblingElement();
    }
    return rel;
}
Relation * Database::processBuilding(QDomElement &e, string t){
    unsigned long int id;
    getValueFromString(e.attribute("id").toStdString(), id );

    Relation * rel = new Building(id);

    QDomElement child = e.firstChildElement();
    while(!child.isNull()){
        if(child.tagName()=="nd"){
            getValueFromString( child.attribute("ref").toStdString(), id );
            Node * n = getNodeById(id);
            ((Building*)rel)->insertNode(n);
        }
        child = child.nextSiblingElement();
    }
    return rel;
}
void Database::insertNewWay(Way *w){
    all_ways.insert(make_pair<unsigned long int,Way *>(w->getId(),w));


    //Go through all the nodes and create waySegments for each pair
    int n_nodes = w->getNumberOfNodes();
    for(int i=0;i<n_nodes-1;i++){
        Relation *wsA;
        switch(w->getOnewayType()){
        case ns_oneway::yes:
        {
            //oneway i->i+1
            wsA = new WaySegment(w->getNodeAtPos(i),w->getNodeAtPos(i+1),w);
            w->getNodeAtPos(i)->addOutcomingConnection((WaySegment*)wsA);
        }
            break;
        case ns_oneway::opposite:
        {
            //oneway i+1->i
            wsA = new WaySegment(w->getNodeAtPos(i+1),w->getNodeAtPos(i),w);
            w->getNodeAtPos(i+1)->addOutcomingConnection((WaySegment*)wsA);
        }
            break;
        default:
        {
            //Bothways street
            //junction is also one way
            if(w->isRoundabout()){
                //i->i+1
                wsA = new WaySegment(w->getNodeAtPos(i),w->getNodeAtPos(i+1),w);
                w->getNodeAtPos(i)->addOutcomingConnection((WaySegment*)wsA);
            }
            else{
                //i->i+1

                wsA = new WaySegment(w->getNodeAtPos(i),w->getNodeAtPos(i+1),w);
                w->getNodeAtPos(i)->addOutcomingConnection((WaySegment*)wsA);
                //i+1->i
                Relation *wsB = new WaySegment(w->getNodeAtPos(i+1),w->getNodeAtPos(i),w);
                w->getNodeAtPos(i+1)->addOutcomingConnection((WaySegment*)wsB);
            }
        }
            break;
        }

        //Create element for RTREE

        //Create bounding box
        bg::model::segment<boost_xy_point> s(w->getNodeAtPos(i)->getGeoPosition(),w->getNodeAtPos(i+1)->getGeoPosition());
        bg::model::box<boost_xy_point> box;
        bg::envelope(s,box);

        //insert into rtree
        rtree_element rte = std::make_pair(box,wsA);
        rtree.insert(rte);

    }

}
void Database::insertNewBuilding(Building *b){
    all_buildings.insert(make_pair<unsigned long int,Building *>(b->getId(),b));
}


void Database::insertNewNode(Node *n){
    all_nodes.insert(make_pair<unsigned long int,Node *>(n->getId(),n));
}

Node* Database::getNodeById(unsigned long int i){
    map<unsigned long int,Node *>::iterator it = all_nodes.find(i);
    if(it!=all_nodes.end()){
        return it->second;
    }
    return 0;
}
Way* Database::getWayById(unsigned long int i){
    map<unsigned long int,Way *>::iterator it = all_ways.find(i);
    if(it!=all_ways.end()){
        return it->second;
    }
    return 0;
}

//Check if segment is also permited to use by transport type we define
void Database::searchWaySegmentsInArea(boost_xy_point min,boost_xy_point max,vector<WaySegment*> &objects_in_area,ns_permisions::transport_type &tt){
    bg::model::box<boost_xy_point > searchBox(min,max);
    std::vector<rtree_element> search_results;

    //we query the rtree
    rtree.query(bg::index::intersects(searchBox),std::back_inserter(search_results));

    //Parse the results to get only way segments
    for(unsigned int i=0;i<search_results.size();i++){
        if(search_results.at(i).second->getRelationType()==ns_relation::way_segment && (tt==ns_permisions::any || ((WaySegment*)search_results.at(i).second)->isPermitedOnThisRoad(tt))){
            objects_in_area.push_back((WaySegment*)search_results.at(i).second);
        }
    }
}

map<unsigned long int,Way *>* Database::getAllWays(){
    return &all_ways;
}



void Database::insertNewPOICategory(POICategory *poi_c){
    all_poi_categories.insert(make_pair<unsigned int,POICategory *>(poi_c->getId(),poi_c));
}
void Database::insertNewPOIPoint(POIPoint *poi_p){
    all_poi_points.insert(make_pair<unsigned int,POIPoint *>(poi_p->getId(),poi_p));
}
POICategory* Database::getPOICategoryById(unsigned int i){
    map<unsigned int,POICategory *>::iterator it = all_poi_categories.find(i);
    if(it!=all_poi_categories.end()){
        return it->second;
    }
    return 0;
}

POIPoint* Database::getPOIPointByPosition(unsigned int cat_id,unsigned int point_pos){
    POICategory* p_cat = getPOICategoryById(cat_id);

    if(p_cat!=0){
        return p_cat->getPOIPointAt(point_pos);
    }
    return 0;
}

map<unsigned int,POICategory *>* Database::getCategoryCatalog(){
    return &all_poi_categories;
}


vector<POIPoint*> Database::getPOIPointsInCategories(vector<unsigned int> &cat_ids){
    vector<POIPoint*> poi_points;
    for (vector<unsigned int>::iterator it = cat_ids.begin(); it!=cat_ids.end();it++){
        POICategory* cat_current = getPOICategoryById(*it);
        poi_points.insert(poi_points.end(),cat_current->getPOIPointsBegin(),cat_current->getPOIPointsEnd());
    }
    return poi_points;
}

vector<POIPoint*> Database::getPOIPointsInCategories(){
    vector<POIPoint*> poi_points;
    for(map<unsigned int,POIPoint *>::iterator it = all_poi_points.begin(); it!=all_poi_points.end();it++){
        poi_points.push_back(it->second);
    }
    return poi_points;
}




