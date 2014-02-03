// ball_tactics.h
//
// Ball tactics.
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

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"

#include "user_interface/field_related/field_global_function.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"
#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"

CR_DECLARE(SHOOT_AIM_PREF_AMOUNT);
CR_DECLARE(SHOOT_MIN_ANGLE_TOLERANCE);
CR_DECLARE(SHOOT_DRIBBLE_IF_NO_SHOT);
CR_DECLARE(PASS_TARGET_WIDTH);

inline static void cr_do_setup()
{
    static bool cr_setup = false;
    if (!cr_setup)
    {
        CR_SETUP(tactic, SHOOT_AIM_PREF_AMOUNT, CR_DOUBLE);
        CR_SETUP(tactic, SHOOT_MIN_ANGLE_TOLERANCE, CR_DOUBLE);
        CR_SETUP(tactic, SHOOT_DRIBBLE_IF_NO_SHOT, CR_INT);
        CR_SETUP(tactic, PASS_TARGET_WIDTH, CR_DOUBLE);
        cr_setup = true;
    }
}

TShoot::TShoot(Type _type, int _deflect_target) : RobotTactic(true, true)
{

    type = _type;
    //ʽŵķ
    deflect_target = _deflect_target;
    prev_target_set = false;
    //Ĭֻ10
    eval.set(
        BRegion(BCoordinate(0, 0,
                            BCoordinate::SBall,
                            BCoordinate::OBall, true),
                800),
        &eval_fn,
        0.2442
    );
    deflect_target = _deflect_target;
        //cr_do_setup();
}

void TShoot::LoadConfig()
{
    CR_SETUP(tactic, SHOOT_AIM_PREF_AMOUNT, CR_DOUBLE);
    CR_SETUP(tactic, SHOOT_MIN_ANGLE_TOLERANCE, CR_DOUBLE);
    CR_SETUP(tactic, SHOOT_DRIBBLE_IF_NO_SHOT, CR_INT);
    CR_SETUP(tactic, PASS_TARGET_WIDTH, CR_DOUBLE);
}

Tactic *TShoot::parser(const char *param_string)
{
    Type type;
    int target;
    char t;
    param_string += Parse::pChar(param_string, t);

    switch (t)
    {
    case 'N':
        type = NoAim;
        break;
    case 'D':
        type = Deflect;
        break;
    case 'A':
    default:
        type = Aim;
        break;
    }
    if (type == Deflect)
    {
        Parse::pInt(param_string, target);
    }
    TShoot* tac=new TShoot(type, target);
    if (tac)
    {
        tac->LoadConfig();
    }
    return tac;
}

