//
// defense_tactics.h
//
// Defense tactics.
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

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/defense_tactics.h"

CR_DECLARE(DEFENSE_OFF_BALL);
CR_DECLARE(MARK_OFF_OPPONENT);

inline static void cr_do_setup()
{
	CR_SETUP(tactic, DEFENSE_OFF_BALL, CR_DOUBLE);
	CR_SETUP(tactic, MARK_OFF_OPPONENT, CR_DOUBLE);
}
//--------------------------------------------------------------------------------------
//ֱ߷
TDefendLine::TDefendLine(BCoordinate p1, BCoordinate p2,
                         double _distmin, double _distmax)
		:  RobotTactic(false)
{
	p[0] = p1;
	p[1] = p2;
	distmin = _distmin;
	distmax = _distmax;
}

void TDefendLine::LoadConfig()
{
        //cr_do_setup();
}

void TDefendLine::command(World &world, int me, Robot::RobotCommand &command,
                          bool debug)
{
	intercepting = true;
	MyVector2d ball = world.ball_position(me);
	MyVector2d target, velocity;
	double angle;
	MyVector2d v[2] = { p[0].asVector(world), p[1].asVector(world) };
	// Position
	//DEFENSE_OFF_BALL =   90 # mm
	if (!evaluation.defend_line(world, world.now, v[0], v[1],
	                            distmin, distmax, DVAR(DEFENSE_OFF_BALL),
	                            intercepting, target, velocity))
	{
		if (debug)
		{
			gui_debug_printf(me, GDBG_TACTICS,
			                 "DefendLine: WARNING evaluation.defend_line() returned false.");
		}
		//⣬ײ뱾
		//????
		target = ball;
		velocity = MyVector2d(0, 0);
	}
	// Obstacles... don't avoid the ball if away from the line.
	int obs_flags = OBS_EVERYTHING_BUT_ME(me);
	MyVector2d mypos = world.GetRobotPositionByID(me);
	//ŸͰ򲻵ϰ
	if (distance_to_line(v[0], v[1], mypos) <
	        distance_to_line(v[0], v[1], ball))
	{
		obs_flags &= ~OBS_BALL;
	}
	if (debug)
	{
		gui_debug_printf(me, GDBG_TACTICS, "Intercepting = %d\n", intercepting);
	}
	//
	target = evaluation.findOpenPositionAndYield(world, target,
	         (v[0] + v[1]) / 2.0,
	         getObsFlags() | OBS_OPPONENTS);
	// Angle
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		angle = world.teammate_nearest_direction(me, (v[0] - v[1]).angle());
	}
	else
	{
		angle = (ball - target).angle();
	}
	// Debug output
	if (0 && debug)
	{
		gui_debug_line(me, GDBG_TACTICS, v[0], target);
		gui_debug_line(me, GDBG_TACTICS, v[1], target);
		gui_debug_line(me, GDBG_TACTICS,
		               world.GetRobotPositionByID(me), target, G_ARROW_FORW);
		gui_debug_line(me, GDBG_TACTICS,
		               target, target + velocity, G_ARROW_FORW);
	}
	command.cmd = Robot::CmdPosition;
	command.target = target;
	command.velocity = velocity;
	command.angle = angle;
	command.observation_type = obs_flags;
}

Tactic *TDefendLine::parser(const char *param_string)
{
	BCoordinate c[2];
	double mind, maxd;
	param_string += Parse::pTCoordinate(param_string, c[0]);
	param_string += Parse::pTCoordinate(param_string, c[1]);
	param_string += Parse::pDouble(param_string, mind);
	param_string += Parse::pDouble(param_string, maxd);
	TDefendLine* pTDefendLine;
	pTDefendLine=new TDefendLine(c[0], c[1], mind, maxd);
	pTDefendLine->LoadConfig();
	return pTDefendLine;
}
//--------------------------------------------------------------------------------------
//
TDefendPoint::TDefendPoint(BCoordinate _center,
                           double _distmin, double _distmax)
		: RobotTactic(false)
{
	center = _center;
	distmin = _distmin;
	distmax = _distmax;
	cr_do_setup();
}

