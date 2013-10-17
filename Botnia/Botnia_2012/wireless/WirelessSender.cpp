#include "WirelessSender.h"
#include "WheelConversion2002.h"
#include "WheelConversion2001.h"
#include "WheelConversion2000.h"
#include <iostream>
#include "PacketStructure2001.h"
#include "PacketStructure2002.h"
#include <QMutex>

using namespace std;
QMutex qmx;

//-----------------------------------------------------
WirelessSender::WirelessSender()
{
	lastUsedAddrByte = HDLC_FR3;
	serialPacket->clear();
}

//-----------------------------------------------------
WirelessSender::~WirelessSender()
{
	;
}
//-----------------------------------------------------
void WirelessSender::sendCommands()
{
	qmx.lock();
	serialPacket->clear();
	//buildPacket(0);	return;

	if (serMode == 2) {
		lastUsedAddrByte = nextaddr(lastUsedAddrByte);
		serialPacket->append(lastUsedAddrByte);
		serialPacket->append(HDLC_CTRL);
		serialPacket->append(botniaMac);
	}


	for (int i=0; i<5; i++)
	{
		if (serMode == 1)
			buildPacket(i);
		else if (serMode == 2) {
			buildDECTPacket(i);
		}
	}

	if (serMode == 2) {

		unsigned int hldc_crc = pppfcs16(serialPacket->data(), serialPacket->size());
		serialPacket->append((char)(hldc_crc%256));
		serialPacket->append((char)(hldc_crc/256));
		char* i=serialPacket->data();
		int j=0;

		while (j<serialPacket->size()) {
			if(*i == 0x7e)
			{
				QByteArray tmp;
				tmp.clear();
				tmp.append(0x7d);
				tmp.append(0x5e);
				serialPacket->replace(j, tmp);
				j++;
			}
			else if (*i == 0x7d)
			{
				QByteArray tmp;
				tmp.clear();
				tmp.append(0x7d);
				tmp.append(0x5d);
				serialPacket->replace(j, tmp);
				j++;
			}
			i++;
			j++;
		}

		serialPacket->prepend(HDLC_STARTSTOP);
		serialPacket->append(HDLC_STARTSTOP);		
	}
	qmx.unlock();
}

//-----------------------------------------------------
void WirelessSender::setRobotCommand(int index, RobotCommand* command)
{
	if ((index < 0) || (index >= NUM_AGENTS)) return;
	memcpy( &commands[index], command, sizeof(RobotCommand));
}

//-----------------------------------------------------
void WirelessSender::setRobotVelocity(int index, float xVel, float yVel, float rotVel)
{
	if(xVel>255)	xVel=255;
	else if(xVel<-255)	xVel=-255;
	if(yVel>255)	yVel=255;
	else if(yVel<-255)	yVel=-255;
	const int	maxrotvel=60;
	if(rotVel>maxrotvel)	rotVel=maxrotvel;
	else if(rotVel<-maxrotvel)	rotVel=-maxrotvel;

	commands[index].xVel    = (int)xVel;
	commands[index].yVel    = (int)yVel;
	commands[index].rotVel  = (int)rotVel;
	//cout<<"x: "<<xVel<<endl;
	//cout<<"y: "<<yVel<<endl;
	//cout<<"z: "<<rotVel<<endl;
}

//-----------------------------------------------------
void WirelessSender::setRobotActions (int index, int kick, int dribble, int chipkick)
{
	commands[index].kick      = kick;
	//static int kicktimes=0;
	//if(kick>0){	kicktimes++;	cout<<"kick the ball: "<<kicktimes<<endl;	}
	commands[index].dribble   = -dribble;
	//cout<<"dribble: "<<dribble<<endl;
	commands[index].chipkick  = chipkick;
	//cout<<"vDribble: "<<dribble<<endl;
}

//-----------------------------------------------------
void WirelessSender::setRobotRotation(int index, float rotation)
{
	if ((index < 0) || (index >= NUM_AGENTS)) return;
	rotations[index] = rotation;
}

//-----------------------------------------------------
void WirelessSender::setRobotType(int index, RobotType newRobotType)
{
	robotType[index] = newRobotType;
}

