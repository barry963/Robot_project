/*This class deals with the progress of sending information to the
client
*/




#include "net_radio_server.h"
#include "net_radio_send_thread.h"
#include <QDebug>

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


extern double GetTimeSec();

QSemaphore RadioSendRequest(5);
QSemaphore RadioSendOk;
QMutex RadioSendMutex;

RadioSendThread::RadioSendThread ( FieldView *_field_view )
{
    strategyGraphicsView = _field_view;
    shutdownView = false;
}

void RadioSendThread::Terminate()
{
    shutdownView = true;
}

SSL_RadioFrame RadioCmds;

void RadioSendThread::run()
{

    RadioServer Server;
    if (!Server.Open())
    {
        printf("Server Open failed!\r\n");
        return;
    }

    int size;
    while (true)
    {

        qDebug() << "Available: " << RadioSendRequest.available();
        RadioSendRequest.acquire();
        if (shutdownView)break;
        RadioSendMutex.lock();
        RadioCmds.set_time_stamp(0);
        size= RadioCmds.cmds_size();
        qDebug() << "size:" << size;
        qDebug() << "IsInitialized: " << RadioCmds.IsInitialized();

        if (RadioCmds.IsInitialized() && size>0 && size<10)
        {

            RadioCmds.set_time_stamp(0);
            Server.Send(RadioCmds);
            qDebug() << "sending";
        }
        else
        {
            printf("Radio Send Packet Error!\r\n");
        }

        Server.Send(RadioCmds);

        RadioCmds.clear_cmds();
        RadioSendMutex.unlock();
        RadioSendRequest.release();
    }
}
