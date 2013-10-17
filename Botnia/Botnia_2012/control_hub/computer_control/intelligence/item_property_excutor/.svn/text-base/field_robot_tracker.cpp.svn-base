#include "field_robot_tracker.h"


//#include <stdio.h>
#include <math.h>

#include <QDebug>

#include "user_interface/field_related/field_constants.h"
#include "field_robot_tracker.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"

#pragma warning(disable:4100)

static bool cr_setup = false;
CR_DECLARE(ROBOT_PRINT_KALMAN_ERROR)
CR_DECLARE(ROBOT_FAST_PREDICT)
CR_DECLARE(ROBOT_USE_AVERAGES_IN_PROPAGATION)
CR_DECLARE(ROBOT_CONFIDENCE_THRESHOLD)
CR_DECLARE(ROBOT_POSITION_VARIANCE)
CR_DECLARE(ROBOT_THETA_VARIANCE)
CR_DECLARE(ROBOT_NONE_VELOCITY_VARIANCE)
CR_DECLARE(ROBOT_NONE_ANGVEL_VARIANCE)
CR_DECLARE(ROBOT_DIFF_VELOCITY_VARIANCE)
CR_DECLARE(ROBOT_DIFF_VELOCITY_VARIANCE_PERP)
CR_DECLARE(ROBOT_DIFF_ANGVEL_VARIANCE)
CR_DECLARE(ROBOT_OMNI_VELOCITY_VARIANCE)
CR_DECLARE(ROBOT_OMNI_ANGVEL_VARIANCE)
CR_DECLARE(ROBOT_STUCK_VARIANCE)
CR_DECLARE(ROBOT_VELOCITY_NEXT_STEP_COVARIANCE)
CR_DECLARE(ROBOT_STUCK_DECAY)
CR_DECLARE(ROBOT_STUCK_THRESHOLD)

//
// State = ( x, y, theta, v_par, v_perp, v_theta, stuck )
// Observation = ( x, y, theta )
//
// Noise_Propagate = ( v_par, v_perp, v_theta, stuck )
// Noise_Update = ( x, y, theta )
//

RobotTracker::RobotTracker(int _type, double _latency)
    : Kalman(7, 3, FRAME_PERIOD)
{
    type = _type;
    latency = _latency;
    reset_on_observation = 1;
}

void RobotTracker::LoadConfig()
{
    CR_SETUP(tracker, ROBOT_PRINT_KALMAN_ERROR, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_FAST_PREDICT, CR_INT);
    CR_SETUP(tracker, ROBOT_USE_AVERAGES_IN_PROPAGATION, CR_INT);
    CR_SETUP(tracker, ROBOT_CONFIDENCE_THRESHOLD, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_POSITION_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_THETA_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_NONE_VELOCITY_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_NONE_ANGVEL_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_DIFF_VELOCITY_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_DIFF_VELOCITY_VARIANCE_PERP, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_DIFF_ANGVEL_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_OMNI_VELOCITY_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_OMNI_ANGVEL_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_STUCK_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_VELOCITY_NEXT_STEP_COVARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_STUCK_DECAY, CR_DOUBLE);
    CR_SETUP(tracker, ROBOT_STUCK_THRESHOLD, CR_DOUBLE);

    //# Output Kalman Errors
    //ROBOT_PRINT_KALMAN_ERROR = 0 # false
    if (IVAR(ROBOT_PRINT_KALMAN_ERROR))
    {
        prediction_lookahead = LATENCY_DELAY;
    }
}

//获取指定时间的命令，如果超前，返回停止状态
RobotTracker::rcommand RobotTracker::get_command(double time)
{
    rcommand c={ 0.0, MyVector3d(0.0, 0.0, 0.0) };
    if (cs.empty() || cs[0].timestamp > time)
    {
        return c;
    }

    uint i;

    for(i = 1; i < cs.size(); i++)
    {
        if (cs[i].timestamp > time) break;
    }
    return cs[i-1];
}

//压入命令
void RobotTracker::command(double timestamp, MyVector3d vs)
{
    rcommand c = { timestamp + latency - (FRAME_PERIOD / 2.0), vs };

    //清除旧的command
    while(cs.size() > 1 && cs[0].timestamp < time - stepsize_)
    {
        cs.pop_front();
    }
    //如果有和timestamp相等时间的命令则清除
    while(!cs.empty() && cs.back().timestamp == c.timestamp)
    {
        cs.pop_back();
    }
    cs.push_back(c);
}

