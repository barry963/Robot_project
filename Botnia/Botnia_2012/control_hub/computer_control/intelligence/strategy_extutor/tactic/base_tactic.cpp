#include  "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
// tactic.cc
//
// Parent class for tactics.
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu)
//
/* LICENSE:
  =========================================================================
    CMDragons'02 RoboCup F180 Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
                       James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>

#include "user_interface/field_related/motion_simulation_tools/field_util.h"
#include "user_interface/field_related/field_global_function.h"

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"

MyVector2d BCoordinate::asVectorNotAbsolute(World &w)
{
    MyVector2d v = c;
    switch (side)
    {
    case SBall:
        v.y *= w.sideBall();
        break;
    case SStrong:
        v.y *= w.sideStrong();
        break;
    case SBallOrStrong:
        v.y *= w.sideBallOrStrong();
        break;
    case SGui:
        v *= w.side_;
        break;
    case SAbsolute:
        break;
    }
    switch (origin)
    {
    case OBall:
        v += w.ball_position();
        break;
    case OAbsolute:
        break;
    }
    if (!dynamic)
    {
        c = v;
        origin = OAbsolute;
        side = SAbsolute;
        absolute = true;
    }
    return v;
}

//ŒÆËãÇøÓòÖÐÐÄ
MyVector2d BRegion::center(World &w)
{
    switch (type)
    {
    case Rectangle:
        return (p[0].asVector(w) + p[1].asVector(w)) / 2.0;
    case Circle:
    default:
        return p[0].asVector(w);
    }
}

//ŽÓÇøÓòÖÐËæ»ú²ÉÑùÒ»žöµã
MyVector2d BRegion::sample(World &w)
{
    switch (type)
    {
    case Rectangle:
    {
        MyVector2d v0 = p[0].asVector(w);
        MyVector2d v1 = p[1].asVector(w);
        double w = (rand() / (double) RAND_MAX * 2 * radius) - radius;
        double l = rand() / (double) RAND_MAX * (v0 - v1).length();
        return v0 + (v1 - v0).norm(l) + (v1 - v0).perp().norm(w);
    }
    case Circle:
    default:
    {
        double r = sqrt(rand() / (double) RAND_MAX) * radius;
        double a = rand() / (double) RAND_MAX * M_2PI;
        return p[0].asVector(w) + MyVector2d(r, 0).rotate(a);
    }
    }
}

MyVector2d BRegion::centerVelocity(World &w)
{
    switch (type)
    {
    case Rectangle:
        return (p[0].getVelocity(w) + p[1].getVelocity(w)) / 2.0;
    case Circle:
    default:
        return p[0].getVelocity(w);
    }
}

//»æÖÆ¶ÔÏó
void BRegion::gui_draw(int robot, int level, World &w)
{
    switch (type)
    {
    case Rectangle:
    {
        MyVector2d v0 = p[0].asVector(w);
        MyVector2d v1 = p[1].asVector(w);
        gui_debug_line(robot, level,
                       v0 - (v0 - v1).perp().norm(radius),
                       v0 + (v0 - v1).perp().norm(radius));
        gui_debug_line(robot, level,
                       v0 + (v0 - v1).perp().norm(radius),
                       v1 + (v0 - v1).perp().norm(radius));
        gui_debug_line(robot, level,
                       v1 + (v0 - v1).perp().norm(radius),
                       v1 - (v0 - v1).perp().norm(radius));
        gui_debug_line(robot, level,
                       v1 - (v0 - v1).perp().norm(radius),
                       v0 - (v0 - v1).perp().norm(radius));
        break;
    }
    case Circle:
        gui_debug_arc(robot, level, p[0].asVector(w),
                      MyVector2d(2 * radius, 2 * radius), 0, M_2PI);
        break;
    }
}

void BRegion::diagonal(World &w, MyVector2d x, MyVector2d &d1, MyVector2d &d2)
{
    switch (type)
    {
    case Rectangle:
    {
        MyVector2d v0 = p[0].asVector(w);
        MyVector2d v1 = p[1].asVector(w);
        MyVector2d c = center(w);
        MyVector2d r[4] =
        {
            v0 - (v0 - v1).perp().norm(radius),
            v0 + (v0 - v1).perp().norm(radius),
            v1 + (v0 - v1).perp().norm(radius),
            v1 - (v0 - v1).perp().norm(radius)
        };
        MyVector2d perp = (x - c).perp();
        d1 = segment_near_line(r[0], r[1], c, c + perp);
        if (d1 == r[0] || d1 == r[1])
            d1 = segment_near_line(r[1], r[2], c, c + perp);
        d2 = segment_near_line(r[2], r[3], c, c - perp);
        if (d2 == r[2] || d2 == r[3])
            d2 = segment_near_line(r[3], r[0], c, c - perp);
        break;
    }
    case Circle:
    default:
    {
        MyVector2d center = p[0].asVector(w);
        d1 = center + (x - center).perp().norm(radius);
        d2 = center - (x - center).perp().norm(radius);
        break;
    }
    }
}

//ÅÐ¶ÏµãÊÇ·ñÔÚÇøÓòÄÚ
bool BRegion::inRegion(World &w, MyVector2d x)
{
    switch (type)
    {
    case Rectangle:
    {
        MyVector2d v0 = p[0].asVector(w);
        MyVector2d v1 = p[1].asVector(w);
        return ((v0 - v1).dot(x - v1) > 0 &&
                (v1 - v0).dot(x - v0) > 0 &&
                fabs(distance_to_line(v0, v1, x)) < radius);
    }
    case Circle:
    default:
        return (x - center(w)).length() < radius;
    }
}

//------------------------------------------------------------------------------


deque<Tactic::registration> Tactic::strategy_register_list;
bool Tactic::registerParser(const char *name, parse_fn parser)
{
    strategy_register_list.push_back(registration(name, parser));
    return true;
}


// this function is used to parse the string that is given in the GUI
Tactic *Tactic::parse(const char *string)
{
    if (strncmp("NULL", string, 4) == 0 ||
            strncmp("None", string, 4) == 0 ||
            strncmp("none", string, 4) == 0)
    {
        return NULL;
    }
    int regsize=strategy_register_list.size();
    for (int i=0; i<regsize; i++)
    {
        int nlen = strlen(strategy_register_list[i].name);
        if (strncmp(strategy_register_list[i].name, string, nlen) == 0 &&
                (string[nlen] == ' ' || string[nlen] == '\0' ||
                 string[nlen] == '\n' || string[nlen] == '\t'))
        {
            return (strategy_register_list[i].parser)(string + nlen);
        }
    }
    return NULL;
}

//Ñ¡ÔñÆÀ¹ÀÊ±Œä×î¶ÌµÄ
//bias Ê±ŒäÆ«²îÁÐ±í
int Tactic::selectRobot(World &world, bool candidates[], double bias[])
{
    int best = -1;
    double best_t = 0.0;
    for (int i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        if (!candidates[i]) continue;
        double t = estimatedTime(world, i) + bias[i];
        if (best < 0 || t < best_t)
        {
            best = i;
            best_t = t;
        }
    }
    return best;
}

void RobotTactic::makeCommand(World &world, int me, bool debug,
                              Robot::RobotCommand &c,
                              bool &ignore_status)
{
    // Set defaults type for go to point
    c.goto_point_type = Robot::GotoPointMove;
    // check whether the ball is out of boundary or not
    // if this tactic needs to control the ball, but the ball is out then deal it with "freeKick method"
    if (manipulates_ball && world.ballOutOfPlay())
    {
        qDebug()<<"ball is out of border, freeKick method start";//lu_test
        MyVector2d fk = world.freeKickPosition(world.ball_position());
        c.cmd = Robot::CmdPosition;

        //c.target = fk + (world.our_goal - fk).norm(170.0 + world.teammate_radius(me));//lu_test
        c.target = world.GetRobotPositionByID(me);//stop immediately after the ball is out of border

        c.velocity = MyVector2d(0, 0);
        c.angle = (fk - world.our_goal).angle();
        c.observation_type = OBS_EVERYTHING_BUT_ME(me);
        if (debug)
        {
            gui_debug_line(me, GDBG_TACTICS, world.GetRobotPositionByID(me),
                           c.target, G_ARROW_FORW);
            gui_debug_line(me, GDBG_TACTICS, fk, c.target);
        }
        ignore_status = true;
    }
    else
    {
        // make command for going to some position
        command(world, me, c, debug);
        ignore_status = false;
    }
}
