#ifndef RRT_H
#define RRT_H

#include "rrt/mytree.h"
#include "rrt/mynode.h"
//#include "gui/paintwidget.h"      // Reinclude will cause random error!!!
#include <QString>
#include <QThread>
#include <QImage>

class PaintWidget;

class MyRRT : public QThread
{
    Q_OBJECT
public:
    enum Type {
        RRT,
        RRT_plus,
        RRT_star
    };

    MyRRT(PaintWidget* paintWidget);
    MyRRT(Type t);

    void setType(MyRRT::Type t);
    MyRRT::Type getType();

    void setStart_x(int x);
    int getStart_x();
    void setStart_y(int y);
    int getStart_y();
    void setStart_r(int r);
    int getStart_r();

    void setGoal_x(int x);
    int getGoal_x();
    void setGoal_y(int y);
    int getGoal_y();
    void setGoal_r(int r);
    int getGoal_r();
    void setIsRun(bool isrun);
    bool getIsRun();
    void setIsStop(bool isstop);
    bool getIsStop();
    int getIteration()
    {
        return this->iteration;
    }
    MyTree* getMyTree()
    {
        return this->myTree;
    }
    int getNodeCount()
    {
        return this->nodeCount;
    }
    double getPathLengthFromGoal()
    {
        return this->pathLengthFromGoal;
    }




    // Path planning algorithms
    void rrt();
    void rrt_plus();
    void rrt_star();

protected:
    void run();

private: // methods
    void setDefaultType();

private:

//    PaintWidget* paintWidget;

    MyRRT::Type type;

    int start_x;
    int start_y;
    int start_r;
    int goal_x;
    int goal_y;
    int goal_r;

    bool isrun;
    bool isstop;

    MyTree* myTree;
    MyNode* startNode;

    int nodeCount;
    int iteration;
    int tryGoalFactor;
    bool adaptiveMode;

    int maxIteration;
    double pathLengthFromGoal;
    double maxEdgeLength;
    QString algorithm;

    PaintWidget* paintWidget;
    QImage* image;

};


#endif // RRT_H
