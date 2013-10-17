#ifndef ROBOT_H_
#define ROBOT_H_


typedef enum
{
    TEST_TYPE,
    COMPETE_TYPE
}RobotType;


typedef struct
{
    int index;
    int x_velocity;
    int y_velocity;
    float total_velocity;
    int kick;
    int dribble;
    int chipkick;
    int rotate_velocity;
    RobotType robot_type;
}RobotParamters;


RobotParamters ClearRobotParameters(RobotParamters robot_patameters);



class WirelessRobot
{
private:
    RobotParamters robot_parameters_;
public:
    WirelessRobot(RobotParamters robot_paramters);



    int index(void) {return robot_parameters_.index;}
    void set_index(int index) {robot_parameters_.index = index;}

    float x_velocity(void) {return robot_parameters_.x_velocity;}
    void set_x_velocity(float x_velocity){ robot_parameters_.x_velocity = x_velocity;}

    float y_velocity(void) {return robot_parameters_.y_velocity;}
    void set_y_velocity(float y_velocity){ robot_parameters_.y_velocity = y_velocity;}

    float total_velocity(void) {return robot_parameters_.total_velocity;}
    void set_total_velocity(int total_velocity){robot_parameters_.total_velocity=total_velocity; }

    int kick(void) {return robot_parameters_.kick;}
    void set_kick(int kick) {robot_parameters_.kick = kick;}

    int dribble(void) {return robot_parameters_.dribble;}
    void set_dribble(int dribble) {robot_parameters_.dribble = dribble;}

    int chipkick(void){return robot_parameters_.chipkick;}
    void set_chipkick(int chipkick){ robot_parameters_.chipkick = chipkick;}

    float rotate_velocity(void){return robot_parameters_.rotate_velocity;}
    void set_rotate_velocity(float rotate_velocity){robot_parameters_.rotate_velocity=rotate_velocity;}

    RobotType robot_type(void){return robot_parameters_.robot_type;}
    void set_robot_type(RobotType robot_type){robot_parameters_.robot_type= robot_type;}

    void set_robot_parameters(RobotParamters robot_parameters);
    RobotParamters robot_parameters(void){return robot_parameters_;}

    void set_robot_velocity(int index,float x_velocity,float y_velocity,float rotate_velocity);
    void set_robot_actions(int index, int kick,int dribble,int chipkick);



};
#endif
