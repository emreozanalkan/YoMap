#include "logic.h"
#include "algorithms/pathalgorithms.h"
#include <iostream>

using namespace std;

Logic::Logic(){

#ifdef __APPLE__
    db.build("/Users/emreozanalkan/QTWorkspace/YoMap/data/LeCreusotWaysFF.osm");
    db.buildPOIs("/Users/emreozanalkan/QTWorkspace/YoMap/data/LeCreusot_POI.xml");
#else
    db.build("C:\\Users\\tuitikki\\Documents\\GitHub\\YoMap\\data\\LeCreusotWaysFF.osm");
    db.buildPOIs("C:\\Users\\tuitikki\\Documents\\GitHub\\YoMap\\data\\LeCreusot_POI.xml");
#endif

}

//void Logic::printmsg(){
//    cout << "Button pressed" << endl;
//}

map<unsigned long int,Way *>* Logic::getAllWays(){
    return db.getAllWays();
}


int Logic::getShortestPath( QPointF &A, QPointF &B, int transportMode, vector<WaySegment*> &path, float &distance, float &time)
{
    //returns 0 if query succeded;
    //1 if A out of bound;
    //2 is B out of bound;
    //3 no path found

    //TransportMode 0 - drive, 1 - walk

    //TODO: check if A, B in bounds.

    //vector<WaySegment*> path;
    //float distance,time;
    boost_xy_point start_point(A.x(),A.y());
    boost_xy_point end_point(B.x(),B.y());
    ns_permisions::transport_type tt;

    if (transportMode == 0) tt = ns_permisions::car;
    else if (transportMode == 1) tt = ns_permisions::foot;

    bool found = PathAlgorithms::findShortestPath(&db, start_point, end_point, tt, path, distance, time);
    if (found) return 0;
    return 3; //no path found
}
