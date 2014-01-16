#ifndef FIELD_NETVISION_THREAD_H
#define FIELD_NETVISION_THREAD_H


#include "user_interface/field_related/field_robot.h"
#include "user_interface/field_related/field_view.h"

#include "net_vision_server.h"
#include "net_vision_client.h"


#include <QThread>
#include <QVector>
#include <QPointF>
#include <QQueue>
#include <QSemaphore>

extern QSemaphore VisionReceiveSignal;
int VisionUpdate(const SSL_WrapperPacket &f);

class VisionReceiveThread : public QThread
{
        Q_OBJECT
  private:
    bool vision_initial_flag;
    bool thread_terminated;
    FieldView *strategyGraphicsView;
//    GLSoccerView * ai_view;
  public:
    VisionReceiveThread();
    VisionReceiveThread ( FieldView *_soccer );
    void run();
    void Terminate();
    void UpdateAIView();
};

#endif // FIELD_NETVISION_THREAD_H
