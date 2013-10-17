#ifndef FIELD_GLOBAL_FUNCTION_H
#define FIELD_GLOBAL_FUNCTION_H

// soccer.cc
//
// Defines global functions for all of the soccer code.
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


#include "net/net_vision_interface.h"
#include "net/net_vision_gui_interface.h"
#include <deque>
#include <QPointF>
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_parametres.h"
#include "user_interface/field_related/field_global_function.h"

enum Status { Failed, Aborted, InProgress, Completed, Succeeded, Busy};

// Gui Messages
struct gmsg
{
        char m[net_gui_out_maxsize];
        int size;
};

extern void RegisterTactics();
extern deque<net_gdebug> gmsg_queue;

inline const char *status_as_string(Status s)
{
        switch (s)
        {
        case Failed:
                return "Failed";
        case Aborted:
                return "Aborted";
        case InProgress:
                return "InProgress";
        case Completed:
                return "Completed";
        case Succeeded:
                return "Succeeded";
        case Busy:
                return "Busy";
        }
        return "";
}

void gui_debug_point(const char robot, MyVector2d p1 );
void gui_debug_line(const char robot, const char level,
                    MyVector2d p1, MyVector2d p2, char flags = 0);
void gui_debug_arc(const char robot, const char level,
                   MyVector2d p1, MyVector2d dimens,
                   double start_angle, double stop_angle,
                   char flags = 0);
void gui_debug_x(const char robot, const char level, MyVector2d p);
void gui_debug_printf(const char robot, const char level,
                      const char *fmt, ...);

void soccermain(void);

extern QString application_path;
extern char soccerside;
extern char soccerteam;
extern ParamSet* settings;
extern bool bEnCamera[2];

extern bool bEnBallNoise;
extern bool bEnBigRobotID;
extern bool bEnBlueRobotNoise;
extern bool bEnYellowRobotNoise;




#endif // FIELD_GLOBAL_FUNCTION_H
