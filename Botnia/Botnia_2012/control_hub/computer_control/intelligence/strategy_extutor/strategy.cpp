//��������״̬���������ͽ�ɫ���о������䣬ͬʱΪ������
//׼����һ����ǰ��������

// strategy.cc
//
// Encapsulates the strategy engine
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu),
//              Brett Browning (brettb@cs.cmu.edu)
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

#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_configreader.h"
#include <QString>


#include "control_hub/computer_control/knowledge_base/database/world_state/field_world.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/base_tactic.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/skill/simple_tactics.h"

#include "strategy.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"
#include "net/net_refbox_command.h"

#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/goalie.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/play/play.h"

static bool cr_setup = false;
CR_DECLARE(PLAYEXEC_OPPORTUNISM);
CR_DECLARE(PLAYEXEC_ROLE_SWITCHING);
CR_DECLARE(PLAYEXEC_FIXED_ROLES);
CR_DECLARE(PLAYEXEC_DEBUG_ROLE_SWITCHING);
CR_DECLARE(RESPONSIBLE_MIN_RUNTIME);
CR_DECLARE(RESPONSIBLE_MAX_RUNTIME);
CR_DECLARE(CREDIT_MIN_RUNTIME);
CR_DECLARE(PLAYBOOK_ADAPT_WEIGHTS);
CR_DECLARE(PLAYBOOK);


//角色执行者状态初始化，清空策略队列，检查赛场状态;
PlayExecutor::PlayExecutor()
{
    id_goalie = -1;
    tactic_goalie = new TGoalie();
    tactic_opportunity_off = new TShoot(TShoot::Aim);
    tactic_opportunity_def = new TClear();
    status = Aborted;
    teammate_map = TRoleMap(MAX_PLAY_ROLES);
    opponent_map = TRoleMap(MAX_PLAY_ROLES);
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        teammate_map[i] = opponent_map[i] = i;
    }
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        tactics[i] = NULL;
    }
    runtime = 0;
}

void PlayExecutor::LoadConfig()
{
    tactic_goalie->LoadConfig();
    tactic_opportunity_off->LoadConfig();
    //tactic_opportunity_def->LoadConfig();
}

//重新设定play
void PlayExecutor::set(World &world, Play *_play)
{
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        //如果切换到新的play，删除忙策略
        if (tactics[i] && busy_tactics[i] != tactics[i])
        {
            delete tactics[i];
        }
        tactics[i] = NULL;
    }
    play = _play;
    sequence_index = -1;
    sequence_length = play->length();
    start_timestamp = -1;
    status = InProgress;
    runtime = 0;
    completed = false;
}

//将除了守门员之外设定为角色候选人
void PlayExecutor::makeCandidates(World &world,
                                  bool candidates[MAX_TEAM_ROBOTS])
{
    for (int i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        candidates[i] = (i < world.n_teammates && i != id_goalie);
    }
}

//更新比赛场景信息，确认角色分配情况
void PlayExecutor::updateMaps(World &world)
{
    if (id_goalie >= 0 && id_goalie < world.n_teammates)
    {
        //设定己方守门员
        world.trole_goalie = id_goalie;
    }
        //设定己方守门员
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        if (tactics[i] && tactics[i]->active)
        {
            world.trole_active = assign[i];
            break;
        }
    }
    //�����������ͼ
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        teammate_map[i] = assign[i];
    }
    //���ݽű�Ҫ�󣬽���Է�����ͼ
    play->updateOpponentMap(world, &opponent_map);
}

//�̶���ɫָ��
void PlayExecutor::fixedAssignment(World &world)
{
    assign[play->getFixedRoleID(0)] = (world.sideBall() > 0 ? 0 : 1);
    assign[play->getFixedRoleID(1)] = (world.sideBall() > 0 ? 1 : 0);
    assign[play->getFixedRoleID(2)] = 2;
    assign[play->getFixedRoleID(3)] = 3;
}

//��ʼ��������������;
void PlayExecutor::initialAssignment(World &world)
{
    bool candidates[MAX_TEAM_ROBOTS];
    //
    makeCandidates(world, candidates);
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        if (!tactics[i]) continue;
        int s = tactics[i]->selectRobot(world, candidates);
        if (s < 0)
        {
            assign[i] = -1;
        }
        else
        {
            candidates[s] = false;
            assign[i] = s;
        }
    }
}

