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
#include <assert.h>

#include <QDebug>

#include "control_hub/computer_control/intelligence/world_analysor/analizing_tools/field_geometry.h"
#include "user_interface/field_related/field_constants.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/thread_arismatic_tools/algorithm_base.h"
#include "user_interface/field_related/field_global_function.h"

#include "control_hub/computer_control/knowledge_base/database/world_state/field_world_obstacle.h"
#include "path_planner.h"

//Ч?·??ж??λá?
//?λ????·?????·
/*
extern xwin field;
void DrawLine(xdrawable &w,double x1,double y1,double x2,double y2);
*/

const bool plan_print = false;
const double out_of_obs_dot = 0.40; // 0.1; // 0.7071;
unsigned int debugfreq=0;

//?Χ?[-1,1]
double sdrand48()
{
    return(2*drand48()-1);
}

//?λ
state PathPlanner::random_state()
{
    state s;
    s.pos.set((FIELD_LENGTH_H+GOAL_DEPTH)*sdrand48(),FIELD_WIDTH_H*sdrand48());
    // s.vel.set(0,0);
    s.parent = NULL;
    return(s);
}

//??
//·???
void PathPlanner::init(int _max_nodes,int _num_waypoints,
                       double _goal_target_prob,//=0.15
                       double _waypoint_target_prob,//0.75
                       double _step_size)
// _step_size=100
{
    int i;
    vector2f minv,maxv;
    max_nodes = _max_nodes;
    num_waypoints = _num_waypoints;
    goal_target_prob = _goal_target_prob;
    waypoint_target_prob = _waypoint_target_prob;
    step_size = _step_size;
    for (i=0; i<num_waypoints; i++)
    {
        waypoint[i] = random_state();
    }
    mzero(out_of_obs);
    minv.set(-FIELD_LENGTH_H-GOAL_DEPTH,-FIELD_WIDTH_H);
    maxv.set( FIELD_LENGTH_H+GOAL_DEPTH, FIELD_WIDTH_H);
    tree.setdim(minv,maxv,16,8);//nleaf_size=16  nmax_depth=8
}

//????;
double PathPlanner:: distance(state &s0,state &s1)
//double PathPlanner:: distance(state s0,state s1)
{
    float dx,dy;
//    qDebug()<<"internal s0 x:"<<s0.pos.x << "y:" << s0.pos.y ;
//    qDebug()<<"internal s1 x:"<<s1.pos.x << "y:" << s1.pos.y ;
    dx = s1.pos.x - s0.pos.x;
    dy = s1.pos.y - s0.pos.y;
    return(sqrt(dx*dx + dy*dy));
    // return(MyVector::distance(s0.pos,s1.pos));
}

// add a child note to the parent node
state *PathPlanner::add_node(state n,state *parent)
{
#if 1//lu_test change 0 to 1
    if (num_nodes > max_nodes)
    {
        qDebug()<<"max nodes reached...";
        return(NULL);
    }
#endif
    n.parent = parent;
    // store the state of the node
    node[num_nodes] = n;
    // attach the node to the tree
    tree.add(&node[num_nodes]);

    // increase the node number by 1
    num_nodes++;

    //	if(parent){
    //           field.setColor(0,0,0);
    //           DrawLine(field,parent->pos.x,parent->pos.y,n.pos.x,n.pos.y);
    //	}

    return(&node[num_nodes-1]);
}

//???????
state PathPlanner::choose_target(int &targ)
{
    double p = drand48();
    int i;
    if (p < goal_target_prob)
    {
        targ = 0;
        return(goal);
    }
    else if (p < goal_target_prob+waypoint_target_prob)
    {
        targ = 1;
        i = lrand48() % num_waypoints;
        return(waypoint[i]);
    }
    else
    {
        targ = 2;
        return(random_state());
    }
}

