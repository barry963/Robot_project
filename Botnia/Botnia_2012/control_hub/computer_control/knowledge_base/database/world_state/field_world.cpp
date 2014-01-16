//是视觉数据的接口函数，为决策系统提供了场地上详细的比赛
//状态信息，包括当前时刻球所在的位置、运动速度，每一个机器人的确切位置、
//方向、速度及其与球的相对位置关系，进攻状态还是防守状态等等。

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

#include <stdio.h>

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "user_interface/field_related/field_constants.h"
#include "user_interface/field_related/field_global_function.h"
#include "field_world.h"
#include "field_world_robot.h"  //robot info in the world
#include "user_interface/field_related/field_base_constants.h"
#include "net/net_radio_send_thread.h"
#include "net/net_refbox_receive_thread.h"
#include "net/message_serilization/radio_client_out_files/messages_robocup_ssl_cmd.pb.h"
#include "net/message_serilization/radio_client_out_files/messages_robocup_ssl_radio.pb.h"
//#include "wireless/wireless_refbox.h"

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor.h"

// strategy thread
#include "control_hub/computer_control/intelligence/strategy_extutor/strategy_thread.h"

#include <math.h>

/********** robot return info mux**************************/
QMutex robot_return_info_mutex_;
/*************************************************/

CR_DECLARE(DZONE_HYSTERESIS_DURATION)
CR_DECLARE(POSSESSION_US_HYSTERESIS_DURATION)
CR_DECLARE(POSSESSION_THEM_HYSTERESIS_DURATION)

// World Model
World world;

REFBOX_CMD RefboxCmd0;

//是否显示tactics调试信息
//whether to display tactics debug info
bool bDebugTactic=true;//lu_test, from false -> true

//是否显示轨迹相关调试信息
//whether display path debug info
bool bDebugPathPlan=true;//lu_test, from false -> true

//调试射门
//debug for shooting
bool bDebugTShoot=false;
#ifdef PORT_TEST
CMyComm	m_SerialPort;
#endif


World::World()
{
    serial_sever_ = new SerialServer();
}

World::~World()
{
    if(serial_sever_)
    {
        delete serial_sever_;
        serial_sever_ =NULL;
    }
#ifdef PORT_TEST
    if (m_SerialPort.IsOpen())
    {
        m_SerialPort.Close();
    }
#endif
}
void World::LoadConfig()
{

    object_tracker_.LoadConfig();
    CR_SETUP(strategy, DZONE_HYSTERESIS_DURATION, CR_DOUBLE);
    CR_SETUP(strategy, POSSESSION_US_HYSTERESIS_DURATION, CR_DOUBLE);
    CR_SETUP(strategy, POSSESSION_THEM_HYSTERESIS_DURATION, CR_DOUBLE);
}

void World::setside(int side)
{
    side_ = side;
}

void World::setcolor(int color)
{
    color_ = color;
}

//初始化场地和颜色目标的基本数据、各种状态标志、原始角色分配结果等:
void World::init(int _side, int _color)
{
    int i;

    LoadConfig();
#if PORT_TEST
    int m_iComPort=1;
    if (!m_SerialPort.IsOpen())
    {
        if (!m_SerialPort.Open(m_iComPort,38400))
        {
            m_SerialPort.Close();
            printf("Serial Open Failed!\r\n");
        }
    }
    if (m_SerialPort.IsOpen())
    {
        printf("Serial Open OK!\r\n");
    }
#endif
    side_ = _side;
    color_ = _color;
    // Basic Field Vectors
    //己方球门中心点坐标
    our_goal = MyVector2d(-FIELD_LENGTH_H, 0.0);
    //己方球门最里面柱子中心点坐标
    our_goal_l = MyVector2d(-FIELD_LENGTH_H, GOAL_WIDTH_H);
    our_goal_r = MyVector2d(-FIELD_LENGTH_H, -GOAL_WIDTH_H);
    //对方球门坐标
    their_goal = -our_goal;
    their_goal_l = -our_goal_r;
    their_goal_r = -our_goal_l;
    down_field = MyVector2d(1.0, 0);
    now = LATENCY_DELAY;
    game_state = 'S';
    // Robot Internal State
    for (i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        path[i].init(MAX_NODES,MAX_WAYPTS,0.15,0.75,100);
        path[i].robot_id = i;
        robot[i] = new Robot;
        robot[i]->init(i);
    }
    // init the modeller data sructures
    //modeller.initialize();
    // init roles
    orole_goalie = -1;
    trole_goalie = -1;
    trole_active = -1;
}



