
#include "RCWirelessSender.h"
#include "WheelConversion.h"
#include "iostream.h"

static const int BYTES_PER_ROBOT = 5;

/*
Byte 1     - Wheel 1 (Left) velocity magnitude
Byte 2     - Wheel 2 (Right) velocity magnitude
Byte 3     - Wheel 3 (Forward) velocity magnitude
Byte 4
        bit 7:6 - Vertical Dribbler
        bit 5:4 - Low 2 bits of wheel 3 velocity magnitude
        bit 3:2 - Low 2 bits of wheel 2 velocity magnitude
        bit 1:0 - Low 2 bits of wheel 1 velocity magnitude
Byte 5
        bit 7   - Wheel 1 velocity sign
        bit 6   - Wheel 2 velocity sign
        bit 5   - Wheel 3 velocity sign
        bit 4:3 - Kicker
        bit 2:1 - Horizontal Dribbler
        bit 0   - Beep
*/

// BYTE 4
static const unsigned char PACKET_BEEP           = (0x01); // 00000001

static const unsigned char PACKET_FAST_DRIBBLE   = (0x06); // 00000110
static const unsigned char PACKET_MID_DRIBBLE    = (0x04); // 00000100
static const unsigned char PACKET_SLOW_DRIBBLE   = (0x02); // 00000010

static const unsigned char PACKET_KICK_SHOT      = (0x18); // 00011000
static const unsigned char PACKET_KICK_PASS      = (0x10); // 00010000
static const unsigned char PACKET_KICK_SLOW      = (0x08); // 00001000

static const unsigned char PACKET_WHEEL_ONE      = (0x80); // 10000000
static const unsigned char PACKET_WHEEL_TWO      = (0x40); // 01000000
static const unsigned char PACKET_WHEEL_THREE    = (0x20); // 00100000


// BYTE 3
static const unsigned char PACKET_FAST_V_DRIBBLE = (0xC0); // 11000000
static const unsigned char PACKET_MID_V_DRIBBLE  = (0x80); // 10000000
static const unsigned char PACKET_SLOW_V_DRIBBLE = (0x40); // 01000000



static float PI = 3.1459f;

static float gear             =    12.2f;
static float diameter         =     0.040f;  //meters
static float quadrature       =     4.0f;
static float counts_per_rev   =   100.0f;
static float sample_time_8mhz =     0.000256f;
static float PIDconst         = 65536.0f;

static float linearSpeedModifier;

static int bitsToIgnore       = 11;

static float velocityScalingFactor = 1.15f;  // tested empirically


static float wheelOne, wheelTwo, wheelThree;

static WheelConversion* robots2001;

//-----------------------------------------------------
WirelessSender::WirelessSender(char* portName)
{
	robots2001 = new WheelConversion();
	// Calculate Linear Speed Modifier
	linearSpeedModifier = (gear / (PI * diameter))
	                      * counts_per_rev    // counts per revolution
	                      * sample_time_8mhz  // sample period
	                      * PIDconst          // fit for PID
	                      * quadrature;       // for quadrature encoders
	int status;
	// Open connection
	status = RCSPort.openConnection(portName);
	if (status == RCS_INTERFACE_CANNOT_SET_COM_STATE)
		cout << "Cannot open port " << portName << endl;
	else
	{
		cout << "Wireless Transmission on Port " << portName << endl;
	}
	for (int i=0; i<NUM_ROBOTS; i++)
	{
		// Initialize 'rotations' array
		rotations[i] = 0.0f;
		// Initialize 'commands' array
		commands[i].xVel = 0.0f;
		commands[i].yVel = 0.0f;
		commands[i].rotVel = 0.0f;
		commands[i].kick = 0;
		commands[i].dribble = 0;
		commands[i].vDribble = 0;
		commands[i].beep = false;
		// Initialize 'wheels' array
		wheels[i].xVel = 0.0f;
		wheels[i].yVel = 0.0f;
		wheels[i].rotVel = 0.0f;
		wheels[i].kick = 0;
		wheels[i].dribble = 0;
		wheels[i].vDribble = 0;
		wheels[i].beep = false;
	}
	// Initialize data packet
	for (int j=0; j<PACKET_SIZE; j++)
	{
		dataPacket[j] = 0x00;
	}
}


//-----------------------------------------------------
WirelessSender::~WirelessSender()
{
	RCSPort.closeConnection();
}


//-----------------------------------------------------
int WirelessSender::sendCommands()
{
	int status;
	status = RCSPort.sendPacket((char*)dataPacket);
	return status;
}

//-----------------------------------------------------
void WirelessSender::setRobotCommand(int index, RobotCommand* command)
{
	if ((index < 0) || (index >= NUM_ROBOTS)) return;
	memcpy( &commands[index], command, sizeof(RobotCommand));
}


//-----------------------------------------------------
void WirelessSender::setRobotVelocity(int index, float xVel, float yVel, float rotVel)
{
	commands[index].xVel    = xVel;
	commands[index].yVel    = yVel;
	commands[index].rotVel  = rotVel;
}