void TDefendPoint::command(World &world, int me, Robot::RobotCommand &command,
                           bool debug)
{
	intercepting = true;
	MyVector2d ball = world.ball_position(me);
	MyVector2d centerv = center.asVector(world);
	MyVector2d target, velocity;
	double angle;
	// Position
	if (!evaluation.defend_point(world, world.now, centerv,
	                             distmin, distmax, DVAR(DEFENSE_OFF_BALL),
	                             intercepting, target, velocity))
	{
		//falseʾѾ
		if (debug)
		{
			gui_debug_printf(me, GDBG_TACTICS,
			                 "DefendCircle: WARNING evaluation.defend_line() returned false.");
		}
		//ѾСֵߵǰͣ
		target = ball;
		velocity = MyVector2d(0, 0);
	}
	//ĿĵǶ
	// Angle
	angle = (target - centerv).angle();
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		angle = world.teammate_nearest_direction(me, anglemod(angle + M_PI_2));
	}
	// Obstacles... don't avoid the ball if away from the point.
	int obs_flags = OBS_EVERYTHING_BUT_ME(me);
	MyVector2d mypos = world.GetRobotPositionByID(me);
	//ӷĵ㿴ӭ
	if ((mypos - centerv).dot(ball - mypos) > 0)
	{
		obs_flags &= ~OBS_BALL;
	}
	//ѰĿĵأҪ󲻳ͻ
	target = evaluation.findOpenPositionAndYield(world, target, centerv,
	         getObsFlags() | OBS_OPPONENTS);
	// Debug output
	double r = (target - centerv).length();
	if (debug)
	{
		gui_debug_arc(me, GDBG_TACTICS, centerv,
		              MyVector2d(2 * r, 2 * r), 0.0, M_2PI);
		gui_debug_line(me, GDBG_TACTICS,
		               world.GetRobotPositionByID(me), target, G_ARROW_FORW);
	}
	command.cmd = Robot::CmdPosition;
	command.target = target;
	command.velocity = velocity;
	command.angle = angle;
	command.observation_type = obs_flags;
}

Tactic *TDefendPoint::parser(const char *param_string)
{
	BCoordinate center;
	double mind, maxd;
	param_string += Parse::pTCoordinate(param_string, center);
	param_string += Parse::pDouble(param_string, mind);
	param_string += Parse::pDouble(param_string, maxd);
	return new TDefendPoint(center, mind, maxd);
}
//--------------------------------------------------------------------------------------
//
TDefendLane::TDefendLane(BCoordinate _p1, BCoordinate _p2)
		: RobotTactic(false)
{
	p[0] = _p1, p[1] = _p2;
	cr_do_setup();
}

void TDefendLane::command(World &world, int me, Robot::RobotCommand &command,
                          bool debug)
{
	intercepting = true;
	MyVector2d target, velocity;
	double angle;
	MyVector2d v0 = p[0].asVector(world);
	MyVector2d v1 = p[1].asVector(world);
	evaluation.defend_on_line(world, world.now, v0, v1,
	                          intercepting, target, velocity);
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		angle = (v0 - v1).angle();
		angle = world.teammate_nearest_direction(me, angle);
	}
	else
	{
		angle = (world.ball_position() - target).angle();
	}
	MyVector2d opt0, opt1;
	opt0 = evaluation.findOpenPositionAndYield(world, target, v0,
	        getObsFlags() | OBS_OPPONENTS);
	opt1 = evaluation.findOpenPositionAndYield(world, target, v1,
	        getObsFlags() | OBS_OPPONENTS);
	target = ((target - opt0).length() < (target - opt1).length()) ? opt0 : opt1;
	if (debug)
	{
		gui_debug_line(me, GDBG_TACTICS, v0, v1);
		gui_debug_line(me, GDBG_TACTICS,
		               world.GetRobotPositionByID(me), target, G_ARROW_FORW);
	}
	command.cmd = Robot::CmdPosition;
	command.target = target;
	command.velocity = velocity;
	command.angle = angle;
	command.observation_type = OBS_EVERYTHING_BUT_ME(me) & ~OBS_BALL;
}

