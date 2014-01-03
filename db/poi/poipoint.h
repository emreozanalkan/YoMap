#ifndef POIPOINT_H
#define POIPOINT_H

#include "../db_definitions.hpp"
#include <string>

using namespace std;
class POICategory;

class POIPoint
{
public:
    POIPoint(const unsigned int &);
    POIPoint(const unsigned int &, const double &, const double &,const string &,const string &, const string&, const string &);
    void setGeoPosition(const double &, const double &);
    boost_xy_point& getGeoPosition();
    void setCategory(POICategory*);
    POICategory* getCategory();
    unsigned int getId();
    string getName();
    void setName(string);
    string getAddress();
    void setAddress(string);
    string getIconPath();
    string getPhotoPath();
    void setPhotoPath(string);
    string getUser();
    void setUser(string);

private:
    unsigned int id;
    //EPSG:3785
    boost_xy_point geoPosition;
    string name;
    string addr;
    string photo_path;
    string user;
    POICategory* category;
};

#endif // POIPOINT_H
