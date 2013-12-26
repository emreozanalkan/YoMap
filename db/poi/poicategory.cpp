#include "poicategory.h"

POICategory::POICategory(const unsigned int &i, const string &n, const string &icon)
{
    id = i;
    name = n;
    icon_path = icon;
}

unsigned int POICategory::getId(){
    return id;
}

string POICategory::getName(){
    return name;
}

void POICategory::addPOI(POIPoint* p){
    poi_points.push_back(p);
}

string POICategory::getIconPath(){
    return ":/map_icons/"+icon_path;
}

vector<POIPoint *>::iterator POICategory::getPOIPointsBegin(){
    return poi_points.begin();
}

vector<POIPoint *>::iterator POICategory::getPOIPointsEnd(){
    return poi_points.end();
}

POIPoint* POICategory::getPOIPointAt(unsigned int &pos){
    if(poi_points.size()<pos)
        return 0;
    else
        return poi_points[pos];
}

