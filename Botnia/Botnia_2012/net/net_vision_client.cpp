#include "net_vision_client.h"


VisionClient::VisionClient(int port_,
                                   string net_address,
                                   string net_interface)
{
        _port=port_;
        _net_address=net_address;
        _net_interface=net_interface;
        in_buffer=new char[65536];
}


VisionClient::~VisionClient()
{
        delete[] in_buffer;
}

void VisionClient::Close()
{
        multicast_server.Close();
}

bool VisionClient::Open()
{
        Close();
        if (!multicast_server.Open(_port,true,true,true,true))
        {
                fprintf(stderr,"vision receiver Unable to open UDP network port_: %d\n",_port);
                fflush(stderr);
                return(false);
        }
        else
        {
                qDebug()<<"vision receiver Open Successed! Address="<<_net_address.data()<<" port_="<<_port;
        }
        net::Address multiaddr,interface_addr;
        multiaddr.SetHost(_net_address.c_str(),_port);
        if (_net_interface.length() > 0)
        {
                interface_addr.SetHost(_net_interface.c_str(),_port);
        }
        else
        {
                interface_addr.SetAny();
        }
        if (!multicast_server.AddMulticast(multiaddr,interface_addr))
        {
                fprintf(stderr,"Unable to setup UDP multicast\n");
                fflush(stderr);
                return(false);
        }
        return(true);
}

bool VisionClient::Receive(SSL_WrapperPacket & packet,int iWait)
{
        net::Address src;
        int r=0;
        r = multicast_server.Recv(in_buffer,MaxDataGramSize,src,iWait);
        if (r>0)
        {
                return packet.ParseFromArray(in_buffer,r);
        }
        return false;
}

