#include "wireless_robot.h"

RobotParamters ClearRobotParameters(RobotParamters robot_patameters)
{
    robot_patameters.chipkick = 0;
    robot_patameters.dribble = 0;
    robot_patameters.index = 0;
    robot_patameters.rotate_velocity = 0;
    robot_patameters.y_velocity=0;
    robot_patameters.x_velocity = 0;
    robot_patameters.robot_type = TEST_TYPE;
    robot_patameters.kick = 0;
    robot_patameters.total_velocity = 0;
    return robot_patameters;
}


WirelessRobot::WirelessRobot(RobotParamters robot_parameters)
{
    robot_parameters_=robot_parameters;
}

void WirelessRobot::set_robot_velocity(int index, float x_velocity, float y_velocity, float rotate_velocity)
{
    robot_parameters_.index = index;
    x_velocity = (x_velocity > 255)?255:((x_velocity < -255)?-255:x_velocity);
    robot_parameters_.x_velocity = x_velocity;
    y_velocity = (y_velocity > 255)?255:((y_velocity < -255)?-255:y_velocity);
    robot_parameters_.y_velocity = y_velocity;
    rotate_velocity = (rotate_velocity > 255)?255:((rotate_velocity < -255)?-255:rotate_velocity);
    robot_parameters_.rotate_velocity = rotate_velocity;
}

void WirelessRobot::set_robot_actions(int index, int kick, int dribble, int chipkick)
{
    robot_parameters_.index = index;
    robot_parameters_.kick = kick;
    robot_parameters_.dribble = dribble;
    robot_parameters_.chipkick = chipkick;
}

