//
// strategy.h
//
// Encapsulates the strategy engine.
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu),
//              Brett Browning (brettb@cs.cmu.edu)
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

#ifndef __STRATEGY_H__
#define __STRATEGY_H__

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/goalie.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/play/play.h"

class PlayExecutor
{
private:
	typedef int assignment[MAX_PLAY_ROLES];
	assignment assign;

       // int type assignment

	int id_goalie, id_opportunity;

	double start_timestamp;
	double last_run_timestamp;
	double runtime;
	bool completed;

	Play *play;
	int sequence_index;
	int sequence_length;

	Status status;

	TRoleMap teammate_map;
	TRoleMap opponent_map;

	Tactic *tactics[MAX_PLAY_ROLES];
	Tactic *tactic_opportunity;
	Tactic *busy_tactics[MAX_TEAM_ROBOTS];

	TGoalie *tactic_goalie;
	TShoot *tactic_opportunity_off;
	TClear *tactic_opportunity_def;

	void makeCandidates(World &world, bool candidates[MAX_TEAM_ROBOTS]);

	void initialAssignment(World &world);
	void fixedAssignment(World &world);

	void updateMaps(World &world);

	void checkAssignment(World &world);
	void checkForOpportunity(World &world);

	Status nextInSequence(World &world);

	void setTactics(World &world, Tactic *t[],
	                bool set_anyone = true,
	                bool set_everyone = true,
	                bool set_goalie = true);
	void checkStatus(World &world);
	void checkBusy(World &world);

public:
	PlayExecutor();
	void LoadConfig();

	void set(World &world, Play *_play);
	void setGoalie(int _id_goalie)
	{
		id_goalie = _id_goalie;
	}

	void run(World &world, Tactic *t[]);

	Status isDone(World &world)
	{
		if (status != InProgress)
		{
			return status;
		}
		status = play->isDone(world);
		if (status == Completed || status == Succeeded)
		{
			return Completed;
		}
		if (status == Aborted || status == Failed)
		{
			return Aborted;
		}
		else
		{
			return InProgress;
		}
	}

	double runningTime(World &world)
	{
		return runtime;
	}
};

class PlayBook
{
private:

	class Results
	{
	public:
		int counts[5];

		//״̬
		Results()
		{
			counts[0]=counts[1]=counts[2]=counts[3]=counts[4]=0;
		}

		//״̬
		void incr(Status s)
		{
			counts[(int) s]++;
		}

		void gui_print()
		{
			gui_debug_printf(-1, GDBG_STRATEGY, "F:%d A:%d I:%d C:%d S:%d\n",
			                 counts[0], counts[1], counts[2], counts[3], counts[4]);
		}
	};

	vector<Play *> plays;
	vector<double> weights;
	vector<Results> results;
	vector<char *> play_names;

	double weight(uint index);

public:
	PlayBook() { }

	void add(Play *p, char *name, double w)
	{
		plays.push_back(p);
		play_names.push_back(name);
		weights.push_back(w);
		results.push_back(Results());
	}

	bool load(const char *filename);
	bool save(const char *filename);

	Play *select(World &w);

	void credit(Play *p, Status s)
	{
		for (uint i=0; i<plays.size(); i++)
			if (plays[i] == p)
			{
				results[i].incr(s);
				break;
			}
	}
};

class Warmup
{
private:
	int curr_goals[MAX_TEAM_ROBOTS];
	MyVector2d goalvec[MAX_TEAM_ROBOTS];
	int count;
	bool ready;
	MyVector2d cPosition;
	Tactic *temptac[MAX_TEAM_ROBOTS];
	static const double goalpts[MAX_TEAM_ROBOTS][2];

public:
	int initialized;
	int warmupNum;

	Warmup(void)
	{
		ready=false;
		count = 0;
		for (int i = 0; i < MAX_TEAM_ROBOTS; i++)
			goalvec[i].set(goalpts[i][0], goalpts[i][1]);
	}

	void basic(World &world, Tactic *tactics[]);
	void speed(World &world, Tactic *tactics[]);
	void spiralmove(World &world, Tactic *tactics[]);
	void spiral(World &world, Tactic *tactics[]);
	void sine(World &world, Tactic *tactics[]);
	void robottest(World &world, Tactic *tactics[]);
	void navchallenge(World &world, Tactic *tactics[]);
};

class Strategy
{
private:
	PlayBook playbook;
	PlayExecutor executor;
	Warmup drills;

	// Current and recent play.
	Play *current_play, *last_play;
	double last_play_endtime;
	Status last_play_status;

	// Tactic to stop robots.
	TStop t_stop;

	// Last game state.
	char last_game_state;

	double stopped_time;

	enum {NONE=0, PLAYS, WARMUP} doing;

public:
	Strategy();
	void LoadConfig();

        bool parse(char *string);

	void init(const char *playbook_file);

	void playEnded(World &world, Status status);
	Play *responsiblePlay(World &world);

	void stop(World &world, Tactic *tactics[]);

	void run(World &world, Tactic *tactics[]);
	void think(World &world);

	void credit(World &world, Play *play, Status status);
};



#endif /* __STRATEGY_H__ */