void World::update(VisionInfo& info0)
{
    VisionRawInfo vision_raw_info;
    int i,j;
    MyVector2d pos;
    double conf;
    time = info0.time;
    //球选择策略
    //1.可信度最高的球(比赛模式下用)
    //2.考虑半场因素(半场训练模式下用)
    //选球策略1
    conf=0;
    int iWhichBall=-1;
    for ( i = 0; i < MAX_BALLS; i++ )
    {
        if (info0.Balls[i].conf==0.0)continue;
        if (iWhichBall<0)
        {
            conf=info0.Balls[i].conf;
            pos=info0.Balls[i].pos;
            iWhichBall=i;
        }
        else
        {
            if (info0.Balls[i].conf>conf)
            {
                conf=info0.Balls[i].conf;
                pos=info0.Balls[i].pos;
                iWhichBall=i;
            }
        }
    }
    if (iWhichBall>=0)
    {
        //info[iWhichBall]=info.Balls[i];
        // we need to update the ball first
        vision_raw_info.pos = pos;
        vision_raw_info.timestamp = time;
        vision_raw_info.angle = 0.0;
        vision_raw_info.conf = conf;
        object_tracker_.ball.observe(vision_raw_info, time);
    }
    for (i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        object_tracker_.index2id[teamBlue][i]=-1;
        object_tracker_.index2id[teamYellow][i]=-1;
    }
    for (i=0; i<MAX_ROBOTS; i++)
    {
        object_tracker_.id2index[teamBlue][i]=-1;
        object_tracker_.id2index[teamYellow][i]=-1;
    }
    //机器人选择策略
    //按照编号从低到高
    int iBlueIndex=0;
    for ( i = 0; i < MAX_ROBOTS; i++ )
    {
        if (info0.Robots[teamBlue][i].conf==0.0)
        {
            object_tracker_.id2index[teamBlue][i]=-1;
            continue;
        }
        object_tracker_.index2id[teamBlue][iBlueIndex]=i;
        object_tracker_.id2index[teamBlue][i]=iBlueIndex;
        vision_raw_info.pos = info0.Robots[teamBlue][i].pos;
        vision_raw_info.timestamp = time;
        vision_raw_info.angle = info0.Robots[teamBlue][i].orientation;
        vision_raw_info.conf = info0.Robots[teamBlue][i].conf;
        object_tracker_.robots[teamBlue][i].observe(vision_raw_info, time);
        iBlueIndex++;
    }
    int iYellowIndex=0;
    for ( i = 0; i < MAX_ROBOTS; i++ )
    {
        if (info0.Robots[teamYellow][i].conf==0.0)
        {
            object_tracker_.id2index[teamYellow][i]=-1;
            continue;
        }
        object_tracker_.index2id[teamYellow][iYellowIndex]=i;
        object_tracker_.id2index[teamYellow][i]=iYellowIndex;
        vision_raw_info.pos = info0.Robots[teamYellow][i].pos;
        vision_raw_info.timestamp = time;
        vision_raw_info.angle = info0.Robots[teamYellow][i].orientation;
        vision_raw_info.conf = info0.Robots[teamYellow][i].conf;
        object_tracker_.robots[teamYellow][i].observe(vision_raw_info, time);
        iYellowIndex++;
    }
    // Game State
    goal_scored = 0;




    // deal with refbox to know the state of the game


    RefboxReceiveMutex.lock();
    RefboxCmd0=RefboxCmd;
    RefboxReceiveMutex.unlock();
    RefboxCmd0.Cmd='s';

    //tT=timeout gG=Goal Scored z=Time out End
    if (strchr("tTgGz", RefboxCmd0.Cmd) == NULL)
    {
        game_state = RefboxCmd0.Cmd;
    }
    else if (strchr("gG", RefboxCmd0.Cmd) != NULL && last_ref_state_ != RefboxCmd0.Cmd)
    {
        //判断哪方得分
        if ((RefboxCmd0.Cmd == 'g' && color_ == TEAM_YELLOW) || (RefboxCmd0.Cmd == 'G' && color_ == TEAM_BLUE))
            goal_scored = 1;
        else
            goal_scored = -1;
    }
    last_ref_state_ = RefboxCmd0.Cmd;

    //*************************************************************************************************







    // Update Robot Internal State
    //更新机器人内部状态，更新球对象内部保存的相关信息，包括球的速度，位置，与球的距离
    for (i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        robot[i]->updateSensors(*this);
    }
    n_teammates=0;
    n_opponents=0;
    for (i=1; i<NUM_TEAMS; i++)
    {
        for (j=0; j<MAX_TEAM_ROBOTS; j++)
        {

            if (!object_tracker_.Exists(i, j))
            {
                continue;
            }
            //qDebug()<<"color:"<< color_;
            if (i == color_)
            {
                teammate_id_to_index_[n_teammates++] = j;
            }
            else
            {
                opponent_id_to_index_[n_opponents++] = j;
            }
            // qDebug()<< "check:" << (i == color_)<< "n_teammates:" <<n_teammates<<"n_opponents:"<<n_opponents;
        }
    }
    UpdateHighLevel();
}

