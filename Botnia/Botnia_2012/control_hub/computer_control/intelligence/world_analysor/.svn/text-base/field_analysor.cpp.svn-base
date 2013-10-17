#include "field_analysor.h"

/*
 * TITLE:  Simulator.cc
 *
 * PURPOSE:  This file implements the small size robot simulator classes.
 *
 * WRITTEN BY:  Scott Lenser, Michael Bowling, Brett Browning
 */
/* LICENSE:
  =========================================================================
    CMDragons'02 RoboCup F180 Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
                       James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "user_interface/field_related/field_constants.h"
//#include <lineseg.h>
#include "user_interface/field_related/motion_simulation_tools/field_util.h"
#include "field_analysor_configreader.h"
#include "field_analysor.h"
//#include "commands.h"
//#include "RadioReceiveThread.h"
#include "user_interface/field_related/field_view.h"
#include "field_analysor_parametres.h"
#include "net/thread_tools/field_timer.h"

//#define DEBUG
//#define USE_VQUEUE

// temp
#define FSGN(a)    (((a) >= 0) ? 1 : -1)

/**************************** TYPES ******************************************/
/* polygons for different robot types
 * -  must be in clockwise order
 * - first side has kicker
 * - TYPE none is a circle and is not a polygon
 */
static const double field_minX = -FIELD_LENGTH_H-FieldConstantsRoboCup2012::boundary_width_;
static const double field_maxX = +FIELD_LENGTH_H+FieldConstantsRoboCup2012::boundary_width_;
static const double field_minY = -FIELD_WIDTH_H-FieldConstantsRoboCup2012::boundary_width_;
static const double field_maxY = +FIELD_WIDTH_H+FieldConstantsRoboCup2012::boundary_width_;


// config parameters
// ball parameters
CR_DECLARE(BALL_CARPET_FRICTION_COEF)
CR_DECLARE(WALL_PERP_BOUNCE_DECAY)

// diffbot parameters
CR_DECLARE(DIFFBOT_POLY_X)
CR_DECLARE(DIFFBOT_POLY_Y)
CR_DECLARE(DIFFBOT_NRPOINTS)
CR_DECLARE(DIFFBOT_WHEELBASE)
CR_DECLARE(DIFFBOT_MAX_SPEED)
CR_DECLARE(DIFFBOT_MAX_ANG_VEL)
CR_DECLARE(DIFFBOT_MAX_ACC)
CR_DECLARE(DIFFBOT_MAX_ANG_ACC)
CR_DECLARE(DIFFBOT_PERP_BOUNCE_DECAY)
CR_DECLARE(DIFFBOT_KICKER_SPEED)
CR_DECLARE(DIFFBOT_DRIB_CATCH_SPEED)
CR_DECLARE(DIFFBOT_DRIB_BOUNCE_DECAY)

// omnibot parameters
CR_DECLARE(OMNIBOT_POLY_X)
CR_DECLARE(OMNIBOT_POLY_Y)
CR_DECLARE(OMNIBOT_NRPOINTS)
CR_DECLARE(OMNIBOT_WHEELBASE)
CR_DECLARE(OMNIBOT_MAX_SPEED)
CR_DECLARE(OMNIBOT_MAX_ANG_SPEED)
CR_DECLARE(OMNIBOT_MAX_ACC)
CR_DECLARE(OMNIBOT_MAX_ANG_ACC)
CR_DECLARE(OMNIBOT_PERP_BOUNCE_DECAY)
CR_DECLARE(OMNIBOT_KICKER_SPEED)
CR_DECLARE(OMNIBOT_DRIB_CATCH_SPEED)
CR_DECLARE(OMNIBOT_DRIB_BOUNCE_DECAY)

/**************************** GLOBALS ****************************************/

Simulator m_Simulator;

/***************************** ROBOTSIMULATOR CLASS ****************************/

/*
 * rand_posvector -
 *
 * function to calculate the random position vector on the field with a normal
 * distribution
 */
//获得一个随机位置向量
MyVector2d Simulator::rand_posvector(void)
{
    double x,y;
    x=(rand() * (maxX - minX)) / RAND_MAX + minX;
    y=(rand() * (maxY - minY)) / RAND_MAX + minY;
    return MyVector2d(x,y);
}

//获得一个随机方向
double Simulator::rand_heading(void)
{
    return (((double) rand() * M_PI * 2.0) / ((double) RAND_MAX));
}

/*
 * SetPolygon -
 *
 * Creates a new polygon class for the specified robot type
 */
//根据机器人类型设定机器人外形

