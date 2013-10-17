
// ************************************************************************************************
//     Copyright 2012 Gao Yuan (robotics-computing.tk)
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//
//     Additional license information:
//
//  **********************************************************************************************/

/**    @file    - move_as_straight_tactic.cpp
  *
  *    @class   - TMoveAsStraight
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 12/20/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#include "move_as_straight_tactic.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"



const double FACE_UP = 0.0;
const double FACE_DOWN = -M_PI;


const MyVector2d TMoveAsStraight::START_POINT = MyVector2d(1500,1500);
const MyVector2d TMoveAsStraight::END_POINT = MyVector2d(1500,-1500);

// set variable as PDataMap
CR_DECLARE(SHOOT_AIM_PREF_AMOUNT)
CR_DECLARE(DISTANCE_FROM_PENALTY_LINE)

inline void cr_do_setup()
{
    CR_SETUP(tactic, SHOOT_AIM_PREF_AMOUNT, CR_DOUBLE);
    CR_SETUP(tactic, DISTANCE_FROM_PENALTY_LINE, CR_DOUBLE);
}

TMoveAsStraight::TMoveAsStraight()
{
}

void TMoveAsStraight::LoadConfig()
{
    cr_do_setup();
}

void TMoveAsStraight::command(World &world, int me,
                         Robot::RobotCommand &command,
                         bool debug)
{
    double radius_limite = 3;
    MyVector2d robot_postion = world.GetRobotPositionByID(me);
    current_target_ = MyVector2d(0,0);
    //MyVector2d target = MyVector2d(0,0);
    double target_distance = 0.0;
    if(ToStart())
    {
        target_distance = (robot_postion - START_POINT).length();
        current_target_ = START_POINT;
        if(target_distance < radius_limite)
        {
            start_or_end_= AT_START_POINT;
        }
    }
    else
    {
        target_distance = (robot_postion - END_POINT).length();
        current_target_ = END_POINT;
        if(target_distance < radius_limite)
        {
            start_or_end_= AT_END_POINT;
        }
    }


    command.cmd = Robot::CmdPosition;
    command.target = current_target_;
    command.velocity = MyVector2d(0, 0);
    command.angle = FACE_UP;
    command.observation_type = OBS_EVERYTHING_BUT_ME(me);
    command.goto_point_type = Robot::GotoPointMoveForw;
}

bool TMoveAsStraight::ToStart()
{
    if(start_or_end_)
        return true;
    else
        return false;
}
