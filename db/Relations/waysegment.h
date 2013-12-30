#ifndef WAYSEGMENT_H
#define WAYSEGMENT_H
#include "way.h"
#include "relation.h"


class Node;
class WaySegment : public Relation
{
public:
    WaySegment(Node*, Node*,Way*);
    //Construction used for cloning waysegment
    WaySegment(Node* x, Node* y,WaySegment* ws);
    Way* getWay();
    //Get start and end point
    Node* getPointA();
    Node* getPointB();
    //Distance of this segment
    void setCost(float &);
    float getCost();
    //Checks if we can use this segment with transport specified in tt
    bool isPermitedOnThisRoad(ns_permisions::transport_type &tt);
private:
    Way* W;
    float cost;
};

#endif // WAYSEGMENT_H
