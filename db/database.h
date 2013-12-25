#ifndef DATABASE_H
#define DATABASE_H
#include "db_definitions.hpp"

#include "node.h"
#include "Relations/waysegment.h"
#include "Relations/way.h"
#include "Relations/relation.h"
#include "libs/RapidXML/rapidxml.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace rapidxml;

class Database
{
public:
    Database();
    Database(string path);
    ~Database();
    void build(string path);

    Node* getNodeById(unsigned long int);
    Way* getWayById(unsigned long int);
    map<unsigned long int,Way *>* getAllWays();

    void searchWaySegmentsInArea(boost_xy_point min,boost_xy_point max,vector<WaySegment*> &objects_in_area,ns_permisions::transport_type &tt);

    bool findShortestWay(unsigned long int A, unsigned long int B, vector<WaySegment*> &path,float &total_cost);
    bool findShortestWay(boost_xy_point &A, boost_xy_point &B, vector<WaySegment*> &path,float &total_cost);
    bool findClosestWaySegment(boost_xy_point &A, WaySegment* closestSegment, boost_xy_point &pointOfContact);
    boost_xy_point&  projectPointToSegment(boost_xy_point &A,boost_xy_point &B, boost_xy_point &C);
private:
    //containers of all nodes by type
    map<unsigned long int,Node *> all_nodes;

    //containers of all relations by type
    map<unsigned long int,Way *> all_ways;


    //Rtree with all rtree elements
    boost_rtree rtree;

    Relation * processWay(xml_node<> *, string);
    void insertNewWay(Way *);
    void insertNewNode(Node *);
};

#endif // DATABASE_H

