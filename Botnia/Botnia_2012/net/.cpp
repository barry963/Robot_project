
//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    ClientThreading.cpp
  \brief   C++ Implementation: ViewUpdateThread
  \author  Joydeep Biswas, Stefan Zickler (C) 2009
*/
//========================================================================







#include "field_netvision_thread.h"
#include "../field_item.h"
#include "../field_world_state/field_world.h"


extern double GetTimeSec();
extern QString sTactics[5];

QSemaphore VisionReceiveSignal;

VisionReceiveThread::VisionReceiveThread ( FieldView *_field_view )
{
    field_view = _field_view;
    shutdownView = false;
}

VisionReceiveThread::VisionReceiveThread ( GLSoccerView *_ai_view )
{
    ai_view = _ai_view;
    shutdownView = false;
}

void VisionReceiveThread::Terminate()
{
    shutdownView = true;
}

void VisionReceiveThread::run()
{
    SSL_WrapperPacket packet;
    RoboCupSSLClient client;

    if(!client.open ())
    {
        printf("Client Open failed!\r\n");
        return;
    }
    while ( !shutdownView )
    {
        if ( client.receive ( packet ) )
        {
            SSL_DetectionFrame detection = packet.detection();
            if(shutdownView)break;
            ai_view->updateDetection(detection);
        }
        if(shutdownView)
        {
            break;
        }
    }
}



//int VisionUpdate(const SSL_WrapperPacket &f)
//{
//    MyVector2d pos;
//    double orientation;
//    int iCurIndex,i;
//    double confsum,conf;
//    bool bFound;

//    static int oldcamera_id=-1;
//    int camera_id;
//    int iUpdateMode=0;
//    if ( f.has_detection() )
//    {
//        SSL_DetectionFrame detection = f.detection();
//        VInfo.time = detection.t_sent();
//        camera_id=detection.camera_id();
//        if(camera_id>1)
//        {
//            printf("cameraid=%d cannot bigger than 1!\r\n",camera_id);
//            return 0;
//        }
////        BOOL bUpdate=false;
//        bool bUpdate=false;
//        DispUpdateMutex.lock();
//        if(camera_id==oldcamera_id)
//        {
//            //只有一台摄像机数据的情况
//            iUpdateMode=1;
//            //清除所有球信息
//            for(i=0;i<MAX_BALLS;i++)
//            {
//                VInfo.Balls[i].conf=0.0;

//                VInfoRaw.BallInfos[0][i].conf=0.0;
//                VInfoRaw.BallInfos[1][i].conf=0.0;
//            }
//            //清除所有机器人信息
//            for(i=0;i<MAX_ROBOTS;i++)
//            {
//                VInfo.Robots[teamBlue][i].conf=0.0;
//                VInfo.Robots[teamYellow][i].conf=0.0;

//                VInfoRaw.RobotInfos[0][teamBlue][i].conf=0.0;
//                VInfoRaw.RobotInfos[1][teamBlue][i].conf=0.0;
//                VInfoRaw.RobotInfos[0][teamYellow][i].conf=0.0;
//                VInfoRaw.RobotInfos[1][teamYellow][i].conf=0.0;
//            }
//        }
//        else if(camera_id==1)
//        {
//            //有两台摄像机数据的情况
//            iUpdateMode=2;
//            //清除1号摄像机所有球信息
//            for(i=0;i<MAX_BALLS;i++)
//            {
//                VInfo.Balls[i].conf=0.0;

//                VInfoRaw.BallInfos[1][i].conf=0.0;
//            }
//            //清除1号摄像机所有机器人信息
//            for(i=0;i<MAX_ROBOTS;i++)
//            {
//                VInfo.Robots[teamBlue][i].conf=0.0;
//                VInfo.Robots[teamYellow][i].conf=0.0;

