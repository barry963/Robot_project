// anticipation_tactics.h
//
// Tactics for anticipatory positions.
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

#ifndef __anticipation_tactics_h__
#define __anticipation_tactics_h__

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"

class TPositionForLooseBall : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "position_for_loose_ball";
	}

private:
	BRegion region;

public:
	TPositionForLooseBall(BRegion _region)
	{
		region = _region;
	}

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TPositionForLooseBall(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};

class TPositionFor : public RobotTactic
{
protected:
	EvaluationPosition eval;

public:
	virtual ~TPositionFor() { }

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug)
	{
		// Debug is the only way to know its for real...  Debug should be
		//  changed to a for_real flag.
		eval.update(world, getObsFlags());
		command.cmd = Robot::CmdPosition;
		command.target = eval.point();
		command.velocity = MyVector2d(0, 0);
		command.angle = eval.angle();
		command.observation_type = OBS_EVERYTHING_BUT_ME(me);
		if (debug)
		{
			eval.region.gui_draw(me, GDBG_TACTICS, world);
			gui_debug_x(me, GDBG_TACTICS, command.target);
		}
	}
};

class TPositionForRebound : public TPositionFor
{
public:
	virtual const char *name() const
	{
		return "position_for_rebound";
	}

private:
	static double eval_fn(World &world, const MyVector2d p,
	                      int obs_flags, double &a);

public:
	TPositionForRebound(BRegion _region)
	{
		eval.set(_region, &eval_fn, 0.2442);
	}

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TPositionForRebound(*this);
	}
};

class TPositionForPass : public TPositionFor
{
public:
	virtual const char *name() const
	{
		return "position_for_pass";
	}

private:
	static double eval_fn(World &world, const MyVector2d p,
	                      int obs_flags, double &a);

public:
	TPositionForPass(BRegion _region)
	{
		eval.set(_region, &eval_fn, 0.1221);
	}

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TPositionForPass(*this);
	}
};

class TPositionForDeflection : public TPositionFor
{
public:
	virtual const char *name() const
	{
		return "position_for_deflection";
	}

private:
	static double eval_fn(World &world, const MyVector2d p,
	                      int obs_flags, double &a);

public:
	TPositionForDeflection(BRegion _region)
	{
		eval.set(_region, &eval_fn, 0.2442);
	}

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TPositionForDeflection(*this);
	}
};

#endif
