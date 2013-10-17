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


#ifndef NET_WEBSERVER_SEND_THREAD_H
#define NET_WEBSERVER_SEND_THREAD_H

#include <QThread>
#include <QVector>
#include <QPointF>
#include <QQueue>
#include <QMutex>
#include <QSemaphore>
#include <QTimer>
#include "user_interface/field_related/field_view.h"
#include "user_interface/field_related/field_robot.h"

extern QSemaphore NetWebserverSendRequest;
extern QSemaphore NetWebserverSendOk;
extern QMutex NetWebserverSendMutex;

class NetWebserverSendThread : public QThread
{
private:
  bool shutdownView;
  FieldView *strategyGraphicsView;
//  QTimer *netWebserverSendtimer;

public:
    NetWebserverSendThread();
    NetWebserverSendThread ( FieldView *_field_view );
    void timeout();
    void run();
    void Terminate();
};

#endif // NET_WEBSERVER_SEND_THREAD_H
