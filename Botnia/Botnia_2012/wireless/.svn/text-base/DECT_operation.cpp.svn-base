#include "DECT_operation.h"


unsigned int pppfcs16(unsigned char* cp, int len)
{
    unsigned int fcs = PPPINITFCS16;
    while (len--)
    {
        fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];
    }

    return (fcs);
}

unsigned char nextAddr(unsigned char addr)
{
    switch (addr)
    {
    case HDLC_FR0: return HDLC_FR1;
    case HDLC_FR1: return HDLC_FR2;
    case HDLC_FR2: return HDLC_FR3;
    case HDLC_FR3: return HDLC_FR0;
    default: ;//assert(false);
    }
    return 0;
}

char *string_to_hex(char* a,char* finalhash)
{
    memset(finalhash,0,strlen(finalhash));
    unsigned int j;
    for(j=0; j < strlen(a); j++)
    {
        if(j%2 == 0){
            switch(a[j])
            {

            case '0':
                finalhash[j/2] |= 0x00<<4;
                break;
            case '1':
                finalhash[j/2] |= 0x01<<4;
                break;
            case '2':
                finalhash[j/2] |= 0x02<<4;
                break;
            case '3':
                finalhash[j/2] |= 0x03<<4;
                break;
            case '4':
                finalhash[j/2] |= 0x04<<4;
                break;
            case '5':
                finalhash[j/2] |= 0x05<<4;
                break;
            case '6':
                finalhash[j/2] |= 0x06<<4;
                break;
            case '7':
                finalhash[j/2] |= 0x07<<4;
                break;
            case '8':
                finalhash[j/2] |= 0x08<<4;
                break;
            case '9':
                finalhash[j/2] |= 0x09<<4;
                break;
            case 'a':
                finalhash[j/2] |= 0x0a<<4;
                break;
            case 'b':
                finalhash[j/2] |= 0x0b<<4;
                break;
            case 'c':
                finalhash[j/2] |= 0x0c<<4;
                break;
            case 'd':
                finalhash[j/2] |= 0x0d<<4;
                break;
            case 'e':
                finalhash[j/2] |= 0x0e<<4;
                break;
            case 'f':
                finalhash[j/2] |= 0x0f<<4;
                break;
            default:
                continue;
            }
        }
        if(j%2 == 1){
            switch(a[j])
            {
            case '0':
                finalhash[(j-1)/2] |= 0x00;
                break;
            case '1':
                finalhash[(j-1)/2] |= 0x01;
                break;
            case '2':
                finalhash[(j-1)/2] |= 0x02;
                break;
            case '3':
                finalhash[(j-1)/2] |= 0x03;
                break;
            case '4':
                finalhash[(j-1)/2] |= 0x04;
                break;
            case '5':
                finalhash[(j-1)/2] |= 0x05;
                break;
            case '6':
                finalhash[(j-1)/2] |= 0x06;
                break;
            case '7':
                finalhash[(j-1)/2] |= 0x07;
                break;
            case '8':
                finalhash[(j-1)/2] |= 0x08;
                break;
            case '9':
                finalhash[(j-1)/2] |= 0x09;
                break;
            case 'a':
                finalhash[(j-1)/2] |= 0x0a;
                break;
            case 'b':
                finalhash[(j-1)/2] |= 0x0b;
                break;
            case 'c':
                finalhash[(j-1)/2] |= 0x0c;
                break;
            case 'd':
                finalhash[(j-1)/2] |= 0x0d;
                break;
            case 'e':
                finalhash[(j-1)/2] |= 0x0e;
                break;
            case 'f':
                finalhash[(j-1)/2] |= 0x0f;
                break;
            default:
                continue;
            }
        }
    }
    return finalhash;
}



DECTOperation::DECTOperation(void)
{
    last_used_addr_byte_ = HDLC_FR3;
    final_package_length = 1;
    botniaMac[0] = 'R';
    botniaMac[1] = 'o';
    botniaMac[2] = 'b';
    botniaMac[3] = 'o';
    botniaMac[4] = 't';
    botniaMac[5] = 'V';
    port_activated_=new bool;
    send_byte_count_ = -1;
    port_activated_=true;
}

DECTOperation::DECTOperation(int port,PortOperation* port_operation)
{
    port_ = port;
    port_operation_ = port_operation;
}


DECTOperation::~DECTOperation(void)
{
    if(port_operation_)
        port_operation_ = NULL;
}

bool DECTOperation::PortIsActivated(void)
{
    return port_activated_;
}

void DECTOperation::AddByte(QByteArray &temp_byte, char temp_char)
{
    if(temp_char == CHARSYN)
    {
        temp_byte.append(CHARESC);
        temp_byte.append(ESC_SYN);
    }
    else if(temp_char == CHARSYN)
    {
        temp_byte.append(CHARESC);
        temp_byte.append(ESC_ESC);
    }
    else
    {
        temp_byte.append(temp_char);
    }
}
//QString SendDECTPackage::FormStringCommand(QString string,WirelessRobot* robot)
//{

//}

