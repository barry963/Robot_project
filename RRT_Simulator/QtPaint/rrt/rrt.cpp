#include "rrt/rrt.h"
#include "rrt/miscmethods.h"

#include <vector>
#include <algorithm>
#include <iterator>
#include <QDebug>

using namespace std;

MyRRT::MyRRT(PaintWidget* paintWidget) : start_x(-1), start_y(-1), goal_x(-1), goal_y(-1), goal_r(0)
{
    this->paintWidget = paintWidget;

    image = new QImage(paintWidget->size(), QImage::Format_ARGB32);
    QPainter painter(image);
    paintWidget->render(&painter);
    painter.end();
    myTree = new MyTree();

    setDefaultType();
}

void MyRRT::setDefaultType()
{
    setType(MyRRT::RRT);
}
MyRRT::MyRRT(Type t)
{
    this->type = t;
}


void MyRRT::setType(MyRRT::Type t)
{
    this->type = t;
}



MyRRT::Type MyRRT::getType()
{
    return this->type;
}


void MyRRT::setStart_x(int x)
{
    this->start_x = x;
}

int MyRRT::getStart_x()
{
    return this->start_x;
}

void MyRRT::setStart_y(int y)
{
    this->start_y = y;
}

int MyRRT::getStart_y()
{
    return start_y;
}

void MyRRT::setStart_r(int r)
{
    this->start_r = r;
}

int MyRRT::getStart_r()
{
    return start_r;
}

void MyRRT::setGoal_x(int x)
{
    this->goal_x = x;
}

int MyRRT::getGoal_x()
{
    return goal_x;
}

void MyRRT::setGoal_y(int y)
{
    this->goal_y = y;
}

int MyRRT::getGoal_y()
{
    return this->goal_y;
}

void MyRRT::setGoal_r(int r)
{
    this->goal_r = r;
}

int MyRRT::getGoal_r()
{
    return this->goal_r;
}

void MyRRT::setIsRun(bool isrun)
{
    this->isrun = isrun;
}

bool MyRRT::getIsRun()
{
    return isrun;
}

void MyRRT::setIsStop(bool isstop)
{
    this->isstop = isstop;
}

bool MyRRT::getIsStop()
{
    return isstop;
}


void MyRRT::rrt()
{
    qDebug() << "MyRRT::rrt()";
    startNode = new MyNode(start_x, start_y);
    qDebug() << "startNode: " << start_x << "," << start_y;

    myTree->addNode(startNode, NULL);

    MyNode* randomNode = NULL;
    MyNode* nearestNode = NULL;
    MyNode* newNode = NULL;
    MyNode* goalNode = NULL;

    MiscMethods* mm = new MiscMethods();
    isstop = false;
    isrun = true;
    maxEdgeLength = 10;
    //    goal_r = 10;

    //    tryGoalFactor = mm->getTryGoalMode();

    if(tryGoalFactor == 0)
    {
        adaptiveMode = true;
        tryGoalFactor = 1;
    }
    else{
        adaptiveMode = false;
    }

    while(!isstop){
        while(isrun){
            randomNode = mm->getRandomState(image, paintWidget->getShapeList(), paintWidget->getStartR(), paintWidget->getPathShapeList(), myTree);
            if(goal_x!=-1 && goal_y!=-1 && tryGoalFactor!=-1)
            {
                if(iteration%tryGoalFactor == 0)
                {
                    randomNode = new MyNode(goal_x, goal_y);
                }
            }
            nearestNode = mm->getNearestNode(myTree, randomNode);

            if(mm->isValidExpansion(image, nearestNode, randomNode, 99999, paintWidget->getShapeList(), paintWidget->getStartR(), paintWidget->getPathShapeList())){
                // Create new node
                newNode = mm->goTowardsNode(nearestNode, randomNode, maxEdgeLength);
                //                qDebug() << "newNode: " << newNode->getX() << "," << newNode->getY();
                mm->drawLineSegment(paintWidget->getPathShapeList(), nearestNode, newNode);
                myTree->addChild(nearestNode, newNode);
                myTree->addNode(newNode, nearestNode);
                nodeCount++;
                nearestNode = newNode;
                // Keep expanding until reach the randomNode
                while(mm->distanceBetween(newNode, randomNode) > 0){
                    newNode = mm->goTowardsNode(nearestNode, randomNode, maxEdgeLength);
                    // Draw a line from nearest node to the new node
                    mm->drawLineSegment(paintWidget->getPathShapeList(), nearestNode, newNode);

                    // Set the new node as the child of the nearest node
                    myTree->addChild(nearestNode, newNode);
                    myTree->addNode(newNode, nearestNode);
                    nodeCount++;

                    nearestNode = newNode;
                }

                // Check if we have reached the goal or not
                if(mm->goalReached(newNode, goal_x, goal_y, goal_r)){
                    isrun = false;
                    isstop = true;
                    goalNode = newNode;
                    QList<MyNode*>* path = mm->getPath(startNode, newNode, myTree);


                    // Highlight valid path!
                    mm->highLightPath(paintWidget->getPathShapeList(), path, maxEdgeLength);
                    //                    double pLength = mm->getPathLength(start, newNode);
                    //                    jTextField_pathLength.setText(MM.roundDecimal(pLength)+"");
                }


            }
            else if(adaptiveMode && tryGoalFactor < 10){
                tryGoalFactor++;
            }
            iteration++;
            if(iteration == maxIteration){isrun = false; isstop = true;}
        }
    }
    pathLengthFromGoal = goalNode->getPathLengthFromRoot();
}

