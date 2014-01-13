// Transparent halt 7e000000000000157
//                 indicator   number of robot
// Transparent run  7e 00 00 00 00 1e 00 33  7e010000001e0034  7e020000001e0035  7e030000001e0036  7e040000001e0037
//                  1  2  3  4  5  6  7  8

//                  1: 01111110
//                  2: 00000000
//                  3: 00000000
//                  4: 00000000
//                  5: 00000000
//                  6: 00011110
//                  7: 00000000
//                  8: 00110011
//
//                  7e 00 00 00 00 1e 00 33  7e010000001e0034  7e020000001e0035  7e030000001e0036  7e040000001e0037

#include "transparent_operation.h"




TransparentOperation::TransparentOperation(void)
{
    send_byte_count_ = -1;
}
TransparentOperation::TransparentOperation(int port,PortOperation* port_operation)
{
    send_byte_count_ = -1;
    port_ = port;
    port_operation_ = port_operation;
}
TransparentOperation::~TransparentOperation(void)
{
    port_ = NULL;
    if(port_operation_)
    {
        port_operation_ = NULL;
    }
}

void TransparentOperation::ClosePort()
{
    port_operation_->ClosePort();
}
int TransparentOperation::PortInitialization()
{
    port_ = port_operation_->OpenPort();
    printf("Transparent init");
    return port_;
}
QByteArray TransparentOperation::FormByteCommand(QByteArray temp_byte,WirelessRobot* robot)
{
    temp_byte.clear();
    RobotParamters robot_parameters = robot->robot_parameters();
    char temp_char=robot_parameters.index;
    if (robot_parameters.kick)
    {
            temp_char|=COM_KICK;//(1<<3)
    }
    else if(robot_parameters.chipkick)
    {
            temp_char|=COM_CHIPKICK;//(1<<4)
    }
    AddByte(temp_byte, temp_char);  // first time adding byte
    temp_char=0;
    int temp_x_velocity = robot_parameters.x_velocity;
    int temp_y_velocity = robot_parameters.y_velocity;
    int temp_rotate_velocity = robot_parameters.rotate_velocity;

    int dribbler_speed=robot_parameters.dribble ;
    if(dribbler_speed!=0)
    {
            dribbler_speed=10;
            temp_char=(dribbler_speed<<4)|COM_DRIBBLER_DIR;
    }
    if(temp_x_velocity>0)
    {
        temp_char |= COM_VXSIGN;
    }
    if(temp_x_velocity<0)
    {
        temp_x_velocity = ~temp_x_velocity +1;
    }
    if(temp_y_velocity>0)
    {
        temp_char |= COM_VYSIGN;
    }
    if(temp_y_velocity<0)
    {
        temp_y_velocity = ~temp_y_velocity +1;
    }
    if(temp_rotate_velocity<0)
    {
        temp_char |= COM_VZSIGN;
        temp_rotate_velocity = temp_rotate_velocity;
    }
    temp_rotate_velocity *=10;

    AddByte (temp_byte,temp_char); // second time adding byte
    AddByte (temp_byte,char(temp_y_velocity>OVERALLVELOCITYLIMIT?OVERALLVELOCITYLIMIT:temp_y_velocity)); // third time adding
    printf("%d\n",char(temp_x_velocity));
    AddByte (temp_byte,char(temp_x_velocity>OVERALLVELOCITYLIMIT?OVERALLVELOCITYLIMIT:temp_x_velocity)); // forth time adding
    AddByte (temp_byte,char(temp_rotate_velocity>255?255:temp_rotate_velocity)); //fifth time adding
    if (robot_parameters.kick) // sixth time adding
    {
            AddByte(temp_byte, robot_parameters.kick);
    }
    else if(robot_parameters.chipkick)
    {
            AddByte(temp_byte, robot_parameters.chipkick);
    }
    else
    {
            AddByte(temp_byte, 0);
    }

    if (temp_byte.size()>=6) {
            temp_char=0;
            int j=0;
            char* cs = temp_byte.data();
            while (j<temp_byte.size()) {
                    temp_char+=*cs;
                    cs++;
                    j++;
            }
            temp_char+=0x15;//???
            temp_byte.append(temp_char); // seven'th time adding
    }
    return temp_byte;
}