//-----------------------------------------------------
void WirelessSender::setRobotActions (int index, int kick, int dribble, int vDribble)
{
	commands[index].kick      = kick;
	commands[index].dribble   = dribble;
	commands[index].vDribble  = vDribble;
}

//-----------------------------------------------------
void WirelessSender::setRobotRotation(int index, float rotation)
{
	rotations[index] = rotation;
}


//-----------------------------------------------------
void WirelessSender::getWheelCommands(int index, RobotCommand* wheel)
{
	if ((index < 0) || (index >= NUM_ROBOTS)) return;
	memcpy( wheel, &wheels[index], sizeof(RobotCommand));
}

//-----------------------------------------------------
void WirelessSender::getPacket(char packet[PACKET_SIZE])
{
	memcpy(packet, dataPacket, PACKET_SIZE);
}

//-----------------------------------------------------
void WirelessSender::buildPacket()
{
	unsigned char result;
	// Clear data packet
	for (int i=0; i<PACKET_SIZE; i++)
	{
		dataPacket[i] = 0x00;
	}
	// Build packets for each robot
	for (int robot=0; robot<NUM_ROBOTS; robot++)
	{
		//**********************************************************
		// Add parameter bits to packet
		result = 0x00;
		if (commands[robot].beep)             result |= PACKET_BEEP;
		if (commands[robot].kick > 1)         result |= PACKET_KICK_SHOT;
		else if (commands[robot].kick > 0)    result |= PACKET_KICK_PASS;
		if (commands[robot].dribble > 1)      result |= PACKET_FAST_DRIBBLE;
		else if (commands[robot].dribble > 0) result |= PACKET_MID_DRIBBLE;
		dataPacket[BYTES_PER_ROBOT * robot + 4] = result;
		if (commands[robot].vDribble > 0)
			dataPacket[BYTES_PER_ROBOT * robot + 3] |= PACKET_FAST_V_DRIBBLE;
		//**********************************************************
		// Convert robot velocities into wheel velocities
		robots2001->convertWheelVelocities( commands[robot].xVel,
		                                    commands[robot].yVel,
		                                    commands[robot].rotVel,
		                                    rotations[robot],
		                                    wheels[robot].xVel,
		                                    wheels[robot].yVel,
		                                    wheels[robot].rotVel);
		//**********************************************************
		// Convert wheel velocities into packet velocities
		wheelOne    = velocityScalingFactor * linearSpeedModifier * wheels[robot].xVel;
		wheelTwo    = velocityScalingFactor * linearSpeedModifier * wheels[robot].yVel;
		wheelThree  = velocityScalingFactor * linearSpeedModifier * wheels[robot].rotVel;
		// Set sign bits:
		// 0x80 for wheel one
		// 0x40 for wheel two
		// 0x20 for wheel three
		if (wheelOne < 0)
		{
			dataPacket[BYTES_PER_ROBOT * robot + 4] |= PACKET_WHEEL_ONE;
			wheelOne *= -1;
		}
		if (wheelTwo < 0)
		{
			dataPacket[BYTES_PER_ROBOT * robot + 4] |= PACKET_WHEEL_TWO;
			wheelTwo *= -1;
		}
		if (wheelThree < 0)
		{
			dataPacket[BYTES_PER_ROBOT * robot + 4] |= PACKET_WHEEL_THREE;
			wheelThree *= -1;
		}
		// Build 10 bit velocity
		unsigned short wheelCommand;
		unsigned char temp_byte;
		// Wheel One
		wheelCommand = ((int) wheelOne ) >> bitsToIgnore;
		if ( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;
		temp_byte = wheelCommand >> 2 ;
		dataPacket[BYTES_PER_ROBOT * robot + 0] = temp_byte;
//    temp_byte = wheelCommand & 0x0003 ;
//    dataPacket[BYTES_PER_ROBOT * robot + 3] |= (temp_byte);
		// Wheel Two
		wheelCommand = ((int) wheelTwo ) >> bitsToIgnore;
		if ( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;
		temp_byte = wheelCommand >> 2 ;
		dataPacket[BYTES_PER_ROBOT * robot + 1] = temp_byte;
//    temp_byte = wheelCommand & 0x0003 ;
//    dataPacket[BYTES_PER_ROBOT * robot + 3] |= (temp_byte << 2);
		// Wheel Three
		wheelCommand = ((int) wheelThree ) >> bitsToIgnore;
		if ( wheelCommand > 0x3FF ) wheelCommand = 0x3FF;
		temp_byte = wheelCommand >> 2 ;
		dataPacket[BYTES_PER_ROBOT * robot + 2] = temp_byte;
//    temp_byte = wheelCommand & 0x0003 ;
//    dataPacket[BYTES_PER_ROBOT * robot + 3] |= (temp_byte << 4);
	}
}


