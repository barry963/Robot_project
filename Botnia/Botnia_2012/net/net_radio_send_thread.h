#ifndef NET_RADIO_SERVER_THREAD_H
#define NET_RADIO_SERVER_THREAD_H




#include <QThread>
#include <QVector>
#include <QPointF>
#include <QQueue>
#include <QMutex>
#include <QSemaphore>
#include "user_interface/field_related/field_view.h"
#include "user_interface/field_related/field_robot.h"
#include "net_vision_server.h"
#include "net_vision_client.h"
#include "net_radio_server.h"
#include "net_radio_client.h"



/*
  TODO:
  Do this after Chunqiu Lv has done his interface job

  */

extern QSemaphore RadioSendRequest;
extern QSemaphore RadioSendOk;
extern QMutex RadioSendMutex;

class RadioSendThread : public QThread
{
  private:
    bool shutdownView;
    FieldView *strategyGraphicsView;

  public:
    RadioSendThread();
    RadioSendThread ( FieldView *_field_view );
    void run();
    void Terminate();
};



#endif // NET_RADIO_SERVER_THREAD_H
