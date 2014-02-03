#include "field_world_robot.h"

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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "user_interface/field_related/field_constants.h"
#include "net/net_vision_interface.h"
#include "user_interface/field_related/motion_simulation_tools/field_util.h""
#include "control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_geometry.h"
#include "field_world.h"

#include "field_world_obstacle.h"
#include "field_world_robot.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_parametres.h"
#include "user_interface/field_related/field_global_function.h"


const bool robot_print     = false;//lu_test change all to true
const bool robot_sub_state = false;
const bool robot_debug_die = false;


//封装了整个队伍的行为，体现了就近的机器人优先采取行动的原则，描述了机器人作为不同角色球员时的行为和反应过程。
const char *state_name[] =
{
    "SMGotoBall",				//跑向球
    "SMFaceBall",				//朝向球
    "SMApproachBall",		//接近球
    "SMPullBall",				//拉球
    "SMFaceTarget",			//朝向目标
    "SMDriveToGoal",		//冲向球门
    "SMKick",						//起脚踢球
    "SMSpinAtBall",			//在球前旋转
    "SMPosition",				//跑位
    "SMRecieveBall",		//接球
    "SMWait"						//待命
};
//定义了我方单个机器人可能存在的各种状态模式，随后定义了相应的状态处理函数。

//将机器人限制在场地内
MyVector2d on_field(MyVector2d pos,double radius)
{
    pos.x = bound(pos.x,-FIELD_LENGTH_H+radius,FIELD_LENGTH_H-radius);
    pos.y = bound(pos.y,-FIELD_WIDTH_H +radius,FIELD_WIDTH_H -radius);
    return(pos);
}

void Robot::init(int _my_id)
{
    my_id = _my_id;
    state = last_state = SMGotoBall;
    state_start_time = 0.0;
    last_dist_from_target = 0.0;
    last_target_da = 0.0;
    spin_dir = 0;
}

const MyVector2d own_goal_pos(-FIELD_LENGTH_H-2*BALL_RADIUS,0);
const MyVector2d opp_goal_pos( FIELD_LENGTH_H+2*BALL_RADIUS,0);

//更新赛场上的信息，如我的当前位置、角度、速度、角速度，我到对方球门中点的向量，障碍物的情况等等;
void Robot::updateSensors(World &world)
{
    Sensors &s = sensors;
    VisionRawInfo vpos;
    double f;
    //==== update high level sensors ====//
    // find out current position
    //当前机器人位置
    s.r_pos = world.GetRobotPositionByID(my_id);
    //当前机器人角度
    s.r_ang = world.teammate_direction(my_id);
    //当前机器人速度
    s.r_vel = world.GetRobotVelocityByID(my_id);
    //当前机器人角速度
    s.r_ang_vel = world.teammate_angular_velocity(my_id);
    //
    s.r_fwd.set(cos(s.r_ang),sin(s.r_ang));
    s.opp_goal_rel = opp_goal_pos - s.r_pos;
    // ball information
    //球位置
    s.ball_pos  = world.ball_position();
    //球速度
    s.ball_vel  = world.ball_velocity();
    //球相对机器人坐标
    s.ball_rel  = s.ball_pos - s.r_pos;
    //球相对机器人距离
    s.ball_dist = s.ball_rel.length();
    s.own_goal_to_ball = s.ball_pos - own_goal_pos;
    s.ball_to_opp_goal = opp_goal_pos - s.ball_pos;
    s.ball_dist_from_wall = min(FIELD_LENGTH_H-fabs(s.ball_pos.x),
                                FIELD_WIDTH_H -fabs(s.ball_pos.y));
    f = (s.ball_pos.x + FieldConstantsRoboCup2012::field_length_) / FieldConstantsRoboCup2012::field_length_;
    s.good_ball_dir = (s.own_goal_to_ball.norm() * (1-f) +
                       s.ball_to_opp_goal.norm() * f).norm();
    s.spin_dir = (int)sign_nz(dot(s.ball_rel.perp(),s.good_ball_dir));
    /*
        printf("SPIN: gdb=<%f,%f> bp=<%f,%f> %f %d",
               V2COMP(s.good_ball_dir),V2COMP(s.ball_rel.perp()),
               dot(s.ball_rel.perp(),s.good_ball_dir),s.spin_dir);
        */
    s.ball_conf = world.ball_raw(vpos);
    //球在对方球门内
    s.ball_in_opp_goal = ((s.ball_pos.x > FIELD_LENGTH_H+2*BALL_RADIUS)  && (fabs(s.ball_pos.y) < GOAL_WIDTH_H) );
    //球相对机器人矢量旋转到X轴方向
    s.robot_ball = s.ball_rel.rotate(-s.r_ang);
    //球在机器人内部
    s.ball_in_front = (s.robot_ball.x > -20) &&
            (fabs(s.robot_ball.y) < 60);
    //球在机器人前方
    s.ball_on_front = s.ball_in_front && (s.robot_ball.x < 120);
    //s.ball_target.set(FIELD_LENGTH_H /*+BALL_RADIUS*/,
    //                  bound(s.ball_pos.y,
    //                        -GOAL_WIDTH_H+ROBOT_RADIUS,
    //                         GOAL_WIDTH_H-ROBOT_RADIUS));
    //s.ball_target_rel = s.ball_target - s.r_pos;
    //
    //s.on_goal = (s.r_fwd.cross(world.their_goal_l - s.r_pos) > 0) &&
    //            (s.r_fwd.cross(world.their_goal_r - s.r_pos) < 0);
    // printf("on_goal=%d\n",s.on_goal);
    s.can_drive = target_ball_rel.x>60.0 &&
            target_ball_rel.x<500.0 &&
            fabs(target_ball_rel.y)<60.0;
    //==== set up obstacles ====//
    MyVector2d p,v;
    int i;
    // set up teammates as obstacles
    for (i=0; i<world.n_teammates; i++)
    {
        if (i != my_id)
        {
            p = world.GetRobotPositionByID(i);
            v = world.GetRobotVelocityByID(i);
            s.obs.add_circle(p.x,p.y,110,v.x,v.y,1);
        }
    }
    // set up opponents as obstacles
    for (i=0; i<world.n_teammates; i++)
    {
        p = world.opponent_position(i);
        v = world.opponent_velocity(i);
        s.obs.add_circle(p.x,p.y,95,v.x,v.y,1);
    }
    // walls
    // OBS_WALLS
    s.obs.add_half_plane(-FIELD_LENGTH_H,              0, 1, 0,1);
    s.obs.add_half_plane( FIELD_LENGTH_H,              0,-1, 0,1);
    s.obs.add_half_plane(               0,-FIELD_WIDTH_H, 0, 1,1);
    s.obs.add_half_plane(               0, FIELD_WIDTH_H, 0,-1,1);
    // defense zones
    // OBS_OUR_DZONE
    s.obs.add_rectangle(-FIELD_LENGTH_H-DEFENSE_DEPTH,0,
                        DEFENSE_DEPTH*4,DEFENSE_WIDTH,1);
    //OBS_THEIR_DZONE
    s.obs.add_rectangle( FIELD_LENGTH_H+DEFENSE_DEPTH,0,
                         DEFENSE_DEPTH*4,DEFENSE_WIDTH,1);
    s.obs.set_mask(OBS_EVERYTHING);
}