QByteArray TransparentOperation::FormByteCommand(QByteArray temp_byte,WirelessRobot robot)
{
    temp_byte.clear();
    RobotParamters robot_parameters = robot.robot_parameters();
    char temp_char=robot_parameters.index;
    if (robot_parameters.kick)
    {
            temp_char|=COM_KICK;//(1<<3)
    }
    else if(robot_parameters.chipkick)
    {
            temp_char|=COM_CHIPKICK;//(1<<4)
    }
    AddByte(temp_byte, temp_char);  // first time adding byte
    temp_char=0;
    int temp_x_velocity = robot_parameters.x_velocity;
    int temp_y_velocity = robot_parameters.y_velocity;
    int temp_rotate_velocity = robot_parameters.rotate_velocity;

    int dribbler_speed=robot_parameters.dribble ;
    if(dribbler_speed!=0)
    {
            dribbler_speed=10;
            temp_char=(dribbler_speed<<4)|COM_DRIBBLER_DIR;
    }
    if(temp_x_velocity>0)
    {
        temp_char |= COM_VXSIGN;
    }
    if(temp_x_velocity<0)
    {
        temp_x_velocity = ~temp_x_velocity +1;
    }
    if(temp_y_velocity>0)
    {
        temp_char |= COM_VYSIGN;
    }
    if(temp_y_velocity<0)
    {
        temp_y_velocity = ~temp_y_velocity +1;
    }
    if(temp_rotate_velocity<0)
    {
        temp_char |= COM_VZSIGN;
        temp_rotate_velocity = temp_rotate_velocity;
    }
    temp_rotate_velocity *=10;

    AddByte (temp_byte,temp_char); // second time adding byte
    AddByte (temp_byte,char(temp_y_velocity>OVERALLVELOCITYLIMIT?OVERALLVELOCITYLIMIT:temp_y_velocity)); // third time adding
    printf("%d\n",char(temp_x_velocity));
    AddByte (temp_byte,char(temp_x_velocity>OVERALLVELOCITYLIMIT?OVERALLVELOCITYLIMIT:temp_x_velocity)); // forth time adding
    AddByte (temp_byte,char(temp_rotate_velocity>255?255:temp_rotate_velocity)); //fifth time adding
    if (robot_parameters.kick) // sixth time adding
    {
            AddByte(temp_byte, robot_parameters.kick);
    }
    else if(robot_parameters.chipkick)
    {
            AddByte(temp_byte, robot_parameters.chipkick);
    }
    else
    {
            AddByte(temp_byte, 0);
    }

    if (temp_byte.size()>=6) {
            temp_char=0;
            int j=0;
            char* cs = temp_byte.data();
            while (j<temp_byte.size()) {
                    temp_char+=*cs;
                    cs++;
                    j++;
            }
            temp_char+=0x15;//???
            temp_byte.append(temp_char); // seven'th time adding
    }
    return temp_byte;
}






int TransparentOperation::SendPackage(unsigned char* buffer, int length,int port_)
{
    int j = 0;
    unsigned char final_packet[1024];
    final_packet[0] = HDLC_STARTSTOP;
    memcpy(final_packet+1,buffer,length);
    for(j = 0;j<length+1;j++)
    {
        printf("inside:%02x\n",*(final_packet+j));
    }
    send_byte_count_ = write(port_,final_packet, length+1);
    printf("\n%d\nport:%d\n",send_byte_count_,port_);
    return 0;
}
void TransparentOperation::AddByte(QByteArray &temp_byte, char temp_char)
{
    if(temp_char == CHARSYN)//CHARSYN == 0x7e ???
    {
        temp_byte.append(CHARESC);
        temp_byte.append(ESC_SYN);
    }
    else if(temp_char == CHARSYN)
    {
        temp_byte.append(CHARESC);//CHARESC == 0xF0
        temp_byte.append(ESC_ESC);
    }
    else
    {
        temp_byte.append(temp_char);
    }
}
