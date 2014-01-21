#include "field_ball_tracker.h"

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

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "user_interface/field_related/field_constants.h"
#include "field_ball_tracker.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"

// Parameters
static bool cr_setup = false;
CR_DECLARE(BALL_WALLS_SLOPED)
CR_DECLARE(BALL_WALLS_OOB)
CR_DECLARE(BALL_IMPROBABILITY_FILTERING)
CR_DECLARE(BALL_TEAMMATE_COLLISION_RADIUS)
CR_DECLARE(BALL_OPPONENT_COLLISION_RADIUS)

CR_DECLARE(BALL_PRINT_KALMAN_ERROR)
CR_DECLARE(BALL_POSITION_VARIANCE)
CR_DECLARE(BALL_VELOCITY_VARIANCE_NO_ROBOT)
CR_DECLARE(BALL_VELOCITY_VARIANCE_NEAR_ROBOT)
CR_DECLARE(BALL_FRICTION)
CR_DECLARE(BALL_CONFIDENCE_THRESHOLD)
CR_DECLARE(BALL_OCCLUDE_TIME)
CR_DECLARE(BALL_LIKELIHOOD_THRESHOLD)

//
// State = ( x, y, v_x, v_y )
// Observation = ( x, y )
//
// Noise_Propagate = ( v_x, v_y )
// Noise_Observe = ( x, y )
//

BallTracker::BallTracker() : Kalman(4, 2, FRAME_PERIOD)
{
    //printf("BallTracker constructor\r\n");
    _reset = 1;
    occluded_ = Visible;		//球是否被捕捉住，默认是可见的
    tracker = NULL;
}

void BallTracker::LoadConfig()
{
    CR_SETUP(tracker, BALL_WALLS_SLOPED,CR_INT);
    CR_SETUP(tracker, BALL_WALLS_OOB, CR_INT);
    CR_SETUP(tracker, BALL_IMPROBABILITY_FILTERING, CR_INT);
    CR_SETUP(tracker, BALL_TEAMMATE_COLLISION_RADIUS, CR_DOUBLE);
    CR_SETUP(tracker, BALL_OPPONENT_COLLISION_RADIUS, CR_DOUBLE);

    CR_SETUP(tracker, BALL_PRINT_KALMAN_ERROR, CR_INT);
    CR_SETUP(tracker, BALL_POSITION_VARIANCE, CR_DOUBLE);
    CR_SETUP(tracker, BALL_VELOCITY_VARIANCE_NO_ROBOT, CR_DOUBLE);
    CR_SETUP(tracker, BALL_VELOCITY_VARIANCE_NEAR_ROBOT, CR_DOUBLE);
    CR_SETUP(tracker, BALL_FRICTION, CR_DOUBLE);
    CR_SETUP(tracker, BALL_CONFIDENCE_THRESHOLD, CR_DOUBLE);
    CR_SETUP(tracker, BALL_OCCLUDE_TIME, CR_DOUBLE);
    CR_SETUP(tracker, BALL_LIKELIHOOD_THRESHOLD, CR_DOUBLE);

    // Output Kalman Errors
    if (IVAR(BALL_PRINT_KALMAN_ERROR))
    {
        prediction_lookahead = LATENCY_DELAY;
    }
}

//速度方差，根据周围是否有机器人有2档值，平滑过渡
double BallTracker::velocity_variance(const Matrix &x)
{
    if (!tracker) return DVAR(BALL_VELOCITY_VARIANCE_NEAR_ROBOT);
    MyVector2d ball = MyVector2d(x.e(0, 0), x.e(1, 0));
    double dist = 5000.0;
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < MAX_TEAM_ROBOTS; j++)
        {
            if (!tracker->Exists(i, j)) continue;
            double d = (tracker->robots[i][j].position(0.0) - ball).length();
            if (d < dist) dist = d;
        }
    }
    double r = bound((dist - ROBOT_DEF_WIDTH_H) / ROBOT_DEF_WIDTH_H, 0, 1);
    return r * DVAR(BALL_VELOCITY_VARIANCE_NO_ROBOT) +
            (1 - r) * DVAR(BALL_VELOCITY_VARIANCE_NEAR_ROBOT);
}

