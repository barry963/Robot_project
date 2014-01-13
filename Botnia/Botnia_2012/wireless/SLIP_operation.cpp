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

/**    @file    - slip_package.cpp
  *
  *    @class   - SLIPPackage
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 10/29/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#include "SLIP_operation.h"
#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"

#include <QDebug>

static int READING_SIZE = 1;

SLIPOperation::SLIPOperation()
{
        qDebug()<<"SLIP init";
    port_ = port_operation_->OpenPort();
}
SLIPOperation::SLIPOperation(int port,PortOperation* port_operation)
{
    port_ = port;
    port_operation_ = port_operation;
    // set all the values in the recieve package, this way ensure
    // all the values will be zero except what we got from the
    // port
    memset(recieve_packege_,0,sizeof(recieve_packege_));
    // set all the values stored in the package to be zero
    // in this way it will not start with the random values
    slip_packege_.clear();
    //memset(slip_packege_,0,sizeof(slip_packege_));

    read_state_ = OUT_FRAME;
}

SLIPOperation::~SLIPOperation()
{
    if(port_operation_)
    {
        port_operation_ = NULL;
    }
}
void SLIPOperation::ClosePort()
{
    port_operation_->ClosePort();
}


void SLIPOperation::RecieveFromPort()
{
    // this function is import as it controls whether the port should read or write
    // in the official discription, it means wait untill the all the thing should be transmitted.

    // this variable will read from the first to last to identify the in and out place of the package
    // this curcor will point to the number in the recieve_array
    int read_curcor = 0;
    // qFatal will take control if port is not opened
    if(port_ == -1)
        qFatal("The port was not initialized!");
    tcdrain (port_);
    while(TRUE)
    {

        read_byte_count_ = read(port_,recieve_packege_,READING_SIZE);
        if(read_byte_count_ == -1)
            qWarning()<< "There is no byte recieved!";
        while(recieve_packege_[read_curcor])
        {
            switch(recieve_packege_[read_curcor])
            {
                // big endian conversion
            case 0xc0:
                if(read_state_ == OUT_FRAME)
                    read_state_ = IN_OUT_FRAME;
                else if(read_state_ == IN_OUT_FRAME)
                {
                    read_state_ = IN_FRAME;
                    slip_packege_.append(recieve_packege_[read_curcor]);
                }
                // going out of the frame, unpack the data then
                else if(read_state_ == IN_FRAME)
                {
                    slip_packege_.append(recieve_packege_[read_curcor]);
                    unsigned char* test = (unsigned char*)slip_packege_.data();

                    Unpack(test,final_packege_,slip_packege_.size());

                    // parse robot return info and update world
                    ParseAndUpdateRobotReturnInfo();


                    qDebug()<<slip_packege_.toHex();
                    slip_packege_.clear();
                    read_state_ = OUT_FRAME;
                }
                break;

            default:
                if(read_state_ == OUT_FRAME)
                {}
                else if(read_state_ == IN_OUT_FRAME)
                {
                    read_state_ = IN_FRAME;
                    slip_packege_.append(0xc0);
                    slip_packege_.append(recieve_packege_[read_curcor]);
                }
                else if(read_state_ == IN_FRAME)
                {
                    slip_packege_.append(recieve_packege_[read_curcor]);
                }


            }
            read_curcor++;

        }
        read_curcor = 0;

    }


}
unsigned int SLIPOperation::Unpack(unsigned char* input, unsigned char* output, int array_size)
{
    unsigned char i,j,l;
    i=0;
    j=0;
    l=array_size;
    while(array_size--)
    {

        switch(input[j])
        {
        case END:
            if((j!=0)&&(j!=(l-1))) return 0;

            break;

        case ESC:

            j++;array_size--;

            if(input[j]==ESC_ESC) output[i++]=ESC;
            else if(input[j]==ESC_END) output[i++]=END;
            else return 0;
            break;

        default:

            output[i++]=input[j];
        }

        j++;
    }
    return i;

}


void SLIPOperation::ParseAndUpdateRobotReturnInfo()
{
    int i = 0;

    robot_return_info_mutex_.lock();
    // reverse the change in the ARM code, as the UART module cannot send 0x00;
    for(i = 0;i<FINAL_PACKAGE_SIZE;i++)
    {
        if(final_packege_[i] == 0xff)
            final_packege_[i] = 0x00;
    }
    // the first and second byte represent the cell voltage
    world.set_cell_voltage((float)final_packege_[1]/10);
    // the third and fourth byte represent the capacitor voltage
    // so final_package[2] final_package[3]
    world.set_capacitor_voltage(final_packege_[3]);
    world.set_current_level(0.0);
    world.set_kicking_voltage(0.0);
    world.set_number_of_package(*(unsigned int*)(final_packege_+8));
    robot_return_info_mutex_.unlock();
}
