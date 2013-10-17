#include "net_vision_send_thread.h"
#include "user_interface/field_related/field_robot.h"
#include  "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "user_interface/field_related/field_global_function.h"
#include "net/thread_tools/field_timer.h"


VisionSendThread::VisionSendThread()
{
}

VisionSendThread::VisionSendThread (MainWindow *pStrategy )
{
    m_Strategy = pStrategy;
    m_Simulator.Initialize();// .Initialize();
}

static int iReceived0=0;
static int iReceived1=0;

const double posError=7;
const double angleError=3*M_PI/180;

double GetConfRand()
{
    double dRet;
    dRet=(double) rand() / (double) RAND_MAX;
    return dRet;
}

double GetPosRand(double dBase)
{
    double dRet;
    dRet=dBase*(GetConfRand()-0.5);
    return dRet;
}

void VisionSendThread::run()
{
    brun=true;
    SSL_DetectionFrame detection_;/* = packet->detection_();*/
    VisionServer Server;
    //    RUNSTATUS tRunStatus;
    int frameCount;
    if (!Server.Open ( ))
    {
	printf("Server Open failed!\r\n");
	return;
    }

    SSL_DetectionBall* pball;
    SSL_DetectionRobot* probot;
    double curtime;
    unsigned long tick,tickperiod,oldtick=GetTickCount();
    int i,j;

    const int FilterBufMax=50;
    double OldPos[FilterBufMax][5];
    bool bFirst=true;
    double dSum[5];
    int iCur=0;
    bool bNeedProc;
    int  iCamera;

    while (brun)
    {
	tick=GetTickCount();
	tickperiod=tick-oldtick;
	if(tickperiod<1)
	{
	    usleep(1);
	    continue;
	}
	//printf("period=%d\r\n",tickperiod);
	oldtick=tick;

	//显示策略运行周期
	//printf("Tick=%d\r\n",tickperiod);
	//        //检查运行状态--------------------------------
	//                tRunStatus=CurRunStatus;
	//                switch (tRunStatus.Status)
	//                {
	//                case Run_Pause:
	//                    continue;
	//                    break;
	//                case Run_Step:
	//                    CurRunStatus.Status=Run_Pause;
	//                    break;
	//                case Run_Stop:
	//                    continue;
	//                    break;
	//                }
	frameCount=m_Simulator.Tick();

	//首先发送后半场信息
	for(iCamera=0;iCamera<2;iCamera++)
	{
	    if(!bEnCamera[iCamera])continue;

	    //向策略服务器发送信息
	    detection_.Clear();
	    detection_.clear_balls();
	    detection_.clear_robots_blue();
	    detection_.clear_robots_yellow();
	    detection_.set_frame_number(frameCount);


	    curtime=GetTimeSec();
	    detection_.set_t_capture(curtime);
	    curtime=GetTimeSec();
	    detection_.set_t_sent(curtime);




	    detection_.set_camera_id(iCamera);
	    //更新屏幕显示
	    display_update_mutex.lock();
	    //清除所有球信息
	    for (i=1; i<MAX_BALLS; i++)
	    {
		vision_info.Balls[i].conf=0.0;
	    }
	    vision_info.Balls[0].conf=1;
	    vision_info.Balls[0].pos=m_Simulator.ball.pos;
	    bNeedProc=false;
	    if(iCamera==0)
	    {
		if(m_Simulator.ball.pos.x<300)bNeedProc=true;
	    }
	    else
	    {
		if(m_Simulator.ball.pos.x>-300)bNeedProc=true;
	    }
	    if(bNeedProc)
	    {
		pball=detection_.add_balls();
		//pball->set_area ( 20 );
		if(bEnBallNoise)
		{
		    pball->set_confidence(GetConfRand());
		    pball->set_x(vision_info.Balls[0].pos.x+GetPosRand(posError));
		    pball->set_y(vision_info.Balls[0].pos.y+GetPosRand(posError));
		}
		else
		{
		    pball->set_confidence(vision_info.Balls[0].conf);
		    pball->set_x(vision_info.Balls[0].pos.x);
		    pball->set_y(vision_info.Balls[0].pos.y);
		}
		//pball->set_z(1);
		pball->set_pixel_x(12);
		pball->set_pixel_y(12);
	    }
	    //清除所有机器人信息
	    for (i=0; i<MAX_ROBOTS; i++)
	    {
		//处理蓝队机器人
		vision_info.Robots[teamBlue][i].conf=m_Simulator.blue_robots[i].conf;
		vision_info.Robots[teamBlue][i].pos=m_Simulator.blue_robots[i].pos.p;
		vision_info.Robots[teamBlue][i].orientation=m_Simulator.blue_robots[i].pos.dir;

		bNeedProc=false;
		if(iCamera==0)
		{
		    if(m_Simulator.blue_robots[i].pos.p.x<300)bNeedProc=true;
		}
		else
		{
		    if(m_Simulator.blue_robots[i].pos.p.x>-300)bNeedProc=true;
		}
		if (bNeedProc && vision_info.Robots[teamBlue][i].conf>0.001)
		{
		    probot=detection_.add_robots_blue();
		    if(bEnBlueRobotNoise)
		    {
			probot->set_confidence(GetConfRand());
			probot->set_orientation(m_Simulator.blue_robots[i].pos.dir+GetPosRand(angleError));
			probot->set_x(m_Simulator.blue_robots[i].pos.p.x+GetPosRand(posError));
			probot->set_y(m_Simulator.blue_robots[i].pos.p.y+GetPosRand(posError));
		    }
		    else
		    {
			probot->set_confidence(m_Simulator.blue_robots[i].conf);
			probot->set_orientation(m_Simulator.blue_robots[i].pos.dir);
			probot->set_x(m_Simulator.blue_robots[i].pos.p.x);
			probot->set_y(m_Simulator.blue_robots[i].pos.p.y);
		    }
		    if(bEnBigRobotID)
		    {
			if(i==0)
			{
			}
			else
			{
			    probot->set_robot_id(i+5);
			}
		    }
		    else
		    {
			probot->set_robot_id(i);
		    }
		    probot->set_pixel_x(10);
		    probot->set_pixel_y(10);
		}

		//处理黄队机器人
		vision_info.Robots[teamYellow][i].conf=m_Simulator.yellow_robots[i].conf;
		vision_info.Robots[teamYellow][i].pos=m_Simulator.yellow_robots[i].pos.p;
		vision_info.Robots[teamYellow][i].orientation=m_Simulator.yellow_robots[i].pos.dir;
		bNeedProc=false;
		if(iCamera==0)
		{
		    if(m_Simulator.yellow_robots[i].pos.p.x<300)bNeedProc=true;
		}
		else
		{
		    if(m_Simulator.yellow_robots[i].pos.p.x>-300)bNeedProc=true;
		}

		if (bNeedProc && vision_info.Robots[teamYellow][i].conf>0.001)
		{
		    probot=detection_.add_robots_yellow();
		    if(bEnYellowRobotNoise)
		    {
			probot->set_confidence(GetConfRand());
			probot->set_orientation(m_Simulator.yellow_robots[i].pos.dir+GetPosRand(angleError));
			probot->set_x(m_Simulator.yellow_robots[i].pos.p.x+GetPosRand(posError));
			probot->set_y(m_Simulator.yellow_robots[i].pos.p.y+GetPosRand(posError));
		    }
		    else
		    {
			probot->set_confidence(m_Simulator.yellow_robots[i].conf);
			probot->set_orientation(m_Simulator.yellow_robots[i].pos.dir);
			probot->set_x(m_Simulator.yellow_robots[i].pos.p.x);
			probot->set_y(m_Simulator.yellow_robots[i].pos.p.y);
		    }
		}
	    }
	    display_update_mutex.unlock();
	    if (!detection_.IsInitialized())
	    {
		//qDebug()<<"detection_ initialize fail!";
	    }
	    else
	    {
		Server.Send(detection_);
	    }
	}
    }
}