void TShoot::command(World &world, int me, Robot::RobotCommand &command,
                     bool debug)
{
    MyVector2d ball = world.ball_position();
    MyVector2d robot_position = world.GetRobotPositionByID(me);
    MyVector2d target;
    double angle_tolerance;

    if (!prev_target_set)
    {
        prev_target = (ball - (robot_position - ball));//Ϊģ˵ĶԳƵ
        prev_target_set = true;
    }
    Type the_type = type;

    if (world.twoDefendersInTheirDZone())
    {
        the_type = NoAim;
    }
    bool got_target = false;
    //# The amount we'd prefer to shoot at our previous angle.  If an another
    //#  at least this much bigger appears we'll switch to that.
    //SHOOT_AIM_PREF_AMOUNT = 0.01745 # 1 degree

    if (the_type == Aim)
    {
        got_target = evaluation.aim(world, world.now, world.ball_position(),
                                    world.their_goal_r,
                                    world.their_goal_l,
                                    OBS_EVERYTHING_BUT_ME(me),
                                    prev_target, DVAR(SHOOT_AIM_PREF_AMOUNT),
                                    target, angle_tolerance);
    }

    if (the_type == Deflect)
    {
        MyVector2d t = world.GetRobotPositionByID(getTeammateId(deflect_target));
        MyVector2d toward = t - ball;
        MyVector2d towardperp = toward.perp();
        if (towardperp.x < 0)
        {
            towardperp *= -1;
        }
        got_target = evaluation.aim(world, world.now, world.ball_position(),
                                    t + towardperp.norm(80.0),
                                    t - towardperp.norm(40.0),
                                    OBS_EVERYTHING_BUT_US,
                                    prev_target, DVAR(SHOOT_AIM_PREF_AMOUNT),
                                    target, angle_tolerance);
        got_target = true;
        if ((target - ball).length() < 400.0)
        {
            target = (ball + target) / 2.0;
        }
        else
        {
            target = target + (ball - target).norm(200.0);
        }
    }
    if (the_type == NoAim || (!got_target && !IVAR(SHOOT_DRIBBLE_IF_NO_SHOT)))
    {
        // Guaranteed to return true and fill in the parameters when
        // obs_flags is empty.
        evaluation.aim(world, world.now, world.ball_position(),
                       world.their_goal_r, world.their_goal_l, 0,
                       target, angle_tolerance);
        got_target = true;
    }
    if (got_target)
    {

        if (debug)
        {
            gui_debug_line(me, GDBG_TACTICS, ball, target);
            gui_debug_line(me, GDBG_TACTICS, ball,
                           (target - ball).rotate(angle_tolerance) + ball);
            gui_debug_line(me, GDBG_TACTICS, ball,
                           (target - ball).rotate(-angle_tolerance) + ball);
        }

        //# We make sure the angle tolerance is always this big.
        //SHOOT_MIN_ANGLE_TOLERANCE = 0.1745 # Pi / 16
        if (angle_tolerance < DVAR(SHOOT_MIN_ANGLE_TOLERANCE))
        {
            angle_tolerance = DVAR(SHOOT_MIN_ANGLE_TOLERANCE);
        }
        //target ײŵ
        prev_target = target;
        // Aim for point in front of obstacles.
        MyVector2d rtarget;
        MyVector2d targ_ball;
        targ_ball = target - ball;

        world.obsLineFirst(target-targ_ball.bound(500)*0.75, target,
                           OBS_OPPONENTS /*| OBS_THEIR_DZONE*/, rtarget);
        command.cmd = Robot::CmdMoveBall;
        command.ball_target = target;
        command.target = rtarget;
        command.angle_tolerance = angle_tolerance;
        command.ball_shot_type = Robot::BallShotOnGoal;

    }
    else
    {
        //ʼ
        //getObsFlags 㼺Ϊϰ־
        //Ϊģ400Ϊ뾶10㣬ѰõĿŵ
        //???ΪʲôֻҷΪϰ
        eval.update(world, getObsFlags());
        //˴򵽿
        command.cmd = Robot::CmdDribble;
        command.target = eval.point();
        command.ball_target = eval.point();
        command.angle = eval.angle();
        if (debug)
        {
            eval.region.gui_draw(me, GDBG_TACTICS, world);
            gui_debug_x(me, GDBG_TACTICS, command.target);
        }
    }
}

double TShoot::successProb(World &world)
{
    Robot::RobotCommand c;
    command(world, 0, c, false);
    return bound(c.angle_tolerance / M_PI_16, 0, 1);
}

double TShoot::eval_fn(World &world, const MyVector2d p,
                       int obs_flags, double &a)
{
    MyVector2d target;
    double tolerance;
    obs_flags |= OBS_OPPONENTS | OBS_OUR_DZONE | OBS_THEIR_DZONE | OBS_WALLS;

    if (world.obsPosition(p, obs_flags))
    {
        return -1;
    }

    obs_flags = OBS_OPPONENTS;
    for (int i=0; i<world.n_opponents; i++)
    {
        if (point_on_segment(p, world.ball_position(), world.opponent_position(i))
                == world.ball_position())
        {
            obs_flags &= ~OBS_OPPONENT(i);
        }
    }

    if (world.obsLine(p, world.ball_position(), obs_flags))
    {
        return -1;
    }
    //


    if (evaluation.aim(world, world.now, p,
                       world.their_goal_r, world.their_goal_l,
                       OBS_OPPONENTS, target, tolerance))
    {

        a = (target - p).angle();
        return tolerance;
    }
    else
    {
        return 0.0;
    }
}


Tactic *TSteal::parser(const char *param_string)
{
    int n = Parse::skipWS(param_string, " \t");
    if (param_string[n] == '\0' || param_string[n] == '\n')
    {
        return new TSteal();
    }
    BCoordinate t;
    Parse::pTCoordinate(param_string, t);
    return new TSteal(t);
}

