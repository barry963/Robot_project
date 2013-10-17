#ifndef FIELD_TRACKER_H
#define FIELD_TRACKER_H
/*
 * TITLE:    VTracker.h
 *
 * PURPOSE:  Encapsulates the Extended Kalman Filter Tracker system
 *           It wraps around the ball_Tracker and robot_tracker classes
 *           It needs to be wrapped in a single class as there are
 *           between tracking objects interdependencies
 *
 * WRITTEN BY: Michael Bowling, Brett Browining
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


#include "user_interface/field_related/field_constants.h"
#include <stdio.h>
#include <stdlib.h>
#include "field_ball_tracker.h"
#include "field_robot_tracker.h"



struct VObjInfo
{
    MyVector2d pos;
    double conf;
    double orientation;
};

class ObjectTracker
{
public:
    BallTracker ball;
    RobotTracker robots[NUM_TEAMS][MAX_TEAM_ROBOTS];

    //保存合并摄像机信息后的没有ID号的机器人
    int iNoIDblueRobot;
    int iNoIDyellowRobot;
    VObjInfo BallInfos[NUM_CAMERAS][MAX_BALLS];
    VObjInfo RobotInfos[NUM_CAMERAS][NUM_TEAMS][MAX_ROBOTS];

    int id2index[NUM_TEAMS][MAX_ROBOT_ID];
    int index2id[NUM_TEAMS][MAX_TEAM_ROBOTS];

    void LoadConfig();

    // temp matrix to store ball covariances
    Matrix bcovar;

public:
    ObjectTracker(void);

    bool Exists(int team, int robot)
    {
        return (index2id[team][robot] >= 0);
    }
    double Height(int team, int robot)
    {
        switch (Type(team, robot))
        {
        case ROBOT_TYPE_DIFF:
            return DIFFBOT_HEIGHT;
        case ROBOT_TYPE_OMNI:
            return OMNIBOT_HEIGHT;
        default:
            return OPPONENT_HEIGHT;
        }
    }

    double Radius(int team, int robot)
    {
        if(team){};
        if(robot){};
        return 90.0;
    }

    int Type(int team, int robot)
    {
        if (team){};
        if (robot){};
        return ROBOT_TYPE_OMNI;
    }

    void ResetAll(void);

};





#endif // FIELD_TRACKER_H