//限制机器人加速度
void Simulator::AccelerationLimit(SimRobot &sr, double maxdv)
{
#if 0
    double a = sr.vel.v.angle() - sr.pos.dir;
    double vmag = sr.vel.v.length();

    MyVector2d oldv(vmag * cos(a), vmag * sin(a));
    MyVector2d dv = sr.vel.v - oldv;
    double dvmag = dv.length();

    if (dvmag > maxdv)
        sr.vel.v = oldv + dv * (maxdv / dvmag);
#else
    MyVector2d curv = sr.vel.v.rotate(-sr.pos.dir);
    MyVector2d dv = sr.vcmd.v - curv;

    if (dv.length() > maxdv)
    {
        sr.vcmd.v = curv + dv.norm() * maxdv;
    }
#endif
}

//限制机器人速度
void Simulator::SpeedLimit(SimRobot &sr, double maxv)
{
    // Check maximum speed
    double vmag = sr.vel.v.length();
    if (vmag > maxv)
    {
        sr.vel.v *= maxv / vmag;
    }
}

//撞墙处理
bool Simulator::CheckWallCollision(SimRobot &r, double minx, double maxx,
                                   double miny, double maxy)
{
    bool out_x_min,out_x_max,out_y_min,out_y_max;
    bool collision;

    out_x_min = (r.pos.p.x < minx);
    out_x_max = (r.pos.p.x > maxx);
    out_y_min = (r.pos.p.y < miny);
    out_y_max = (r.pos.p.y > maxy);

    if (out_x_min)
    {
        r.pos.p.x = minx;
        if(fabs(r.vel.v.x) > fabs(r.vel.v.y))
        {
            r.pos.p.y = r.oldpos.p.y;
        }
    }
    else if (out_x_max)
    {
        r.pos.p.x = maxx;
        if(fabs(r.vel.v.x) > fabs(r.vel.v.y))
        {
            r.pos.p.y = r.oldpos.p.y;
        }
    }

    if (out_y_min)
    {
        r.pos.p.y = miny;
        if (!(out_x_min || out_x_max))
        {
            if (fabs(r.vel.v.y) > fabs(r.vel.v.x))
            {
                r.pos.p.x = r.oldpos.p.x;
            }
        }
    }
    else if (out_y_max)
    {
        r.pos.p.y = maxy;
        if (!(out_x_min || out_x_max))
        {
            if(fabs(r.vel.v.y) > fabs(r.vel.v.x))
            {
                r.pos.p.x = r.oldpos.p.x;
            }
        }
    }

    collision = (out_x_min || out_x_max || out_y_min || out_y_max);
    if (collision)
    {
        //如果发生冲撞,将速度设定为0,保持上一个方向
        r.vel.v.set(0.0, 0.0);
        r.pos.dir = r.oldpos.dir;
    }

    return (collision);
}


//dt 时间变化量
long iii=0;
void Simulator::UpdateRobot(SimRobot &r, double dt)
{
    if(r.conf==0.0)
    {
        return;
    }
    /* do an acceleration check */
    AccelerationLimit(r, maxRobotAccel * dt);

    /* Speed Limit */
    SpeedLimit(r, maxRobotSpeed);

    /* store off old velocities and positions */
    r.oldpos = r.pos;

    /* transform to world coords */
    r.vel.v = r.vcmd.v;
    r.vel.v = r.vel.v.rotate(r.pos.dir);

    r.pos.p += r.vel.v * dt;
    r.pos.dir = anglemod(r.pos.dir + r.vcmd.va * dt);

    /* look for wall collisions */
    r.collision = CheckWallCollision(r,
                                     field_minX,
                                     field_maxX,
                                     field_minY,
                                     field_maxY);
}

