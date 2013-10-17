// goalie.cc
//
// Goalie tactic.
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu)
//
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

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"

#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"
#include "user_interface/field_related/field_global_function.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/goalie.h"

CR_DECLARE(GOALIE_MAX_ANGLE);
CR_DECLARE(GOALIE_DIST_OFF_BALL);
CR_DECLARE(GOALIE_DIST_MIN);
CR_DECLARE(GOALIE_DIST_MAX);

CR_DECLARE(GOALIE_PENALTY_MATCH_THEM);
CR_DECLARE(GOALIE_PENALTY_LINE);
CR_DECLARE(GOALIE_PENALTY_MAX);

//ԱԷڳزͬλúԱվλ
//;ΪԷŷԼѡʵ
//ʱҪ:


static bool cr_setup = false;



TGoalie::TGoalie()
{
	penalty_play = false;
	penalty_tend = 0.0;
}

void TGoalie::LoadConfig()
{
	CR_SETUP(tactic, GOALIE_MAX_ANGLE, CR_DOUBLE);
	CR_SETUP(tactic, GOALIE_DIST_OFF_BALL, CR_DOUBLE);
	CR_SETUP(tactic, GOALIE_DIST_MIN, CR_DOUBLE);
	CR_SETUP(tactic, GOALIE_DIST_MAX, CR_DOUBLE);
	CR_SETUP(tactic, GOALIE_PENALTY_MATCH_THEM, CR_INT);
	CR_SETUP(tactic, GOALIE_PENALTY_LINE, CR_DOUBLE);
	CR_SETUP(tactic, GOALIE_PENALTY_MAX, CR_DOUBLE);
}

