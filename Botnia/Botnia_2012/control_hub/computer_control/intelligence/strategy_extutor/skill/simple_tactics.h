// simple_tactics.h
//
// Very basic tactics.
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

#ifndef __simple_tactics_h__
#define __simple_tactics_h__

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include  "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"

class TStop : public Tactic
{
public:
    virtual const char *name() const
    {
        return "Stop";
    }
    TStop() : Tactic(false) { }

    static Tactic *parser(const char *param_string);
    virtual Tactic *clone() const
    {
        return new TStop(*this);
    }

    virtual void run(World &world, int me);
};

class TVelocity : public Tactic
{
    double vx, vy, va;

public:
    virtual const char *name() const
    {
        return "Velocity";
    }

    TVelocity(double _vx, double _vy, double _va) : Tactic(false)
    {
        vx = _vx;
        vy = _vy;
        va = _va;
    }

    static Tactic *parser(const char *param_string);
    virtual Tactic *clone() const
    {
        return new TVelocity(*this);
    }
    virtual void run(World &world, int me);
};

class SPosition : public RobotTactic
{
public:
    BCoordinate position;
    BCoordinate faceto;
    int obs;
    bool use_obsflags;
    virtual ~SPosition(){}   // if the class inharited is virtusl you should also use at here. {}
    virtual const char *name() const
    {
        return "Position";
    }
    /* lu_test
      _position: target position coordinate
      _faceto: target angle vector
    */
    SPosition(BCoordinate _position, BCoordinate _faceto) : RobotTactic(false)
    {
        //position = _position;
        position=BCoordinate(1000,1000);
        faceto = _faceto;
        use_obsflags = false;
    }
    SPosition(BCoordinate _position=BCoordinate(1000,1000), double angle=90.0) : RobotTactic(false)
    {
        position = _position;
        faceto = MyVector2d(1, 0).rotate(angle);
        use_obsflags = false;
    }

    SPosition(BCoordinate _position=BCoordinate(1000,1000), int _obs=0) : RobotTactic(false)
    {
        position = _position;
        faceto = MyVector2d(1, 0);
        obs = _obs;
        use_obsflags = true;
    }

    static Tactic *parser(const char *param_string);
    virtual Tactic *clone() const
    {
        return new SPosition(*this);
    }

    virtual void command(World &world, int me, Robot::RobotCommand &command,
                         bool debug);
};


class SDribbleToPosition : public RobotTactic
{
public:
    BCoordinate position;
    BCoordinate faceto;
    virtual const char *name() const
    {
        return "DribbleToPosition";
    }

    SDribbleToPosition(BCoordinate _position, BCoordinate _faceto)
        : RobotTactic(false)
    {
        position = _position;
        faceto = _faceto;
    }
    SDribbleToPosition(BCoordinate _position, double angle)
        : RobotTactic(false)
    {
        position = _position;
        faceto = MyVector2d(1, 0).rotate(angle);
    }

    static Tactic *parser(const char *param_string);
    virtual Tactic *clone() const
    {
        return new SDribbleToPosition(*this);
    }

    virtual void command(World &world, int me, Robot::RobotCommand &command,
                         bool debug);
};

#endif