void PlayExecutor::checkAssignment(World &world)
{
    bool switched = false;
    bool candidates[MAX_TEAM_ROBOTS];
    makeCandidates(world, candidates);
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        if (!tactics[i])
        {
            assign[i] = -1;
            continue;
        }
        if (assign[i]<0)continue;
        double bias[MAX_TEAM_ROBOTS] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
        bias[assign[i]] = -0.3;
        int s = tactics[i]->selectRobot(world, candidates, bias);
        if (s < 0)
        {
            assign[i] = -1;
        }
        else
        {
            if (IVAR(PLAYEXEC_DEBUG_ROLE_SWITCHING) && s != assign[i] || switched)
            {
                if (!switched)
                {
                    gui_debug_printf(-1, GDBG_STRATEGY, "-----\n");
                }
                gui_debug_printf(-1, GDBG_STRATEGY,"ROLE %d: Previous = %d (%f), New = %d (%f).\n",
                                 i, assign[i],
                                 (assign[i] > 0 ? tactics[i]->estimatedTime(world, assign[i]) : -1),
                                 s, s > 0 ? tactics[i]->estimatedTime(world, s) : -1);
                switched = true;
            }
            candidates[s] = false;
            assign[i] = s;
        }
    }
    if (switched && IVAR(PLAYEXEC_DEBUG_ROLE_SWITCHING))
    {
        gui_debug_printf(-1, GDBG_STRATEGY, "-----\n");
    }
}

void PlayExecutor::checkForOpportunity(World &world)
{
    if (world.situation == World::Offense)
    {
        //����
        tactic_opportunity = tactic_opportunity_off;
    }
    else if (world.situation == World::Defense)
    {
        //����
        tactic_opportunity = tactic_opportunity_def;
    }
    else
    {
        tactic_opportunity = NULL;
    }
    if (!tactic_opportunity)
    {
        return;
    }
    //����������
    if (world.ballOutOfPlay())
    {
        return;
    }
    bool candidates[MAX_TEAM_ROBOTS];
    makeCandidates(world, candidates);
    double best_t = 0;
    int best_i = -1;
    //�Ի����˽���ʱ�����
    //�����ǽ�����������������ʱ�����
    //�����Ƿ��أ�����TClear����ʱ�����
    for (int i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        if (!candidates[i])
        {
            continue;
        }
        double t = tactic_opportunity->estimatedTime(world, i);
        if (i == id_opportunity)
        {
            t -= 0.2;
        }
        if (best_i < 0 || t < best_t)
        {
            best_i = i;
            best_t = t;
        }
    }
    if (best_t < 0.5)
    {
        if (id_opportunity != best_i)
        {
            gui_debug_printf(-1, GDBG_STRATEGY,
                             "OPPORTUNITY: %d in %fs\n", best_i, best_t);
        }
        id_opportunity = best_i;
    }
    else
    {
        id_opportunity = -1;
    }
}

void PlayExecutor::checkBusy(World &world)
{
    // Check for no longer busy tactics.
    for (int i=0; i<MAX_TEAM_ROBOTS; i++)
    {
        //�����Ƿ���ս������æ�������Ѿ�æ�꣬ɾ��æս��
        //æս���������ڽ��з����Ļ����ˣ��������ڽ���������ز����������ɫת������
        if (busy_tactics[i] && busy_tactics[i]->isDone(world, i) != Busy)
        {
            if (tactics[i] != busy_tactics[i])
            {
                delete busy_tactics[i];
            }
            busy_tactics[i] = NULL;
        }
    }
    // Check for busy tactics.
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        if (!busy_tactics[i] && assign[i] >= 0 && tactics[i] && tactics[i]->isDone(world, i) == Busy)
        {
            //�����趨æս��
            busy_tactics[i] = tactics[i];
        }
    }
}

Status PlayExecutor::nextInSequence(World &world)
{
    if (sequence_index + 1 >= sequence_length)
    {
        return Completed;
    }
    sequence_index++;
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        PlayRole &r = play->getRole(i);
        Tactic *t = r[sequence_index];
        if (t)
        {
            //�����л�����һ����ս��ر�æ��־
            if (tactics[i] && busy_tactics[i] != tactics[i])
            {
                delete tactics[i];
            }
            tactics[i] = t->clone();
            tactics[i]->setTeammateMap(&teammate_map);
            tactics[i]->setOpponentMap(&opponent_map);
            tactics[i]->setPriority(i);
        }
    }
    return InProgress;
}

