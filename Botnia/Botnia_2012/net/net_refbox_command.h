#ifndef FIELD_NETCOMMAND_H
#define FIELD_NETCOMMAND_H
/*
 * TITLE:	Commands.h
 *
 * PURPOSE: This file contains the constants that define the various
 *			serial commands the referee box can send to the operating
 *			computers.
 *
 * WRITTEN BY: Brett Browning
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


/* Baud rate */
#define COMM_BAUD_RATE			9600

/* commands to actually start and stop play */
#define COMM_START				's'
#define COMM_STOP				'S'

#define COMM_FIRST_HALF			'1'
#define COMM_HALF_TIME			'h'
#define COMM_SECOND_HALF		'2'
#define COMM_OVER_TIME			'o'
#define COMM_PENALTY_SHOOTOUT	'a'

/* game flow commands */
#define COMM_RESTART			'r'

#define COMM_KICKOFF_YELLOW		'k'
#define COMM_KICKOFF_BLUE		'K'

#define COMM_PENALTY_YELLOW		'p'
#define COMM_PENALTY_BLUE		'P'

#define COMM_FREEKICK_YELLOW	'f'
#define COMM_FREEKICK_BLUE		'F'

/* update commands */
#define COMM_TIMEOUT_YELLOW		't'
#define COMM_TIMEOUT_BLUE		'T'
#define COMM_TIMEOUT_END		'z'

#define COMM_GOAL_YELLOW		'g'
#define COMM_GOAL_BLUE			'G'


#endif // FIELD_NETCOMMAND_H