//ԱĻΧԼͬ׶εķ統
//ʱ߳;ٽӽʱס
void TGoalie::command(World &world, int me, Robot::RobotCommand &command,
                      bool debug)
{
	if (penalty_play && world.isGameRunning() && (penalty_tend > world.time) &&
	        (world.teammate_type(me) != ROBOT_TYPE_DIFF))
	{
		//ڷ״̬
		if (debug)
		{
			gui_debug_printf(me, GDBG_TACTICS, "Charging %f : penalty_end!!!\n", penalty_tend - world.time);
		}
		//
		penaltyCharge(world, me, command,debug);
		return;
	}
	//Էʼ
	else if (world.restartPenalty() && (world.restartWhoseKick() == World::TheirBall))
	{
		//      printf("Running penalty play\n");
		if (debug)
		{
			gui_debug_printf(me, GDBG_TACTICS, "Starting penalty play...\n");
		}
		penalty_tend = world.time + 0.75;
		penalty_play = true;
		penaltyPlay(world, me, command,debug);
		return;
	}
	penalty_play = false;
	penalty_tend = 0;
	bool intercept = true;
	MyVector2d ball = world.ball_position();
	MyVector2d target, target_vel;
	MyVector2d pos, dir, obs;
	double angle,max_x;
	//
	const MyVector2d bbox_min(-FIELD_WIDTH_H,-DEFENSE_WIDTH_H);
	const MyVector2d bbox_max(             0, DEFENSE_WIDTH_H);
	pos = world.GetRobotPositionByID(me);
	dir.set(-1,0);
	//ѰҾԱxĻ˵x꣬(ֻѰҼ볡ǰ)
	obs = evaluation.farthest(world, world.now,
	                          (OBS_TEAMMATES|OBS_OPPONENTS)&~OBS_TEAMMATE(me),
	                          bbox_min,bbox_max,
	                          dir);
	if (debug)
	{
		// printf("%f,%f\n",V2COMP(obs));
		gui_debug_line(me, GDBG_TACTICS, obs-dir.perp()*100, obs+dir.perp()*100);
	}
	//GOALIE_DIST_MAX = 500 # mm, DEFENSE_DEPTH + 40
	//GOALIE_DIST_MIN = 100 # mm
	max_x = bound(obs.x - OMNIBOT_RADIUS,
	              -FIELD_LENGTH_H + DEFENSE_DEPTH-OMNIBOT_RADIUS,
	              -FIELD_LENGTH_H + DVAR(GOALIE_DIST_MAX));
	/*
	printf("%f (%f [%f,%f])\n",FIELD_LENGTH_H+max_x,
	       obs.x-OMNIBOT_RADIUS,
	       -FIELD_LENGTH_H+DEFENSE_DEPTH-OMNIBOT_RADIUS,
	       -FIELD_LENGTH_H+DVAR(GOALIE_DIST_MAX));
	*/
	//ֱ߷عص
	// Call defend to find the target position.
	if (!evaluation.defend_line(world, world.now,
	                            world.our_goal_r, world.our_goal_l,
	                            DVAR(GOALIE_DIST_MIN), FIELD_LENGTH_H + max_x,
	                            DVAR(GOALIE_DIST_OFF_BALL), intercept,
	                            target, target_vel))
	{
		//λ֮䣬50mm
		target = ball;
		target.x -= 50.0;
		target_vel = MyVector2d(0, 0);
	}
	//
	// Bound position
	//޶Ŀ㲻ܱԶ
	if (target.x > ball.x)
	{
		//ֻĶxܻƫĿ꣬ӦӦĶY
		//???
		target.x = ball.x;
	}
	//
	if (fabs(target.y) > GOAL_WIDTH_H)
	{
		//ĿֱֱߵĽΪµĿ
		target = intersection(ball, target,
		                      MyVector2d(-FIELD_LENGTH_H, GOAL_WIDTH_H * sign_nz(target.y)),
		                      MyVector2d(0, GOAL_WIDTH_H * sign_nz(target.y)));
		target.x = bound(target.x, -FIELD_LENGTH_H+DVAR(GOALIE_DIST_MIN), max_x);
	}
	// printf("x = %f\n",x+FIELD_LENGTH_H-DEFENSE_DEPTH);
	//-------------------------------------------------------------------
	//ѾĿ㣬洦ûֱλ
	//-------------------------------------------------------------------
	// Use angle to ball as the goalie's facing.
	//ĽǶ
	angle = (ball - target).angle();
	//# The maximum angle (different from downfield) that we'll face when
	//# the ball is in the corner.
	//# GOALIE_MAX_ANGLE = 1.3744 # 78.75 deg
	//GOALIE_MAX_ANGLE = 0.96 # 55.00 deg
	//޶ǶȷΧ
	angle = abs_bound(angle,DVAR(GOALIE_MAX_ANGLE));
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		angle = world.teammate_nearest_direction(me, angle + M_PI_2);
	}
	// Do not avoid any obstacles, except the ball.
	int obs_flags = 0;
	// get behind ball (avoiding it) if in front of it
	//ǰңŸ
	if ((ball.x < pos.x) && (fabs(ball.y) < GOAL_WIDTH_H+BALL_RADIUS))
	{
		//ֽǶ
		angle = world.teammate_direction(me);
		//ĿΪ˻˰뾶ȥľ
		//???
		target.x -= TEAMMATE_EFFECTIVE_RADIUS + BALL_RADIUS;
		obs_flags = OBS_BALL;
	}
	else if (world.inOurDefenseZone())
	{
		//Ÿ
		target = ball;
		gui_debug_printf(me, GDBG_TACTICS,"Goalie:Clearing\n");
	}
	if (0 && debug)
	{
		MyVector2d aim_target;
		double angle_tolerance;
		gui_debug_line(me, GDBG_TACTICS,
		               world.GetRobotPositionByID(me), target, G_ARROW_FORW);
		evaluation.aim(world, world.now, world.ball_position(),
		               world.our_goal_r, world.our_goal_l, OBS_TEAMMATE(me),
		               aim_target, angle_tolerance);
		gui_debug_line(me, GDBG_TACTICS, ball, aim_target);
		gui_debug_line(me, GDBG_TACTICS, ball,
		               (aim_target - ball).rotate(angle_tolerance) + ball);
		gui_debug_line(me, GDBG_TACTICS, ball,
		               (aim_target - ball).rotate(-angle_tolerance) + ball);
	}
	command.cmd = Robot::CmdPosition;
	command.target = target;
	command.velocity = MyVector2d(0, 0); // target_vel;
	command.angle = angle;
	command.observation_type = obs_flags;
}

Tactic *TGoalie::parser(const char *param_string)
{
	TGoalie* t=new TGoalie();
	if (t)
	{
		t->LoadConfig();
	}
	return t;
}


