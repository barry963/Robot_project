#include "joystick.h"
#include  "SDL.h"
//#include <linux/joystick.h>
#include "stdio.h"
#include <iostream>

using namespace std;

JoyStick joystick;

bool JoyStick::init(bool bDisplayInfo)
{
	joy=NULL;
	if ( 0 > SDL_Init( SDL_INIT_JOYSTICK))
	{
		if (bDisplayInfo)
		{
			cout<<"Init SDL joystick " << SDL_GetError();
		}
		SDL_Quit();
	}
	// Check for joystick
	if (SDL_NumJoysticks()>0)
	{
		// Open joystick
		joy=SDL_JoystickOpen(0);
		if (joy)
		{
			printf("Opened Joystick 0\n");
			printf("Name: %s\n", SDL_JoystickName(0));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
			return true;
		}
		else if (bDisplayInfo)
		{
			printf("Couldn't open Joystick 0\n");
		}
	}
	else if (bDisplayInfo)
	{
		printf("No Joystick Present!\n");
	}
	return false;
}

bool JoyStick::IsOK()
{
	return (joy!=NULL);
}

void JoyStick::check()
{
	if (!joy) return;
	SDL_JoystickUpdate();
	data.x = SDL_JoystickGetAxis(joy, 0) >> 8;
	data.y = SDL_JoystickGetAxis(joy, 1) >> 8;
	data.z = SDL_JoystickGetAxis(joy, 2) >> 8;
	for (int i=0; i<12; i++)
	{
		data.b[i]=SDL_JoystickGetButton(joy,i)?true:false;
	}
}
void JoyStick::close()
{
	if (joy)
	{
		SDL_JoystickClose(joy);
		SDL_Quit();
	}
	// Close if opened
//	if(SDL_JoystickOpened(0))
//		SDL_JoystickClose(joy);
}

