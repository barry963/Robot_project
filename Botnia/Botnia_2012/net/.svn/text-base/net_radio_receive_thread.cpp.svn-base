
#include "net/net_radio_receive_thread.h"
#include "user_interface/field_related/field_robot.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor.h"
#include "net/thread_tools/field_timer.h"


#include <QSemaphore>





QSemaphore radio_receive_signal;


RadioReceiveThread::RadioReceiveThread ( FieldView *_field_view,int _iPort)
{
    strategyGraphicsView = _field_view;
    shutdownView = false;
    iPort=_iPort;
}

void RadioReceiveThread::Terminate()
{
    shutdownView = true;
}

SSL_RadioPacket RobotRadioPacket;

void RadioReceiveThread::run()
{
    RadioClient client;
    SSL_RadioPacket packet0;
    if(!client.Open (iPort))
    {
        printf("Radio Receive,Port=%d Open failed!\r\n",iPort);
        return;
    }
    //unsigned long tick,tickperiod,oldtick=GetTickCount();
    while ( !shutdownView )
    {
        if ( client.Receive ( packet0 ) )
        {
            if(shutdownView)break;
            SSL_RadioFrame frame=packet0.radioframe();
            m_Simulator.lock();
            m_Simulator.do_radio_recv(frame);
            m_Simulator.unlock();
        }
        if(shutdownView)break;
    }
}



