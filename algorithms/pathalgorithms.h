#ifndef PATHALGORITHMS_H
#define PATHALGORITHMS_H
#include "../db/node.h"
#include "../db/database.h"
#include "../db/Relations/path.h"
#include <set>

class CompareNodes
{
public:
    bool operator()(Node* l, Node* r) {return l->getFscore() > r->getFscore();}
};

class ComparePaths
{
public:
    bool operator()(Path* l, Path* r) {return l->getCost() < r->getCost();}
};

class PathAlgorithms
{
public:
    static float EarthRadius;
    static double pi;

    //  This function converts decimal degrees to radians
    static double deg2rad(const double &);
    //  This function converts radians to decimal degrees
    static double rad2deg(const double &);

    //Calculate air distance between two points
    static float calculateDistancePoints(boost_xy_point &, boost_xy_point &);
    static float calculateDistancePoints(Node* &, Node* &);

    //Get projection of a point on a line between two other points
    static boost_xy_point& projectPointToSegment(boost_xy_point &,boost_xy_point &, boost_xy_point &);

    //Find closest way segment in db and point where point A would be projected to
    static bool findClosestWaySegment(Database &,boost_xy_point &,ns_permisions::transport_type &, WaySegment* &, boost_xy_point &);

    /*
     *  SHORTEST PATH ALGORITHM A->B
     */
    //Constructs back the best path by back-traversing nodes
    static void constructBackShortestPath(PathSegment* &,Node* &, Node* &);
    //Find shortest path between two nodes spcified by pointers to nodes
    static PathSegment*& findShortestPath(Node* &, Node* &,ns_permisions::transport_type &);
    //Find shortest path between two nodes specified by id from database
    static int findShortestPath(Database &,unsigned long int &, unsigned long int &,ns_permisions::transport_type &,Path &);
    //Find shortest path between two lon-lat points
    static int findShortestPath(Database &,boost_xy_point &, boost_xy_point &,ns_permisions::transport_type &,Path &);
    //Find shortest path between two points on map with specified closest waysegments
    static PathSegment*& findShortestPath(Node* &,WaySegment* &, Node* &, WaySegment* &, ns_permisions::transport_type &);

    /*
     *  SHORTEST PATH ALGORITHM A -> all POI (of category) in radius
     */
    static int findPathsInRadius(Database &, boost_xy_point &, POICategory* &, float &, ns_permisions::transport_type &,set<Path*,ComparePaths> &);


private:
    PathAlgorithms();
};
#endif // PATHALGORITHMS_H
