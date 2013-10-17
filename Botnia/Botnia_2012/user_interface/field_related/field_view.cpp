#include <QGLWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include "math.h"

#include "net/net_radio_send_thread.h"
#include "net/net_vision_receive_thread.h"
#include "net/net_vision_send_thread.h"

#include "field_global_function.h"


#include "user_interface/field_related/field_view.h"
#include "field_ball.h"
#include "field_robot.h"
#include "field_scene.h"

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor.h"
#include "user_interface/strategy_control_window.h"

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"

#include "user_interface/field_related/paint_tools/paint_buffer.h"
#include "net/thread_tools/field_timer.h"

#include <math.h>



#include <QGraphicsScene>
#include <QGLWidget>
#include <QWidget>
#include <QMutex>
#include <QWheelEvent>
#include <QDebug>


QMutex draw_mutex;
QMutex display_update_mutex;

#define LU_TEST

FieldView::FieldView(QWidget* parent):QGraphicsView(parent)
{
    gl_widget = new QGLWidget ( QGLFormat ( QGL::SampleBuffers ) );
    this->setViewport (gl_widget );
    this->LoadFieldGeometry();
    this->scene = new FieldScene();
    this->setScene (scene);
    this->scene->setBackgroundBrush ( QBrush ( QColor ( 0,0x91,0x19,255 ),Qt::SolidPattern ) );
    this->scene->setSceneRect ( -3275,-2275,6550,4550 );
    this->ConstructField();


    field_brush = new QBrush ( Qt::NoBrush );
    field_line_pen = new QPen();
    field_line_pen->setColor ( Qt::white );
    field_line_pen->setWidth ( 10 );
    field_line_pen->setJoinStyle ( Qt::MiterJoin );
    field_item = scene->addPath ( *field_painter,*field_line_pen,*field_brush );
    field_item->setZValue(0);

    this->setRenderHint ( QPainter::Antialiasing, true );
    this->setRenderHint ( QPainter::HighQualityAntialiasing, true );
    this->setDragMode ( QGraphicsView::ScrollHandDrag );

    this->scale(-0.1,0.1);
    this->rotate(-90);
    this->shutdownfield_view = false;


}

FieldView::~FieldView()
{

}

void FieldView::tickslot ()
{
    tick();
}

void FieldView::tick ()
{
    if ( shutdownfield_view )
    {
        return;
    }
    UpdateViewScale();

    UpdateRobotsDisplay(); //Lu_test


    if ( !draw_mutex.tryLock() )
        return;
    this->viewport()->update();
    draw_mutex.unlock();
}

void FieldView::LoadFieldGeometry()
{
    this->line_width_ = FieldConstantsRoboCup2012::line_width_;
    this->field_length_ = FieldConstantsRoboCup2012::field_length_;
    this->field_width_ = FieldConstantsRoboCup2012::field_width_;
    this->boundary_width_ = FieldConstantsRoboCup2012::boundary_width_;
    this->referee_width_ = FieldConstantsRoboCup2012::referee_width_;
    this->goal_width_ = FieldConstantsRoboCup2012::goal_width_;
    this->goal_depth_ = FieldConstantsRoboCup2012::goal_depth_;
    this->goal_wall_width_ = FieldConstantsRoboCup2012::goal_wall_width_;
    this->center_circle_radius_ = FieldConstantsRoboCup2012::center_circle_radius_;
    this->defense_radius_ = FieldConstantsRoboCup2012::defense_radius_;
    this->defense_stretch_ = FieldConstantsRoboCup2012::defense_stretch_;
    this->free_kick_from_defense_dist_ = FieldConstantsRoboCup2012::free_kick_from_defense_dist_;
    this->penalty_spot_from_field_line_dist_ = FieldConstantsRoboCup2012::penalty_spot_from_field_line_dist_;
    this->penalty_line_from_spot_dist_ = FieldConstantsRoboCup2012::penalty_line_from_spot_dist_;
}

