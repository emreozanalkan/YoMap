#ifndef LOGIC_H
#define LOGIC_H
#include "db/database.h"
#include "db/poi/poipoint.h"
#include <QPointF>
#include <vector>

class Logic
{
private:
    Database db;

public:
    Logic();
    bool checkIfWaysDBIsBuild();
    bool checkIfPOIDBIsBuild();
    map<unsigned long int,Way *>* getAllWays();
    vector<POIPoint*> getPOIPointsInCategories(vector<unsigned int> &);
    vector<POIPoint*> getPOIPointsInCategories();

   // void printmsg();
    int getShortestPath( QPointF &A, QPointF &B, int transportMode, vector<WaySegment*> &path, float &distance, float &time);
    map<unsigned int,POICategory *>* getCategoryCatalog();
};

#endif // LOGIC_H
