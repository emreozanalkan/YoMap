#include "pathsegment.h"

PathSegment::PathSegment(unsigned long int i):Relation(i,ns_relation::path_segment)
{
    cost = 0.0f;
}
PathSegment::~PathSegment(){
    for(vector<WaySegment*>::iterator it = segments.begin();it!=segments.end();it++){
        delete (*it)->getPointA();
        delete (*it)->getPointB();
        delete (*it);
    }
}
bool PathSegment::isEmpty(){
    return segments.empty();
}

void PathSegment::addSegment(WaySegment* &ws){
    segments.push_back(ws);
    cost+=ws->getCost();
}
vector<WaySegment*>::iterator PathSegment::getWaySegmentsBegin(){
    return segments.begin();
}
vector<WaySegment*>::iterator PathSegment::getWaySegmentsEnd(){
    return segments.end();
}
float PathSegment::calculateCost(){
    //Go through all segments and sum for the cost of the path segment
    cost=0.0f;
    for (vector<WaySegment*>::iterator it=segments.begin(); it!=segments.end(); it++){
        cost +=(*it)->getCost();
    }
    return cost;
}
float PathSegment::getCost(){
    return cost;
}

//Calculate travel time according to means of transport
float PathSegment::getTravelTime(ns_permisions::transport_type &tt){
    float time=0.0f;
    for (vector<WaySegment*>::iterator it=segments.begin(); it!=segments.end(); it++){
        float d=(*it)->getCost();
        (*it)->getWay()->getWayType();
        float v = getSpeedFromWayType(tt,(*it)->getWay()->getWayType());
        //if speed is 0 we return as 0 (it would be inf);
        if(v==0)
            return 0;
        time += (d/v);
    }
    return time;
}