//==== State Machine Core Functions ==================================//

Robot::SMState Robot::gotoBall(World &world,Sensors &s,RobotCommand &cmd,
                               NavTarget &nav)
{
    MyVector2d targ,ball_pred;
    double targ_dist,max_behind,behind,t;
    double ball_rad;
    int obs_id;
    // obstacle radius that nav_to_point uses
    ball_rad = bound(s.ball_dist-180,30,60);
    if (cmd.cmd == CmdSteal)
    {
        max_behind = 0;
    }
    else
    {
        max_behind = 160; // bound(100,1,100); // -100/10*time_in_state,1,100);
    }
    //按照速度1000预测，计算球的位置
    t = max(s.ball_dist-200.0,0.0) / 1000.0;
    ball_pred = world.ball_position(world.now + t);
    // walk forward looking for free space
    //按照上面设定的max_behind距离逐步前进，寻找空闲地点
    //用于将机器人带到球中心附近160mm范围内
    obs_id = -1;
    for (behind=max_behind; behind>=0.0; behind-=10.0)
    {
        //target_rel 目的地相对机器人位置差矢量
        targ = ball_pred - target_rel.norm(behind);
        //将机器人限制在场地内
        targ = on_field(targ,ROBOT_RADIUS);
        if (s.obs.check(targ,obs_id))
        {
            break;
        }
    }
    if (robot_print)
    {
        printf("  behind = %f (obs %d)\n",behind,obs_id);
    }
    //如果与球的距离少于机器人加球半径，即球在机器人内，则删除球障碍设定
    if (fabs(behind) < ball_rad+OMNIBOT_RADIUS)
    {
        //
        targ = ball_pred - (ball_pred - s.r_pos).norm(ball_rad+90+10);
        nav.obs &= ~(OBS_BALL);
    }
    /*
        // if no free space go directly to ball
        if(!s.obs.check(targ)){
          behind = 0.0;
          targ = ball_pred;
        }
        */
    //计算机器人与球附近目标点距离
    targ_dist = MyVector::distance(targ,s.r_pos);
    if (state_changed)
    {
        last_dist_from_target = targ_dist;
    }
    gui_debug_printf(my_id, GDBG_TACTICS,
                     "  targ_dist=%f\n",targ_dist);
    if (false)
    {
        printf("  behind=%f targ_dist=%f last=%f\n",
               behind,targ_dist,last_dist_from_target);
    }
    bool candriver=true;

    qDebug()<<"Target_ball_rel:("<<target_ball_rel.x<<","<<target_ball_rel.y<<")\n";
    if (target_ball_rel.x<20.0 ||
            target_ball_rel.x>250.0 ||
            fabs(target_ball_rel.y)>90.0)
    {
        candriver=false;
    }
    // probably want to add obstacle check here
    if (!world.inOurDefenseZone())
    {
        switch (cmd.cmd)
        {
        case CmdMoveBall:
        case CmdDribble:
            if (targ_dist<150 &&
                    //趋势是接近目标点
                    last_dist_from_target<=targ_dist)
            {
                // printf("  %f <= %f\n",last_dist_from_target,targ_dist);
                //球在机器人前方，能够驱动
                qDebug()<<"Can_drive"<<s.can_drive<<" candriver:"<<candriver;
                if (s.can_drive && candriver)
                {
                    //printf("gotoball %3.2f,%3.2f\r\n",target_ball_rel.x,target_ball_rel.y);
                    return(SMDriveToGoal);
                    /*}else if(fabs(target_ball_rel.x) < 0.0){
                                           return(SMSpinAtBall);*/
                }
                //球速度
                else if (s.ball_vel.length()<200)
                {
                    return(SMFaceBall);
                }
            }/* else if(target_ball_rel.x>60.0 && fabs(target_ball_rel.y)<50.0){
          return(SMDriveToGoal);
      }*/
            break;
        case CmdSteal:
            if (targ_dist<200 && last_dist_from_target<targ_dist)
            {
                return(SMSpinAtBall);
            }
            break;
        default:
            printf("Unimeplemented state/cmd pair.\n");
        }
    }
    nav.pos    = targ;
    nav.angle  = s.ball_rel.angle();
    //允许进入对方禁区
    nav.obs  &= ~(OBS_THEIR_DZONE);
    last_dist_from_target = targ_dist;
    if (false)
    {
        printf("  targ=<%f,%f> targ_dist = %f navangle= %f \n",V2COMP(targ),targ_dist,nav.angle);
    }
    return(SMGotoBall);
}