//检查球是否被机器人捕捉
//通过多边形判断
//bool Simulator::RobotBallCollisionPolygon(SimRobot &r, SimBall &b, double dt)
//{
//	double dist = (r.pos.p - b->get_pos()).length();
//  LineSeg result;
//  int line_id = -1;
//  LineSeg test_seg(b->oldpos, b->get_pos());
//
//  /* check if we need to do anything at all */
//  if (dist > robotHalfSize + BALL_RADIUS * 2.0)
//    return (false);
//
//  /* translate/rotate the edge polygon */
//  r.polygon.RotateTranslate(r.pos.dir, r.pos.p);
//
//  /* test for collisisions between the old position and the new */
//  if (!r.polygon.GetIntersectingLine(test_seg, result, &line_id)) {
//
//    /* if the b is inside the robot then we need to check if we hit it */
//    if (!r.polygon.IsInside(b.pos))
//      return (false);
//
//    double dist = (r.pos.p - r.oldpos.p).length();
//    double angle = (r.pos.p - r.oldpos.p).angle();
//
//    /* did we instead drive through the b ? */
//    if (!r.polygon.GetIntersectingLineSweep(b->get_pos(), dist, angle, result, &line_id)) {
//      double angle = r.pos.dir;
//      angle += M_PI_2;
//
//      MyVector2d p1 = MyVector2d(cos(angle), sin(angle)) * 200.0;
//      result = LineSeg(r.pos.p + p1, r.pos.p - p1);
//    }
//  }
//
//  // work out if we are kicking the ball
//  // is it on our kicker?
//  MyVector2d kvel(0.0, 0.0);
//  if (line_id == 0) {
//    if (r.vcmd.kick) {
//      if (r.rtype == ROBOT_TYPE_DIFF)
//				kvel.x = DVAR(DIFFBOT_KICKER_SPEED);
//      else
//				kvel.x = DVAR(OMNIBOT_KICKER_SPEED);
//      kvel.rotate(r.pos.dir);
//      printf("Kick!!!!...\n");
//    } else if (r.vcmd.drib) {
//      MyVector2d brel = ball->vel - r.vel.v;
//      MyVector2d n(1.0,0);
//      n = n.rotate(r.pos.dir);
//      MyVector2d p = n.perp();
//
//      MyVector2d bdrib(dot(brel, n), dot(brel, p));
//      double dlimit;
//      dlimit = (r.rtype ? DVAR(DIFFBOT_DRIB_CATCH_SPEED) :
//			DVAR(OMNIBOT_DRIB_CATCH_SPEED));
//      printf("drib %f %f\n", bdrib.x, bdrib.y);
//
//      if (bdrib.x < 0) {
//				ball->vel = bdrib;
//				if (fabs(bdrib.x) < dlimit) {
//					ball->vel.x = 0;
//				} else {
//					ball->vel.x *= (r.rtype ? DVAR(DIFFBOT_DRIB_BOUNCE_DECAY) :
//						 DVAR(OMNIBOT_DRIB_BOUNCE_DECAY));
//				}
//				ball->vel = ball->vel.rotate(r.pos.dir + M_PI_2);
//				ball->vel += r.vel.v;
//      }
//    }
//  }
//
//  /* calculate a line segment representing relative vector from origin */
//  LineSeg seg(MyVector2d(0.0,0.0), b->vel - r.vel.v - kvel);
//  seg.Reflect(result);
//  b->vel = seg.LineVector() * robotPerpBounceDecay + r.vel.v;
//
//  // need this to avoid oscilation around the edge of the robot
//  b->pos = b->oldpos + b.vel * dt;
//
//  // this bit of code was broken ???
//  /* last final catch all check to prevent screw ups */
//  //  if (r.polygon.IsInside(b.pos)) {
//  //    fprintf(stderr, "Moving ball outside...\n");
//  //    r.polygon.MoveOutside(b.pos);
//  //  }
//
//  return (true);
//}

