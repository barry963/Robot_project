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

/**    @file    - port_operation.cpp
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


#include "port_operation.h"
#include "wireless/wireless_parametres.h"

#include <QDebug>

#include <errno.h>


char const *PORT  = "/dev/ttyUSB1";

PortOperation::PortOperation()
{
}


int PortOperation::OpenPort()
{
    port_ = open(PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if(port_ == -1) // if open is unsucessful
    {
            qDebug()<<QString("Error 1:Open Port Error: Unable to open %1.").arg(PORT);
    }
    else
    {
            fcntl(port_, F_SETFL, 0);
            qDebug("port_ is open.\n");
    }
    struct termios port_settings;      // structure to store the port_ settings in
    int termios_state = 0;
    termios_state = tcgetattr(port_,&port_settings);
    if (termios_state < 0) {
        qWarning()<<QObject::tr("Error 2:Termios GET error: %1").arg(strerror (errno));
    }

    cfsetispeed(&port_settings, B115200);    // set baud rates
    cfsetospeed(&port_settings, B115200);

//    port_settings.c_cflag &= ~PARENB;    // set no parity, one stop bits, 8 data bits
//    port_settings.c_cflag &= ~CSTOPB;
//    port_settings.c_cflag &= ~CSIZE;
//    port_settings.c_cflag |= CS8;
    port_settings.c_iflag |= IGNPAR;

    port_settings.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CREAD | CLOCAL);
    port_settings.c_cflag |= CS8;
    port_settings.c_cflag |= CREAD;
    port_settings.c_cflag |= CLOCAL;

    port_settings.c_lflag &= ~(ICANON | ECHO);
    port_settings.c_cc[VMIN] = 1;
    port_settings.c_cc[VTIME] = 0;

    termios_state = tcsetattr(port_, TCSANOW, &port_settings);    // apply the settings to the port_
    if (termios_state < 0) {
        qWarning()<<QObject::tr("Error 3:Termios SET error: %1").arg(strerror (errno));
    }
    port_activated_ = true;


    return port_;
}


void PortOperation::ClosePort()
{
    close(port_);
}

PortOperation::~PortOperation()
{
    ClosePort();
}