// observe the raw infomation, but just like human ut will use some method to filter the noice
// this is just for one robot
void RobotTracker::observe(VisionRawInfo obs, double timestamp)
{
    //如果有1秒没有来更新信号，自动复位，防止繁殖的程序内存不够
    if(timestamp-time>0.5)
    {
        printf("Robot obvserve reset\r\n");
        reset_on_observation = true;
    }

    // if there is no renew signal
    // if the observation has been reset
    if (reset_on_observation)
    {
        if (obs.conf <= 0.0)
        {
            return;
        }

        static Matrix observe_matrix(7,1), P(7);

        observe_matrix.e(0,0) = obs.pos.x;
        observe_matrix.e(1,0) = obs.pos.y;
        observe_matrix.e(2,0) = obs.angle;
        observe_matrix.e(3,0) = 0.0;
        observe_matrix.e(4,0) = 0.0;
        observe_matrix.e(5,0) = 0.0;
        observe_matrix.e(6,0) = 0.0;

        P.e(0,0) = DVAR(ROBOT_POSITION_VARIANCE);
        P.e(1,1) = DVAR(ROBOT_POSITION_VARIANCE);
        P.e(2,2) = DVAR(ROBOT_THETA_VARIANCE);

        P.e(3,3) = 0.0; // 0m/s
        if (type == ROBOT_TYPE_DIFF)
        {
            P.e(4,4) = 0.0;
        }
        else
        {
            P.e(4,4) = 0.0; // 0m/s
        }
        P.e(5,5) = 0.0;

        P.e(6,6) = 0.0;

        initial(obs.timestamp, observe_matrix, P);

        reset_on_observation = false;

    }
    else
    {
        // If this is a new observation.
        if (timestamp > time)
        {
            // Tick to current time.
            tick(timestamp - time);

            // Make observation
            if (obs.timestamp == timestamp)
            {
                double xtheta = xs[0].e(2,0);

                Matrix o(3,1);
                o.e(0,0) = obs.pos.x;
                o.e(1,0) = obs.pos.y;
                o.e(2,0) = anglemod(obs.angle - xtheta) + xtheta;

                update(o);
            }

            if (error_time_elapsed() > 10.0)
            {
                fprintf(stderr, "Kalman Error (pos, theta, vpos, vtheta): ");
                fprintf(stderr, "%f ",
                        hypot(error_mean().e(0, 0), error_mean().e(1, 0)));
                fprintf(stderr, "%f ", error_mean().e(2, 0));
                fprintf(stderr, "%f ",
                        hypot(error_mean().e(3, 0), error_mean().e(4, 0)));
                fprintf(stderr, "%f\n", error_mean().e(5, 0));
                error_reset();
            }
        }
    }
}

//用指定的时间和参数复位滤波器
void RobotTracker::reset(double timestamp, float state[7])
{
    Matrix x(7,1,state), P(7);
    //XY值
    //ROBOT_POSITION_VARIANCE = 4.0 # Stddev = 2mm
    P.e(0,0) = DVAR(ROBOT_POSITION_VARIANCE);
    P.e(1,1) = DVAR(ROBOT_POSITION_VARIANCE);
    //ROBOT_THETA_VARIANCE = 0.00006 # Stddev = 0.5 degree
    P.e(2,2) = DVAR(ROBOT_THETA_VARIANCE);

    //速度矢量
    P.e(3,3) = 0.0; // 0m/s
    if (type == ROBOT_TYPE_DIFF)
    {
        P.e(4,4) = 0.0;
    }
    else
    {
        P.e(4,4) = 0.0; // 0m/s
    }
    //角速度
    P.e(5,5) = 0.0;

    //机器人被困住相关的参数
    P.e(6,6) = 0.0;

    initial(timestamp, x, P);

    reset_on_observation = false;
}

//返回预测的位置
// gao mark
MyVector2d RobotTracker::position(double time)
{
    //# Use a closed form prediction based on current velocity for position.
    //ROBOT_FAST_PREDICT = 1 # true
    if (IVAR(ROBOT_FAST_PREDICT))
    {
        if (time > latency)
        {
            //起始时间大于延迟，向后推延迟长度
            Matrix x = predict(latency);
            return MyVector2d(x.e(0,0), x.e(1,0)) +
                    MyVector2d(x.e(3,0), x.e(4,0)) * (time - latency);
        }
        else
        {
            //起始时间比延迟大，则直接取起点参数
            Matrix x = predict(time);

            return MyVector2d(x.e(0,0), x.e(1,0));
        }
    }
    else
    {
        //返回起点参数
        Matrix x = predict(time);
        return MyVector2d(x.e(0,0), x.e(1,0));
    }
}