void FieldView::ConstructField()
{
    field_painter = new QPainterPath();
    //画中线
    field_painter->moveTo ( 0,-this->field_width_/2 );
    field_painter->lineTo ( 0,this->field_width_/2 );
    //画中心圆
    field_painter->addEllipse ( -this->center_circle_radius_,-this->center_circle_radius_,\
                                2*this->center_circle_radius_,2*this->center_circle_radius_ );
    //球场边界
    field_painter->moveTo ( this->field_length_/2,-this->field_width_/2 );
    field_painter->lineTo ( this->field_length_/2,this->field_width_/2 );
    field_painter->moveTo ( -this->field_length_/2,-this->field_width_/2 );
    field_painter->lineTo ( -this->field_length_/2,this->field_width_/2 );
    field_painter->moveTo ( -this->field_length_/2,-this->field_width_/2 );
    field_painter->lineTo ( this->field_length_/2,-this->field_width_/2 );
    field_painter->moveTo ( -this->field_length_/2,this->field_width_/2 );
    field_painter->lineTo ( this->field_length_/2,this->field_width_/2 );
    //球围栏
    field_painter->moveTo (  this->field_length_/2+this->boundary_width_,-this->field_width_/2-this->boundary_width_ );
    field_painter->lineTo (  this->field_length_/2+this->boundary_width_, this->field_width_/2+this->boundary_width_ );
    field_painter->moveTo ( -this->field_length_/2-this->boundary_width_,-this->field_width_/2-this->boundary_width_ );
    field_painter->lineTo ( -this->field_length_/2-this->boundary_width_, this->field_width_/2+this->boundary_width_ );
    field_painter->moveTo ( -this->field_length_/2-this->boundary_width_,-this->field_width_/2-this->boundary_width_ );
    field_painter->lineTo (  this->field_length_/2+this->boundary_width_,-this->field_width_/2-this->boundary_width_ );
    field_painter->moveTo ( -this->field_length_/2-this->boundary_width_, this->field_width_/2+this->boundary_width_ );
    field_painter->lineTo (  this->field_length_/2+this->boundary_width_, this->field_width_/2+this->boundary_width_ );
    //球门
    field_painter->moveTo ( this->field_length_/2,this->goal_width_/2 );
    field_painter->lineTo ( ( this->field_length_/2+goal_depth_ ),this->goal_width_/2 );
    field_painter->lineTo ( ( this->field_length_/2+goal_depth_ ),-this->goal_width_/2 );
    field_painter->lineTo ( this->field_length_/2,-this->goal_width_/2 );
    field_painter->moveTo ( ( this->field_length_/2-this->defense_radius_ ),this->defense_stretch_/2 );
    field_painter->lineTo ( ( this->field_length_/2-this->defense_radius_ ),-this->defense_stretch_/2 );
    field_painter->moveTo ( ( this->field_length_/2-this->defense_radius_ ),this->defense_stretch_/2 );
    field_painter->arcTo ( ( this->field_length_/2-this->defense_radius_ ),- ( this->defense_radius_-this->defense_stretch_/2 ),this->defense_radius_*2,this->defense_radius_*2,180,90 );
    field_painter->moveTo ( ( this->field_length_/2-this->defense_radius_ ),-this->defense_stretch_/2 );
    field_painter->arcTo ( ( this->field_length_/2-this->defense_radius_ ),- ( this->defense_radius_+this->defense_stretch_/2 ),this->defense_radius_*2,this->defense_radius_*2,180,-90 );
    field_painter->moveTo ( -this->field_length_/2,this->goal_width_/2 );
    field_painter->lineTo ( - ( this->field_length_/2+goal_depth_ ),this->goal_width_/2 );
    field_painter->lineTo ( - ( this->field_length_/2+goal_depth_ ),-this->goal_width_/2 );
    field_painter->lineTo ( -this->field_length_/2,-this->goal_width_/2 );
    field_painter->moveTo ( - ( this->field_length_/2-this->defense_radius_ ),this->defense_stretch_/2 );
    field_painter->lineTo ( - ( this->field_length_/2-this->defense_radius_ ),-this->defense_stretch_/2 );
    field_painter->moveTo ( - ( this->field_length_/2-this->defense_radius_ ),this->defense_stretch_/2 );
    field_painter->arcTo ( - ( this->field_length_/2+this->defense_radius_ ),- ( this->defense_radius_-this->defense_stretch_/2 ),this->defense_radius_*2,this->defense_radius_*2,0,-90 );
    field_painter->moveTo ( - ( this->field_length_/2-this->defense_radius_ ),-this->defense_stretch_/2 );
    field_painter->arcTo ( - ( this->field_length_/2+this->defense_radius_ ),- ( this->defense_radius_+this->defense_stretch_/2 ),this->defense_radius_*2,this->defense_radius_*2,0,90 );
}

void FieldView::ScaleView ( qreal scale_factor )
{
    qreal factor = matrix().scale ( scale_factor, scale_factor ).mapRect ( QRectF ( 0, 0, 1, 1 ) ).width();
    if ( factor > 0.07 && factor < 100.0 )
    {
        draw_scale = factor;
    }
}


void FieldView::UpdateViewScale()
{
    //重新设定放大系数
    if ( this->scaling_requested )
    {
        qreal factor = matrix().scale ( draw_scale, draw_scale ).mapRect ( QRectF ( 0, 0, 1, 1 ) ).width();
        if ( factor > 0.07 && factor < 100.0 )
            scale ( draw_scale, draw_scale );
        scaling_requested = false;
    }
}


