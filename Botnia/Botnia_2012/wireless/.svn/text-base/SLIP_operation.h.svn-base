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

/**    @file    - slip_package.h
  *
  *    @class   - SLIPPackage
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 10/29/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#ifndef SLIP_PACKAGE_H
#define SLIP_PACKAGE_H

#include "wireless/wireless_parametres.h"
#include "wireless/port_operation.h"
#include "wireless/DECT_operation.h"


#include <QByteArray>

// this gives the recieve package size
static const int RECIEVE_PACKAGE_SIZE = 50;
// unpacked data
static const int FINAL_PACKAGE_SIZE = 50;

class SLIPOperation
{
// a number which stores the infomation of the port

private:

    int port_;
    PortOperation* port_operation_;
    // this parameter counts the number of bytes read from the port
    int read_byte_count_;
    // this variable will store the final infomation unpacked from the rs 232 port
    // and the reason that was declared as short int is because when unpack the infomation. I need to
    // check byte by byte

    // 读到的字符被认为是负值，导致输出前加上了“FF”。
    unsigned char recieve_packege_[RECIEVE_PACKAGE_SIZE];
    QByteArray slip_packege_;
    unsigned char final_packege_[FINAL_PACKAGE_SIZE];
    // this enum type will specify the state of classify the frame
    // the protocol will have three states, the first one is in the frame as the frame is surounded with "0xc0"
    // when you read c0, you do not know is it the last c0 or the first c0, until you read the next number
    enum Reading_State { IN_FRAME, OUT_FRAME, IN_OUT_FRAME};
    Reading_State read_state_ ;
    void ParseAndUpdateRobotReturnInfo();


public:
    SLIPOperation();
    SLIPOperation(int port,PortOperation* port_operation);
    ~SLIPOperation();
    // initilize the port with certain parameter
    void PortInitialization();
    // close the port
    void ClosePort();
    // unpack the infomation from the SLIP Package
    unsigned int Unpack(unsigned char* input, unsigned char* output, int array_size);
    // this method will form the package for unpack
    // in detail, this method will deal with the situation
    // e.g recieve from the half of the frame
    void FormFinalPackage();
    void RecieveFromPort();
};

#endif // SLIP_PACKAGE_H
