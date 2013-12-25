#include "logic.h"
#include <iostream>
using namespace std;

Logic::Logic(){

#ifdef __APPLE__
    db.build("/Users/emreozanalkan/QTWorkspace/YoMap/data/LeCreusotWaysFF.osm");
    db.buildPOIs("/Users/emreozanalkan/QTWorkspace/YoMap/data/LeCreusot_POI.xml");
#else
    db.build("D:\\Bicycle\\LeCreusotWaysFF.osm");
    db.buildPOIs("OKSANA CALL THIS WITH YOUR POI.XML PATH :)");
#endif

}

void Logic::printmsg(){
    cout << "Button pressed" << endl;
}

map<unsigned long int,Way *>* Logic::getAllWays(){
    return db.getAllWays();
}