////// Goalie Penalty kick code
//ʱԱְΪλáѵվλľ
//Ϣϳһ˶
void TGoalie::penaltyPlay(World &world, int me, Robot::RobotCommand &command,
                          bool debug)
{
	if (debug)
	{
		gui_debug_printf(me, GDBG_TACTICS, "Running penalty play");
	}
	MyVector2d ball = world.ball_position();
	MyVector2d target, target_vel;
	MyVector2d pos, dir;
	double angle;
	pos = world.GetRobotPositionByID(me);
	dir.set(-1,0);
	// Call defend to find the target position.
	target.set(DVAR(GOALIE_PENALTY_LINE), 0);
	target_vel = MyVector2d(0, 0);
	angle = 0;
	// need to adjust for where opponent is kicking here
	// need to gate this
	//ѡˣΪλóߣXŵĻ
	int oid = world.choosePenaltyKicker();
	if ((oid >= 0) && IVAR(GOALIE_PENALTY_MATCH_THEM))
	{
		MyVector2d kpos = world.opponent_position(oid);
		MyVector2d kvec = (ball - kpos).norm();
		MyVector2d knorm = kvec.perp();
		// we want kperp to point towards the goal to use the right side
		if (knorm.x > 0)
		{
			knorm = -knorm;
		}
		// make robot independent
		double rad = OMNIBOT_RADIUS;
		if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
		{
			rad = DIFFBOT_LENGTH_H;
		}
		MyVector2d projpoint = ball + knorm * rad;
		MyVector2d glinea(DVAR(GOALIE_PENALTY_LINE), -GOAL_WIDTH_H);
		MyVector2d glineb(DVAR(GOALIE_PENALTY_LINE), GOAL_WIDTH_H);
		MyVector2d go2point = intersection(projpoint, projpoint + kvec, glinea, glineb);
		MyVector2d ipoint = intersection(ball, ball + kvec, glinea, glineb);
		// need bounding
		// fix this!!!
		go2point.y = bound(go2point.y, -DVAR(GOALIE_PENALTY_MAX), DVAR(GOALIE_PENALTY_MAX));
		// check we are not just in the middle
		//    if ((go2point.y - ball.y) * (kpos.y - ball.y) > 0)
		//      go2point.y = ball.y;
		if (fabs(ipoint.y) < OMNIBOT_RADIUS)
		{
			go2point.y = ball.y;
		}
		if (0 && debug)
		{
			gui_debug_line(me, GDBG_TACTICS, kpos, kpos + kvec * 1000.0, 0);
			gui_debug_line(me, GDBG_TACTICS,
			               MyVector2d(DVAR(GOALIE_PENALTY_LINE), -DVAR(GOALIE_PENALTY_MAX)),
			               MyVector2d(DVAR(GOALIE_PENALTY_LINE), DVAR(GOALIE_PENALTY_MAX)), 0);
			gui_debug_line(me, GDBG_TACTICS,
			               projpoint, projpoint + kvec, 0);
			gui_debug_line(me, GDBG_TACTICS,
			               projpoint, projpoint + kvec * 1000.0, 0);
			gui_debug_line(me, GDBG_TACTICS,
			               go2point, go2point, G_ARROW_FORW);
		}
		// lets try it
		target = go2point;
		angle = (ball - target).angle();
	}
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		angle = world.teammate_nearest_direction(me, angle + M_PI_2);
	}
	// Do not avoid any obstacles, except the ball.
	int obs_flags = 0;
	// get behind ball (avoiding it) if in front of it
	if (debug)
	{
		MyVector2d aim_target;
		double angle_tolerance;
		gui_debug_line(me, GDBG_TACTICS,
		               world.GetRobotPositionByID(me), target, G_ARROW_FORW);
		evaluation.aim(world, world.now, world.ball_position(),
		               world.our_goal_r, world.our_goal_l, OBS_TEAMMATE(me),
		               aim_target, angle_tolerance);
		gui_debug_line(me, GDBG_TACTICS, ball, aim_target);
		gui_debug_line(me, GDBG_TACTICS, ball,
		               (aim_target - ball).rotate(angle_tolerance) + ball);
		gui_debug_line(me, GDBG_TACTICS, ball,
		               (aim_target - ball).rotate(-angle_tolerance) + ball);
	}
	command.cmd = Robot::CmdPosition;
	command.target = target;
	command.velocity = MyVector2d(0, 0); // target_vel;
	command.angle = angle;
	command.observation_type = obs_flags;
}

//
bool TGoalie::penaltyCharge(World &world, int me, Robot::RobotCommand &command,
                            bool debug)
{
	if (debug)
	{
		gui_debug_printf(me, GDBG_TACTICS, "Penalty charge down");
	}
	command.cmd = Robot::CmdPosition;
	//λʸ
	MyVector2d ball = world.ball_position();
	MyVector2d mypos = world.GetRobotPositionByID(me);
	MyVector2d r2ball = (ball - mypos);
	//ȴڷ
	if ((mypos.x > -(PENALTY_SPOT - OMNIBOT_RADIUS)) ||
	        (r2ball.x < OMNIBOT_RADIUS))
	{
		command.target = mypos;
		command.velocity.set(0,0);
		command.angle = r2ball.angle();
		command.observation_type = 0; //OBS_EVERYTHING_BUT_ME(me);
		printf("Maybe do something now!!!\n");
		return (false);
	}
	//ײ
	// change this to drive at ball
	// nav to point and fix obstacles
	command.target = ball;
	command.velocity = r2ball.norm(500.0);
	command.angle = 0; //r2ball.angle();
	command.observation_type = 0; //OBS_EVERYTHING_BUT_ME(me);
	return (true);
}