void World::UpdateHighLevel()
{
    static double last_time = -1;
    // Elapsed is set to the time passed since last call to updateHighLevel().
    double elapsed = (last_time < 0) ? 0.0 : time - last_time;
    last_time = time;
    MyVector2d ball = ball_position();
    //计算求的门限，带有施密特性质，施密特宽度100.0
    // ball position
    double epsilon = 100.0;
    if (ball.x - epsilon > ballXThreshold)
    {
        ballXThreshold = ball.x - epsilon;
    }
    else if (ball.x + epsilon < ballXThreshold)
    {
        ballXThreshold = ball.x + epsilon;
    }
    if (ball.y - epsilon > ballYThreshold)
    {
        ballYThreshold = ball.y - epsilon;
    }
    else if (ball.y + epsilon < ballYThreshold)
    {
        ballYThreshold = ball.y + epsilon;
    }
    // fieldPosition
    // 球在场地位置判断
    if (ballXThreshold > 400)
    {
        fieldPosition = TheirSide;
    }
    else if (ballXThreshold < -400)
    {
        fieldPosition = OurSide;
    }
    else
    {
        fieldPosition = Midfield;
    }
    // possession
    //计算我方离球距离
    double our_dist =
            (GetRobotPositionByID(nearest_teammate(ball)) - ball).length();
    //计算对方离球距离
    double their_dist =
            (opponent_position(nearest_opponent(ball)) - ball).length();
    //计算己方持球时间，如果离球距离超过180，按失球处理
    if (our_dist > 180)
    {
        possession_our_duration_ = 0;
    }
    else if (their_dist > 180)
    {
        possession_our_duration_ += elapsed;
    }
    //计算对方持球时间，如果离球距离超过180，按失球处理
    if (their_dist > 180)
    {
        possession_their_duration_ = 0;
    }
    else if (our_dist > 180)
    {
        possession_their_duration_ += elapsed;
    }
    //如果己方持球时间超过设定阀值，设定为己方持球状态
    if (possession_our_duration_ > DVAR(POSSESSION_US_HYSTERESIS_DURATION) &&
            possession_their_duration_ < DVAR(POSSESSION_THEM_HYSTERESIS_DURATION))
    {
        possession = OurBall;
    }
    //如果对方持球时间超过设定阀值，设定为对方持球状态
    else if (possession_our_duration_ < DVAR(POSSESSION_US_HYSTERESIS_DURATION) &&
             possession_their_duration_ > DVAR(POSSESSION_THEM_HYSTERESIS_DURATION))
    {
        possession = TheirBall;
    }
    //如果双方都没有持球
    else if (possession_our_duration_ == 0 &&
             possession_their_duration_ == 0)
    {
        possession = LooseBall;
    }
    // situation
    if ((strchr("s ", game_state) != NULL))
    {
        //COMM_START=s
        //如果对方持球，在我方场地，转为防守状态
        if (possession == TheirBall || fieldPosition == OurSide)
        {
            situation = Defense;
        }
        //如果我方持球，在对方场地，转为进攻状态
        else if (possession == OurBall || fieldPosition == TheirSide ||
                 situation == Special)
        {
            situation = Offense;
        }
        else
        {
            situation = situation;
        }
    }
    else if (game_state != 'S')
    {
        //COMM_STOP=S
        //如果停止，则为特殊状态
        situation = Special;
        if (World::restart())
        {
            //重新开始比赛的时候检查是哪方开球,根据开球方决定谁持球
            possession = restartWhoseKick();
        }
    }
    // opponent roles
    // 如果对方守门员没有确定，或者对方守门员不在对方防守区域内，选择一个在对方防守区域的机器人作为对方守门员
    // goalie
    if (orole_goalie < 0 ||
            //对方守门员不在对方防守区域内
            !obsPosition(opponent_position(orole_goalie), OBS_THEIR_DZONE))
    {
        for (int i=0; i<n_opponents; i++)
        {
            if (i == orole_goalie) continue;
            //选择一个在对方防守区域的机器人作为对方守门员
            if (obsPosition(opponent_position(i), OBS_THEIR_DZONE))
            {
                orole_goalie = i;
                break;
            }
        }
    }
    //如果该机器人越过中线，就肯定不是守门员
    if (orole_goalie > 0 && opponent_position(orole_goalie).x < 0)
    {
        orole_goalie = -1;
    }
    // defense zones
    //如果球在己方区域，计算在球在己方区域的时间长度，否则清零
    if (obsPosition(ball_position(), OBS_OUR_DZONE, BALL_RADIUS))
    {
        our_dzone_duration_ += elapsed;
    }
    else
    {
        our_dzone_duration_ = 0.0;
    }
    //如果球在对方区域，计算在球在对方区域的时间长度，否则清零
    if (obsPosition(ball_position(), OBS_THEIR_DZONE, BALL_RADIUS))
    {
        their_dzone_duration_ += elapsed;
    }
    else
    {
        their_dzone_duration_ = 0.0;
    }
}

MyVector2d World::ball_position(double t)
{
    if (t < 0) t = now;
    //qDebug()<<"robot position"<<object_tracker_.ball.position(t).x<<object_tracker_.ball.position(t).y;
    return object_tracker_.ball.position(t) * side_;
}

MyVector2d World::ball_velocity(double t)
{
    if (t < 0) t = now;
    return object_tracker_.ball.velocity(t) * side_;
}

Matrix World::ball_covariances(double t)
{
    if (t < 0) t = now;
    return object_tracker_.ball.covariances(t);
}

int World::teammate_type(int id)
{
    return ROBOT_TYPE_OMNI;
}

double World::teammate_radius(int id)
{
    return OMNIBOT_RADIUS;
}

int World::teammate_frame_index(int id)
{
    return teammate_id_to_index_[id];
}

//确定当前队友的位置
// get robot position by id
MyVector2d World::GetRobotPositionByID(int id, double t)
{
    if (t < 0) t = now;
//    qDebug()<<"id"<<id<<"t:"<<t;
//    qDebug() << "side:"<<(int)side_;
//    qDebug()<<"x:"<<(object_tracker_.robots[color_][teammate_id_to_index_[id]].position(t) * side_).x;
//    qDebug()<<"y:"<<(object_tracker_.robots[color_][teammate_id_to_index_[id]].position(t) * side_).y;
    return object_tracker_.robots[color_][teammate_id_to_index_[id]].position(t) * side_;
}

//确定当前队友的速度
MyVector2d World::GetRobotVelocityByID(int id, double t)
{
    if (t < 0) t = now;
    return object_tracker_.robots[color_][teammate_id_to_index_[id]].velocity(t) * side_;
}

MyVector2d World::teammate_robot_velocity(int id, double t)
{
    return GetRobotVelocityByID(id, t).rotate(-teammate_direction(id, t));
}

