// special_tactics.cc
//
// Very basic tactics.
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


#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/special_tactics.h"


// set variable as PDataMap
CR_DECLARE(SHOOT_AIM_PREF_AMOUNT);
CR_DECLARE(DISTANCE_FROM_PENALTY_LINE);


inline void cr_do_setup()
{
        CR_SETUP(tactic, SHOOT_AIM_PREF_AMOUNT, CR_DOUBLE);
        CR_SETUP(tactic, DISTANCE_FROM_PENALTY_LINE, CR_DOUBLE);
}

// TPositionForStart
//
// Sends the robot to a position on the field.
//


Tactic *TPositionForStart::parser(const char *param_string)
{
	BCoordinate c;
	BCoordinate f;
	param_string += Parse::pTCoordinate(param_string, c);
	param_string += Parse::pTCoordinateDir(param_string, f);
        TPositionForStart* tactic=new TPositionForStart(c, f);
        cr_do_setup();
        return tactic;
}

Status TPositionForStart::isDone(World &world, int me)
{
	//COMM_START=s
	if (strchr("s ", world.game_state) != NULL)
	{
		return Succeeded;
	}
	else
	{
		return InProgress;
	}
}

TPositionForKick::TPositionForKick()
{
}

void TPositionForKick::LoadConfig()
{
    cr_do_setup();
}

void TPositionForKick::command(World &world, int me,
                               Robot::RobotCommand &command,
                               bool debug)
{
    MyVector2d ball_position = world.ball_position();
	MyVector2d target;
	double angle_tolerance;
	if (!prev_target_set)
	{
		prev_target = world.their_goal;
		prev_target_set = true;
	}
	//# The amount we'd prefer to shoot at our previous angle.  If an another
	//#  at least this much bigger appears we'll switch to that.
	//SHOOT_AIM_PREF_AMOUNT = 0.01745 # 1 degree
	// (1) Try shooting on goal.
    if (!evaluation.aim(world, world.now, world.ball_position(),
	                    world.their_goal_r,
	                    world.their_goal_l,
	                    OBS_EVERYTHING_BUT_US,
	                    prev_target, DVAR(SHOOT_AIM_PREF_AMOUNT),
	                    target, angle_tolerance))
	{
        //back border of the goal lu_test
		MyVector2d downfield[2];
                downfield[0].set(ball_position.x + 180.0, -FIELD_WIDTH_H);
                downfield[1].set(ball_position.x + 180.0, FIELD_WIDTH_H);
		// (2) Try clearing the ball

        if (!evaluation.aim(world, world.now, world.ball_position(),
		                    downfield[0], downfield[1],
		                    OBS_EVERYTHING_BUT_ME(me),
		                    prev_target, DVAR(SHOOT_AIM_PREF_AMOUNT),
		                    target, angle_tolerance))

		{
			// Guaranteed to return true and fill in the parameters when
			// obs_flags is empty.
			// (3) Just shoot downfield.
			//
			evaluation.aim(world, world.now, world.ball_position(),
			               downfield[0], downfield[1],
			               0, target, angle_tolerance);
            qDebug()<<"shoot downfield";
		}
	}
	if (debug)
	{
                gui_debug_line(me, GDBG_TACTICS, ball_position, target);
                gui_debug_line(me, GDBG_TACTICS, ball_position,
                               (target - ball_position).rotate(angle_tolerance) + ball_position);
                gui_debug_line(me, GDBG_TACTICS, ball_position,
                               (target - ball_position).rotate(-angle_tolerance) + ball_position);
	}
	prev_target = target;
	//
        double ball_distance = (world.GetRobotPositionByID(me) - ball_position).length();
    //question lu_test
	if (world.GetRobotVelocityByID(me).length() < 20.0)
	{
                ball_distance -= 20.0;
	}
	// put this in config
	double closest = 85.0;
    //question lu_test constant setting
        ball_distance = bound(ball_distance, closest, 150.0);
	//target85150
        MyVector2d targetp = ball_position - (target - ball_position).norm(ball_distance);
        double angle = (ball_position - targetp).angle();
	int obs = OBS_EVERYTHING_BUT_ME(me);
	MyVector2d r2target = (targetp - world.GetRobotPositionByID(me));
	double d2target = r2target.sqlength();
	//20150
    //question lu_test ???
	if ((d2target < 150.0 * 150.0) && (d2target > 20.0 * 20.0) &&
	        (fabs(angle_mod(angle - r2target.angle())) < M_PI_4))
	{
		//    obs = OBS_WALLS;
		obs = 0;
		//    printf("turning off obstacle avoidance!!!\n");
		if (debug)
		{
			gui_debug_printf(me, GDBG_TACTICS, "turning off obstacle avoidance!!!\n");
		}
	}
	command.cmd = Robot::CmdPosition;
	command.target = targetp;
	command.velocity = MyVector2d(0, 0);
	command.angle = angle;
    //command.obs = OBS_EVERYTHING_BUT_ME(me);
	command.observation_type = obs;
    command.goto_point_type = Robot::GotoPointMoveForw;
}