//检查球是否被机器人吸住
bool BallTracker::check_occlusion()
{
    occluded_=Visible;
    if (!tracker) return false;

    for (int i = 1; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < MAX_TEAM_ROBOTS; j++)
        {
            if (!tracker->Exists(i, j)) continue;
            //球相对机器人坐标
            MyVector2d ball_rel  = position(0.0) - tracker->robots[i][j].position(0.0);
            //球相对机器人矢量旋转到X轴方向
            MyVector2d robot_ball = ball_rel.rotate(-tracker->robots[i][j].direction(0.0));
            double dist = ball_rel.length();
            //计算球相对机器人方向与机器人正方向的角度差
            double da=anglemod(ball_rel.angle()-tracker->robots[i][j].direction(0.0));
            //检查球与机器人是否碰撞
            if (dist > robotHalfSize + BALL_RADIUS-4.0)
            {
                continue;
            }
            if(i==world.color_)	//检查是否属于己方队控球
            {
                if(fabs(da)<M_PI/4)
                {
                    occluded_ = MaybeOccluded;
                    occluding_team = i;
                    occluding_robot = j;
                    occluding_offset = ball_rel;
                }
            }
            else
            {
                occluded_ = MaybeOccluded;
                occluding_team = i;
                occluding_robot = j;
                occluding_offset = ball_rel;
            }
        }
    }
    return (occluded_ != Visible);
}


void BallTracker::tick_occlusion(double dt)
{
    MyVector2d camera(0, 0);

    MyVector2d p = tracker->robots[occluding_team][occluding_robot]
            .position(0.0);
    MyVector2d v = tracker->robots[occluding_team][occluding_robot]
            .velocity(0.0);
    MyVector2d b = occluding_offset;//.rotate((p - camera).angle());

    double bdelta = MAX(v.dot(b.norm()) * dt, 0.0);
    double radius;

    switch(tracker->Type(occluding_team, occluding_robot))
    {
    case ROBOT_TYPE_DIFF:
    case ROBOT_TYPE_OMNI:
        radius = DVAR(BALL_TEAMMATE_COLLISION_RADIUS); break;
    default:
        radius = DVAR(BALL_OPPONENT_COLLISION_RADIUS); break;
    }

    if (b.length() - bdelta < radius)
    {
        b = b.norm(radius);
    }
    else
    {
        b = b.norm(b.length() - bdelta);
    }

    //occluding_offset = b.rotate(-(p - camera).angle());

    // Update the x and P queue.
    Matrix x(4,1), P(4);
    MyVector2d xp = occluded_position(dt);
    MyVector2d xv = occluded_velocity(dt);

    x.e(0,0) = xp.x;
    x.e(1,0) = xp.y;
    x.e(2,0) = xv.x;
    x.e(3,0) = xv.y;

    P.e(0,0) *= DVAR(BALL_POSITION_VARIANCE);
    P.e(1,1) *= DVAR(BALL_POSITION_VARIANCE);
    P.e(2,2) *= 250000.0; // 500m/s
    P.e(3,3) *= 250000.0; // 500m/s

    predict_count_=1;
    xs[0]=x;
    Ps[0]=P;
    time += dt;
}

//计算被机器人控球后的位置
MyVector2d BallTracker::occluded_position(double time)
{
    if (!tracker)
    {
        return MyVector2d(0.0, 0.0);
    }

    MyVector2d camera(0, 0);
    MyVector2d b;

    b = tracker->robots[occluding_team][occluding_robot].position(time);
    b += occluding_offset;//.rotate((b - camera).angle());

    return b;
}

//计算被机器人控球后的速度
MyVector2d BallTracker::occluded_velocity(double time)
{
    if (!tracker)
    {
        return MyVector2d(0.0, 0.0);
    }
    return tracker->robots[occluding_team][occluding_robot].velocity(time);
}