void MyRRT::rrt_plus()
{
    qDebug() << "MyRRT::rrt_plus()";
    startNode = new MyNode(start_x, start_y);
    //qDebug() << "startNode: " << start_x << "," << start_y;

    myTree->addNode(startNode, NULL);

    MyNode* randomNode = NULL;
    MyNode* nearestNode = NULL;
    MyNode* newNode = NULL;
    MyNode* goalNode = NULL;

    MiscMethods* mm = new MiscMethods();
    isstop = false;
    isrun = true;
    maxEdgeLength = 10;
    //    goal_r = 10;

    //    tryGoalFactor = mm->getTryGoalMode();

    if(tryGoalFactor == 0)
    {
        adaptiveMode = true;
        tryGoalFactor = 1;
    }
    else{
        adaptiveMode = false;
    }

    while(!isstop){
        QThread::msleep(10);

        while(isrun){
            QThread::msleep(10);

            randomNode = mm->getRandomState(image, paintWidget->getShapeList(), paintWidget->getStartR(), paintWidget->getPathShapeList(), myTree);

            if(goal_x!=-1 && goal_y!=-1 && tryGoalFactor!=-1)
            {
                if(iteration%tryGoalFactor == 0)
                {
                    randomNode = new MyNode(goal_x, goal_y);
                }
            }

            nearestNode = mm->getNearestNode(myTree, randomNode);

            if(mm->isValidExpansion(image, nearestNode, randomNode, 99999, paintWidget->getShapeList(), paintWidget->getStartR(), paintWidget->getPathShapeList())){

                qDebug() << "valid";
                // Create new node
                newNode = mm->goTowardsNode(nearestNode, randomNode, maxEdgeLength);
                //                qDebug() << "newNode: " << newNode->getX() << "," << newNode->getY();

                mm->drawLineSegment(paintWidget->getPathShapeList(), nearestNode, newNode);
                myTree->addChild(nearestNode, newNode);
                myTree->addNode(newNode, nearestNode);
                nodeCount++;

                nearestNode = newNode;

                // Compare with RRT, here we remove the part that "Keep expanding until reach the randomNode"

                // Check if we have reached the goal or not
#if 1
                if(mm->goalReached(newNode, goal_x, goal_y, goal_r)){
                    isrun = false;
                    isstop = true;
                    goalNode = newNode;
                    QList<MyNode*>* path = mm->getPath(startNode, newNode, myTree);


                    // Highlight valid path!
                    mm->highLightPath(paintWidget->getPathShapeList(), path, maxEdgeLength);
                    //                    double pLength = mm->getPathLength(start, newNode);
                    //                    jTextField_pathLength.setText(MM.roundDecimal(pLength)+"");
                }
#endif

            }
            else if(adaptiveMode && tryGoalFactor < 10){
                tryGoalFactor++;
            }
            iteration++;
            qDebug()<<iteration;
            if(iteration == maxIteration){isrun = false; isstop = true;}
        }
    }
    pathLengthFromGoal = goalNode->getPathLengthFromRoot();
}

