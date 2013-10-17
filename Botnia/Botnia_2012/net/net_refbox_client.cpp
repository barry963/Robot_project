//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    robocup_ssl_client.cpp
  \brief   C++ Implementation: robocup_ssl_client
  \author  Stefan Zickler, 2009
*/
//========================================================================
//#include "net/net_radio_server.h"
#include "net/net_refbox_client.h"

RefboxClient::RefboxClient(int port_,
        string net_address,
        string net_interface)
{
	_port=port_;
	_net_address=net_address;
	_net_interface=net_interface;
}


RefboxClient::~RefboxClient()
{
}

void RefboxClient::Close()
{
        multicast_server.Close();
}

bool RefboxClient::Open()
{
        Close();
        if (!multicast_server.Open(_port,true,true,true,true))
	{
		fprintf(stderr,"refbox receiver Unable to open UDP network port_: %d\n",_port);
		fflush(stderr);
		return(false);
	}
	else
	{
		qDebug()<<"refbox receiver Open Successed! Address="<<_net_address.data()<<" port_="<<_port;
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
		fprintf(stderr,"refbox receiver Unable to setup UDP multicast\n");
		fflush(stderr);
		return(false);
	}
	return(true);
}

bool RefboxClient::ReadyForRecv()
{
        return multicast_server.HavePendingData();
}

int RefboxClient::Receive(char* buf,int bufsize)
{
        net::Address src;
        return  multicast_server.Recv(buf,bufsize,src);
}