Tactic *TDefendLane::parser(const char *param_string)
{
	BCoordinate p1, p2;
	param_string += Parse::pTCoordinate(param_string, p1);
	param_string += Parse::pTCoordinate(param_string, p2);
	return new TDefendLane(p1, p2);
}

TBlock::TBlock(double _distmin, double _distmax, int _prefside)
		: RobotTactic(false)
{
	distmin = _distmin;
	distmax = _distmax;
	prefside = _prefside;
	cr_do_setup();
}

void TBlock::command(World &world, int me, Robot::RobotCommand &command,
                     bool debug)
{
	intercepting = true;
	//λ
	MyVector2d ball = world.ball_position(me);
	//ǰ˵λ
	MyVector2d mypos = world.GetRobotPositionByID(me);
	MyVector2d v[2] = { world.our_goal_l, world.our_goal_r };
	//ľ
	double mydist = distance_to_line(v[0], v[1], mypos);
	MyVector2d target, velocity;
	double angle;
	MyVector2d pref_point;
	// Side preference
	if (prefside != 0)
	{
		pref_point = MyVector2d(world.our_goal_l.x,
		                        world.our_goal_l.y * prefside * world.sideBall());
	}
	else if (!pref_point_set)
	{
		pref_point = world.our_goal;
	}
	// Take into account teammates behind us.
	//ѰĻ
	int obs_flags = 0;
	for (int i=0; i < world.n_teammates; i++)
	{
		if (i == me)
		{
			continue;
		}
		if (distance_to_line(v[0], v[1], world.GetRobotPositionByID(i)) < mydist)
		{
			obs_flags |= OBS_TEAMMATE(i);
		}
	}
	// Position
	if (!evaluation.defend_line(world, world.now, v[0], v[1],
	                            distmin, distmax, DVAR(DEFENSE_OFF_BALL),
	                            intercepting, obs_flags, pref_point, 0.1221,
	                            target, velocity) &&
	        !evaluation.defend_line(world, world.now, v[0], v[1],
	                                distmin, distmax, DVAR(DEFENSE_OFF_BALL),
	                                intercepting, target, velocity))
	{
		if (debug)
		{
			gui_debug_printf(me, GDBG_TACTICS,
			                 "Block: WARNING evaluation.defend_line() returned false.");
		}
		target = mypos;
		velocity = MyVector2d(0, 0);
	}
	else
	{
		pref_point = target;
		pref_point_set = true;
	}
	if (!finite(target.x))
	{
		printf("TBlock:0 \r\n");
	}
	if (debug)
	{
		gui_debug_printf(me, GDBG_TACTICS, "Intercepting = %d\n", intercepting);
	}
	target = evaluation.findOpenPositionAndYield(world, target, world.our_goal,
	         getObsFlags() | OBS_OPPONENTS);
	if (!finite(target.x))
	{
		printf("TBlock:1 \r\n");
	}
	// Obstacles... don't avoid the ball if away from the line.
	//߱ǸԶ
	obs_flags = OBS_EVERYTHING_BUT_ME(me);
	if (distance_to_line(v[0], v[1], mypos) < distance_to_line(v[0], v[1], ball))
	{
		obs_flags &= ~OBS_BALL;
	}
	// Angle
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		angle = world.teammate_nearest_direction(me, (v[0] - v[1]).angle());
	}
	else
	{
		//
		angle = (ball - target).angle();
	}
	command.cmd = Robot::CmdPosition;
	command.target = target;
	command.velocity = velocity;
	command.angle = angle;
	command.observation_type = obs_flags;
}

Tactic *TBlock::parser(const char *param_string)
{
	double mindist, maxdist;
	int prefside;
	param_string += Parse::pDouble(param_string, mindist);
	param_string += Parse::pDouble(param_string, maxdist);
	param_string += Parse::pInt(param_string, prefside);
	return new TBlock(mindist, maxdist, prefside);
}