//实现面向球
Robot::SMState Robot::faceBall(World &world,Sensors &s,RobotCommand &cmd,
                               NavTarget &nav)
{
    double da,dta;
    //da=机器人与球连线角度与机器人本身角度差
    da  = angle_mod(s.ball_rel.angle() - s.r_ang);
    dta = angle_mod(target_rel.angle() - s.r_ang);
    //如果角度小于0.1弧度
    if (fabs(da) < 0.1)
    {
        if (s.can_drive)
        {
            //将球带向守门员
            return(SMDriveToGoal);
        }
        else
        {
            //如果角度过大,旋转机器人,面向球
            //接近球
            return(SMApproachBall);
        }
    }
    //填充命令结构
    nav.direct = true;
    //设定旋转量
    nav.vel_xya.set(0,0,6*sin(da));
    //qDebug()<<"Nav(vx,vy,va)"<<nav.vel_xya.x<<", "<<nav.vel_xya.y<<", "<<nav.vel_xya.z<<")";
    return(SMFaceBall);
}

//靠近球
Robot::SMState Robot::approachBall(World &world,Sensors &s,RobotCommand &cmd,
                                   NavTarget &nav)
{
    MyVector2d ball_targ,ball_dir;
    double da,speed;
    /*
        printf("  (%d) (%d %d) %d\n",
               s.ball_dist > 300,
               target_ball_rel.x>0.0,fabs(target_ball_rel.y)<60.0,
               s.ball_on_front);
        */
    //inOurDefenseZone 检查球是否在我方禁区内
    //如果与球的距离大于300，或者在我方防区内
    if ((s.ball_dist > 300) || world.inOurDefenseZone())
    {
        return(SMGotoBall);
    }
    //
    if (s.can_drive && (s.ball_dist_from_wall > 100) && time_in_state>=1.0)
    { // && s.r_vel.length()<200)){
        return(SMDriveToGoal);
    }
    // printf("  bof=%d t=%f wd=%f\n",s.ball_on_front,time_in_state,s.ball_dist_from_wall);
    if (s.ball_on_front && time_in_state >= 1.0)
    {
        // return((s.ball_dist_from_wall > 100)? SMFaceTarget : SMPullBall); question lu_test
        return((s.ball_dist_from_wall > 50)? SMFaceTarget : SMSpinAtBall);
    }
    else if (time_in_state >= 1.0)
    {
        return(SMFaceBall);
    }
    if (state_changed)
    {
        initial_ball_dist = s.ball_dist;
    }
    ball_targ = on_field(s.ball_pos,OMNIBOT_RADIUS);
    ball_dir = (ball_targ - s.r_pos).norm();
    da = angle_mod(s.ball_rel.angle() - s.r_ang);
    speed = 1.5*initial_ball_dist * (1.0 - time_in_state);
    nav.dribble_power = 15;
    nav.direct  = true;
    if (omni)
    {
        ball_dir = ball_dir.rotate(-s.r_ang) * speed;
        nav.vel_xya.set(ball_dir.x,ball_dir.y,0);
    }
    else
    {
        nav.vel_xya.set(speed,0,0);
    }
    // printf("  <%f,%f> <%f,%f,%f>\n",V2COMP(ball_dir),V3COMP(nav.vel_xya));
    return(SMApproachBall);
}

