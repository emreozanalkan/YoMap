#ifndef PATHSEGMENT_H
#define PATHSEGMENT_H
#include "relation.h"
#include "waysegment.h"
#include <vector>

class PathSegment : public Relation
{
public:
    PathSegment(unsigned long int);
    ~PathSegment();
    void addSegment(WaySegment* &);
    vector<WaySegment*>::iterator getWaySegmentsBegin();
    vector<WaySegment*>::iterator getWaySegmentsEnd();
    float calculateCost();
    float getCost();
    float getTravelTime(ns_permisions::transport_type &);
    bool isEmpty();
    vector<WaySegment*> segments;
private:
    float cost;
};

#endif // PATHSEGMENT_H