void TSteal::command(World &world, int me, Robot::RobotCommand &command,
                     bool debug)
{
    command.cmd = Robot::CmdSteal;
    command.target = world.ball_position();
    command.ball_target = target_set ? target.asVector(world) : (world.ball_position() + world.robot[me]->sensors.good_ball_dir * 300.0);
    if (debug)
    {
        gui_debug_line(me, GDBG_TACTICS, world.GetRobotPositionByID(me),
                       world.ball_position(), G_ARROW_FORW);
        gui_debug_line(me, GDBG_TACTICS, world.ball_position(),
                       command.target, G_ARROW_FORW);
    }
}

// 1. if can find an angel to shoot than shoot
// 2. the can find a gap to shoot to their field than shoot
// 3 . if cannot find than ignore the obstacle shoot to their field
void TClear::command(World &world, int me, Robot::RobotCommand &command,
                     bool debug)
{
    MyVector2d ball = world.ball_position();
    MyVector2d target;
    double angle_tolerance;
    bool aimed = false;
    MyVector2d downfield[2];
    Robot::BallShotType shot_type= Robot::BallShotClear;
    downfield[0].set(ball.x + 180.0, -FIELD_WIDTH_H);
    downfield[1].set(ball.x + 180.0,  FIELD_WIDTH_H);
    if (debug)
    {
        gui_debug_line(me, GDBG_TACTICS, downfield[0], downfield[1]);
    }
    if (!prev_target_set) prev_target = world.their_goal;

    aimed = evaluation.aim(world, world.now, world.ball_position(),
                           downfield[0], downfield[1],
                           OBS_EVERYTHING_BUT_ME(me),
                           prev_target, DVAR(SHOOT_AIM_PREF_AMOUNT),
                           target, angle_tolerance);

    if (!aimed)
    {
        // Guaranteed to return true and fill in the parameters when
        // obs_flags is empty.
        //ȷtrue
        evaluation.aim(world, world.now, world.ball_position(),
                       downfield[0], downfield[1],
                       0, target, angle_tolerance);
    }
    target = (target - ball).norm(MIN(FIELD_LENGTH_H - ball.x, 1000.0)) + ball;
    // If the target tolerances include the goal then just aim there.
    double a = (target - ball).angle();
    double a_to_goal = (world.their_goal - ball).angle();
    //double a_to_goal_l = (world.their_goal_l - ball).angle();
    //double a_to_goal_r = (world.their_goal_r - ball).angle();
    MyVector2d rtarget;
    rtarget=target;

    if (fabs(anglemod(a - a_to_goal)) < 0.8 * angle_tolerance)
    {
        bool got_target;
        got_target = evaluation.aim(world, world.now, world.ball_position(),
                                    world.their_goal_r,
                                    world.their_goal_l,
                                    OBS_EVERYTHING_BUT_ME(me),
                                    prev_target, DVAR(SHOOT_AIM_PREF_AMOUNT),
                                    target, angle_tolerance);
        if (got_target)
        {

            prev_target = target;
            // Aim for point in front of obstacles.
            MyVector2d targ_ball;
            targ_ball = target - ball;

            world.obsLineFirst(target-targ_ball.bound(500)*0.75, target,
                               OBS_OPPONENTS /*| OBS_THEIR_DZONE*/, rtarget);
            angle_tolerance = angle_tolerance;
            shot_type = Robot::BallShotOnGoal;
        }
    }
    if (debug)
    {
        gui_debug_line(me, GDBG_TACTICS, ball, target);
        gui_debug_line(me, GDBG_TACTICS, ball,
                       (target - ball).rotate(angle_tolerance) + ball);
        gui_debug_line(me, GDBG_TACTICS, ball,
                       (target - ball).rotate(-angle_tolerance) + ball);
    }

    if (angle_tolerance < DVAR(SHOOT_MIN_ANGLE_TOLERANCE))
    {
        angle_tolerance = DVAR(SHOOT_MIN_ANGLE_TOLERANCE);
    }
    prev_target = target;
    prev_target_set = true;
    command.cmd = Robot::CmdMoveBall;
    command.target = rtarget;
    command.ball_target = target;
    command.angle_tolerance = angle_tolerance;
    command.ball_shot_type = shot_type;
}


