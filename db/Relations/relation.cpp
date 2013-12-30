#include "relation.h"
#include "../node.h"

Relation::Relation(const unsigned long int& i, ns_relation::relation_type r)
{
    id = i;
    type = r;
}
//Basic data
unsigned long int Relation::getId(){
    return id;
}
ns_relation::relation_type Relation::getRelationType(){
    return type;
}

//Nodes which belong to this relationship
bool Relation::insertNode(Node * n){
    ns_relation::relation_type rt = n->getType();
    if(rt==ns_relation::undefined){
        n->setType(type);
    }

    nodes.push_back(n);
    return true;
}
bool Relation::containsNode(unsigned long int i){
    for (vector<Node*>::iterator it=nodes.begin(); it!=nodes.end(); it++){
        if((*it)->getId()==i)
            return true;
    }
    return false;
}
int Relation::getNumberOfNodes(){
    return nodes.size();
}
vector<Node*>::iterator Relation::getNodesBegin(){
    return nodes.begin();
}
vector<Node*>::iterator Relation::getNodesEnd(){
    return nodes.end();
}
Node * Relation::getNodeAtPos(int i){
    if(i<nodes.size()){
        return nodes[i];
    }
    return 0;
}
