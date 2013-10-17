#ifndef TRANSPARENT_PACKAGE_H
#define TRANSPARENT_PACKAGE_H
#include "wireless_robot.h"
#include "wireless_parametres.h"
#include "port_operation.h"

#include <QBitArray>





class TransparentOperation
{
 private:
    int send_byte_count_;
    bool port_activated_;
    int port_;
    PortOperation* port_operation_;
protected:

public:
    TransparentOperation();
    TransparentOperation(int port,PortOperation* port_operation);
    ~TransparentOperation();
    // this class will deal with the basic operation like open the port with the specific  parameters
    // it will be responsable for the close of the port.
    // how ever it will not cause any problem if the port is not activated

    int PortInitialization(void);
    void ClosePort();

    int SendPackage(unsigned char* buffer, int length,int fd);
    void AddByte(QByteArray &temp_byte, char temp_char);
    QByteArray FormByteCommand(QByteArray temp_byte,WirelessRobot* robot);
    QByteArray FormByteCommand(QByteArray temp_byte,WirelessRobot robot);
    bool port_activated(){return port_activated_;}
    void set_port_activated(bool port_activated){port_activated_ = port_activated;}

};

#endif // TRANSPARENT_PACKAGE_H
