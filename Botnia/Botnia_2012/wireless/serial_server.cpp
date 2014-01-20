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
//
//
// 
//  **********************************************************************************************/

/**    @file    - serial_operations.cpp
  *
  *    @class   - SerialServer
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


#include "serial_server.h"

#include <QDebug>
#include <assert.h>

SerialServer::SerialServer()
{
    qDebug()<<"Serial Server init";
    port_operation_ = new PortOperation();
    port_ = port_operation_->OpenPort();

    transparent_operation_ = new TransparentOperation(port_,port_operation_);
    dect_operation_ = new DECTOperation(port_,port_operation_);
    slip_operation_ = new SLIPOperation(port_,port_operation_);

    ClearRobotParameters(robot_paramters_);
    wireless_robot_ = new WirelessRobot(robot_paramters_);
}

SerialServer::SerialServer(RobotParamters  robot_parameters)
{
    port_ = transparent_operation_->PortInitialization();
    robot_paramters_ = robot_parameters;
    wireless_robot_ = new WirelessRobot(robot_paramters_);
}

SerialServer::~SerialServer()
{

    // close the transparent operation
    if(transparent_operation_)
    {
        delete transparent_operation_;
        transparent_operation_ = NULL;
    }
    // close the DECT operation
    if(dect_operation_)
    {
        delete dect_operation_;
        dect_operation_ = NULL;
    }
    // close the SLIP operation
    if(slip_operation_)
    {
        delete slip_operation_;
        slip_operation_ = NULL;
    }

    // close the port
    if(port_operation_)
    {
        delete port_operation_;
        port_operation_ = NULL;
    }


    delete wireless_robot_;
    wireless_robot_ = NULL;
}


SerialServer::SendingState SerialServer::SendTransparentPackage()
{
    senting_array_ = transparent_operation_->FormByteCommand(senting_array_,this->wireless_robot_);
    int temp_size = senting_array_.size();
    temp_pointer_ = new unsigned char(temp_size);
    memcpy(temp_pointer_, senting_array_.data(), temp_size);
    transparent_operation_->SendPackage(temp_pointer_,temp_size,port_);
    delete temp_pointer_;
    return this->SENT_SUCCEDED;
}


SerialServer::SendingState SerialServer::SendTransparentPackage(QByteArray sending_array)
{
    this->set_senting_array(sending_array);
    int temp_size = senting_array_.size();
    temp_pointer_ = new unsigned char(temp_size);
    memcpy(temp_pointer_, senting_array_.data(), temp_size);
    transparent_operation_->SendPackage(temp_pointer_,temp_size,port_);
    delete temp_pointer_;
    return this->SENT_SUCCEDED;
}


SerialServer::SendingState SerialServer::SendTransparentPackage(WirelessRobot wireless_robot)
{
    senting_array_ = transparent_operation_->FormByteCommand(senting_array_,wireless_robot);
    int temp_size = senting_array_.size();
    temp_pointer_ = new unsigned char(temp_size);
    memcpy(temp_pointer_, senting_array_.data(), temp_size);
    transparent_operation_->SendPackage(temp_pointer_,temp_size,port_);
    delete temp_pointer_;
    return this->SENT_SUCCEDED;
}

SerialServer::SendingState SerialServer::SendTransparentPackage(unsigned char * temp_pointer_,int temp_size)
{
    transparent_operation_->SendPackage(temp_pointer_,temp_size,port_);

    return this->SENT_SUCCEDED;
}

SerialServer::SendingState SerialServer::RecievePackage()
{
    slip_operation_->RecieveFromPort();
}



bool SerialServer::IsOpen(void)
{
    return transparent_operation_->port_activated();
}

int SerialServer::port()
{
    if (IsOpen())
    {
        return port_;
    }
    else
    {
        qWarning()<< "port is not opened!";
        return NULL;
    }

}
