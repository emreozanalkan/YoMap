#include "way.h"

Way::Way(unsigned long int &i):Relation(i,ns_relation::way)
{
  waytype=ns_way::unclassified;
   access=ns_access::all;
    oneway=ns_oneway::no;
    bridge=false;
    roundabout=false;
}

void Way::setStreetName(string t){
    street_name = t;
}

void Way::setWayType(ns_way::way_type r){
    waytype = r;
}

void Way::setAccessType(ns_access::access_type a){
    access = a;
}

void Way::setOnewayType(ns_oneway::oneway_type o){
    oneway = o;
}

void Way::setAsBridge(const bool &b){
    bridge = b;
}

void Way::setAsRoundabout(const bool &b){
    roundabout = b;
}
string Way::getStreetName(){
    return street_name;
}
ns_way::way_type Way::getWayType(){
    return waytype;
}

ns_access::access_type Way::getAccessType(){
    return access;
}

ns_oneway::oneway_type Way::getOnewayType(){
    return oneway;
}

bool Way::isBridge(){
    return bridge;
}

bool Way::isRoundabout(){
    return roundabout;
}
