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

/**    @file    - %FILENAME%
  *
  *    @class   - %CLASS%
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 10/30/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#include "net_webserver_send_thread.h"
#include "net/net_radio_server.h"
#include "net/web_data_model/webdatamodel.h"




QSemaphore NetWebserverSendRequest(5);
QSemaphore NetWebserverSendOk;
QMutex NetWebserverSendMutex;

SSL_RadioFrame NetWebserverCmds;

NetWebserverSendThread::NetWebserverSendThread(){
    shutdownView = false;
    qDebug() << "constructor";
}


NetWebserverSendThread::NetWebserverSendThread ( FieldView *_field_view ){
    strategyGraphicsView = _field_view;
    shutdownView = false;
}

void NetWebserverSendThread::run(){


//    RadioServer Server;
//    if (!Server.Open())
//    {
//        printf("Server Open failed!\r\n");
//        return;
//    }
//
//    int size;
//    while (true)
//    {
//
//        qDebug() << "Available: " << NetWebserverSendRequest.available();
//        NetWebserverSendRequest.acquire();
//        if (shutdownView)break;
//        NetWebserverSendMutex.lock();
//        NetWebserverCmds.set_time_stamp(0);
//        size= NetWebserverCmds.cmds_size();
//        qDebug() << "size:" << size;
//        qDebug() << "IsInitialized: " << NetWebserverCmds.IsInitialized();
//
//        if (NetWebserverCmds.IsInitialized() && size>0 && size<10)
//        {
//
//            NetWebserverCmds.set_time_stamp(0);
//            Server.Send(NetWebserverCmds);
//            qDebug() << "sending";
//        }
//        else
//        {
//            printf("Radio Send Packet Error!\r\n");
//        }
//
//        Server.Send(NetWebserverCmds);
//
//        NetWebserverCmds.clear_cmds();
//        NetWebserverSendMutex.unlock();
//        NetWebserverSendRequest.release();
//    }

    while(true){
//        NetWebserverSendRequest.acquire();
//        NetWebserverSendMutex.lock();

        WebDataModel* dataModel;
        QUdpSocket *udpSocket = new QUdpSocket();;
        QString msg = dataModel->createWebData();
        int port = 10006;

        int length = 0;
        if(msg == "")
        {
    //        return;
        }


        if((length=udpSocket->writeDatagram(msg.toLatin1(),msg.length(), QHostAddress::Broadcast, port))!=msg.length())
        {
//            qDebug() << "Failed!";
        }
        else{
//            qDebug() << "Success";
        }
        this->sleep(1);
//        NetWebserverSendMutex.unlock();
//        NetWebserverSendRequest.release();
    }
}



void NetWebserverSendThread::Terminate(){
    shutdownView = true;
}



