#ifndef NODE_H
#define NODE_H

#include "db_definitions.hpp"
#include <map>

using namespace std;

class WaySegment;
class Relation;
class Node
{
public:
    Node(const unsigned long int & = 0);
    Node(Node*);
    Node(const unsigned long int &, ns_relation::relation_type);
    ~Node();
    //Setters
    void setGeoPosition(const double &, const double &);
    void setMercatorPosition(const float &x, const float &y);
    //Getters
    unsigned long int getId();
    ns_relation::relation_type getType();
    void setType(ns_relation::relation_type);
    boost_xy_point& getGeoPosition();
    boost_xy_point& getMercatorPosition();

//    void addIncomingConnection(WaySegment *);
    void addOutcomingConnection(WaySegment *);
    //OZAN UPDATE
    WaySegment * removeLastOutcomingConnection();
    vector<WaySegment *>::iterator getOutcomingEdgesBegin();
    vector<WaySegment *>::iterator getOutcomingEdgesEnd();



    bool isMemberOf(unsigned long int &);
    bool addRelation(Relation*);
    int getDegree();
    map<unsigned long int, Relation*>::iterator getRelationBegin();
    map<unsigned long int, Relation*>::iterator getRelationEnd();


//    vector<WaySegment *> incoming;

    void setGscore(float g);
    void setFscore(float f);
    void setPrevNodeSegment(WaySegment *n);
    float getGscore();
    float getFscore();
    WaySegment* getPrevNodeSegment();


private:
    unsigned long int id;
    //EPSG:3785
    boost_xy_point geoPosition;
    //EPSG:900913
    boost_xy_point mercatorPosition;
    //container of all relations it belongs to
    map<unsigned long int,Relation *> memberOf;

    //pointer to segments where we can move from this node
    vector<WaySegment *> outcoming;


    //what type of relations it belongs to
    ns_relation::relation_type type;

    //A* algorithm data
    float f_score;
    float g_score;
    //segment by which we came to this point
    WaySegment* prevNodeSegment;

};

#endif // NODE_H
