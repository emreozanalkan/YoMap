#include "waysegment.h"
#include "../../algorithms/pathalgorithms.h"

WaySegment::WaySegment(Node* x, Node* y,Way* z):Relation(0,ns_relation::way_segment)
{
    //startPoint is saved as first point in relation
    insertNode(x);
    //startPoint is saved as second point in relation
    insertNode(y);

    //calculate the cost of the segment-distance between points
    cost = PathAlgorithms::calculateDistancePoints(x->getGeoPosition(),y->getGeoPosition());

    W = z;
}

WaySegment::WaySegment(Node* x, Node* y,WaySegment* ws):Relation(0,ns_relation::way_segment)
{
    //startPoint is saved as first point in relation
    insertNode(x);
    //startPoint is saved as second point in relation
    insertNode(y);

    //calculate the cost of the segment-distance between points
    cost = ws->getCost();

    W = ws->getWay();
}


Way* WaySegment::getWay(){
    return W;
}

Node* WaySegment::getPointA(){
    return getNodeAtPos(0);
}

Node* WaySegment::getPointB(){
    return getNodeAtPos(1);
}

void WaySegment::setCost(float &c){
    cost = c;
}

float WaySegment::getCost(){
    return cost;
}

 bool WaySegment::isPermitedOnThisRoad(ns_permisions::transport_type &tt){
    if(W->getAccessType()==ns_access::privat)
        return false;

    switch(tt){
        case ns_permisions::car:
            switch(W->getWayType()){
            case ns_way::raceway:
                return false;
                break;
            case ns_way::footway:
                return false;
                break;
            case ns_way::steps:
                return false;
                break;
            }
            break;
        case ns_permisions::foot:
            break;
    }
    return true;
}


