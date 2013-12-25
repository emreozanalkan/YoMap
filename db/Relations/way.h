#ifndef WAY_H
#define WAY_H
#include "relation.h"
#include "../db_definitions.hpp"
#include <string>


class Way : public Relation
{
public:
    Way(unsigned long int &);

    //set and get properties of way
    void setStreetName(string);
    void setWayType(ns_way::way_type);
    void setAccessType(ns_access::access_type);
    void setOnewayType(ns_oneway::oneway_type);
    void setAsBridge(const bool &b);
    void setAsRoundabout(const bool &b);
    string getStreetName();
    ns_way::way_type getWayType();
    ns_access::access_type getAccessType();
    ns_oneway::oneway_type getOnewayType();
    bool isBridge();
    bool isRoundabout();
private:
    string street_name;
    ns_way::way_type waytype;
    ns_access::access_type access;
    ns_oneway::oneway_type oneway;
    //it is on a bridge
    bool bridge;
    //if its a roundabout
    bool roundabout;

};

#endif // WAY_H