//返回预测的速度
//速度以场地坐标系为基准
MyVector2d RobotTracker::velocity(double time)
{
    Matrix x;

    if (IVAR(ROBOT_FAST_PREDICT))
    {
        if (time > latency)
        {
            x = predict(latency);
        }
        else
        {
            x = predict(time);
        }
    }
    else
    {
        x = predict(time);
    }

    double a = x.e(2,0);
    double c = cos(a);
    double s = sin(a);

    double vx = x.e(3,0);
    double vy = x.e(4,0);

    double stuck = x.e(6,0);

    // Threshold after which the robot is considered stuck and zero's velocity.
    //ROBOT_STUCK_THRESHOLD = 0.6 # Set to 1.1 to turn off.
    if (stuck > DVAR(ROBOT_STUCK_THRESHOLD))
    {
        return MyVector2d(0.0, 0.0);
    }
    //fprintf(stderr, "=============>\nx =\n");
    //x.print();
    //fprintf(stderr, "---->\nx =\n");

    MyVector2d v0;
    v0.set(c * vx - s * vy, s * vx + c * vy);
    //旋转角度为负
    return v0;//MyVector2d(c * vx - s * vy, s * vx + c * vy);

}

// return the velocity un-rotate
//以机器人中心为坐标原点,机器人基准坐标系
MyVector2d RobotTracker::velocity_raw(double time)
{
    Matrix x;

    if (IVAR(ROBOT_FAST_PREDICT))
    {
        if (time > latency)
        {
            x = predict(latency);
        }
        else
        {
            x = predict(time);
        }
    }
    else
    {
        x = predict(time);
    }
    double vx = x.e(3,0);
    double vy = x.e(4,0);

    double stuck = x.e(6,0);

    if (stuck > 0.8) return MyVector2d(0.0, 0.0);

    return MyVector2d(vx, vy);
}

//返回预测的方向
double RobotTracker::direction(double time)
{
    Matrix x = predict(time);
    return x.e(2,0);
}

//返回预测的角速度
double RobotTracker::angular_velocity(double time)
{
    Matrix x = predict(time);
    return x.e(5,0);
}

//
double RobotTracker::stuck(double time)
{
    Matrix x = predict(time);
    return bound(x.e(6,0), 0, 1);
}

//Matrix x: [0,0]=x [0,1]=y [0,2]=angle [0,3]=vx [0,4]=vy [0,5]=vangle [0,6]
//计算下一步的x,y,角度
Matrix& RobotTracker::f(const Matrix &x, Matrix &I)
{
    I = Matrix();

    static Matrix f;
    f = x;

    rcommand c = get_command(stepped_time);

    double
            &_x = f.e(0,0),
            &_y = f.e(1,0),
            &_theta = f.e(2,0),
            &_vpar = f.e(3,0),
            &_vperp = f.e(4,0),
            &_vtheta = f.e(5,0),
            &_stuck = f.e(6,0);

    _stuck = bound(_stuck, 0, 1) * DVAR(ROBOT_STUCK_DECAY);

    double avg_vpar = 0, avg_vperp = 0, avg_vtheta = 0, avg_theta = 0;
    double avg_weight = 0.5;

    //# Maybe an improved method for propagating.
    //ROBOT_USE_AVERAGES_IN_PROPAGATION = 0 # false
    if (IVAR(ROBOT_USE_AVERAGES_IN_PROPAGATION)) {
        avg_vpar = avg_weight * _vpar;
        avg_vperp = avg_weight * _vperp;
        avg_vtheta = avg_weight * _vtheta;
    }

    if (type != ROBOT_TYPE_NONE)
    {
        _vpar = c.vs.x;
        _vperp = c.vs.y;
        _vtheta = c.vs.z;
    }

    //# Maybe an improved method for propagating.
    //ROBOT_USE_AVERAGES_IN_PROPAGATION = 0 # false
    if (IVAR(ROBOT_USE_AVERAGES_IN_PROPAGATION))
    {
        avg_vpar += (1.0 - avg_weight) * _vpar;
        avg_vperp += (1.0 - avg_weight) * _vperp;
        avg_vtheta += (1.0 - avg_weight) * _vtheta;

        avg_theta = avg_weight * _theta;

        _theta += (1.0 - _stuck) * stepsize_ * avg_vtheta;
        avg_theta += (1.0 - avg_weight) * _theta;

        _x += (1.0 - _stuck) * stepsize_ *
                (avg_vpar * cos(avg_theta) + avg_vperp * -sin(avg_theta));
        _y += (1.0 - _stuck) * stepsize_ *
                (avg_vpar * sin(avg_theta) + avg_vperp * cos(avg_theta));
    }
    else
    {
        //根据速度更新位置
        _theta += (1.0 - _stuck) * stepsize_ * _vtheta;
        _x += (1.0 - _stuck) * stepsize_ *
                (_vpar * cos(_theta) + _vperp * -sin(_theta));
        _y += (1.0 - _stuck) * stepsize_ *
                (_vpar * sin(_theta) + _vperp * cos(_theta));
    }

    _theta = anglemod(_theta);

    return f;
}