//确定当前队友的方向
double World::teammate_direction(int id, double t)
{
    if (t < 0) t = now;
    return anglemod( (side_ < 0 ? M_PI : 0.0) +
                     object_tracker_.robots[color_][teammate_id_to_index_[id]].direction(t));
}

//确定当前队友的角速度;
double World::teammate_angular_velocity(int id, double t)
{
    if (t < 0) t = now;//estimated latency time
    return object_tracker_.robots[color_][teammate_id_to_index_[id]].angular_velocity(t);
}

//确定当前队友是否被围困或卡住
//ensure that whether teammate is stuck
double World::teammate_stuck(int id)
{
    return object_tracker_.robots[color_][teammate_id_to_index_[id]].stuck(0.0);
}

//当前队友接收的控制命令:
void World::teammate_command(int id, double vx, double vy, double va)
{
    object_tracker_.robots[color_][teammate_id_to_index_[id]].command(time, MyVector3d(vx, vy, va));
}

//确定对方队员的位置
MyVector2d World::opponent_position(int id, double t)
{
    if (t < 0) t = now;
    char ocolor;
    if (color_==teamBlue)
    {
        ocolor=teamYellow;
    }
    else
    {
        ocolor=teamBlue;
    }
    return object_tracker_.robots[ocolor][opponent_id_to_index_[id]].position(t) * side_;
}

//确定对方队员的运动速度;
MyVector2d World::opponent_velocity(int id, double t)
{
    if (t < 0) t = now;
    char ocolor;
    if (color_==teamBlue)
    {
        ocolor=teamYellow;
    }
    else
    {
        ocolor=teamBlue;
    }
    return object_tracker_.robots[ocolor][opponent_id_to_index_[id]].velocity(t) * side_;
}

void World::teammate_raw(int id, VisionRawInfo &vpos)
{
    vpos.pos = vision_info_.Robots[color_][teammate_id_to_index_[id]].pos;
    vpos.conf = vision_info_.Robots[color_][teammate_id_to_index_[id]].conf;
    vpos.angle = vision_info_.Robots[color_][teammate_id_to_index_[id]].orientation;
    vpos.pos *= -1;
    vpos.angle = anglemod(vpos.angle + M_PI);
}

void World::opponent_raw(int id, VisionRawInfo &vpos)
{
    char ocolor;
    if (color_==teamBlue)
    {
        ocolor=teamYellow;
    }
    else
    {
        ocolor=teamBlue;
    }
    vpos.pos = vision_info_.Robots[ocolor][teammate_id_to_index_[id]].pos;
    vpos.conf = vision_info_.Robots[ocolor][teammate_id_to_index_[id]].conf;
    vpos.angle = vision_info_.Robots[ocolor][teammate_id_to_index_[id]].orientation;
    vpos.pos *= -1;
    vpos.angle = anglemod(vpos.angle + M_PI);
}

double World::ball_raw(VisionRawInfo &vpos)
{
    vpos.pos = vision_info_.Balls[0].pos;
    vpos.conf = vision_info_.Balls[0].conf;
    vpos.pos *= -1;
    vpos.angle = anglemod(vpos.angle + M_PI);
    return((time > vision_info_.time)?
               0.0 : vision_info_.Balls[0].conf);
}

//判断哪一方碰了球
int World::ball_collision(double t)
{
    if (t < 0) t = now;
    char ocolor;
    if (color_==teamBlue)
    {
        ocolor=teamYellow;
    }
    else
    {
        ocolor=teamBlue;
    }
    int team, robot;
    if (object_tracker_.ball.collision(t, team, robot))
    {
        if (team == color_)
        {
            return OBS_TEAMMATE(robot);
        }
        else if (team == ocolor)
        {
            return OBS_OPPONENT(robot);
        }
    }
    return 0;
}

#define COMBUFSIZE 100
unsigned char ComBuf[COMBUFSIZE];


int iComBufSize=0;
#if 0
const unsigned char CHARSYN=0x7E;
const unsigned char CHARESC=0xf0;
const unsigned char ESC_SYN=0xf1;
const unsigned char ESC_ESC=0xf2;
#endif
void ClearCombufArray()
{
//    for(int i=0;i<COMBUFSIZE;i++)
//    {
//        ComBuf[i]='\0';
//    }
    iComBufSize=0;
}

void ClearCombuf()
{
    ComBuf[iComBufSize++]=CHARSYN;
}

void FillComChar(unsigned char c)
{
    if (c==CHARSYN)
    {
        ComBuf[iComBufSize++]=CHARESC;
        ComBuf[iComBufSize++]=ESC_SYN;
    }
    else if (c==CHARESC)
    {
        ComBuf[iComBufSize++]=CHARESC;
        ComBuf[iComBufSize++]=ESC_ESC;
    }
    else
    {
        ComBuf[iComBufSize++]=c;
    }
}

void FillVerify()
{
    int i;
    unsigned char cVerify=0;
    if (iComBufSize<1)return;
    for (i=1; i<iComBufSize; i++)
    {
        cVerify=cVerify+ComBuf[i];
    }
    cVerify+=0x15;
    ComBuf[iComBufSize++]=cVerify;

}

unsigned char ComChar=0;

#define COM_SENDBACK			(1<<7)
#define COM_DRIBBLER_EN			(1<<6)
#define COM_FORCEKICK			(1<<5)
#define COM_CHIPKICK			(1<<4)
#define COM_KICK			(1<<3)

#define COM_VXSIGN			(1<<0)
#define COM_VYSIGN			(1<<1)
#define COM_VZSIGN			(1<<2)
#define COM_DRIBBLER_DIR		(1<<3)
//#define COM_DRIBBLER_SPEED	高4位