//检查球是否被机器人捕捉
//通过半径判断
bool Simulator::RobotBallCollisionRadius(SimRobot &r, SimBall &b, double dt)
{
    static int iDribbleBy=-1;
    //球相对机器人坐标
    MyVector2d ball_rel  = b.pos - r.pos.p;
    //球相对机器人矢量旋转到X轴方向
    MyVector2d robot_ball = ball_rel.rotate(-r.pos.dir);
    //球在机器人内部
    //bool ball_in_front = (robot_ball.x > -20) &&
    //                  (fabs(robot_ball.y) < 60);
    //球在机器人前方
    //bool ball_on_front = ball_in_front && (robot_ball.x < 120);

    double dist = ball_rel.length();
    //计算球相对机器人方向与机器人正方向的角度差
    double da=anglemod(ball_rel.angle()-r.pos.dir);
    if(da>M_PI)
    {
        da-=M_2PI;
    }
    if(da<-M_PI)
    {
        da+=M_2PI;
    }

    //检查球与机器人是否碰撞
    if (dist > robotHalfSize + BALL_RADIUS)
    {
        return (false);
    }
    bool bBounce=false;

    //if(ball_on_front)
    if(fabs(da)<M_PI/4)
    {
        if (r.vcmd.kick_power)
        {
            long kick_power=labs(r.vcmd.kick_power);
            if(kick_power>giShootPowerMax)
            {
                kick_power=giShootPowerMax;
            }
            b.vel.set(DVAR(OMNIBOT_KICKER_SPEED)*kick_power*1.0/giShootPowerMax,0);
            b.vel=b.vel.rotate(r.pos.dir);
            printf("Kicked=%d\r\n",r.vcmd.kick_power);

            b.pos.set(OMNIBOT_RADIUS+BALL_RADIUS+10,0);
            b.pos=b.pos.rotate(r.pos.dir);
            b.pos+= r.pos.p;
            return true;
        }
        else
        {
            //计算球相对机器人速度
            MyVector2d brel = b.vel - r.vel.v;
            //计算相对速度在机器人正方向上投影
            MyVector2d n(1,0);
            n = n.rotate(r.pos.dir);
            MyVector2d p = n.perp();
            MyVector2d bdrib(dot(brel, n), dot(brel, p));

            MyVector2d bpos=b.pos-r.pos.p;
            double bposx=dot(bpos, n);
            double bposy=dot(bpos, p);
            if(bposx<OMNIBOT_RADIUS-5)
            {
                b.vel.set(0,0);
                if(r.vcmd.dribble_power>0)
                {
                    b.pos.set(OMNIBOT_RADIUS-18,0);
                }
                else
                {
                    b.pos.set(OMNIBOT_RADIUS-18,bposy);

                }
                b.pos=b.pos.rotate(r.pos.dir);
                b.pos+= r.pos.p;
                //iDribbleBy=r.id;
            }
            //else if(bposx<OMNIBOT_RADIUS-5)
            //{
            //	b.pos.set(OMNIBOT_RADIUS-18,0);
            //	b.pos=b.pos.rotate(r.pos.dir);
            //	b.pos+= r.pos.p;
            //}
            //if(r.vcmd.dribble_power>0 &&


            //if (bposx<OMNIBOT_RADIUS-5 && (bdrib.x < 0.001 || (r.vcmd.dribble_power>0) || iDribbleBy>=0) )
            //{
            //	//printf("Dribble!!!!... %4.3f,%4.3f\n",bdrib.x,bdrib.y);
            //	double dlimit;
            //	dlimit = DVAR(OMNIBOT_DRIB_CATCH_SPEED)*3;

            //	if (fabs(bdrib.x) < dlimit)
            //	{
            //		b.vel.x = 0;
            //	}
            //	else
            //	{
            //	//	b.vel.x *= DVAR(OMNIBOT_DRIB_BOUNCE_DECAY);
            //	}

            //	//b.vel = r.vel.v.rotate(r.pos.dir);
            //	//b.vel += r.vel.v;
            //	b.vel.set(0,0);
            //	b.pos.set(OMNIBOT_RADIUS-18,0);
            //	b.pos=b.pos.rotate(r.pos.dir);
            //	b.pos+= r.pos.p;
            //	iDribbleBy=r.id;
            //}
            //else
            //{
            //	iDribbleBy=-1;
            //}
            return true;
        }
    }
    else
    {
        iDribbleBy=-1;
        bBounce=true;
    }
    if(!bBounce)
    {
        return true;
    }
    /* last final catch all check to prevent screw ups */
    //如果机器人与球相撞，球被弹开到机器人半径加球半径的位置
    b.pos = r.pos.p + ball_rel.norm() * (robotHalfSize + BALL_RADIUS);
    b.vel=r.vel.v.rotate(r.pos.dir)-b.vel;

    b.pos = b.pos + b.vel*dt;

    /* find the closest point on teh line segment to our robot
   * center and check if it is close enough. If so we hit the ball
   */
    //  MyVector2d closest;
    //	dist = ball_seg.Distance(r.pos, closest);
    //	if (dist > robotHalfSize + BALL_RADIUS)
    //		return (false);

    /* TO DO: need to work this part out still */


    return (true);
}

/*
 * Initialize -
 *
 * This function initialises the simulator variables and links for
 * the main program
 *
 * RETURN VALUE: TRUe on success, FALSE on failure
 */
//仿真器初始化
bool Simulator::Initialize(bool _usegoals, bool _usenoise)
{
    lock();
    LoadConfig();

    //使用随机数初始化球
    // Ball
    ball.pos=rand_posvector();
    ball.oldpos = ball.pos;
    ball.vel.set(0, 0);
    //ballUpdateParam(true);
    //    double mbs = maxBallSpeed / sqrt(2);
    //-------------------------------------------------------------
    /* intialise the robot coordinates etc */
    //使用随机数初始化机器人
    for (int i = 0; i < MAX_TEAM_ROBOTS; i++)
    {
        blue_robots[i].rtype = ROBOT_TYPE_NONE;
        SetRobotRandom(false, i);
        blue_robots[i].vcmd.v.set(0,0);
        blue_robots[i].vcmd.va=0;

        yellow_robots[i].rtype = ROBOT_TYPE_NONE;
        SetRobotRandom(true, i);
        yellow_robots[i].vcmd.v.set(0,0);
        yellow_robots[i].vcmd.va=0;
    }
    //-------------------------------------------------------------
    usegoals = _usegoals;
    usenoise = _usenoise;
    unlock();
    return (true);
}

