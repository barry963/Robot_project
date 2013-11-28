/*
This class contains the information exchange method of conmend of the
robot.
*/





#include "net_radio_client.h"



RadioClient::RadioClient(int port_,
        string net_address,
        string net_interface)
{
        _port=port_;
        _net_address=net_address;
        _net_interface=net_interface;
        in_buffer=new char[3000];
}


RadioClient::~RadioClient()
{
        delete[] in_buffer;
}

void RadioClient::Close()
{
        multicast_server.Close();
}

bool RadioClient::Open(int port_)
{
        Close();
        _port=port_;
        if (!multicast_server.Open(_port,true,true,true,true))
        {
                fprintf(stderr,"radio receiver Unable to open UDP network port_: %d\n",_port);
                fflush(stderr);
                return(false);
        }
        else
        {
                qDebug()<<"radio receiver Open Successed! Address="<<_net_address.data()<<" port_="<<_port;
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

bool RadioClient::ReadyForRecv()
{
        return multicast_server.HavePendingData();
}

bool RadioClient::Receive(SSL_RadioPacket & packet,int iWait)
{
        net::Address src;
        int r=0;
        r = multicast_server.Recv(in_buffer,MaxDataGramSize,src,iWait);
        if (r>0)
        {
                qDebug()<<"rec in radio client recieve";
                fflush(stdout);
                //decode packet:
                return packet.ParseFromArray(in_buffer,r);
        }
        return false;
}


