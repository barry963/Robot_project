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

/**    @file    - kinect.cpp
  *
  *    @class   - Kinect
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 11/3/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */




#include "kinect.h"

using namespace Kinect_Name_Space;

Kinect::Kinect()
{
}

int Kinect::Run(int snstvty_in)
{
    int res;
    int argc = 1;
    char* argv [1];



    snstvty = snstvty_in;

    // initialize the mouse task
    mousemask(ALL_MOUSE_EVENTS, NULL);

    display = XOpenDisplay(0);

    root_window = DefaultRootWindow(display);

    screenw = XDisplayWidth(display, SCREEN);
    screenh = XDisplayHeight(display, SCREEN);

    printf("\nDefault Display Found\n");
    printf("\nSize: %dx%d\n", screenw, screenh);

    screenw += 200;
    screenh += 200;

    int i;
    for (i=0; i<2048; i++) {
            float v = i/2048.0;
            v = powf(v, 3)* 6;
            t_gamma[i] = v*6*256;
    }

    // gl argc and argv
    g_argc = argc;
    argv[0] = strdup ("Kinect Control");
    g_argv = argv;

    if (freenect_init(&f_ctx, NULL) < 0) {
            printf("freenect_init() failed\n");
            return 1;
    }

    freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);

    int nr_devices = freenect_num_devices (f_ctx);
    printf ("\nNumber of Devices Found: %d\n", nr_devices);

    int user_device_number = 0;
    if (argc > 1)
            user_device_number = atoi(argv[1]);

    if (nr_devices < 1)
            return 1;

    if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
            printf("\nCOULD NOT LOCATE KINECT :(\n");
            return 1;
    }

    res = pthread_create(&freenect_thread, NULL, freenect_threadfunc, NULL);
    if (res) {
            printf("Could Not Create Thread\n");
            return 1;
    }

    gl_threadfunc(NULL);

    return 0;
}

