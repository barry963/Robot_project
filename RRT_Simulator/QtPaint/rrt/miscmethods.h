#ifndef MISCMETHODS_H
#define MISCMETHODS_H

#include "gui/circle.h"
#include "gui/rect.h"

#include "rrt/mynode.h"
#include "rrt/mytree.h"
#include "gui/paintwidget.h"

#include <QImage>
#include <QRgb>


class MiscMethods
{
public:
    MiscMethods();
    MyNode* getRandomState(QImage* image, QList<Shape *> *shapeList, int startR, QList<Shape*>* pathShapeList, MyTree *tree);
    double random();

    MyNode* getNearestNode(MyTree* tree, MyNode* randNode);
    bool isValidExpansion(QImage* image, MyNode* nearestNode, MyNode* randomNode, double maxDist, QList<Shape*>* shapeList, int startR, QList<Shape*>* pathShapeList);
    MyNode* goTowardsNode(MyNode* nearestNode, MyNode* randomNode, double maxDist);
    double distanceBetween(MyNode* n1, MyNode* n2);
    void drawLineSegment(QList<Shape*>* shapeList, MyNode* nearestNode, MyNode* newNode);
    bool goalReached(MyNode* newNode, int goal_x, int goal_y, int goal_r);
    QList<MyNode*>* getPath(MyNode* start, MyNode* end, MyTree* tree);
    void highLightPath(QList<Shape*>* shapeList, QList<MyNode*>* path, double edgeLength);
    QList<MyNode*>* getNearestNodes(MyTree* tree, MyNode* node, double radius);
    QList< QPair<MyNode*, double>* >* sortHashMap(QHash<MyNode*, double>* input);
    void updateEdges(QList<Shape*>* shapeList, MyTree* tree, MyNode* start, MyNode* goal);

    bool circleCircleCollide(Circle* s1, Circle* s2);
    bool circleRectCollide(Circle* s1, Rect* s2);
    bool circleAngleRectCollide(Circle *s1, Rect *s2, double angle);
    double findDistance(double x1, double y1, double x2, double y2);

    bool circleAngleRectCollide2(Circle *s1, Rect *s2, double angle);
    bool testRectangleToPoint(double rectWidth, double rectHeight, double rectRotation, double rectCenterX, double rectCenterY, double pointX, double pointY);
    bool testCircleToSegment(double circleCenterX, double circleCenterY, double circleRadius, double lineAX, double lineAY, double lineBX, double lineBY);

private:
    QRgb blackRgb;
    QRgb redRgb;
};

#endif // MISCMETHODS_H
