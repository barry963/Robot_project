#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"

#ifndef MOVE_AS_SIN_TACTIC_H
#define MOVE_AS_SIN_TACTIC_H



class TMoveAsSin : public RobotTactic
{
private:
    enum { AT_START_POINT = 0 , AT_END_POINT =1};
    static const MyVector2d START_POINT;
    static const MyVector2d END_POINT;
    static const double START_TO_END_DISTANCE;
    bool is_at_end_;
    unsigned int step_number_;
    MyVector2d x_step_vector_;
    MyVector2d current_target_;
    MyVector2d start_to_end_distance_;
    bool amplitude_;
    bool frequency_;
    inline bool ToStart();
    inline bool ToEnd();
    bool is_at_end();
    void set_is_at_end();
    MyVector2d* point_set_;
    // get step_size from start and end point
    void SetupSettings();
    void SpecifyMoveSettings();

public:
    TMoveAsSin();
    ~TMoveAsSin();
    virtual const char *name() const
    {
        return "Move As Sin";
    }

    static Tactic *parser(const char *param_string)
    {
        return new TMoveAsSin();
    }

    void LoadConfig();
    virtual Tactic *clone() const
    {
        return new TMoveAsSin(*this);
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



#endif // MOVE_AS_SIN_TACTIC_H