//????·???
state *PathPlanner::find_nearest(state target)
{
    state *nearest;
    double d,nd;
    float td;
    int i;
    td = 0;
    nearest = tree.nearest(td,target.pos);
    //?????
    if (!nearest)
    {
        //??
        // NOTE: something bad must have happened if we get here.
        // find closest current state
        nearest = &node[0]; // num_nodes-1];
        //assert(nearest!=NULL); Lu_test command
        nd = distance(*nearest, target);
        for (i=0; i<num_nodes; i++)
        {
            d = distance(node[i], target);
            if (d < nd)
            {
                nearest = &node[i];
                nd = d;
            }
        }
    }
    return(nearest);
}

int PathPlanner::extend(state *s,state target)
{
    state n;
    vector2f step,p;
    vector2f f,fg;
    int num = 0;
    int id;
    double d,a,r;
    step = target.pos - s->pos;
    d = step.length();
    if (d < step_size)
    {
        return(0);
    }
    //??step_size?
    step *= step_size / d;
    //??step?
    n.pos = s->pos + step;
    // n.vel = s->vel;
    //s?
    if (!obs->check(*s,id))
    {
        //???λ?
        f  = obs->repulse(*s);
        //????
        fg = n.pos - s->pos;
        //С?????Χ
        if (f.dot(fg) > out_of_obs_dot)
        {
            // leaving obstacle, ok
            // printf("out <%f,%f> <%f,%f> %f\n",V2COMP(f),V2COMP(fg),f.dot(fg));
        }
        else
        {
            // fail if would go into obstacle more
            return(num);
        }
    }
    //n?
    else if (!obs->check(n,id))
    {
        if (obs->obs[id].type == OBS_CIRCLE)
        {//??
            // find tangent angle
            //???н
            p = obs->obs[id].pos;
            r = max(obs->obs[id].rad.x,obs->obs[id].rad.y);
            d = MyVector::distance(n.pos,p);
            a = asin(max(r,d) / d);
            // try each one
            n.pos = s->pos + step.rotate(a);
            if (!obs->check(n))
            {
                n.pos = s->pos + step.rotate(-a);
                if (!obs->check(n))
                {
                    //е??????
                    return(num);
                }
            }
        }
    }
    // add node
    s = add_node(n,s);
    num++;
    return(num);
}

