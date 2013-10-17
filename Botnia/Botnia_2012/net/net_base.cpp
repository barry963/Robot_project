


#include "net_base.h"


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "control_hub/computer_control/knowledge_base/algorithms/numeric_operation_templates.h"



//broadcast send process
//1 socket
//2 initialize the sockaddr_in structure
//3 setsockopt IP_MULTICAST_LOOPBACK
//4 setsockopt IP_MULTICAST_IF
//5 sendto
//6 close

//broadcast receive process
//1 socket
//2 setsockopt SO_REUSEADDR
//3 bind
//4 setsockopt IP_ADD_MEMBERSHIP
//5 read
//6 close


namespace net
{



bool Address::SetHost(const char *hostname,int _port)
{
        QString shostname=hostname;
        if (!addr.setAddress(shostname))
        {
                return false;
        }
        QString s=addr.toString();
        port_=_port;
        return(true);
}

void Address::SetAny(int _port)
{
        QString s="0.0.0.0";
        addr.setAddress(s);
        port_=_port;
}

quint32 Address::GetInAddr() const
{
        union ltochar
        {
                quint32 l;
                quint8  c[4];
        } s;
        s.l=addr.toIPv4Address();
        quint32 ret;
        ret=s.c[0];
        ret=ret*256+s.c[1];
        ret=ret*256+s.c[2];
        ret=ret*256+s.c[3];
        return ret;
}

//void Address::Print(FILE *out) const
//{
//#if 0
//        if (!addr_len)
//        {
//                printf("null");
//                return;
//        }
//        sockaddr_in *sockname = (sockaddr_in*)(&addr);
//        if (sockname->sin_family == AF_INET)
//        {
//                unsigned a = ntohl(sockname->sin_addr.s_addr);
//                unsigned short p = ntohs(sockname->sin_port);
//                fprintf(out,"%d.%d.%d.%d:%d",
//                        (a>>24)&0xFF, (a>>16)&0xFF, (a>>8)&0xFF, a&0xFF, p);
//        }
//        else
//        {
//                fprintf(out,"?");
//        }
//#endif
//}

//====================================================================//
//  Net::UDP: Simple raw UDP messaging
//  (C) James Bruce
//====================================================================//

bool UDP::Open(int port_, bool bind, bool share_port_for_multicasting, bool multicast_include_localhost, bool blocking)
{
        bool bOk;
        if (UdpSocket)
        {
                UdpSocket->close();
                delete UdpSocket;
        }
        UdpSocket=new QUdpSocket();
        // open the socket
        if (bind)
        {
                bOk=UdpSocket->bind(port_,QUdpSocket::ShareAddress);
                if (!bOk)return false;
                fd = UdpSocket->socketDescriptor();
        }
        else
        {
                fd=::socket(AF_INET,SOCK_DGRAM,0);
                if (fd<0)return false;
                UdpSocket->setSocketDescriptor(fd);
        }
        if (fd == -1)return false;
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags < 0) flags = 0;
        fcntl(fd, F_SETFL, flags | (blocking ? 0 : O_NONBLOCK));
        if (share_port_for_multicasting)
        {
                int reuse=1;
                if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse,sizeof(reuse))!=0)
                {
                        fprintf(stderr,"ERROR WHEN SETTING SO_REUSEADDR ON UDP SOCKET\n");
                        fflush(stderr);
                }
        }
        if (multicast_include_localhost)
        {
                int yes = 1;
                // allow packets to be received on this host
                if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&yes, sizeof(yes))!=0)
                {
                        fprintf(stderr,"ERROR WHEN SETTING IP_MULTICAST_LOOP ON UDP SOCKET\n");
                        fflush(stderr);
                }
        }
        return(true);
}

