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

/**    @file    - move_to_middle_tactic.cpp
  *
  *    @class   - TMoveAsSin
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


#include "move_as_sin_tactic.h"

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"



const double FACE_UP = 0.0;
const double FACE_DOWN = -M_PI;
const MyVector2d TMoveAsSin::START_POINT = MyVector2d(1500,1500);
const MyVector2d TMoveAsSin::END_POINT = MyVector2d(1500,-1500);
const double TMoveAsSin::START_TO_END_DISTANCE = (TMoveAsSin::END_POINT -TMoveAsSin::START_POINT).length();


// set variable as PDataMap
CR_DECLARE(SHOOT_AIM_PREF_AMOUNT)
CR_DECLARE(DISTANCE_FROM_PENALTY_LINE)

inline void cr_do_setup()
{
    CR_SETUP(tactic, SHOOT_AIM_PREF_AMOUNT, CR_DOUBLE);
    CR_SETUP(tactic, DISTANCE_FROM_PENALTY_LINE, CR_DOUBLE);
}

TMoveAsSin::TMoveAsSin()
{
}

void TMoveAsSin::LoadConfig()
{
    cr_do_setup();
}

void TMoveAsSin::command(World &world, int me,
                         Robot::RobotCommand &command,
                         bool debug)
{
    SetupSettings();
    double radius_limite = 3;
    MyVector2d robot_postion = world.GetRobotPositionByID(me);
    double target_distance = 0.0;
    if(ToStart())
    {
        target_distance = (robot_postion - current_target_).length();
        if(target_distance < radius_limite )
        {


            if( current_target_ == START_POINT)
            {
                is_at_end_= AT_START_POINT;
                SpecifyMoveSettings();
            }
            current_target_ += x_step_vector_;
        }

    }
    else
    {
        target_distance = (robot_postion - current_target_).length();
        if(target_distance < radius_limite )
        {

            if( current_target_ == END_POINT)
            {
                is_at_end_= AT_END_POINT;
                SpecifyMoveSettings();
            }
            current_target_ += x_step_vector_;


        }
    }


    command.cmd = Robot::CmdPosition;
    command.target = current_target_;
    command.velocity = MyVector2d(0, 0);
    command.angle = FACE_UP;
    command.observation_type = OBS_EVERYTHING_BUT_ME(me);
    command.goto_point_type = Robot::GotoPointMoveForw;
}
inline bool TMoveAsSin::ToStart()
{
    if(is_at_end_)
        return true;
    else
        return false;
}
inline bool TMoveAsSin::ToEnd()
{
    return !ToStart();
}
void TMoveAsSin::SetupSettings()
{
    step_number_ = 4;
    amplitude_ = 300;
    frequency_ = 1;
    point_set_ = new MyVector2d[step_number_];
    current_target_ = START_POINT;
    is_at_end_ = 1;

}
void TMoveAsSin::SpecifyMoveSettings()
{

    if(ToEnd())
    {
        x_step_vector_ = (END_POINT - START_POINT)/step_number_;
    }
    else
    {
        x_step_vector_ = (START_POINT - END_POINT)/step_number_;
    }

}

TMoveAsSin::~TMoveAsSin()
{
    delete[] point_set_;
}
