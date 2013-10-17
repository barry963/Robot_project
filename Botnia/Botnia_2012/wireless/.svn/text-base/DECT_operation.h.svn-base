#ifndef DECT_PACKAGE_H_
#define DECT_PACKAGE_H_



#include "wireless_robot.h"
#include "wireless_parametres.h"
#include "wireless/port_operation.h"



class DECTOperation
{
        int final_package_length;
        unsigned char last_used_addr_byte_;
        char botniaMac[6];
        int send_byte_count_;
        bool port_activated_;
        int port_;
        PortOperation* port_operation_;
    public:
        DECTOperation();
        DECTOperation(int port,PortOperation* port_operation_);
        ~DECTOperation();
        void SendPackage(unsigned char *, unsigned char* , int length);
        int PortInitialization();
        bool PortIsActivated();
        QString FormStringCommand(QString qstring,WirelessRobot* robot);
        QByteArray FormByteCommand(QByteArray temp_byte,WirelessRobot* robot);
        void AddByte(QByteArray& temp_byte,char temp_char);
};

#endif