//根据观察结果更新卡夫曼滤波
void BallTracker::observe(VisionRawInfo obs, double timestamp)
{
    //如果有1秒没有来更新信号，自动复位，方式繁殖的程序内存不够
    if(timestamp-time>0.5)
    {
        printf("Ball obvserve reset\r\n");
        _reset = true;
    }

    // mhb: Need this?
    if (isnan(xs[0].e(0,0))) _reset = true;
    if (_reset && obs.timestamp >= timestamp &&
            obs.conf >= DVAR(BALL_CONFIDENCE_THRESHOLD))
    {
        Matrix x(4,1), P(4);

        x.e(0,0) = obs.pos.x;
        x.e(1,0) = obs.pos.y;
        x.e(2,0) = 0.0;
        x.e(3,0) = 0.0;

        //# Observation noise in position.
        //BALL_POSITION_VARIANCE = 4.0 # Stddev = 2mm
        P.e(0,0) *= DVAR(BALL_POSITION_VARIANCE);
        P.e(1,1) *= DVAR(BALL_POSITION_VARIANCE);
        P.e(2,2) *= 250000.0; // 500m/s
        P.e(3,3) *= 250000.0; // 500m/s

        initial(obs.timestamp, x, P);

        //occluded = Visible;

        _reset = false;

    }
    else
    {
        if (_reset && occluded_ != Occluded) return;

        // If this is a new observation.
        if (timestamp > time)
        {
            // Tick to current time.
            if (occluded_ == Occluded)
            {
                printf("occlude!\r\n");
                tick_occlusion(timestamp - time);
            }
            else
            {
                if (occluded_ == MaybeOccluded)
                {
                    printf("MaybeOccluded!\r\n");
                }
                else
                {
                    //printf("Visible!\r\n");
                }
                tick(timestamp - time);
            }

            // Make Observation Matrix
            Matrix o(2,1);
            o.e(0,0) = obs.pos.x;
            o.e(1,0) = obs.pos.y;

            //# Improbability filtering (if true, see BALL_LIKELIHOOD_THRESHOLD below)
            //BALL_IMPROBABILITY_FILTERING = 0 # false
            //检查观察结果的可信度，如果小于设定阀值，将忽略这个观察
            if (IVAR(BALL_IMPROBABILITY_FILTERING))
            {
                // Check for improbable observations (i.e. noise)
                if (obs_likelihood(0.0, o) <= DVAR(BALL_LIKELIHOOD_THRESHOLD))
                    obs.conf = -1.0;
            }

            if(occluded_==Visible)
            {
                occluded_last_obs_time = obs.timestamp;
            }
            OccludeFlag oldOccluded=occluded_;
            check_occlusion();
            if(occluded_ == MaybeOccluded)
            {
                if(obs.conf > DVAR(BALL_CONFIDENCE_THRESHOLD))
                {
                    printf("---------------------\r\n");
                    if(oldOccluded == MaybeOccluded)
                    {
                        if(timestamp - occluded_last_obs_time > DVAR(BALL_OCCLUDE_TIME))
                        {
                            occluded_ = Occluded;
                            _reset = true;
                        }
                    }
                }
                else
                {
                    occluded_=oldOccluded;
                }
            }
            else
            {
                // if it is visible, it is still here
                //printf("ball_tracker\r\n");
                if(oldOccluded == Occluded)
                {
                    occluded_ = Occluded;
                }
            }
            update(o);

            // Make observation
            if (error_time_elapsed() > 10.0) {
                fprintf(stderr, "Kalman Error (pos, vpos): ");
                fprintf(stderr, "%f ",
                        hypot(error_mean().e(0, 0), error_mean().e(1, 0)));
                fprintf(stderr, "%f\n",
                        hypot(error_mean().e(2, 0), error_mean().e(3, 0)));
                error_reset();
            }
        }
    }
}

//复位球的参数
void BallTracker::reset(double timestamp,MyVector2d pos)
{
    Matrix x(4,1), P(4);
    x.e(0,0) = pos.x;
    x.e(1,0) = pos.y;
    x.e(2,0) = 0.0;
    x.e(3,0) = 0.0;

    //# Observation noise in position.
    //BALL_POSITION_VARIANCE = 4.0 # Stddev = 2mm
    P.e(0,0) *= DVAR(BALL_POSITION_VARIANCE);
    P.e(1,1) *= DVAR(BALL_POSITION_VARIANCE);
    P.e(2,2) *= 250000.0; // 500m/s
    P.e(3,3) *= 250000.0; // 500m/s

    initial(timestamp, x, P);

    occluded_ = Visible;
    _reset = false;
}

