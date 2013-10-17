#ifndef MOVE_AS_STRAIGHT_TACTIC_H
#define MOVE_AS_STRAIGHT_TACTIC_H


#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"



class TMoveAsStraight : public RobotTactic
{
private:

    enum { AT_START_POINT = 0 , AT_END_POINT =1};
        static const MyVector2d START_POINT;
        static const MyVector2d END_POINT;
        MyVector2d current_target_;
        bool start_or_end_;
        bool ToStart();
        bool ToEnd();
        bool start_or_end();
        void set_start_or_end();

public:
        TMoveAsStraight();
        virtual const char *name() const
        {
                return "Move As Sin";
        }

        static Tactic *parser(const char *param_string)
        {
                return new TMoveAsStraight();
        }

        void LoadConfig();
        virtual Tactic *clone() const
        {
                return new TMoveAsStraight(*this);
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

        //ѡ񱾶Ļ
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

        virtual void command(World &world, int me, Robot::RobotCommand &command,
                             bool debug);
};



#endif // MOVE_AS_STRAIGHT_TACTIC_H
