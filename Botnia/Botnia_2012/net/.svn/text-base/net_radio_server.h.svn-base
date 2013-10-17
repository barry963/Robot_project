#ifndef NET_RADIO_SERVER_H
#define NET_RADIO_SERVER_H

#include "net_base.h"



#include <string>
#include <QMutex>



#include "message_serilization/radio_client_out_files/messages_robocup_ssl_radio.pb.h"


class RadioServer
{
private:
    int _port;
    std::string _internet_address;
    std::string _initernet_interface;
    net::UDP *multicast_server;
    QMutex radio_server_mutex;

public:
    RadioServer(int port_ = 10003,\
                std::string internet_address = "224.5.25.2",\
                std::string internet_interface = "" );
    ~RadioServer();
    bool Open(int port_ = 10003);
    void Close();
    void ChangePortTo(int port_);
    void Recieve(SSL_RadioPacket package,int wait_time);
    bool Send(const SSL_RadioPacket & packet);
    bool Send(const SSL_RadioFrame & frame);


};

#endif // NET_RADIO_SERVER_H
