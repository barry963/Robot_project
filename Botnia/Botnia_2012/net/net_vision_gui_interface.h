#ifndef FIELD_NETVISIONGUI_INTERFACE_H
#define FIELD_NETVISIONGUI_INTERFACE_H


#include "control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_geometry.h"
#include <string.h>

#define NET_GUI_PROTOCOL    Socket::TCP
#define NET_GUI_ACK_PERIOD  1
#define NET_GUI_PORT        32883

// ------------------------------------------------------------------
// Output Messages
// ------------------------------------------------------------------

// Debug levels
#define GDBG_VISION      0x01
#define GDBG_TRACKER     0x02
#define GDBG_MOTION      0x04
#define GDBG_NAVIGATION  0x08
#define GDBG_TACTICS     0x10
#define GDBG_STRATEGY    0x20


#define NET_GUI_DEBUG_LINE  1
#define NET_GUI_DEBUG_ARC   2
#define NET_GUI_DEBUG_MSG   3

//
// net_gdebug
//

struct net_gdebug;

#define G_ARROW_FORW    (1 << 1)
#define G_ARROW_BACK    (1 << 2)
#define G_ARROW_BOTH    (G_ARROW_FORW | G_ARROW_BACK)

struct gdebug_line
{
        MyVector2d_struct p[2];  // Two points determine a line
        char flags;     // Flags... see above
};

#define G_ARC_FILL      (1 << 1)

struct gdebug_arc
{
        MyVector2d_struct center, dimens;
        double a1, a2;
        char flags;     // Flags... see above
};

#define G_MSG_MAXLENGTH 256

struct gdebug_msg
{
        char msg[G_MSG_MAXLENGTH];
};

struct net_gdebug
{
        char msgtype; // = NET_GUI_DEBUG_*

        char team;
        char robot;   // -1:    No particular robot
        char level;   // 0-100: Where high means more detail.

        double timestamp;

        union
        {
                gdebug_line line;
                gdebug_arc arc;
                gdebug_msg msg;
        } info;

        int size() const;
};

inline int net_gdebug::size() const
{
        return sizeof(net_gdebug) -
               (msgtype != NET_GUI_DEBUG_MSG ? 0 :
                (G_MSG_MAXLENGTH - (strlen(info.msg.msg) + 1)));
}

const int net_gui_out_maxsize = sizeof(net_gdebug);

// ------------------------------------------------------------------
// Input Messages
// ------------------------------------------------------------------

#define NET_GUI_TACTIC 101

struct net_gtactic;

#define G_TACTIC_MAXLENGTH 256

struct net_gtactic
{
        char msgtype; // = NET_GUI_TACTIC
        char robot;
        char string[G_TACTIC_MAXLENGTH];

        int size() const;
};

inline int net_gtactic::size() const
{
        return sizeof(net_gtactic) - (G_TACTIC_MAXLENGTH - (strlen(string) + 1));
}

const int net_gui_in_maxsize = sizeof(net_gtactic);



#endif // FIELD_NETVISIONGUI_INTERFACE_H
