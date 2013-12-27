#ifndef PATHALGORITHMS_H
#define PATHALGORITHMS_H
#include "../db/node.h"
#include "../db/database.h"

class PathAlgorithms
{
public:
    static float EarthRadius;
    static double pi;

    //  This function converts decimal degrees to radians
    static double deg2rad(double deg);
    //  This function converts radians to decimal degrees
    static double rad2deg(double rad);

    static float calculateDistancePoints(boost_xy_point &,boost_xy_point&);
    static float calculateDistancePoints(Node *, Node *);

    static boost_xy_point& projectPointToSegment(boost_xy_point &A,boost_xy_point &B, boost_xy_point &C);
    static bool findClosestWaySegment(Database *d,boost_xy_point &A,ns_permisions::transport_type &tt, WaySegment* &closestSegment, boost_xy_point &pointOfContact);


    static bool findShortestPath(Node* start, Node* end,ns_permisions::transport_type &tt,vector<WaySegment*> &best_path);
    static bool findShortestPath(Database *d,unsigned long int A, unsigned long int B,ns_permisions::transport_type &tt, vector<WaySegment*> &path,float &total_cost, float &total_time);
    static bool findShortestPath(Database *d,boost_xy_point &A, boost_xy_point &B,ns_permisions::transport_type &tt, vector<WaySegment*> &path,float &total_cost, float &total_time);
    static bool findShortestPath(Node* &node_start,WaySegment* &start_segment, Node* &node_end, WaySegment* &end_segment, ns_permisions::transport_type &tt, vector<WaySegment *> &best_path);

    static void constructBackShortestPath(vector<WaySegment*> &,Node* element, Node* start);
    static void safelyDeletePath(vector<WaySegment*> &path);
    static float timeToTravelPath(ns_permisions::transport_type &tt, vector<WaySegment*> &path);
    static float distanceToTravelPath(vector<WaySegment*> &path);

private:
    PathAlgorithms();
};
#endif // PATHALGORITHMS_H
