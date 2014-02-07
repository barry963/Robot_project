// tactic.cc
//
// Parent class for tactics.
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu)
//

// ************************************************************************************************
//     Copyright 2013-2014 modified by Lu Chunqiu
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//
//
//
//  **********************************************************************************************/


/* LICENSE:

 ************************************************************************************************
     Copyright 2013-2014 modified by Lu Chunqiu

     This software is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

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

#include "user_interface/field_related/motion_simulation_tools/field_util.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"

#include "user_interface/field_related/field_global_function.h"
#include "field_world_robot.h"
//#include "base.h"

static bool cr_setup_omni = false;
CR_DECLARE(OMNI_MAX_ACCEL);
CR_DECLARE(OMNI_MAX_SPEED);
CR_DECLARE(OMNI_MAX_ANG_ACCEL);
CR_DECLARE(OMNI_MAX_ANG_VEL);
CR_DECLARE(OMNI_ACCEL_FACTOR);
CR_DECLARE(OMNI_ANG_ACCEL_FACTOR);

static bool cr_setup_diff = false;
CR_DECLARE(DIFF_MAX_ACCEL);
CR_DECLARE(DIFF_MAX_SPEED);
CR_DECLARE(DIFF_MAX_ANG_ACCEL);
CR_DECLARE(DIFF_MAX_ANG_VEL_MOVING);
CR_DECLARE(DIFF_MAX_ANG_VEL_TURNING);
CR_DECLARE(DIFF_ACCEL_FACTOR);
CR_DECLARE(DIFF_POSITION_TOLERANCE);
CR_DECLARE(DIFF_USE_GOTOPOINT_SPEED);

static bool cr_setup_robot = false;
CR_DECLARE(NAV_OUR_OBSTACLE_RADIUS);
CR_DECLARE(NAV_THEIR_OBSTACLE_RADIUS);
CR_DECLARE(NAV_THEIR_GOALIE_OBSTACLE_RADIUS);


int debugfreq=0;//lu_test just for test

#define LU_VERSION //lu_test for new speed planning

//按照梯形速度曲线，计算运动消耗时间
float Robot::motion_time_1d(float dx,float vel0,float vel1,
                            float max_vel,float max_accel,
                            float &t_accel,float &t_cruise,float &t_decel)
// Calculates the timings for a trapezoidal velocity profile with
// constant acceleration.  'dx' is the distance to be traveled, and
// 'vel0' and 'vel1' are the desired initial and final velocities.
// 'max_vel' == maximum veloicty, 'max_accel' == maximum acceleration
// the individual timings are returned in the last three parameters:
// 't_accel' is the time spent in the acceleration phase, 't_cruise'
// is the time spent at maximum velocity, and 't_decel' is the time
// spent in the deceleration phase.  Any of these can be 0.  The total
// of the three times is returned by the function.
{
	float tmp,vmax;
	const bool debug = false;
	if (debug)
	{
		printf("  d%f v%f-%f m%f a%f = ",
		       dx,vel0,vel1,max_vel,max_accel);
	}
	if (dx < 0)
	{
		dx = -dx;
		vel0 = -vel0;
		vel1 = -vel1;
	}
	//限制最高速度
	if (vel0 > max_vel) vel0 = max_vel;
	if (vel1 > max_vel) vel1 = max_vel;
	// stop  降低速度
	if (vel0 > vel1)
	{
		//计算减速时间
		t_decel = (vel0 + vel1) / 2*dx;
		if (debug) printf("<%f> ",fabs(vel1 - vel0)/t_decel);
		if (fabs(vel1 - vel0)/t_decel > max_accel)
		{
			t_accel = t_cruise = 0;
			if (debug)
			{
				printf("\n");
			}
			return(t_decel);
		}
	}
	// calculate time spent at max velocity
	tmp = 2*max_accel*dx + sq(vel0) + sq(vel1);
	//巡航时间
	t_cruise = (tmp - 2*sq(max_vel)) / (2*max_vel*max_accel);
	if (t_cruise > 0)
	{
		vmax = max_vel;
	}
	else
	{
		vmax = sqrt((max_accel*dx + sq(vel0) + sq(vel1))/2);
		t_cruise = 0;
	}
	if (debug)
	{
		printf("[am%f] ",vmax);
	}
	t_accel = max(vmax - vel0,(float)0.0) / max_accel;
	t_decel = fabs(vmax - vel1) / max_accel;
	if (debug)
	{
		printf("t(%f,%f,%f):%f\n",
		       t_accel,t_cruise,t_decel,t_accel+t_cruise+t_decel);
	}
	return(t_accel + t_cruise + t_decel);
}

//从速度0开始，在距离=dx情况下，最大能提高的速度
double Robot::max_speed(double dx,double max_a)
// maximum speed we will be willing to go if we are dx away from
// target and have an acceleration of max_a
{
	return(sqrt(2*max_a*dx));
}

#if 0
void Robot::compute_motion_1d(double x0, double v0, double v1,
                              double a_max, double v_max, double a_factor,
                              double &traj_accel, double &traj_time)
{
	float ta,tc,td;
	ta = tc = td = 0.0;
	traj_time = motion_time_1d(x0,v0,v1,v_max,a_max/a_factor,ta,tc,td);
	if (ta > 0.0)
	{
		traj_accel = -a_max * sign(x0);
	}
	else if (tc > 0.0)
	{
		traj_accel = 0.0;
	}
	else
	{
		traj_accel =  a_max * sign(x0);
	}
	printf("  accel=%f\n",traj_accel);
}
#endif

#if 1
/*
a_max=12*(0.5->1.0)
v_max=6
a_factor=1.1
*/