//�����������ƽ�����ԣ���ÿ�������˷���ָ�����񣬲�������Ӧ״̬��־λ
void PlayExecutor::setTactics(World &world, Tactic *t[],
                              bool set_everyone,
                              bool set_anyone,
                              bool set_goalie)
{
    //printf("setTactic----------------------------------------------\r\n");
    //assign �����趨�����˵Ľ�ɫ���ţ����ڶ�̬��ɫ
    if (set_everyone || set_anyone)
    {
        for (int i=0; i<MAX_PLAY_ROLES; i++)
        {
            if (assign[i] < 0)
            {
                if (!set_everyone)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
#if 1
            //��æ����صĻ����˽�ֹ��ɫ�л�
            if (busy_tactics[assign[i]])
            {
                //fprintf(stderr, "BUSY: %d, %s\n", assign[i], busy_tactics[assign[i]]->name());
                t[assign[i]] = busy_tactics[assign[i]];
            }
            else
            {
                //���ɽ�ɫ�л�
                //fprintf(stderr, "Tactic: %d, %s\n", assign[i], tactics[i]->name());
                t[assign[i]] = tactics[i];
            }
#else
            t[assign[i]] = tactics[i];
#endif
            //�����Ѿ��趨����Ա���Ͳ�Ҫ���趨����Ա��
            if (assign[i] == id_goalie)
            {
                set_goalie = false;
            }
            if (!set_everyone)
            {
                break;
            }
        }
    }
    //�趨����Ա�������Ų���
    if (set_goalie && id_goalie >= 0)
    {
        t[id_goalie] = tactic_goalie;
    }
    //���ڲⶨ�߱����û������Ż��߷��صĻ����ˣ���ʱ�������߱�������ɫ
    //PLAYEXEC_OPPORTUNISM =    1 # boolean
    if (IVAR(PLAYEXEC_OPPORTUNISM) && tactic_opportunity && id_opportunity >= 0)
    {
        //fprintf(stderr, "Opp Tactic: %d, %s\n", id_opportunity, tactic_opportunity->name());
        t[id_opportunity] = tactic_opportunity;
    }
}

void PlayExecutor::checkStatus(World &world)
{
    //Ĭ��ÿ��play����ά��25��
    //��ʱ����
    if (world.game_state == COMM_START &&
            play->timeout() >= 0.0  &&
            world.time - start_timestamp > play->timeout())
    {
        //����play��ʱ
        status = completed ? Completed : Aborted;
        return;
    }
    //
    for (int i=0; i<MAX_PLAY_ROLES; i++)
    {
        //���ݻtactic�Ƿ�������ж�play�Ƿ�����
        if (tactics[i] && tactics[i]->active)
        {
            Status s = tactics[i]->isDone(world, assign[i]);
            if (s != InProgress)
            {
                gui_debug_printf(-1, GDBG_STRATEGY, "ACTIVE TACTIC DONE: %s\n",
                                 status_as_string(s));
            }
            if (s == Failed || s == Aborted)
            {
                status = Aborted;
                return;
            }
            else if (s == Succeeded || s == Completed)
            {
                //����һ��tactic���ɣ���ʼ��һ��tactic
                status = nextInSequence(world);
                if (status == InProgress)
                {
                    gui_debug_printf(-1, GDBG_STRATEGY, "NEXT IN SEQUENCE.\n");
                }
                else if (status == Completed)
                {
                    if (!completed)
                    {
                        gui_debug_printf(-1, GDBG_STRATEGY, "SEQUENCE COMPLETED.\n");
                    }
                    completed = true;
                    status = InProgress;
                }
                return;
            }
        }
    }
}

//���в���
void PlayExecutor::run(World &world, Tactic *t[])
{
    if (play)
    {
        // Need a World to set the start timestamp, so we set it if not set.
        //����û���趨��ʼʱ�䣬�趨һ��
        if (start_timestamp < 0.0)
        {
            start_timestamp = world.time;
        }
        //����play�����˶೤ʱ��
        else if ((strchr("s ", world.game_state) != NULL) && last_run_timestamp != world.time)
        {
            runtime += (world.time - last_run_timestamp);
        }
        // Robot assignment
        //�����˽�ɫ����
        if (sequence_index < 0)
        {//��play��һ�ν���
            //���tactics�б�
            nextInSequence(world);
            //����ҷ���ɫ��������
            //PLAYEXEC_FIXED_ROLES =    0 # boolean
            if (IVAR(PLAYEXEC_FIXED_ROLES))
            {
                fixedAssignment(world);//�̶���ɫָ��
            }
            else
            {
                initialAssignment(world);	//��̬��ɫָ��
            }
            if (world.n_teammates == 1)
            {
                id_goalie = -1;	//��������ֻ��һ�������ˣ���������Ա
            }
            else
            {
                //���򽫱��������Ļ������趨Ϊ����Ա
                id_goalie = world.n_teammates - 1;	//Ĭ��ȡ����������Ϊ����Ա
            }
        }
        //PLAYEXEC_ROLE_SWITCHING = 1 # boolean
        else if (IVAR(PLAYEXEC_ROLE_SWITCHING))
        {
            //������ɫ�л�
            checkAssignment(world);
        }
        updateMaps(world);
        // Opportunism
        //PLAYEXEC_OPPORTUNISM =    1 # boolean
        if (IVAR(PLAYEXEC_OPPORTUNISM))
        {
            checkForOpportunity(world);
        }
    }
    // Set tactics
    //
    // If we're playing or on a kickoff, everyone can move.
    // If it's a restart for us then only the kicker can move.
    // Goalie always runs.
    //
    //COMM_START=s COMM_KICKOFF_YELLOW=k  COMM_PENALTY_YELLOW=p
    //�����ǿ�ʼ�����߿��򣬻��߷����������趨ÿһ�������˵Ĺ��ս��
    bool set_everyone = (strchr("skKpP ", world.game_state) != NULL);
    //�������ҷ�������ֻ�趨һ�������˵�ս��
    bool set_anyone = (world.restart() &&
                       world.restartWhoseKick() == World::OurBall);
    //������ĳһ�����򣬲����������趨����Ա����
    bool set_goalie = !world.restartNeutral();
    //
    if (!play)
    {
        set_everyone = false;
        set_anyone = false;
    }
    // Check status
    if (play)
    {
        //���鱾play�Ƿ��Ѿ�����
        checkStatus(world);
    }
    // Check busy
    checkBusy(world);
    setTactics(world, t, set_everyone, set_anyone, set_goalie);
    last_run_timestamp = world.time;
}

extern QString application_path;
bool PlayBook::load(const char *filename)
{
    bool rv = true;
    QString filepath=application_path+"/config/plays/";
    filepath+=filename;
        FILE *f = fopen(filepath.toStdString().c_str(), "r");
    if (!f) return false;
    while (!feof(f))
    {
        static char line[256];
        double weight;
        bool error;
        Play *p;
        int n = 0;
        char *name;
        if (!fgets(line, 256, f)) continue;
        if (line[0] == '#' || (uint) Parse::skipWS(line) >= strlen(line)) continue;
        n += Parse::pDouble(line + n, weight);
        n += Parse::pWord(line + n, &name);
        filepath=application_path+"/config/plays/";
        filepath+=name;
        filepath+=".ply";
                p = new PlayAscii(filepath.toStdString().c_str(), error);
        if (!error && p && weight > 0.0)
        {
            add(p, name, weight);
        }
        else
        {
            if (error || !p)
                fprintf(stderr, "ERROR: PlayBook invalid play: %s", line);
            else
                fprintf(stderr, "ERROR: PlayBook invalid weight: %s", line);
            rv = false;
        }
    }
    return rv;
}


bool PlayBook::save(const char *filename)
{
    QString filepath=application_path+"/config/plays/";
    filepath+=filename;
        FILE *f = fopen(filepath.toStdString().c_str(), "w");
    if (!f) return false;
    for (uint i=0; i<plays.size(); i++)
    {
        fprintf(f, "%f\t%s\n", weight(i), play_names[i]);
    }
    fclose(f);
    return true;
}

//ÿ��play����Ȩ�أ�������������play������ѡ��ѡ��
Play *PlayBook::select(World &w)
{
    bool* applicable=new bool[plays.size()];
    double sum = 0.0;
    //��������������playȨ��֮��
    for (uint i=0; i<plays.size(); i++)
    {
        applicable[i] = plays[i]->isApplicable(w);
        if (applicable[i])
        {
            sum += weight(i);
        }
    }
    if (sum == 0)
    {
        delete applicable;
        return NULL;
    }
    //��ӡÿ����������play������
    gui_debug_printf(-1, GDBG_STRATEGY, "SELECT PLAY\n");
    for (uint i=0; i<plays.size(); i++)
    {
        if (applicable[i])
        {
                    // Bin: TODO: problematic code,needs converting!
                        vision_info.sPlay = QString::fromUtf8(plays[i]->name);
                    //vision_info.sPlay = QString("test");

            //gui_debug_printf(-1, GDBG_STRATEGY,
            //     "  %5g; %s; ", weight(i), plays[i]->name);
            results[i].gui_print();
        }
    }
    //����������
    double r = sum * (rand() / ((double) RAND_MAX + 1));
    //����������ѡ��
    for (uint i=0; i<plays.size(); i++)
    {
        if (!applicable[i]) continue;
        r -= weight(i);
        if (r < 0)
        {
            gui_debug_printf(-1, GDBG_STRATEGY, "  Selected: %s\n", plays[i]->name);
            delete applicable;
            return plays[i];
        }
    }
    delete applicable;
    return NULL;
}

//
double PlayBook::weight(uint index)
{
    double weight = weights[index];
    Results &r = results[index];
    if (IVAR(PLAYBOOK_ADAPT_WEIGHTS))
    {
        weight *= pow(4.0, r.counts[Succeeded] - r.counts[Failed]);
        weight *= pow(4.0 / 3.0, r.counts[Completed] - r.counts[Aborted]);
    }
    return weight;
}

Strategy::Strategy()
{
}

void Strategy::LoadConfig()
{
    executor.LoadConfig();
    CR_SETUP(strategy, PLAYBOOK, CR_STRING);
    CR_SETUP(strategy, PLAYEXEC_OPPORTUNISM, CR_INT);
    CR_SETUP(strategy, PLAYEXEC_ROLE_SWITCHING, CR_INT);
    CR_SETUP(strategy, PLAYEXEC_FIXED_ROLES, CR_INT);
    CR_SETUP(strategy, PLAYEXEC_DEBUG_ROLE_SWITCHING, CR_INT);
    CR_SETUP(strategy, RESPONSIBLE_MIN_RUNTIME, CR_DOUBLE);
    CR_SETUP(strategy, RESPONSIBLE_MAX_RUNTIME, CR_DOUBLE);
    CR_SETUP(strategy, PLAYBOOK_ADAPT_WEIGHTS, CR_INT);
    CR_SETUP(strategy, CREDIT_MIN_RUNTIME, CR_DOUBLE);
    doing = NONE;
}

//��ʼ�����ɺ����Զ�װ��playbook
void Strategy::init(const char *playbook_file)
{
    LoadConfig();
    current_play = last_play = NULL;
    if (playbook_file)
    {
        fprintf(stderr, "Loading playbook: %s.\n", playbook_file);
        playbook.load(playbook_file);
    }
    else
    {
        fprintf(stderr, "Loading playbook: %s.\n", SVAR(PLAYBOOK));
        playbook.load(SVAR(PLAYBOOK));
    }
}

bool Strategy::parse(char *string)
{
    char *str;
    char buf[1];
    buf[0] = string[7];
#ifdef DEBUG
    fprintf(stderr, "Strategy:parse with %s\n", string);
#endif
    int n = 0;
    //������һ��ʳ���
    for (str = string; (*str != 0) && (*str != ' '); str++, n++)
        ;
    if (strncmp(string, "warmup", n) == 0)
    {
        doing = WARMUP;
        drills.warmupNum = atoi(buf);
        drills.initialized = false;
        return (true);
    }
    else if (strncmp(string, "teamplay", n) == 0)
    {
        doing = PLAYS;
        return (true);
    }
    else if (strncmp(string, "none", n) == 0)
    {
        doing = NONE;
        return (false);
    }
    else
    {
        doing = NONE;
        return (false);
    }
}

void Strategy::stop(World &world, Tactic *tactics[])
{
    for (int i=0; i<world.n_teammates; i++)
    {
        tactics[i] = &t_stop;
    }
}

//�л�play
void Strategy::playEnded(World &world, Status status)
{
    //�����оɵ�play
    if (current_play)
    {
        //RESPONSIBLE_MIN_RUNTIME = 2.0 # s
        if (executor.runningTime(world) > DVAR(RESPONSIBLE_MIN_RUNTIME))
        {
            last_play = current_play;
            last_play_endtime = world.time;
            last_play_status = executor.isDone(world);
        }
        //CREDIT_MIN_RUNTIME = 1.0 # s
        if (executor.runningTime(world) > DVAR(CREDIT_MIN_RUNTIME))
        {
            credit(world, current_play, status);
        }
        else
        {
            gui_debug_printf(-1, GDBG_STRATEGY,
                             "PLAY NO CREDIT %s, %s.  Running Time = %f.\n",
                             current_play->name, status_as_string(status),
                             executor.runningTime(world));
        }
    }
    //����ѡ���µ�play
    current_play = playbook.select(world);
    if (current_play)
    {
        executor.set(world, current_play);
    }
}

Play *Strategy::responsiblePlay(World &world)
{
    if (current_play && executor.runningTime(world) >
            DVAR(RESPONSIBLE_MIN_RUNTIME))
    {
        return current_play;
    }
    if (stopped_time - last_play_endtime <
            DVAR(RESPONSIBLE_MAX_RUNTIME))
    {
        return last_play;
    }
    return NULL;
}

void Strategy::run(World &world, Tactic *tactics[])
{
    // Warmups
    if (doing==WARMUP)
    {
        qDebug()<<"game run warmup";
        switch (drills.warmupNum)
        {
        case 1:
            drills.sine(world, tactics);
            break;
        case 2:
            drills.spiral(world, tactics);
            break;
        case 3:
            drills.spiralmove(world, tactics);
            break;
        case 4:
            drills.speed(world, tactics);
            break;
        case 5:
            drills.robottest(world,tactics);
            break;
        case 6:
            drills.navchallenge(world,tactics);
            break;
        default:
            drills.basic(world, tactics);
            break;
        }
        return;
    }
    //'S'=Stop
    // Check for stopped game.
    //ÿ���л���Stop״̬�����浱ǰplaybook
    if (world.game_state == 'S')
    {
        qDebug()<<"game state=s";
        if (last_game_state != 'S')
        {
            stopped_time = world.time;
            if (playbook.save("running.plb"))
                fprintf(stderr, "Writing out playbook...\n");
            else
                fprintf(stderr, "Error writing out playbook...\n");
        }
        stop(world, tactics);
        last_game_state = world.game_state;
        return;
    }
    //'s'=Force Start ' '=normal Start
    // If the game state changes to a special state, reselect the play.
    //�����л������µ�����״̬����Ҫ����ѡ��play����
    if ((strchr("s ", world.game_state) == NULL) && world.game_state != last_game_state)
    {
        qDebug()<<"playended1";
        playEnded(world, InProgress);
    }
    // If we don't have a play, select one.
    //����û��play������ѡ��һ���µ�play����
    else if (!current_play)
    {
        qDebug()<<"playended2";
        playEnded(world, InProgress);
    }
    // Run the play executor.
    //����֮ǰ�ȳ�ʼ��Ϊֹͣս��
    stop(world, tactics);
    //����tactics�ķ�������
    executor.run(world, tactics);
}

void Strategy::credit(World &world, Play *play, Status status)
{
    if (play)
    {
        gui_debug_printf(-1, GDBG_STRATEGY, "PLAY CREDIT %s, %s.\n",
                         play->name, status_as_string(status));
        playbook.credit(play, status);
    }
}

//��������״̬�������µ��жϺ;��ߡ�
void Strategy::think(World &world)
{
    // Check for special conditions, such as a goal.
    //�������ŵ÷�����
    if (world.goal_scored)
    {
        Play *play = responsiblePlay(world);
        gui_debug_printf(-1, GDBG_STRATEGY, "GOAL SCORED: %s\n",
                         world.goal_scored < 0 ? "Them" : "Us");
        if (play)
        {
            gui_debug_printf(-1, GDBG_STRATEGY, "  RESPONSIBLE PLAY: %s\n",
                             play ? play->name : "None");
        }
        else
        {
            gui_debug_printf(-1, GDBG_STRATEGY, "  NO RESPONSIBLE PLAY: %f %f.\n",
                             stopped_time, last_play_endtime);
        }
        credit(world, play, world.goal_scored > 0 ? Succeeded : Failed);
        playEnded(world, InProgress);
    }
    // Or a kick for one of the teams.
    if (world.game_state != last_game_state && world.restart())
    {
        Play *play = responsiblePlay(world);
        gui_debug_printf(-1, GDBG_STRATEGY, "RESTART: %s\n",
                         (world.restartWhoseKick() == World::TheirBall ?
                          "Them" : "Us"));
        if (play)
            gui_debug_printf(-1, GDBG_STRATEGY, "  RESPONSIBLE PLAY: %s\n",
                             play ? play->name : "None");
        else
            gui_debug_printf(-1, GDBG_STRATEGY, "  NO RESPONSIBLE PLAY: %f %f.\n",
                             stopped_time, last_play_endtime);
        if (world.restartPenalty())
        {
            credit(world, play, (world.restartWhoseKick() == World::OurBall ?
                                 Succeeded : Failed));
            playEnded(world, InProgress);
        }
        else if (world.restartWhoseKick() == World::OurBall)
        {
            credit(world, play, Completed);
            playEnded(world, InProgress);
        }
        else if (world.restartWhoseKick() == World::TheirBall)
        {
            credit(world, play, Aborted);
            playEnded(world, Aborted);
        }
        else
        {
            playEnded(world, InProgress);
        }
    }
    // Check for end of play.
    if (executor.isDone(world) != InProgress && world.game_state != 'S')
    {
        playEnded(world, executor.isDone(world));
    }
    last_game_state = world.game_state;
}

const double Warmup::goalpts[MAX_TEAM_ROBOTS][2] =
{{1000, 700}, {-1000, 700},
    {-1000, -700}, {1000, -700},
    {0, 0}
};

void Warmup::basic(World &world, Tactic *tactics[])
{
    ready = true;
    if (!initialized)
    {
        for (int i = 0; i < MAX_TEAM_ROBOTS; i++)
        {
            curr_goals[i] = i;
            if (temptac[i])
            {
                delete temptac[i];
            }
            temptac[i] = new SPosition((BCoordinate)goalvec[curr_goals[i]], 0.0);
        }
        initialized = true;
        return;
    }
    for (int i = 0; i < world.n_teammates; i++)
    {
        MyVector2d p = world.GetRobotPositionByID(i) - goalvec[curr_goals[i]];
        if (p.length() > 180.0)
        {
            ready = false;
        }
    }
    if (ready)
    {
        for (int i = 0; i < world.n_teammates; i++)
        {
            curr_goals[i] = (curr_goals[i] + 1) % MAX_TEAM_ROBOTS;
            if (temptac[i])
                delete temptac[i];
            temptac[i] = new SPosition((BCoordinate)goalvec[curr_goals[i]], 0.0);
        }
    }
    for (int i=0; i < world.n_teammates; i++)
    {
        tactics[i]=temptac[i];
    }
}

void Warmup::speed(World &world, Tactic *tactics[])
{
    int speed = 40;
    double height = 1100.0;
    double excess = 200.0;
    MyVector2d target[5];
    // MyVector2d polarval[5];
    int stage = (count % (3*speed)) / speed;
    // int stage2 = ((count - 35) % 150) / 50;
    double val = ((count % (3 * speed)) - speed) / (2.0 * speed);
    if (stage == 0)
    {
        target[0] = MyVector2d(-550.0, -800.0);
        target[1] = MyVector2d(550.0, 800.0);
        target[2] = MyVector2d(-800.0, 550.0);
        target[3] = MyVector2d(800.0, -550.0);
        target[4] = MyVector2d(0.0, 0.0);
    }
    else
    {
        target[0] = MyVector2d((val * 1100.0) -550.0, (height * sin(val * M_PI)) - height + excess);
        target[1] = MyVector2d(-1.0 * ((val * 1100.0) -550.0), -1.0 * ((height * sin(val * M_PI)) - height + excess));
        target[2] = MyVector2d( (height * sin(val * M_PI)) - height + excess , (-1.0 *(val * 1100.0)) + 550.0);
        target[3] = MyVector2d(-1.0 * ((height * sin(val * M_PI)) - height + excess), -1.0 * ((-1.0 * (val * 1100.0)) + 550.0));
    }
    if (!ready)
    {
        ready = true;
        for (int i = 0; i < world.n_teammates; i++)
        {
            MyVector2d p = world.GetRobotPositionByID(i) - target[i];
            if (p.length() > 180.0)
                ready = false;
        }
    }
    if (ready || !initialized)
    {
        for (int i = 0; i < world.n_teammates; i++)
        {
            if (temptac[i])
            {
                delete temptac[i];
            }
            temptac[i] = new SPosition((BCoordinate)target[i],M_PI/2);
        }
        initialized = true;
        count++;
    }
    for (int i=0; i<world.n_teammates; i++)
    {
        tactics[i]=temptac[i];
    }
}

void Warmup::spiralmove(World &world, Tactic *tactics[])
{
    MyVector2d target[5];
    MyVector2d polarval[5];
    double dist, rMin = 400.0, rMax = 400.0, incre = (rMax - rMin) / 99;
    dist = rMin + (incre * (count % 100));
    if (count % 200 > 99)
        dist = rMax - (incre * (count % 100));
    polarval[0] = MyVector2d(dist , M_PI * 0 + (count * 0.04));
    polarval[1] = MyVector2d(dist , M_PI * 0.5 + (count * 0.04));
    polarval[2] = MyVector2d(dist , M_PI * 1.0 + (count * 0.04));
    polarval[3] = MyVector2d(dist , M_PI * 1.5 + (count * 0.04));
    cPosition = MyVector2d(800.0 * sin(M_PI * (count / 150.0)), 500.0 * sin(M_PI * (count / 100.0)));
    if (!ready)
    {
        ready = true;
        for (int i = 0; i < world.n_teammates -1 ; i++)
            target[i] = MyVector2d((polarval[i].x * cos(polarval[i].y)) + cPosition.x, (polarval[i].x * sin(polarval[i].y)) + cPosition.y);
        target[world.n_teammates - 1] = cPosition;
        for (int i = 0; i < world.n_teammates ; i++)
        {
            MyVector2d p = world.GetRobotPositionByID(i) - target[i];
            if (p.length() > 180.0)
                ready = false;
        }
    }
    if (ready || !initialized)
    {
        for (int i = 0; i < world.n_teammates - 1; i++)
            target[i] = MyVector2d((polarval[i].x * cos(polarval[i].y)) + cPosition.x, (polarval[i].x * sin(polarval[i].y)) + cPosition.y);
        target[world.n_teammates - 1] = cPosition;
        for (int i = 0; i < world.n_teammates; i++)
        {
            if (temptac[i])
                delete temptac[i];
            temptac[i] = new SPosition((BCoordinate)target[i],M_PI/2);
        }
        initialized = true;
        count++;
    }
    for (int i=0; i<world.n_teammates; i++)
        tactics[i]=temptac[i];
}

void Warmup::spiral(World &world, Tactic *tactics[])
{
    MyVector2d target[5];
    MyVector2d polarval[5];
    double dist;
    dist = 400.0 + (5 * (count % 100));
    if (count % 200 > 99)
        dist = 895.0 - (5 * (count % 100));
    polarval[0] = MyVector2d(dist , M_PI * 0 + (count * 0.04));
    polarval[1] = MyVector2d(dist , M_PI * 0.4 + (count * 0.04));
    polarval[2] = MyVector2d(dist , M_PI * 0.8 + (count * 0.04));
    polarval[3] = MyVector2d(dist , M_PI * 1.2 + (count * 0.04));
    polarval[4] = MyVector2d(dist , M_PI * 1.6 + (count * 0.04));
    if (!ready)
    {
        ready = true;
        for (int i = 0; i < world.n_teammates; i++)
        {
            target[i] = MyVector2d(polarval[i].x * cos(polarval[i].y), polarval[i].x * sin(polarval[i].y));
            MyVector2d p = world.GetRobotPositionByID(i) - target[i];
            if (p.length() > 180.0)
                ready = false;
        }
    }
    if (ready || !initialized)
    {
        for (int i = 0; i < world.n_teammates; i++)
        {
            target[i] = MyVector2d(polarval[i].x * cos(polarval[i].y), polarval[i].x * sin(polarval[i].y));
            if (temptac[i])
                delete temptac[i];
            temptac[i] = new SPosition((BCoordinate)target[i],M_PI/2);
        }
        initialized = true;
        count++;
    }
    for (int i=0; i<world.n_teammates; i++)
        tactics[i]=temptac[i];
}

void Warmup::sine(World &world, Tactic *tactics[])
{
    MyVector2d target[5];
    target[0] = MyVector2d(1000.0,800.0);
    target[1] = MyVector2d(500.0,800.0);
    target[2] = MyVector2d(0.0,800.0);
    target[3] = MyVector2d(-500.0,800.0);
    target[4] = MyVector2d(-1000.0,800.0);
    if (!initialized)
    {
        for (int i = 0; i < MAX_TEAM_ROBOTS; i++)
        {
            if (temptac[i])
            {
                delete temptac[i];
            }
            temptac[i] = new SPosition((BCoordinate)target[i], M_PI/2);
        }
        initialized = true;
        for (int i=0; i<world.n_teammates; i++)
        {
            tactics[i]=temptac[i];
        }
        return;
    }
    if (ready == false)
    {
        ready = true;
        for (int i = 0; i < world.n_teammates; i++)
        {
            MyVector2d p = world.GetRobotPositionByID(i) - target[i];
            if (p.length() > 180.0)
            {
                ready = false;
            }
        }
    }
    if (ready)
    {
        for (int i = 0; i < world.n_teammates; i++)
        {
            if (count -(20 * i) > 0)
            {
                target[i] = MyVector2d(target[i].x, 800.0 * cos((count-(20*i)) * M_PI / 100));
                if (temptac[i])
                {
                    delete temptac[i];
                }
                temptac[i] = new SPosition((BCoordinate)target[i],M_PI/2);
            }
        }
        count++;
    }
    for (int i=0; i<world.n_teammates; i++)
    {
        tactics[i]=temptac[i];
    }
}

void Warmup::robottest(World &world, Tactic *tactics[])
{
    double t;
    MyVector2d target;
    for (int i=0; i<world.n_teammates; i++)
    {
        // t = 1.0*world.time + 0.60*i;
        t = 1.0*world.time+(2*M_PI/5)*i;
        target = MyVector2d(900*cos(t), 600*sin(2*t));
        if (temptac[i])
        {
            delete temptac[i];
        }
        temptac[i] = new SPosition((BCoordinate) target, M_PI/2);
    }
    for (int i=0; i<world.n_teammates; i++)
    {
        tactics[i]=temptac[i];
    }
}

void Warmup::navchallenge(World &world, Tactic *tactics[])
{
    if (!initialized)
    {
        printf("Running Nav Challenging!!!\n");
        goalvec[0].set(FIELD_LENGTH_H - DEFENSE_DEPTH + ROBOT_DEF_WIDTH_H + 70.0, 0);
        goalvec[1] = -goalvec[0];
        for (int i = 0; i < MAX_TEAM_ROBOTS; i++)
        {
            curr_goals[i] = 0;
            if (temptac[i])
                delete temptac[i];
            int obs = OBS_EVERYTHING_BUT_ME(i) & (~OBS_OUR_DZONE) & (~OBS_THEIR_DZONE);
            temptac[i] = new SPosition((BCoordinate)goalvec[curr_goals[i]], obs);
        }
        initialized = true;
        return;
    }
    for (int i = 0; i < world.n_teammates; i++)
    {
        gui_debug_printf(i, GDBG_TACTICS, "Running Nav challenge goal %i", curr_goals[i]);
        MyVector2d p = world.GetRobotPositionByID(i) - goalvec[curr_goals[i]];
        if (p.length() < 180.0)
        {
            curr_goals[i] = (curr_goals[i] + 1) % 2;
            if (temptac[i])
            {
                delete temptac[i];
            }
            int obs = OBS_EVERYTHING_BUT_ME(i) & (~OBS_OUR_DZONE) & (~OBS_THEIR_DZONE);
            temptac[i] = new SPosition((BCoordinate)goalvec[curr_goals[i]], obs);
        }
    }
    for (int i=0; i < world.n_teammates; i++)
    {
        tactics[i]=temptac[i];
    }
}