Tactic *TActiveDef::parser(const char *param_string)
{
    int n = Parse::skipWS(param_string, " \t");
    if (param_string[n] == '\0' || param_string[n] == '\n')
    {
        return new TActiveDef();
    }
    BCoordinate t;
    Parse::pTCoordinate(param_string, t);
    return new TActiveDef(t);
}

void TActiveDef::command(World &world, int me, Robot::RobotCommand &command,
                         bool debug)
{
    //Է,͵սԷ볡
    if (world.possession == World::TheirBall)
    {
        steal.command(world, me, command, debug);
    }
    else
    {
        clear.command(world, me, command, debug);
    }
}

TPass::TPass(int _target)
{
    cr_do_setup();
    target = _target;
}

Tactic *TPass::parser(const char *param_string)
{
    int target;
    Parse::pInt(param_string, target);
    return new TPass(target);
}

void TPass::command(World &world, int me, Robot::RobotCommand &command,
                    bool debug)
{
    qDebug()<<"TPass command: "<<target;
    MyVector2d p[2], targetp, ball;
    double angle_tolerance;
    targetp = world.GetRobotPositionByID(1);

//    targetp = world.GetRobotPositionByID(getTeammateId(target)); //lu_test arbitratry getTeammateId

//    qDebug()<<"pass target: "<<world.GetRobotPositionByID(1).x<<world.GetRobotPositionByID(1).y;

//    qDebug()<<"me id: "<<me<<"location"<<world.GetRobotPositionByID(me).x<<world.GetRobotPositionByID(me).y;
//    qDebug()<<"target id: "<<target<<"location"<<world.GetRobotPositionByID(target).x<<world.GetRobotPositionByID(target).y;

    ball = world.ball_position();
    targetp += (ball - targetp).norm(70.0);//something wrong here, targetp+= lu_test
    p[0] = targetp + (targetp - ball).perp().norm(-DVAR(PASS_TARGET_WIDTH));//tactic.cfg 30.0
    p[1] = targetp + (targetp - ball).perp().norm(DVAR(PASS_TARGET_WIDTH));
    evaluation.aim(world, world.now, world.ball_position(),
                   p[0], p[1],
                   OBS_EVERYTHING_BUT_ME(me) & ~OBS_TEAMMATE(target),
                   targetp, angle_tolerance);
    if (debug)
    {
        gui_debug_line(me, GDBG_TACTICS, ball, targetp);
        gui_debug_line(me, GDBG_TACTICS, ball,
                       (targetp - ball).rotate(angle_tolerance) + ball);
        gui_debug_line(me, GDBG_TACTICS, ball,
                       (targetp - ball).rotate(-angle_tolerance) + ball);
    }
    // Set the drive target as 1m from the target, with some exceptions
    // when close.
    MyVector2d mytarget;
    if ((targetp - ball).length() > 1100)//too far
    {
        mytarget = ball + (targetp - ball).norm(1000); //1000 distance target
    }
    else if ((targetp - ball).length() < 100)//too closed
    {
        mytarget = targetp;
    }
    else
    {
        mytarget = targetp + (ball - targetp).norm(100); //minus 100 distance
    }
    command.cmd = Robot::CmdMoveBall;
    command.target = mytarget; // lu_test change from targetp to mytarget
    command.ball_target = targetp;
    command.angle_tolerance = angle_tolerance;
    command.ball_shot_type = Robot::BallShotPass;

    //command.observation_type=OBS_EVERYTHING_BUT_BALL;//LU_TEST
}

Tactic *TDribbleToShoot::parser(const char *param_string)
{
    BRegion r;
    param_string += Parse::pTRegion(param_string, r);
    return new TDribbleToShoot(r);
}

double TDribbleToShoot::eval_fn(World &world, const MyVector2d p,
                                int obs_flags, double &a)
{
    MyVector2d target;
    double tolerance;
    obs_flags |= OBS_OPPONENTS | OBS_OUR_DZONE | OBS_THEIR_DZONE;
    //ϰ
    if (world.obsPosition(p, obs_flags))
    {
        a = 0.0;
        return -1;
    }
    //ŵǶȷΧΪ׼
    if (evaluation.aim(world, world.now, p,
                       world.their_goal_r, world.their_goal_l,
                       OBS_OPPONENTS, target, tolerance))
    {
        a = (target - p).angle();
        return tolerance;
    }
    else
    {
        return 0.0;
    }
}