//复位球的参数
void BallTracker::reset(double timestamp, float state[4], float variances[16],
                        OccludeFlag _occluded,
                        char _occluding_team, char _occluding_robot,
                        vector2f _occluding_offset)
{
    //state=x,y,vx,vy
    //
    Matrix x(4,1,state), P(4,4,variances);
    initial(timestamp, x, P);
    occluded_ = _occluded;
    occluding_team = _occluding_team;
    occluding_robot = _occluding_robot;
    occluding_offset = MyVector2d(_occluding_offset.x, _occluding_offset.y);

    _reset = false;
}

//返回球的预测位置
MyVector2d BallTracker::position(double time)
{
    if (occluded_ == Occluded) return occluded_position(time);

    Matrix x = predict(time);
    //qDebug()<<"ball position"<<x.e(0,0)<<x.e(1,0);
    return MyVector2d(x.e(0,0), x.e(1,0));

}

//返回球的预测速度
MyVector2d BallTracker::velocity(double time)
{
    if (occluded_ == Occluded) return occluded_velocity(time);

    Matrix x = predict(time);
    return MyVector2d(x.e(2,0), x.e(3,0));
}

//协变的 covariances
Matrix BallTracker::covariances(double time)
{
    return predict_cov(time);
}

//collision 碰撞, 冲突
//检查发生
bool BallTracker::collision(double time, int &team, int &robot)
{
    Matrix I = predict_info(time);

    if (I.nrows() <= 1) return false;

    team = (int) rint(I.e(1, 0));
    robot = (int) rint(I.e(1, 0));

    return true;
}

#ifndef MIN
#define MIN(a,b) ((a<b) ? a : b)
#endif

//x为原始位置和速度 I返回计算后的信息
//返回为新的速度和加速度
//考虑了摩擦系数，和机器人碰撞
//Matrix x: [0,0]=x [0,1]=y [0,2]=vx [0,3]=vy
Matrix& BallTracker::f(const Matrix &x, Matrix &I)
{
    I = Matrix();

    static Matrix f;

    f = x; // Copy Matrix
    double &_x = f.e(0,0), &_y = f.e(1,0), &_vx = f.e(2,0), &_vy = f.e(3,0);
    //合成矢量速度
    double _v = sqrt(_vx * _vx + _vy * _vy);

    //BALL_FRICTION 球的摩擦系数
    //# This is (Friction_Coef / Ball_Mass), it's the ratio, A_friction / A_normal.
    //# Measured empirically last year.
    //BALL_FRICTION = 0.025
    //根据摩擦系数计算的加速度
    double _a = MIN(DVAR(BALL_FRICTION) * GRAVITY, _v / stepsize_);
    double _ax = (_v == 0.0) ? 0.0 : -_a * _vx / _v;
    double _ay = (_v == 0.0) ? 0.0 : -_a * _vy / _v;

    double walls = false;

    //# Sloped Walls
    //BALL_WALLS_SLOPED = 1 # true
    //
    if (IVAR(BALL_WALLS_SLOPED)) {
        if (fabs(_x) > FIELD_LENGTH_H && fabs(_y) > GOAL_WIDTH_H) {
            _ax += copysign(M_SQRT1_2 * GRAVITY * 5.0 / 7.0, -_x);
            walls = true;
        }

        if (fabs(_y) > FIELD_WIDTH_H) {
            _ay = copysign(M_SQRT1_2 * GRAVITY * 5.0 / 7.0, -_y);
            walls = true;
        }
    }

    //如果球超过墙边界,需排除从球门出去的情况
    //# If true ball can go out of bounds.  This is handled by "stopping" the ball
    //# at the position where it went out-of-bounds.
    //BALL_WALLS_OOB = 1 # true
    if (IVAR(BALL_WALLS_OOB))
    {
        if ((fabs(_x) > FIELD_LENGTH_H + WALL_WIDTH &&
             fabs(_y) > GOAL_WIDTH_H) ||
                (fabs(_y) > FIELD_WIDTH_H + WALL_WIDTH))
        {
            _vx = 0.0;
            _vy = 0.0;
            _ax = 0.0;
            _ay = 0.0;

            walls = true;
        }
    }

    // Update Position
    _x += _vx * stepsize_ + 0.5 * _ax * stepsize_ * stepsize_;
    _y += _vy * stepsize_ + 0.5 * _ay * stepsize_ * stepsize_;

    //如果球与一个对象冲撞，将球的速度设定为被撞物体速度
    // If there's a collision... then set ball's velocity to the colliding
    //  object's velocity.
    MyVector2d cv, cp;
    int team, robot;

    if (!walls && check_for_collision(f, cp, cv, team, robot))
    {
        _vx = cv.x; _vy = cv.y;
        I = Matrix(2, 1);
        I.e(0, 0) = team;
        I.e(1, 0) = robot;
    }
    else
    {
        //如果没有冲撞，按照加速度计算新的速度
        _vx += _ax * stepsize_;
        _vy += _ay * stepsize_;
    }
    return f;
}