void FieldView::UpdateView()
{
    if ( shutdownfield_view )
    {
        return;
    }
    if ( !draw_mutex.tryLock() )
        return;
    this->viewport()->update();
    draw_mutex.unlock();
}

/*
This function define the usage of event caused by wheel button
now it only deals with the scaling.
*/



void FieldView::wheelEvent ( QWheelEvent *event )
{
    int i;
    int itype;
    FieldItem* pItem;
    RobotItem* p_robot;
    int id,teamID;
    QList<QGraphicsItem *> list;
    list=scene->selectedItems ();
    if(list.size()==0)
    {
        draw_scale = pow ( 2.0, event->delta() / 540.0 );
        scaling_requested = true;
    }
    else
    {
        draw_mutex.lock();
        for(i=0;i<list.size();i++)
        {
            itype=(list.at(i))->type();
            if(itype!=QGraphicsItem::UserType+15)continue;
            pItem=(FieldItem*)(list.at(i));
            if(pItem->ItemType()==FieldItem::ROBOT_ITEM_TYPE)
            {
                p_robot=(RobotItem*)pItem;
                id=p_robot->id;
                teamID=p_robot->teamID;
                conf=p_robot->location_possibility;
                draw_mutex.unlock();
                m_Simulator.lock();
                if(teamID==teamBlue)
                {
                    if(m_Simulator.blue_robots[id].pos.dir<M_PI*2)
                    {
                        m_Simulator.blue_robots[id].pos.dir+=event->delta() / 1440.0;
                        if(m_Simulator.blue_robots[id].pos.dir>=M_PI*2)m_Simulator.blue_robots[id].pos.dir-=M_PI*2;
                        if(m_Simulator.blue_robots[id].pos.dir<=0     )m_Simulator.blue_robots[id].pos.dir+=M_PI*2;
                    }
                }
                else if(teamID==teamYellow)
                {
                    if(m_Simulator.yellow_robots[id].pos.dir<M_PI*2)
                    {
                        m_Simulator.yellow_robots[id].pos.dir+=event->delta() / 1440.0;
                        if(m_Simulator.yellow_robots[id].pos.dir>=M_PI*2)m_Simulator.yellow_robots[id].pos.dir-=M_PI*2;
                        if(m_Simulator.yellow_robots[id].pos.dir<=0     )m_Simulator.yellow_robots[id].pos.dir+=M_PI*2;
                    }
                }
                m_Simulator.unlock();
                draw_mutex.lock();
                viewport()->update();
                break;
            }
        }
        draw_mutex.unlock();
    }
    event->ignore();
}


void FieldView::BuildRobotAndBall()
{
    int i,j;
    BallItem* pBall;
    bool ball_at_top;
    //-------------------------------------------------------------------
    //为每个摄像机建立一个球队列
    QVector<BallItem*> tmp;
    while (balls_.size()<MAX_CAMERA_COUNT)
    {
        balls_.append(tmp);
    }
    //-------------------------------------------------------------------
    //加入所有球
    scene->AddBall();
    ball_at_top=true;
    for (i=0; i<MAX_CAMERA_COUNT; i++)
    {
        for (j=0; j<MAX_BALLS; j++)
        {
            pBall=scene->AddBall ();
            if( ball_at_top)
            {
                pBall->SetLocationPossibility(1.0);
                ball_at_top=false;
            }
            else
            {
                pBall->SetLocationPossibility(0.0);
            }
            pBall->setZValue(3);
            balls_[i].append ( pBall );
        }
    }
    //加入各种机器人
    RobotItem* p_robot;
    int l=0;
    for (i=0; i<MAX_CAMERA_COUNT; i++)
    {
        for (j=0; j<MAX_ROBOTS; j++)
        {
            p_robot=new RobotItem ( 100*l,0,0,teamBlue,l,0,0 );
#ifdef LU_TEST
            p_robot->SetLocationPossibility(1.0);
#endif
            AddRobot ( p_robot );
            p_robot=new RobotItem ( 0,100*l,0,teamYellow,l,0,0 );
#ifdef LU_TEST
            p_robot->SetLocationPossibility(1.0);
#endif
            AddRobot ( p_robot );

            l++;
        }
    }

}


void FieldView::AddBall ( BallItem *ball,int cameraID )
{
    //-------------------------------------------------------------------
    //为每个摄像机建立一个球队列
    QVector<BallItem*> tmp;
    while (cameraID+1>balls_.size())
    {
        balls_.append(tmp);
    }
    //-------------------------------------------------------------------
    balls_[cameraID].append ( ball );
    scene->addItem ( ball );
    ball->setZValue(3);
}

