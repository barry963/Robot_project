#ifndef FIELD_ANALYSOR_ROBOT_H
#define FIELD_ANALYSOR_ROBOT_H

/*
 * TITLE:        rtypes.h
 *
 * PURPOSE:      This is file contains robot types
 *
 * WRITTEN BY:   Michael Bowling, James R Bruce, Brett Browning
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
#include "control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_geometry.h"

// pthread_create start routine
typedef void *(*pthread_start)(void *);

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;


/* struct to encapsulate the three vector position */
//机器人坐标和方向
struct RPosition
{
        MyVector2d p;
        double dir;
};

/* structure to store the vcommands */
//机器人速度，角速度，是否击球，是否带球
struct RVelocity
{
        MyVector2d v;
        double   va;
        int kick_power;
        int dribble_power;
        bool bchipkick;
};


#endif // FIELD_RTYPES_H
