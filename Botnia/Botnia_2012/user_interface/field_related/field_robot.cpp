
#include "field_robot.h"
//#include "SoccerScene.h"
#include "user_interface/field_related/field_view.h"
#include "field_constants.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor.h"
#include "math.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QList>

#include <QDebug>




#if 1
//----------------------------------------------------------------------------------

RobotItem::RobotItem()
    :FieldItem(FieldItem::ROBOT_ITEM_TYPE,0,0)
{
    RobotItem ( 0,0,0,teamUnknown,0,0,0 );
    robotTactic="213213";
}

RobotItem::RobotItem ( double _x, double _y, double _orientation, int _teamID, int _id, int _key, double _location_possibility )
    :FieldItem(FieldItem::ROBOT_ITEM_TYPE,0,0)
{
    location_possibility = _location_possibility;
    key = _key;
    setVisible ( true );
    setZValue ( 2 );
    setPos(_x,_y);
    orientation=_orientation;
    setPos(_x,_y);
    teamID = _teamID;
    id = _id;
    robotTactic="213213";
    robotOutline.moveTo ( 90,0 );
    robotOutline.arcTo ( -90,-90,180,180,0,270 );
    robotOutline.closeSubpath();
    robotOutlineCircle.addEllipse ( -90,-90,180,180 );
    if ( id!=NA )
        robotLabel = QString ( QByteArray ( ( char * ) &id, 4 ).toHex() ).mid ( 1,1 ).toUpper();
    else
        robotLabel = "?";
    drawFont = QFont ( "Courier",80,2,false );
    robotID.addText ( -25,25,QFont ( "Courier",80,2,false ),robotLabel );
    switch ( teamID )
    {
    case teamBlue:
    {
        brush = new QBrush ( QColor ( 0x41,0x7e,0xff,255 ),Qt::SolidPattern );
        pen = new QPen ( QColor ( 0x12,0x3b,0xa0,255 ) );

        break;
    }
    case teamYellow:
    {
        brush = new QBrush ( QColor ( 0xff,0xf3,0x3e,255 ),Qt::SolidPattern );
        pen = new QPen ( QColor ( 0xcc,0x9d,0x00,255 ) );
        break;
    }
    default:
    {
        brush = new QBrush ( QColor ( 150,150,150,255 ),Qt::SolidPattern );
        pen = new QPen ( QColor ( 70,70,70,255 ) );
        break;
    }
    }
    pen->setWidth ( 6 );
    id_pen = new QPen ( Qt::black );
    id_pen->setWidth ( 0 );
    conf_pen = new QPen ( Qt::white );
    conf_pen->setWidth ( 1 );
}

void RobotItem::Setid(int _teamID,int _id)
{
    id=_id;
    if ( id!=NA )
        robotLabel = QString ( QByteArray ( ( char * ) &id, 4 ).toHex() ).mid ( 1,1 ).toUpper();
    else
        robotLabel = "?";
}

void RobotItem::SetTactic(QString s)
{
    robotTactic = s;
}


RobotItem::~RobotItem()
{
    delete brush;
    delete pen;
    delete id_pen;
}

QRectF RobotItem::boundingRect() const
{
    return QRectF ( -95,-95,190,190 );
}

void RobotItem::paint ( QPainter *painter, const QStyleOptionGraphicsItem* , QWidget* )
{
    if ( location_possibility<=0.0 )
    {
        setVisible (false);
        return;
    }
    if ( location_possibility >1.0) location_possibility = 1.0;
    //机器人本体绘制
    painter->setPen ( *pen );
    painter->setBrush ( *brush );
    double _orientation=orientation*180.0/M_PI;

    if ( fabs ( _orientation ) <360 )
    {
        painter->rotate ( -45+_orientation );
        painter->drawPath ( robotOutline );
        painter->rotate ( 45-_orientation );
    }
    else
    {
        painter->drawPath ( robotOutlineCircle );
    }
    painter->scale(1,-1);
    //绘制id文字
    painter->setPen ( *id_pen );
    painter->setBrush ( Qt::black );
    painter->setFont ( drawFont );
    //painter->drawPath(robotID);
    painter->drawText ( -45,-60,1000,1000,0,robotLabel );
    //绘制tactic文字
    painter->setPen ( Qt::yellow );
    painter->setBrush ( Qt::black );
    painter->setFont ( drawFont );
    painter->drawText ( -90,-230,1500,1000,0,robotTactic );
    //绘制可信度进度条
    painter->setPen ( Qt::NoPen );
    painter->setBrush ( *brush );
    painter->drawRect ( -90,-130, ( int ) ( ( ( double ) 180 ) *location_possibility ),30 );
    //绘制可信度外框
    painter->setPen ( *pen );
    if (isSelected())
    {
        painter->setBrush ( Qt::red );
    }
    else
    {
        painter->setBrush ( QBrush ( Qt::white, Qt::NoBrush ) );
    }
    painter->drawRect ( -90,-130,180,30 );
    painter->scale(1,-1);
    //绘制机器人速度是否Kick是否Dribble
}

QPainterPath RobotItem::shape() const
{
    QPainterPath path;
    path.addEllipse ( -90, -90, 180, 180 );
    return path;
}

void RobotItem::SetLocationPossibility(double _location_possibility)
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

void RobotItem::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Delete:
        location_possibility=0;
        //cout<<"Key_F1 pressed!";
        break;
    }
}

//! [5]
void RobotItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(isSelected())
    {
        QPointF p=mouseEvent->scenePos();
        if(p.x()>FIELD_LENGTH_H+FieldConstantsRoboCup2012::boundary_width_ || p.x()<-FIELD_LENGTH_H-FieldConstantsRoboCup2012::boundary_width_)return;
        if(p.y()>FIELD_WIDTH_H+FieldConstantsRoboCup2012::boundary_width_  || p.y()<-FIELD_WIDTH_H-FieldConstantsRoboCup2012::boundary_width_)return;

        m_Simulator.lock();
        if(teamID==teamBlue)
        {
            m_Simulator.blue_robots[id].pos.p.set(p.x(),p.y());
        }
        else if(teamID==teamYellow)
        {

            m_Simulator.yellow_robots[id].pos.p.set(p.x(),p.y());
        }
        m_Simulator.unlock();

        setPos(p.x(),p.y());//lu_test add

    }


}

void RobotItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{

/*
    if(isSelected())
    {
        QPointF p=event->scenePos();

        m_Simulator.lock();
        if(teamID==teamBlue)
        {
            m_Simulator.blue_robots[id].pos.dir=;
        }
        else if(teamID==teamYellow)
        {

            m_Simulator.yellow_robots[id].pos.dir=;
        }
        m_Simulator.unlock();
    }
    */

}


#endif
