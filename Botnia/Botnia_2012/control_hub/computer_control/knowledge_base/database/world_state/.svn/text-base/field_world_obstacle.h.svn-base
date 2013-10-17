#ifndef FIELD_WORLD_OBSTACLE_H
#define FIELD_WORLD_OBSTACLE_H

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


#include "control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_geometry.h"

#define ROBOT_RADIUS ROBOT_DEF_WIDTH_H

#define OBS_RECTANGLE  0
#define OBS_CIRCLE     1
#define OBS_HALF_PLANE 2

struct state
{
        vector2f pos;
        // vector2f vel;
        state *parent;
        state *next;
};

/*
struct obs_map{
  char *map;
  int width,height; // resolution of map
  double rad_x,rad_y;
};
*/

class obstacle
{
public:
        //mask用于控制是否被屏蔽检查
        int type,mask; // type of obstacle, enable mask
        vector2f pos;  // location of center
        //如果是圆形障碍物，rad.x保存圆半径
        //如果是矩形障碍物,保存宽和高
        //如果是平面障碍物,保存那个方向和哪边
        vector2f rad;  // (x,y) radii
        vector2f vel;  // object velocity
public:
        //一下函数中的state s用于存储机器人的位置
        double margin(state s);	//计算机器人和障碍物之间相隔的距离
        vector2f closest_point(state s);//寻找障碍物上离s最近的点
        bool check(state s);//计算机器人是否没与障碍物碰撞  =true 没有碰到 =false 碰到了
        bool check(state s0,state s1);//计算由s0和s1两点确定的直线是否没与障碍物碰撞
        vector2f repulse(state s);
        // state tangent(state s0,state s1);
};

#define MAX_OBSTACLES 24
// 2*MAX_TEAM_ROBOTS+1+2+4
// robots, ball, defense areas, walls

class obstacles
{
public:
        obstacle obs[MAX_OBSTACLES];
        int num,current_mask;
public:
        obstacles()
        {
                num=current_mask=0;
        }

        void clear()
        {
                num = 0;
        }
        void add_rectangle(float cx,float cy,float w,float h,int mask);
        void add_circle(float x,float y,float radius,
                        float vx,float vy,int mask);
        void add_half_plane(float x,float y,float nx,float ny,int mask);

        void set_mask(int mask)
        {
                current_mask = mask;
        }
        bool check(MyVector2d p);
        bool check(MyVector2d p,int &id);
        bool check(state s);
        bool check(state s,int &id);
        bool check(state s0,state s1);
        bool check(state s0,state s1,int &id);
        vector2f repulse(state s);
};

#endif // FIELD_WORLD_OBSTACLE_H