//完成拉球功能
Robot::SMState Robot::pullBall(World &world,Sensors &s,RobotCommand &cmd,
                               NavTarget &nav)
{
    double vx;
    if (time_in_state > 1.0)
    {
        return(SMFaceTarget);
    }
    //球不在面前
    if (!s.ball_on_front && s.ball_conf>0.5)
    {
        return(SMGotoBall);
    }
    //拉球速度逐渐增加
    vx = -500.0 * time_in_state;
    nav.dribble_power = 15;
    nav.direct = true;
    nav.vel_xya.set(vx,0,0);
    return(SMPullBall);
}

//
Robot::SMState Robot::faceTarget(World &world,Sensors &s,RobotCommand &cmd,
                                 NavTarget &nav)
{
    double da,sx,sa,vx,va;
    //计算球与
    da = angle_mod(target_rel.angle() - s.r_ang);
    if (robot_print)
    {
        printf("  (%d %d) (%d) (%d)\n",
               !s.ball_on_front,s.ball_conf>0.5,
               fabs(da) < 0.2,time_in_state > 4.0);
    }
    if (!s.ball_on_front && s.ball_conf>0.5)
    {
        return(SMGotoBall);
    }
    if (target_ball_rel.x>0.0 && fabs(target_ball_rel.y)<60.0)
    {
        return(SMDriveToGoal);
    }
    if (world.teammate_stuck(my_id)>0.90 && time_in_state>1.0)
    {
        return(SMSpinAtBall);
    }
    sa = min(  4.0*time_in_state,  2.5);
    sx = min(500.0*time_in_state,500.0);
    va = bound(2*da,-sa,sa);
    vx = bound((ROBOT_RADIUS+BALL_RADIUS)*va/tan(da),-sx,sx);
    //--------------------------------------------------------------
    nav.dribble_power = 15;
    nav.direct = true;
    nav.vel_xya.set(vx,0,va);
    return(SMFaceTarget);
}

//带球或者射门
//carry the ball or shoot
Robot::SMState Robot::driveToGoal(World &world,Sensors &s,RobotCommand &cmd,
                                  NavTarget &nav)
{
    //球撞击点相对球的位置差矢量
    MyVector2d ball_to_target = cmd.ball_target-s.ball_pos;
    // double carrot_dist = max(500.0-s.ball_dist,1.0);
    // MyVector2d carrot_pos = s.ball_pos + ball_to_target.norm(carrot_dist);
    double t,dba;
    double tdist,tbdist;
    //s.ball_rel 球相对机器人坐标 target_rel 目的地相对机器人位置差矢量
    //计算这两个矢量单位化后的点积
    t = cosine(s.ball_rel,target_rel);
    //射门需要角度与球姿态角度差
    dba = angle_mod(ball_to_target.angle() - s.r_ang);
    //机器人距离目标点距离
    tdist  = target_rel.length();
    //机器人距离球的撞击点距离
    tbdist = MyVector::distance(cmd.ball_target,s.r_pos);
    if (robot_print)
    {
        printf("  to kick: c%d f%d s%d a%d d%d\n",
               cmd.cmd==CmdMoveBall,s.ball_on_front,
               MyVector::dot(s.r_vel,s.ball_rel.norm()) > 100.0,
               (fabs(dba) < cmd.angle_tolerance),
               (tbdist < 1000+500*omni));
    }
    if (cmd.cmd==CmdMoveBall && s.ball_on_front &&
            //机器人相对球的速度差超过100
            MyVector::dot(s.r_vel,s.ball_rel.norm()) > 100.0 &&
            (fabs(dba) < cmd.angle_tolerance))
    {
        switch (cmd.ball_shot_type)
        {
        case BallShotOnGoal:
        case BallShotPass:
            //5000=球速度10000*0.5秒
            //角度保留25%的余量
            if ((tbdist > 100) && (fabs(dba) < cmd.angle_tolerance*0.75) )//&& (tbdist < 5000) )
            {
                return(SMKick);
            }
            break;
            //射向空地
        case BallShotClear:
            // if(!omni || s.on_goal) return(SMKick);
            //防止将球射出场地，0.5弧度大于等于30度
            //用于将球射向空地
            if (!omni || fabs(s.r_ang)<0.5)
            {
                return(SMKick);
            }
            // don't let the omni kick out of the field
            // 0.5 radians is about 30 degrees
        }
    }
    if (target_ball_rel.x<20.0 ||
            target_ball_rel.x>250.0 ||
            fabs(target_ball_rel.y)>90.0)
    {
        //printf("driveToGoal %3.2f,%3.2f\r\n",target_ball_rel.x,target_ball_rel.y);
        return(SMGotoBall);
    }
    //球离墙只有50,旋转将球带入场内
    if (s.ball_dist_from_wall < 50)
    {
        return(SMSpinAtBall);
    }
    /*
        if(tdist > 200 &&
           (target_ball_rel.x<0.0 ||
            fabs(target_ball_rel.y)>90.0 ||
            fabs(target_ball_rel.x)>90.0)) return(SMGotoBall);
        // if(t < 0.0) return(SMGotoBall);
        */
    /*
        if(world.teammate_stuck(my_id) > 0.90){
          return(SMSpinAtBall);
        }
        */
    if (robot_print) printf("  <%f,%f> dba=%f\n",
                            V2COMP(target_ball_rel),dba);
    if (omni)
    {
        nav.dribble_power = 15;
        nav.pos   = cmd.target; // carrot_pos; // s.ball_pos; // cmd.target;
        nav.angle = (cmd.target - s.r_pos).angle();
        nav.vel   = ball_to_target.norm(1000);
        nav.vel_xya.set(0,bound(target_ball_rel.y,-100,100),0);
        // nav.pos   = carrot_pos; // cmd.target;
        // nav.vel   = ball_to_target.norm(500);
        // nav.angle = (cmd.target - s.r_pos).angle();
        /*
                // nav.pos   = cmd.target;
                if(MyVector::distance(cmd.target,s.r_pos) < 40){
                  nav.angle = (cmd.ball_target - s.r_pos).angle();
                }else{
                   // nav.angle = (cmd.target - s.r_pos).angle();
                }
                */
    }
    else
    {
        if (s.r_vel.length() < 1000)
        {
            nav.dribble_power = 15;
        }
        nav.pos   = cmd.target; // carrot_pos;
        // nav.vel   = ball_to_target.norm(500);
        // nav.pos   = cmd.target;
        // nav.angle = target_rel.angle();
        nav.angle = ball_to_target.angle();
    }
    nav.obs  &= ~(OBS_BALL|OBS_THEIR_DZONE);
    nav.type = GotoPointShoot;
    return(SMDriveToGoal);
}

