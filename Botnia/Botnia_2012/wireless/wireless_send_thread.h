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

/**    @file    - wireless_send_thread.h
  *
  *    @class   - WirelessSendThread
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 10/1/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#ifndef WIRELESS_SEND_THREAD_H
#define WIRELESS_SEND_THREAD_H

#include "wireless/wireless_robot.h"
#include "wireless/wireless_parametres.h"
#include "wireless/transparent_operation.h"

#include <QThread>
#include <QMutex>

#if 0
QMutex wireless_mutex;

class WirelessSendThread:public QThread
{
private:
    bool initialized_;

public:

    WirelessSendThread();
    ~WirelessSendThread();

    bool initialized(){return this->initialized_;}
    void set_initialized(bool initialized){ this ->initialized_ = initialized;}

    void run();
    void Terminate();

};
#endif

#endif // WIRELESS_SEND_THREAD_H
