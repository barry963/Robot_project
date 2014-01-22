#include "strategy_thread.h"

#include "user_interface/field_related/field_robot.h"
#include "user_interface/field_related/field_ball.h"
#include "user_interface/strategy_control_window.h"
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <deque>
#include <QMutex>

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "net/net_vision_interface.h"
#include "net/net_refbox_command.h"

#include "net/net_radio_server.h"
#include "net/net_refbox_client.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/play/play.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/defense_tactics.h"
#include "user_interface/field_related/paint_tools/paint_buffer.h"
#include "user_interface/field_related/field_global_function.h"
#include "net/thread_tools/field_timer.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/thread_count_tools/hires_timer.h"
#include "control_hub/human_control/joystick_gao.h"

/*******************************************************************/
// communication through serial port
#include "wireless/serial_server.h"
/*******************************************************************/
#include <iostream>


// mutex for all running things
QMutex runstatusMutex;
static   Robot::RobotCommand robot_command;
RunStatus InternalStatus,OldInternalStatus,StatusOnGUI={RUN_PAUSE,STRATEGY_RUN,0,NULL};

/*Lu_test It seems that tactics are made automatically, but gui_tactics are made by the control panel */
Tactic *tactics[MAX_TEAM_ROBOTS];


Tactic *RunGui_tactics[MAX_TEAM_ROBOTS] = { NULL, NULL, NULL, NULL, NULL};

//网络传输 network communication
SSL_WrapperPacket VisionPacket;
// open a serial port to communicate with the real robot
// SerialServer serial_send_server; // initialized in the world
// open a radio send send server. it is used in the strategy thread
RadioServer radio_send_server;
// open a vision client and wait for the infomation. it is also used in the strategy thread
// VisionClient vision_client;
// open a refbox client to get infomation from the reference box
RefboxClient refbox_client;

//通过游戏操作杆操作的机器人编号
int iJoystickRobot=0;
// Gui Tactics.  These override strategy's tactics.
extern Tactic *gui_tactics[MAX_TEAM_ROBOTS];//tactic for each robot

StrategyTimer timer;

StrategyThread::StrategyThread ( FieldView *_soccerView,bool _bThreadMode )
{
    soccerView = _soccerView;
    bThreadMode=_bThreadMode;
    thread_terminated_ = false;
}

void RadioNetCmdSend()
{
    int size= RadioCmds.cmds_size();
    if(size<=0)
    {
        return;
    }

    RadioCmds.set_time_stamp(0);
    if(!RadioCmds.IsInitialized())
    {
        printf("Radio Send Packet IsInitialized Error!\r\n");
        return;
    }
    // send the command package thread the internet
    radio_send_server.Send(RadioCmds);
}






QString sTactics[5];

char guitacticsbuf[1000];//temperary store the tactic name of InternalStatus.tactic_name_