//
Robot::SMState Robot::kick(World &world,Sensors &s,RobotCommand &cmd,
                           NavTarget &nav)
{
    double da,va;
    double k;
    if (state_changed)
    {
        last_target_da = 2*M_PI;
    }

/* lu_test comment the whole code, as it is not neccesary
    //如果处于踢球状态时间长，迎向球
    if (time_in_state>0.25+1.75*omni)
    {
        qDebug()<<"Kick state too long, so switch to go to ball"<<time_in_state;
        return(SMGotoBall);
    }
*/

    //如果球不在机器人内部，让机器人迎向球
    if (!s.ball_in_front)
    {
        return(SMGotoBall);
    }
    //
    da = angle_mod((cmd.ball_target-s.ball_pos).angle() - s.r_ang);
    // - s.ball_vel.angle());
    // (s.r_ang_vel*da < 0.0)
    /*
          ((fabs(da) < 0.25) && (fabs(da) > fabs(last_target_da))) ||
          (time_in_state > 0.50)){
        */
    k = omni? 1 : 4;
    if (fabs(da) < min(0.50+k*time_in_state,1.0)*cmd.angle_tolerance
            && (!omni || world.time - last_kick_timestamp > 0.75))
    {
        if (cmd.ball_shot_type==BallShotPass)
        {
            nav.kick_power = giShootPowerPass;
        }
        else
        {
            nav.kick_power = giShootPowerMax;
        }
        qDebug()<<"Kick! t="<<time_in_state<<", power="<<nav.kick_power;
        last_kick_timestamp = world.time;
    }
    if (robot_print)
    {
        printf("  targ=<%f,%f>:%f va=%f da=%f k=%d\n",
               V2COMP(target_rel),target_rel.angle(),
               s.r_ang_vel,da,
               nav.kick_power);
    }
    nav.direct = true;
    if (omni)
    {
        va = 6*sin(da);
        nav.vel_xya.set(
                    world.teammate_robot_velocity(my_id).x,
                    -va*70.0,
                    va
                    );
    }
    else
    {
        nav.vel_xya.set(
                    world.teammate_robot_velocity(my_id).x + 1000*FRAME_PERIOD,
                    0.0,
                    6*sin(da)
                    );
    }
    last_target_da = da;
    return(SMKick);
}

