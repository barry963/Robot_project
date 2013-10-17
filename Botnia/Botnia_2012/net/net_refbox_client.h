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
  \file    robocup_ssl_client.h
  \brief   C++ Interface: robocup_ssl_client
  \author  Stefan Zickler, 2009
*/
//========================================================================
#ifndef ROBOCUP_SSL_REFBOXCLIENT_H
#define ROBOCUP_SSL_REFBOXCLIENT_H
#include "net/net_base.h"
#include "net_radio_date_structures.h"
#include <string>
#include <QMutex>
using namespace std;
/**
	@author Author Name
*/

class RefboxClient
{
protected:
        net::UDP multicast_server; // multicast client
	QMutex mutex;
	int _port;
	string _net_address;
	string _net_interface;
public:
        RefboxClient(int port_ = 10001,
	                       string net_ref_address="224.5.92.5",
	                       string net_ref_interface="");

        ~RefboxClient();
        bool Open();
        void Close();
        int Receive(char* buf,int bufsize);
        bool ReadyForRecv();

};

#endif
