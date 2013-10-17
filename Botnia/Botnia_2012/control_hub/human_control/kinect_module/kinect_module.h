#ifndef KINECT_MODULE
#define KINECT_MUDULE



#include "ncurses_for_kinect.h" // has deleted the conflict file with graphics.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libfreenect.h"
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

// for port operation
#include <fcntl.h>
#include <termios.h>

//#include <graphics.h>// it has global so it can only be placed here
#include "graphics.h"// it has global so it can only be placed here

#define SCREEN (DefaultScreen(display))

extern int depth;
extern char *display_name;

extern Display *display;
extern Window main_window;
extern Window root_window;

extern pthread_t freenect_thread;
extern volatile int die;

extern int g_argc;
extern char **g_argv;

extern int window;

extern float tmprot;

extern pthread_mutex_t gl_backbuf_mutex;

extern uint8_t gl_depth_front[640*480*4];
extern uint8_t gl_depth_back[640*480*4];

extern uint8_t gl_rgb_front[640*480*4];
extern uint8_t gl_rgb_back[640*480*4];

extern GLuint gl_depth_tex;
extern GLuint gl_rgb_tex;

extern freenect_context *f_ctx;
extern freenect_device *f_dev;

extern int freenect_angle;
extern int freenect_led;


extern float pointerx, pointery;
extern float mousex, mousey ;
extern float tmousex, tmousey ;
extern int screenw , screenh;
extern int snstvty;

extern int pause_ ;
extern int pusx, pusy;


extern pthread_cond_t gl_frame_cond;
extern int got_frames;
extern uint16_t t_gamma[2048];

// screen size for graph window
extern int screen_x;
extern int screen_y;

// this variable desides when curcor is in the circle whether the view is refreshed
extern bool refreshed;

// predefined info to drive the robot
extern int port;
extern unsigned char stop_pattern[8] ;
extern unsigned char forward_pattern[8];
extern unsigned char backward_pattern[8];
extern unsigned char left_pattern[8];
extern unsigned char right_pattern[8];



void DrawGLScene();
void keyPressed(unsigned char key, int x, int y);
void ReSizeGLScene(int Width, int Height);
void InitGL(int Width, int Height);
void *gl_threadfunc(void *arg);
void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp);
void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp);
void *freenect_threadfunc(void *arg);
int getRootWindowSize(int *w, int *h);
// open port
int OpenPort();
// send package
void SendPackage(unsigned char* buffer, int length,int port_);

#endif