/*
 * Simulator constructor -
 *
 * Constructor initialises the appropriate variables. It starts with the default
 * configuration.
 */
Simulator::Simulator()
//:	radio_s(0,NET_RADIO_PROTOCOL, NET_RADIO_ACK_PERIOD)
{
    /* initialise the random seed */
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    srand(GetTickCount());

    running_state = RUNSTATE_PLAY;

    // goal depth on screen appears to be short..so make a little short to make sure
    // we can get the ball back
    field_max.set(FIELD_LENGTH_H + FieldConstantsRoboCup2012::boundary_width_-OMNIBOT_RADIUS
                  , FIELD_WIDTH_H + FieldConstantsRoboCup2012::boundary_width_ -OMNIBOT_RADIUS);

    // default number of robots
    num_yellow = num_blue = 0;
    //num_blue=1;

    frameCount = 0;
    goalscored = false;

    int i;
    for(i=0;i<MAX_TEAM_ROBOTS;i++)
    {
        blue_robots[i].conf = 0;
        yellow_robots[i].conf = 0;
    }
}

void Simulator::LoadConfig()
{
 //    read in the config parameters
      CR_SETUP(simulator, BALL_CARPET_FRICTION_COEF, CR_DOUBLE);
      CR_SETUP(simulator, WALL_PERP_BOUNCE_DECAY, CR_DOUBLE);

      // diffbot parameters
      CR_SETUP(simulator, DIFFBOT_POLY_X, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_POLY_Y, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_NRPOINTS, CR_INT);
      CR_SETUP(simulator, DIFFBOT_WHEELBASE, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_MAX_SPEED, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_MAX_ANG_VEL, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_MAX_ACC, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_MAX_ANG_ACC, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_PERP_BOUNCE_DECAY, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_KICKER_SPEED, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_DRIB_CATCH_SPEED, CR_DOUBLE);
      CR_SETUP(simulator, DIFFBOT_DRIB_BOUNCE_DECAY, CR_DOUBLE);

      // omnibot parameters
      CR_SETUP(simulator, OMNIBOT_POLY_X, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_POLY_Y, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_NRPOINTS, CR_INT);
      CR_SETUP(simulator, OMNIBOT_WHEELBASE, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_MAX_SPEED, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_MAX_ANG_SPEED, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_MAX_ACC, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_MAX_ANG_ACC, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_PERP_BOUNCE_DECAY, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_KICKER_SPEED, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_DRIB_CATCH_SPEED, CR_DOUBLE);
      CR_SETUP(simulator, OMNIBOT_DRIB_BOUNCE_DECAY, CR_DOUBLE);
    tracker.LoadConfig();
}

/*
 * ~Simulator -
 *
 * This deconstructor deallocates all the allocated main variables
 */
Simulator::~Simulator()
{
}

/*
 * AddRobot -
 *
 *( This function adds a robot to the specified position and orientation
 * it also sets the robot type
 *
 * RETURN VALUE: TRUE on success, false on failure
 */

//添加机器人
bool Simulator::AddRobot(bool yellow, int rtype, double x, double y, double a)
{
    if (yellow)
    {
        if (num_yellow >= MAX_TEAM_ROBOTS)
        {
            return false;
        }
        yellow_robots[num_yellow].rtype = rtype;
        SetRobot(yellow, num_yellow, MyVector2d(x, y), a, MyVector2d(0, 0), 0);

        /* all done so add numbe rof robots */
        num_yellow++;
    }
    else
    {
        if (num_blue >= MAX_TEAM_ROBOTS)
        {
            return false;
        }
        blue_robots[num_blue].rtype = rtype;
        SetRobot(yellow, num_blue, MyVector2d(x, y), a, MyVector2d(0, 0), 0);

        /* all done so add numbe rof robots */
        num_blue++;
    }
#ifdef DEBUG
    printf("Adding robot %i, ny %i, nb %i\n", yellow, num_yellow, num_blue);
#endif
    return (true);
}

/*
 * SetNumRobots -
 *
 * This function adds robots to the list (or deletes ones currently there) to
 * reach the desired number of bots
 */
//设定机器人数量,并自动添加和删除机器人对象
bool Simulator::SetNumRobots(bool yellow, int n)
{
    if ((n < 0) || (n > MAX_TEAM_ROBOTS))
    {
        return false;
    }

    if (yellow)
    {
        while (num_yellow < n)
        {
            AddRobot(yellow, ROBOT_TYPE_DIFF);
            SetRobotRandom(yellow, num_yellow - 1);
        }
        if (n < num_yellow)
        {
            num_yellow = n;
        }
    }
    else
    {
        while (num_blue < n)
        {
            AddRobot(yellow, ROBOT_TYPE_DIFF);
            SetRobotRandom(yellow, num_blue - 1);
        }
        if (n < num_blue)
        {
            num_blue = n;
        }
    }
    return (true);
}