void Robot::compute_motion_1d(double x0, double v0, double v1,
                              double a_max, double v_max, double a_factor,
                              double &traj_accel, double &traj_time)
{
	// First check to see if nothing needs to be done...
	//检查是否没有什么需要做的
	if (x0 == 0 && v0 == v1)
	{
		traj_accel = 0;
		traj_time = 0;
		return;
	}
	if (!finite(x0))
	{
		printf("1 Robot::compute_motion_1d: NANs!\n");
		traj_accel = 0;
		traj_time = 0;
		return;
	}
	if (!finite(v0))
	{
		printf("2 Robot::compute_motion_1d: NANs!\n");
		traj_accel = 0;
		traj_time = 0;
		return;
	}
	if (!finite(v1))
	{
		printf("2 Robot::compute_motion_1d: NANs!\n");
		traj_accel = 0;
		traj_time = 0;
		return;
	}
	// Need to do some motion.
	a_max /= a_factor;
	//计算从v0到v1速度需要消耗时间
	double time_to_v1 = fabs(v0 - v1) / a_max;
	//计算从速度v0到v1需要行走距离
	double x_to_v1 = fabs((v0 + v1) / 2.0) * time_to_v1;
	//设定刹车需要的时间范围
	double period = 2.0 * FRAME_PERIOD;
	//采用v1的绝对值 -x0的符号
	v1 = copysign(v1, -x0);
	if (v0 * x0 > 0 || (fabs(v0) > fabs(v1) && x_to_v1 > fabs(x0)))
	{
		// Time to reach goal after stopping + Time to stop.
		//v0刹车需要的时间
		double time_to_stop = fabs(v0) / a_max;
		//v0刹车需要的距离
		double x_to_stop = v0 * v0 / (2 * a_max);
		//计算反向后升速到v1需要花费的时间
		compute_motion_1d(x0 + copysign(x_to_stop, v0), 0, v1, a_max * a_factor,
		                  v_max, a_factor, traj_accel, traj_time);
		//计算总花费时间
		traj_time += time_to_stop;
		// Decelerate 减速
		//如果两帧内完成减速然后反向升速动作，计算加速度
		if (traj_time < period)
		{
			traj_accel = compute_stop(v0, a_max * a_factor);
		}
		//如果在两帧内完成刹车
		else if (time_to_stop < period)
		{
			traj_accel = time_to_stop / period * - copysign(a_max * a_factor, v0) +
			             (1.0 - time_to_stop / period) * traj_accel;
		}
		else
		{
			traj_accel = -copysign(a_max * a_factor, v0);
		}
		return;
	}
	// At this point we have two options.  We can maximally accelerate
	// and then maximally decelerate to hit the target.  Or we could
	// find a single acceleration that would reach the target with zero
	// velocity.  The later is useful when we are close to the target
	// where the former is less stable.
	//有两个方法：
	//1，采用最大加速度加速，然后采用最大减速度减速，达到目的地
	//2，单一加速度，到达目的地后速度等于0.
	//后一方法在接近目标时有用，前一方法稳定性较差
	//------------------------------------------------------------------------------
	//考虑只有升速或者只有降速的情况
	// OPTION 1
	// This computes the amount of time to accelerate before decelerating.
	double t_a, t_accel, t_decel;
	//以下为fabs(v1)>=fabs(v0)的情况
	if (x_to_v1 >= fabs(x0))
	{
		if (fabs(v0) > fabs(v1))
		{
			//如果距离不足以将速度变到v1,表示只有升速过程
			t_a = (sqrt(v1 * v1 + 2 * a_max * fabs(x0)) - fabs(v1)) / a_max;
			t_accel = 0.0;
			t_decel = t_a;
		}
		else
		{
			//如果距离不足以将速度变到v1,表示只有升速过程
			t_a = (sqrt(v0 * v0 + 2 * a_max * fabs(x0)) - fabs(v0)) / a_max;
			t_accel = t_a;
			t_decel = 0.0;
		}
	}
	else
	{
		if (fabs(v0) > fabs(v1))
		{
			//只有降速
			//计算v0到v1降速需要消耗的时间
			//    t_a = (sqrt((3*v1*v1 + v0*v0) / 2.0 - fabs(v0 * v1) + fabs(x0) * a_max)
			//	   - fabs(v0)) / a_max;
			t_a = (sqrt((v0 * v0 + v1 * v1) / 2.0 + fabs(x0) * a_max)  - fabs(v0)) / a_max;
			if (t_a < 0.0) t_a = 0;
			t_accel = t_a;
			t_decel = t_a + time_to_v1;
		}
		else
		{
			//    t_a = (sqrt((3*v0*v0 + v1*v1) / 2.0 - fabs(v0 * v1) + fabs(x0) * a_max)
			//  - fabs(v1)) / a_max;
			//从v0升速到v1需要消耗的时间
			t_a = (sqrt((v0 * v0 + v1 * v1) / 2.0 + fabs(x0) * a_max) - fabs(v1)) / a_max;
			if (t_a < 0.0) t_a = 0;
			t_accel = t_a + time_to_v1;
			t_decel = t_a;
		}
	}
	// OPTION 2
	//double a_to_v1_at_x0 = (v0 * v0 - v1 * v1) / (2 * fabs(x0));
	//double t_to_v1_at_x0 = (-fabs(v0) + sqrt(v0 * v0 + 2 * fabs(a_to_v1_at_x0) * fabs(x0))) / fabs(a_to_v1_at_x0);
	//// We follow OPTION 2 if t_a is less than a FRAME_PERIOD making it
	//// difficult to transition to decelerating and stopping exactly.
	//if (0 && a_to_v1_at_x0 < a_max && a_to_v1_at_x0 > 0.0 &&
	//    t_to_v1_at_x0 < 2.0 * FRAME_PERIOD && 0)
	//{
	//  // OPTION 2
	//  // Use option 1 time, even though we're not following it.
	//  traj_time = t_accel + t_decel;;
	//  // Target acceleration to stop at x0.
	//  traj_accel = -copysign(a_to_v1_at_x0, v0);
	//  return;
	//}
	//else
	{
		// OPTION 1
		// Time to accelerate and decelerate.
		traj_time = t_accel + t_decel;
		// If the acceleration time would get the speed above v_max, then
		//  we need to add time to account for cruising at max speed.
		if (t_accel * a_max + fabs(v0) > v_max)
		{
			traj_time +=
			    pow(v_max - (a_max * t_accel + fabs(v0)), 2.0) / a_max / v_max;
		}
		// Accelerate (unless t_accel is less than FRAME_PERIOD, then set
		// acceleration to average acceleration over the period.)
		if (t_accel < period && t_decel == 0.0)
		{
			traj_accel = copysign(a_max * a_factor, -x0);
		}
		else if (t_accel < period && t_decel > 0.0)
		{
			traj_accel = compute_stop(v0, a_max * a_factor);
		}
		else if (t_accel < period)
		{
			traj_accel = copysign((2.0 * t_accel / (period) - 1) * a_max * a_factor, v0);
		}
		else
		{
			traj_accel = copysign(a_max * a_factor, -x0);
		}
	}
}

