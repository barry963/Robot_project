/*
 * TITLE:  VQueue.cc
 *
 * PURPOSE:  This file implements the small size robot simulator classes.
 *
 * WRITTEN BY:	Brett Browning, adapted from the 2001 simulator written by
 *				Michael Bowling, Scott Lenser
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

#include <stdio.h>
#include <string.h>
#include "field_vqueue.h"


/**************************** TYPES ******************************************/


/***************************** Velocity QUEUE CLASS ***************************/
/*
 * VelocityQueue -
 *
 * This constructor intialises the variables in the class
 */
VelocityQueue::VelocityQueue(void)
{
  exec_cmd = 0;
  set_cmd = exec_cmd + 1;
  nr_commands = 0;
  memset(vbuffer, 0, sizeof(vbuffer[0]) * buffer_size);
  memset(tstamp, 0, sizeof(tstamp[0]) * buffer_size);
}

/*
 * SetCommand -
 *
 * set the next command in teh latency queue. It will take LatencySize
 * frames to execute
 */
void VelocityQueue::SetCommand(double vx, double vy, double va,
                               int kick_power, int dribble_power, bool bchipkick, int t)
{
  RVelocity	vc;

  vc.v.set(vx, vy);
  vc.va = va;
  vc.kick_power = kick_power;
  vc.dribble_power = dribble_power;
        vc.bchipkick = bchipkick;
  SetCommand(vc, t);
}

/*
 * SetCommand -
 *
 * set the next command in teh latency queue. It will take LatencySize
 * frames to execute
 */
void VelocityQueue::SetCommand(MyVector2d v, double va, int kick_power,
                               int dribble_power, bool bchipkick, int t)
{
  RVelocity vc;

  vc.v = v;
  vc.va = va;
  vc.kick_power = kick_power;
  vc.dribble_power = dribble_power;
  vc.bchipkick = bchipkick;
  SetCommand(vc, t);
}

/*
 * SetCommand -
 *
 * set the next command in teh latency queue. It will take LatencySize
 * frames to execute
 */
void VelocityQueue::SetCommand(RVelocity vc, int t)
{
  int prev_cmd = (set_cmd - 1);
  if (prev_cmd < 0)
        {
    prev_cmd += buffer_size;
        }
  t += LATENCY_FTIME;

  if (t == tstamp[prev_cmd])
        {
    set_cmd = prev_cmd;
    nr_commands--;
  }
        else if (t < tstamp[prev_cmd] || set_cmd == exec_cmd)
        {
                return;
        }

  vbuffer[set_cmd] = vc;
  tstamp[set_cmd] = t;
  set_cmd = (set_cmd + 1) % buffer_size;
  nr_commands++;

}

/*
 * GetCommand -
 *
 * Get the next command from the queue
 */
void VelocityQueue::GetCommand(double *vx, double *vy, double *va,
                               int *kick_power, int *dribble_power, bool* bchipkick, int t)
{
  RVelocity	vc;

  GetCommand(vc, t);
  *vx = vc.v.x;
  *vy = vc.v.y;
  *va = vc.va;
  *kick_power = vc.kick_power;
  *dribble_power = vc.kick_power;
  *bchipkick = vc.bchipkick;
}

/*
 * GetCommand -
 *
 * Get the next command from the queue
 */
void VelocityQueue::GetCommand(MyVector2d &v, double *va,
                               int *kick_power, int *dribble_power, bool* bchipkick, int t)
{
  RVelocity vc;

  GetCommand(vc, t);
  v = vc.v;
  *va = vc.va;
  *kick_power = vc.kick_power;
  *dribble_power = vc.kick_power;
  *bchipkick = vc.bchipkick;
}

/*
 * GetCommand -
 *
 * Get the next command from the queue
 */
void VelocityQueue::GetCommand(RVelocity &vc, int t)
{
  int nextcmd = (exec_cmd + 1) % buffer_size;

  /* return the command */
  vc = vbuffer[exec_cmd];

  /* move exec cmd along if time stamps are up */
  if ((nr_commands > 0) && (tstamp[nextcmd] <= t))
        {
    nr_commands--;
    exec_cmd = nextcmd;
  }
}