Matrix& RobotTracker::h(const Matrix &x)
{
    static Matrix h(3,1);

    h.e(0,0) = x.e(0,0);
    h.e(1,0) = x.e(1,0);
    h.e(2,0) = x.e(2,0);

    return h;
}

Matrix& RobotTracker::Q(const Matrix &x)
{
    static Matrix Q(4);

    switch (type) {
    case ROBOT_TYPE_DIFF:
        Q.e(0,0) = DVAR(ROBOT_DIFF_VELOCITY_VARIANCE);
        Q.e(1,1) = DVAR(ROBOT_DIFF_VELOCITY_VARIANCE_PERP);
        Q.e(2,2) = DVAR(ROBOT_DIFF_ANGVEL_VARIANCE);
        Q.e(3,3) = DVAR(ROBOT_STUCK_VARIANCE);
        break;

    case ROBOT_TYPE_OMNI:
        Q.e(0,0) = DVAR(ROBOT_OMNI_VELOCITY_VARIANCE);
        Q.e(1,1) = DVAR(ROBOT_OMNI_VELOCITY_VARIANCE);
        Q.e(2,2) = DVAR(ROBOT_OMNI_ANGVEL_VARIANCE);
        Q.e(3,3) = DVAR(ROBOT_STUCK_VARIANCE);
        break;

    case ROBOT_TYPE_NONE:
        Q.e(0,0) = DVAR(ROBOT_NONE_VELOCITY_VARIANCE);
        Q.e(1,1) = DVAR(ROBOT_NONE_VELOCITY_VARIANCE);
        Q.e(2,2) = DVAR(ROBOT_NONE_ANGVEL_VARIANCE);
        Q.e(3,3) = 0.0;
        break;
    }

    return Q;
}

Matrix& RobotTracker::R(const Matrix &x)
{
    static Matrix R;

    if (!R.nrows())
    {
        R.identity(3);
        R.e(0,0) = (DVAR(ROBOT_POSITION_VARIANCE));
        R.e(1,1) = (DVAR(ROBOT_POSITION_VARIANCE));
        R.e(2,2) = (DVAR(ROBOT_THETA_VARIANCE));
    }

    return R;
}

Matrix& RobotTracker::A(const Matrix &x)
{
    static Matrix A(7);

    double theta = x.e(2,0);

    double vpar = x.e(3,0), vperp = x.e(4,0), vtheta = x.e(5,0);
    double stuck = x.e(6,0);
    double cos_theta = cos(theta), sin_theta = sin(theta);

    A.e(0,2) = (1.0 - stuck) * stepsize_ *
            (vpar * -sin_theta + vperp * -cos_theta);

    A.e(0,3) = cos_theta * (1.0 - stuck) * stepsize_;
    A.e(0,4) = -sin_theta * (1.0 - stuck) * stepsize_;
    A.e(0,6) = -stepsize_ * (vpar * cos_theta + vperp * -sin_theta);

    A.e(1,2) = (1.0 - stuck) * stepsize_ *
            (vpar * cos_theta + vperp * -sin_theta);
    A.e(1,3) = sin_theta * (1.0 - stuck) * stepsize_;
    A.e(1,4) = cos_theta * (1.0 - stuck) * stepsize_;
    A.e(1,6) = -stepsize_ * (vpar * sin_theta + vperp * cos_theta);

    A.e(2,5) = (1.0 - stuck) * stepsize_;
    A.e(2,6) = -stepsize_ * vtheta;

    A.e(3,3) = DVAR(ROBOT_VELOCITY_NEXT_STEP_COVARIANCE);

    A.e(4,4) = DVAR(ROBOT_VELOCITY_NEXT_STEP_COVARIANCE);

    A.e(5,5) = DVAR(ROBOT_VELOCITY_NEXT_STEP_COVARIANCE);

    A.e(6,6) = DVAR(ROBOT_STUCK_DECAY);

    return A;
}

Matrix& RobotTracker::W(const Matrix &x)
{
    static Matrix W("[ 0, 0, 0, 0; "
                    "  0, 0, 0, 0; "
                    "  0, 0, 0, 0; "
                    "  1, 0, 0, 0; "
                    "  0, 1, 0, 0; "
                    "  0, 0, 1, 0; "
                    "  0, 0, 0, 1 ]");
    return W;
}

Matrix& RobotTracker::H(const Matrix &x)
{
    static Matrix H("[ 1, 0,  0,  0, 0, 0, 0; "
                    "  0, 1,  0,  0, 0, 0, 0; "
                    "  0, 0,  1,  0, 0, 0, 0 ]");
    return H;
}

Matrix& RobotTracker::V(const Matrix &x)
{
    static Matrix V("[ 1, 0, 0; "
                    "  0, 1, 0; "
                    "  0, 0, 1 ]");
    return V;
}