RobotType WirelessSender::getRobotType(int index)
{
	return robotType[index];
}

//-----------------------------------------------------
void WirelessSender::buildPacket(int index)
{
	QByteArray tmpBuffer;
	tmpBuffer.clear();

	char ch=index;
	ch&=0x07;
	//if (forcekick_on)
	{
		//ch|=COM_FORCEKICK;
	}
	switch (robotType[index])
	{
	case ROBOT_2008:
		ch|=0x80;
		break;
	case ROBOT_2009:
	default:
		break;
	}
	if (commands[index].kick)
	{
		ch|=COM_KICK;
	}
	else if(commands[index].chipkick)
	{
		ch|=COM_CHIPKICK;
	}

	addByte(tmpBuffer, ch);

	ch=0;
	int	vx,vy,va;
	vx = commands[index].xVel;
	vy = commands[index].yVel;
	va = commands[index].rotVel;
	int dribbler_speed=commands[index].dribble;
	//if(commands[index].kick || dribbler_speed!=0)
	if(dribbler_speed!=0)
	{
		dribbler_speed=10;
		ch=(dribbler_speed<<4)|COM_DRIBBLER_DIR;
	}
	/*
	if (dribbler_speed<0)
	{
		dribbler_speed=10;//dribbler_speed;
		if (dribbler_speed>15)	dribbler_speed=15;
		ch=(dribbler_speed<<4)|COM_DRIBBLER_DIR;
	}
	else
	{
		if (commands[index].dribble!=0)		dribbler_speed=10;
		if (dribbler_speed>15)	dribbler_speed=15;
		ch=dribbler_speed<<4;
	}*/
	if (vx<0)
	{
		ch|=COM_VXSIGN;
		vx=-vx;
	}
	if (vy<0)
	{
		ch|=COM_VYSIGN;
		vy=-vy;
	}
	if (va<0)
	{
		ch|=COM_VZSIGN;
		va=-va;
	}
	va=va*10;
	addByte(tmpBuffer, ch);
	addByte(tmpBuffer, char(vx>overallVelocityLimit?overallVelocityLimit:vx));
	addByte(tmpBuffer, char(vy>overallVelocityLimit?overallVelocityLimit:vy));
	addByte(tmpBuffer, char(va>255?255:va));
	if (commands[index].kick)
	{
		addByte(tmpBuffer, commands[index].kick);
	}
	else if(commands[index].chipkick)
	{
		addByte(tmpBuffer, commands[index].chipkick);
	}
	else
	{
		addByte(tmpBuffer, 0);
	}
	//cout<<"kick "<<commands[index].kick<<endl;
	
	if (tmpBuffer.size()>=6) {
		ch=0;
		int j=0;
		char* cs = tmpBuffer.data();
		while (j<tmpBuffer.size()) {
			ch+=*cs;
			cs++;
			j++;
		}
		ch+=0x15;
		tmpBuffer.append(ch);
	}

	serialPacket->append(CHARSYN);
	serialPacket->append(tmpBuffer);
}