/*
 * SetRobotCommand -
 *
 * This function sets the robot commands for the next time step
 */
void Simulator::SetRobotCommand(bool yellow, int robot, double vx, double vy, double va,
                                int kick_power, int dribble_power,bool bchipkick)
{
    commands_mutex.lock();
#ifdef USE_VQUEUE
    if (yellow)
    {
        yellow_robots[robot].vqueue.SetCommand(vx, vy, va, kick_power, dribble_power, bchipkick, frameCount);
    }
    else
    {
        blue_robots[robot].vqueue.SetCommand(vx, vy, va, kick_power, dribble_power, bchipkick, frameCount);
    }
#else
    if (yellow)
    {
        yellow_robots[robot].vcmd.v.set(vx, vy);
        yellow_robots[robot].vcmd.va = va;
        yellow_robots[robot].vcmd.kick_power = kick_power;
        yellow_robots[robot].vcmd.dribble_power = dribble_power;
    }
    else
    {
        blue_robots[robot].vcmd.v.set(vx, vy);
        blue_robots[robot].vcmd.va = va;
        blue_robots[robot].vcmd.kick_power = kick_power;
        blue_robots[robot].vcmd.dribble_power = dribble_power;
    }
#endif
    commands_mutex.unlock();
}

/*
 * StopAll -
 *
 * This command stops all the robots dead
 */
//在队列尾部追加停止命令,停止命令速度为0,禁止踢球和带球
void Simulator::StopAll(void)
{
    commands_mutex.lock();

    for(int i = 0; i < num_yellow; i++)
    {
        yellow_robots[i].vqueue.SetCommand(0, 0, 0, 0, 0, false, frameCount);
    }
    for(int i = 0; i < num_blue; i++)
    {
        blue_robots[i].vqueue.SetCommand(0, 0, 0, 0, 0, false, frameCount);
    }
    commands_mutex.unlock();
}

/*
 * Tick -
 *
 * This funciton is the guts of the simulator. It should be called on each simulation tick.
 * (Hence the name :-). The function calculates the new position for the robots and ball
 * based on their kinematics and dynamics. It then accounts for the collisions between
 * all the objects and the objects and the field.
 */