Robot::SMState Robot::spinAtBall(World &world,Sensors &s,RobotCommand &cmd,
                                 NavTarget &nav)
{
    double ofs;
    // MyVector2d dzone_closest;
    if (state_changed)
    {
        start_dist_from_ball = s.ball_dist;
    }
    if (cmd.cmd == CmdSpin)
    {
        ofs = offset_to_line(s.r_pos,cmd.ball_target,s.ball_pos);
        nav.pos = cmd.target;
    }
    else
    {
        if (world.inOurDefenseZone())
        {
            return(SMGotoBall);
        }
        if (s.ball_dist > min(start_dist_from_ball+10,300.0))
        {
            return(SMGotoBall);
        }
        if (time_in_state > 10.0)
        {
            return(SMGotoBall);
        }
        if (cmd.ball_shot_type == BallShotClear)
        {
            /*
                        dzone_closest.set(
                          bound(s.ball_pos.x,-FIELD_WIDTH_H,-FIELD_WIDTH_H+DEFENSE_DEPTH),
                          bound(s.ball_pos.y,-DEFENSE_WIDTH_H, DEFENSE_WIDTH_H));
                        ofs = offset_to_line(dzone_closest,s.r_pos,s.ball_pos);
                        */
            ofs = MyVector::dot(s.good_ball_dir.perp(),s.ball_pos-s.r_pos);
        }
        else
        {
            ofs = offset_to_line(s.r_pos,cmd.ball_target,s.ball_pos);
        }
        nav.pos   = s.ball_pos;
    }
    if (false && world.obsLine(s.r_pos,s.ball_pos - s.ball_rel.norm(90),
                               (OBS_OPPONENTS|OBS_TEAMMATES)&~(OBS_TEAMMATE(my_id))))
    {
        // printf("spinAtBall::goto\n");
        nav.pos   = s.ball_pos - s.ball_pos.norm(45);
        nav.angle = s.ball_rel.angle();
    }
    else
    {
        // printf("spinAtBall::spin\n");
        spin_dir = (int)sign_nz(ofs);
        nav.angle = -10*spin_dir;
        nav.spin  = true;
    }
    return(SMSpinAtBall);
}

//机器人运动到指定坐标
// this is the function that assign the target in command to the to the target in navigation algorithm
// gao mark
// final place of assigning state
Robot::SMState Robot::position(World &world,Sensors &s,RobotCommand &cmd,
                               NavTarget &nav)
{
    nav.pos   = cmd.target;
    nav.vel   = cmd.velocity;
    nav.angle = cmd.angle;
    nav.obs   = cmd.observation_type;
    nav.type  = cmd.goto_point_type;
    return(SMPosition);
}

//接球基本动作
Robot::SMState Robot::recieveBall(World &world,Sensors &s,RobotCommand &cmd,
                                  NavTarget &nav)
{
    double a,da;
    //如果球不在面前,计算和面向球的角度差，然后转向球
    if (!s.ball_on_front)
    {
        a  = s.ball_rel.angle();
        da = angle_mod(a - s.r_ang);
    }
    //否则不用转向
    else
    {
        da = 0;
    }
    //启动带球功能
    nav.dribble_power = 15;
    nav.direct = true;
    nav.vel_xya.set(0,0,3*sin(da));
    return(SMRecieveBall);
}

//机器人等待
Robot::SMState Robot::wait(World &world,Sensors &s,RobotCommand &cmd,
                           NavTarget &nav)
{
    if (time_in_state > 1.0)
    {
        return(SMGotoBall);
    }
    nav.direct = true;
    nav.vel_xya.set(0,0,0);
    return(SMWait);
}

