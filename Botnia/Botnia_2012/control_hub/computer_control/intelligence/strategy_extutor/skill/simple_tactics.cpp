// simple_tactics.cc
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
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"

// TStop
//
// Makes the robot stop.
//

void TStop::run(World &world, int me)
{
	world.halt(me);
}

Tactic *TStop::parser(const char *param_string)
{
	return new TStop();
}

// TVelocity
//
// Specifies specific parallel, perpendicular, and angular velocities for
// the robot.
//

void TVelocity::run(World &world, int me)
{
	world.go(me, vx, vy, va);
}

Tactic *TVelocity::parser(const char *param_string)
{
	double vx, vy, va;
	sscanf(param_string, "%lf %lf %lf", &vx, &vy, &va);
	return new TVelocity(vx, vy, va);
}

// TPosition
//
// Sends the robot to a position on the field.
//
// ûߵָط
void SPosition::command(World &world, int me, Robot::RobotCommand &command,
                        bool debug)
{
	command.cmd = Robot::CmdPosition;
    command.target = position.asVector(world);
    command.velocity = MyVector2d(0, 0);//目的点速度等于0
	command.angle = faceto.asDirection(world);
	if (use_obsflags)
	{
		command.observation_type = obs;
	}
	else
	{
		command.observation_type = OBS_EVERYTHING_BUT_ME(me);
	}
}

Tactic *SPosition::parser(const char *param_string)
{
	BCoordinate c;
	BCoordinate f;
	param_string += Parse::pTCoordinate(param_string, c);
	param_string += Parse::pTCoordinateDir(param_string, f);
	return new SPosition(c, f);
}

// TDribbleToPosition
//
// Sends the robot to a position on the field.
//
//
void SDribbleToPosition::command(World &world, int me,
                                 Robot::RobotCommand &command,
                                 bool debug)
{
	command.cmd = Robot::CmdDribble;
	command.target = position.asVector(world);
	command.angle = faceto.asDirection(world);
}

Tactic *SDribbleToPosition::parser(const char *param_string)
{
	BCoordinate c;
	BCoordinate f;
	param_string += Parse::pTCoordinate(param_string, c);
	param_string += Parse::pTCoordinateDir(param_string, f);
	return new SDribbleToPosition(c, f);
}
