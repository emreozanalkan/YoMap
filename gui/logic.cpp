#include "logic.h"
#include "algorithms/pathalgorithms.h"
#include <iostream>

using namespace std;

Logic::Logic(){
    db.build(":/data/ways.osm");
    db.buildPOIs(":/data/POI.xml");
}

bool Logic::checkIfWaysDBIsBuild(){
    return db.isWaysBuild();
}
bool Logic::checkIfPOIDBIsBuild(){
    return db.isPOIBuild();
}

map<unsigned long int,Way *>* Logic::getAllWays(){
    return db.getAllWays();
}

map<unsigned int,POICategory *>* Logic::getCategoryCatalog(){
    return db.getCategoryCatalog();
}

vector<POIPoint*> Logic::getPOIPointsInCategories(vector<unsigned int> &cat_ids){
    return db.getPOIPointsInCategories(cat_ids);
}

//if we want all POIPoints from all categories
vector<POIPoint*> Logic::getPOIPointsInCategories(){
    return db.getPOIPointsInCategories();
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

    //CHECK for inbounds for point A and B
    //OKSANA IMPLEMENT IT!
    //db.checkIfInBoundsOfMap(start_point);


    if (transportMode == 0) tt = ns_permisions::car;
    else if (transportMode == 1) tt = ns_permisions::foot;

    bool found = PathAlgorithms::findShortestPath(&db, start_point, end_point, tt, path, distance, time);
    if (found) return 0;
    return 3; //no path found
}



 int  Logic::getShortestPathsInRadius( QPointF &A,  POICategory* p_cat, float &max_radius, int transportMode,  vector<vector<WaySegment*> > &possible_paths, vector<POIPoint*> &poi_goals, float &distance, float &time)
 {
     //returns 0 if query succeded;
     //1 if A out of bound;
     //3 no path found

     //TransportMode 0 - drive, 1 - walk

     //TODO: check if A, B in bounds.

     //vector<WaySegment*> path;
     //float distance,time;
     boost_xy_point start_point(A.x(),A.y());
     ns_permisions::transport_type tt;

     //CHECK for inbounds for point A and B
     //OKSANA IMPLEMENT IT!
     //db.checkIfInBoundsOfMap(start_point);


     if (transportMode == 0) tt = ns_permisions::car;
     else if (transportMode == 1) tt = ns_permisions::foot;

     bool found = PathAlgorithms::findPathsInRadius(&db, start_point, p_cat, max_radius, tt, possible_paths, poi_goals);
     if (found) return 0;
     return 3; //no path found
 }

QString Logic::TimetoSting(double Time)
{
    double TimeHours, TimeMinutes;
    TimeMinutes = modf(Time, &TimeHours);
    TimeMinutes = round(TimeMinutes*60);

    QString StrTimeMinutes = "";
    QString StrTimeHours = "";

    if (TimeHours>0)
    {
        StrTimeHours.setNum(TimeHours);
        if (TimeHours>1) StrTimeHours.append(" hours ");
        else StrTimeHours.append(" hour ");
    }

    if (TimeMinutes>0)
    {
        StrTimeMinutes.setNum(TimeMinutes);
        if (TimeMinutes>1) StrTimeMinutes.append(" minutes");
        else StrTimeMinutes.append(" minute");
    }

    return StrTimeHours + StrTimeMinutes;
}
