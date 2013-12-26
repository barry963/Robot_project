

#include "net/net_vision_gui_interface.h"
#include <deque>
#include <QPointF>
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_parametres.h"
#include "field_global_function.h"
#include "field_constants.h"




#include <QtGui/QApplication>
#include <QCoreApplication>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include "net/net_vision_interface.h"
#include "net/net_refbox_command.h"

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/play/play.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/anticipation_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/special_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/defense_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/move_as_sin_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/move_as_straight_tactic.h"

#include "control_hub/computer_control/intelligence/strategy_extutor/strategy.h"
#include "user_interface/field_related/field_global_function.h"
#include "user_interface/field_related/paint_tools/paint_buffer.h"


#define DEBUG

#define TEAMBLUE
const bool print_frames_missing = true;

///******************************* GLOBALS ****************************/

#define MAX_HOST   256
// MainWindow
Strategy strategy;
ParamSet *settings=NULL;

//// Gui Tactics.  These override strategy's tactics.
Tactic *gui_tactics[MAX_TEAM_ROBOTS] = { NULL, NULL, NULL, NULL, NULL};

deque<net_gdebug> gmsg_queue;
bool run;


char *tactic_string = NULL;


void gui_debug_point(const char robot, MyVector2d p1 )
{
        GuiCmd.AddPoint(p1.x,p1.y);
}

void gui_debug_line(const char robot, const char level,
                    MyVector2d p1, MyVector2d p2, char flags)
{
        GuiCmd.AddLine(p1.x,p1.y,p2.x,p2.y);

        //net_gdebug d = { NET_GUI_DEBUG_LINE, world.color, robot, level, world.time};
  //d.info.line.p[0].set(world.from_world(p1));
  //d.info.line.p[1].set(world.from_world(p2));
  //d.info.line.flags = flags;
}

void gui_debug_x(const char robot, const char level, MyVector2d p)
{
      gui_debug_line(robot, level, p + MyVector2d(20, 20), p + MyVector2d(-20, -20));
      gui_debug_line(robot, level, p + MyVector2d(-20, 20), p + MyVector2d(20, -20));
}

void gui_debug_arc(const char robot, const char level,
                   MyVector2d p1, MyVector2d dimens,
                   double start_angle, double stop_angle,
                   char flags)
{
    net_gdebug d = { NET_GUI_DEBUG_ARC, world.color_, robot, level, world.time};

    d.info.arc.center.set(world.from_world(p1));
    d.info.arc.dimens.set(dimens);
    d.info.arc.a1 = world.from_world_dir(start_angle);
    d.info.arc.a2 = world.from_world_dir(stop_angle);
    d.info.arc.flags = flags;

}

void gui_debug_printf(const char robot, const char level,
                      const char *fmt, ...)
{
    net_gdebug d = { NET_GUI_DEBUG_MSG, world.color_, robot, level, world.time};
    va_list ap;
    va_start(ap, fmt);

    vsprintf(d.info.msg.msg, fmt, ap);
}




void RegisterTactics()
{

        Tactic::registerParser("shoot", TShoot::parser);
        Tactic::registerParser("steal", TSteal::parser);
        Tactic::registerParser("clear", TClear::parser);

        Tactic::registerParser("move_to_middle", SPosition::parser);
        Tactic::registerParser("move_as_sin", TMoveAsSin::parser);
        Tactic::registerParser("move_as_straight", TMoveAsStraight::parser);

        Tactic::registerParser("position_for_start", TPositionForStart::parser);
        Tactic::registerParser("position_for_kick", TPositionForKick::parser);
        Tactic::registerParser("position_for_penalty", TPositionForPenalty::parser);
        Tactic::registerParser("position_for_rebound", TPositionForRebound::parser);
        Tactic::registerParser("position_for_pass", TPositionForPass::parser);
        Tactic::registerParser("position_for_deflection", TPositionForDeflection::parser);
        Tactic::registerParser("position_for_loose_ball", TPositionForLooseBall::parser);


        Tactic::registerParser("shoot_charge_ball", TChargeBall::parser);//lu_test->
        Tactic::registerParser("shoot_simple_shoot", TShoot::parser);
        Tactic::registerParser("shoot_dribble_shoot", TDribbleToShoot::parser);


        Tactic::registerParser("charge_ball", TChargeBall::parser);
        Tactic::registerParser("success", TSuccess::parser);
        Tactic::registerParser("complete", TComplete::parser);
        Tactic::registerParser("dribble_to_position", SDribbleToPosition::parser);
        Tactic::registerParser("position", SPosition::parser);
        Tactic::registerParser("stop", TStop::parser);
        Tactic::registerParser("block", TBlock::parser);
        Tactic::registerParser("goalie", TGoalie::parser);
        Tactic::registerParser("mark", TMark::parser);
        Tactic::registerParser("defend_point", TDefendPoint::parser);

        Tactic::registerParser("velocity", TVelocity::parser);
        Tactic::registerParser("defend_lane", TDefendLane::parser);
        Tactic::registerParser("defend_line", TDefendLine::parser);
        Tactic::registerParser("pass", TPass::parser);
        Tactic::registerParser("dribble_to_shoot", TDribbleToShoot::parser);
        Tactic::registerParser("active_def", TActiveDef::parser);
        Tactic::registerParser("dribble_to_region", TDribbleToRegion::parser);
        Tactic::registerParser("spin_to_region", TSpinToRegion::parser);
        Tactic::registerParser("receive_pass", TReceivePass::parser);
        Tactic::registerParser("receive_deflection", TReceiveDeflection::parser);//<- lu_test
}

QString application_path;
bool bSimServer;
bool bThreadMode;
char soccerside;
char soccerteam;

bool bEnCamera[2]={true,true};

bool bEnBallNoise=true;
bool bEnBigRobotID=false;
bool bEnBlueRobotNoise=true;
bool bEnYellowRobotNoise=true;


