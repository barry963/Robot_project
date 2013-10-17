#include <string>
#include "net_radio_server.h"




RadioServer::RadioServer(int port_,
                         std::string internet_address,
                         std::string internet_interface)
{
    this->_port=port_;
    this->_internet_address = internet_address;
    this->_initernet_interface = internet_interface;
    this->multicast_server = new net::UDP();

}



RadioServer::~RadioServer()
{
    this->multicast_server->Close();
    delete multicast_server;
}

bool RadioServer::Open(int port_)
{
        this->multicast_server->Close();
        this->_port = port_;
        if (!multicast_server->Open(_port,false,true,true))
        {
            fprintf(stderr,"radio sender Unable to open UDP network port_: %d\n",_port);
            return(false);
        }
        else
        {
            qDebug()<<"radio sender Open Successed! Address="<<_internet_address.data()<<" port_="<<_port;
        }
        net::Address multicast_address,interface_address;
        multicast_address.SetHost(_internet_address.c_str(),_port);
        if(_initernet_interface.length()>0)
        {
            interface_address.SetHost(_initernet_interface.c_str(),_port);
        }
        else
        {
            interface_address.SetAny();
        }
        return true;
}

bool RadioServer::Send(const SSL_RadioPacket & packet)
{
    /*
        std::string buffer;
        packet.SerializeToString(&buffer);
        net::Address multiaddr;
        multiaddr.SetHost(_internet_address.c_str(),_port);
        bool result;
        radio_server_mutex.lock();
        result= this->multicast_server->Send(buffer.c_str(),buffer.length(),multiaddr);
        radio_server_mutex.unlock();
        if (result==false)
        {
                fprintf(stderr,"Sending Radio UDP datagram failed (maybe too large?). Size was: %d byte(s)\n",buffer.length());
        }
        return(result);
        */
    //qDebug() << "RadioServer::Send(const SSL_RadioPacket & packet)";
    std::string buffer;
    packet.SerializeToString(&buffer);
    net::Address multiaddr;
    multiaddr.SetHost(_internet_address.c_str(),_port);
    bool result;
    radio_server_mutex.lock();
    result= this->multicast_server->Send(buffer.c_str(),buffer.length(),multiaddr);
    radio_server_mutex.unlock();
    if (result==false)
    {
        fprintf(stderr,"Sending Radio UDP datagram failed (maybe too large?). Size was: %d byte(s)\n",buffer.length());
    }
    return(result);
}

bool RadioServer::Send(const SSL_RadioFrame & frame)
{
        SSL_RadioPacket pkt;
        SSL_RadioFrame * nframe = pkt.mutable_radioframe();
        nframe->CopyFrom(frame);
        return Send(pkt);
}
