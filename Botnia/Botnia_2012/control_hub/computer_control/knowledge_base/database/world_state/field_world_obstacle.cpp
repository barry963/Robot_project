

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

#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

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

#include "control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_geometry.h"
#include "user_interface/field_related/field_constants.h"

#include "field_world_obstacle.h"
#include "QDebug"

//====================================================================//
//    Obstacle class implementation
//====================================================================//
//寻找障碍物上离s最近的点
vector2f obstacle::closest_point(state s)
{
        vector2f p,d;
        double o;
        switch (type)
        {
        case OBS_CIRCLE:
                //计算面向s的单位方向矢量
                d = (s.pos - pos).norm();
                //计算面向s的方向上的边界位置
                p = pos + d*rad.x;
                break;
        case OBS_RECTANGLE:
                //如果是矩形，直接限定范围就可以
                p.set(bound(s.pos.x,pos.x-rad.x,pos.x+rad.x),
                      bound(s.pos.y,pos.y-rad.y,pos.y+rad.y));
                break;
        case OBS_HALF_PLANE:
                //如果是平面
                o = (s.pos - pos).dot(rad);
                p = (o > 0)? s.pos - rad*o : p = s.pos;
                break;
        default:
                p.set(0,0);
        }
        return(p);
}

//计算机器人和障碍物之间相隔的距离
//state s存储机器人的位置
double obstacle::margin(state s)
{
        vector2f p;
        double d;
        switch (type)
        {
        case OBS_CIRCLE:
                //rad.x保存当前障碍物半径
                //求取中心距离
                d = MyVector::distance(pos,s.pos);
                //减去两者半径
                return(d - rad.x - ROBOT_RADIUS);
        case OBS_RECTANGLE:
                //求取最近边界点
                p.set(bound(s.pos.x,pos.x-rad.x,pos.x+rad.x),
                      bound(s.pos.y,pos.y-rad.y,pos.y+rad.y));
                //计算机器人中心到边界点距离
                d = MyVector::distance(p,s.pos);
                //减去机器人半径
                return(d - ROBOT_RADIUS);
        case OBS_HALF_PLANE:
                //
                d = (s.pos - pos).dot(rad);
                return(max(d - ROBOT_RADIUS,0.0));
        }
        return(0.0);
}

//计算机器人是否没与障碍物碰撞
//=true 没有碰到
//=false 碰到了
bool obstacle::check(state s)
{
        double dx,dy;
        if (type==OBS_CIRCLE || type==OBS_RECTANGLE)
        {
                dx = fabs(s.pos.x - pos.x);
                dy = fabs(s.pos.y - pos.y);
                if (dx>rad.x+ROBOT_RADIUS || dy>rad.y+ROBOT_RADIUS)
                {
                        return(true);
                }
        }
        return(margin(s) > 0.0);
}


//计算由s0和s1两点确定的直线是否没与障碍物碰撞
//=true 没有碰到
//=false 碰到了
bool obstacle::check(state s0,state s1)
{
        vector2f p;
        vector2f c[4];
        int i;
        double d;
        d = MyVector::distance(s0.pos,s1.pos);
        //如果两点间距太小，只检查一点
        if (d < EPSILON)
        {
                return(check(s0));
        }
        switch (type)
        {
        case OBS_CIRCLE:
                //point_on_segment
                // returns nearest point on line segment s0.pos-s1.pos to point pos
                //计算直线段(s0,s1)上离pos点最近的点
                p = MyVector::point_on_segment(s0.pos,s1.pos,pos);
                if (false)
                {
                        printf("pos(%f,%f)-(%f,%f):(%f,%f)=(%f,%f)\n",
                               s0.pos.x,s0.pos.y,
                               s1.pos.x,s1.pos.y,
                               pos.x,pos.y,
                               p.x,p.y);
                }
                d = MyVector::distance(p,pos);
                return(d > rad.x+ROBOT_RADIUS);
        case OBS_RECTANGLE:
                c[0].set(pos.x - rad.x,pos.y - rad.y);
                c[1].set(pos.x + rad.x,pos.y - rad.y);
                c[2].set(pos.x + rad.x,pos.y + rad.y);
                c[3].set(pos.x - rad.x,pos.y + rad.y);
                // check box against oriented sweep
                for (i=0; i<4; i++)
                {
                        //计算两个线段间的最小距离
                        d = MyVector::distance_seg_to_seg(s0.pos,s1.pos,c[i],c[(i+1)%4]);
                        // printf("%f \n",d);
                        if (d < ROBOT_RADIUS)
                        {
                                return(false);
                        }
                }
                // printf("\n");
                return(check(s0));
                /*
                n = (s1.pos - s0.pos).norm();
                n.set(-n.y,n.x);
                p = s0.pos;
                o = n.dot(p);

                p.set(pos.x - rad.x,pos.y - rad.y);
                if(n.dot(p) - o < ROBOT_RADIUS) return(false);
                p.set(pos.x + rad.x,pos.y - rad.y);
                if(n.dot(p) - o < ROBOT_RADIUS) return(false);
                p.set(pos.x - rad.x,pos.y + rad.y);
                if(n.dot(p) - o < ROBOT_RADIUS) return(false);
                p.set(pos.x + rad.x,pos.y + rad.y);
                if(n.dot(p) - o < ROBOT_RADIUS) return(false);

                return(check(s0) && check(s1));
                */
        case OBS_HALF_PLANE:
                return((s0.pos - pos).dot(rad)>ROBOT_RADIUS &&
                       (s1.pos - pos).dot(rad)>ROBOT_RADIUS);
        }
        return(true);
}

