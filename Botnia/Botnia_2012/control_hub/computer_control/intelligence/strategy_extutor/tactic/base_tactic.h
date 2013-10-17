#ifndef STRATEGY_TACTIC_H
#define STRATEGY_TACTIC_H



#include <deque>
#include <vector>

#include "user_interface/field_related/field_global_function.h"
#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/knowledge_base/database/world_state/field_world_robot.h"

#include <QList>



// it seems it is one skill class that drives the robot to some point
class BCoordinate
{
public:
    enum otype { OAbsolute, OBall };
    enum stype { SAbsolute, SBall, SStrong, SBallOrStrong, SGui };

private:
    otype origin;
    stype side;
    bool dynamic;
    bool absolute;

    MyVector2d c;

    MyVector2d asVectorNotAbsolute(World &w);

public:
    BCoordinate(double x, double y,
                stype _side = SAbsolute,
                otype _origin = OAbsolute,
                bool _dynamic = false)
    {
        c = MyVector2d(x, y);
        side = _side;
        origin = _origin;
        dynamic = _dynamic;
        absolute = (origin == OAbsolute && side == SAbsolute);
    }

    BCoordinate(MyVector2d _p = MyVector2d(0, 0),
                stype _side = SAbsolute,
                otype _origin = OAbsolute,
                bool _dynamic = false)
    {
        c = _p;
        side = _side;
        origin = _origin;
        dynamic = _dynamic;
        absolute = (origin == OAbsolute && side == SAbsolute);
    }

    MyVector2d asVector(World &w)
    {
        if (absolute)
        {
            return c;
        }
        else
        {
            return asVectorNotAbsolute(w);
        }
    }

    double asDirection(World &w)
    {
        return asVector(w).angle();
    }

    MyVector2d getVelocity(World &w)
    {
        if (dynamic && origin == OBall)
        {
            return w.ball_velocity();
        }
        else
        {
            return MyVector2d(0, 0);
        }
    }
};

class BRegion
{
private:
    enum { Circle,  Rectangle } type;

    BCoordinate p[2];

    double radius;

public:
    BRegion()
    {
        type = Circle;
        radius = 0;
    }
    BRegion(BCoordinate p1, BCoordinate p2, double _radius)
    {
        type = Rectangle;
        p[0] = p1;
        p[1] = p2;
        radius = _radius;
    }
    BRegion(BCoordinate p1, double _radius)
    {
        type = Circle;
        p[0] = p1;
        radius = _radius;
    }

    MyVector2d center(World &w);
    MyVector2d sample(World &w);

    MyVector2d centerVelocity(World &w);

    void diagonal(World &w, MyVector2d p, MyVector2d &d1, MyVector2d &d2);

    bool inRegion(World &w, MyVector2d p);

    void gui_draw(int robot, int level, World &world);
};

typedef vector<int> TRoleMap;

class Tactic
{
public:
    virtual const char *name() const
    {
        return "Unknown Tactic";
    }
    virtual ~Tactic() {};
private:
    // These things may be set to give a broader context for the tactic.
    // See, e.g., getTeammateId() and getObsFlags().
    TRoleMap *teammate_map, *opponent_map;
    int priority;

public:
    // Constructor
    Tactic(bool _active = false, bool _manipulates_ball = false)
    {
        active = _active;
        manipulates_ball = _manipulates_ball;
        teammate_map = opponent_map = NULL;
        priority = 0;
    }

    // Register
    //
    // This allows a tactic to be associated with a string and parsing
    // routine so that ascii commands can be converted into tactic classes
    // to be executed.  Useful for gui specification of tactics.
    //
    typedef Tactic * (*parse_fn)(const char *param_string);
    static bool registerParser(const char *name, parse_fn parser);
    static Tactic *parse(const char *string);

    // Clone
    //
    // Each subclass must define a clone method.  For most cases the following
    // just needs to be added in the class definition:
    //
    // virtual Tactic *clone() const { return new NewTactic(*this); }
    //
    virtual Tactic *clone() const = 0;

    // Role Mapping
    //
    // This provides a mechanism for referring to robots indirectly.  Id's
    // are passed through the tactic's role map to get a robot id.
    //
    // You can also grab an obs_flags int with all teammates higher in
    // the rolemap than the tactic's priority.  This can then be used to
    // yield to higher priority tactics when, e.g., finding good
    // positions on the field.
    //
    void setTeammateMap(TRoleMap *m)
    {
        teammate_map = m;
    }
    void setOpponentMap(TRoleMap *m)
    {
        opponent_map = m;
    }

    int getTeammateId(unsigned id)
    {
        if (id<0)return -1;
        if (!teammate_map)
        {
            return -1;
        }
        if (((unsigned int)id)>=(*teammate_map).size())
        {
            return -1;
        }
        return (*teammate_map)[id];
    }

