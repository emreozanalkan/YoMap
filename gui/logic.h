#ifndef LOGIC_H
#define LOGIC_H
#include "db/database.h"

class Logic
{
private:
    Database db;
public:
    Logic();
    map<unsigned long int,Way *>* getAllWays();
    void printmsg();
};

#endif // LOGIC_H
