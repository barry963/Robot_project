#ifndef FIELD_WORLD_H
#define FIELD_WORLD_H
// world.h
//
// This class holds the entire state of the soccer system.  This includes
// the known state of the world as well as any required memory for the
// system.
//
// Any general state retrieval methods go in this class.
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
#include "control_hub/computer_control/intelligence/item_property_excutor/field_ball_tracker.h"
#include "control_hub/computer_control/intelligence/item_property_excutor/field_robot_tracker.h"
#include "net/net_vision_receive_thread.h"
#include "net/net_refbox_command.h"
#include "control_hub/computer_control/knowledge_base/database/world_state/field_world_robot.h"
#include "control_hub/computer_control/knowledge_base/algorithms/path_planner/path_planner.h"

#include <QMutex>
/******************** serial port communication ********************************/

#include "wireless/serial_server.h"

extern QMutex robot_return_info_mutex_;
class SerialServer;
class SLIPOperation;
class DECTOperation;
class TransparentOperation;
/*****************************************************************/


//#include "../field_base_constants.h"

//class VTracker;

class Robot;



struct VBallInfo
{
    MyVector2d pos;
    double conf;
};

struct VRobotInfo
{
    MyVector2d pos;
    double conf;
    double orientation;
    QString Tactic;
};

struct VisionInfo
{
    double time;
    QString sPlay;
    VBallInfo  Balls[MAX_BALLS];
    VRobotInfo Robots[NUM_TEAMS][MAX_ROBOTS];
};

struct VINFO_RAW
{
    double time;
    VBallInfo  BallInfos[NUM_CAMERAS][MAX_BALLS];
    VRobotInfo RobotInfos[NUM_CAMERAS][NUM_TEAMS][MAX_ROBOTS];
};

extern VisionInfo vision_info;
extern VINFO_RAW VInfoRaw;

//-------------------------------------------------------------------------

class World
{

private:
    struct RobotReturnInfo
    {
        double cell_voltage_;
        double capacitor_voltage_;
        double kicking_voltage_;
        double current_level_;
        int number_of_package_;
    };
    RobotReturnInfo robot_return_info_;



public:


    // serial server operations
    SerialServer * serial_sever_;
    // declaretion of the vision infomation
    VisionInfo vision_info_;

    // declaretion of the tracking system for robots and ball
    ObjectTracker object_tracker_;

    // declaretion the array of robots' id, for our team and oppoent's team
    int teammate_id_to_index_[MAX_TEAM_ROBOTS];
    int opponent_id_to_index_[MAX_TEAM_ROBOTS];

    // it seems this function is to update some paramters related to strategy excution
    void UpdateHighLevel();

    // stores the time duration when the ball is in our zone
    double our_dzone_duration_;

    // stores the time duration when the ball is in the opponents' zone
    double their_dzone_duration_;

    // store the time duration when we are in control of the ball
    double possession_our_duration_;

    // store the time duration when the opponents are in control of the ball
    double possession_their_duration_;

    // store the last refer box state
    char last_ref_state_;

public:
    World();
    ~World();
    void LoadConfig();


    // store the color and the side information of team
    char color_;
    char side_;

    void setside(int side);
    void setcolor(int color);


    // initialize
    void init(int _side, int _color);


    // Update
    void update(VisionInfo& info0);



    // Time information
    // The timestamp corresponding to the world's state.
    double time;
    // This is the time it will be before any robot would
    double now;

    //  receive a command from the system.  This predicts
    //  ahead our latency.
    //  This is substituded as the time argument if the time
    //  argument is not specified or is negative.


    /* from here is the infomation of the objects on the field */

    // Basic Field Information
    // central point of our goal
    MyVector2d our_goal, their_goal;
    // inside the our goal, store the position of two pillars
    MyVector2d our_goal_r, our_goal_l;
    // inside the opponents' goal, store the position of two pillars
    MyVector2d their_goal_r, their_goal_l;

