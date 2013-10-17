// calibration_tactics.cc
//
// Calibration tactics.
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
#include "calibration_tactics.h"

// TLatency
//
// Checks the robot's command latency.
//

bool tlatency_registered = Tactic::registerParser("latency", TLatency::parser);

#define LATENCY_THRESH_ANG    0.03
#define LATENCY_THRESH_POS    3.0

void TLatency::run(World &world, int me)
{
	static int mode = 0;
	static int dir = 1;
	static double start_time;
	static double start_ang;
	static vector2f start_pos;
	vraw raw;
	world.teammate_raw(me, raw);
	switch (mode)
	{
	case 0:
		world.go(me, 0.0, 0.0, 0.0);
		start_time = world.time;
		start_ang = raw.angle;
		mode++;
		break;
	case 1:
		world.go(me, 0.0, 0.0, 0.0);
		if (world.time - start_time > 1.0) mode++;
		break;
	case 2:
		world.go(me, 0.0, 0.0, 6.0 * dir);
		start_time = world.time;
		start_ang = raw.angle;
		mode++;
		fprintf(stderr, "Latency Turn (+/- %f): ", LATENCY_THRESH_ANG / 6.0);
		break;
	case 3:
		world.go(me, 0.0, 0.0, 6.0 * dir);
		if (fabs(anglemod(start_ang - raw.angle)) > LATENCY_THRESH_ANG)
		{
			fprintf(stderr, "%f\n", world.time - start_time);
			start_time = world.time;
			mode++;
		}
		break;
	case 4:
		world.go(me, 0.0, 0.0, 0.0);
		start_time = world.time;
		start_ang = raw.angle;
		mode++;
		break;
	case 5:
		world.go(me, 0.0, 0.0, 0.0);
		if (world.time - start_time > 1.0) mode++;
		break;
	case 6:
		world.go(me, 1000.0 * dir, 0.0, 0.0);
		start_time = world.time;
		start_pos = raw.pos;
		mode++;
		fprintf(stderr, "Latency Forw (+/- %f): ",
		        LATENCY_THRESH_POS / 1000.0);
		break;
	case 7:
		world.go(me, 1000.0 * dir, 0.0, 0.0);
		if ((start_pos - raw.pos).length() > LATENCY_THRESH_POS)
		{
			fprintf(stderr, "%f\n", world.time - start_time);
			start_time = world.time;
			mode++;
		}
		break;
	case 8:
		mode = 0;
		dir *= -1;
		break;
	}
}

Tactic *TLatency::parser(const char *param_string)
{
	return new TLatency();
}

