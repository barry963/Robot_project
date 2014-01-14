
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__
extern "C"
{
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
// ************************************************************************************************
//     Copyright 2013-2014 modified by Lu Chunqiu
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//
//     Additional license information:
//
//  **********************************************************************************************/


#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
}
#include "wireless/wireless_robot.h"
#include "wireless/serial_server.h"

#define JOYSTICK_DEVNAME "/dev/input/js0"

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */


struct js_event {
    unsigned int time;      /* event timestamp in milliseconds */
    short value;   /* value */
    unsigned char type;     /* event type */
    unsigned char number;   /* axis/button number */
};

struct wwvi_js_event {
    int button[12];
    int stick_x;
    int stick_y;
};

class Joystick
{
private:
    js_event jse_;
    wwvi_js_event wjse_;
    bool js_test_flag;//lu_test
    bool js_run_flag;

public:
    Joystick();
    ~Joystick();
    int open_joystick();
    int read_joystick_event(struct js_event *jse,int joystick_fd);
    int joystick_x_axis();
    int joystick_y_axis();
    void close_joystick();   
    void JoystickControl(SerialServer* serial_server);
    void terminate_joystick();

};
#endif