int Simulator::Tick(void)
{
    lock();
    double secs = timePerFrame;

    /* wait on the runstate mutex to prevent conflicts */
    runstate_mutex.lock();
    //处理暂停
    // If paused then still signal a new frame.
    if (running_state == RUNSTATE_PAUSE)
    {
        runstate_mutex.unlock();
        unlock();
        return frameCount;
    }
    //处理单步
    else if (running_state == RUNSTATE_STEPFORWARD)
    {
        running_state = RUNSTATE_PAUSE;
    }
    runstate_mutex.unlock();

    commands_mutex.lock();
    // turn of the goal score flag
    goalscored = 0;

    /* Update each team of Robots with their new velocities */
    for(int i = 0; i < MAX_TEAM_ROBOTS; i++)
    {
        if(yellow_robots[i].conf==0.0)continue;
#ifdef USE_VQUEUE
        //读取一条命令
        yellow_robots[i].vqueue.GetCommand(yellow_robots[i].vcmd, frameCount);
#endif
        UpdateRobot(yellow_robots[i], secs);
    }

    /* Update each team of Robots with their new velocities */
    for(int i = 0; i < MAX_TEAM_ROBOTS; i++)
    {
        if(blue_robots[i].conf==0.0)continue;
#ifdef USE_VQUEUE
        //读取一条命令
        blue_robots[i].vqueue.GetCommand(blue_robots[i].vcmd, frameCount);
#endif
        UpdateRobot(blue_robots[i], secs);
    }

    // Update Ball
    /* bound the ball off the wall */
    ball.oldpos = ball.pos;
    MyVector2d v = ball.vel;
    ////如果球出现范围超界,速度除以sqrt(2)
    //if ((ball.pos.x < minX) || (ball.pos.x > maxX))
    //{
    //  v.x *= M_SQRT1_2;
    //}
    //if ((ball.pos.y < minY-FieldConstantsRoboCup2012::boundary_width_) || (ball.pos.y > maxY+FieldConstantsRoboCup2012::boundary_width_))
    //{
    //  v.y *= M_SQRT1_2;
    //}
    ball.pos += ball.vel * secs;

    /* decay the ball velocity with friction */
    //计算摩擦力影响速度后的结果
    double dv_mag = ballCarpetFrictionCoef * GRAVITY * secs;
    double vmag = ball.vel.length();
    MyVector2d dv = -ball.vel;
    if (vmag > dv_mag)
    {
        dv *= dv_mag / vmag;
    }
    ball.vel += dv;

    // Check for Walls
    //
    // (mhb) Updated for 45' walls.
    //  See: http://dept.physics.upenn.edu/courses/gladney/
    //  mathphys/java/sect4/subsubsection4_1_4_3.html

    //球撞墙后反弹
    bool bOutSide=false;
    //上下出界
    if (fabs(ball.pos.y) > FIELD_WIDTH_H)
    {
        if (fabs(ball.pos.y) >= field_max.y)
        {
            ball.vel.set(0,0);
        }
        bOutSide=true;
        //ball.vel.y -= FSGN(ball.pos.y) * secs * M_SQRT1_2 * GRAVITY * 5.0/7.0;
    }
    //左右出界并且没有进入球门
    if ((fabs(ball.pos.x) > FIELD_WIDTH_H) && (fabs(ball.pos.y) > GOAL_WIDTH_H))
    {
        if((fabs(ball.pos.x) >= field_max.x))
        {
            ball.vel.set(0,0);
        }
        bOutSide=true;
        //ball.vel.x -= FSGN(ball.pos.x) * secs * M_SQRT1_2 * GRAVITY * 5.0/7.0;
    }
    //球进入门中的处理
    if ( fabs(ball.pos.x) > FIELD_LENGTH_H && fabs(ball.pos.y) <= GOAL_WIDTH_H)
    {
        if(fabs(ball.pos.x) >= FIELD_LENGTH_H+180.0)
        {
            ball.vel.set(0,0);
        }
        bOutSide=true;
    }

    static int OutSideDelay=120;
    if(bOutSide)
    {
        if(OutSideDelay==0)
        {
            //ball.pos.set(0,0);
        }
        else
        {
            OutSideDelay--;
        }
    }
    else
    {
        OutSideDelay=120;
    }

    /* now check for robot collisions with the ball
   * original code by S Lenser
   */
    //限制蓝队队员运动范围
    for(int i = 0; i < MAX_TEAM_ROBOTS; i++)
    {
        if(blue_robots[i].conf==0.0)continue;
        RobotBallCollisionRadius(blue_robots[i], ball, secs);
        // bound the ball position
        blue_robots[i].pos.p.x = bound(blue_robots[i].pos.p.x, -field_max.x, field_max.x);
        blue_robots[i].pos.p.y = bound(blue_robots[i].pos.p.y, -field_max.y, field_max.y);
    }

    //限制黄队队员运动范围
    for(int i = 0; i < MAX_TEAM_ROBOTS; i++)
    {
        if(yellow_robots[i].conf==0.0)continue;
        RobotBallCollisionRadius(yellow_robots[i], ball, secs);
        // bound the ball position
        yellow_robots[i].pos.p.x = bound(yellow_robots[i].pos.p.x, -field_max.x, field_max.x);
        yellow_robots[i].pos.p.y = bound(yellow_robots[i].pos.p.y, -field_max.y, field_max.y);
    }

    // bound the ball position
    //限制球的运动范围
    ball.pos.x = bound(ball.pos.x, -field_max.x, field_max.x);
    ball.pos.y = bound(ball.pos.y, -field_max.y, field_max.y);

    // check if the ball is in the goal
    //计分处理
    if (usegoals)
    {
        if ((fabs(ball.pos.x) > FIELD_LENGTH_H + BALL_RADIUS * 2.0) &&
                fabs(ball.pos.y) < GOAL_WIDTH_H)
        {

            goalscored = ((ball.pos.x > 0) ? 1 : -1);

            printf("Goal scored on goal %i!!!!\n", ball.pos.x > 0);
            ball.pos.set(0, 0);
            ball.vel.set(0, 0);
        }
    }

    // generate noise input on confidences
    //在可信度上叠加噪声
    if (usenoise) {
        // crappy for now
        ball.conf = (double) rand() / (double) RAND_MAX;
        for (int i = 0; i < num_yellow; i++)
        {
            yellow_robots[i].conf = (double) rand() / (double) RAND_MAX;
        }
        for (int i = 0; i < num_blue; i++)
        {
            blue_robots[i].conf = (double) rand() / (double) RAND_MAX;
        }
    }

    /* increment the ol frame counter */
    frameCount++;

    // take care of semaphores
    commands_mutex.unlock();

    //run
    //double timestamp = GetTimeSec();

    // get any new radio commands
    //接收无线电命令

    unlock();
    return frameCount;
}

