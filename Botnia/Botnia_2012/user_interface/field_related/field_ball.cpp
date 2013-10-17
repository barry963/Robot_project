#include "field_ball.h"
#include "user_interface/field_related/field_view.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor.h"
#include "field_constants.h"
#include "math.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QList>
#include <QDebug>



BallItem::BallItem():FieldItem(FieldItem::BALL_ITEM_TYPE,0,0)
{
        setPos(0,0);
        brush = new QBrush ( QColor ( 0xff,0x81,0x00,255 ),Qt::SolidPattern );
        pen = new QPen ( QColor ( 0xcd,0x59,0x00,255 ) );
        pen->setWidth ( 4 );
        this->setVisible(false);
        ball_out_line.addEllipse ( -21,-21,42,42 );
        setPath(ball_out_line);
        location_possibility=1;
}

void BallItem::SetLocationPossibility(double _location_possibility)
{
        location_possibility=_location_possibility;
        if (location_possibility>1.0)
        {
                location_possibility=1.0;
        }
        if (location_possibility>0.0)
        {
                setVisible(true);
        }
        else
        {
                setVisible(false);
        }
}

BallItem::~BallItem()
{
        delete brush;
        delete pen;
}

QRectF BallItem::boundingRect() const
{
        QRectF boundR( -21,-21,42,42 );
        return boundR;
}

void BallItem::paint ( QPainter *painter, const QStyleOptionGraphicsItem* , QWidget* )
{
        if ( location_possibility<=0.0 )
                return;
        if ( location_possibility >1.0) location_possibility = 1.0;
        painter->setPen ( *pen );
        if (isSelected())
        {
                painter->setBrush ( Qt::blue );
        }
        else
        {
                painter->setBrush ( *brush );
        }
        painter->drawPath ( ball_out_line );
}

//! [5]
void BallItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
        double maxx,maxy;
        if(isSelected())
        {
                QPointF p=mouseEvent->scenePos();
                maxx=FIELD_LENGTH_H+FieldConstantsRoboCup2012::boundary_width_;
                maxy=FIELD_WIDTH_H+FieldConstantsRoboCup2012::boundary_width_;
                if(p.x()>maxx || p.x()<-maxx)
                {
                        return;
                }
                if(p.y()>maxy   || p.y()<-maxy)
                {
                        return;
                }
                m_Simulator.lock();
                m_Simulator.ball.pos.set(p.x(),p.y());
                m_Simulator.unlock();

                setPos(p.x(),p.y());//lu_test add
        }
}
