#ifndef FIELD_REDIORECEIVE_THREAD_H
#define FIELD_REDIORECEIVE_THREAD_H


#include <QThread>
#include <QVector>
#include <QPointF>
#include <QMutex>
#include <QSemaphore>
#include <QQueue>

#include "user_interface/field_related/field_view.h"
#include "net_vision_server.h"
#include "net_vision_client.h"
#include "net_radio_server.h"
#include "net_radio_client.h"

extern QSemaphore radio_receive_signal;
extern QMutex RadioReceiveMutex;
extern SSL_RadioPacket RobotRadioPacket;

class RadioReceiveThread : public QThread
{
private:
    bool shutdownView;
    int iPort;
    FieldView *strategyGraphicsView;

public:
    RadioReceiveThread() {}
    RadioReceiveThread ( FieldView *_soccer,int _Port);
    void run();
    void Terminate();
};


#endif // RADIORECEIVETHREAD_H
