// ball_tactics.h
//
// Ball tactics.
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

#ifndef __ball_tactics_h__
#define __ball_tactics_h__

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"

//ս
class TShoot : public RobotTactic
{
public:
	enum Type { Aim, NoAim, Deflect };

	virtual const char *name() const
	{
		return "shoot";
	}

	static double eval_fn(World &world, const MyVector2d p,
	                      int obs_flags, double &a);

private:
	Type type;
	int deflect_target;

	MyVector2d prev_target;
	bool prev_target_set;

	EvaluationPosition eval;

public:
	TShoot(Type _shot_type = Aim, int _deflect_target = -1);
	virtual ~TShoot() { }
	void LoadConfig();

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		TShoot* t=new TShoot(*this);
		if (t)
		{
			t->LoadConfig();
		}
		return t;
	}

	virtual int selectRobot(World &world, bool candidates[], double bias[])
	{
		if (type == Deflect)
		{
			bool mycandidates[MAX_TEAM_ROBOTS];
			bool omni_candidate = false;
			for (int i=0; i<MAX_TEAM_ROBOTS; i++)
			{
				if (candidates[i] && world.teammate_type(i) == ROBOT_TYPE_OMNI)
				{
					mycandidates[i] = true;
					omni_candidate = true;
				}
				else
				{
					mycandidates[i] = false;
				}
				if (omni_candidate)
				{
					return Tactic::selectRobot(world, mycandidates, bias);
				}
			}
		}
		return Tactic::selectRobot(world, candidates, bias);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);

	double successProb(World &world);
};

//͵ս
class TSteal : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "steal";
	}

private:
	bool target_set;
	BCoordinate target;

public:
	TSteal() : RobotTactic(true, true)
	{
		target_set = false;
	}
	TSteal(BCoordinate _target)
	{
		target = _target;
		target_set = true;
	}

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TSteal(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};

//ս
class TClear : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "clear";
	}

private:
	MyVector2d prev_target;
	bool prev_target_set;

public:
	TClear() : RobotTactic(true, true) { }

	static Tactic *parser(const char *param_string)
	{
		return new TClear();
	}
	virtual Tactic *clone() const
	{
		return new TClear(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);

	double successProb(World &world);
};

class TActiveDef : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "active_def";
	}

private:
	TClear clear;
	TSteal steal;

public:
	TActiveDef() : RobotTactic(true, true) { }
	TActiveDef(BCoordinate _target) : RobotTactic(true, true)
	{
		steal = TSteal(_target);
	}

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TActiveDef(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};

//ս
class TPass : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "pass";
	}

private:
	int target;

public:
    TPass(int _target);

    static Tactic *parser(const char *param_string="1");
	virtual Tactic *clone() const
	{
		return new TPass(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};

//Ӵս
class TReceivePass : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "receive_pass";
	}

public:
	TReceivePass() : RobotTactic(true, true) { }

	static Tactic *parser(const char *param_string)
	{
		return new TReceivePass();
	}
	virtual Tactic *clone() const
	{
		return new TReceivePass(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};

//ս
class TDribbleToShoot : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "dribble_to_shoot";
	}

private:
	EvaluationPosition eval;

	static double eval_fn(World &world, const MyVector2d p,
	                      int obs_flags, double &a);

public:
	TDribbleToShoot(BRegion _region) : RobotTactic(true, true)
	{
		eval.set(_region, &eval_fn, 0.2442);
	}
	virtual ~TDribbleToShoot() { } // Compiler complains without this.

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TDribbleToShoot(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};

class TDribbleToRegion : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "dribble_to_region";
	}

private:
	BRegion region;

public:
	TDribbleToRegion(BRegion _region) : RobotTactic(true, true)
	{
		region = _region;
	}
	virtual ~TDribbleToRegion() { } // Compiler complains without this.

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TDribbleToRegion(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);

	virtual Status isDone(World &world, int me)
	{
		if (region.inRegion(world, world.ball_position()))
		{
			return Completed;
		}
		else
		{
			return InProgress;
		}
	}
};

class TSpinToRegion : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "spin_to_region";
	}

private:
	BRegion region;

public:
	TSpinToRegion(BRegion _region) : RobotTactic(true, true)
	{
		region = _region;
	}
	virtual ~TSpinToRegion() { } // Compiler complains without this.

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		return new TSpinToRegion(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);

	virtual Status isDone(World &world, int me)
	{
		if (region.inRegion(world, world.ball_position()))
		{
			return Completed;
		}
		else
		{
			return InProgress;
		}
	}
};

class TReceiveDeflection : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "receive_deflection";
	}

private:
	bool got_to_spin;

public:
	TReceiveDeflection() : RobotTactic(true, true)
	{
		got_to_spin = false;
	}

	static Tactic *parser(const char *param_string)
	{
		return new TReceiveDeflection();
	}
	virtual Tactic *clone() const
	{
		return new TReceiveDeflection(*this);
	}

	virtual double estimatedTime(World &world, int me)
	{
		return 0.0;
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);

	virtual Status isDone(World &world, int me);
};


#endif