    int getOpponentId(int id)
    {
        if (id<0)return -1;
        if (!opponent_map)
        {
            return -1;
        }
        if ( ((unsigned int)id) >=(*opponent_map).size())
        {
            return -1;
        }
        return (*opponent_map)[id];
    }

    int getObsFlags()
    {
        if (!teammate_map) return 0;
        int obs_flags = 0;
        for (int i=0; i<priority ; i++)
        {
            obs_flags |= OBS_TEAMMATE(getTeammateId(i));
        }
        return obs_flags;
    }

    void setPriority(int _priority)
    {
        priority = _priority;
    }

    //
    // Tactic specified methods and fields.
    //
    //是否激活
    bool active;  // Is true if this is an active tactic.
    //是否操作球
    bool manipulates_ball; // Is true if tactic manipulates the ball.

    // Returns the robot most apt to accomplish the tactic.
    // By default this is the robot with the lowest estimated time modulo
    //  the provided time bias.
    virtual int selectRobot(World &world, bool candidates[], double bias[]);

    //根据策略需求选择机器人
    int selectRobot(World &world, bool candidates[])
    {
        static double bias[MAX_TEAM_ROBOTS] = {0};
        return selectRobot(world, candidates, bias);
    }

    // Returns the time to accomplish the tactic.
    //计算完成策略消耗的时间
    virtual double estimatedTime(World &world, int me)
    {
        return 0.0;
    }

    // Returns the status of the tactic.
    // 策略是否执行完成
    virtual Status isDone(World &world, int me)
    {
        return Succeeded;
    }

    // Performs the tactic and returns the motor traj.
    // 执行策略
    virtual void run(World &world, int me) = 0;

    //private:
    // a stract used to store infomations of tactics
    struct registration
    {
        registration()
        {
            name=NULL;
            parser=NULL;
        };
        registration(const char * _name,parse_fn _parser)
        {
            name=_name;
            parser=_parser;
        };
        const char *name;
        parse_fn parser;
    };
    static deque<registration> strategy_register_list;
};

class RobotTactic : public Tactic
{
protected:
    Status the_status;

    void makeCommand(World &world, int me, bool debug,
                     Robot::RobotCommand &command,
                     bool &ignore_status);

public:
    RobotTactic(bool _active = false, bool _manipulates_ball = false) :
        Tactic(_active, _manipulates_ball)
    {
        the_status = InProgress;
    }

    virtual void command(World &world, int me, Robot::RobotCommand &command,
                         bool debug)
    {
        command.cmd = Robot::CmdPosition;

        command.target = world.GetRobotPositionByID(me);

        command.velocity = MyVector2d(0, 0);
        command.angle = world.teammate_direction(me);
        command.observation_type = 0;
    }

    // This is slightly inefficient.  Both estimated_time() and doit()
    // generate the command afresh.  But since the command may be robot
    // dependent it's not something one can easily cache.
    //
    // Also debugging output is not printed on call from estimatedTime()
    // but are on the call from run().

public:
    virtual double estimatedTime(World &world, int me)
    {
        Robot::RobotCommand the_command;
        bool ignore_status;
        //为机器人生成下一条命令
        makeCommand(world, me, false, the_command, ignore_status);
        //评估这条命令执行消耗的时间，为选择机器人角色提供依据
        return world.robot[me]->time(world, the_command);
    }

    virtual Status isDone(World &world, int me)
    {
        return the_status;
    }

    // run
    virtual void run(World &world, int me)
    {
        Robot::RobotCommand the_command;
        the_command.cmd=Robot::CmdError;
        bool ignore_status;
        bool bNAN=false;
        // form the command
        // need the
        // world : the world module
        // me : the index of robot
        // it seems it is the initialization of the command only according to type
        // the real figures are not assigned
        makeCommand(world, me, bDebugTactic, the_command, ignore_status);

        //validate x,y and angle
        if (!finite(the_command.target.x) || !finite(the_command.target.y))
        {
            bNAN=true;
            printf("tactic x=:NANs!!! %s %d,%3.2f,%3.2f\n",name(),the_command.cmd,the_command.target.x,the_command.target.y);
        }
        if (!finite(the_command.angle))
        {
            qDebug()<< "command angle :"<<the_command.angle;
            bNAN=true;
            printf("tactic a=:NANs!!! %s %d,%3.2f\n",name(),the_command.cmd,the_command.angle);
        }

        if (!bNAN && the_command.cmd>=0)
        {
            //执行命令
            the_status = world.robot[me]->run(world, the_command);


        }
        if (ignore_status)
        {
            the_status = InProgress;
        }
    }
};



#endif // STRATEGY_TACTIC_H
