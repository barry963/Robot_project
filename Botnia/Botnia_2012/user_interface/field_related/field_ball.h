#ifndef FIELD_BALL_H
#define FIELD_BALL_H

#include "field_constants.h"
//#include "robocup_ssl_visionclient.h"
#include "field_item.h"
//#include "vqueue.h"
#include "user_interface/field_related/motion_simulation_tools/field_vectors.h"
#include "field_item.h"



#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QVector>
#include <QGraphicsView>
#include <QPainterPath>
#include <QApplication>
#include <QThread>
#include <QGLWidget>
#include <QMutex>



class BallItem : public FieldItem
{
public:
        //int cameraId;                 //ID of the camera
        QBrush *brush;
        QPen *pen;
        QPainterPath ball_out_line;

        //-------------------------------------------------------
        BallItem();
        ~BallItem();

        QRectF boundingRect() const;
        void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
        void SetLocationPossibility(double _location_possibility);
protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

#endif // FIELD_BALL_H
