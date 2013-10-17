#ifndef FIELD_SCENE_H
#define FIELD_SCENE_H


#include "field_ball.h"
#include "field_robot.h"



#include <QGraphicsScene>



QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE



class FieldScene : public QGraphicsScene
{
        Q_OBJECT
public:
        FieldScene(QObject* parent = 0);
        FieldScene(QMenu *item_menu, QObject *parent = 0);
        BallItem* AddBall();
        void AddRobot(RobotItem* robot);

public slots:

signals:
        void NotifyMouseMove(double x,double y);

protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
        QMenu *my_item_menu;
};
//! [0]
#endif // FIELD_SCENE_H
