#include "net/net_vision_receive_thread.h"
#include "user_interface/field_related/field_item.h"
#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"


extern double GetTimeSec();
extern QString sTactics[5];

QSemaphore VisionReceiveSignal;

VisionReceiveThread::VisionReceiveThread ( FieldView *_field_view )
{
    strategyGraphicsView = _field_view;
    thread_terminated = false;
}

void VisionReceiveThread::Terminate()
{
    thread_terminated = true;
}

void VisionReceiveThread::run()
{
    SSL_WrapperPacket packet;
    VisionClient client;

    if(!client.Open ())
    {
	printf("Client Open failed!\r\n");
	return;
    }
    while ( !thread_terminated )
    {
	if ( client.Receive ( packet ) )
	{
	    if(thread_terminated)break;
	    //处理数据

        VisionUpdate(packet);
	}
	if(thread_terminated)
	{
	    break;
	}
    }
}

int VisionUpdate(const SSL_WrapperPacket &f)
{
    MyVector2d pos;
    double orientation;
    int iCurIndex,i;
    double confsum,conf;
    bool bFound;

    static int oldcamera_id=-1;
    int camera_id;


    // this flag will show how will we update 0: do not update 1: update with one comera condition
    // 2: update with two comera condition
    int update_mode=0;

// if the packege has a detection
    if ( f.has_detection() )
    {
	// transfer the infomation to the new ssl detection frame
	SSL_DetectionFrame detection = f.detection();
	// store time
	vision_info.time = detection.t_sent();
	// store camera id
	camera_id = detection.camera_id();

//	qDebug() << "camera_id: " << camera_id;

	// check whether camera_id is bigger than 1 or not than I will know about the
	// how many cameras are there camera id (0,1)
	if(camera_id>1)
	{
	    printf("cameraid=%d cannot bigger than 1! than it means there are more than 2 camera\r\n",camera_id);
	    return 0;
	}
	// this flag decides whether should I update or not
	bool update_flag=false;
	display_update_mutex.lock();
    //
	if(camera_id==oldcamera_id)
	{
        qDebug() << "camera_id==oldcamera_id: ";

	    // hacked by Bin, may improve later
#if 0
	    //只有一台摄像机数据的情况
	    update_mode=1;
        //清除所有球信息
	    for(i=0;i<MAX_BALLS;i++)
	    {
		vision_info.Balls[i].conf=0.0;

		VInfoRaw.BallInfos[0][i].conf=0.0;
		VInfoRaw.BallInfos[1][i].conf=0.0;
	    }
	    //清除所有机器人信息
	    for(i=0;i<MAX_ROBOTS;i++)
	    {
		vision_info.Robots[teamBlue][i].conf=0.0;
		vision_info.Robots[teamYellow][i].conf=0.0;

		VInfoRaw.RobotInfos[0][teamBlue][i].conf=0.0;
		VInfoRaw.RobotInfos[1][teamBlue][i].conf=0.0;
		VInfoRaw.RobotInfos[0][teamYellow][i].conf=0.0;
		VInfoRaw.RobotInfos[1][teamYellow][i].conf=0.0;
	    }
#endif
	}
	else if(camera_id==1)
	{
        qDebug() << "camera_id==1";
	    //有两台摄像机数据的情况
	    update_mode=2;

	    // hacked by Bin, may improve later
#if 0
	    //清除1号摄像机所有球信息
	    for(i=0;i<MAX_BALLS;i++)
	    {
		vision_info.Balls[i].conf=0.0;
		VInfoRaw.BallInfos[1][i].conf=0.0;
	    }

	    //清除1号摄像机所有机器人信息
	    for(i=0;i<MAX_ROBOTS;i++)
	    {
		vision_info.Robots[teamBlue][i].conf=0.0;
		vision_info.Robots[teamYellow][i].conf=0.0;

		VInfoRaw.RobotInfos[1][teamBlue][i].conf=0.0;
		VInfoRaw.RobotInfos[1][teamYellow][i].conf=0.0;
	    }
#endif
	}
	else
	{

        qDebug() << "No need to update, for recording purpose only";
	    //不需要更新，只是记录的情况
	    update_mode=0;


	    //清除所有球信息
	    for(i=0; i<MAX_BALLS; i++)
	    {
		VInfoRaw.BallInfos[0][i].conf=0.0;
		VInfoRaw.BallInfos[1][i].conf=0.0;
	    }
	    //清除所有机器人信息
	    for(i=0; i<MAX_ROBOTS; i++)
	    {
		VInfoRaw.RobotInfos[0][teamBlue][i].conf=0.0;
		VInfoRaw.RobotInfos[1][teamBlue][i].conf=0.0;
		VInfoRaw.RobotInfos[0][teamYellow][i].conf=0.0;
		VInfoRaw.RobotInfos[1][teamYellow][i].conf=0.0;
	    }

	}
	oldcamera_id = camera_id;
	//计算多个球的信息
	int balls_n = detection.balls_size();
	SSL_DetectionBall ball;

	for ( i = 0; i < balls_n; i++ )
	{

	    ball = detection.balls ( i );

//        qDebug() << "ball info: " << ball.x() << ", " << ball.y();

	    conf = ball.confidence();
	    if ( conf == 0.0 )continue;
	    pos.set(ball.x(),ball.y());
	    bFound = false;
	    //寻找球的索引
	    for(iCurIndex=0; iCurIndex<MAX_BALLS; iCurIndex++)
	    {
		if(VInfoRaw.BallInfos[0][iCurIndex].conf==0.0 && VInfoRaw.BallInfos[1][iCurIndex].conf==0.0)
		{
		    //没有找到球
		    VInfoRaw.BallInfos[camera_id][iCurIndex].pos=pos;
		    VInfoRaw.BallInfos[camera_id][iCurIndex].conf=conf;
		    bFound=true;
		    break;
		}

		//两个球的距离应该小于球的半径，即21mm
		if((VInfoRaw.BallInfos[1-camera_id][iCurIndex].pos-pos).length()<21.0)
		{
		    //两个球信息足够近
		    VInfoRaw.BallInfos[camera_id][iCurIndex].pos=pos;
		    VInfoRaw.BallInfos[camera_id][iCurIndex].conf=conf;
		    bFound=true;
		    break;
		}
	    }

//	    qDebug() << "update_mode: " << update_mode << ", bFound: " << bFound;
	    // hacked by Bin, may improve later
//	    if(update_mode!=0 && bFound)
	    if(bFound)
	    {
//		qDebug() << "update_mode!=0 && bFound";
		confsum = VInfoRaw.BallInfos[0][iCurIndex].conf+VInfoRaw.BallInfos[1][iCurIndex].conf;
		pos = VInfoRaw.BallInfos[0][iCurIndex].pos*VInfoRaw.BallInfos[0][iCurIndex].conf;
		pos += VInfoRaw.BallInfos[1][iCurIndex].pos*VInfoRaw.BallInfos[1][iCurIndex].conf;
		pos /= confsum;

//		qDebug() << "pos: " << pos.x << ", " << pos.y << ", iCurIndex: " << iCurIndex;

		//更新球对象
		vision_info.Balls[iCurIndex].pos = pos;
		vision_info.Balls[iCurIndex].conf = max(VInfoRaw.BallInfos[0][iCurIndex].conf, VInfoRaw.BallInfos[1][iCurIndex].conf);
		update_flag=true;
	    }
	}

//	qDebug() << "vision_info ball: " << vision_info.Balls[0].pos.x << ", " << vision_info.Balls[0].pos.y;

	//计算机器人信息
	SSL_DetectionRobot robot_i;
	int id;
	int robots_n;

    //黄色机器人
    robots_n = detection.robots_yellow_size();
    for ( int i = 0; i < robots_n; i++ )
    {
        robot_i = detection.robots_yellow ( i );
        conf=robot_i.confidence();
//        qDebug() << "yellow "<<i<<" :"<< robot_i.x() << ", " << robot_i.y() << ", conf: " << conf;

        if ( conf == 0.0 )continue;
        pos.set(robot_i.x(),robot_i.y());

        if(robot_i.has_robot_id())
        {
        id=robot_i.robot_id();
        if(id>=MAX_ROBOT_ID)
        {
            printf("id=%d,MAXID=%d\r\n",id,MAX_ROBOT_ID-1);
            continue;
        }
        VInfoRaw.RobotInfos[camera_id][teamYellow][id].pos=pos;
        VInfoRaw.RobotInfos[camera_id][teamYellow][id].conf=conf;
        VInfoRaw.RobotInfos[camera_id][teamYellow][id].orientation=robot_i.orientation();
   //     if(update_mode!=0) lu_test solve the bug!!!
        {
            confsum=VInfoRaw.RobotInfos[0][teamYellow][id].conf+VInfoRaw.RobotInfos[1][teamYellow][id].conf;
            pos =VInfoRaw.RobotInfos[0][teamYellow][id].pos*VInfoRaw.RobotInfos[0][teamYellow][id].conf;
            pos+=VInfoRaw.RobotInfos[1][teamYellow][id].pos*VInfoRaw.RobotInfos[1][teamYellow][id].conf;
            pos/=confsum;
            orientation =VInfoRaw.RobotInfos[0][teamYellow][id].orientation*VInfoRaw.RobotInfos[0][teamYellow][id].conf;
            orientation+=VInfoRaw.RobotInfos[1][teamYellow][id].orientation*VInfoRaw.RobotInfos[1][teamYellow][id].conf;
            orientation/=confsum;

            vision_info.Robots[teamYellow][id].pos=pos;
            vision_info.Robots[teamYellow][id].orientation=orientation;
            vision_info.Robots[teamYellow][id].conf=max(VInfoRaw.RobotInfos[0][teamYellow][id].conf,VInfoRaw.RobotInfos[1][teamYellow][id].conf);
            if(world.color_==teamYellow)
            {
            vision_info.Robots[teamYellow][id].Tactic=sTactics[i];
            }
            else
            {
            vision_info.Robots[teamYellow][id].Tactic="";
            }
            update_flag=true;
        }
        }
        else
        {
        id = NA;
        //通过距离寻找同一个机器人
        }
    }

	//蓝色机器人
	robots_n = detection.robots_blue_size();
	for ( int i = 0; i < robots_n; i++ )
	{
	    robot_i = detection.robots_blue ( i );
	    conf = robot_i.confidence();
//        qDebug() << "blue "<<i<<" :"<< robot_i.x() << ", " << robot_i.y() << ", conf: " << conf;
	    // correct!
	    if ( conf == 0.0 )continue;
	    pos.set(robot_i.x(),robot_i.y());

	    if(robot_i.has_robot_id())
	    {
		id=robot_i.robot_id();
		if(id>=MAX_ROBOT_ID)
		{
		    printf("id=%d,MAXID=%d\r\n",id,MAX_ROBOT_ID-1);
		    continue;
		}
		VInfoRaw.RobotInfos[camera_id][teamBlue][id].pos=pos;
		VInfoRaw.RobotInfos[camera_id][teamBlue][id].conf=conf;
		VInfoRaw.RobotInfos[camera_id][teamBlue][id].orientation=robot_i.orientation();
		// hacked by Bin, may improve later
//		if(update_mode!=0)
		{
		    confsum=VInfoRaw.RobotInfos[0][teamBlue][id].conf+VInfoRaw.RobotInfos[1][teamBlue][id].conf;
		    pos =VInfoRaw.RobotInfos[0][teamBlue][id].pos*VInfoRaw.RobotInfos[0][teamBlue][id].conf;
		    pos+=VInfoRaw.RobotInfos[1][teamBlue][id].pos*VInfoRaw.RobotInfos[1][teamBlue][id].conf;
		    pos/=confsum;
		    orientation =VInfoRaw.RobotInfos[0][teamBlue][id].orientation*VInfoRaw.RobotInfos[0][teamBlue][id].conf;
		    orientation+=VInfoRaw.RobotInfos[1][teamBlue][id].orientation*VInfoRaw.RobotInfos[1][teamBlue][id].conf;
		    orientation/=confsum;

		    vision_info.Robots[teamBlue][id].pos=pos;
//		    qDebug() << "robot_i: " << vision_info.Robots[teamBlue][id].pos.x << ", " << vision_info.Robots[teamBlue][id].pos.y;

		    vision_info.Robots[teamBlue][id].orientation=orientation;
		    vision_info.Robots[teamBlue][id].conf=max(VInfoRaw.RobotInfos[0][teamBlue][id].conf,VInfoRaw.RobotInfos[1][teamBlue][id].conf);
		    if(world.color_==teamBlue)
		    {
			vision_info.Robots[teamBlue][id].Tactic=sTactics[i];
		    }
		    else
		    {
			vision_info.Robots[teamBlue][id].Tactic="";
		    }
		    update_flag=true;
		}
	    }
	    else
	    {
		id = NA;
		//通过距离寻找同一个机器人

	    }
	}


	display_update_mutex.unlock();
	if(update_flag)
	{
	    //通知显示线程和策略线程
	    VisionReceiveSignal.release();
	}
    }
    return update_mode;
}
