#ifndef NODE_H
#define NODE_H

#include "db_definitions.hpp"
#include "poi/poipoint.h"

#include "Relations/relation.h"
#include "Relations/waysegment.h"

#include <map>

using namespace std;

class Node
{
public:
    //Constructors
    Node(const unsigned long int & = 0);
    Node(Node*);
    //Deconstructor
    ~Node();

    unsigned long int getId();
    //Type of the relation in which node is
    ns_relation::relation_type getType();
    void setType(ns_relation::relation_type);

    //Location
    void setGeoPosition(const double &, const double &);
    boost_xy_point& getGeoPosition();

    //Ways which it belongs to
    bool isMemberOf(unsigned long int &);
    bool addRelation(Relation*);
    int getDegree();
    map<unsigned long int, Relation*>::iterator getRelationBegin();
    map<unsigned long int, Relation*>::iterator getRelationEnd();

    //Outgoing connections
    void addOutcomingConnection(WaySegment *);
    WaySegment * removeLastOutcomingConnection();
    vector<WaySegment *>::iterator getOutcomingEdgesBegin();
    vector<WaySegment *>::iterator getOutcomingEdgesEnd();

    //A* algorithm weights
    void setGscore(float g);
    float getGscore();
    void setFscore(float f);
    float getFscore();
    void setPrevWaySegment(WaySegment *n);
    WaySegment* getPrevWaySegment();

    //POI
    void setPOI(POIPoint*);
    POIPoint* getPOI();

private:
    unsigned long int id;
    //Location
    boost_xy_point geoPosition;

    //what type of relations it belongs to
    ns_relation::relation_type type;
    //container of all relations it belongs to
    map<unsigned long int,Relation *> memberOf;
    //pointer to segments where we can move from this node
    vector<WaySegment *> outcoming;

    //A* algorithm data
    float f_score;
    float g_score;
    //segment by which we came to this point
    Relation* prevNodeSegment;

    //Possible POIPoint
    POIPoint* poi;

};

#endif // NODE_H