// main program
void StrategyThread::run()
{
    int iFrameCount = 0;
    // set all robot parameters to 0
    mzero(robot_command);
    // set the command type
    robot_command.cmd = Robot::CmdPosition;
    // set the command angle
    robot_command.angle = M_PI/4;
    // set the angle tolerance
    robot_command.angle_tolerance = 0.5;
    // set observation type of world
    robot_command.observation_type = OBS_EVERYTHING;
    // set the velocity to be zero
    robot_command.velocity.set(0,0);

    // create 4 TCoordinate instances, I suppose they are five points that we need to let robot to go.
    BCoordinate ct(0,0);
    BCoordinate c0(-1000,0);
    BCoordinate c1(-1000,500);
    BCoordinate c2(-1000,-500);
    BCoordinate c3(-2000,500);
    BCoordinate c4(-2000,-500);

    double dAngle = 0;
//    double dAngle=M_PI/4;
    //TCoordinate f(100,100);

    // specify the name of the play book
    char *playbook_file = "advanced.plb";

    unsigned long tick,oldtick;
    tick = oldtick = GetTickCount();

    unsigned long tick1,oldtick1;
    tick1 = oldtick1 = GetTickCount();

    // initialize the all robots' tactics to be NULL
    for (int i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        tactics[i] = NULL;
    }

    // initialize the world's state
    world.init(soccerside, soccerteam);
    static int oldsoccerteam = soccerteam;


    // initialize the radio send server thread

//    if (!radio_send_server.Open ( soccerteam==teamBlue?10003:10005))
//    {
//        printf("Radio Send Server Open failed!\r\n");
//    }


    // trigger updates on the field gui
    connect(this,SIGNAL(guiRefresh()),soccerView,SLOT(tickslot()),Qt::AutoConnection);

    // initialize strategy
    strategy.init(playbook_file);
    oldTickCount = ::GetTickCount();
    //strategy.parse("warmup 1");

    bool bVisionOverTime;

    while(!thread_terminated_)//excute the strategy here lu_test
    {
        /// if change the team, then we need to restart the server
        if (oldsoccerteam != soccerteam)
        {
            if (!radio_send_server.Open ( soccerteam==teamBlue?10003:10005))
            {
                printf("Radio Send Server Open failed!\r\n");
            }
            else
            {
                printf("Radio Send Server Open OK!\r\n");
            }
            oldsoccerteam = soccerteam;
        }

        //-----------------------------
        if(SkipVisionWait())
        {
            emit guiRefresh();
            Sleep(16);
        }
        else
        {
            bVisionOverTime = do_vision_recv();
            if(SkipVisionWait())bVisionOverTime=false;
        }

        if(thread_terminated_)break;

        if(bVisionOverTime)
        {
            printf("Strategy continue\r\n");
            continue;
        }
        RadioCmds.Clear();

        //Gui显示缓冲区切换
        GuiCmd.StrategySwitchCmds();

        //策略运行频率计算--------------
        iFrameCount++;

        //runstatusMutex.lock();


        // assign the status on GUI to the internal status
        InternalStatus = StatusOnGUI;

        // copy the selected tactic_string to tRunStatus
        if(StatusOnGUI.tactic_name_)
        {
            strcpy(guitacticsbuf,StatusOnGUI.tactic_name_);
            InternalStatus.tactic_name_ = guitacticsbuf;
        }
        else
        {
            InternalStatus.tactic_name_ = NULL;
        }

        //-----------------------------------------------------
        //计算策略周期 calculate strategy cycle
        tick1 = ::GetTickCount();
        if(tick1-oldtick1>18)
        {
            //printf("Strategy OverTime Error %d\r\n",tick1-oldtick1);
        }
        oldtick1=tick1;
        //-----------------------------------------------------
        //计算频率 calculate frequency

        TickCount=::GetTickCount();
        if(TickCount-oldTickCount>2000)
        {
            oldTickCount = TickCount;
            iFrameCount = 0;
            printf("Strategy Over Time Error,auto reset!(StrategtThread.cpp)\r\n");
        }
        else if(TickCount-oldTickCount>1000)
        {
            StatusOnGUI.dFreq = iFrameCount*1.0/(TickCount-oldTickCount)*1000;
            oldTickCount = TickCount;
            iFrameCount = 0;
        }
        //runstatusMutex.unlock();

        //运行状态控制--running status control
        switch(InternalStatus.Status)
        {
            case RUN_PAUSE:
                continue;
                // break the outside loop
                break;
            case RUN_STEP:
                //runstatusMutex.lock();
                StatusOnGUI.Status=RUN_PAUSE;
                //runstatusMutex.unlock();
                break;
            case RUN_STOP:
                for(int i=0; (i<world.n_teammates); i++)
                {
                    // this will send the command through net and the serial
                    world.go(i);
                }
                // send the package thread the internet
                RadioNetCmdSend();
                continue;
                break;
            default:
                //qDebug() << "Enter default InternalStatus.Status (Running)...";
                break;
        }

//        qDebug()<<InternalStatus.tactic_name_;
//        qDebug()<<OldInternalStatus.tactic_name_;
//        qDebug()<<!IsStrategyOld();

//        if (InternalStatus.tactic_name_ && !IsStrategyOld())
        if (InternalStatus.tactic_name_ && !IsStrategyOld())
        {
            for (int i=0; i<world.n_teammates; i++)
            {
                // this may be used to clear the strategy related effect on GUI
                ClearGuiTactic(i);
                // assign different gui status to each robot
                // this gui_tatics will lead to the real tactic.
                // call corresponding tactic
                //qDebug() << "InternalStatus.tactic_string: " << InternalStatus.tactic_name_;
                gui_tactics[i] = Tactic::parse(InternalStatus.tactic_name_);
            }
            InternalStatus.tactic_name_ = NULL;
        }

        /// really starting point of the strategy
        /// test the strategy state
        switch(InternalStatus.StrategyIndex)
        {
        case STRATEGY_JOYSTICK:
            //do_StrategyJoystick();
            break;
        // THE STATEGY_TEST case will just do tatic. the tactic has been set previously
        case STRATEGY_TEST:
            //joystick.close_joystick();

            DoTactics();
            //OldInternalStatus = InternalStatus;
            //continue;
            break;
        case STRATEGY_RUN:
            //joystick.close_joystick();

            //timer.markStartTime();
            if(OldInternalStatus.StrategyIndex!=STRATEGY_RUN)
            {
                ClearGuiTactics();
            }
            // Strategy sets the player's tactics.
            timer.MarkStartTime();
            strategy.run(world, tactics);
            timer.MarkEndTime();
            DoTactics();
            RadioNetCmdSend();

            // Strategy may need some more intensive thinking time.  This
            // needs to be called every frame since otherwise things like
            // goals might get missed.  Some parts of this, though, may not
            // need run at frame rate.
            strategy.think(world);
            OldInternalStatus = InternalStatus;
            strategyGetTimeVal = timer.GetInterval_ms();
            //if(strategyGetTimeVal>10)
//	    {
            //printf("strategyGetTimeVal=%f\r\n",strategyGetTimeVal);
//	    }
            continue;
            break;

        default:
            continue;
            break;
        }
        OldInternalStatus = InternalStatus;
        /// net signal sending sending
        RadioNetCmdSend();
    }
    ClearGuiTactics();
}

