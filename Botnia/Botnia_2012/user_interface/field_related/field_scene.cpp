



#include "field_robot.h"
#include "field_scene.h"


#include <QtGui>

FieldScene::FieldScene(QObject *parent): QGraphicsScene(parent)
{
}

FieldScene::FieldScene(QMenu *item_menu, QObject *parent): QGraphicsScene(parent)
{
        my_item_menu = item_menu;
}
//! [0]

BallItem* FieldScene::AddBall()
{
        BallItem* pBall=new BallItem();
        pBall->SetLocationPossibility(0);
        addItem(pBall);
        return pBall;
}

void FieldScene::AddRobot(RobotItem* robot)
{
        robot->SetLocationPossibility(0.0f);
        this->addItem(robot);
//        return temp_robot_;
}

void FieldScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
        QPointF pos=mouseEvent->scenePos();
        emit NotifyMouseMove(pos.x(),pos.y());
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}