QByteArray DECTOperation::FormByteCommand(QByteArray temp_byte,WirelessRobot* robot)
{
    temp_byte.append(CHARSYN);
    char temp_char = robot->robot_parameters().index;
    temp_char &=0x07;
    temp_byte.clear();
    if(robot->robot_parameters().kick)
    {
        temp_char |= COM_KICK;

    }
    else if(robot->robot_parameters().chipkick)
    {
        temp_char |= COM_CHIPKICK;
    }

    AddByte(temp_byte,temp_char);
    temp_char = 0;
    int temp_x_velocity = robot->x_velocity();
    int temp_y_velocity = robot->y_velocity();
    int temp_rotate_velocity = robot->rotate_velocity();

    if(robot->robot_parameters().dribble)
    {
        temp_rotate_velocity = 10;
        temp_char = (temp_rotate_velocity<<4)  | COM_DRIBBLER_DIR;

    }
    if(temp_x_velocity<0)
    {
        temp_char |= COM_VXSIGN;
    }
    if(temp_y_velocity<0)
    {
        temp_char |= COM_VYSIGN;
    }
    if(temp_rotate_velocity<0)
    {
        temp_char |= COM_VZSIGN;
        temp_rotate_velocity = temp_rotate_velocity;
    }
    temp_rotate_velocity *=10;
    AddByte (temp_byte,temp_char);
    AddByte (temp_byte,char(temp_x_velocity>OVERALLVELOCITYLIMIT?OVERALLVELOCITYLIMIT:temp_x_velocity));
    AddByte (temp_byte,char(temp_y_velocity>OVERALLVELOCITYLIMIT?OVERALLVELOCITYLIMIT:temp_y_velocity));
    AddByte (temp_byte,char(temp_rotate_velocity>255?255:temp_rotate_velocity));
    if(robot->kick())
    {
        AddByte(temp_byte,robot->kick());
    }
    else if(robot->chipkick())
    {
        AddByte(temp_byte,robot->chipkick());
    }
    else
    {
        AddByte(temp_byte,0);
    }
    if(temp_byte.size()>=6)
    {
        temp_char = 0;
        int j =0;
        char* temp_data_pointer = temp_byte.data();
        while(j<temp_byte.size())
        {
            temp_char += *temp_data_pointer;
            temp_data_pointer++;
            j++;
        }
        temp_char +=0x05;
        temp_byte.append(temp_char);

    }
    //TODO: before adding temp_byte CHATSYN needs to be added
    return temp_byte;
}

void DECTOperation::SendPackage(unsigned char *adr, unsigned char* buffer, int length)
{
    unsigned char packet[length+20];
    packet[0] = HDLC_STARTSTOP;
    packet[1] = nextAddr(last_used_addr_byte_);
    last_used_addr_byte_ = packet[1];
    packet[2] = HDLC_CTRL;
    memcpy(packet+3,(char *)adr,6);
    memcpy(packet+9,botniaMac,6);
    length+=2;
    packet[15] = 0x00;
    packet[16] = (unsigned char)length;
    unsigned int current_sequence_number = 0x1234;
    memcpy(packet+17,&current_sequence_number, sizeof(unsigned int));
    memcpy(packet+19,buffer, length-2);
    int hdlc_crc = pppfcs16(&packet[1], 2+6+6+2+length);
    memcpy (packet+17+length, &hdlc_crc, sizeof(int));
    unsigned char final_packet[1024];
    final_packet[0] = HDLC_STARTSTOP;
    unsigned char i;
    for (i = 1; i < 19+length; i++)
    {
        if(packet[i] == 0x7e)
        {
            final_packet[final_package_length] = 0x7d;
            final_package_length++;
            final_packet[final_package_length] = 0x5e;
        }
        else if (packet[i] == 0x7d)
        {
            final_packet[final_package_length] = 0x7d;
            final_package_length++;
            final_packet[final_package_length] = 0x5d;
        }
        else
        {
            final_packet[final_package_length] = packet[i];
        }
        final_package_length++;
    }

    final_packet[final_package_length] = HDLC_STARTSTOP;
    final_package_length++;
    send_byte_count_ = write(port_, final_packet, final_package_length);
    printf("\n%d\nport:%d\n",send_byte_count_,port_);
    final_package_length =1;
}
int DECTOperation::PortInitialization()
{
    int fd;
    struct termios oldtio,newtio;
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY  | O_NDELAY);
    if (fd <0) {perror(MODEMDEVICE); exit(-1); }
    tcgetattr(fd,&oldtio);
    bzero(&newtio, sizeof(newtio)); /* clear struct for new port_ settings */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;
    newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
    newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    newtio.c_cc[VERASE]   = 0;     /* del */
    newtio.c_cc[VKILL]    = 0;     /* @ */
    newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
    newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
    newtio.c_cc[VSWTC]    = 0;     /* '\0' */
    newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
    newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    newtio.c_cc[VEOL]     = 0;     /* '\0' */
    newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    newtio.c_cc[VEOL2]    = 0;     /* '\0' */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
    port_activated_ = true;
    port_ = fd;
    return fd;
}


