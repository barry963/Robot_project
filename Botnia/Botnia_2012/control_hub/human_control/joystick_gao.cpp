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

#if 1
#include <stdio.h>
#include "wireless/DECT_operation.h"
#include "wireless/transparent_operation.h"
#include <sys/wait.h>

#include <string.h>
#include "control_hub/human_control/joystick_gao.h"
// for sending signals


static int joystick_fd = -1;

Joystick::Joystick()
{
    wjse_.stick_x = 0;
    wjse_.stick_y = 0;
    js_test_flag = true;
    js_run_flag = true;
}

Joystick::~Joystick()
{
}

int Joystick::open_joystick()
{
    joystick_fd = open(JOYSTICK_DEVNAME, O_RDONLY | O_NONBLOCK); /* read write for force feedback? */
    if (joystick_fd < 0)
        return joystick_fd;

    /* maybe ioctls to interrogate features here? */

    return joystick_fd;
}

int Joystick::read_joystick_event(struct js_event *jse,int joystick_fd)
{
    int bytes;

    bytes = read(joystick_fd, jse, sizeof(*jse));

    if (bytes == -1)
        return 0;

    if (bytes == sizeof(*jse))
        return 1;

    printf("Unexpected bytes from joystick:%d\n", bytes);

    return -1;
}

void Joystick::close_joystick()
{
    close(joystick_fd);
}

void Joystick::JoystickControl(SerialServer* serial_server)
{
    int fd, rc;
    struct js_event jse;
    fd = open_joystick();
    if (fd < 0) {
        printf("joystick open failed.\n");
        exit(1);
    }

    RobotParamters robot_parameters;
    robot_parameters = ClearRobotParameters(robot_parameters);
    TransparentOperation package;
    QByteArray temp_array;
    int fd_wireless=serial_server->port();

    if(js_test_flag)// to cut the noise
    {
        for(int i=0;i<100;i++)
        {
            rc = read_joystick_event(&jse,fd);
        }
        js_test_flag = false;
    }

    //while (!js_run_flag)
    while (1)
    {
        rc = read_joystick_event(&jse,fd);
        usleep(1000);

        if (rc == 1)
        {
            if(jse.value!=0 &&jse.value!=1)
            {
                jse.value /=128;
                // jse.value -= 256 ;
            }
            //printf("Event: time %8u, value %8hd, type: %3u, axis/button: %u\n ",
            //       jse.time, jse.value, jse.type, jse.number);
            jse.type &= ~JS_EVENT_INIT; /* ignore synthetic events */
            if (jse.type == JS_EVENT_AXIS)
            {
                switch (jse.number) {
                case 0: wjse_.stick_x = jse.value;
                    break;
                case 1: wjse_.stick_y = jse.value;
                    break;
                case 2: wjse_.stick_y = jse.value;
                    break;
                default:
                    break;
                }
                //printf("stick x :%8hd,stick y :%8hd\n",wjse_.stick_x,wjse_.stick_y);
                robot_parameters.x_velocity = joystick_x_axis();
                robot_parameters.y_velocity = joystick_y_axis();
                //printf("test_x : %d;test_y: %d \n",robot_parameters.x_velocity,robot_parameters.y_velocity);

                WirelessRobot robot = WirelessRobot(robot_parameters);
                robot.set_x_velocity(robot_parameters.x_velocity);
                robot.set_y_velocity(robot_parameters.y_velocity);


                temp_array = package.FormByteCommand(temp_array,robot);
                int temp_size = temp_array.size();
                unsigned char *temp_pointer = new unsigned char(temp_size);
                memcpy(temp_pointer, temp_array.data(), temp_size);
                while(1)
                {
                    package.SendPackage(temp_pointer,temp_size,fd_wireless);
                    break;
                }
            } else if (jse.type == JS_EVENT_BUTTON) {
                if (jse.number < 11) {
                    switch (jse_.value) {
                    case 0:
                        wjse_.button[jse.number] = jse.value;//release
                    case 1:                 //press
                        wjse_.button[jse.number] = jse.value;
                        break;
                    default:
                        break;
                    }
                }
//                for( int x=0 ; x<11 ; ++x )
//                    printf( "button: %d",wjse_.button[x] );
//                printf("\n");
            }
        }

    }//end of while
}

int Joystick::joystick_x_axis()
{   //int axis= ;
    return wjse_.stick_x;
}

int Joystick::joystick_y_axis()
{   //int axis=
    return wjse_.stick_y;
}

#endif
