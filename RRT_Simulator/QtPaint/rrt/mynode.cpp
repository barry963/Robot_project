#include "mynode.h"
#include <limits>

using namespace std;

MyNode::MyNode()
{
    x = -1;
    y = -1;
    pathLengthFromRoot = -1;
    distanceFromGoal = numeric_limits<double>::max();
}

MyNode::MyNode(int eks, int wye)
{
    x = eks;
    y = wye;
    pathLengthFromRoot = -1;
    distanceFromGoal = numeric_limits<double>::max();
}

MyNode::MyNode(int eks, int wye, double pathLength, double distFromGoal)
{
    x = eks;
    y = wye;
    pathLengthFromRoot = pathLength;
    distanceFromGoal = distFromGoal;
}

void MyNode::setCoordinate(int eks, int wye)
{
    x = eks;
    y = wye;
}


void MyNode::setPathLengthFromRoot(double pathLength)
{
    pathLengthFromRoot = pathLength;
}

void MyNode::setDistFromGoal(double dist)
{
    distanceFromGoal = dist;
}

double MyNode::getDistFromGoal(double dist)
{
    return distanceFromGoal;
}

int MyNode::getX()
{
    return x;
}

int MyNode::getY()
{
    return y;
}

double MyNode::getPathLengthFromRoot()
{
    return pathLengthFromRoot;
}
