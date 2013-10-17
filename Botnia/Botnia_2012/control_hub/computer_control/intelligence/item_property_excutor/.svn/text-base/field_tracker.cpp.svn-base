#include "field_tracker.h"

/*
 * TITLE:    Vtracker.cc
 *
 * PURPOSE:  Encapsulates the Extended Kalman Filter Tracker system
 *           It wraps around the ball_Tracker and robot_tracker classes
 *           It needs to be wrapped in a single class as there are
 *           between tracking objects interdependencies
 *
 * WRITTEN BY: Michael Bowling, Brett Browining
 */


#include <string.h>
#include <assert.h>
#include <QDebug>

/****************************** CONSTANTS ****************************/

#define ISBLUE(c)          ((c) == TEAM_BLUE)
#define CHOOSETEAM(c, b, y)   (ISBLUE(c) ? (b) : (y))

//#define DEBUG

/****************************** CODE *********************************/

ObjectTracker::ObjectTracker(void)
{
    iNoIDblueRobot=0;
    iNoIDyellowRobot=0;

    for (int t = 0; t < NUM_TEAMS; t++)
    {
        for (int i = 0; i < MAX_TEAM_ROBOTS; i++)
        {
            index2id[t][i] = -1;
            robots[t][i].set_type(ROBOT_TYPE_OMNI);
        }
        for (int i = 0; i < MAX_ROBOT_ID; i++)
        {
            id2index[t][i] = -1;
        }
    }
    // set an internal object tracker opinter of ball tracker
    ball.set_tracker(this);
}

void ObjectTracker::LoadConfig()
{
    int i,j;
    ball.LoadConfig();
    for(i=0;i<NUM_TEAMS;i++)
    {
        for(j=0;j<MAX_TEAM_ROBOTS;j++)
        {
            robots[i][j].LoadConfig();
        }
    }
}

void ObjectTracker::ResetAll(void)
{
    ball.reset();
    for (int t = 0; t < NUM_TEAMS; t++)
    {
        for (int i = 0; i < MAX_TEAM_ROBOTS; i++)
        {
            if (index2id[t][i] >= 0)
            {
                robots[t][i].reset();
            }
        }
    }
}
