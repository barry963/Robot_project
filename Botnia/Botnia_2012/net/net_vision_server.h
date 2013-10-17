#ifndef NET_VISION_SERVER_H
#define NET_VISION_SERVER_H


#include "net_base.h"
#include <string>
#include <QMutex>
#include "message_serilization/radio_client_out_files/messages_robocup_ssl_detection.pb.h"
#include "message_serilization/radio_client_out_files/messages_robocup_ssl_geometry.pb.h"
#include "message_serilization/radio_client_out_files/messages_robocup_ssl_wrapper.pb.h"
using namespace std;

class VisionServer
{
protected:
        net::UDP multicast_server; // multicast server
        QMutex mutex;
        int _port;
        string _net_address;
        string _net_interface;

public:
        VisionServer(int port_ = 10002,
                         string net_ref_address="224.5.23.2",
                         string net_ref_interface="");

        ~VisionServer();
        bool Open();
        void Close();
        bool Send(const SSL_WrapperPacket & packet);
        bool Send(const SSL_DetectionFrame & frame);
        bool Send(const SSL_GeometryData & geometry);

};


#endif // NET_VISION_SERVER_H