#endif

#if 0
//功能:
//运动控制计算,计算加速度和总消耗时间
//参数:
//x0需要行走距离
//v0初始速度
//v1终点速度
//a_max最大加速度
//v_max最大速度
//a_factory 比例因子

void Robot::compute_motion_1d(double x0, double v0, double v1,
                              double a_max, double v_max, double a_factor,
                              double &traj_accel, double &traj_time)
{
	// First check to see if nothing needs to be done...
	//检查是否没有什么需要做的
	if (x0 == 0 && v0 == v1)
	{
		traj_accel = 0;
		traj_time = 0;
		return;
	}
	if (!finite(x0) || !finite(v0) || !finite(v1))
	{
		printf("Robot::compute_motion_1d: NANs!\n");
		traj_accel = 0;
		traj_time = 0;
		return;
	}
	//通过配置文件配置a_factor的值，用于调节加速度
	a_max /= a_factor;
	//设定刹车需要的时间范围
	double period = 2.0 * FRAME_PERIOD;
	double dv=a_max * FRAME_PERIOD;
	//计算从v0到v1速度需要消耗时间
	double time_to_v1 = fabs(v0 - v1) / a_max;
	double time_to_v1_round=(int)(time_to_v1/FRAME_PERIOD)*FRAME_PERIOD+period;
	//计算从速度v0到v1需要行走距离
	double x_to_v1 = fabs((v0 + v1) / 2.0) * time_to_v1_round ;
	//有两个方法：
	//1，采用最大加速度加速，然后采用最大减速度减速，达到目的地
	//2，单一加速度，到达目的地后速度等于0.
	//后一方法在接近目标时有用，前一方法稳定性较差
	//------------------------------------------------------------------------------
	//考虑没有最高速度限制，从v0升速到某个不受限制的最高速度vtop后，然后没有匀速，直接降速到v1的过程
	double t_a, t_accel, t_decel,t_avg,vtop;
	int ip0=-1,ip1=-1;
	int iadir=1;
	if (x0*v0>0)
	{
		//速度和距离相反则只能降速
		t_accel = 0.0;
		t_decel = fabs(v0) / a_max;
		if (v0>0)iadir=-1;
		t_avg = 0;
		ip0=0;
	}
	else if ( x_to_v1 >= fabs(x0) )
	{
		vtop=max(v0,v1);
		//如果距离不足以将速度变到v1,表示只有升速或者降速过程
		t_a = fabs(v1-v0) / a_max;
		if ( v0 > v1)
		{
			//降速
			t_accel = 0.0;
			t_decel = t_a;
			if ( v0 > 0 )iadir = -1;
			ip0=1;
		}
		else
		{
			//升速
			t_accel = t_a;
			t_decel = 0.0;
			if ( v0 > 0 )iadir = -1;
			ip0=2;
		}
		t_avg=0;
	}
	else
	{
		//包含匀速过程
		vtop=sqrt((v0 * v0 + v1 * v1) / 2.0 + fabs(x0) * a_max);
		if (v0<0 && v1<0)
		{
		}
		vtop=min(vtop,v_max);
		dv=vtop*FRAME_PERIOD;
		if (v0<vtop-2*dv)
		{
			//升速
			t_accel = fabs(vtop  - v0) / a_max;
			t_decel = fabs(vtop  - v1) / a_max;
			t_avg= (fabs(x0)-(vtop+v0)*t_accel/2-(vtop+v1)*t_decel/2)/v_max;
			if (v0<0)iadir=-1;
			ip0=3;
		}
		else if (v0>=vtop-dv)
		{
			//匀速
			t_accel = 0;
			t_decel = fabs(vtop  - v1) / a_max;
			t_avg= (fabs(x0)-(vtop+v1)*t_decel/2)/vtop;
			ip0=4;
		}
		else
		{
			//降速
			t_accel = 0;
			t_decel = fabs(vtop  - v1) / a_max;
			t_avg= 0;
			if (v0>0)iadir=-1;
			ip0=5;
		}
		if (t_avg<0)t_avg=0;
	}
	//-----------------------------------------------------
	if (t_accel == 0.0 )
	{
		if (t_avg == 0.0 )
		{
			//只有降速过程
			if (t_decel >= period)
			{
				//快速降速
				ip1=0;
				traj_accel = a_max * a_factor;
				//traj_accel = _copysign(traj_accel,-v0);
			}
			else if (t_decel == 0.0)
			{
				//静止
				ip1=1;
				traj_accel=0;
			}
			else
			{
				ip1=2;
				//缓慢降速
				traj_accel = min(fabs(v1-v0)/t_decel,a_max*a_factor);
				//traj_accel = _copysign(traj_accel,-v0);
				t_decel=period;
			}
		}
		else
		{
			//匀速过程
			ip1=3;
			traj_accel=0;
		}
	}
	else
	{
		//加速过程
		ip1=4;
		traj_accel = a_max * a_factor;
	}
	traj_accel=traj_accel*iadir;
	traj_time = t_accel + t_decel + t_avg;
	//static double olda=0;
	//if(olda>0.1 && traj_accel<0.1)
	//{
	//	printf("+");
	//}
	//else
	//{
	//	printf(" ");
	//}
	//olda=traj_accel;
	//printf("x=%2.1f,l=%2.1f,v0=%2.1f,t+=%3.2f,ta=%3.2f,t-=%3.2f,a=%3.2f,p=%d%d\r\n",x0,x_to_v1,v0,t_accel,t_avg,t_decel,traj_accel,ip0,ip1);
}
#endif