void WirelessSender::buildDECTPacket(int index)
{
	char ch=index;
	ch&=0x07;
	//if (forcekick_on)
	{
		//ch|=COM_FORCEKICK;
	}
	switch (robotType[index])
	{
	case ROBOT_2008:
		ch|=0x80;
		break;
	case ROBOT_2009:
	default:
		break;
	}
	if (commands[index].kick)
	{
		ch|=COM_KICK;
	}
	else if(commands[index].chipkick)
	{
		ch|=COM_CHIPKICK;
	}
	serialPacket->append(ch);

	ch=0;
	int	vx,vy,va;
	vx = commands[index].xVel;
	vy = commands[index].yVel;
	va = commands[index].rotVel;
	int dribbler_speed=commands[index].dribble;
	//if(commands[index].kick || dribbler_speed!=0)
	if(dribbler_speed!=0)
	{
		dribbler_speed=10;
		ch=(dribbler_speed<<4)|COM_DRIBBLER_DIR;
	}
	/*
	if (dribbler_speed<0)
	{
		dribbler_speed=10;//dribbler_speed;
		if (dribbler_speed>15)	dribbler_speed=15;
		ch=(dribbler_speed<<4)|COM_DRIBBLER_DIR;
	}
	else
	{
		if (commands[index].dribble!=0)		dribbler_speed=10;
		if (dribbler_speed>15)	dribbler_speed=15;
		ch=dribbler_speed<<4;
	}*/
	if (vx<0)
	{
		ch|=COM_VXSIGN;
		vx=-vx;
	}
	if (vy<0)
	{
		ch|=COM_VYSIGN;
		vy=-vy;
	}
	if (va<0)
	{
		ch|=COM_VZSIGN;
		va=-va;
	}
	va=va*10;
	serialPacket->append(ch);
	serialPacket->append(char(vx>overallVelocityLimit?overallVelocityLimit:vx));
	serialPacket->append(char(vy>overallVelocityLimit?overallVelocityLimit:vy));
	serialPacket->append(char(va>255?255:va));
	if (commands[index].kick)
	{
		serialPacket->append(commands[index].kick);
	}
	else if(commands[index].chipkick)
	{
		serialPacket->append(commands[index].chipkick);
	}
	else
	{
		serialPacket->append(char(0));
	}			
}

void WirelessSender::addByte(QByteArray& tmpBuffer, char ch)
{
	if (ch==CHARSYN)
	{
		tmpBuffer.append(CHARESC);
		tmpBuffer.append(ESC_SYN);
	}
	else if (ch==CHARESC)
	{
		tmpBuffer.append(CHARESC);
		tmpBuffer.append(ESC_ESC);
	}
	else
	{
		tmpBuffer.append(ch);
	}
}

char WirelessSender::nextaddr(char addr)
{
	switch (addr)
	{
	case HDLC_FR0:
		return HDLC_FR1;
	case HDLC_FR1:
		return HDLC_FR2;
	case HDLC_FR2:
		return HDLC_FR3;
	case HDLC_FR3:
	default:
		return HDLC_FR0;
	}
}

unsigned int WirelessSender::pppfcs16(char* data,int len)
{	
	unsigned int fcs = PPPINITFCS16;
	if (len<1)	return 0;
	while (len--)
	{
		fcs = (fcs >> 8) ^ fcstab[(fcs ^ *data++) & 0xff];
	}
	return fcs;
}

void WirelessSender::stopRobot(int robot, int offsetByte)
{
	/*dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 0] = 0x00;
	dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 1] = 0x00;
	dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 2] = 0x00;
	dataPacket[offsetByte + BYTES_PER_ROBOT * robot + 3] = 0x00;*/
	setRobotVelocity(robot,0,0,0);
	setRobotActions(robot,0,0,0);
}

void WirelessSender::reInit()
{
	for (int i=0; i<NUM_AGENTS; i++) {
		commands[i].xVel = 0;
		commands[i].yVel = 0;
		commands[i].rotVel = 0;
		commands[i].kick = 0;
		commands[i].chipkick = 0;
		//commands[i].kickWaitInfared = false;
		commands[i].dribble = 0;
		robotType[i]=ROBOT_2009;
	}
}

void WirelessSender::loadValues()
{
	///Open the parameter file:
	///REMEMBER TO CHANGE THE PARAMETER TEXT FILE NAME TO THE APPROPRIATE NAME!
	//ifstream mainFile;
	//mainFile.open("Params/Skill_Params/DribblePickScoreSkillParams.txt", ios_base::in);//ios::in);//ios::nocreate);
	//ASSERT(mainFile.fail() == 0, "CAN'T OPEN PARAMETER FILE!");
	//Load parameter values.	See params.h and params.cpp for good examples.
	//################	ADD PARAMETERS BELOW ################
	//READ_FLOAT(TURN_HAPPY_ANGLE);
	//READ_FLOAT(PAST_DIST);
	//READ_FLOAT(FORWARD_DIST);
	//READ_FLOAT(PICK_BUFFER);
	//READ_FLOAT(CLOSE_DIST);
	//################	ADD PARAMETERS ABOVE ################
	//mainFile.close();
}
