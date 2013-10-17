
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
#include "net_refbox_receive_thread.h"
#include "net_refbox_client.h"
#include "user_interface/field_related/field_robot.h"

extern double GetTimeSec();
QMutex RefboxReceiveMutex;
REFBOX_CMD RefboxCmd;

RefboxReceiveThread::RefboxReceiveThread()
{
    shutdownView = false;
}

void RefboxReceiveThread::Terminate()
{
    shutdownView = true;
}

void RefboxReceiveThread::run()
{
    RefboxClient client;
    if (!client.Open ())
    {
        printf("Client Open failed!\r\n");
        return;
    }
    const int bufsize=200;
    char* buf=new char[bufsize];
    int count;
    static unsigned char oldCmd=0;
    while ( !shutdownView )
    {
        count=client.Receive ( buf ,bufsize);
        if (shutdownView)break;
        if (count==6)
        {
            RefboxReceiveMutex.lock();
            RefboxCmd.Cmd=buf[0];
            RefboxCmd.Count=buf[1];
            RefboxCmd.GoalsYellow=buf[2];
            RefboxCmd.GoalsBlue=buf[3];
            RefboxCmd.TimeLeft=(*(unsigned short*)(buf+4));
            RefboxReceiveMutex.unlock();
            if (oldCmd!=buf[0])
            {
                oldCmd=buf[0];
                printf("Cmd=%c,Count=%d,Goals Yellow=%d,Goals blue=%d,time left=%ds\r\n",buf[0],buf[1],buf[2],buf[3],(*(unsigned short*)(buf+4)));
            }
        }
    }
    delete buf;
}