//输入:
//x0 需要行走长度矢量
//v0 当前速度
//v1 目标速度
//a_max 最大加速度
//v_max 最大速度
//a_factor 比例因子
//输出:
//traj_accel 加速度
//time需要消耗时间
void Robot::compute_motion_2d(MyVector2d x0, MyVector2d v0, MyVector2d v1,
                              double a_max, double v_max, double a_factor,
                              MyVector2d &traj_accel, double &time)
{
	double time_x, time_y;
	double rotangle;
	if (v1.length() > 0.0) rotangle = v1.angle();
	else rotangle = 0.0;
	x0 = x0.rotate(-rotangle);
	v0 = v0.rotate(-rotangle);
	v1 = v1.rotate(-rotangle);
	//计算x方向需要的加速度和消耗的时间
	compute_motion_1d(x0.x, v0.x, v1.x, a_max, v_max, a_factor,
	                  traj_accel.x, time_x);
	//计算y方向需要的加速度和消耗的时间
	compute_motion_1d(x0.y, v0.y, v1.y, a_max, v_max, a_factor,
	                  traj_accel.y, time_y);
	if (v1.length() == 0.0)
	{
		//如果目的速度等于0
                double rx = time_x / hypot(time_x, time_y);
                double ry = time_y / hypot(time_x, time_y);
		compute_motion_1d(x0.x, v0.x, v1.x, a_max * rx, v_max * rx, a_factor,
		                  traj_accel.x, time_x);
		compute_motion_1d(x0.y, v0.y, v1.y, a_max * ry, v_max * ry, a_factor,
		                  traj_accel.y, time_y);
	}
	else
	{
		if (time_x < time_y * 1.5)
		{
			double rx_a = pow(time_x / (time_y * 1.5), 2.0);
			double rx_v = time_x / (time_y * 1.5);
			compute_motion_1d(x0.x, v0.x, v1.x, a_max * rx_a, v_max * rx_v, a_factor, traj_accel.x, time_x);
		}
	}
	traj_accel = traj_accel.rotate(rotangle);
	time = MAX(time_x, time_y);
}

//计算刹车加速度
double Robot::compute_stop(double v, double max_a)
{
	if (fabs(v) > max_a * FRAME_PERIOD)
	{
		return copysign(max_a, -v);
	}
	//如果还一帧内可以刹车完成，修改加速度
	else
	{
		return -v / FRAME_PERIOD;
	}
}

//当前机器人根据场上形势(world)，以相应的目标速度(target_vel)、目标角度(target_ang)和
//运动类型(type)运动到指定目标点(target_pos)的处理函数，其中包含了对运动状态的分析和计算
Robot::Trajectory Robot::goto_point(World &world, int me,
                                    MyVector2d target_pos, MyVector2d target_vel,
                                    double target_ang,GotoPointType type)
{
	if (!cr_setup_omni)
	{
		CR_SETUP(motion, OMNI_MAX_ACCEL, CR_DOUBLE);
		CR_SETUP(motion, OMNI_MAX_SPEED, CR_DOUBLE);
		CR_SETUP(motion, OMNI_MAX_ANG_ACCEL, CR_DOUBLE);
		CR_SETUP(motion, OMNI_MAX_ANG_VEL, CR_DOUBLE);
		CR_SETUP(motion, OMNI_ACCEL_FACTOR, CR_DOUBLE);
		CR_SETUP(motion, OMNI_ANG_ACCEL_FACTOR, CR_DOUBLE);
		cr_setup_omni = true;
	}
	if (!cr_setup_diff)
	{
		CR_SETUP(motion, DIFF_MAX_ACCEL, CR_DOUBLE);
		CR_SETUP(motion, DIFF_MAX_SPEED, CR_DOUBLE);
		CR_SETUP(motion, DIFF_MAX_ANG_ACCEL, CR_DOUBLE);
		CR_SETUP(motion, DIFF_MAX_ANG_VEL_MOVING, CR_DOUBLE);
		CR_SETUP(motion, DIFF_MAX_ANG_VEL_TURNING, CR_DOUBLE);
		CR_SETUP(motion, DIFF_ACCEL_FACTOR, CR_DOUBLE);
		CR_SETUP(motion, DIFF_POSITION_TOLERANCE, CR_DOUBLE);
		CR_SETUP(motion, DIFF_USE_GOTOPOINT_SPEED, CR_INT);
		cr_setup_diff = true;
	}
	if (0)
	{
		//gui_debug_line(me, GDBG_MOTION, world.teammate_position(me), target_pos, G_ARROW_FORW);
	}
	//gui_debug_line(me, GDBG_MOTION,
	//   target_pos,
	//   target_pos + target_vel,
	//   G_ARROW_FORW);
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		if (IVAR(DIFF_USE_GOTOPOINT_SPEED))
		{
			return goto_point_speed(world, me, target_pos,
			                        target_vel, target_ang, type);
		}
		else
		{
			return goto_point_diff(world, me, target_pos,
			                       target_vel, target_ang, type);
		}
	}
	else
	{
		return goto_point_omni(world, me, target_pos,
		                       target_vel, target_ang, type);
	}
}

