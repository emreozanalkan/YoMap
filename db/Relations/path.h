#ifndef PATH_H
#define PATH_H
#include <vector>
#include "pathsegment.h"

class Path
{
public:
    Path();
    ~Path();
    void addSegment(PathSegment* &);
    vector<PathSegment*>::iterator getPathSegmentsEnd();
    vector<PathSegment*>::iterator getPathSegmentsBegin();
    float calculateCost();
    float getCost();
    float getTravelTime(ns_permisions::transport_type &);
    bool isEmpty();
    vector<PathSegment*> segments;
private:
    float cost;
};

#endif // PATH_H
