// goalie.h
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

#ifndef __goalie_h__
#define __goalie_h__

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"

class TGoalie : public RobotTactic
{
public:
	double penalty_tend;
	bool penalty_play;

	virtual const char *name() const
	{
		return "goalie";
	}

	TGoalie(void);
	void LoadConfig();

	static Tactic *parser(const char *param_string);
	virtual Tactic *clone() const
	{
		TGoalie* t=new TGoalie(*this);
		if (t)
		{
			t->LoadConfig();
		}
		return t;
	}

	virtual void command(World &world, int me, Robot::RobotCommand &command,
	                     bool debug);
	bool penaltyCharge(World &world, int me, Robot::RobotCommand &command,
	                   bool debug);
	void penaltyPlay(World &world, int me, Robot::RobotCommand &command,
	                 bool debug);

};



#endif