void FieldView::SetRobot ( int team,int id,bool bVisible )
{
    int i;
    m_Simulator.lock();
    for ( i=0; i<robots.size(); i++ )
    {
        if ( (robots[i]->get_teamID()==team) && (robots[i]->id==id) && (id<MAX_TEAM_ROBOTS) )
        {
            if (team==teamBlue)
            {
                m_Simulator.blue_robots[id].pos.p.set(-200,(id-2)*180);
                m_Simulator.blue_robots[id].pos.dir=0;
                m_Simulator.blue_robots[id].vcmd.v.set(0,0);
                m_Simulator.blue_robots[id].vcmd.va=0;
                m_Simulator.blue_robots[id].vel.v.set(0,0);
                m_Simulator.blue_robots[id].vel.va=0;
            }
            else
            {
                m_Simulator.yellow_robots[id].pos.p.set(200,-(id-2)*180);
                m_Simulator.yellow_robots[id].pos.dir=M_PI;
                m_Simulator.yellow_robots[id].vcmd.v.set(0,0);
                m_Simulator.yellow_robots[id].vcmd.va=0;
                m_Simulator.yellow_robots[id].vel.v.set(0,0);
                m_Simulator.yellow_robots[id].vel.va=0;
            }
            if (bVisible)
            {
                if (team==teamBlue)
                {
                    m_Simulator.blue_robots[id].conf=1.0;
                    m_Simulator.num_blue ++;
                }
                else
                {
                    m_Simulator.yellow_robots[id].conf=1.0;
                    m_Simulator.num_yellow ++;
                }
            }
            else
            {
                if (team==teamBlue)
                {
                    m_Simulator.blue_robots[id].conf=0.0;
                    m_Simulator.num_blue --;
                }
                else
                {
                    m_Simulator.yellow_robots[id].conf=0.0;
                    m_Simulator.num_yellow --;
                }
            }
            m_Simulator.unlock();
            return;
        }
    }
    m_Simulator.unlock();
}


void FieldView::AddRobot ( RobotItem *robot )
{
    robots.append ( robot );
    scene->addItem ( robot );
}


void FieldView::Initilize()
{
    BuildRobotAndBall();
}
static VisionInfo VInfo0;  // vision info

void FieldView::UpdateRobotsDisplay ( )
{


    //更新机器人和球信息
    QList<QGraphicsItem *> items;
    BallItem* pball;
    RobotItem* p_robot;
    FieldItem* gitem;
    MyVector2d pos;
    int itype;
    //根据视觉接收结果更新显示
    display_update_mutex.lock();
    VInfo0 = vision_info;
    display_update_mutex.unlock();


    int iBallIndex=0;
    items=scene->items();


    for (int i=0; i<items.size(); i++)
    {
        itype=(items.at(i))->type();
        if (itype!=QGraphicsItem::UserType+15)continue;

        gitem=(FieldItem*)(items.at(i));
        switch (gitem->ItemType())
        {
        case FieldItem::BALL_ITEM_TYPE:
            pball=(BallItem*)gitem;
            pos=VInfo0.Balls[iBallIndex].pos;
            if (iBallIndex<MAX_BALLS)
            {
                pball->SetLocationPossibility(VInfo0.Balls[iBallIndex].conf);
                //pball->set_area ( 20 );
                pball->setPos(pos.x,pos.y);
                //pball->set_z(1);
            }
            else
            {
                pball->SetLocationPossibility(0);
            }
            iBallIndex++;
            break;
        default:
            p_robot=(RobotItem*)gitem;
            if (p_robot->id<MAX_ROBOTS)
            {
                p_robot->SetLocationPossibility(VInfo0.Robots[p_robot->teamID][p_robot->id].conf);

                if (p_robot->location_possibility!=0.0)
                {
                    pos=VInfo0.Robots[p_robot->teamID][p_robot->id].pos;
                    p_robot->orientation=VInfo0.Robots[p_robot->teamID][p_robot->id].orientation;
                    p_robot->setPos(pos.x,pos.y);
                    p_robot->SetTactic(VInfo0.Robots[p_robot->teamID][p_robot->id].Tactic);
                }
            }
            else
            {
                p_robot->SetLocationPossibility(0);
            }
            break;
        }
    }


}

void FieldView::drawForeground ( QPainter * painter, const QRectF & rect )
{
    GuiCmd.GuiSwitchCmds();
    GuiCmd.ExecCmds(painter);
    QFont drawFont = QFont ( "Courier",80,2,false );
    QPen pen=QPen(Qt::yellow);
    painter->setFont ( drawFont );
    painter->setPen(pen);
    painter->scale(1,-1);
    painter->drawText(-300,2150,VInfo0.sPlay);
    painter->scale(1,-1);
}