/**************************** State manipulation variables *******************/

double Simulator::Time(void)
{
    return frameCount * timePerFrame;
}

int Simulator::Frame(void)
{
    return frameCount;
}

//获取机器人当前位置
MyVector2d Simulator::GetRobotPosition(bool yellow, int robot)
{
    return (IS_YELLOW(yellow, yellow_robots[robot].pos.p,
                      blue_robots[robot].pos.p));
}

//获取机器人当前方向
double Simulator::GetRobotDirection(bool yellow, int robot)
{
    return (IS_YELLOW(yellow, yellow_robots[robot].pos.dir,
                      blue_robots[robot].pos.dir));
}

//获取机器人当前速度
MyVector2d Simulator::GetRobotVelocity(bool yellow, int robot)
{
    return (IS_YELLOW(yellow, yellow_robots[robot].vel.v,
                      blue_robots[robot].vel.v));
}

//获取机器人当前角速度
double Simulator::GetRobotAngularVelocity(bool yellow, int robot)
{
    return (IS_YELLOW(yellow, yellow_robots[robot].vel.va,
                      blue_robots[robot].vel.va));
}

//获取机器人类型
int Simulator::GetRobotType(bool yellow, int robot)
{
    return (IS_YELLOW(yellow, yellow_robots[robot].rtype,
                      blue_robots[robot].rtype));
}

//获取机器人可信度
double Simulator::GetRobotConfidence(bool yellow, int robot)
{
    if (usenoise)
    {
        return (IS_YELLOW(yellow, yellow_robots[robot].conf,
                          blue_robots[robot].conf));
    }
    else
    {
        return (1.0);
    }
}

//获取球的位置
MyVector2d Simulator::GetBallPosition(void)
{
    return (ball.pos);
}

//获取球的速度
MyVector2d Simulator::GetBallVelocity(void)
{
    return (ball.vel);
}

//获取球的可信度
double Simulator::GetBallConfidence(void)
{
    if (usenoise)
    {
        return (ball.conf);
    }
    else
    {
        return (1.0);
    }
}

void Simulator::SetBall(MyVector2d p, MyVector2d v)
{
    ball.pos = p;
    ball.oldpos = p;
    ball.vel = v;
}

void Simulator::SetBallPosition(MyVector2d p)
{
    ball.pos = p;
    ball.oldpos = p;
}

void Simulator::SetBallVelocity(MyVector2d v)
{
    ball.vel = v;
}

void Simulator::StopBall(void)
{
    ball.vel.set(0, 0);
}

void Simulator::SetRobot(bool yellow, int robot, MyVector2d p, double a, MyVector2d v, double va)
{
    if (yellow) {
        yellow_robots[robot].pos.p = p;
        yellow_robots[robot].oldpos.p = p;
        yellow_robots[robot].vel.v = v;
        yellow_robots[robot].vel.va = va;
        yellow_robots[robot].pos.dir = a;
        yellow_robots[robot].oldpos.dir = a;
    }
    else
    {
        blue_robots[robot].pos.p = p;
        blue_robots[robot].oldpos.p = p;
        blue_robots[robot].vel.v = v;
        blue_robots[robot].vel.va = va;
        blue_robots[robot].pos.dir = a;
        blue_robots[robot].oldpos.dir = a;
    }
}

void Simulator::SetRobotRandom(bool yellow, int robot)
{
    SetRobot(yellow, robot, rand_posvector(), rand_heading());
}

void Simulator::do_radio_recv(SSL_RadioFrame& frame)
{
    int cmdsize;
    bool bOk;
    SSL_RobotCmd cmd;

    cmdsize=frame.cmds_size();
    for(int i=0;i<cmdsize;i++)
    {
        bOk=true;
        cmd = frame.cmds(i);
        int rid = cmd.id();
        int t = cmd.team();
        // check for a valid array id
        if(t>=NUM_TEAMS)
        {
            printf("bad team id\n");
            bOk=false;
        }
        if ((rid < 0) || (rid >= MAX_TEAM_ROBOTS))
        {
            printf("bad id\n");
            bOk=false;
        }
        if(bOk)
        {
            MyVector3d vcmd(cmd.vx(), cmd.vy(), cmd.va());
            SetRobotCommand((t==teamYellow), rid, vcmd.x, vcmd.y, vcmd.z,
                            cmd.kick_time(), cmd.drib_speed());
            tracker.robots[t][rid].command(Time(), vcmd);
            //printf("ID=%d,(%3.2f,%3.2f,%3.2f)\r\n",rid, vcmd.x, vcmd.y, vcmd.z);
        }
    }
}

