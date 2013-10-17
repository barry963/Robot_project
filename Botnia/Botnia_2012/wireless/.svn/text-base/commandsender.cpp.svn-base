#include "commandsender.h"

/*static const unsigned char ComDesAddr[][6]=
{
	{0x98,0xa5,0xc0,0x2e,0x30,0x00},	//Robot0
	{0x40,0xa5,0xc0,0x2e,0x30,0x00},	//Robot1
	{0x90,0xa5,0xc0,0x2e,0x30,0x00},	//Robot2
	{0x20,0xa5,0xc0,0x2e,0x30,0x00},	//Robot3
	{0x10,0xa5,0xc0,0x2e,0x30,0x00},	//Robot4
	{0xff,0xff,0xff,0xff,0xff,0xff}		//broadcast Addr
};*/
//static const unsigned char ComScrAddr[6]={0x60,0xa5,0xc0,0x2e,0x30,0x00};

void BytesList::Reset(unsigned char RobotID,RobotAction action)
{
	count=0;
	//头部
//	memcpy(comheadpart.DesAddr,ComDesAddr[RobotID],6);
	/*memcpy(comheadpart.DesAddr,ComDesAddr[5],6);
	memcpy(buffer,&comheadpart,sizeof(comheadpart));
	count+=sizeof(comheadpart);*/
	//数据部分
	buffer[count++]=CHARSYN;
	buffer[count++]=RobotID | action;
	//buffer[count++]=action;
	//buffer[count++]=0;
}
/*static unsigned short CalculateCRC(	// copyed from HW86012_FM_210 datasheet
				unsigned char *Block,      //array of bytes
				unsigned int BlockLen      //length of Block in bytes
				)
{
   unsigned short crc = 0; //CRC initialised with zero
   unsigned char BitPos;      //counter for bit level loop
   while (BlockLen != 0)      //main loop over all bytes
   {
       crc ^= ( (unsigned short) *Block++ << 8);
                              //modulo-2 add a byte
       for (BitPos=0; BitPos<8; BitPos++)
                              //loop over all bits of byte
       {
          if (crc & 0x8000)
            crc = (crc << 1) ^ 0x1021;
          else
            crc <<= 1;
                              //apply generator polynomial
       }
       BlockLen--;            //decrement loop counter
   }
   return crc;
}
void BytesList::AddEndPart(void)
{
	//计算校验和
//	unsigned short checksum=CalculateCRC((unsigned char*)(&comheadpart),3);
	unsigned short checksum=CalculateCRC((unsigned char*)(buffer)+1,count-1);
	comendpart.CrcLow=checksum;
	comendpart.CrcHigh=checksum>>8;
//	comendpart.CrcLow=0x47;
//	comendpart.CrcHigh=0xbb;

	//
	memcpy(buffer+count,&comendpart,sizeof(comendpart));
	count+=sizeof(comendpart);
	//comheadpart.DataLenLow=count-20;
	comheadpart.DataLenLow=count-4;
	buffer[15]=comheadpart.DataLenLow;
}*/
void BytesList::Send(void)
{
	serialport.Write(buffer,count);
	//char *str="good\r\n";
	//static unsigned char data=0x00;
	//serialport.Write(&data,1);
	//data++;
}
CommandSender::CommandSender()
{
}
CommandSender::~CommandSender()
{
}
void CommandSender::SendToRobot(Robot& p,unsigned char RobotID)
{
	SetData(p);
	GetChecksum();
	buffer.Reset(RobotID,action);
//	buffer.Display();
	buffer.AddByte(vxh);
	buffer.AddByte(vxl);
	buffer.AddByte(vyh);
	buffer.AddByte(vyl);
	buffer.AddByte(vth);
	buffer.AddByte(vtl);
	buffer.AddByte(CheckSum);
	buffer.AddByte(0xff-CheckSum);
//	buffer.Display();
//	buffer.AddEndPart();
//	buffer.Display();
	buffer.Send();
}