void MyRRT::rrt_star()
{
    isstop = false;
    isrun = true;
    maxEdgeLength = 10;
    maxIteration = 5000;

    startNode = new MyNode(start_x, start_y);
    myTree->addNode(startNode, NULL);
    MyNode* goal = NULL;

    if(goal_x!=-1 && goal_y!=-1)
    {
        goal = new MyNode(goal_x, goal_y);
    }

    MyNode* randomNode = NULL;
    MyNode* nearestNode = NULL;
    MyNode* newNode = NULL;
    MyNode* goalNode = NULL;

    bool goalReached = false;
    bool goalFound = false;
    double radius = 9999;
    QList<MyNode*>* nearestNodes = new QList<MyNode*>();
    bool nodeAdded = false;

    MiscMethods* mm = new MiscMethods();
    //    tryGoalFactor = mm->getTryGoalMode(jComboBox_tryGoalMode);

    if(tryGoalFactor == 0){
        adaptiveMode = true;
        tryGoalFactor = 1;
    }
    else{
        adaptiveMode = false;
    }

    while(!isstop){
        QThread::msleep(10);

        while(isrun){
            QThread::msleep(10);

            randomNode = mm->getRandomState(image, paintWidget->getShapeList(), paintWidget->getStartR(), paintWidget->getPathShapeList(), myTree);
            nodeAdded = false;

            if(goal!=NULL && tryGoalFactor!=-1){
                if(iteration%tryGoalFactor == 0){
                    randomNode = new MyNode(goal_x, goal_y);
                }
            }

            nearestNodes = mm->getNearestNodes(myTree, randomNode, radius);

            if(nearestNodes->size() > 200){
                radius = mm->distanceBetween(randomNode, nearestNodes->at(200));
            }

            //            if(nearestNodes->isEmpty()) radius = 9999;

            for(int i=0; i<nearestNodes->size(); ++i){

                nearestNode = nearestNodes->at(i);

                if(mm->isValidExpansion(image, nearestNode, randomNode, 99999, paintWidget->getShapeList(), paintWidget->getStartR(), paintWidget->getPathShapeList())){
                    //                    qDebug() << "maxEdgeLength: " << maxEdgeLength;
                    newNode = mm->goTowardsNode(nearestNode, randomNode, maxEdgeLength);
                    if(!goalFound){
                        mm->drawLineSegment(paintWidget->getPathShapeList(), nearestNode, newNode);
                    }

                    myTree->addChild(nearestNode, newNode);
                    //                    qDebug() << "startNode:" << start_x + "," << start_y << ",nearestNode:" << nearestNode->getX() << "," << nearestNode->getY() << ". newNode:" << newNode->getX() << "," << newNode->getY();


                    myTree->addNode(newNode, nearestNode);
#if 1
                    nodeCount++;
                    nearestNode = newNode;

                    // Keep expanding until reach the randomNode
                    while(mm->distanceBetween(newNode, randomNode) > 0){
                        newNode = mm->goTowardsNode(nearestNode, randomNode, maxEdgeLength);
                        if(!goalFound){
                            // Draw a line from nearest node to the new node
                            mm->drawLineSegment(paintWidget->getPathShapeList(), nearestNode, newNode);
                        }

                        // Set the new node as the child of the nearest node
                        myTree->addChild(nearestNode, newNode);
                        myTree->addNode(newNode, nearestNode);
                        nodeCount++;
                        nearestNode = newNode;
                    }


                    // Check if we have reached the goal or not
                    if(mm->goalReached(newNode, goal_x, goal_y, goal_r) && !goalReached){

                        newNode->setCoordinate(goal_x, goal_y);
                        QList<MyNode*>* path = mm->getPath(startNode, newNode, myTree);

                        // Highlight valid path!
                        mm->highLightPath(paintWidget->getPathShapeList(), path, maxEdgeLength);
                        //                    double pLength = mm->getPathLength(start, newNode);
                        //                    jTextField_pathLength.setText(MM.roundDecimal(pLength)+"");

                        mm->updateEdges(paintWidget->getPathShapeList(), myTree, startNode, newNode);
                        goalNode = newNode;
                        goalFound = true;
                    }
#endif


                    if(adaptiveMode){
                        tryGoalFactor = 1;
                    }

                    nodeAdded = true;
                }
                else if(adaptiveMode && tryGoalFactor<10){
                    ++tryGoalFactor;
                }

                if(nodeAdded){
                    break;
                }
            }

#if 1
            if(nodeAdded){
                //                qDebug() << "nodeAdded";
                for(int i=0; i<nearestNodes->size(); ++i){
                    MyNode* thisNode = nearestNodes->at(i);
                    double newNodePathLength = newNode->getPathLengthFromRoot();
                    double thisNodePathLength = thisNode->getPathLengthFromRoot();
                    double distance = mm->distanceBetween(newNode, thisNode);
                    //                    qDebug() << "newNodePathLength: " << newNodePathLength << ", distance: " << distance << ", thisNodePathLength: " << thisNodePathLength;

                    if(newNodePathLength+distance < thisNodePathLength){

                        if(mm->isValidExpansion(image, newNode, thisNode, 99999, paintWidget->getShapeList(), paintWidget->getStartR(), paintWidget->getPathShapeList())){
                            //                             qDebug() << "***";
                            myTree->changeParent(thisNode, newNode);
                            myTree->addChild(newNode, thisNode);
                            thisNode->setPathLengthFromRoot(newNodePathLength + distance);
                            if(!goalFound){
                                mm->drawLineSegment(paintWidget->getPathShapeList(), thisNode, newNode);
                            }
                        }

                    }
                }
            }
#endif
            iteration++;


            //            jTextField_iteration.setText(MM.roundDecimal(iteration)+"");
            if(iteration == maxIteration){
                isrun = false;
                isstop = true;
                if(goalFound){
                    mm->updateEdges(paintWidget->getPathShapeList(), myTree, startNode, goalNode);
                }
            }


        }

    }
    pathLengthFromGoal = goalNode->getPathLengthFromRoot();
}


void MyRRT::run()
{
    //qDebug() << "run in MyRRT, " << thread()->currentThread();
    iteration = 0;
    switch(type)
    {
    case MyRRT::RRT:
        rrt();
        break;
    case MyRRT::RRT_plus:
        rrt_plus();
        break;
    case MyRRT::RRT_star:
        rrt_star();
        break;
    default:
    ;
    }

    //    rrt_star();
}
