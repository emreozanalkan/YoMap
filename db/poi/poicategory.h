#ifndef POICATEGORY_H
#define POICATEGORY_H

#include "poipoint.h"
#include <string>
#include <vector>

using namespace std;

class POICategory
{
public:
    POICategory(const unsigned int&, const string&, const string&);
    unsigned int getId();
    string getName();
    void addPOI(POIPoint*);
    string getIconPath();

    vector<POIPoint *>::iterator getPOIPointsBegin();
    vector<POIPoint *>::iterator getPOIPointsEnd();
    POIPoint* getPOIPointAt(unsigned int &pos);

private:
    unsigned int id;
    string name;
    string icon_path;
    vector<POIPoint*> poi_points;

};

#endif // POICATEGORY_H
