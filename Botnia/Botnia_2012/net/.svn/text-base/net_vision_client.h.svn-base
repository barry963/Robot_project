#ifndef NET_VISION_CLIENT_H
#define NET_VISION_CLIENT_H


#include "net_base.h"
#include <string>
#include <QMutex>
#include "message_serilization/radio_client_out_files/messages_robocup_ssl_detection.pb.h"
#include "message_serilization/radio_client_out_files/messages_robocup_ssl_geometry.pb.h"
#include "message_serilization/radio_client_out_files/messages_robocup_ssl_wrapper.pb.h"
using namespace std;
/**
        @author Author Name
*/

class VisionClient
{
protected:
        static const int MaxDataGramSize = 65536;
        char * in_buffer;
        net::UDP multicast_server; // multicast client
        QMutex mutex;
        int _port;
        string _net_address;
        string _net_interface;
public:
        VisionClient(int port_ = 10002,
                         string net_ref_address="224.5.23.2",
                         string net_ref_interface="");

        ~VisionClient();
        bool Open();
        void Close();
        bool Receive(SSL_WrapperPacket & packet,int iWait=200);

};

#endif // NET_VISION_CLIENT_H
