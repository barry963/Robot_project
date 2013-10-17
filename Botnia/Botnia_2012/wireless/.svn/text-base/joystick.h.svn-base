#ifndef joystick_h
#define joystick_h

#include "SDL.h"

#define MAXNUMJOYS 7

struct JOYSTICK
{
	int x; //!< X axis value
	int y; //!< Y axis value
	int z; //!< Z axis value
	bool b[12]; //!< botton on/off values
};

class JoyStick
{
public:
	JoyStick()
	{
		joy=NULL;
	}
	~JoyStick() {}
private:

public:
	SDL_Joystick*	joy;
	JOYSTICK	data;
	bool IsOK();
//	SDL_Joystick *joy[MAXNUMJOYS];
	bool init(bool bDisplayInfo=false);
	void check(void);
	void close(void);
};

extern JoyStick joystick;
#endif

