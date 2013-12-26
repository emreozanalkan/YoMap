#include "poipoint.h"


POIPoint::POIPoint(const unsigned int &i)
{
    id = i;
}

POIPoint::POIPoint(const unsigned int &i, const double &x, const double &y,const string &n,const string &a, const string &p,const string &u){
    id = i;
    geoPosition.x(x);
    geoPosition.y(y);
    name = n;
    addr = a;
    photo_path = p;
    user = u;
}

void POIPoint::setGeoPosition(const double &x, const double &y){
    geoPosition.x(x);
    geoPosition.y(y);
}
boost_xy_point& POIPoint::getGeoPosition(){
    return geoPosition;
}

void POIPoint::setCategory(POICategory *c){
    category = c;
}

POICategory* POIPoint::getCategory(){
    return category;
}

unsigned int POIPoint::getId(){
    return id;
}

string POIPoint::getName(){
    return name;
}
string POIPoint::getIconPath(){
    return category->getIconPath();
}