//                VInfoRaw.RobotInfos[1][teamBlue][i].conf=0.0;
//                VInfoRaw.RobotInfos[1][teamYellow][i].conf=0.0;
//            }
//        }
//        else
//        {
//            //不需要更新，只是记录的情况
//            iUpdateMode=0;
//            //清除所有球信息
//            for(i=0;i<MAX_BALLS;i++)
//            {
//                VInfoRaw.BallInfos[0][i].conf=0.0;
//                VInfoRaw.BallInfos[1][i].conf=0.0;
//            }
//            //清除所有机器人信息
//            for(i=0;i<MAX_ROBOTS;i++)
//            {
//                VInfoRaw.RobotInfos[0][teamBlue][i].conf=0.0;
//                VInfoRaw.RobotInfos[1][teamBlue][i].conf=0.0;
//                VInfoRaw.RobotInfos[0][teamYellow][i].conf=0.0;
//                VInfoRaw.RobotInfos[1][teamYellow][i].conf=0.0;
//            }
//        }
//        oldcamera_id=camera_id;
//        //计算多个球的信息
//        int balls_n = detection.balls_size();
//        SSL_DetectionBall ball;

//        for ( i = 0; i < balls_n; i++ )
//        {
//            ball = detection.balls ( i );
//            conf=ball.confidence();
//            if ( conf == 0.0 )continue;
//            pos.set(ball.x(),ball.y());
//            bFound=false;
//            //寻找球的索引
//            for(iCurIndex=0;iCurIndex<MAX_BALLS;iCurIndex++)
//            {
//                if(VInfoRaw.BallInfos[0][iCurIndex].conf==0.0 && VInfoRaw.BallInfos[1][iCurIndex].conf==0.0)
//                {
//                    //没有找到球
//                    VInfoRaw.BallInfos[camera_id][iCurIndex].pos=pos;
//                    VInfoRaw.BallInfos[camera_id][iCurIndex].conf=conf;
//                    bFound=true;
//                    break;
//                }
//                //两个球的距离应该小于球的半径，即21mm
//                if((VInfoRaw.BallInfos[1-camera_id][iCurIndex].pos-pos).length()<21.0)
//                {
//                    //两个球信息足够近
//                    VInfoRaw.BallInfos[camera_id][iCurIndex].pos=pos;
//                    VInfoRaw.BallInfos[camera_id][iCurIndex].conf=conf;
//                    bFound=true;
//                    break;
//                }
//            }
//            if(iUpdateMode!=0 && bFound)
//            {
//                confsum=VInfoRaw.BallInfos[0][iCurIndex].conf+VInfoRaw.BallInfos[1][iCurIndex].conf;
//                pos=VInfoRaw.BallInfos[0][iCurIndex].pos*VInfoRaw.BallInfos[0][iCurIndex].conf;
//                pos+=VInfoRaw.BallInfos[1][iCurIndex].pos*VInfoRaw.BallInfos[1][iCurIndex].conf;
//                pos/=confsum;

//                //更新球对象
//                VInfo.Balls[iCurIndex].pos=pos;
//                VInfo.Balls[iCurIndex].conf=max(VInfoRaw.BallInfos[0][iCurIndex].conf,VInfoRaw.BallInfos[1][iCurIndex].conf);
//                bUpdate=true;
//            }
//        }

//        //计算机器人信息
//        SSL_DetectionRobot robot_i;
//        int id;
//        int robots_n;
//        //蓝色机器人
//        robots_n = detection.robots_blue_size();
//        for ( int i = 0; i < robots_n; i++ )
//        {
//            robot_i = detection.robots_blue ( i );
//            conf=robot_i.confidence();
//            if ( conf == 0.0 )continue;
//            pos.set(robot_i.x(),robot_i.y());
//            if(robot_i.has_robot_id())
//            {
//                id=robot_i.robot_id();
//                if(id>=MAX_ROBOT_ID)
//                {
//                    printf("id=%d,MAXID=%d\r\n",id,MAX_ROBOT_ID-1);
//                    continue;
//                }
//                VInfoRaw.RobotInfos[camera_id][teamBlue][id].pos=pos;
//                VInfoRaw.RobotInfos[camera_id][teamBlue][id].conf=conf;
//                VInfoRaw.RobotInfos[camera_id][teamBlue][id].orientation=robot_i.orientation();
//                if(iUpdateMode!=0)
//                {
//                    confsum=VInfoRaw.RobotInfos[0][teamBlue][id].conf+VInfoRaw.RobotInfos[1][teamBlue][id].conf;
//                    pos =VInfoRaw.RobotInfos[0][teamBlue][id].pos*VInfoRaw.RobotInfos[0][teamBlue][id].conf;
//                    pos+=VInfoRaw.RobotInfos[1][teamBlue][id].pos*VInfoRaw.RobotInfos[1][teamBlue][id].conf;
//                    pos/=confsum;
//                    orientation =VInfoRaw.RobotInfos[0][teamBlue][id].orientation*VInfoRaw.RobotInfos[0][teamBlue][id].conf;
//                    orientation+=VInfoRaw.RobotInfos[1][teamBlue][id].orientation*VInfoRaw.RobotInfos[1][teamBlue][id].conf;
//                    orientation/=confsum;

