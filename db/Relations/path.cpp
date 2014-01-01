#include "path.h"

Path::Path()
{
    cost = 0.0f;
}
Path::~Path(){
//    for(vector<PathSegment*>::iterator it = segments.begin();it!=segments.end();it++){
//        delete (*it);
//    }
}

bool Path::isEmpty(){
    if(segments.empty())
        return true;
    return false;
}
void Path::addSegment(PathSegment* &ps){
    segments.push_back(ps);
    cost += ps->calculateCost();
}
void Path::addSegments(Path &p){
    for(vector<PathSegment*>::iterator it = p.segments.begin();it!=p.segments.end();it++){
        addSegment(*it);
    }
}
vector<PathSegment*>::iterator Path::getPathSegmentsBegin(){
    return segments.begin();
}
vector<PathSegment*>::iterator Path::getPathSegmentsEnd(){
    return segments.end();
}

float Path::calculateCost(){
    cost=0.0f;
    for (vector<PathSegment*>::iterator it=segments.begin(); it!=segments.end(); it++){
        cost +=(*it)->calculateCost();
    }
    return cost;
}
float Path::getCost(){
    return cost;
}


float Path::getTravelTime(ns_permisions::transport_type &tt){
    float time=0.0f;
    for (vector<PathSegment*>::iterator it=segments.begin(); it!=segments.end(); it++){
        float t=(*it)->getTravelTime(tt);
        if(t==0.0)
            return 0;
        time += t;
    }
    return time;
}