extern SSL_RadioFrame RadioCmds;
//向指定的机器人发出踢球和带球命令
void World::go(int id, double vx, double vy, double va,
               int kick_power, bool chipkick_on,
               int dribbler_speed,
               bool forcekick_on,
               bool sendback_on
               )
{
    // ensure the velocity of the x, y and z are not none
    if (isnan(vx) || isnan(vy) || isnan(va))
    {
        fprintf(stderr, "WARNING NAN: World::go(id = %d)\n", id);
        return;
    }
#if PORT_TEST
    if (fabs(vx) < 10.0 && fabs(vy) < 10.0 && fabs(va) < 0.0001 &&
            !kicker_on && !dribbler_on)
    {
        fprintf(stderr, "HALT %d: %f %f %f\n", id, vx, vy, va);
        return halt(id);
    }
    else
        fprintf(stderr, "NOHALT %d: %f %f %f\n", id, vx, vy, va);
#endif
    // set the veocity of the robot
    teammate_command(id, vx, vy, va);
    // lock the resources so that no other program will use this
    RadioSendMutex.lock();
    // create a pointer of the command type of SSL Robot command which will be sent through the net
    SSL_RobotCmd* pCmd;
    // store the infomation of whether the robot has been found
    bool has_found_robot=false;
    // store the infomation of the size of the command
    int isize=RadioCmds.cmds_size();
    // enable the changes on different robots

    for (int i=0; i<isize; i++)
    {
        pCmd=RadioCmds.mutable_cmds(i);
        if (pCmd->id()==id)
        {
            has_found_robot=true;
            break;
        }
    }
    // if the robot has not been found, then add a new command for robot
    /*
      Here in my opinion, it is not about whether robot has been found, it is
      about whether there is cmd in the radiocmd. If there is, then do not send
      any new command, but if there is not, then arrange a new command. lu_test
    */
    if (!has_found_robot)
    {
        pCmd=RadioCmds.add_cmds();
    }

    // set the command for robot
    qDebug()<<"Command:id="<<id<<", vx"<<vx<<", vy="<<vy<<", va="<<va;
    pCmd->set_id(id);
    pCmd->set_team(color_);
    pCmd->set_vx(vx);
    pCmd->set_vy(vy);
    pCmd->set_va(va);
    pCmd->set_is_chip(chipkick_on);
    if (dribbler_speed<0)
    {
        pCmd->set_drib_dir(1);
    }
    else
    {
        pCmd->set_drib_dir(0);
    }
    pCmd->set_send_back(sendback_on);
    pCmd->set_kick_time(kick_power);
    pCmd->set_forcekick(forcekick_on);
    pCmd->set_drib_speed(abs(dribbler_speed));

    RadioSendMutex.unlock();

#if 0
    int fd_wireless=serial_sever_->port();
    TransparentOperation package;
    RobotParamters robot_parameters;
    robot_parameters = ClearRobotParameters(robot_parameters);
    robot_parameters.x_velocity = vx;
    robot_parameters.y_velocity = vy;
    WirelessRobot robot1 = WirelessRobot(robot_parameters);
    robot1.set_x_velocity(vx);
    robot1.set_y_velocity(vy);
    robot1.set_index(id);
    robot1.set_dribble(1);


    QByteArray temp_array;
    temp_array = package.FormByteCommand(temp_array,robot1);
    int temp_size = temp_array.size();
    unsigned char *temp_pointer = new unsigned char(temp_size);
    memcpy(temp_pointer, temp_array.data(), temp_size);

    package.SendPackage(temp_pointer,temp_size,fd_wireless);

#endif


#if 1 //lu_test
    // deal with the physical communication
    if (serial_sever_->IsOpen())
    {
        if (forcekick_on)
        {
            ComChar|=COM_FORCEKICK;
        }
        if (sendback_on)
        {
            ComChar|=COM_SENDBACK;
        }
        if (kick_power)
        {
            if (chipkick_on)
            {
                ComChar|=COM_CHIPKICK;
            }
            else
            {
                ComChar|=COM_KICK;
            }
        }
        FillComChar(ComChar);
        //第二个字符
        ComChar=0;
        if (dribbler_speed<0)
        {
            dribbler_speed=-dribbler_speed;
            ComChar=dribbler_speed<<4|COM_DRIBBLER_DIR;
        }
        else
        {
            ComChar=dribbler_speed<<4;
        }
        if (vx<0)
        {
            ComChar|=COM_VXSIGN;
            vx=-vx;
        }
        if (vy<0)
        {
            ComChar|=COM_VYSIGN;
            vy=-vy;
        }
        if (va<0)
        {
            ComChar|=COM_VZSIGN;
            va=-va;
        }
        va=va*10;
        FillComChar(ComChar);
        //第三个字符
        FillComChar((unsigned char)(vx>255?255:vx));
        FillComChar((unsigned char) (vy>255?255:vy));
        FillComChar((unsigned char)(va>255?255:va));
        FillComChar(kick_power);
        FillVerify();
        serial_sever_->SendTransparentPackage((unsigned char*)ComBuf,iComBufSize);
        ClearCombufArray();
    }
#endif

#ifdef PORT_TEST
    //        if (m_SerialPort.IsOpen())
    //        {
    //                ClearCombuf();
    //                //第一个字符
    //                ComChar=id&0x07;
    //                if (forcekick_on)
    //                {
    //                        ComChar|=COM_FORCEKICK;
    //                }
    //                if (sendback_on)
    //                {
    //                        ComChar|=COM_SENDBACK;
    //                }
    //                if (kick_power)
    //                {
    //                        if (chipkick_on)
    //                        {
    //                                ComChar|=COM_CHIPKICK;
    //                        }
    //                        else
    //                        {
    //                                ComChar|=COM_KICK;
    //                        }
    //                }
    //                FillComChar(ComChar);
    //                //第二个字符
    //                ComChar=0;
    //                if (dribbler_speed<0)
    //                {
    //                        dribbler_speed=-dribbler_speed;
    //                        ComChar=dribbler_speed<<4|COM_DRIBBLER_DIR;
    //                }
    //                else
    //                {
    //                        ComChar=dribbler_speed<<4;
    //                }
    //                if (vx<0)
    //                {
    //                        ComChar|=COM_VXSIGN;
    //                        vx=-vx;
    //                }
    //                if (vy<0)
    //                {
    //                        ComChar|=COM_VYSIGN;
    //                        vy=-vy;
    //                }
    //                if (va<0)
    //                {
    //                        ComChar|=COM_VZSIGN;
    //                        va=-va;
    //                }
    //                va=va*10;
    //                FillComChar(ComChar);
    //                //第三个字符
    //                FillComChar(unsigned char(vx>255?255:vx));
    //                FillComChar(unsigned char(vy>255?255:vy));
    //                FillComChar(unsigned char(va>255?255:va));
    //                FillComChar(kick_power);
    //                FillVerify();
    //                m_SerialPort.Write((char*)ComBuf,iComBufSize);
#endif
    //        }
}

