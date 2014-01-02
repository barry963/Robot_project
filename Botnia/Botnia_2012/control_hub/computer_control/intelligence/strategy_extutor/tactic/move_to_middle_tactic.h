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

/**    @file    - move_to_middle_tactic.h
  *
  *    @class   - TMoveToMiddle
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 12/20/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#ifndef MOVE_TO_MIDDLE_TACTIC_H
#define MOVE_TO_MIDDLE_TACTIC_H
#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"

class TMoveToMiddle : public RobotTactic
{
private:
        MyVector2d prev_target;
        bool prev_target_set;

public:
        TMoveToMiddle();
        virtual const char *name() const
        {
                return "KickLoc";
        }

        static Tactic *parser(const char *param_string)
        {
                return new TMoveToMiddle();
        }

        void LoadConfig();
        virtual Tactic *clone() const
        {
                return new TMoveToMiddle(*this);
        }
        virtual Status isDone(World &world, int me)
        {
                //COMM_START		   's'
                if (strchr("s ", world.game_state) != NULL)
                {
                        return Succeeded;
                }
                else
                {
                        return InProgress;
                }
        }

        virtual int selectRobot(World &world, bool candidates[], double bias[])
        {
                int best_i = -1;
                double best = 0;
                for (uint i=0; i<MAX_TEAM_ROBOTS; i++)
                {
                        if (!candidates[i])
                                continue;
                        if (bias[i] < 0.0)
                                return i;
                        double d = (world.GetRobotPositionByID(i) - world.ball_position()).sqlength();
                        if ((best_i < 0) || (d < best))
                        {
                                best = d;
                                best_i = i;
                        }
                }
                return best_i;
        }


};
#endif // MOVE_TO_MIDDLE_TACTIC_H
