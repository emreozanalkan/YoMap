#ifndef PATHSEGMENT_H
#define PATHSEGMENT_H
#include "relation.h"
#include "waysegment.h"
#include "../node.h"
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
    void setCost(float &);
    float getTravelTime(ns_permisions::transport_type &);
    bool isEmpty();
    void setStartEnd(Node* &start, Node* &end);
    Node* getStartNode();
    Node* getEndNode();
    vector<WaySegment*> segments;
private:
    float cost;
    Node* startNode;
    Node* endNode;
};

#endif // PATHSEGMENT_H
