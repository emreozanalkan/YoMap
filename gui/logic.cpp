#include "logic.h"
#include <iostream>
using namespace std;

Logic::Logic(){

#ifdef __APPLE__
    db.build("/Users/emreozanalkan/Desktop/SoftwareEngineering/osm/LeCreusotWaysFF.osm");
#else
    db.build("D:\\Bicycle\\LeCreusotWaysFF.osm");
#endif

}

void Logic::printmsg(){
    cout << "Button pressed" << endl;
}

map<unsigned long int,Way *>* Logic::getAllWays(){
    return db.getAllWays();
}
