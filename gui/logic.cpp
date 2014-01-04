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

map<unsigned long int, Building*>* Logic::getAllBuildings()
{
    return db.getAllBuildings();
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

float Logic::getPathTime(Path &p,int transportMode){
    //Set transportation mode
    ns_permisions::transport_type tt;
    if (transportMode == 0) tt = ns_permisions::car;
    else if (transportMode == 1) tt = ns_permisions::foot;

    return p.getTravelTime(tt);
}



int Logic::getShortestPath( QPointF &A, QPointF &B, int transportMode, Path &best_path)
{
    //returns 0 if query succeded;
    //1 if A out of bound;
    //2 is B out of bound;
    //3 no path found
    //TransportMode 0 - drive, 1 - walk

    //Transform points to boost points
    boost_xy_point start_point(A.x(),A.y());
    boost_xy_point end_point(B.x(),B.y());

    //check if point A is inbound
    if(!db.checkIfInBoundsOfMap(start_point)){
        return 1;
    }
    //check if point B is inbound
    if(!db.checkIfInBoundsOfMap(end_point)){
        return 2;
    }

    //Set transportation mode
    ns_permisions::transport_type tt;
    if (transportMode == 0) tt = ns_permisions::car;
    else if (transportMode == 1) tt = ns_permisions::foot;

    //Do the search
    int found = PathAlgorithms::findShortestPath(db, start_point, end_point, tt,best_path);

    return found;
}



 int Logic::getShortestPathsInRadius( QPointF &A,  POICategory* p_cat, float &max_radius, int transportMode,  set<Path*,ComparePaths> &all_paths)
 {
     //returns 0 if query succeded;
     //1 if A out of bound;
     //3 no path found

//     //TransportMode 0 - drive, 1 - walk

//     //TODO: check if A, B in bounds.

     //vector<WaySegment*> path;
     //float distance,time;
     boost_xy_point start_point(A.x(),A.y());
     ns_permisions::transport_type tt;

     //CHECK for inbounds for point A and B
     //OKSANA IMPLEMENT IT!
     //db.checkIfInBoundsOfMap(start_point);


     if (transportMode == 0) tt = ns_permisions::car;
     else if (transportMode == 1) tt = ns_permisions::foot;

     int found = PathAlgorithms::findPathsInRadius(db, start_point, p_cat, max_radius, tt, all_paths);
     return found; //no path found
 }

 int Logic::getBicycle( QPointF &A, QPointF &B,   vector<unsigned int> mid_cat, float &max_radius, int transportMode,  Path &path)
 {
     //returns 0 if query succeded;
     //1 if A out of bound;
     //3 no path found

//     //TransportMode 0 - drive, 1 - walk

//     //TODO: check if A, B in bounds.

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

     int found = PathAlgorithms::BicycleSearch(db, start_point,end_point, mid_cat, max_radius, true, tt, path);
     return found; //no path found
 }




QString Logic::TimetoSting(double Time)
{
    double TimeHours, TimeMinutes;
    TimeMinutes = modf(Time, &TimeHours);
    TimeMinutes = round(TimeMinutes*60.0);

    QString StrTimeMinutes = "";
    QString StrTimeHours = "";

    if (TimeHours>0.0)
    {
        StrTimeHours.setNum(TimeHours);
        if (TimeHours>1) StrTimeHours.append(" hours ");
        else StrTimeHours.append(" hour ");
    }

    if (TimeMinutes>0.0)
    {
        StrTimeMinutes.setNum(TimeMinutes);
        if (TimeMinutes>1) StrTimeMinutes.append(" minutes");
        else StrTimeMinutes.append(" minute");
    }
    else
      {
          StrTimeMinutes.append("Less than a minute");
      }

    return StrTimeHours + StrTimeMinutes;
}

boost_xy_point Logic::getMapMinBound()
{
    return db.getMapMinBound();
}

boost_xy_point Logic::getMapMaxBound()
{
    return db.getMapMaxBound();
}