void StrategyThread::ClearGuiTactic(int i)
{
    if (gui_tactics[i])
    {
        delete gui_tactics[i];
    }
    gui_tactics[i] = NULL;
}

void StrategyThread::ClearGuiTactics()
{
    for (int i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        ClearGuiTactic(i);
    }
}

// do proper tactics()
void StrategyThread::DoTactics()
{
    // tactics 显示.
    // if nothing is defined in the gui_tactics then goes to tactics
    // it seems that if the gui tactic is not defined, then it mean the tactic will not show on gui ?
    for (int i=0; i<world.n_teammates; i++)
    {
        if (gui_tactics[i])
        {
            sTactics[i] = gui_tactics[i]->name();
        }
        else if (tactics[i])
        {
            sTactics[i] = tactics[i]->name();
        }
        else
        {
            sTactics[i] = "";
        }
    }
    //tactics 运行.
    // excute strategy for each robot

    // pay attention to the work division lu_test
    for (int i=0; i<world.n_teammates; i++)
    {
        if (gui_tactics[i])
        {
            // execute tactics!!!
            //printf("gui_tactics:%s ,%d\n",gui_tactics[i]->name(),i);
            gui_tactics[i]->run(world, i);
/*
            if(gui_tactics[i]->isDone(world,i)==3)//if tactics complete lu_test
            {
                InternalStatus.Status=RUN_STOP;
                OldInternalStatus.Status=RUN_STOP;
            }
*/
        }
        else if (tactics[i])
        {
            tactics[i]->run(world, i);
        }
    }
    // Send all the radio commands.
    //client.Send();
}

void StrategyThread::do_StrategyJoystick()
{
     SerialServer serial_server = SerialServer();
     joystick.open_joystick();
     RobotParamters robot_parameters;
     robot_parameters = ClearRobotParameters(robot_parameters);
     joystick.JoystickControl(&serial_server);

 /*
     robot_parameters.x_velocity = joystick.set_joystick_x_axis();
     robot_parameters.y_velocity = joystick.set_joystick_y_axis();

     WirelessRobot robot = WirelessRobot(robot_parameters);
     robot.set_x_velocity(robot_parameters.x_velocity);
     robot.set_y_velocity(robot_parameters.y_velocity);

     TransparentPackage package;
     QByteArray temp_array;
     temp_array = package.FormByteCommand(temp_array,&robot);

     int fd=package.PortInitialization();
     int temp_size = temp_array.size();
     unsigned char *temp_pointer = new unsigned char(temp_size);
     memcpy(temp_pointer, temp_array.data(), temp_size);*/



//        double vx,vy,vz;
//        int iRobot;
//        if (!joystick.IsOK())
//        {
//                joystick.init();
//                if (!joystick.IsOK())
//                {
//                        return;
//                }
//        }
//        joystick.check();
//        iRobot=iJoystickRobot;
//        if (abs(joystick.data.x)<20)
//        {
//                vx=0;
//        }
//        else
//        {
//                if (joystick.data.x<0)
//                {
//                        vx=joystick.data.x+20;
//                }
//                else
//                {
//                        vx=joystick.data.x-20;
//                }
//        }
//        vx=-vx*2;
//        if (abs(joystick.data.y)<20)
//        {
//                vy=0;
//        }
//        else
//        {
//                if (joystick.data.y<0)
//                {
//                        vy=joystick.data.y+20;
//                }
//                else
//                {
//                        vy=joystick.data.y-20;
//                }
//        }
//        vy=-vy*2;
//        vz=joystick.data.z;
//        if (abs(joystick.data.z)<20)
//        {
//                vz=0;
//        }
//        else
//        {
//                if (joystick.data.z<0)
//                {
//                        vz=joystick.data.z+20;
//                }
//                else
//                {
//                        vz=joystick.data.z-20;
//                }
//        }
//        vz=vz;
//        bool kick_on;
//        bool chipkick_on;
//        static int dribbler_speed=0;
//        static int kickpower=0;		//力度
//        //-----------------------------------------------
//        //踢球挑球
//        kick_on=joystick.data.b[5];
//        chipkick_on=joystick.data.b[7];
//        //-----------------------------------------------
//        //带球电机
//        dribbler_speed=8;
//        if (joystick.data.b[8])
//        {
//                kickpower=0;
//                dribbler_speed=0;
//                for (int i=0; i<4; i++)
//                {
//                        if (joystick.data.b[i])
//                        {
//                                dribbler_speed+=1<<(i);
//                                kickpower+=1<<(i);
//                        }
//                }
//                kickpower*=4;
//                if (kickpower)	kickpower+=3;
//        }
//        if (joystick.data.b[4])
//        {
//                dribbler_speed=-dribbler_speed;
//        }
//        else if (!joystick.data.b[6])
//        {
//                dribbler_speed=0;
//        }
//        //cout<<"x:"<<vy<<"  y:"<<vx<<"  z:"<<vz<<endl;
//        static int _framecount=0;
//        _framecount++;
//        cout<<"framecount: "<<_framecount<<endl;
//        static unsigned long tickstart=0;
//        unsigned long tickcurrent;
//        while(true)
//        {
//                tickcurrent=GetTickCount();
//                if(tickcurrent-tickstart>15)
//                {
//                        tickstart=tickcurrent;
//                        break;
//                }
//        }
//        world.go(iRobot,vx,vy,vz,
//                 (kick_on||chipkick_on)?kickpower:0,
//                 chipkick_on,dribbler_speed);
}

