#ifndef MYNODE_H
#define MYNODE_H

class MyNode
{
public:
    MyNode();
    MyNode(int eks, int wye);
    MyNode(int eks, int wye, double pathLength, double distFromGoal);
    void setCoordinate(int eks, int wye);
    void setPathLengthFromRoot(double pathLength);
    void setDistFromGoal(double dist);
    double getDistFromGoal(double dist);
    int getX();
    int getY();
    double getPathLengthFromRoot();


private:
    int x;
    int y;

    double pathLengthFromRoot;
    double distanceFromGoal;





};

#endif // MYNODE_H