Robot::Trajectory Robot::goto_point_omni(World &world, int me,
        MyVector2d target_pos,
        MyVector2d target_vel,
        double target_ang,
        GotoPointType the_type)
{
	//计算当前点到目的点向量
        MyVector2d x = world.GetRobotPositionByID(me) - target_pos;

	if (!finite(x.x))
	{
		Trajectory t(0, 0, 0, 0);
		t.bValid=false;
		return t;
	}
	//计算速度向量
    MyVector2d v = world.GetRobotVelocityByID(me);
	//计算当前角度与目的角度差
	double dangleme=world.teammate_direction(me);
    double ang = angle_mod(dangleme - target_ang);
	//查询当前角速度
	double ang_v = world.teammate_angular_velocity(me);
	MyVector2d a;
	double ang_a,factor_a;
	double time_a, time;
	int type = the_type;
	if (type == GotoPointMoveForw) type = GotoPointMove;
	//XY方向加速度和时间计算

    qDebug()<<"CurrentSpeed:("<<v.x<<","<<v.y<<","<<ang_v<<") AngDiff"<<ang*180/M_PI<<"\n";

//    if(ang<0.4||ang>6.24)//lu_test angle difference
//    {
//        ang=0.0;
//    }

	compute_motion_2d(x, v, target_vel,
	                  VDVAR(OMNI_MAX_ACCEL)[type],
	                  VDVAR(OMNI_MAX_SPEED)[type],
	                  DVAR(OMNI_ACCEL_FACTOR),
	                  a, time);
	//旋转加速度和时间计算
	//从0.4倍角速度开始计算，寻找一个能在XY运动到位之前就能完成旋转的角速度
	factor_a = 0.5 - 0.1;
	do
	{
		factor_a += 0.1;
		//if(!finite(ang))
		//{
		//	printf("omniang:NANs!%3.2f,%3.2f,%3.2f\r\n",ang,dangleme,target_ang);
		//}
		compute_motion_1d(ang, ang_v, 0.0,
		                  VDVAR(OMNI_MAX_ANG_ACCEL)[type]*factor_a,
		                  VDVAR(OMNI_MAX_ANG_VEL)[type],
		                  DVAR(OMNI_ANG_ACCEL_FACTOR),
		                  ang_a, time_a);
	}
	while (factor_a<1.0 && time_a>time);
	//计算新的速度
	v += a * FRAME_PERIOD;
	ang_v += ang_a * FRAME_PERIOD;
	//经过旋转后新的速度矢量
	v = v.rotate(-world.teammate_direction(me));
	//防止速度超过限制
	if (v.length() > VDVAR(OMNI_MAX_SPEED)[type])
	{
		v = v.norm() * VDVAR(OMNI_MAX_SPEED)[type];
	}

	//防止角速度超过限制
	ang_v = bound(ang_v,
	              -VDVAR(OMNI_MAX_ANG_VEL)[type],
	              VDVAR(OMNI_MAX_ANG_VEL)[type]);
	//  

	Trajectory t(v.x, v.y, ang_v, max(time,time_a));
    t.DataDisplay();
	return t;
}

Robot::Trajectory Robot::goto_point_diff(World &world, int me,
        MyVector2d target_pos,
        MyVector2d target_vel,
        double target_ang,
        GotoPointType the_type)
{
        MyVector2d x = world.GetRobotPositionByID(me) - target_pos;
        MyVector2d v = world.GetRobotVelocityByID(me);
	double ang = angle_mod(world.teammate_direction(me) - target_ang);
	double ang_v = world.teammate_angular_velocity(me);
	MyVector2d a;
	double time;
	bool forward_only = (the_type == GotoPointMoveForw);
	int type = the_type;
	if (type == GotoPointMoveForw) type = GotoPointMove;
	x = x.rotate(-world.teammate_direction(me));
	v = v.rotate(-world.teammate_direction(me));
	target_vel = target_vel.rotate(-world.teammate_direction(me));
	// We cannot do both direction and position simultaneously.  So we
	// only worry about direction if the position is within tolerances.
	if (target_vel.length() == 0.0 &&
	        x.length() < DVAR(DIFF_POSITION_TOLERANCE) &&
	        v.length() < VDVAR(DIFF_MAX_ACCEL)[type] * FRAME_PERIOD)
	{
		double ang_a;
		gui_debug_printf(me, GDBG_MOTION, "Turning: x = %f, v = %f\n", ang, ang_v);
		compute_motion_1d(ang, ang_v, 0.0,
		                  VDVAR(DIFF_MAX_ANG_ACCEL)[type],
		                  VDVAR(DIFF_MAX_ANG_VEL_TURNING)[type],
		                  DVAR(DIFF_ACCEL_FACTOR),
		                  ang_a, time);
		ang_v += ang_a * FRAME_PERIOD;
		ang_v = bound(ang_v,
		              -VDVAR(DIFF_MAX_ANG_VEL_TURNING)[type],
		              VDVAR(DIFF_MAX_ANG_VEL_TURNING)[type]);
		return Trajectory(0.0, 0.0, ang_v, 0.0);
	}
	// This allows some tolerance (ignoring) any small amounts of error
	// perpendicular position/velocity.  The sqrt(2) is needed otherwise
	// it falls in and out of some cases.
	if (fabs(x.y) < DVAR(DIFF_POSITION_TOLERANCE) / M_SQRT2)
		x.y = 0.0;
	if (fabs(target_vel.y) * FRAME_PERIOD <
	        DVAR(DIFF_POSITION_TOLERANCE) / M_SQRT2)
		target_vel.y = 0.0;
	// It turns out that we want to kill off our y velocity as quickly
	// as possible.  The best way to do this is use to act as if we want
	// to hit the point with velocity.
	if (target_vel.length() == 0.0 && x.length() > 0.0)
		target_vel = x.norm() * -0.1;
	compute_motion_2d(x, v, target_vel,
	                  VDVAR(DIFF_MAX_ACCEL)[type],
	                  VDVAR(DIFF_MAX_SPEED)[type],
	                  DVAR(DIFF_ACCEL_FACTOR),
	                  a, time);
	// Determine new velocities.
	v.x += a.x * FRAME_PERIOD;
	// We can extract angular velocity from the y component of the
	// acceleration.
	double new_ang_v;
	if (a.y == 0.0)
	{
		new_ang_v = 0.0;
	}
	else
	{
		new_ang_v = bound(a.y / (forward_only ? fabs(v.x) : v.x),
		                  -VDVAR(DIFF_MAX_ANG_VEL_MOVING)[type],
		                  VDVAR(DIFF_MAX_ANG_VEL_MOVING)[type]);
	}
	if (forward_only && v.x < 0.0)
	{
		v.x = 0.0;
	}
	// If the angular acceleration exceeds the bounds fix it.  If the
	// angular decelaration exceeds bounds, don't worry about it.
	//
	// This is necessary because the accelerations required to follow
	// the a.y/v.x based angular velocity may be larger than the robot
	// can perform.
	if (new_ang_v * ang_v > 0.0)
	{
		if (fabs(new_ang_v) - fabs(ang_v) > VDVAR(DIFF_MAX_ANG_ACCEL)[type] * FRAME_PERIOD)
		{
			new_ang_v = ang_v + copysign(VDVAR(DIFF_MAX_ANG_ACCEL)[type] * FRAME_PERIOD, new_ang_v - ang_v);
		}
	}
	else
	{
		if (fabs(new_ang_v) > VDVAR(DIFF_MAX_ANG_ACCEL)[type] * FRAME_PERIOD)
			new_ang_v = copysign(VDVAR(DIFF_MAX_ANG_ACCEL)[type] * FRAME_PERIOD,
			                     new_ang_v);
	}
	ang_v = new_ang_v;
	// Check bounds on velocity.
	if (v.length() > VDVAR(DIFF_MAX_SPEED)[type])
	{
		v = v.norm() * VDVAR(DIFF_MAX_SPEED)[type];
	}
	ang_v = bound(ang_v,
	              -VDVAR(DIFF_MAX_ANG_VEL_MOVING)[type],
	              VDVAR(DIFF_MAX_ANG_VEL_MOVING)[type]);
	return Trajectory(v.x, 0.0, ang_v, time);
}

