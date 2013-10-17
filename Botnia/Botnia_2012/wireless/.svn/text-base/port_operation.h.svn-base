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

/**    @file    - port_operation.h
  *
  *    @class   - PortOperation
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


#ifndef PORT_OPERATION_H
#define PORT_OPERATION_H

#include "wireless_robot.h"
#include "wireless_parametres.h"

// port to be opened
extern char const * PORT;

class PortOperation
{
private:
    int port_;
    bool port_activated_;

public:
    PortOperation();
    ~PortOperation();
    int OpenPort();
    void ClosePort();
};

#endif // PORT_OPERATION_H
