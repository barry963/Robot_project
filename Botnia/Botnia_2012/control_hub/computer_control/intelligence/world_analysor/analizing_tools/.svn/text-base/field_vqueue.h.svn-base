#ifndef FIELD_VQUEUE_H
#define FIELD_VQUEUE_H
/*
 * TITLE: Vqueue.h
 *
 * PURPOSE: This file implements the small size robot simulator classes.
 *
 * WRITTEN BY: Brett Browning, Michael Bowling
 */
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

#include "field_geometry.h"
#include "user_interface/field_related/field_constants.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_robot.h"


/* size of latency in vision frames - need to add the frame
 * for the command to be sent
 *
 * mhb: fiddling with this... it seems to get true latency you need to
 *      subtract 1, not add 1.
 */

#define LATENCY_FTIME ((int) (LATENCY_DELAY * FRAME_RATE + 0.5) - 2)

/*
 * VelocityQueue -
 *
 * This class implements teh velocity queue to model the latency
 * that occurs in the small size system. It is effectively a FIFO
 * with a fixed removal such that each command undergoes a latency_size
 * delay before execution
 */

class VelocityQueue
{
public:
        static const int buffer_size = LATENCY_FTIME + 2;

        RVelocity	vbuffer[buffer_size];
        int		tstamp[buffer_size];
        int		exec_cmd, set_cmd;
        int		nr_commands;

public:
        VelocityQueue();
        void SetCommand(double vx, double vy, double va, int kick_power, int dribble_power, bool bchipkick, int t);
        void SetCommand(MyVector2d v, double va, int kick_power, int dribble_power, bool bchipkick, int t);
        void SetCommand(RVelocity vc, int t);
        void GetCommand(double *vx, double *vy, double *va, int *kick_power, int *drib_power, bool* bchipkick, int t);
        void GetCommand(MyVector2d &v, double *va, int *kick_power, int *drib_power, bool* bchipkick, int t);
        void GetCommand(RVelocity &vc, int t);
};


#endif // FIELD_VQUEUE_H
