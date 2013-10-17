#ifndef WIRELESS_PACKET_H
#define WIRELESS_PACKET_H

#include "RCSerialPortInterface.h"

#define PACKET_SIZE 25
#define NUM_ROBOTS 5

// Data Structure to hold Robot Commands
class RobotCommand
{

public:
	float xVel;
	float yVel;
	float rotVel;

	int kick;
	int dribble;
	int vDribble;

	bool beep;
};


// Class for sending wireless packets to the robots
class WirelessSender
{

public:

	WirelessSender(char* portName);
	~WirelessSender();

	void setRobotCommand (int index, RobotCommand* command);
	void setRobotVelocity(int index, float xVel, float yVel, float rotVel);
	void setRobotActions (int index, int kick, int dribble, int vDribble);

	void setRobotRotation(int index, float rotation);

	void getWheelCommands(int index, RobotCommand* command);

	void buildPacket();
	int sendCommands();

	void getPacket(char packet[PACKET_SIZE]);

private:

	// Serial Port Interface
	RCSerialPortInterface RCSPort;

	// Data packet
	unsigned char dataPacket[PACKET_SIZE];


	// Robot Rotations
	float rotations[NUM_ROBOTS];

	// Robot Commands
	RobotCommand commands[NUM_ROBOTS];

	// Wheel Commands
	RobotCommand wheels[NUM_ROBOTS];

};


#endif WIRELESS_MODULE_H