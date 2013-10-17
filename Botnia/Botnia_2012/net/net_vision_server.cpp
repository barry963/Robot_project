#include "net_vision_server.h"


VisionServer::VisionServer(int port_,
                                   string net_address,
                                   string net_interface)
{
        _port=port_;
        _net_address=net_address;
        _net_interface=net_interface;
}


VisionServer::~VisionServer()
{
}

void VisionServer::Close()
{
        multicast_server.Close();
}

bool VisionServer::Open()
{
        Close();
        if (!multicast_server.Open(_port,false,true,true))
        {
                fprintf(stderr,"vision sender Unable to open UDP network port_: %d\n",_port);
                fflush(stderr);
                return(false);
        }
        else
        {
                qDebug()<<"vision sender Open Successed! Address="<<_net_address.data()<<" port_="<<_port;
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
        return(true);
}

bool VisionServer::Send(const SSL_WrapperPacket & packet)
{
        string buffer;
        packet.SerializeToString(&buffer);
        net::Address multiaddr;
        multiaddr.SetHost(_net_address.c_str(),_port);
        bool result;
        mutex.lock();
        result=multicast_server.Send(buffer.c_str(),buffer.length(),multiaddr);
        mutex.unlock();
        if (result==false)
        {
                fprintf(stderr,"Sending Vision UDP datagram failed (maybe too large?). Size was: %d byte(s)\n",buffer.length());
        }
        return(result);
}

bool VisionServer::Send(const SSL_DetectionFrame & frame)
{
        SSL_WrapperPacket pkt;
        SSL_DetectionFrame * nframe = pkt.mutable_detection();
        nframe->CopyFrom(frame);
        return Send(pkt);
}

bool VisionServer::Send(const SSL_GeometryData & geometry)
{
        SSL_WrapperPacket pkt;
        SSL_GeometryData * gdata = pkt.mutable_geometry();
        gdata->CopyFrom(geometry);
        return Send(pkt);
}

