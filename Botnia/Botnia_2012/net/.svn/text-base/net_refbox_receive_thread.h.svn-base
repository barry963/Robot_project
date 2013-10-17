#ifndef FIELD_REFBOXRECEIVE_THREAD_H
#define FIELD_REFBOXRECEIVE_THREAD_H

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
  \file    ClientThreading.h
  \brief   C++ Interface: ViewUpdateThread
  \author  Joydeep Biswas, Stefan Zickler (C) 2009
*/
//========================================================================

#include <QThread>
#include <QVector>
#include <QPointF>
#include <QSemaphore>
#include "net_vision_server.h"
#include "net_vision_client.h"

extern QMutex RefboxReceiveMutex;
struct REFBOX_CMD
{
        unsigned char Cmd;
        unsigned char Count;
        unsigned char GoalsYellow;
        unsigned char GoalsBlue;
        unsigned short TimeLeft;
};
extern REFBOX_CMD RefboxCmd;

class RefboxReceiveThread : public QThread
{
private:
        bool shutdownView;

public:
        RefboxReceiveThread();
        void run();
        void Terminate();
};


#endif // FIELD_REFBOXRECEIVE_THREAD_H