bool UDP::AddMulticast(const Address &multiaddr,const Address &interface_addr)
{
        static const bool debug = false;
        unsigned int ttl=1; // restricted to 38 hops
        // set TTL (Time To Live)
        if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, (const char *)&ttl, sizeof(ttl) ) < 0)
        {
                printf("TimeTo Live Error!\r\n");
                return false;
        }
        struct ip_mreq imreq;
        memset(&imreq,0,sizeof(ip_mreq));
        imreq.imr_multiaddr.s_addr = multiaddr.GetInAddr();			// group addr
        imreq.imr_interface.s_addr = interface_addr.GetInAddr();
        if (debug)
        {
                printf("0x%08X 0x%08X\n",
                       (unsigned)interface_addr.GetInAddr(),
                       (unsigned)INADDR_ANY);
        }
        int ret = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                             (const char *)(&imreq), sizeof(imreq));
        if (debug) printf("ret=%d\n",ret);
        return(ret == 0);
}

void UDP::Close()
{
        if (UdpSocket)
        {
                UdpSocket->close();
                delete UdpSocket;
        }
        UdpSocket=NULL;
        sent_packets = 0;
        sent_bytes   = 0;
        recv_packets = 0;
        recv_bytes   = 0;
}

bool UDP::Send(const void *data,int length,Address &dest)
{
        qint64 len = UdpSocket->writeDatagram((const char*)data,length,dest.addr,dest.port_);
        if (len > 0)
        {
                sent_packets++;
                sent_bytes += len;
        }
        return(len == length);
}

int UDP::Recv(void *data,int length,Address &src,int iWait)
{
        int len=0;
        if (iWait)
        {
                if (!UdpSocket->waitForReadyRead(iWait))
                {
                        return 0;
                }
        }
        else
        {
                if (!UdpSocket->hasPendingDatagrams())
                {
                        return 0;
                }
        }
        len = UdpSocket->readDatagram((char *)data, length,&src.addr,&src.port_);
        if (len > 0)
        {
                //UdpSocket->readAll();
                recv_packets++;
                recv_bytes += len;
        }
        return(len);
}

bool UDP::Wait(int timeout_ms) const
{
        return UdpSocket->waitForReadyRead(timeout_ms);
}

}

//====================================================================//
//  Testing Code
//====================================================================//
// compile with: g++ -g -Wall -O2 -DNETRAW_TEST netraw.cc -o nettest

#ifdef NETRAW_TEST

static const int Size = 32;

//const char *hostname = "Localhost";
const char *hostname = "127.0.0.1";

int port_ = 45454;

void EchoServer()
{
        Net::UDP net;
        Net::Address addr;
        char msg[Size+1];
        if (!net.open(port_))
        {
                printf("open net failed\r\n");
                return;
        }
        while (true)
        {
                if (net.wait(1000))
                {
                        int l = net.recv(msg,Size,addr);
                        if (l > 0)
                        {
                                msg[l] = 0;
                                printf("[");
                                addr.print();
                                printf("]: %s",msg);
                                if (msg[l-1] != '\n') printf("\n");
                                net.send(msg,l,addr);
                        }
                }
        }
        net.close();
}

void EchoClient()
{
        int state;
        Net::UDP net;
        Net::Address addr,raddr;
        char msg[Size+1]="1231231";
        int l;
        addr.setHost(hostname,port_);
        if (!net.open())
        {
                printf("open net failed\r\n");
                return;
        }
        printf("client started\r\n");
        while (fgets(msg,Size,stdin))
        {
                l = strlen(msg);
                net.send(msg,l,addr);
                net.wait(2000);
                l = net.recv(msg,Size,raddr);
                if (l>0)
                {
                        msg[l] = 0;
                        printf("reply: %s",msg);
                }
                else
                {
                        printf("no reply");
                }
        }
}

int main(int argc, char **argv)
{
        char ch;
        bool server = false;
        bool client = false;
        if (argc==2)
        {
                server=true;
        }
        else
        {
                client=true;
        }
        //while((ch = getopt(argc, argv, "sc:p:")) != EOF){
        //  switch(ch){
        //    case 's': server = true; break;
        //    case 'c': client = true; hostname = optarg; break;
        //    case 'p': port_ = atoi(optarg); break;
        //  }
        //}
        if (server == client) exit(1);
        if (server)
        {
                EchoServer();
        }
        else
        {
                EchoClient();
        }
}
#endif
