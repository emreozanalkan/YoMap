#ifndef RELATION_H
#define RELATION_H

#include "../db_definitions.hpp"
#include "../node.h"

class Relation
{
public:
    Relation(const unsigned long int&, ns_relation::relation_type);
    unsigned long int getId();
    ns_relation::relation_type getRelationType();
    bool insertNode(Node *);
    bool containsNode(unsigned long int);
    int getNumberOfNodes();
    vector<Node*>::iterator getNodesBegin();
    vector<Node*>::iterator getNodesEnd();
    Node * getNodeAtPos(int);
private:
    unsigned long int id;
    vector<Node *> nodes;
    ns_relation::relation_type type;
};

#endif // RELATION_H