//                    VInfo.Robots[teamBlue][id].pos=pos;
//                    VInfo.Robots[teamBlue][id].orientation=orientation;
//                    VInfo.Robots[teamBlue][id].conf=max(VInfoRaw.RobotInfos[0][teamBlue][id].conf,VInfoRaw.RobotInfos[1][teamBlue][id].conf);
//                    if(world.color==teamBlue)
//                    {
////                        VInfo.Robots[teamBlue][id].Tactic=sTactics[i];
//                    }
//                    else
//                    {
////                        VInfo.Robots[teamBlue][id].Tactic="";
//                    }
//                    bUpdate=true;
//                }
//            }
//            else
//            {
//                id = NA;
//                //通过距离寻找同一个机器人

//            }
//        }

//        //黄色机器人
//        robots_n = detection.robots_yellow_size();
//        for ( int i = 0; i < robots_n; i++ )
//        {
//            robot_i = detection.robots_yellow ( i );
//            conf=robot_i.confidence();
//            if ( conf == 0.0 )continue;
//            pos.set(robot_i.x(),robot_i.y());
//            if(robot_i.has_robot_id())
//            {
//                id=robot_i.robot_id();
//                if(id>=MAX_ROBOT_ID)
//                {
//                    printf("id=%d,MAXID=%d\r\n",id,MAX_ROBOT_ID-1);
//                    continue;
//                }
//                VInfoRaw.RobotInfos[camera_id][teamYellow][id].pos=pos;
//                VInfoRaw.RobotInfos[camera_id][teamYellow][id].conf=conf;
//                VInfoRaw.RobotInfos[camera_id][teamYellow][id].orientation=robot_i.orientation();
//                if(iUpdateMode!=0)
//                {
//                    confsum=VInfoRaw.RobotInfos[0][teamYellow][id].conf+VInfoRaw.RobotInfos[1][teamYellow][id].conf;
//                    pos =VInfoRaw.RobotInfos[0][teamYellow][id].pos*VInfoRaw.RobotInfos[0][teamYellow][id].conf;
//                    pos+=VInfoRaw.RobotInfos[1][teamYellow][id].pos*VInfoRaw.RobotInfos[1][teamYellow][id].conf;
//                    pos/=confsum;
//                    orientation =VInfoRaw.RobotInfos[0][teamYellow][id].orientation*VInfoRaw.RobotInfos[0][teamYellow][id].conf;
//                    orientation+=VInfoRaw.RobotInfos[1][teamYellow][id].orientation*VInfoRaw.RobotInfos[1][teamYellow][id].conf;
//                    orientation/=confsum;

//                    VInfo.Robots[teamYellow][id].pos=pos;
//                    VInfo.Robots[teamYellow][id].orientation=orientation;
//                    VInfo.Robots[teamYellow][id].conf=max(VInfoRaw.RobotInfos[0][teamYellow][id].conf,VInfoRaw.RobotInfos[1][teamYellow][id].conf);
//                    if(world.color==teamYellow)
//                    {
////                        VInfo.Robots[teamYellow][id].Tactic=sTactics[i];
//                    }
//                    else
//                    {
////                        VInfo.Robots[teamYellow][id].Tactic="";
//                    }
//                    bUpdate=true;
//                }
//            }
//            else
//            {
//                id = NA;
//                //通过距离寻找同一个机器人
//            }
//        }
//        DispUpdateMutex.unlock();
//        if(bUpdate)
//        {
//            //通知显示线程和策略线程
//            VisionReceiveSignal.release();
//        }
//    }
//    return iUpdateMode;
//}
