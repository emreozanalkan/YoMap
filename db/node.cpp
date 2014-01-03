#include "node.h"
#include "Relations/relation.h"

/*
 * Constructors of class Node
 */
Node::Node(const unsigned long int &i)
{
    id = i;
    type=ns_relation::undefined;
}
Node::Node(Node* n)
{
    //we clone node n
    id = n->id;
    type=n->type;
    setGeoPosition(n->getGeoPosition().x(),n->getGeoPosition().y());
}
Node::~Node()
{
    //We go through all waysements which go from this node and delete them
    vector<WaySegment*>::iterator it = outcoming.begin();
    for(;it!=outcoming.end();it++){
        delete *it;
    }
}
unsigned long int Node::getId(){
    //return the ID of the node
    return id;
}
//Type of relations it belong to
ns_relation::relation_type Node::getType(){
    return type;
}

void Node::setType(ns_relation::relation_type t){
    type = t;
}

bool Node::isMemberOf(unsigned long int &i){
    if(memberOf.find(i)==memberOf.end()){
        return false;
    }
    else{
        return true;
    }
}

bool Node::addRelation(Relation* r){
    if(r->getRelationType()==type){
        memberOf.insert(make_pair<unsigned long int,Relation *>(r->getId(),r));
        return true;
    }
    return false;
}

int Node::getDegree(){
    return memberOf.size();
}

map<unsigned long int, Relation*>::iterator Node::getRelationBegin(){
    return memberOf.begin();
}

map<unsigned long int, Relation*>::iterator Node::getRelationEnd(){
    return memberOf.end();
}

//Location of node
boost_xy_point& Node::getGeoPosition(){
    return geoPosition;
}

void Node::setGeoPosition(const double &x, const double &y){
    geoPosition.x(x);
    geoPosition.y(y);
}


//WaySegments which come out of this point
void Node::addOutcomingConnection(WaySegment *ns){
    outcoming.push_back(ns);
}
WaySegment * Node::removeLastOutcomingConnection(){
    WaySegment* s = outcoming.back();
    outcoming.pop_back();
    return s;
}
vector<WaySegment *>::iterator Node::getOutcomingEdgesBegin(){
    return outcoming.begin();
}
vector<WaySegment *>::iterator Node::getOutcomingEdgesEnd(){
    return outcoming.end();
}

//A* algorithm
void Node::setGscore(float g){
    g_score=g;
}
void Node::setFscore(float f){
    f_score=f;
}
float Node::getGscore(){
    return g_score;
}

float Node::getFscore(){
    return f_score;
}
void Node::setPrevWaySegment(WaySegment* n){
    prevNodeSegment = (Relation*)n;
}

WaySegment* Node::getPrevWaySegment(){
    return (WaySegment*)prevNodeSegment;
}

//POI
void Node::setPOI(POIPoint* p){
    poi = p;
}
POIPoint* Node::getPOI(){
    return poi;
}