// precondition: update_sensors has been run this timestep
Status Robot::run(World &world,RobotCommand &cmd,Trajectory &tcmd)
{
    Sensors &s = sensors;
    Status status;
    NavTarget nav;
    //nav.pos = cmd.target;
    int old_state;
    int n;
    omni = (world.teammate_type(my_id) == ROBOT_TYPE_OMNI);
    // If command changed, set initial state
    //检查命令
    if (cmd.cmd != last_cmd)
    {
        switch (cmd.cmd)
        {
        //it seems that every cmd doesnot point to every SMstate,
        //so future work is to ad more cmd lu_test
        case CmdMoveBall:
            state = SMGotoBall;
            break;
        case CmdSteal:
            state = SMGotoBall;
            break;
        case CmdDribble:
            state = SMGotoBall;
            break;
        case CmdRecieveBall:
            state = SMRecieveBall;
            break;
        case CmdPosition:
            state = SMPosition;
            break;
        case CmdSpin:
            state = SMSpinAtBall;
            break;
        default:
            printf("Invalid Command %d.\n",cmd.cmd);
            return(Failed);
            break;
        }
    };
    // common calculations
    //目的地相对机器人位置差矢量
    target_rel = cmd.target - s.r_pos;

    //uselesses target_ball_rel lu_test
    target_ball_rel.set(s.ball_rel.dot(target_rel.norm()),
                        s.ball_rel.dot(target_rel.norm().perp()));

    // target_ball_rel = s.ball_rel.rotate(-(cmd.target-s.r_pos).angle());
    // execute states until no longer switching
    //循环执行,直到不再切换状态
    const int maxloop=10;
    n = maxloop;
    state_changed = false;
    do
    {

        old_state = state;
        //计算在本状态中保持的时间
        time_in_state = world.time - state_start_time;
        mzero(nav);
        nav.obs = OBS_EVERYTHING_BUT_ME(my_id);
        if (robot_sub_state)
        {
            printf("SubState [%s]  execute times: %d\n",state_name[state],n-maxloop+1);
        }
        switch (state)
        {

        //printf("cmd: target(%f,%f)");//lu_test add
        case SMGotoBall:
            state = gotoBall    (world,s,cmd,nav);
            break;
        case SMFaceBall:
            state = faceBall    (world,s,cmd,nav);
            break;
        case SMApproachBall:
            state = approachBall(world,s,cmd,nav);
            break;
        case SMPullBall:
            state = pullBall    (world,s,cmd,nav);
            break;
        case SMFaceTarget:
            state = faceTarget  (world,s,cmd,nav);
            break;
        case SMDriveToGoal:
            state = driveToGoal (world,s,cmd,nav);
            break;
        case SMKick:
            state = kick        (world,s,cmd,nav);
            break;
        case SMSpinAtBall:
            state = spinAtBall  (world,s,cmd,nav);
            break;
            // gao mark
        case SMPosition:
            state = position    (world,s,cmd,nav);
            break;
        case SMRecieveBall:
            state = recieveBall (world,s,cmd,nav);
            break;
        case SMWait:
            state = wait        (world,s,cmd,nav);
            break;
        }

        if (state!=old_state)
        {
            state_start_time = world.time;
            state_changed = true;
            switch(state){
                case SMGotoBall:
                    printf("SMGotoBall\r\n");break;
                case SMFaceBall:
                    printf("SMFaceBall\r\n");break;
                case SMApproachBall:
                    printf("SMApproachBall\r\n");break;
                case SMPullBall:
                    printf("SMPullBall\r\n");break;
                case SMFaceTarget:
                    printf("SMFaceTarget\r\n");break;
                case SMDriveToGoal:
                    printf("SMDriveToGoal\r\n");break;
                case SMKick:
                    printf("SMKick\r\n");break;
                case SMSpinAtBall:
                    printf("SMSpinAtBall\r\n");break;
                case SMPosition:
                    printf("SMPosition\r\n");break;
                case SMRecieveBall:
                    printf("SMRecieveBall\r\n");break;
                case SMWait:
                    printf("SMWait\r\n");break;
            }
        }

    } while (state!=old_state && --n);

/*lu_test
    if (n <= 0)
    {
        //连续发生10次状态变化，这是不正常情况
        printf("Robot::Oscillation Error! in class Robot::run\n");
        nav.direct = true;
        nav.vel_xya.set(100,0,0);
        if (robot_debug_die)
        {
            exit(1);
        }
    }
*/

//    printf("State: %s %0.2fs R(%8.2f,%8.2f)\n",
//           state_name[state],time_in_state, V2COMP(s.r_pos));

    if (robot_print)
    {
        printf("State: %s %0.2fs R(%8.2f,%8.2f)\n",
               state_name[state],time_in_state, V2COMP(s.r_pos));
        gui_debug_printf(my_id, GDBG_TACTICS,
                         "State: %s %0.2fs\n",state_name[state],time_in_state);
        gui_debug_printf(my_id, GDBG_TACTICS,
                         "  robot_ball=<%f,%f>\n",V2COMP(s.robot_ball));
    }
    if (robot_print)
    {
        printf("  robot_ball=<%f,%f>,%d target_ball_rel=<%f,%f> stuck=%f\n",
               V2COMP(s.robot_ball),s.ball_on_front,
               V2COMP(target_ball_rel),
               world.teammate_stuck(my_id));
   }
    // carry out target command
    //直接设定机器人的速度参数
    if (nav.direct)
    {
        tcmd.vx = nav.vel_xya.x;
        tcmd.vy = nav.vel_xya.y;
        tcmd.va = nav.vel_xya.z;
        //qDebug()<<"direct speed setting";

    }
    //通过路径导航实现速度控制
    else
    {
        if (nav.spin)
        {
            //qDebug()<<"spin to point";

            //旋转方式到指定点，不躲避障碍
            tcmd = spin_to_point(world,my_id,nav.pos,nav.angle);
        }
        else
        {
            //qDebug()<<"spin and direct to point";

            //static int iErr=0;
            //int bOppError=0,bBall=0;
            //if(nav.obs & OBS_OPPONENTS!=OBS_OPPONENTS)
            //{
            //	bOppError=1;
            //	iErr=1;
            //}
            //if(nav.obs & OBS_BALL!=0)
            //{
            //	bBall=1;
            //}
            //printf("nav.obs=%X,(%d,%d)\r\n",nav.obs,iErr,bBall);


            // nav _to_point function included pathplanning
            // lu_test maybe change to another

//            tcmd = goto_point(world,my_id,nav.pos,nav.vel,nav.angle,nav.type);
            tcmd = nav_to_point(world,my_id,
                                nav.pos,nav.vel,nav.angle,
                                nav.obs,nav.type);

            if (!tcmd.bValid)
            {
                return Failed;                
            }
            tcmd.vx += nav.vel_xya.x;
            tcmd.vy += nav.vel_xya.y;
            tcmd.va += nav.vel_xya.z;
        }
    }
    tcmd.kick_power   = nav.kick_power;
    tcmd.dribble_power = nav.dribble_power;
    //tcmd.DataDisplay();//lu_test add

//    qDebug()<<"NavCommand: vx="<<nav.vel_xya.x<<", vy="<<nav.vel_xya.y<<", z="<<nav.vel_xya.z;
//    qDebug()<<"TCommand: vx="<<tcmd.vx<<", vy="<<tcmd.vy
//           <<", va="<<tcmd.va<<", kick_power="<<tcmd.kick_power<<", dribble_power="<<tcmd.dribble_power;

    // if(robot_print && nav.kick) printf("Kick!\n");
    //计算任务是否完成
    switch (cmd.cmd)
    {
    case CmdMoveBall:
        status = (nav.kick_power)? Completed : InProgress;
        //qDebug()<<"cmdmoveball"<<status;
        break;
        //如果是偷球
    case CmdSteal:
        status = InProgress;
        break;
        //是接收传球命令,如果球已经在面前,则完成
    case CmdRecieveBall:
        status = (s.ball_on_front)? Completed : InProgress;
        //qDebug()<<"cmdReceiveball"<<status;
        break;
        //如果是带球,和定位,并且和目标点间距小于20mm，则任务完成
    case CmdSpin:
    case CmdDribble:
    case CmdPosition:
        //20=最高速度/60;
        if (MyVector::distance(cmd.target,s.r_pos) < POSITION_ACCURACY)
        {
            status = Completed;
        }
        else
        {
            status = InProgress;
        }
        break;
    default:
        status = InProgress;
    }
    // some final calculations
//    qDebug()<<"Current("<<s.r_pos.x<<","<<s.r_pos.y<<")->("<<cmd.target.x<<","<<cmd.target.y<<")";
//    qDebug()<<"distance"<<MyVector::distance(cmd.target,s.r_pos);
    last_state = state;
    last_cmd = cmd.cmd;
    return(status);
}