//
void World::halt(int id)
{
    teammate_command(id, 0, 0, 0);
    go(id,0,0,0);
}

//判断球是否出界
bool World::ballOutOfPlay(double time)
{
    MyVector2d ball = ball_position(time);
    return (fabs(ball.x) - BALL_DIAMETER > FIELD_LENGTH_H ||
            fabs(ball.y) - BALL_DIAMETER > FIELD_WIDTH_H);
}

//确定任意球点
MyVector2d World::freeKickPosition(MyVector2d ball)
{
    if (fabs(ball.x) > FIELD_LENGTH_H - FREEKICK_FROM_GOAL)
    {
        ball.x = copysign(FIELD_LENGTH_H - FREEKICK_FROM_GOAL, ball.x);
        ball.y = copysign(FIELD_WIDTH_H - FREEKICK_FROM_WALL, ball.y);
    }
    else if (fabs(ball.y) > FIELD_WIDTH_H - FREEKICK_FROM_WALL)
    {
        ball.y = copysign(FIELD_WIDTH_H - FREEKICK_FROM_WALL, ball.y);
    }
    return ball;
}

//球在左右哪边半场判断
int World::sideBall()
{
    return (ballYThreshold > 0.0 ? 1 : -1);
}

//判断对手左右两边哪边强势
int World::sideStrong()
{
    double center = 0.0;
    for (int i=0; i < n_opponents; i++)
    {
        center += opponent_position(i).y;
    }
    return (center > 0.0 ? 1 : -1);
}

//检查球在球门边线哪一边;
int World::sideBallOrStrong()
{
    if (fabs(ball_position().y) > GOAL_WIDTH_H)
    {
        return sideBall();
    }
    else
    {
        return sideStrong();
    }
}

//检查球是否在我方禁区内
bool World::inOurDefenseZone()
{
    return (our_dzone_duration_ > DVAR(DZONE_HYSTERESIS_DURATION));
}

//检查球是否在对方禁区内
bool World::inTheirDefenseZone()
{
    return (their_dzone_duration_ > DVAR(DZONE_HYSTERESIS_DURATION));
}

//检查对方是否有两人在禁区内防守(这是一种犯规行为)
bool World::twoDefendersInTheirDZone()
{
    int n = 0;
    for (int i=0; i<n_opponents; i++)
    {
        if (obsPosition(opponent_position(i), OBS_THEIR_DZONE, 0))
        {
            n++;
        }
    }
    return (n >= 2);
}

bool World::restart()
{
    //开球、罚球、直接任意球
    return (strchr("kKpPfF ", game_state) != NULL);
}

bool World::restartPenalty()
{
    //罚球
    return (strchr("pP ", game_state) != NULL);
}

//中立重新启动，应该是争球
bool World::restartNeutral()
{
    return (game_state == COMM_RESTART);
}

//重新开始比赛的时候检查是哪方开球
//check which side starts the ball when restart
World::Possession World::restartWhoseKick()
{
    bool blue = (strchr("KPF", game_state) != NULL);
    bool yellow = (strchr("kpf", game_state) != NULL);
    if ((blue && color_ == TEAM_BLUE) ||
            (yellow && color_ == TEAM_YELLOW))
    {
        return OurBall;
    }
    else if (!blue && !yellow)
    {
        return LooseBall;
    }
    else
    {
        return TheirBall;
    }
}

//寻找离点p最近的队友
int World::nearest_teammate(MyVector2d p, double time)
{
    int dist_i = -1;
    double dist = 0;
    for (int i=0; i<n_teammates; i++)
    {
        double d = (p - GetRobotPositionByID(i, time)).length();
        if (dist_i < 0 || d < dist)
        {
            dist_i = i;
            dist = d;
        }
    }
    return dist_i;
}

//找到离点p最近的对手
int World::nearest_opponent(MyVector2d p, double time)
{
    int dist_i = -1;
    double dist = 0;
    for (int i=0; i<n_opponents; i++)
    {
        double d = (p - opponent_position(i, time)).length();
        if (dist_i < 0 || d < dist)
        {
            dist_i = i;
            dist = d;
        }
    }
    return dist_i;
}