bool StrategyThread::SkipVisionWait()
{
    //检查是否处于游戏操作杆控制
    if(thread_terminated_)return true;
    //runstatusMutex.lock();
    InternalStatus = StatusOnGUI;
    //runstatusMutex.unlock();
//    if(InternalStatus.StrategyIndex==STRATEGY_JOYSTICK)
//    {
//        return true;
//    }
    return false;
}

//如果超时返回true
bool StrategyThread::do_refbox_recv()
{
    int count;
    const int bufsize=100;
    char buf[bufsize];
    count=refbox_client.Receive ( buf ,bufsize);
    if (count==6)
    {
    }
    return true;
}

//如果超时返回true
bool StrategyThread::do_vision_recv()
{
    static int times_called = 0;
    static int frames_processed = 0;

    //接收信号，更新显示
    SSL_WrapperPacket packet;

    //获取接收信息
    int i=0;
    timer.MarkStartTime();
    VisionReceiveSignal.acquire();
    if(SkipVisionWait())return false;
    timer.MarkEndTime();
    VisionUpdate(packet);

    display_set_mutex.lock();

//    VisionInfo testVisionInfo;
//    testVisionInfo.sPlay = QString("Test");
//    world.vision_info_ = testVisionInfo;

//    qDebug() << "--------------strategy thread: display_update_mutex locked!!!";
    world.vision_info_ = vision_info;	    // Bug here! No bug now, lu_test
//    qDebug() << "strategy_thread: ball pos: " << world.vision_info_.Balls[0].pos.x << ", " << world.vision_info_.Balls[0].pos.y;
//qDebug() << "vision_info ball: " << world.vision_info_.Balls[0].pos.x << ", " << world.vision_info_.Balls[0].pos.y;



    //times_called++;
    //更新
    world.update(world.vision_info_);

    display_set_mutex.unlock();

    //丢帧检测
    //if (print_frames_missing && frames_processed >= 150) {
    //  if(frames_processed < times_called){
    //    fprintf(stderr, "Frames Missed: %5g/sec.\n",
    //     (frames_processed - times_called) / 5.0);
    //  }
    //  frames_processed = times_called = 0;
    //}
    visionGetTimeVal = timer.GetInterval_ms();

#if 0
    if(visionGetTimeVal>=17)
    {
        //timer.markStartTime();
        printf("%d visionGetTimeVal=%f\r\n",i++,visionGetTimeVal);
    }
#endif
    return false;
}

void StrategyThread::terminate()
{
    thread_terminated_ = true;
    VisionReceiveSignal.release();
}

/// determine whether the strategy is still the old one
/// if it is, then return false else return false
inline bool StrategyThread::IsStrategyOld()
{
    if(!OldInternalStatus.tactic_name_)
    {
        return false;
    }
    else
    {
        return !strcmp(InternalStatus.tactic_name_,OldInternalStatus.tactic_name_);
    }
}