//·??·
state PathPlanner::plan(obstacles *_obs,int obs_mask,
                        state initial,state _goal,int &obs_id,double current_angle)
{
    state target,*nearest,*nearest_goal,*p,*head;
    vector2f f;
    double d,nd,s;
    int i,iter_limit;
    int targ_type;
    bool ok;
    bool inobs;
    goal = _goal;
    obs = _obs;
    obs->set_mask(obs_mask);
    inobs = false;
    tree.clear();

    d = MyVector::distance(initial.pos,goal.pos);
    // check for small trivial plan
    // if the distance between the initial point and the target is too small than return target
    if (d < VERYNEAR)
    {
        if (plan_print)
        {
            printf("  PP: short\n");
        }
        //goal?????????????10%
        target = goal;
        s = 1.0;
        do
        {
            target.pos = initial.pos*(1-s) + goal.pos*s;
            ok = obs->check(initial,target);
            s -= 0.1;
        }
        while (s>0 && !ok);

#if 1//lu_test change 0 to 1
        if (bDebugPathPlan)
        {
            for(int i=0;i++;)
            //gui_debug_robot(initial.pos,180);//Lu_test
            gui_debug_line(0,0,initial.pos,target.pos);
        }
#endif
        return(target);
    }
    //if there is no obstacles, return the target
    if (obs->check(initial,goal))
    {
        if (plan_print)
        {
            printf("  PP: no obs\n");
        }
#if 1//lu_test change 0 to 1
        if (bDebugPathPlan)
        {
            if(debugfreq++>3)
            {
                gui_debug_robot(initial.pos,180);//Lu_test
                debugfreq=0;
            }
            gui_debug_line(0,0,initial.pos,goal.pos);
        }
#endif
        // no obstacles in the way
        return(goal);
        /*
     }else if(!obs->check(initial)){
       if(plan_print) printf("  PP: obs avoid\n");
       // in an obstacle, use gradient to leave
       target = initial;
       f = obs->repulse(initial);
       fg = goal.pos - initial.pos;
       // printf("[%f,%f],[%f,%f]",V2COMP(f),V2COMP(fg));
       if(f.dot(fg) > 0){
  // already leaving the obstacle
  // printf(".");
  target.pos += fg.norm()*100;
  inobs = true;
       }else{
  // leave obstacle and move at target
  target.pos += f.norm()*100 + fg.norm()*100;
  inobs = true;
       }
       return(target);
     */
    }
    else//obstacles
    {
        //???
        if (plan_print)
        {
            printf("  PP: plan(%f,%f)->(%f,%f)\n",
                   V2COMP(initial.pos),V2COMP(goal.pos));
        }

        i = num_nodes = 0;
        //?
        nearest = nearest_goal = add_node(initial,NULL);
        //assert(nearest!=NULL); Lu_test
        d = distance(*nearest,goal);// Lu_test
        // plan
        iter_limit = max_nodes;
        while (i<iter_limit && num_nodes<max_nodes && d>VERYNEAR)
        {
            target = choose_target(targ_type);
            //find_nearest ????target?
            //targ_type????
            nearest = (targ_type == 0)? nearest_goal : find_nearest(target);
            //assert(nearest!=NULL); lu_test
            extend(nearest,target);
            nd = distance(node[num_nodes-1],goal);
            //·??????
            if (nd < d)
            {
                //??·?
                nearest_goal = &node[num_nodes-1];
                d = nd;
            }
            i++;
        }
        inobs = !obs->check(initial);

        // trace back up plan to find simple path
        p = nearest_goal;
        if (!inobs)
        {
            //??Χ
            //??????????????
            //??obs_id?
            while (p!=NULL && !obs->check(initial,*p,obs_id))
            {
                p = p->parent;
            }
        }
        else
        {
            //?Χ
            //?????λ?
            f = obs->repulse(initial);
            if (false)
            {
                printf("BACKTRACE:\n");
                while (p!=NULL)
                {
                    printf("  <%f,%f> %f\n",V2COMP((p->pos - initial.pos)),
                           f.dot(p->pos - initial.pos));
                    p = p->parent;
                }
            }
            p = nearest_goal;
            while (p!=NULL && p->parent!=NULL &&
                   f.dot(p->pos - initial.pos)<out_of_obs_dot)
            {
                //???????
                p = p->parent;
            }
        }
        head = p;
        if (head)
        {
            target = *head;
        }
        else
        {
            target = initial;
        }
        //  if(plan_print)
        //{
        //    if(nearest_goal)
        //	{
        //      printf("  nearest_goal(%f,%f)\n",V2COMP(nearest_goal->pos));
        //    }
        //    printf("  goal(%f,%f)\n",goal.pos.x,goal.pos.y);
        //    printf("  target(%f,%f)\n",target.pos.x,target.pos.y);
        //  }
        /*
     field.setColor(192,192,192);
     p = nearest_goal;
     while(p!=NULL && p->parent!=NULL){
       DrawLine(field,p->parent->pos.x,p->parent->pos.y,p->pos.x,p->pos.y);
       p = p->parent;
     }
     */
        // put in waypoint cache if solution
        //???ι?·?waypoint??ι??
        if (num_waypoints > 0)
        {
#if 1//lu_test change 0 to 1
            if (bDebugPathPlan)
            {
                p = nearest_goal;
                while (p!=NULL && p->parent!=NULL)
                {
                    //???????
                    gui_debug_line(0,0,p->pos,p->parent->pos);
                    p = p->parent;
                }
            }
#endif
            if (p!=NULL && ((d < VERYNEAR) || drand48()<0.1))
            {
                //??·,10%
                p = nearest_goal;
                while (p != NULL)
                {
                    i = lrand48()%num_waypoints;
                    waypoint[i] = *p;
                    waypoint[i].parent = NULL;
                    if (p == head)
                    {
                        break;
                    }
                    p = p->parent;
                }
            }
            else
            {
                //??(90%)??
                i = lrand48()%num_waypoints;
                waypoint[i] = random_state();
            }
        }
        return(target);
    }
}