    // TODO(gaoyuankidult@gmail.com): check what is this vector
    MyVector2d down_field;

    inline MyVector2d to_world(const MyVector2d x)
    {
        return x * side_;
    }
    inline double to_world_dir(const double x)
    {
        return anglemod((side_ < 0 ? M_PI : 0.0) + x);
    }
    inline MyVector2d from_world(const MyVector2d x)
    {
        return x * side_;
    }
    inline double from_world_dir(const double x)
    {
        return (side_ < 0 ? M_PI : 0.0) + x;
    }

    // Basic Ball Information
    // stores the position info
    MyVector2d ball_position(double time = -1);
    // stores the velocity info
    MyVector2d ball_velocity(double time = -1);
    // return the ball covariance
    Matrix ball_covariances(double time = -1);

    // return the confidence of ball at the position
    double ball_raw(VisionRawInfo &vpos);

    // return the id of robot which touched the ball
    int ball_collision(double time = -1);


    /* from here is the teammates infomation */

    // Basic Teammate Information
    int n_teammates;
    // return teamrobot types (version of roobots)
    int teammate_type(int id);

    // return the radius of specified robot
    double teammate_radius(int id);
    // get id of robot return the index of robot
    int teammate_frame_index(int id);

    // get the posisiton of teammate
    MyVector2d GetRobotPositionByID(int id, double time = -1);

    // get the velocity of teammate
    MyVector2d GetRobotVelocityByID(int id, double time = -1); // Not relative
    MyVector2d teammate_robot_velocity(int id, double time = -1);

    // get the direction of the teammate
    double teammate_direction(int id, double time = -1);

    // get the teammat angular velocity
    double teammate_angular_velocity(int id, double time = -1);

    // check whether the teammate as been stucked
    double teammate_stuck(int id);
    void teammate_raw(int id, VisionRawInfo &vpos);

    // get the command of the teammate
    void teammate_command(int id, double vx, double vy, double va);

    /* from here is the opponent infomation */
    // Basic Opponent Information
    int n_opponents;
    // get position of opponent
    MyVector2d opponent_position(int id, double time = -1);

    // get velocity of opponent
    MyVector2d opponent_velocity(int id, double time = -1);
    void opponent_raw(int id, VisionRawInfo &vpos);

/* from here is the game state from ref box*/
    // for saving game state
    char game_state;
    // type of scored
    char goal_scored; // 1: our goal, -1: their goal, 0: no goal

    // Send Command to Robot
    void go(int id, double vx=0, double vy=0, double va=0,
            int kick_power = 0,
            bool chipkick_on = false,
            int dribbler_speed=0,
            bool forcekick_on=false,
            bool sendback_on=false);

    // halt the state of game
    void halt(int id);

    // Robot internal state
    Robot *robot[MAX_TEAM_ROBOTS];

    //set path panner for each robot
    PathPlanner path[MAX_TEAM_ROBOTS];

    /////////////////////////////////////////////////////////////////
    //
    // High-Level Information
    //
    // Useful general computations about the state of the world.
    //

    /////////////////////////
    // Predicate Information

    // this enum contains the possession of the balls
    enum Possession { TheirBall, LooseBall, OurBall };

    // this enum contains the positio of the balls on the field
    enum FieldPosition { TheirSide, Midfield, OurSide };

    // this enum contains the situation of the team
    enum Situation { Defense, Offense, Special };

    // stores the infomation of which robot has the ball
    Possession possession;	//，{World::TheirBall,World::OurBall,World::LooseBall}

    // stores the infomation of which field has the ball
    FieldPosition fieldPosition;

    // stores the infomation of state of our team
    Situation situation;

    // This is related to the ball's current position but with
    // positional hysteresis.
    double ballXThreshold, ballYThreshold;

    // Returns -1 or +1 corresponding to which side has the ball, or is
    // the opponent's strong side.  Uses above hysteresis thresholds.

    // check which side has the ball
    int sideBall();

    // check which side is stroger
    int sideStrong();

