#ifndef NET_RADIO_CLIENT_H
#define NET_RADIO_CLIENT_H


#include "net_base.h"
#include <string>
#include <QMutex>
#include "net_radio_date_structures.h"
#include "message_serilization/radio_client_out_files/messages_robocup_ssl_cmd.pb.h"
#include "message_serilization/radio_client_out_files/messages_robocup_ssl_radio.pb.h"
using namespace std;
/**
        @author Author Name
*/
//replace 0x7E=0xA5 0xE1
//replace 0xA5=0xA5 0xD2
//radio protocol
////------------------------------
//frame flag=0x7E
////------------------------------
//vision number of packet  3bit   match the robot to that packet.
//drib_dir                 1bit   indicate the direction of dribble motor rotation
////------------------------------
//is_chip      1  indicate which kick to perform: flat kick or chip kick
//send_back    1  request the data from the robot but still unused yet
//sign_vx      1  indicate the sign of vx0
//sign_vy      1  indicate the sign of vy0
//sign_wz      1  indicate the sign of wz0
//forcekick    1  indicate kick mode: wait kick or force kick
//drib_speed   2  control the angular velocity of the dribbler motor between four levels
////------------------------------
//vx              value of the velocity command in x axis.
////------------------------------
//vy              value of the velocity command in y axis.
////------------------------------
//wz              value of the angular velocity command in z direction
////------------------------------
//kick_time       indicates pulse generating time to IGBT gate
////------------------------------

class RadioClient
{
protected:
        static const int MaxDataGramSize = 300;
        char * in_buffer;
        net::UDP multicast_server; // multicast client
        QMutex mutex;
        int _port;
        string _net_address;
        string _net_interface;
public:
        RadioClient(int port_ = 10003,
                              string net_ref_address="224.5.25.2",
                              string net_ref_interface="");

        ~RadioClient();
        bool Open(int port_=10003);
        void Close();
        void ChangePortTo(int port_);
        //bool receive(net_rcommands & rcmds);
        bool Receive(SSL_RadioPacket & packet,int iWait=200);
        bool ReadyForRecv();

};



#endif // NET_RADIO_CLIENT_H
