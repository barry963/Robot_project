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

#ifndef __PATH_PLANNER_H__
#define __PATH_PLANNER_H__

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world_obstacle.h"
#include "control_hub/computer_control/knowledge_base/algorithms/path_planner/path_plan_tools/kdtree.h"

#define MAX_NODES  5000
#define MAX_WAYPTS 200
#define VERYNEAR 100
//#define MAX_NODES  800
//#define MAX_WAYPTS 200
//#define VERYNEAR 100

class PathPlanner
{
        state node[MAX_NODES];
        state goal;
        state out_of_obs;

        KDTree<state> tree;
        int num_nodes,max_nodes;
        static const int max_extend = 1;

        double goal_target_prob;
        double waypoint_target_prob;
        double step_size;

        obstacles *obs;
public:
        state waypoint[MAX_WAYPTS];
        int num_waypoints;

        //·滮Ӧ
        int robot_id;
public:
        void init(int _max_nodes,int _num_waypoints,
                  double _goal_target_prob,double _waypoint_target_prob,
                  double _step_size);
        double distance(state &s0,state &s1);
        //double distance(state s0,state s1);
        state random_state();

        state *add_node(state n,state *parent);
        state choose_target(int &targ);
        state *find_nearest(state target);

        int extend(state *s,state target);
        state plan(obstacles *_obs,int obs_mask,
                   state initial,state _goal,int &obs_id);
};

#endif /*__PATH_PLANNER_H__*/
