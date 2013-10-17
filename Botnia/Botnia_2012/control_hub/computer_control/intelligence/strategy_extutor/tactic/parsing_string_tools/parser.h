// parse.h
//
// Utilities for parsing strings and files.
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

#ifndef __parse_h__
#define __parse_h__

#include "user_interface/field_related/motion_simulation_tools/field_vectors.h"

#include "control_hub/computer_control/intelligence/strategy_extutor/play/play.h"

namespace Parse
{
int skipWS(const char *string, const char *ws = " \t\n");
int skipLine(const char *string);

int pDouble(const char *string, double &x);
int pInt(const char *string, int &x);
int pChar(const char *string, char &x);
int pWord(const char *string, char *x, int n);
int pWord(const char *string, char **x);
int pLine(const char *string, char **x);
int pBool(const char *string, bool &x);
int pVector2d(const char *string, MyVector2d &x);
int pTCoordinate(const char *string, BCoordinate &x);
int pTCoordinateDir(const char *string, BCoordinate &x);
int pTRegion(const char *string, BRegion &x);
};

#endif
