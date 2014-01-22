#ifndef FIELD_CONSTANTS_H
#define FIELD_CONSTANTS_H
/*
 * TITLE:        user_interface/field_related/field_constants.h
 *
 * PURPOSE:      This is file contains the major system constants
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


#include "field_size_constants.h"

#define MAX_BALLS       4
#define MAX_ROBOTS      5
const int MAX_TEAM_ROBOTS = 5;

#define MAX_ROBOT_ID    16

#define TEAM_NONE   0
#define TEAM_BLUE   1
#define TEAM_YELLOW 2

#define NUM_CAMERAS 2

#define NUM_TEAMS   3

#define SIDE_LEFT   1
#define SIDE_RIGHT -1

/* robot types */
#define ROBOT_TYPE_NONE   0
#define ROBOT_TYPE_DIFF   1
#define ROBOT_TYPE_OMNI   2 //wan xiang lun
#define ROBOT_TYPE_GOALIE 3
#define NUM_ROBOT_TYPES   4

// check these!
#define DIFF_WHEELBASE_RADIUS 60.0
#define OMNI_WHEELBASE_RADIUS 60.0
/* OMNI_WHEELBASE_RADIUS is 73.0 from measurement */

/* macro for team identity */
#define IS_YELLOW(y, a, b)     ((y) ? (a) : (b))

/* macro to determine the robot type from the id */
#define ROBOT_TYPE_ID(id)      (((id) > 4) ? ROBOT_TYPE_OMNI : ROBOT_TYPE_DIFF)

//==== Field Dimensions (mm) =========================================//

// diagonal is of 6050 x 4050 is 7280.46
#define FIELD_LENGTH_H			(FieldConstantsRoboCup2012::field_length_ /2)
#define FIELD_WIDTH_H				(FieldConstantsRoboCup2012::field_width_  /2)

#define GOAL_DEPTH          180
//禁区Y范围
#define DEFENSE_WIDTH       (FieldConstantsRoboCup2012::defense_radius_*2+FieldConstantsRoboCup2012::defense_stretch_)
//禁区X范围
#define DEFENSE_DEPTH       FieldConstantsRoboCup2012::defense_radius_
#define WALL_WIDTH          FieldConstantsRoboCup2012::goal_wall_width_

//球门
#define GOAL_WIDTH_H				(FieldConstantsRoboCup2012::goal_width_   /2)
#define GOAL_DEPTH_H				(GOAL_DEPTH   /2)
#define DEFENSE_WIDTH_H			(DEFENSE_WIDTH/2)
#define DEFENSE_DEPTH_H			(DEFENSE_DEPTH/2)

//罚球 Penalty
//己方开球罚球点
#define PENALTY_SPOT        (FIELD_LENGTH_H - FieldConstantsRoboCup2012::penalty_spot_from_field_line_dist_)

//任意球 Free Kick
#define FREEKICK_FROM_WALL		150
#define FREEKICK_FROM_GOAL		375

#define BALL_RADIUS						21
#define BALL_DIAMETER					(BALL_RADIUS * 2)

//==== Robot Dimensions (mm) =========================================//

#define ROBOT_DEF_WIDTH     180.0
#define ROBOT_DEF_WIDTH_H   (ROBOT_DEF_WIDTH / 2.0)

#define DIFFBOT_WIDTH       160.0
#define DIFFBOT_LENGTH      120.0
#define DIFFBOT_WIDTH_H     (DIFFBOT_WIDTH / 2.0)
#define DIFFBOT_LENGTH_H    (DIFFBOT_LENGTH / 2.0)
#define DIFFBOT_HEIGHT      100.0
#define DIFFBOT_RADIUS      100.0 // sqrt(width^2 + length^2)

#define OMNIBOT_RADIUS       90.0
#define OMNIBOT_HEIGHT      150.0

#define TEAMMATE_HEIGHT     100.0
#define OPPONENT_HEIGHT     100.0

const double TEAMMATE_EFFECTIVE_RADIUS=100.0;
#define OPPONENT_EFFECTIVE_RADIUS  90.0