Status Robot::run(World &world,RobotCommand &cmd)
{
    Trajectory tcmd;
    Status s;
    // defined in robot specified file (different from this run look at the parameters it take)
    s = run(world,cmd,tcmd);
    if (s!=Failed)
    {
        //向指定的机器人发出踢球和带球命令
        //send shoot and dribble command to the specific robot
        world.go(my_id,tcmd.vx,tcmd.vy,tcmd.va,tcmd.kick_power,tcmd.bchipkick,tcmd.dribble_power);
    }
    return(s);
}

//机器人动作时间评估,用于机器人角色选择评估
//适用函数 obsLineNum
//计算从p1到p2直线上有多少个障碍
//判断身边障碍物的总数(包括球，队友和对方球员);
//robot action time estimate, used for robot role choose estimate
//used for obsLineNum
//calculate how many obstacles between p1 and p2
//judge the total number of obstacles(including ball, teammates and opponents)
double Robot::time(World &world,RobotCommand &cmd)
{
    Sensors &s = sensors;
    double t;
    int nobs = 0;
    switch (cmd.cmd)
    {
    case CmdMoveBall:
    case CmdDribble:
        if (cmd.cmd==CmdMoveBall && cmd.ball_shot_type==Robot::BallShotOnGoal)
        {
            t = (MyVector::distance(s.r_pos,s.ball_pos) +
                 MyVector::distance(s.ball_pos,cmd.target)) / 10000.0;
        }
        else
        {
            t = (MyVector::distance(s.r_pos,s.ball_pos) +
                 MyVector::distance(s.ball_pos,cmd.target)) / 1000.0;
        }
        t += fabs(anglemod((s.ball_pos - s.r_pos).angle() -
                           (cmd.target - s.ball_pos).angle())) / 1.5;
        nobs += world.obsLineNum(s.r_pos, s.ball_pos,
                                 OBS_EVERYTHING_BUT_ME(my_id) & ~OBS_BALL);
        nobs += world.obsLineNum(s.ball_pos, cmd.target,
                                 OBS_EVERYTHING_BUT_ME(my_id) & ~OBS_BALL);
        t += (nobs * M_PI * ROBOT_DEF_WIDTH_H) / 500.0;
        break;
    case CmdSteal:
        t = MyVector::distance(s.r_pos,s.ball_pos) / 1000.0;
        nobs += world.obsLineNum(s.r_pos, s.ball_pos,
                                 OBS_EVERYTHING_BUT_ME(my_id) & ~OBS_BALL);
        t += (nobs * M_PI * ROBOT_DEF_WIDTH_H) / 500.0;
        break;
    case CmdRecieveBall:
    case CmdSpin:
        t = MyVector::distance(s.r_pos,s.ball_pos) / 1000.0;
        break;
    case CmdPosition:
        t = MyVector::distance(s.r_pos,cmd.target) / 1000.0;
        nobs += world.obsLineNum(s.r_pos, cmd.target,
                                 OBS_EVERYTHING_BUT_ME(my_id) & ~OBS_BALL);
        t += (nobs * M_PI * ROBOT_DEF_WIDTH_H) / 500.0;
        break;
    default:
        t = 0;
    }
    return(t);
}