Robot::Trajectory Robot::nav_to_point(World &world, int focused_robot_id,
                                      MyVector2d target_pos, MyVector2d target_vel,
                                      double target_angle,int obs_flags,
                                      GotoPointType type)
{
        obstacles obstacles_instance;
	::state initial,target,goal;
        MyVector2d robot_position,robot_velocity,current_position,current_velocity;
        double robot_speed,s; // relative speeds
	MyVector2d q,qr,obs_vel;
	MyVector2d ball,ball_vel;
        double current_face_angel,t,tmax,qrl,rad;
	double out_x,out_y;
	int obs_id;
	int goalie_id;
	int i;
	//if(!finite(target_pos.x))
	//{
    //   printf("nav_to_point x=: NANs!%3.2f,%3.2f\n",target_pos.x,target_pos.y);
	//}
	//if(!finite(target_angle))
	//{
//   printf("nav_to_point a=: NANs!%3.2f\n",target_angle);
	//}
        /// get the configration
	if (!cr_setup_diff)
	{
		CR_SETUP(robot, NAV_THEIR_OBSTACLE_RADIUS, CR_DOUBLE);
		CR_SETUP(robot, NAV_OUR_OBSTACLE_RADIUS, CR_DOUBLE);
		CR_SETUP(robot, NAV_THEIR_GOALIE_OBSTACLE_RADIUS, CR_DOUBLE);
		cr_setup_robot = true;
	}
	if (0)
	{
		//gui_debug_line(me, GDBG_NAVIGATION, world.teammate_position(me),
		//	 target_pos, G_ARROW_FORW);
	}

        //gui_debug_line(focused_robot_id, GDBG_NAVIGATION, target_pos, target_pos + target_vel,G_ARROW_FORW);

        //gui_debug_robot(target_pos,target_angle);//Lu_test
        //qDebug()<<"target="<<target_pos.x<<","<<target_pos.y<<","<<target_angle;

        /// get the robot parameters
        // get the teammate position
        robot_position = world.GetRobotPositionByID(focused_robot_id);
        // get the teammate velocity
        robot_velocity = world.GetRobotVelocityByID(focused_robot_id);
        robot_speed = robot_velocity.length();
        current_velocity.set(0,0);
        tmax = closest_point_time(robot_position,robot_velocity,target_pos,current_velocity);

        /// get the ball parameters
	ball     = world.ball_position();
	ball_vel = world.ball_velocity();

        /// set wall as obstacles
	if (obs_flags & OBS_WALLS)
	{
		//四面球场
                obstacles_instance.add_half_plane(-FIELD_LENGTH_H-GOAL_DEPTH, 0, 1, 0,1);
                obstacles_instance.add_half_plane( FIELD_LENGTH_H+GOAL_DEPTH, 0,-1, 0,1);
                obstacles_instance.add_half_plane( 0,-FIELD_WIDTH_H, 0, 1,1);
                obstacles_instance.add_half_plane( 0, FIELD_WIDTH_H, 0,-1,1);
		//
#ifndef LU_VERSION //comment by Lu_test, for the sake of convinience, comment the whole part
                obstacles_instance.add_rectangle(-FIELD_LENGTH_H-GOAL_DEPTH,
		                  (FIELD_WIDTH_H+GOAL_WIDTH_H)/2,
		                  GOAL_DEPTH,FIELD_WIDTH_H-GOAL_WIDTH_H,1);
                obstacles_instance.add_rectangle(-FIELD_LENGTH_H-GOAL_DEPTH,
		                  -(FIELD_WIDTH_H+GOAL_WIDTH_H)/2,
		                  GOAL_DEPTH,FIELD_WIDTH_H-GOAL_WIDTH_H,1);
                obstacles_instance.add_rectangle( FIELD_LENGTH_H+GOAL_DEPTH,
		                   (FIELD_WIDTH_H+GOAL_WIDTH_H)/2,
		                   GOAL_DEPTH,FIELD_WIDTH_H-GOAL_WIDTH_H,1);
                obstacles_instance.add_rectangle( FIELD_LENGTH_H+GOAL_DEPTH,
		                   -(FIELD_WIDTH_H+GOAL_WIDTH_H)/2,
		                   GOAL_DEPTH,FIELD_WIDTH_H-GOAL_WIDTH_H,1);
#endif
	}
        /// set up teammates as obstacles
	for (i=0; i<world.n_teammates; i++)
	{
                if (i!=focused_robot_id && (OBS_TEAMMATE(i) & obs_flags))
		{
                        current_position = world.GetRobotPositionByID(i);
                        current_velocity = world.GetRobotVelocityByID(i);
                        s = current_velocity.length();
			// find where we would hit, where each robot is responsible only
			// for its fraction of the total speed.
                        t = closest_point_time(robot_position,robot_velocity,current_position,current_velocity);
                        t *= robot_speed / (robot_speed + s + EPSILON);
			// if we can get to our target, or its far away, don't bother
			t = min3(t,tmax,4.0);
            /*
            printf("  s=(%7.2f,%7.2f) t=%5.4f tmax=%5.4f d=%7.2f\n",
			       rv.length(),v.length(),t,tmax,
			       MyVector::distance(rp,p+v*t));
            */

            q = current_position + current_velocity*t;
            obstacles_instance.add_circle(q.x,q.y,DVAR(NAV_OUR_OBSTACLE_RADIUS),current_velocity.x,current_velocity.y,1);
			// printf("me=%d t=%f\n",me,t);
		}
	}
        /// set up opponents as obstacles
	goalie_id = world.orole_goalie;
	//obs_flags|=OBS_OPPONENTS;
	if (false)
	{
		for (i=0; i<world.n_opponents; i++)
		{
			if (OBS_OPPONENT(i) & obs_flags)
			{
                                current_position = world.opponent_position(i);
                                current_velocity = world.opponent_velocity(i);
                                s = current_velocity.length();
				// find where we would hit, where each robot is responsible only
				// for its fraction of the total speed.
                                t = closest_point_time(robot_position,robot_velocity,current_position,current_velocity);
                                t *= robot_speed / (robot_speed + s + EPSILON);
				// if we can get to our target, or its far away, don't bother
				t = min3(t,tmax,4.0);
                                q = current_position + current_velocity*t;
				rad = (i != goalie_id)? DVAR(NAV_THEIR_GOALIE_OBSTACLE_RADIUS) :
				      DVAR(NAV_THEIR_OBSTACLE_RADIUS);
                                obstacles_instance.add_circle(q.x,q.y,rad,current_velocity.x,current_velocity.y,1);
			}
		}
	}
	else
	{
		for (i=0; i<world.n_opponents; i++)
		{
			if (OBS_OPPONENT(i) & obs_flags)
			{
                 current_position = world.opponent_position(i);
                 current_velocity = world.opponent_velocity(i);
                 obstacles_instance.add_circle(current_position.x,current_position.y,DVAR(NAV_THEIR_OBSTACLE_RADIUS),current_velocity.x,current_velocity.y,1);
				//obs.add_circle(p.x,p.y,100,v.x,v.y,1);
			}
		}
	}

#ifndef LU_VERSION //comment by Lu_test, for the sake of convinience, comment the whole part
        // if needed, set our defense zone as obstacle
	if (obs_flags & OBS_OUR_DZONE)
	{
                obstacles_instance.add_rectangle(-FIELD_LENGTH_H-DEFENSE_DEPTH,0,
		                  DEFENSE_DEPTH*4,DEFENSE_WIDTH,1);
		// make 100% sure we're not going into defense zone
		out_x = (-FIELD_LENGTH_H+DEFENSE_DEPTH+100) - target_pos.x;
		out_y = (DEFENSE_WIDTH_H+100) - fabs(target_pos.y);
		// printf("  out_x=%f out_y=%f\n",out_x,out_y);
		if (out_x>0 && out_y>0)
		{
			if (out_x < out_y)
			{
				target_pos.x += out_x;
			}
			else
			{
				target_pos.y += sign_nz(target_pos.y)*out_y;
			}
		}
	}

    // if needed, set opponents' defense zone as obstacle
	if (obs_flags & OBS_THEIR_DZONE)
	{
                obstacles_instance.add_rectangle( FIELD_LENGTH_H+DEFENSE_DEPTH,0,
		                   DEFENSE_DEPTH*4,DEFENSE_WIDTH,1);
	}
#endif

        // if needed, set ball as obstacle
        // bug here lu_test
    if (obs_flags & OBS_BALL)
    {
                t = bound(MyVector::distance(ball,robot_position)-180,30,60);
                obstacles_instance.add_circle(ball.x,ball.y,t,ball_vel.x,ball_vel.y,1);
    }
        obstacles_instance.set_mask(1);
	//-----------------------------------------------------------------------
        /// set initial state

        current_position = world.GetRobotPositionByID(focused_robot_id);

        //qDebug()<< "get position x:"<<current_position.x<<"y:"<<current_position.y;

        current_velocity = world.GetRobotVelocityByID(focused_robot_id);
        current_face_angel = world.teammate_direction(focused_robot_id);
        initial.pos = vdtof(current_position);

        if(debugfreq++>50)
        {
            gui_debug_robot(initial.pos,current_face_angel);//Lu_test
            debugfreq=0;
        }


	// initial.vel = vdtof(v);
	goal.pos = vdtof(target_pos);
	// goal.vel = vdtof(target_vel);
	// plan
	obs_id = -1;
        // path planning for the goal (bug)
        // gao mark

    //qDebug()<< "goal x:"<<goal.pos.x<< "y:"<< goal.pos.y;

        // path planning return the node it should go
        //qDebug()<<"Path_planning";
        target = world.path[focused_robot_id].plan(&obstacles_instance,1,initial,goal,obs_id);
        gui_debug_line(0,0,initial.pos,target.pos);
        gui_debug_line(0,0,initial.pos,goal.pos);

        //if(!finite(target.pos.x) || !finite(target.pos.y))
	//{
	//	printf("nav_to_point target=: NANs!%3.2f,%3.2f\n",target.pos.x,target.pos.y);
	//}
    if (true)
	{

//		printf("  Init(%f,%f)  Goal(%f,%f)  Target(%f,%f)\n",
//		       V2COMP(initial.pos),V2COMP(goal.pos),V2COMP(target.pos));

//        qDebug()<<"  Init("<<initial.pos.x <<","<<initial.pos.y<<")  Goal("
//               <<goal.pos.x<<","<<goal.pos.y<<") Target("<<target.pos.x <<","<<target.pos.y<<")";

//		if (target.pos.length() < 10)
//		{
//			exit(1);
//		}

	}
	//求下一途经点相对当前位置单位矢量
	//p当前位置
	//q途径点位置
        /// set target position
	q   = vftod(target.pos);
    qr  = q - current_position;

//lu_test
//    gui_debug_veloline(focused_robot_id, GDBG_NAVIGATION, current_position, q,G_ARROW_FORW);

//    qDebug()<<"veloline"<<current_position.x<<current_position.y;
//    qDebug()<<"veloline"<<q.x<<q.y;


	//单位化
	qrl = qr.length();
	qr /= qrl;
	//如果途径点距离目标点多于1mm，并且到下一步的行走距离也多于1mm
	//if(MyVector::distance(q,target_pos)>1 && qrl>1)
	//{
	//  if(obs_id >= 0)
	//  {
	//    //中途有障碍
	//    obs_vel.set(obs.obs[obs_id].vel.x,obs.obs[obs_id].vel.y);
	//    s = bound(MyVector::dot(qr,obs_vel)+500.0,0.0,1500.0);
	//  }
	//  else
	//  {
	//    //中途没有障碍
	//    s = 500;
	//  }
	//  v = qr * s;
	//}
	//else
	//{
        current_velocity = target_vel;
	//}
        return goto_point(world, focused_robot_id, q, current_velocity, target_angle, type);
}

