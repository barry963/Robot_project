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


const bool robot_print     = true;//lu_test change all to true
const bool robot_sub_state = true;
const bool robot_debug_die = true;


//��װ�������������Ϊ�������˾ͽ��Ļ��������Ȳ�ȡ�ж���ԭ�������˻�������Ϊ��ͬ��ɫ��Աʱ����Ϊ�ͷ�Ӧ���̡�
const char *state_name[] =
{
    "SMGotoBall",				//������
    "SMFaceBall",				//������
    "SMApproachBall",		//�ӽ���
    "SMPullBall",				//����
    "SMFaceTarget",			//����Ŀ��
    "SMDriveToGoal",		//��������
    "SMKick",						//�������
    "SMSpinAtBall",			//����ǰ��ת
    "SMPosition",				//��λ
    "SMRecieveBall",		//����
    "SMWait"						//����
};
//�������ҷ����������˿��ܴ��ڵĸ���״̬ģʽ�����������Ӧ��״̬��������

//�������������ڳ�����
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

//���������ϵ���Ϣ�����ҵĵ�ǰλ�á��Ƕȡ��ٶȡ����ٶȣ��ҵ��Է������е���������ϰ��������ȵ�;
void Robot::updateSensors(World &world)
{
    Sensors &s = sensors;
    VisionRawInfo vpos;
    double f;
    //==== update high level sensors ====//
    // find out current position
    //��ǰ������λ��
    s.r_pos = world.GetRobotPositionByID(my_id);
    //��ǰ�����˽Ƕ�
    s.r_ang = world.teammate_direction(my_id);
    //��ǰ�������ٶ�
    s.r_vel = world.GetRobotVelocityByID(my_id);
    //��ǰ�����˽��ٶ�
    s.r_ang_vel = world.teammate_angular_velocity(my_id);
    //
    s.r_fwd.set(cos(s.r_ang),sin(s.r_ang));
    s.opp_goal_rel = opp_goal_pos - s.r_pos;
    // ball information
    //��λ��
    s.ball_pos  = world.ball_position();
    //���ٶ�
    s.ball_vel  = world.ball_velocity();
    //����Ի���������
    s.ball_rel  = s.ball_pos - s.r_pos;
    //����Ի����˾���
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
    //���ڶԷ�������
    s.ball_in_opp_goal = ((s.ball_pos.x > FIELD_LENGTH_H+2*BALL_RADIUS)  && (fabs(s.ball_pos.y) < GOAL_WIDTH_H) );
    //����Ի�����ʸ����ת��X�᷽��
    s.robot_ball = s.ball_rel.rotate(-s.r_ang);
    //���ڻ������ڲ�
    s.ball_in_front = (s.robot_ball.x > -20) &&
            (fabs(s.robot_ball.y) < 60);
    //���ڻ�����ǰ��
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
    //�����ٶ�1000Ԥ�⣬�������λ��
    t = max(s.ball_dist-200.0,0.0) / 1000.0;
    ball_pred = world.ball_position(world.now + t);
    // walk forward looking for free space
    //���������趨��max_behind������ǰ����Ѱ�ҿ��еص�
    //���ڽ������˴��������ĸ���160mm��Χ��
    obs_id = -1;
    for (behind=max_behind; behind>=0.0; behind-=10.0)
    {
        //target_rel Ŀ�ĵ���Ի�����λ�ò�ʸ��
        targ = ball_pred - target_rel.norm(behind);
        //�������������ڳ�����
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
    //�������ľ������ڻ����˼���뾶�������ڻ������ڣ���ɾ�����ϰ��趨
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
    //������������򸽽�Ŀ������
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
                    //�����ǽӽ�Ŀ���
                    last_dist_from_target<=targ_dist)
            {
                // printf("  %f <= %f\n",last_dist_from_target,targ_dist);
                //���ڻ�����ǰ�����ܹ�����
                if (s.can_drive && candriver)
                {
                    //printf("gotoball %3.2f,%3.2f\r\n",target_ball_rel.x,target_ball_rel.y);
                    return(SMDriveToGoal);
                    /*}else if(fabs(target_ball_rel.x) < 0.0){
                                           return(SMSpinAtBall);*/
                }
                //���ٶ�
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
    //�������Է�����
    nav.obs  &= ~(OBS_THEIR_DZONE);
    last_dist_from_target = targ_dist;
    if (robot_print)
    {
        printf("  targ=<%f,%f> targ_dist = %f\n",V2COMP(targ),targ_dist);
    }
    return(SMGotoBall);
}

//ʵ��������
Robot::SMState Robot::faceBall(World &world,Sensors &s,RobotCommand &cmd,
                               NavTarget &nav)
{
    double da,dta;
    //da=�������������߽Ƕ�������˱���ǶȲ�
    da  = angle_mod(s.ball_rel.angle() - s.r_ang);
    dta = angle_mod(target_rel.angle() - s.r_ang);
    //����Ƕ�С��0.1����
    if (fabs(da) < 0.1)
    {
        if (s.can_drive)
        {
            //�����������Ա
            return(SMDriveToGoal);
        }
        else
        {
            //����Ƕȹ���,��ת������,������
            //�ӽ���
            return(SMApproachBall);
        }
    }
    //�������ṹ
    nav.direct = true;
    //�趨��ת��
    nav.vel_xya.set(0,0,6*sin(da));
    return(SMFaceBall);
}

//������
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
    //inOurDefenseZone ������Ƿ����ҷ�������
    //�������ľ������300���������ҷ�������
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
        // return((s.ball_dist_from_wall > 100)? SMFaceTarget : SMPullBall);
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

//���������
Robot::SMState Robot::pullBall(World &world,Sensors &s,RobotCommand &cmd,
                               NavTarget &nav)
{
    double vx;
    if (time_in_state > 1.0)
    {
        return(SMFaceTarget);
    }
    //������ǰ
    if (!s.ball_on_front && s.ball_conf>0.5)
    {
        return(SMGotoBall);
    }
    //�����ٶ�������
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
    //��������
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

//�����������
//carry the ball or shoot
Robot::SMState Robot::driveToGoal(World &world,Sensors &s,RobotCommand &cmd,
                                  NavTarget &nav)
{
    //��ײ����������λ�ò�ʸ��
    MyVector2d ball_to_target = cmd.ball_target-s.ball_pos;
    // double carrot_dist = max(500.0-s.ball_dist,1.0);
    // MyVector2d carrot_pos = s.ball_pos + ball_to_target.norm(carrot_dist);
    double t,dba;
    double tdist,tbdist;
    //s.ball_rel ����Ի��������� target_rel Ŀ�ĵ���Ի�����λ�ò�ʸ��
    //����������ʸ����λ����ĵ��
    t = cosine(s.ball_rel,target_rel);
    //������Ҫ�Ƕ�������̬�ǶȲ�
    dba = angle_mod(ball_to_target.angle() - s.r_ang);
    //�����˾���Ŀ������
    tdist  = target_rel.length();
    //�����˾������ײ�������
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
            //�������������ٶȲ��100
            MyVector::dot(s.r_vel,s.ball_rel.norm()) > 100.0 &&
            (fabs(dba) < cmd.angle_tolerance))
    {
        switch (cmd.ball_shot_type)
        {
        case BallShotOnGoal:
        case BallShotPass:
            //5000=���ٶ�10000*0.5��
            //�Ƕȱ���25%������
            if ((tbdist > 100) && (fabs(dba) < cmd.angle_tolerance*0.75) )//&& (tbdist < 5000) )
            {
                return(SMKick);
            }
            break;
            //����յ�
        case BallShotClear:
            // if(!omni || s.on_goal) return(SMKick);
            //��ֹ����������أ�0.5���ȴ��ڵ���30��
            //���ڽ�������յ�
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
    //����ǽֻ��50,��ת������볡��
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
    //�����������״̬ʱ�䳤��ӭ����
    if (time_in_state>0.25+1.75*omni)
    {
        return(SMGotoBall);
    }
    //������ڻ������ڲ����û�����ӭ����
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
        printf("Kick! (t=%f,power=%d)\n",time_in_state,nav.kick_power);
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

//�������˶���ָ������
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

//�����������
Robot::SMState Robot::recieveBall(World &world,Sensors &s,RobotCommand &cmd,
                                  NavTarget &nav)
{
    double a,da;
    //���������ǰ,�����������ĽǶȲȻ��ת����
    if (!s.ball_on_front)
    {
        a  = s.ball_rel.angle();
        da = angle_mod(a - s.r_ang);
    }
    //������ת��
    else
    {
        da = 0;
    }
    //����������
    nav.dribble_power = 15;
    nav.direct = true;
    nav.vel_xya.set(0,0,3*sin(da));
    return(SMRecieveBall);
}

//�����˵ȴ�
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
    //�������
    if (cmd.cmd != last_cmd)
    {
        switch (cmd.cmd)
        {
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
    //Ŀ�ĵ���Ի�����λ�ò�ʸ��
    target_rel = cmd.target - s.r_pos;
    //
    target_ball_rel.set(s.ball_rel.dot(target_rel.norm()),
                        s.ball_rel.dot(target_rel.norm().perp()));
    // target_ball_rel = s.ball_rel.rotate(-(cmd.target-s.r_pos).angle());
    // execute states until no longer switching
    //ѭ��ִ��,ֱ�������л�״̬
    const int maxloop=10;
    n = maxloop;
    state_changed = false;
    do
    {

        old_state = state;
        //�����ڱ�״̬�б��ֵ�ʱ��
        time_in_state = world.time - state_start_time;
        mzero(nav);
        nav.obs = OBS_EVERYTHING_BUT_ME(my_id);
        if (robot_sub_state)
        {
            printf("SubState [%s]  execute times: %d\n",state_name[state],n-maxloop+1);
        }
        switch (state)
        {
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
            //switch(state){
            //	case SMGotoBall:
            //		printf("SMGotoBall\r\n");break;
            //	case SMFaceBall:
            //		printf("SMFaceBall\r\n");break;
            //	case SMApproachBall:
            //		printf("SMApproachBall\r\n");break;
            //	case SMPullBall:
            //		printf("SMPullBall\r\n");break;
            //	case SMFaceTarget:
            //		printf("SMFaceTarget\r\n");break;
            //	case SMDriveToGoal:
            //		printf("SMDriveToGoal\r\n");break;
            //	case SMKick:
            //		printf("SMKick\r\n");break;
            //	case SMSpinAtBall:
            //		printf("SMSpinAtBall\r\n");break;
            //	case SMPosition:
            //		printf("SMPosition\r\n");break;
            //	case SMRecieveBall:
            //		printf("SMRecieveBall\r\n");break;
            //	case SMWait:
            //		printf("SMWait\r\n");break;
            //}
        }

    } while (state!=old_state && --n);

    nav.DataDisplay();//lu_test add

    if (n <= 0)
    {
        //��������10��״̬�仯�����ǲ��������
        printf("Robot::Oscillation Error! in class Robot::run\n");
        nav.direct = true;
        nav.vel_xya.set(100,0,0);
        if (robot_debug_die)
        {
            exit(1);
        }
    }

    if (robot_print)
    {
        printf("State: %s %0.2fs R(%8.2f,%8.2f)\n",
               state_name[state],time_in_state, V2COMP(s.r_pos));
    }
    gui_debug_printf(my_id, GDBG_TACTICS,
                     "State: %s %0.2fs\n",state_name[state],time_in_state);
    gui_debug_printf(my_id, GDBG_TACTICS,
                     "  robot_ball=<%f,%f>\n",V2COMP(s.robot_ball));
    if (robot_print)
    {
        printf("  robot_ball=<%f,%f>:%d target_ball_rel=<%f,%f> stuck=%f\n",
               V2COMP(s.robot_ball),s.ball_on_front,
               V2COMP(target_ball_rel),
               world.teammate_stuck(my_id));
   }
    // carry out target command
    //ֱ���趨�����˵��ٶȲ���
    if (nav.direct)
    {
        tcmd.vx = nav.vel_xya.x;
        tcmd.vy = nav.vel_xya.y;
        tcmd.va = nav.vel_xya.z;
                qDebug()<<"direct speed setting";

    }
    //ͨ��·������ʵ���ٶȿ���
    else
    {
        if (nav.spin)
        {
            qDebug()<<"spin to point";

            //��ת��ʽ��ָ���㣬������ϰ�
            tcmd = spin_to_point(world,my_id,nav.pos,nav.angle);
        }
        else
        {
            qDebug()<<"spin and direct to point";

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
            // gao mark
//            tcmd = goto_point(world,my_id,nav.pos,nav.vel,nav.angle,
//                                   nav.type);
            tcmd = nav_to_point(world,my_id,
                                nav.pos,nav.vel,nav.angle,
                                nav.obs,nav.type);

            tcmd.DataDisplay();//lu_test add
            MyVector2d current_position = world.GetRobotPositionByID(my_id);
            qDebug()<< "get position x:"<<current_position.x<<"y:"<<current_position.y;


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
    // if(robot_print && nav.kick) printf("Kick!\n");
    //���������Ƿ����
    switch (cmd.cmd)
    {
    case CmdMoveBall:
        status = (nav.kick_power)? Completed : InProgress;
        break;
        //�����͵��
    case CmdSteal:
        status = InProgress;
        break;
        //�ǽ��մ�������,������Ѿ�����ǰ,�����
    case CmdRecieveBall:
        status = (s.ball_on_front)? Completed : InProgress;
        break;
        //����Ǵ���,�Ͷ�λ,���Һ�Ŀ�����С��20mm�����������
    case CmdSpin:
    case CmdDribble:
    case CmdPosition:
        //20=����ٶ�/60;
        qDebug()<<"distance between target and raw position: "<<MyVector::distance(cmd.target,s.r_pos);
        if (MyVector::distance(cmd.target,s.r_pos) < 20.0)
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
        //��ָ���Ļ����˷�������ʹ�������
        //send shoot and dribble command to the specific robot
        world.go(my_id,tcmd.vx,tcmd.vy,tcmd.va,tcmd.kick_power,tcmd.bchipkick,tcmd.dribble_power);
    }
    return(s);
}

//�����˶���ʱ������,���ڻ����˽�ɫѡ������
//���ú��� obsLineNum
//�����p1��p2ֱ�����ж��ٸ��ϰ�
//�ж�����ϰ��������(�����򣬶��ѺͶԷ���Ա);
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