//寻找与球发生冲撞的机器人
bool BallTracker::check_for_collision(const Matrix &x,
                                      MyVector2d &cp, MyVector2d &cv,
                                      int &team, int &robot)
{
    if (!tracker) return false;

    //获得球的位置和速度
    MyVector2d bp = MyVector2d(x.e(0,0), x.e(1,0));
    MyVector2d bv = MyVector2d(x.e(2,0), x.e(3,0));

    double dist = 5000.0;
    bool rv = false;

    for(int i = 0; i < NUM_TEAMS; i++)
    {
        for(int j = 0; j < MAX_TEAM_ROBOTS; j++)
        {
            if (!tracker->Exists(i, j)) continue;

            double radius;

            //获得机器人半径
            switch(tracker->Type(i, j))
            {
            case ROBOT_TYPE_DIFF:
            case ROBOT_TYPE_OMNI:
                radius = DVAR(BALL_TEAMMATE_COLLISION_RADIUS); break;
            default:
                radius = DVAR(BALL_OPPONENT_COLLISION_RADIUS); break;
            }

            if (radius <= 0) continue;

            //获得当前机器人的位置和速度
            MyVector2d p = tracker->robots[i][j].position(stepped_time - time);
            MyVector2d v = tracker->robots[i][j].velocity(stepped_time - time);

            //计算经过stepsize时间运动后，两者距离
            double d = MIN((p - bp).length(),
                           (p + v * stepsize_ - bp + bv * stepsize_).length());

            // Ball is within radius, nothing else is closest, and ball is
            //  moving towards the robot...  Count as collision.
            //如果球在机器人半径内，并且正在朝机器人运动，按照冲突处理
            if (d <= radius && d < dist && (bv - v).dot(p - bp) > 0.0)
            {
                cp = p + (bp - p).norm(radius);
                cv = v; rv = true; dist = d;
                team = i; robot = j;
            }
        }
    }
    return rv;
}

//
Matrix& BallTracker::h(const Matrix &x)
{
    static Matrix h(2,1);
    h.e(0,0) = x.e(0,0);
    h.e(1,0) = x.e(1,0);
    return h;
}

Matrix& BallTracker::Q(const Matrix &x)
{
    static Matrix Q(2);

    // Base noise covariances on distance to nearest robot.
    Q.e(0, 0) = Q.e(1, 1) = velocity_variance(x);

    return Q;
}

Matrix& BallTracker::R(const Matrix &x)
{
    static Matrix R;

    //# Observation noise in position.
    //BALL_POSITION_VARIANCE = 4.0 # Stddev = 2mm
    if (!R.nrows()) {
        R.identity(2);
        R.scale(DVAR(BALL_POSITION_VARIANCE));
    }
    return R;
}

Matrix& BallTracker::A(const Matrix &x)
{
    static Matrix A;

    // This is not quite right since this doesn't account for friction
    // But the Jacobian with friction is pretty messy.

    if (!A.nrows()) {
        A.identity(4);
        A.e(0,2) = stepsize_;
        A.e(1,3) = stepsize_;
    }
    return A;
}

Matrix& BallTracker::W(const Matrix &x)
{
    static Matrix W("[0, 0; 0, 0; 1, 0; 0, 1]");
    return W;
}

Matrix& BallTracker::H(const Matrix &x)
{
    static Matrix H("[ 1, 0, 0, 0; "
                    "  0, 1, 0, 0 ] ");
    return H;
}

Matrix& BallTracker::V(const Matrix &x)
{
    static Matrix V("[ 1, 0; "
                    "  0, 1 ]");

    return V;
}