// 1400.0, 2000.0, 12.0, 12.0
// 1600.0, 3000.0, 18.0, 18.0

// Normal
const double diff_max_vx  = 1200.0; // 1600.0;
const double diff_max_avx = 2000.0; // 3000.0;
const double diff_max_va  =   14.0; //   18.0;
const double diff_max_ava =   14.0; //   18.0;
const double diff_accel_margin = 0.85;

// Dribble
const double diff_ball_max_vx  = 1000.0; // 1600.0;


Robot::Trajectory Robot::goto_point_speed(World &world, int me,
        MyVector2d target_pos,MyVector2d target_vel,
        double target_angle,GotoPointType type)
{
	MyVector2d target,loc,fwd;
	double target_speed;
	double da,daf,d,s,c,ac,angle;
	double cvx,cva;
	double avx,mvx; // ,ava,avm;
	double vx,va,time;
	double f,dt;
	const bool debug = false;
	bool forward_only = false;
        loc = world.GetRobotPositionByID(me);
	angle = world.teammate_direction(me);
	target_speed = target_vel.length();
	fwd.set(1,0);
	fwd = fwd.rotate(angle);
        cvx = fwd.dot(world.GetRobotVelocityByID(me));
	cva = world.teammate_angular_velocity(me);
	target = target_pos - loc;
	da  = angle_mod(target.angle() - angle);
	daf = angle_mod(target_angle - angle);
	d = target.length();
	s = sin(da);
	c = cos(da);
	ac = fabs(c);
	avx = diff_accel_margin*diff_max_avx * (1-0.5*fabs(s));
	// ava = diff_accel_margin*diff_max_ava;
	// avm = ava*sqrt(2*fabs(da)/ava);
	// mvx = (avx*sqrt(2*d*ac/avx) + target_speed)*ac;
	mvx = sqrt(2*avx*d + target_speed*target_speed)*ac;
	vx = forward_only? diff_ball_max_vx : diff_max_vx;
	vx = bound(mvx,0.0,vx)*sign(c);
	if (debug) printf("  mvx=%f vx=%f d=%f\n",mvx,vx,d);
	if (!forward_only)
	{
		va = bound(6*sin(da) * sign(vx),-diff_max_va,diff_max_va);
	}
	else
	{
		va = bound(4*sin(da),-diff_max_va,diff_max_va);
	}
	if (d < 10)
	{
		f = d / 10;
		vx = target_speed + mvx*sign(c)/4;
		// avm = ava*sqrt(2*fabs(da)/ava);
		va = 6*sin(daf);
		// if(forward_only) va = bound(3*daf,-1,1);
	}
	// bound acceleration
	dt = 1 / 30.0;
	// va = bound(va,cva-diff_max_ava*dt,cva+diff_max_ava*dt);
	vx = bound(vx,cvx-diff_max_avx*dt,cvx+diff_max_avx*dt);
	if (debug)
	{
		printf("  tpos=(%8.2f,%8.2f) ts=%6.2f td=%8.2f : da=%5.2f vx=%8.2f/%8.2f:%8.2f va=%6.2f\n",
		       V2COMP(target_pos),target_speed,d,da,vx,mvx,avx,va);
	}
	// printf("  ta=%f ca=%f <%f>\n",target.angle(),angle,da);
	// printf("  vx=%8.2f va=%6.2f aa=%6.2f ax=%6.2f\n",vx,da,ava,avx);
	time = 1;
	return(Trajectory(vx, 0.0, va, time));
}

//旋转到点
Robot::Trajectory Robot::spin_to_point(World &world, int me,
                                       MyVector2d target_pos,
                                       double target_ang_vel)
{
	MyVector2d target,loc,fwd;
	double d,angle;
	double vx,vy,va,time;
        loc = world.GetRobotPositionByID(me);
	angle = world.teammate_direction(me);
	fwd.set(1,0);
	fwd = fwd.rotate(angle);
	target = target_pos - loc;
	d = target.length();
	target *= (150 / d);
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		vx = dot(fwd,target);
		vy = 0;
		va = target_ang_vel;
		time = d / 75;
	}
	else
	{
		vx = dot(fwd,target);
		vy = dot(fwd.perp(),target);
		va = target_ang_vel;
		time = d / 150;
	}
	return(Trajectory(vx, vy, va, time));
}
