#ifndef MYTREE_H
#define MYTREE_H

#include <QList>
#include <vector>
#include "rrt/mynode.h"

using namespace std;

class MyTree
{

public:
    MyTree();
    void addNode(MyNode* n, MyNode* parent);
    void addChild(MyNode* parent, MyNode* child);
    QList<MyNode*>* getChildren(MyNode* n);
    MyNode* getParent(MyNode* n);
    void changeParent(MyNode* n, MyNode* newParent);
    MyNode* getNode(int index);
    int getNodeCount();


private:
//    vector<MyNode*> *vertices;
//    vector< vector<MyNode*>* > *children;
//    vector<MyNode*> *parents;

    QList<MyNode*> *vertices;
    QList< QList<MyNode*>* >* children;
    QList<MyNode*>* parents;


};

#endif // MYTREE_H