//判断位置p是否存在障碍,并判断障碍的属性
//例如obsPosition(opponent_position(orole_goalie), OBS_THEIR_DZONE)用于确定对方守门员是否在对方防守区域内
//输入参数obs_flags用于控制检测哪些类型的障碍
//返回值表示存在哪些类型的障碍
int World::obsPosition(MyVector2d p, int obs_flags,
                       double pradius, double time)
{
    int rv = 0;
    //检查每个队友是否是障碍
    // Teammates
    for (int i=0; i<n_teammates; i++)
    {
        if (!(obs_flags & OBS_TEAMMATE(i))) continue;
        double radius = TEAMMATE_EFFECTIVE_RADIUS + pradius;
        if ((p - GetRobotPositionByID(i, time)).length() <= radius)
        {
            rv |= OBS_TEAMMATE(i);
        }
    }
    //检查每个对手是否是障碍
    // Opponents
    for (int i=0; i<n_opponents; i++)
    {
        if (!(obs_flags & OBS_OPPONENT(i))) continue;
        double radius = OPPONENT_EFFECTIVE_RADIUS + pradius;
        if ((p - opponent_position(i, time)).length() <= radius)
        {
            rv |= OBS_OPPONENT(i);
        }
    }
    //检查球是否是障碍
    // Ball
    if (obs_flags & OBS_BALL)
    {
        double radius = BALL_RADIUS + pradius;
        if ((p - ball_position(time)).length() <= radius)
        {
            rv |= OBS_BALL;
        }
    }
    //在墙外面
    // Walls
    if (obs_flags & OBS_WALLS)
    {
        double radius = pradius;
        if (fabs(p.x) + radius > FIELD_LENGTH_H ||
                fabs(p.y) + radius > FIELD_WIDTH_H)
        {
            rv |= OBS_WALLS;
        }
    }
    //在己方防守区域
    // Defense Zones
    if (obs_flags & OBS_OUR_DZONE)
    {
        double radius = pradius;
        if (p.x <= -FIELD_LENGTH_H + DEFENSE_DEPTH + radius && fabs(p.y) <= DEFENSE_WIDTH_H + radius)
        {
            rv |= OBS_OUR_DZONE;
        }
    }
    //在对方防守区域
    if (obs_flags & OBS_THEIR_DZONE)
    {
        double radius = pradius;
        if (p.x >= FIELD_LENGTH_H - DEFENSE_DEPTH - radius &&	fabs(p.y) <= DEFENSE_WIDTH_H + radius)
        {
            rv |= OBS_THEIR_DZONE;
        }
    }
    // Nothing Left
    return rv;
}

//判断附近的障碍物的属性
//检查直线上是否存在障碍
int World::obsLine(MyVector2d p1, MyVector2d p2, int obs_flags,
                   double pradius, double time)
{
    // Teammates
    //检查队友是否是直线上的障碍
    for (int i=0; i<n_teammates; i++)
    {
        if (!(obs_flags & OBS_TEAMMATE(i))) continue;
        double radius = TEAMMATE_EFFECTIVE_RADIUS + pradius;
        MyVector2d p = GetRobotPositionByID(i, time);
        if ((point_on_segment(p1, p2, p) - p).length() > radius)
        {
            obs_flags &= ~OBS_TEAMMATE(i);
        }
    }
    //检查对方机器人是否是直线上的障碍
    // Opponents
    for (int i=0; i<n_opponents; i++)
    {
        if (!(obs_flags & OBS_OPPONENT(i))) continue;
        double radius = OPPONENT_EFFECTIVE_RADIUS + pradius;
        MyVector2d p = opponent_position(i, time);
        if ((point_on_segment(p1, p2, p) - p).length() > radius)
        {
            obs_flags &= ~OBS_OPPONENT(i);
        }
    }
    // Ball
    //检查球是否是直线上的障碍
    if (obs_flags & OBS_BALL)
    {
        double radius = BALL_RADIUS + pradius;
        MyVector2d p = ball_position(time);
        if ((point_on_segment(p1, p2, p) - p).length() > radius)
        {
            obs_flags &= ~OBS_BALL;
        }
    }
    // Walls
    // Defense Zones
    // Nothing Left
    return obs_flags;
}

