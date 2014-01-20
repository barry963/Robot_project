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

/**    @file    - serial_operations.h
  *
  *    @class   - SerialOperations
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 10/4/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#ifndef SERIAL_OPERATIONS_H
#define SERIAL_OPERATIONS_H

#include "wireless/wireless_robot.h"
#include "wireless/wireless_parametres.h"
#include "wireless/transparent_operation.h"
#include "wireless/DECT_operation.h"
#include "wireless/SLIP_operation.h"


/******************** serial port communication ********************************/

class SLIPOperation;
class DECTOperation;
class TransparentOperation;
/*****************************************************************/

class SerialServer
{
private:
    enum SendingState
    {
        SENT_SUCCEDED,
        SENT_FAILED
    };

    PortOperation* port_operation_;

    TransparentOperation*  transparent_operation_;
    DECTOperation* dect_operation_;
    SLIPOperation* slip_operation_;


    QByteArray  senting_array_;
    RobotParamters  robot_paramters_;
    WirelessRobot*  wireless_robot_;
    unsigned char*  temp_pointer_;
    // the port number
    int port_;


public:
    // getting port is special will be written in cpp
    int port();
    void set_port(int port){port_ = port;}
    SerialServer();
    SerialServer(RobotParamters  robot_paramters);
    ~SerialServer();
    SerialServer::SendingState SendTransparentPackage(void);
    SerialServer::SendingState SendTransparentPackage(QByteArray senting_array);
    SerialServer::SendingState SendTransparentPackage(WirelessRobot wireless_robot);
    SerialServer::SendingState RecievePackage();

    bool IsOpen(void);
    QByteArray senting_array(void) {return senting_array_;}
    void set_senting_array(QByteArray senting_array) {senting_array_ = senting_array;}

    SerialServer::SendingState SendTransparentPackage(unsigned char * temp_pointer_,int temp_size);

};

#endif // SERIAL_OPERATIONS_H
