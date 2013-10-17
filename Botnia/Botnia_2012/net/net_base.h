// ************************************************************************************************
//     Copyright 2012 Gao Yuan (robotics-computing.tk)
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//
//     Additional license information:
//
//  **********************************************************************************************/

/**    @file    - cmd_ui.cpp
  *
  *    @class   - CmdUI
  *
  *    @brief   - This class defines the use of basic linux socket programming. As the basis of communication is ensured by the UDP package
this network programming is the base of all other network programming
  *
  *    @author  - Gao Yuan
  *
  *    @date    - 9/19/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#ifndef NET_BASE_H
#define NET_BASE_H



#include <QUdpSocket>
#include <stdio.h>
#include <string.h>

namespace net
{


class Address
{
    QHostAddress addr;
    quint16      port_;

public:
    Address()
    {
        addr=QHostAddress(QHostAddress::Null);
    }
    Address(const Address &src)
    {
        copy(src);
    }
    ~Address()
    {
        reset();
    }

    bool SetHost(const char *hostname,int _port);
    void SetAny(int _port=0);

    bool operator==(const Address &a) const
    {
        return(port_==a.port_ && addr==a.addr);
    }
    void copy(const Address &src)
    {
        port_=src.port_;
        addr=src.addr;
    }
    void reset()
    {
        addr=QHostAddress(QHostAddress::Null);
        port_=0;
    }
    void clear()
    {
        reset();
    }

    quint32 GetInAddr() const;

    void Print(FILE *out = stdout) const;

    friend class UDP;
};



class UDP
{
    int fd;
public:
    QUdpSocket* UdpSocket;
    unsigned sent_packets;
    unsigned sent_bytes;
    unsigned recv_packets;
    unsigned recv_bytes;
public:
    UDP()
    {
        UdpSocket=new QUdpSocket();// lu_test initialize the original
                                    //null variable
    }
    ~UDP()
    {
        Close();
    }

    bool Open(int port_ = 0, bool bind=true, bool share_port_for_multicasting=false, bool multicast_include_localhost=false, bool blocking=true);
    bool AddMulticast(const Address &multiaddr,const Address &interface_addr);
    void Close();
    bool IsOpen() const
    {
        return(fd >= 0);
    }

    bool Send(const void *data,int length,Address &dest);
    int  Recv(void *data,int length,Address &src,int wait_time=200);
    bool Wait(int timeout_ms = -1) const;
    bool HavePendingData() const
    {
        return UdpSocket->hasPendingDatagrams();
    }

    int get_fd() const
    {
        return(fd);
    }
};

}


#endif // NET_BASE_H