//repulse 排斥
//求机器人指向障碍物方向的单位矢量
//如果与障碍物冲突，求弹回来的单位矢量
vector2f obstacle::repulse(state s)
{
        vector2f p,v;
        double d,f;
        if (type==OBS_CIRCLE)
        {
                return((s.pos-pos).norm());
        }
        if (type==OBS_HALF_PLANE)
        {
                return(rad);
        }
        return((s.pos-pos).norm());
        //error!!!感觉有问题，出现了不可到达代码
        p = closest_point(s);
        d = MyVector::distance(p,s.pos);
        // if(type==OBS_RECTANGLE) printf("d=%f\n",d);
        if (d < EPSILON)
        {
                if (type==OBS_HALF_PLANE)
                {
                        return(rad);
                }
                // printf("ouch!\n");
                return((s.pos-pos).norm());
        }
        f = (d < ROBOT_RADIUS)? 1.0 : 1.0 / (1.0 + d/ROBOT_RADIUS);
        v = (s.pos - ((d > EPSILON)? p : pos)).norm();
        return(v * f);
}

//====================================================================//
//    Obstacles class implementation
//====================================================================//
//增加矩形障碍物
void obstacles::add_rectangle(float cx,float cy,float w,float h,int mask)
{
        if (num >= MAX_OBSTACLES)
        {
                return;
        }
        obs[num].type = OBS_RECTANGLE;
        obs[num].mask = mask;
        obs[num].pos.set(cx,cy);
        obs[num].rad.set(w/2,h/2);
        obs[num].vel.set(0,0);
        num++;
}

//增加圆形障碍物
void obstacles::add_circle(float x,float y,float radius,
                           float vx,float vy,int mask)
{
        if (num >= MAX_OBSTACLES)
        {
                return;
        }
        obs[num].type = OBS_CIRCLE;
        obs[num].mask = mask;
        obs[num].pos.set(x,y);
        obs[num].rad.set(radius,radius);
        //同时记录圆形障碍物速度,圆形障碍物一般是机器人
        obs[num].vel.set(vx,vy);
        num++;
}

//增加障碍平面
void obstacles::add_half_plane(float x,float y,float nx,float ny,int mask)
{
        if (num >= MAX_OBSTACLES)
        {
                return;
        }
        obs[num].type = OBS_HALF_PLANE;
        obs[num].mask = mask;
        obs[num].pos.set(x,y);
        obs[num].rad.set(nx,ny);
        obs[num].vel.set(0,0);
        num++;
}

//计算位于点p的机器人是否没与障碍物碰撞
//=true 没有碰到
//=false 碰到了
bool obstacles::check(MyVector2d p)
{
        state s;
        s.pos.set(p.x,p.y);
        s.parent = NULL;
        return(check(s));
}

bool obstacles::check(MyVector2d p,int &id)
{
        state s;
        s.pos.set(p.x,p.y);
        s.parent = NULL;
        return(check(s,id));
}

//将所有障碍物检查一遍，看是否与位于s的机器人冲突
bool obstacles::check(state s)
{
        int i;
        i = 0;
        while (i<num
                && ( ( (obs[i].mask&current_mask)==0) || obs[i].check(s) )
              )
        {
                i++;
        }
        return(i == num);
}

//将所有障碍物检查一遍，看是否与位于s的机器人冲突，并返回障碍物编号
bool obstacles::check(state s,int &id)
{
        int i;
        i = 0;
        while (i<num && ( ((obs[i].mask&current_mask)==0) || obs[i].check(s)))
        {
                i++;
        }
        //如果冲突了，返回障碍物编号
        if (i < num)
        {
                id = i;
        }
        return(i == num);
}

//将所有障碍物检查一遍，看是否与(s0,s1)线段冲突
bool obstacles::check(state s0,state s1)
{
        int i;
        i = 0;
        qDebug()<<num;
        while (i<num && ( ((obs[i].mask&current_mask)==0) || obs[i].check(s0,s1)))
        {
            qDebug()<<"s0-s1: "<<obs[i].check(s0,s1)<<",i="<<i<<",type="<<obs[i].type;
            i++;
        }
        if(i!=num)
        qDebug()<<"s0-s1: false"<<",i="<<i<<",type="<<obs[i].type<<" ,pos("<<obs[i].pos.x<<","<<obs[i].pos.y<<")";

        return(i == num);
}

//将所有障碍物检查一遍，看是否与(s0,s1)线段冲突,并返回障碍物编号
bool obstacles::check(state s0,state s1,int &id)
{
        int i;
        i = 0;
        while (i<num && ( ((obs[i].mask&current_mask)==0) || obs[i].check(s0,s1)))
        {
                i++;
        }
        //如果产生冲突，返回障碍物编号
        if (i < num)
        {
                id = i;
        }
        return(i == num);
}

//如果与障碍物冲突，求弹回来的单位矢量之和
vector2f obstacles::repulse(state s)
{
        vector2f f;
        int i;
        f.set(0.0,0.0);
        for (i=0; i<num; i++)
        {
                if ((obs[i].mask & current_mask) && !obs[i].check(s))
                {
                        f += obs[i].repulse(s);
                }
        }
        return(f);
}

#endif
