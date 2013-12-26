#ifndef LOGIC_H
#define LOGIC_H
#include "db/database.h"
#include <QPointF>

class Logic
{
private:
    Database db;

public:
    Logic();
    bool checkIfWaysDBIsBuild();
    bool checkIfPOIDBIsBuild();
    map<unsigned long int,Way *>* getAllWays();
   // void printmsg();
    int getShortestPath( QPointF &A, QPointF &B, int transportMode, vector<WaySegment*> &path, float &distance, float &time);
    map<unsigned int,POICategory *>* getCategoryCatalog();
};

#endif // LOGIC_H