////////////////// Penalty ////////////////////

TPositionForPenalty::TPositionForPenalty(void)
{
}

void TPositionForPenalty::LoadConfig(void)
{
	cr_do_setup();
}

//
void TPositionForPenalty::command(World &world, int me,
                                  Robot::RobotCommand &command,
                                  bool debug)
{
	//450mm
	MyVector2d mypos = world.GetRobotPositionByID(me);
	MyVector2d target;
	double linex;
	double penalty_goal_dir;
	// figure out which penalty is going
	if (world.restartWhoseKick() == World::OurBall)
	{
		//
		//
		linex = (PENALTY_SPOT - ROBOT_DEF_WIDTH_H);
		penalty_goal_dir = 1.0;
	}
	else
	{
		//
		linex = -(PENALTY_SPOT - ROBOT_DEF_WIDTH_H);
		penalty_goal_dir = -1.0;
	}
	if (debug)
	{
		gui_debug_line(me, GDBG_TACTICS, MyVector2d(linex, -100), MyVector2d(linex, 100));
	}
	if ((linex - mypos.x) * penalty_goal_dir > 0)
	{//
		// need to halt here
		gui_debug_printf(me, GDBG_TACTICS, "we are in position\n");
		command.cmd = Robot::CmdPosition;
		command.target = mypos;
		command.velocity = MyVector2d(0, 0);
		command.angle = 0; //((penalty_goal_dir > 0) ? M_PI : 0.0);
		command.observation_type = OBS_EVERYTHING_BUT_ME(me);
		command.goto_point_type = Robot::GotoPointMove;
	}
	else
	{
		//Y,
		//???Y
		gui_debug_printf(me, GDBG_TACTICS, "we need to move\n");
		// choose our target point
		target.set(linex, mypos.y);
		if (debug)
		{
			gui_debug_line(me, GDBG_TACTICS, mypos, target);
		}
		command.cmd = Robot::CmdPosition;
		command.target = target;
		command.velocity = MyVector2d(0, 0);
		command.angle = 0; //((penalty_goal_dir > 0) ? M_PI : 0.0);
		command.observation_type = OBS_EVERYTHING_BUT_ME(me);
		command.goto_point_type = Robot::GotoPointMove;
	}
}

//
void TChargeBall::command(World &world, int me, Robot::RobotCommand &command,
                          bool debug)
{
	MyVector2d ball = world.ball_position();
	MyVector2d mypos = world.GetRobotPositionByID(me);
	command.cmd = Robot::CmdMoveBall;

	if ((ball - mypos).x > 0)
	{
		//,500mm
		command.target = ball + (ball - mypos).norm(500);
		command.angle_tolerance = M_PI_2 - fabs((ball - mypos).angle());
	}
	else
	{
		//500mm
		command.target = ball + MyVector2d(500, 0);
		command.angle_tolerance = M_PI_4;
	}
	command.ball_target = command.target;
	command.ball_shot_type = Robot::BallShotClear;
}

//===============PassRecieve

TPassandReceive::TPassandReceive(void)
{
    PassRobot = new TPass(1);
    ReceiveRobot = new TReceivePass();
}

TPassandReceive::~TPassandReceive()
{
    if (!PassRobot)
    {
        delete PassRobot;
    }

    if (!ReceiveRobot)
    {
        delete ReceiveRobot;
    }
}

void TPassandReceive::LoadConfig()
{
    cr_do_setup();
}

void TPassandReceive::command(World &world, int me, Robot::RobotCommand &command, bool debug)
{
    if( me == 0)
    {
        PassRobot->command(world,me,command,debug);
    }
    else
        if( me == 1)
        {
            ReceiveRobot->command(world,me,command,debug);
        }
    else
        {
            qDebug()<<"Something wrong!";
        }
}