TMark::TMark(int _target, Type _type)
{
	target = _target;
	type = _type;
        //2cr_do_setup();
}

Tactic *TMark::parser(const char *param_string)
{
	int target;
	Type type;
	char *word;
	param_string += Parse::pInt(param_string, target);
	param_string += Parse::skipWS(param_string, " \t");
	if (param_string[0] != '\n')
	{
		param_string += Parse::pWord(param_string, &word);
		if (strcmp(word, "ball") == 0)
		{
			type = FromBall;
		}
		else if (strcmp(word, "our_goal") == 0)
		{
			type = FromOurGoal;
		}
		else if (strcmp(word, "their_goal") == 0)
		{
			type = FromTheirGoal;
		}
		else if (strcmp(word, "shot") == 0)
		{
			type = FromShot;
		}
		else type = FromBall;
		if (word)
		{
			delete [] word;
		}
	}
	else
	{
		type = FromBall;
	}
	return new TMark(target, type);
}

void TMark::command(World &world, int me, Robot::RobotCommand &command,
                    bool debug)
{
	int obs_flags = OBS_EVERYTHING_BUT_ME(me);
	int targeti = getOpponentId(target);
	if (targeti<0)
	{
		//???
		//ҲˣӦ÷ֹͣ
		command.cmd = Robot::CmdPosition;
		command.target = world.GetRobotPositionByID(me);
		command.velocity.set(0,0);
		command.angle = world.teammate_direction(me);
		command.observation_type = obs_flags;
		return;
	}
	MyVector2d targetp = world.opponent_position(targeti);
	MyVector2d targetv = world.opponent_velocity(targeti);
	MyVector2d mark_from;
	switch (type)
	{
	case FromOurGoal:
		//ֹӽҷ
		mark_from = world.our_goal;
		break;
	case FromTheirGoal:
		//ֹӽԷ
		mark_from = world.their_goal;
		break;
	case FromBall:
		//ֹӽ
		mark_from = world.ball_position();
		break;
	case FromShot:
	{
		//ֹ
		//ȡ˵ֱߣ򵽶ԷţĴ
		mark_from = point_on_segment(world.ball_position(), world.their_goal, targetp);
		if (world.obsPosition(mark_from, OBS_OPPONENT(targeti)))
		{//õڱĻˣY趨Ϊǰǽ
			mark_from = MyVector2d(targetp.x, copysign(FIELD_WIDTH_H, -targetp.y));
		}
	}
	}
	//MARK_OFF_OPPONENT = 270 # mm
	//뱻Ŀ㷽ؾĵ
	targetp += (mark_from - targetp).norm(DVAR(MARK_OFF_OPPONENT));
	//
	targetp = evaluation.findOpenPositionAndYield(world, targetp,
	          mark_from,
	          getObsFlags() | OBS_OPPONENTS);
	targetv = targetv.rotate(-(mark_from - targetp).angle());
	if (targetv.x > 0.0)
	{
		targetv.x = 0.0;
	}
	targetv = targetv.rotate((mark_from - targetp).angle());
	//
	if (targetv.length() < 80.0)
	{
		targetv = MyVector2d(0, 0);
	}
	double angle = (targetp - mark_from).angle();
	if (world.teammate_type(me) == ROBOT_TYPE_DIFF)
	{
		angle = world.teammate_nearest_direction(me, angle + M_PI_2);
	}
#if 0
	if ((mark_from - targetp).dot(world.teammate_position(me) - targetp) > 0.0)
		obs_flags &= ~OBS_OPPONENT(targeti);
#else
	if (point_on_segment(world.GetRobotPositionByID(me), targetp,
	                     world.opponent_position(targeti)) == targetp)
	{
		obs_flags &= ~OBS_OPPONENT(targeti);
	}
#endif
	gui_debug_arc(me, GDBG_TACTICS,
	              world.opponent_position(targeti),
	              MyVector2d(200, 200), 0, M_2PI);
	command.cmd = Robot::CmdPosition;
	command.target = targetp;
	command.velocity = targetv;
	command.angle = angle;
	command.observation_type = obs_flags;
}


