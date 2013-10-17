#include "mytree.h"
#include <algorithm>
#include <vector>
#include <iterator>
#include <QDebug>

using namespace std;

MyTree::MyTree()
{
//    vertices = new vector<MyNode*>();
//    children = new vector< vector<MyNode*>* >();
//    parents  = new vector<MyNode*>();
    vertices = new QList<MyNode*>();
    children = new QList< QList<MyNode*>* >();
    parents  = new QList<MyNode*>();
}


void MyTree::addNode(MyNode* n, MyNode* parent)
{
//    qDebug() << "MyTree::addNode";
//    if(std::find(vertices->begin(), vertices->end(), n) == vertices->end())
    if(!vertices->contains(n))
    {
        vertices->append(n);
//        int index = std::distance(vertices->begin(), std::find(vertices->begin(), vertices->end(), n));
        int index = vertices->indexOf(n);
        parents->insert(index, parent);
        children->insert(index, new QList<MyNode*>());
    }


//    if(!vertices.contains(n)){
//        vertices.add(n);
//        int index = vertices.indexOf(n);
//        parents.add(index, parent);
//        children.add(index, new ArrayList<Node>());
//    }
}

void MyTree::addChild(MyNode* parent, MyNode* child)
{
//    int index = std::distance(vertices->begin(), std::find(vertices->begin(), vertices->end(), parent));
    int index = vertices->indexOf(parent);


//    if(!children.get(index).contains(child))
//        children.get(index).add(child);

//    if(std::find((*children)[index]->begin(), (*children)[index]->end(), child) == (*children)[index]->end())
    if(!(*children)[index]->contains(child))
    {
        (*children)[index]->append(child);
    }

//    int index = vertices.indexOf(parent);
//    if(!children.get(index).contains(child))
//        children.get(index).add(child);
}

QList<MyNode*>* MyTree::getChildren(MyNode* n)
{
//    int index = std::distance(vertices->begin(), std::find(vertices->begin(), vertices->end(), n));
    int index = vertices->indexOf(n);
    if(index == -1) return NULL;
    else if(children[index].size() == 0) return NULL;
    else return (*children)[index];


//    int index = vertices.indexOf(n);
//    if(index == -1) return null;
//    else if(children.get(index).isEmpty()) return null;
//    else return children.get(index);
}

MyNode* MyTree::getParent(MyNode* n)
{
//    int index = std::distance(vertices->begin(), std::find(vertices->begin(), vertices->end(), n));
    int index = vertices->indexOf(n);
    return (*parents)[index];


//    int index = vertices.indexOf(n);
//    return parents.get(index);
}

void MyTree::changeParent(MyNode* n, MyNode* newParent)
{
//    int index = std::distance(vertices->begin(), std::find(vertices->begin(), vertices->end(), n));
    int index = vertices->indexOf(n);
    MyNode* temp = (*parents)[index];
    MyNode* oldParent = new MyNode(temp->getX(), temp->getY());
    (*parents)[index] = newParent;
//    index = std::distance(vertices->begin(), std::find(vertices->begin(), vertices->end(), oldParent));
    index = vertices->indexOf(oldParent);


//    int index = vertices.indexOf(n);
//    Node temp = parents.get(index);
//    Node oldParent = new Node(temp.getX(), temp.getY());
//    parents.set(index, newParent);
//    index = vertices.indexOf(oldParent);
}

MyNode* MyTree::getNode(int index)
{
    if(index == -1) return NULL;

    if(index < (vertices->size()))
        return (*vertices)[index];
    else
        return NULL;
}

int MyTree::getNodeCount()
{
    return vertices->size();
}
