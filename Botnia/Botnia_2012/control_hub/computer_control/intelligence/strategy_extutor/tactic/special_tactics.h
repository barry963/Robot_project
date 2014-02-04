// ************************************************************************************************
//     Copyright 2013-2014 modified by Lu Chunqiu
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//
//     Additional license information:
//
//  **********************************************************************************************/

// special_tactics.h
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

#ifndef __special_tactics_h__
#define __special_tactics_h__

#pragma warning(disable:4100)

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"

#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"
//lu_test add

class TPositionForStart : public SPosition
{
public:
	virtual const char *name() const
	{
		return "StartLoc";
	}
	TPositionForStart(BCoordinate _position, BCoordinate _faceto) :
			SPosition(_position, _faceto)
	{
		active = true;
	}

	static Tactic *parser(const char *param_string);

	virtual Tactic *clone() const
	{
		return new TPositionForStart(*this);
	}
	virtual Status isDone(World &world, int me);
};


class TPositionForKick : public RobotTactic
{
private:
	MyVector2d prev_target;
	bool prev_target_set;

public:
	TPositionForKick();
	virtual const char *name() const
	{
        return "position_for_kick";
	}

	static Tactic *parser(const char *param_string)
	{
		return new TPositionForKick();
	}

	void LoadConfig();
	virtual Tactic *clone() const
	{
		return new TPositionForKick(*this);
	}
	virtual Status isDone(World &world, int me)
	{
		//COMM_START		   's'
		if (strchr("s ", world.game_state) != NULL)
		{
			return Succeeded;
		}
		else
		{
			return InProgress;
		}
	}

	//ѡ񱾶Ļ
	virtual int selectRobot(World &world, bool candidates[], double bias[])
	{
		int best_i = -1;
		double best = 0;
		for (uint i=0; i<MAX_TEAM_ROBOTS; i++)
		{
			if (!candidates[i])
				continue;
			if (bias[i] < 0.0)
				return i;
			double d = (world.GetRobotPositionByID(i) - world.ball_position()).sqlength();
			if ((best_i < 0) || (d < best))
			{
				best = d;
				best_i = i;
			}
		}
		return best_i;
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};


// position the non-kicking players behind the line when a penalty occursߺ
class TPositionForPenalty : public RobotTactic
{
public:
	TPositionForPenalty();
	virtual const char *name() const
	{
		return "PenaltyLoc";
	}

	void LoadConfig(void);
	static Tactic *parser(const char *param_string)
	{
		return new TPositionForPenalty();
	}

	virtual Tactic *clone() const
	{
		return new TPositionForPenalty(*this);
	}
	virtual Status isDone(World &world, int me)
	{
		return (world.game_state == COMM_START) ? Succeeded : InProgress;
	}
	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};


class TChargeBall : public RobotTactic
{
public:
	virtual const char *name() const
	{
		return "ChargeBall";
	}
	TChargeBall() { }

	static Tactic *parser(const char *param_string)
	{
		return new TChargeBall();
	}
	virtual Tactic *clone() const
	{
		return new TChargeBall(*this);
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
};


class TSuccess : public Tactic
{
private:
	double start_time;
	double wait;

public:
	virtual const char *name() const
	{
		return "Success";
	}
	TSuccess(double _wait = 0)
	{
		wait = _wait;
		start_time = -1;
	}

	static Tactic *parser(const char *param_string)
	{
		return new TSuccess();
	}
	virtual Tactic *clone() const
	{
		return new TSuccess(*this);
	}

	virtual Status isDone(World &world, int me)
	{
		if (world.time - start_time > wait) return Succeeded;
		else return InProgress;
	}
	virtual void run(World &world, int me)
	{
		if (start_time < 0)
		{
			start_time = world.time;
		}
		world.halt(me);
	}
};


class TComplete : public Tactic
{
private:
	double start_time;
	double wait;

public:
	virtual const char *name() const
	{
		return "Complete";
	}
	TComplete(double _wait = 0)
	{
		wait = _wait;
		start_time = -1;
	}

	static Tactic *parser(const char *param_string)
	{
		return new TComplete();
	}
	virtual Tactic *clone() const
	{
		return new TComplete(*this);
	}

	virtual Status isDone(World &world, int me)
	{
		if (world.time - start_time > wait) return Completed;
		else return InProgress;
	}
	virtual void run(World &world, int me)
	{
		if (start_time < 0)
		{
			start_time = world.time;
		}
		world.halt(me);
	}
};


class TPassandReceive: public RobotTactic
{
private:
    TPass  *PassRobot;
    TReceivePass *ReceiveRobot;
    SPosition *PositionRobot;
    //static int PasserRobot=0;

public:
    TPassandReceive();
    ~TPassandReceive();

    virtual const char *name() const
    {
        return "PassandReceive";
    }

    static Tactic *parser(const char *param_string)
    {
        return new TPassandReceive();
    }

    void LoadConfig();
    virtual Tactic *clone() const
    {
        return new TPassandReceive(*this);
    }
/*
    virtual Status isDone(World &world, int me)
    {
        //COMM_START		   's'
        if (strchr("s ", world.game_state) != NULL)
        {
            return Succeeded;
        }
        else
        {
            return InProgress;
        }
    }
*/

    virtual int selectRobot(World &world, bool candidates[], double bias[])
    {
        int best_i = -1;
        double best = 0;
        for (uint i=0; i<MAX_TEAM_ROBOTS; i++)
        {
            if (!candidates[i])
                continue;
            if (bias[i] < 0.0)
                return i;
            double d = (world.GetRobotPositionByID(i) - world.ball_position()).sqlength();
            if ((best_i < 0) || (d < best))
            {
                best = d;
                best_i = i;
            }
        }
        return best_i;
    }

    virtual void command(World &world, int me, Robot::RobotCommand &command,
                         bool debug);
};


#endif
