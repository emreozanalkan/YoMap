#ifndef DATABASE_H
#define DATABASE_H
#include "db_definitions.hpp"

#include "node.h"
#include "Relations/waysegment.h"
#include "Relations/way.h"
#include "Relations/building.h"
#include "Relations/relation.h"
#include "poi/poicategory.h"
#include "poi/poipoint.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <QFile>
#include <QString>
#include <QDomDocument>
#include <QXmlStreamWriter>


class Database
{
public:
    Database();
    Database(QString path);
    ~Database();
    int build(QString path);
    int buildPOIs(QString path);
    int savePOIs(QString path);

    bool checkIfInBoundsOfMap(boost_xy_point);

    Node* getNodeById(unsigned long int);
    Way* getWayById(unsigned long int);

    POICategory* getPOICategoryById(unsigned int i);
    POIPoint* getPOIPointByPosition(unsigned int cat_id,unsigned int point_pos);
    map<unsigned int,POICategory *>* getCategoryCatalog();
    vector<POIPoint*> getPOIPointsInCategories(vector<unsigned int> &);
    //if we want all POIPoints from all categories
    vector<POIPoint*> getPOIPointsInCategories();

    map<unsigned long int,Way *>* getAllWays();
    map<unsigned long int,Building *>* getAllBuildings();

    void searchWaySegmentsInArea(boost_xy_point min,boost_xy_point max,vector<WaySegment*> &objects_in_area,ns_permisions::transport_type &tt);

    bool isWaysBuild();
    bool isPOIBuild();

    boost_xy_point getMapMinBound();
    boost_xy_point getMapMaxBound();

    POIPoint* createPOI();

    void insertNewPOICategory(POICategory*);
    void insertNewPOIPoint(POIPoint *);
private:
    bool ways_build;
    bool poi_build;
    //containers of all nodes by type
    map<unsigned long int,Node *> all_nodes;

    //containers of all relations by type
    map<unsigned long int,Way *> all_ways;

    //containers of all relations by type
    map<unsigned long int,Building *> all_buildings;

    //Rtree with all rtree elements
    boost_rtree rtree;

    //container of all POI categories
    map<unsigned int,POICategory *> all_poi_categories;
    map<unsigned int,POIPoint *> all_poi_points;

    boost_xy_point min_bound;
    boost_xy_point max_bound;

    void setBounds(double &min_lon, double &min_lat, double &max_lon, double &max_lat);

    Relation* processWay(QDomElement &e, string t);
    Relation* processBuilding(QDomElement &e, string t);

    void insertNewWay(Way *);
    void insertNewBuilding(Building *);
    void insertNewNode(Node *);
};

#endif // DATABASE_H

