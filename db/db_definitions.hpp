#ifndef DB_DEFINITIONS_H
#define DB_DEFINITIONS_H

#include "../stable.h"
//#include <boost/geometry.hpp>
//#include <boost/geometry/geometries/point_xy.hpp>
//#include <boost/geometry/index/rtree.hpp>

#include <string>
#include <sstream>

class Relation;
using namespace std;
//definition boost:geometry
namespace bg = boost::geometry;

//definition of 2d point in boost
typedef bg::model::d2::point_xy<double> boost_xy_point;

//definition of rtree element
typedef std::pair<bg::model::box<boost_xy_point>, Relation* > rtree_element;

//definition of rtree
typedef bg::index::rtree<rtree_element, bg::index::rstar<16> > boost_rtree;



//define types of relations - higher more important in opengl
namespace ns_relation {
    enum relation_type
    {
        undefined=0,
        way_segment,
        path_segment,
        way,
        building
    };
}
namespace ns_way {
    enum way_type
    {
        primary=0,
        primary_link,
        secondary,
        secondary_link,
        tertiary,
        tertiary_link,
        residential,
        unclassified,
        footway,
        steps,
        track,
        raceway,
        service

    };
}
namespace ns_access {
    enum access_type
    {
        all=0,
        privat,
        truck
    };
}
namespace ns_oneway {
    enum oneway_type
    {
        no=0,
        yes,
        opposite
    };
}

namespace ns_permisions {
    enum transport_type
    {
        any=0,
        car,
        foot
    };
}




static ns_way::way_type getWayTypeFromString(string t){
    if(t.compare("primary")==0){
         return ns_way::primary;
    }
    else if(t.compare("primary_link")==0){
        return ns_way::primary_link;
    }
    else if(t.compare("secondary")==0){
        return ns_way::secondary;
    }
    else if(t.compare("secondary_link")==0){
        return ns_way::secondary_link;
    }
    else if(t.compare("tertiary")==0){
        return ns_way::tertiary;
    }
    else if(t.compare("tertiary_link")==0){
        return ns_way::tertiary_link;
    }
    else if(t.compare("residential")==0){
        return ns_way::residential;
    }
    else if(t.compare("unclassified")==0){
        return ns_way::unclassified;
    }
    else if(t.compare("steps")==0){
        return ns_way::steps;
    }
    else if(t.compare("track")==0){
        return ns_way::track;
    }
    else if(t.compare("raceway")==0){
        return ns_way::raceway;
    }
    else if(t.compare("service")==0){
        return ns_way::service;
    }
    else if(t.compare("footway")==0){
        return ns_way::footway;
    }
    return ns_way::unclassified;
}

static float getSpeedFromWayType(ns_permisions::transport_type &tt, ns_way::way_type wt){
    switch(tt){
    case ns_permisions::car:
        switch(wt){
            case ns_way::primary:
            case ns_way::primary_link:
            case ns_way::secondary:
            case ns_way::secondary_link:
            case ns_way::tertiary:
            case ns_way::tertiary_link:
            case ns_way::residential:
            case ns_way::unclassified:
                return 50.0;
                break;
            case ns_way::footway:
            case ns_way::steps:
            case ns_way::track:
            case ns_way::raceway:
            case ns_way::service:
                return 0.0;
                break;
        }
        break;
    case ns_permisions::foot:
        switch(wt){
            case ns_way::primary:
            case ns_way::primary_link:
            case ns_way::secondary:
            case ns_way::secondary_link:
            case ns_way::tertiary:
            case ns_way::tertiary_link:
            case ns_way::residential:
            case ns_way::unclassified:
            case ns_way::footway:
                return 4.5;
                break;
            case ns_way::steps:
                return 2.0;
                break;
            case ns_way::track:
            case ns_way::raceway:
            case ns_way::service:
                return 0.0;
                break;
        }
        break;
    case ns_permisions::any:
        switch(wt){
            case ns_way::primary:
            case ns_way::primary_link:
            case ns_way::secondary:
            case ns_way::secondary_link:
            case ns_way::tertiary:
            case ns_way::tertiary_link:
            case ns_way::residential:
            case ns_way::unclassified:
                return 50.0;
                break;
            case ns_way::footway:
                return 4.5;
                break;
            case ns_way::steps:
                return 2.0;
                break;
            case ns_way::track:
            case ns_way::raceway:
            case ns_way::service:
                return 0.0;
                break;
        }
        break;
    }
    return 0.0;
}



static ns_access::access_type getAccessTypeFromString(string s){
    if(s=="private"){
        return ns_access::privat;
    }
    else if(s=="truck"){
        return ns_access::truck;
    }
    return ns_access::all;
}



static ns_oneway::oneway_type getOnewayTypeFromString(string s){
    if(s=="yes"){
        return ns_oneway::yes;
    }
    else if(s=="no"){
        return ns_oneway::no;
    }
    else if(s=="-1"){
        return ns_oneway::opposite;
    }
    return ns_oneway::no;
}
#endif // DB_DEFINITIONS_H
