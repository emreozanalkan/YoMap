#include "database.h"
#include "db_definitions.hpp"
#include "commonFunctions.hpp"
#include <iostream>
#include "../algorithms/pathalgorithms.h"
using namespace std;
using namespace rapidxml;

Database::Database()
{
}
Database::Database(string path)
{
    build(path);
}
Database::~Database()
{
    cout <<" Delete constructor"<<endl;
    //Delete all ways
    map<unsigned long int,Way *>::iterator it = all_ways.begin();
    for(;it!=all_ways.end();it++){
        delete it->second;
    }

    //delete all nodes -> deletes all waysegments
    map<unsigned long int,Node *>::iterator itn = all_nodes.begin();
    for(;itn!=all_nodes.end();it++){
        delete itn->second;
    }

}
void Database::build(string path){
    //code partially taken from example at http://rapidxml.sourceforge.net/manual.html

    cout << "Building data structure..." << endl;

    //XML PARSER
    xml_document<> doc;
    xml_node<> * root_node;

    // Read the xml file into a vector
    ifstream theFile(path.c_str());
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);

    // Find our root node
    root_node = doc.first_node("osm");
    unsigned long int id;

    // Iterate over the all the nodes
    for (xml_node<> * node = root_node->first_node(); node; node = node->next_sibling())
    {
        string node_name(node->name());

        //First we check if it is a node or relation
        if(node_name=="node"){
            getValueFromString( node->first_attribute("id")->value(), id );

            //Create new Node
            Node *n = new Node(id);
            double lat,lon;
            getValueFromString( node->first_attribute("lat")->value(), lat );
            getValueFromString( node->first_attribute("lon")->value(), lon );
            n->setGeoPosition(lon,lat);
            //Insert node in vector of all nodes
            insertNewNode(n);
        }
        else if(node_name=="way"){
            //It is somekind of relation

            //Figure out what kind of relation
            for(xml_node<> * way_node = node->first_node("tag"); way_node; way_node = way_node->next_sibling())
            {
                string tag_type(way_node->first_attribute("k")->value());
                if(tag_type.compare("highway")==0){
                    //Its a way
                    Relation * rel = processWay(node,string(way_node->first_attribute("v")->value()));
                    insertNewWay((Way*)rel);

                }
                else if(tag_type.compare("building")==0 && string(way_node->first_attribute("v")->value()).compare("yes")==0){
                    //its a building
                }
            }
        }


    }


}

Relation * Database::processWay(xml_node<> * node, string t){
    unsigned long int id;
     unsigned long int idA;
    getValueFromString( node->first_attribute("id")->value(), id );
    getValueFromString( node->first_attribute("id")->value(), idA );

    Relation * rel = new Way(id);
    ((Way*)rel)->setWayType(getWayTypeFromString(t));

    for(xml_node<> * way_node = node->first_node(); way_node; way_node = way_node->next_sibling())
    {
        string sub_name(way_node->name());
        if(sub_name=="nd"){
            getValueFromString( way_node->first_attribute("ref")->value(), id );
            Node * n = getNodeById(id);
            ((Way*)rel)->insertNode(n);
            n->addRelation(rel);
        }
        else if(sub_name=="tag"){
            string tag_type(way_node->first_attribute("k")->value());
            if(tag_type=="oneway"){
                ((Way*)rel)->setOnewayType(getOnewayTypeFromString(string(way_node->first_attribute("v")->value())));
            }
            else if(tag_type=="access"){
                ((Way*)rel)->setAccessType(getAccessTypeFromString(string(way_node->first_attribute("v")->value())));
            }
            else if(tag_type=="name"){
                ((Way*)rel)->setStreetName(string(way_node->first_attribute("v")->value()));
            }
            else if(tag_type=="junction" && string(way_node->first_attribute("v")->value())=="roundabout"){
                ((Way*)rel)->setAsRoundabout(true);
            }
            else if(tag_type=="bridge" && string(way_node->first_attribute("v")->value())=="true"){
                ((Way*)rel)->setAsBridge(true);
            }

        }
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