void TDribbleToShoot::command(World &world, int me,
                              Robot::RobotCommand &command,
                              bool debug)
{
    eval.addPoint(world.GetRobotPositionByID(me));
    eval.update(world, getObsFlags());
    command.cmd = Robot::CmdDribble;
    command.target = eval.point();
    command.ball_target = eval.point();
    command.angle = eval.angle();
    if (debug)
    {
        eval.region.gui_draw(me, GDBG_TACTICS, world);
        gui_debug_x(me, GDBG_TACTICS, command.target);
    }
}


Tactic *TDribbleToRegion::parser(const char *param_string)
{
    BRegion r;
    param_string += Parse::pTRegion(param_string, r);
    return new TDribbleToRegion(r);
}

//ָ
void TDribbleToRegion::command(World &world, int me,
                               Robot::RobotCommand &command,
                               bool debug)
{
    command.cmd = Robot::CmdDribble;
    command.target = region.center(world);
    command.ball_target = region.center(world);
    command.angle = (region.center(world) - world.GetRobotPositionByID(me)).angle();
    if (debug)
    {
        region.gui_draw(me, GDBG_TACTICS, world);
        gui_debug_x(me, GDBG_TACTICS, command.target);
    }
}

Tactic *TSpinToRegion::parser(const char *param_string)
{
    BRegion r;
    param_string += Parse::pTRegion(param_string, r);
    return new TSpinToRegion(r);
}

void TSpinToRegion::command(World &world, int me,
                            Robot::RobotCommand &command,
                            bool debug)
{
    command.cmd = Robot::CmdSteal;
    command.target = world.ball_position();
    command.ball_target = region.center(world);
    if (debug)
    {
        region.gui_draw(me, GDBG_TACTICS, world);
        gui_debug_x(me, GDBG_TACTICS, command.target);
    }
}

void TReceivePass::command(World &world, int me, Robot::RobotCommand &command,
                           bool debug)
{
    command.cmd = Robot::CmdRecieveBall;
}

void TReceiveDeflection::command(World &world, int me,
                                 Robot::RobotCommand &command,
                                 bool debug)
{
    MyVector2d mypos = world.GetRobotPositionByID(me);
#if 0
    MyVector2d ball = world.ball_position();
    MyVector2d ballv = world.ball_velocity();
    double t = closest_point_time(ball, ballv, mypos, MyVector2d(0, 0));
    MyVector2d target = (ball + ballv * t);
    target += (target - world.their_goal).norm(40.0);
    gui_debug_line(me, GDBG_TACTICS, world.teammate_position(me), target,
                   G_ARROW_FORW);
    if (ballv.length() < 100 || ballv.dot(mypos - ball) < 0)
    {
        command.cmd = Robot::CmdPosition;
        command.target = world.teammate_position(me);
        command.velocity = MyVector2d(0, 0);
        command.angle = world.teammate_direction(me);
        command.obs = 0;
    }
    else if ((target - mypos).length() < 100.0 && t < 1.0)
    {
        command.cmd = Robot::CmdSpin;
        command.target = target;
        command.ball_target = world.their_goal;
        got_to_spin = true;
    }
    else
    {
        command.cmd = Robot::CmdPosition;
        command.target = target;
        command.velocity = MyVector2d(0, 0);
        command.angle = (target - mypos).angle();
        command.obs = OBS_EVERYTHING_BUT_ME(me) & ~OBS_BALL;
    }
#else
    MyVector2d target = mypos;
    command.cmd = Robot::CmdPosition;
    command.target = target;
    command.velocity = MyVector2d(0, 0);
    command.angle = (target - mypos).angle();
    command.observation_type = OBS_EVERYTHING_BUT_ME(me) & ~OBS_BALL;
#endif
}

Status TReceiveDeflection::isDone(World &world, int me)
{
    MyVector2d ball = world.ball_position();
    MyVector2d ballv = world.ball_velocity();
    MyVector2d mypos = world.GetRobotPositionByID(me);
    if (ballv.length() < 100 || ballv.dot(mypos - ball) < 0)
    {
        return got_to_spin ? Completed : Aborted;
    }
    else
    {
        return InProgress;
    }
}







