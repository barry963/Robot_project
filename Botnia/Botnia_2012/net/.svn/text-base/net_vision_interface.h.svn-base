#ifndef FIELD_NETVISION_INTERFACE_H
#define FIELD_NETVISION_INTERFACE_H



#include "user_interface/field_related/field_constants.h"
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_robot.h"

#include "user_interface/field_related/motion_simulation_tools/field_vectors.h"

/* network port_, protocol and acknowledgement period */


#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

// useful macros




#define NET_VISION_PROTOCOL   QSocket::UDP
#define NET_VISION_ACK_PERIOD 30
#define NET_VISION_PORT       32881

struct net_vframe;
struct net_vconfig;
struct net_vref;

// ------------------------------------------------------------------
// Input Messages
// ------------------------------------------------------------------

#define NET_VISION_CONFIG   1
#define NET_VISION_REF      2
#define NET_VISION_SIM      3

//
// vconfig
//

struct vconfig_robot
{
        char id;             // The robot's id
        char type;           // The robot type (e.g. omni or diff)
}; // 2

#define VCOVER_NONE   0
#define VCOVER_NORMAL 1

struct vconfig_team
{
        char cover_type;
        vconfig_robot robots[MAX_TEAM_ROBOTS];
}; // 1+5*2 = 11

struct net_vconfig
{
        char msgtype; // = NET_VISION_CONFIG
        vconfig_team teams[NUM_TEAMS];
}; // 1+11*2 = 23

//
// vref
//

#define REF_STOP    0
#define REF_GO      1

struct net_vref
{
        char msgtype; // = NET_VISION_REF
        char refstate;
};

//
// vsim
//

#define VSIM_MOVEBALL  1
#define VSIM_MOVEROBOT 2

struct vsim_moveball
{
        MyVector2d_struct pos;
        MyVector2d_struct vel;
};

struct vsim_moverobot
{
        char team, array_id;
        MyVector2d_struct pos;
        double angle;
};

struct net_vsim
{
        char msgtype; // = NET_VISION_SIM
        char command; // = VSIM_*

        union
        {
                vsim_moveball moveball;
                vsim_moverobot moverobot;
        } info;
};

const int net_vision_in_maxsize = MAX(sizeof(net_vconfig),
                                      MAX(sizeof(net_vref),
                                          sizeof(net_vsim)));

// ------------------------------------------------------------------
// Output Messages
// ------------------------------------------------------------------

#define NET_VISION_FRAME   1

struct net_vframe;

struct VisionRawInfo
{
        double timestamp;
        vector2f pos;
        float angle;
        float conf;
}; // 8+4*2+4*2 = 24

struct vball
{
        struct
        {
                float x;
                float y;
                float vx;
                float vy;
        } state;
        float variances[16];

        VisionRawInfo vision;

        vector2f occluding_offset;
        char occluded;
        char occluding_team, occluding_robot;
}; // 20*4 + 24 + 8 + 3 = 115

struct vrobot
{
        struct
        {
                float x;
                float y;
                float theta;
                float vx;
                float vy;
                float vtheta;
                float stuck;
        } state;
        VisionRawInfo vision;
}; // 6*4 + 24 = 48

struct net_vframe
{
        char msgtype;

        double timestamp;

        vball ball;
        vrobot robots[NUM_TEAMS][MAX_TEAM_ROBOTS];

        char refstate;

        net_vconfig config;
}; // 8+8+104+48*2*5+8+23 = 631

const int net_vision_out_maxsize = sizeof(net_vframe);


#endif // FIELD_NETVISION_INTERFACE_H
