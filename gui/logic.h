#ifndef LOGIC_H
#define LOGIC_H
#include "db/database.h"
#include "algorithms/pathalgorithms.h"
#include "db/poi/poipoint.h"
#include "db/Relations/path.h"
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

    bool checkifPOIInBound(POIPoint* point);
    map<unsigned long int,Way *>* getAllWays();
    map<unsigned long int, Building*>* getAllBuildings();
    vector<POIPoint*> getPOIPointsInCategories(vector<unsigned int> &);
    vector<POIPoint*> getPOIPointsInCategories();

    float getPathTime(Path &p,int mode);
    int getShortestPath( QPointF &A, QPointF &B, int transportMode, Path &best_path);
    int getShortestPathsInRadius( QPointF &A,  POICategory* p_cat, float &max_radius, int transportMode,  set<Path*,ComparePaths> &all_paths);
    int getBicycle( QPointF &A, QPointF &B,   vector<unsigned int> mid_cat, float &max_radius, int transportMode,  Path &path);
    map<unsigned int,POICategory *>* getCategoryCatalog();
    QString TimetoSting(double Time);

    boost_xy_point getMapMinBound();
    boost_xy_point getMapMaxBound();

    void savePOIs();

    void insertAndSavePOIs(POIPoint*);

    POIPoint* createPOI();
};

#endif // LOGIC_H
