#ifndef BUILDING_H
#define BUILDING_H
#include "relation.h"
#include "../db_definitions.hpp"

class Building : public Relation
{
public:
    Building(unsigned long int &);
};

#endif // BUILDING_H