    // check
    int sideBallOrStrong();

    // Hysteresis defense zone booleans.  Uses a timed hysteresis.

    // check whether ball is in our defence zone
    bool inOurDefenseZone();

    // check whether ball is in their defence zone
    bool inTheirDefenseZone();

    /////////////////////////
    // Roles

    // Opponent Roles 对方角色
    int orole_goalie;

    // Teammate Roles 己方角色
    int trole_goalie;
    int trole_active;

    bool twoDefendersInTheirDZone();

    /////////////////////////
    // Restarts

    // Is the current state a restart?  Is it our kick?
    bool restart();
    bool isGameRunning()
    {
        return (game_state == COMM_START);
    }
    bool restartPenalty();
    bool restartNeutral();
    Possession restartWhoseKick();

    // Is the ball out of play?
    // Where would the free kick likely be taken given a last ball position.
    bool ballOutOfPlay(double time = -1);
    MyVector2d freeKickPosition(MyVector2d last_ball_position);

    /////////////////////////
    // Obstacle Computations

    // Finds the nearest teammate/opponent to a point on the field.
    int nearest_teammate(MyVector2d p, double time = -1);
    int nearest_opponent(MyVector2d p, double time = -1);

    // Obs methods return an obs_flag set to why a position or other
    // shape is not open.  Or zero if the position or shape is open
    //
    // obsLineNum() returns the number of obstacles on the line.
    //
    // obsBlocksShot() returns wether a point would block a shot right
    // now.

    int obsPosition(MyVector2d p, int obs_flags,
                    double pradius = TEAMMATE_EFFECTIVE_RADIUS,
                    double time = -1);
    int obsLine(MyVector2d p1, MyVector2d p2, int obs_flags,
                double pradius = TEAMMATE_EFFECTIVE_RADIUS, double time = -1);
    int obsLineFirst(MyVector2d p1, MyVector2d p2, int obs_flags,
                     MyVector2d &first,
                     double pradius = TEAMMATE_EFFECTIVE_RADIUS,
                     double time = -1);
    int obsLineNum(MyVector2d p1, MyVector2d p2, int obs_flags,
                   double pradius = TEAMMATE_EFFECTIVE_RADIUS, double time = -1);

    bool obsBlocksShot(MyVector2d p,
                       double pradius = TEAMMATE_EFFECTIVE_RADIUS,
                       double time = -1);

    int choosePenaltyKicker(void);

    /////////////////////////
    // Miscellaneous

    // Given a direction returns either it or the opposite way, whichever
    //  is closest to the robot's current direction.
    double teammate_nearest_direction(int id, double d)
    {
        if (fabs(anglemod(teammate_direction(id) - d)) > M_PI_2)
            return anglemod(d + M_PI);
        else
            return d;
    }

    void set_cell_voltage(double cell_voltage){robot_return_info_.cell_voltage_ = cell_voltage;}
    void set_capacitor_voltage(double capacitor_voltage){robot_return_info_.capacitor_voltage_ = capacitor_voltage;}
    void set_kicking_voltage(double kicking_voltage){robot_return_info_.kicking_voltage_ = kicking_voltage;}
    void set_current_level(double current_level){robot_return_info_.current_level_ = current_level;}
    void set_number_of_package(int number_of_package){robot_return_info_.number_of_package_ = number_of_package;}
    void set_robot_return_info(struct RobotReturnInfo robot_return_info){robot_return_info_ = robot_return_info;}

    double cell_voltage(){return robot_return_info_.cell_voltage_;}
    double capacitor_voltage(){return robot_return_info_.capacitor_voltage_;}
    double kicking_voltage(){return robot_return_info_.kicking_voltage_;}
    double current_level(){return robot_return_info_.current_level_;}
    int number_of_package(){return robot_return_info_.number_of_package_;}
    struct RobotReturnInfo robot_return_info(){return robot_return_info_;}


};

extern World world;//describe the real word lu_test

#endif // FIELD_WORLD_H
