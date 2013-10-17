#ifndef FIELD_NETVISIONSEND_THREAD_H
#define FIELD_NETVISIONSEND_THREAD_H




#include <QThread>
#include <QVector>
#include <QPointF>
#include <QSemaphore>
#include "user_interface/field_related/field_robot.h"
#include "user_interface/field_related/field_view.h"
#include "net_vision_server.h"
#include "net_vision_client.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor.h"

class MainWindow;

class VisionSendThread : public QThread
{
private:
    bool brun;
    MainWindow *m_Strategy;

public:
    VisionSendThread();
    VisionSendThread (MainWindow* pStrategy );
    void run();
    void Terminate(){brun=false;}
};

//#include "user_interface/strategy_control_window.h"

#endif // FIELD_NETVISIONSEND_THREAD_H
