#include "node.h"
#include "Relations/relation.h"


Node::Node(const unsigned long int &i)
{
    id = i;
    type=ns_relation::undefined;
}
Node::Node(Node* n)
{
    id = n->id;
    type=n->type;
    setGeoPosition(n->getGeoPosition().x(),n->getGeoPosition().y());
    setMercatorPosition(n->getMercatorPosition().x(),n->getMercatorPosition().y());
}
Node::Node(const unsigned long int &i, ns_relation::relation_type r)
{
    id = i;
    type = r;
}
Node::~Node()
{
    vector<WaySegment*>::iterator it = outcoming.begin();
    for(;it!=outcoming.end();it++){
        delete *it;
    }
}
unsigned long int Node::getId(){
    return id;
}
ns_relation::relation_type Node::getType(){
    return type;
}
void Node::setType(ns_relation::relation_type t){
    type = t;
}
boost_xy_point& Node::getGeoPosition(){
    return geoPosition;
}
boost_xy_point& Node::getMercatorPosition(){
    return mercatorPosition;
}

void Node::setGeoPosition(const double &x, const double &y){
    geoPosition.x(x);
    geoPosition.y(y);
}

void Node::setMercatorPosition(const float &x, const float &y){
    mercatorPosition.x(x);
    mercatorPosition.y(y);
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

//void Node::addIncomingConnection(WaySegment *ns){
//    incoming.push_back(ns);
//}
void Node::addOutcomingConnection(WaySegment *ns){
    outcoming.push_back(ns);
}
//OZAN UPDATE
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

void Node::setPrevNodeSegment(WaySegment* n){
    prevNodeSegment = n;
}

WaySegment* Node::getPrevNodeSegment(){
    return prevNodeSegment;
}