//==== Obstacle Flags ================================================//

// Standard Obstacles
const int OBS_BALL=(1 << 0);
const int OBS_WALLS=(1 << 1);
//对方禁区障碍
const int OBS_THEIR_DZONE=(1 << 2);
//己方禁区障碍
const int OBS_OUR_DZONE=(1 << 3);

const int OBS_TEAMMATES=((1 << MAX_TEAM_ROBOTS) - 1) <<  4;
const int OBS_OPPONENTS=((1 << MAX_TEAM_ROBOTS) - 1) << (4 + MAX_TEAM_ROBOTS);
const int OBS_EVERYTHING=~((int) 0);
const int OBS_EVERYTHING_BUT_US=OBS_EVERYTHING & (~(OBS_TEAMMATES));
const int OBS_EVERYTHING_BUT_BALL=OBS_EVERYTHING & (~(OBS_BALL));

inline int OBS_TEAMMATE(int id)
{
        if (id<0 || id>=4)return 0;
        else
        {
                return 1 << (4 + id);
        }
};
//#define OBS_TEAMMATE(id) (1 << (4 + id))
#define OBS_OPPONENT(id) (1 << (4 + MAX_TEAM_ROBOTS + id))
#define OBS_EVERYTHING_BUT_ME(id) (OBS_EVERYTHING & (~(OBS_TEAMMATE(id))))


//==== Miscellaneous =================================================//

/* Frame time and latencies */
#define FRAME_RATE    61.57
#define FRAME_PERIOD  (1.0 / FRAME_RATE)

// this needs to be remeasured
#define LATENCY_DELAY 0.200

#define CAMERA_HEIGHT 3500.0

/* Constants */
#define GRAVITY 9800 // mm/s^2

/*Limited deviation*/
const float POSITION_ACCURACY = 20; //approximate there

/*
#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
*/


#ifndef M_PI_4
#define M_PI_4     0.785398163397448309616
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2  0.707106781186547524401
#endif

#ifndef M_PI
#define M_PI      3.14159265358979323
#endif
#ifndef M_PI_2
#define M_PI_2    1.57079632679489661923
#endif
#ifndef M_2PI
#define M_2PI     6.28318530717958648
#endif
#ifndef M_PI_8
#define M_PI_8    0.39269908169872414
#endif
#ifndef M_PI_16
#define M_PI_16   0.19634954084936207
#endif

/* Macros to convert between radians and degrees */
#define RAD2DEG(r)	((r) * 180.0 / M_PI)
#define DEG2RAD(r)	((r) * M_PI / 180.0)

//==== Network Info ===================================================//

// this will change - BB
#define PORT_MAIN              32882
#define PORT_VISION_SERVER     32883
#define PORT_VISION_SERVER_OUT (PORT_VISION_SERVER + 1)
#define PORT_RADIO_SERVER      32885
#define PORT_RADIO_SERVER_OUT  (PORT_RADIO_SERVER + 1)

extern bool bDebugPathPlan;
extern bool bDebugTShoot;
extern bool bDebugTactic;

//仿真所用

const double timePerFrame = FRAME_PERIOD; // 30 fps

const double robotHalfSize = OMNIBOT_RADIUS; // mm
const double maxRobotSpeed = 3000.0; // mm/s
const double maxBallSpeed = 13000.0;  // mm/s
const double maxRobotAccel = 3000.0; // mm/s^2

//球与地毯间摩擦系数
const double ballCarpetFrictionCoef = 0.03;
const double wallPerpBounceDecay = .4;
const double robotPerpBounceDecay = .37;

const double minX = -FIELD_LENGTH_H;
const double maxX = FIELD_LENGTH_H;
const double minY = -FIELD_WIDTH_H;
const double maxY = FIELD_WIDTH_H;


// view constants


const int MAX_CAMERA_COUNT=2;
const int MAX_BALL_COUNT=3;
const int MAX_ROBOT_COUNT=7;




#endif // FIELD_SYSTEM_CONSTANT_H
