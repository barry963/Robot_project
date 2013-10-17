#ifndef commandsender_h
#define commandsender_h

#include "Robot.h"
#include "serial.h"

///////////////////////////////////////////////////////////////////////
/*typedef struct
{
	unsigned char	StartFlag;
	unsigned char	Addr;
	unsigned char	Ctrl;
	unsigned char	DesAddr[6];
	unsigned char	ScrAddr[6];
	unsigned char	DataLenLow;
	unsigned char	DataLenHigh;
}ComHeadPart;

typedef struct
{
	unsigned char	CrcHigh;
	unsigned char	CrcLow;
	unsigned char	EndFlag;
}ComEndPart;*/

///////////////////////////////////////////////////////////////////////


typedef enum
{
	ActionNone=0x00,
	ActionKick=0x80,
	ActionChipKick=0x40,
} RobotAction;
#define CHARSYN		0x75//'%'
#define CHARESC		0xf0
#define ESC_SYN		0xf1
#define ESC_ESC		0xf2
class BytesList
{
public:
#define COM_BUFFER_LENGTH	60
	BytesList()
	{
		count=0;
		buffer=new unsigned char[COM_BUFFER_LENGTH+1];
#ifdef WIN32
		bool openret=serialport.Open((char*)("COM1"),38400);
#else
		bool openret=serialport.Open((char*)("/dev/ttyS0"),38400);
#endif
		if (openret)
		{
			printf("Open Serial Port success!\n");
		}
		else
		{
			printf("Open Serial Port failed!\n");
		}
		/*comheadpart.StartFlag=0x7e;
		comheadpart.Addr=0xa3;
		comheadpart.Ctrl=0x03;
		//comheadpart.DesAddr[6];
		comheadpart.ScrAddr[0]=0x60;
		comheadpart.ScrAddr[1]=0xa5;
		comheadpart.ScrAddr[2]=0xc0;
		comheadpart.ScrAddr[3]=0x2e;
		comheadpart.ScrAddr[4]=0x30;
		comheadpart.ScrAddr[5]=0x00;

		comheadpart.DataLenHigh=0x00;

		comendpart.EndFlag=0x7e;*/
	}
	~BytesList()
	{
		serialport.Close();
		delete buffer;
	}

private:
	//ComHeadPart		comheadpart;
	//ComEndPart		comendpart;

	unsigned char*	buffer;
	unsigned char	count;
	Serial		serialport;
	void Add(unsigned char data)
	{
		if (count>=COM_BUFFER_LENGTH) return;
		buffer[count++]=data;
	}
public:
	void Reset(unsigned char RobotID,RobotAction action);
	void AddByte(unsigned char data)
	{
		switch (data)
		{
		case CHARSYN:
			Add(CHARESC);
			Add(ESC_SYN);
			break;
		case CHARESC:
			Add(CHARESC);
			Add(ESC_ESC);
			break;
		default:
			Add(data);
			break;
		}
	}
	//void AddEndPart(void);
	void Send(void);
	void Display(void)
	{
		for (int i=0; i<count; i++)
			printf("0x%02x ",buffer[i]);
		printf("\n\n\n");
	}
	/*inline void Read()
	{
		unsigned char ret;
		if(serialport.Read(&ret,1))
		{
			printf("%c",ret);
		}
	}*/
//#undef CHARSYN
//#undef CHARESC
//#undef ESC_SYN
//#undef ESC_ESC
};

class CommandSender
{
public:
	CommandSender();
	~CommandSender();

private:
	BytesList		buffer;
	RobotAction		action;
	unsigned char	vxh,vxl,vyh,vyl,vth,vtl;
	unsigned char	CheckSum;

	inline void SetData(Robot& p)
	{
		if (p.Kick)
		{
			action=ActionKick;
//			printf("Kick\n");
		}
		else if (p.ChipKick)
		{
			action=ActionChipKick;
//			printf("ChipKick\n");
		}
		else
		{
			action=ActionNone;
		}
		vxh=p.SpeedX>>8;
		vxl=p.SpeedX;
		vyh=p.SpeedY>>8;
		vyl=p.SpeedY;
		vth=p.SpeedT>>8;
		vtl=p.SpeedT;
	}
	inline void GetChecksum(void)
	{
		//CheckSum=action+vxh+vxl+vyh+vyl+vth+vtl;
		CheckSum=vxh+vxl+vyh+vyl+vth+vtl+0x15;
	}
public:
	void SendToRobot(Robot& p,unsigned char RobotID);
	/*inline void Read(void)
	{
		buffer.Read();
	}*/
};

#endif