//寻找直线上遇到的第一个障碍物
//返回障碍物障碍标志
int World::obsLineFirst(MyVector2d p1, MyVector2d p2, int obs_flags,
                        MyVector2d &first, double pradius, double time)
{
    int rv = 0;
    first = p2;
    // Teammates
    for (int i=0; i<n_teammates; i++)
    {
        if (!(obs_flags & OBS_TEAMMATE(i))) continue;
        double radius = TEAMMATE_EFFECTIVE_RADIUS + pradius;
        MyVector2d p = GetRobotPositionByID(i, time);
        MyVector2d pp = point_on_segment(p1, first, p);
        double d = (pp - p).length();
        if (d < radius)
        {
            double dx = sqrt(radius * radius - d * d);
            if ((p1 - pp).length() < dx)
            {
                first = p1;
                return OBS_TEAMMATE(i);
            }
            else
            {
                first = pp + (p1 - pp).norm(dx);
                rv = OBS_TEAMMATE(i);
            }
        }
    }
    // Opponents
    for (int i=0; i<n_opponents; i++)
    {
        if (!(obs_flags & OBS_OPPONENT(i))) continue;
        double radius = OPPONENT_EFFECTIVE_RADIUS + pradius;
        MyVector2d p = opponent_position(i, time);
        MyVector2d pp = point_on_segment(p1, first, p);
        //机器人到中垂点距离
        double d = (pp - p).length();
        if (d < radius)
        {
            //机器人被直线割掉部分的高度
            double dx = sqrt(radius * radius - d * d);
            if ((p1 - pp).length() < dx)
            {
                //冲突点在对方机器人内，则返回起点
                first = p1;
                return OBS_OPPONENT(i);
            }
            else
            {
                //否则从中垂点后退dx
                first = pp + (p1 - pp).norm(dx);
                rv = OBS_OPPONENT(i);
            }
        }
    }
    // Ball
    if (obs_flags & OBS_BALL)
    {
        double radius = BALL_RADIUS + pradius;
        MyVector2d p = ball_position(time);
        MyVector2d pp = point_on_segment(p1, first, p);
        double d = (pp - p).length();
        if (d < radius)
        {
            double dx = sqrt(radius * radius - d * d);
            if ((p1 - pp).length() < dx)
            {
                first = p1;
                return OBS_BALL;
            }
            else
            {
                first = pp + (p1 - pp).norm(dx);
                rv = OBS_BALL;
            }
        }
    }
    // Walls
    // Defense Zones
    if (obs_flags & OBS_THEIR_DZONE)
    {
        if (obsPosition(p1, OBS_THEIR_DZONE, pradius, time))
        {
            first = p1;
            return OBS_THEIR_DZONE;
        }
        MyVector2d i;
        i = intersection(p1, p2,
                         MyVector2d(FIELD_LENGTH_H-DEFENSE_DEPTH-pradius,
                                    DEFENSE_WIDTH_H+pradius),
                         MyVector2d(FIELD_LENGTH_H-DEFENSE_DEPTH-pradius,
                                    -DEFENSE_WIDTH_H-pradius));
        if ((i - p1).dot(first - p1) > 0 && (i - first).dot(p1 - first) > 0)
        {
            first = i;
            rv = OBS_THEIR_DZONE;
        }
        i = intersection(p1, p2,
                         MyVector2d(FIELD_LENGTH_H-DEFENSE_DEPTH-pradius,
                                    DEFENSE_WIDTH_H+pradius),
                         MyVector2d(FIELD_LENGTH_H, DEFENSE_WIDTH_H+pradius));
        if ((i - p1).dot(first - p1) > 0 && (i - first).dot(p1 - first) > 0)
        {
            first = i;
            rv = OBS_THEIR_DZONE;
        }
        i = intersection(p1, p2,
                         MyVector2d(FIELD_LENGTH_H-DEFENSE_DEPTH-pradius,
                                    -DEFENSE_WIDTH_H-pradius),
                         MyVector2d(FIELD_LENGTH_H, -DEFENSE_WIDTH_H-pradius));
        if ((i - p1).dot(first - p1) > 0 && (i - first).dot(p1 - first) > 0)
        {
            first = i;
            rv = OBS_THEIR_DZONE;
        }
    }
    // Nothing Left
    return obs_flags;
}

//计算从p1到p2直线上有多少个障碍
//判断身边障碍物的总数(包括球，队友和对方球员);
int World::obsLineNum(MyVector2d p1, MyVector2d p2, int obs_flags,
                      double pradius, double time)
{
    int count = 0;
    //计算己方机器人障碍数
    // Teammates
    for (int i=0; i<n_teammates; i++)
    {
        if (!(obs_flags & OBS_TEAMMATE(i))) continue;
        double radius = TEAMMATE_EFFECTIVE_RADIUS + pradius;
        MyVector2d p = GetRobotPositionByID(i, time);
        if ((point_on_segment(p1, p2, p) - p).length() <= radius)
        {
            count++;
        }
    }
    //计算对手机器人障碍数
    // Opponents
    for (int i=0; i<n_opponents; i++)
    {
        if (!(obs_flags & OBS_OPPONENT(i))) continue;
        double radius = OPPONENT_EFFECTIVE_RADIUS + pradius;
        MyVector2d p = opponent_position(i, time);
        if ((point_on_segment(p1, p2, p) - p).length() <= radius)
        {
            count++;
        }
    }
    //计算球障碍数
    // Ball
    if (obs_flags & OBS_BALL)
    {
        double radius = BALL_RADIUS + pradius;
        MyVector2d p = ball_position(time);
        if ((point_on_segment(p1, p2, p) - p).length() <= radius)
        {
            count++;
        }
    }
    // Walls
    // Defense Zones
    // Nothing Left
    return count;
}

//判断进攻队员射门条件是否成熟;
bool World::obsBlocksShot(MyVector2d p, double pradius, double time)
{
    MyVector2d ball = ball_position(time);
    double a = (their_goal_r - ball).perp().dot(p - ball);
    double b = (their_goal_l - ball).perp().dot(p - ball);
    return (a * b) < 0;
}

//选择罚球机器人
//确定对方的罚球队员位置。
//即超过中线后，最靠近己方的机器人
int World::choosePenaltyKicker(void)
{
    int opponentid = -1;
    double minx = 0.0;
    MyVector2d rpos;
    // for a penalty kick we know that they have to be behind
    // the kicker
    for (int i = 0; i < n_opponents; i++)
    {
        rpos = opponent_position(i);
        if (rpos.x < minx)
        {
            opponentid = i;
            minx = rpos.x;
        }
    }
    return (opponentid);
}


VisionInfo vision_info;
VINFO_RAW VInfoRaw;

