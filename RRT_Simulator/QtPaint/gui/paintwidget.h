#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QtGui>
#include <QDebug>
#include "shape.h"
#include "line.h"
#include "rect.h"
#include "circle.h"

#include "rrt/rrt.h"

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    PaintWidget(QWidget *parent = 0);
    QPixmap* getPixmap();
    void setPixmap(QPixmap* pixmap);

    QList<Shape*>* getShapeList();
    QList<Shape*>* getPathShapeList();

    Shape* getCurrStartShape();
    Shape* getCurrGoalShape();

    int getStartX(){
        return startX;
    }
    int getStartY() {
        return startY;
    }
    int getStartR(){
        return startR;
    }
    int getGoalX(){
        return goalX;
    }
    int getGoalY(){
        return goalY;
    }
    int getGoalR(){
        return goalR;
    }

    MyRRT::Type getType()
    {
        return type;
    }

    void setType(MyRRT::Type type)
    {
        this->type = type;
    }
    MyRRT* getMyRRT()
    {
        return myRRT;
    }
        MyRRT* myRRT;


public slots:
    void setCurrentShape(Shape::Code s)
    {
        if(s != currShapeCode) {
            currShapeCode = s;
        }
    }

    void startSimulationSlot();
    void stopSimulationSlot();
    void resetSimulationSlot();

    void rrtTextChangedSlot(QString name);
    void stepChangedSlot(QString step);

signals:
    void currentIteration(int);
    void currentNodeCount(int);
    void currentPathLength(int);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Shape::Code currShapeCode;
    Shape *shape;
    Shape* pathShape;
    Shape* currStartShape;
    Shape* currGoalShape;

    bool perm;
    QList<Shape*>* shapeList;   // Include start node, goal node, and obstacles
    QList<Shape*>* pathShapeList;   // Include line path generated later, which can be cleared

    QPixmap* pixmap;


    MyRRT::Type type;

    int startX;
    int startY;
    int startR;
    int goalX;
    int goalY;
    int goalR;

    QTimer* paintTimer;
};

#endif // PAINTWIDGET_H
