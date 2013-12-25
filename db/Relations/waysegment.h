#ifndef WAYSEGMENT_H
#define WAYSEGMENT_H
#include "way.h"
#include "relation.h"
#include "../node.h"

class WaySegment : public Relation
{
public:
    WaySegment(Node*, Node*,Way*);
    WaySegment(Node* x, Node* y,WaySegment* ws);
    Way* getWay();
    Node* getPointA();
    Node* getPointB();
    void setCost(float &);
    float getCost();
    bool isPermitedOnThisRoad(ns_permisions::transport_type &tt);
private:
    Way* W;
    float cost;
};

#endif // WAYSEGMENT_H
