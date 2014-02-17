// play.cc
//
// Encapsulates team behavior.
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

#include <sys/types.h>
#include <sys/stat.h>

#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/evaluation.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/goalie.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/play/play.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/play/play.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/goalie.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"
#include <stdarg.h>

//判断分析对手的主要信息。
PlayRole::PlayRole(Tactic *t, ...)
{
	va_list ap;
	va_start(ap, t);
	tactics.push_back(t);
	while ((t = va_arg(ap, Tactic *)))
	{
		tactics.push_back(t);
	}
}

PlayRole::PlayRole(const char *string, int &n, bool &error)
{
	error = false;
	for (uint i=0; i<tactics.size(); i++)
	{
		delete tactics[i];
	}
	tactics.clear();
	// Find ROLE keyword.
	//解析ROLE关键字
	static char w[5];
	n += Parse::pWord(string + n, w, 4);
	if (strcmp(w, "ROLE") != 0)
	{
		fprintf(stderr, "ERROR: PlayRole could not find ROLE keyword.\n");
		error = true;
		return;
	}
	n += Parse::skipLine(string + n);
	//解析ROLE相关tactics
	// Read tactics making up role.
	while (1)
	{
		n += Parse::skipWS(string + n);
		if (string[n] == '\0')
		{
			break;
		}
		char *word;
		Parse::pWord(string + n, &word);
		if (strcmp(word, "ROLE") == 0)
		{
			//      n -= strlen(word);
			delete [] word;
			break;
		}
		delete [] word;
		Tactic *t = Tactic::parse(string + n);

		n += Parse::skipLine(string + n);
		//    if (!t) break;
		if (t)
		{
			tactics.push_back(t);
		}
	}
}

//求tactics最大长度
uint Play::length()
{
	uint l = 0;
	for (uint i=0; i<roles.size(); i++)
	{
		PlayRole &r = getRole(i);
		if (r.tactics.size() > l)
		{
			l = r.tactics.size();
		}
	}
	return l;
}

//PlayAscII用于解析脚本
//---------------------------------------------------------------------------------------------------------
PlayAscii::PlayAscii(const char *filename, bool &error)
{
	//printf("PlayAscii %s constructed\r\n",filename);
	struct stat sbuf;
	int length;
	error = false;
	if (stat(filename, &sbuf) != 0)
	{
		fprintf(stderr, "ERROR: PlayAscii could not stat file, %s\n", filename);
		error = true;
		return;
	}
	length = sbuf.st_size;
	FILE *f = fopen(filename, "r");
	if (!f)
	{
		fprintf(stderr, "ERROR: PlayAscii could not open file, %s\n", filename);
		error = true;
		return;
	}
	char* contents= new char[length + 1];
	fread(contents, sizeof(char), length, f);
	contents[length] = '\0';
	fclose(f);
	int n = 0;
	if (!parse(contents, n))
	{
		fprintf(stderr, "ERROR: Could not parse file, %s\n", filename);
		error = true;
	}
	delete contents;
}

PlayAscii::PlayAscii(const char *string, int &n, bool &error)
{
	printf("PlayAscii constructor\r\n");
	error = parse(string, n);
}

void PlayAscii::printErrorLine(const char *string, int n)
{
	int linenum = 0, linebeginning = 0, lineend = 0;
	int i;
	for (i=0; i<n; i++)
	{
		if (string[i] == '\n')
		{
			linenum++;
			linebeginning = i + 1;
		}
	}
	for (lineend = i;
	        string[lineend] != '\n' && string[lineend] != '\0';
	        lineend++);
	fprintf(stderr, "ERROR: On line %d.\n>> ", linenum);
	fwrite(string + linebeginning, sizeof(char),
	       lineend - linebeginning, stderr);
	fprintf(stderr, "\n");
}

bool PlayAscii::parse(const char *string, int &n)
{
	// Initialize fixed roles.
	fixed_roles.clear();
	for (int i=0; i<MAX_PLAY_ROLES; i++)
	{
		fixed_roles.push_back(i);
	}
	// Initialize timeout
	//默认每个tactics最多维持25秒
	play_timeout = 25.0;
	// Find PLAY keyword.
	static char w[5];
	n += Parse::pWord(string + n, w, 4);
	if (strcmp(w, "PLAY") != 0)
	{
		fprintf(stderr, "ERROR: PlayAscii could not find PLAY keyword.\n");
		printErrorLine(string, n);
		return false;
	}
	// Read name.
	n += Parse::pLine(string + n, &name);
	n += Parse::skipLine(string + n);
	// Read conditions.
	while (1)
	{
		char *word;
		n += Parse::pWord(string + n, &word);
		if (strcmp(word, "APPLICABLE") == 0)
		{
			//解析规则启动条件
			applicable.push_back(parsePredicate(string, n));
			n += Parse::skipLine(string + n);
		}
		else if (strcmp(word, "DONE") == 0)
		{
			//解析规则终止条件（可能是多个）
			static char rv[16];
			n += Parse::pWord(string + n, rv, 16);
			if (strcmp(rv, "succeeded") == 0) done_status.push_back(Succeeded);
			else if (strcmp(rv, "failed") == 0) done_status.push_back(Failed);
			else if (strcmp(rv, "aborted") == 0) done_status.push_back(Aborted);
			else if (strcmp(rv, "completed") == 0) done_status.push_back(Completed);
			else
			{
				fprintf(stderr, "ERROR: PlayAscii unknown done status, %s.\n", rv);
				printErrorLine(string, n);
				done_status.push_back(Failed);
			}
			done.push_back(parsePredicate(string, n));
		}
		else if (strcmp(word, "FIXEDROLES") == 0)
		{
			//固定角色
			for (int i=0; i<MAX_PLAY_ROLES; i++)
			{
				n += Parse::pInt(string + n, fixed_roles[i]);
			}
			n += Parse::skipLine(string + n);
		}
		else if (strcmp(word, "TIMEOUT") == 0)
		{
			//超时
			n += Parse::pDouble(string + n, play_timeout);
		}
		else if (strcmp(word, "OROLE") == 0)
		{
			//用于告知系统，本play关心哪些对手角色
			//对方角色
			OpponentRole o = parseORole(string, n);
			if (o) opponent_roles.push_back(o);
		}
		else if (strcmp(word, "ROLE") == 0)
		{
			//己方角色
			n -= 4;
			delete [] word;
			break;
		}
		else
		{
			fprintf(stderr, "ERROR: PlayAscii unknown key word, %s.\n", word);
			printErrorLine(string, n);
			n += Parse::skipLine(string + n);
		}
		delete [] word;
	}
	// Read roles.
	for (int i=0; i<MAX_PLAY_ROLES; i++)
	{
		static bool error;
		PlayRole r(string, n, error);
		if (error)
		{
			printErrorLine(string, n);
			return false;
		}
		roles.push_back(r);
	}
	return true;
}

//寻找对方机器人中最有威胁的射手
int PlayAscii::orole_best_shot(World &world, bool candidates[])
{
	int best_id = -1;
	double best = 0.0;
	if (world.orole_goalie >= 0) candidates[world.orole_goalie] = false;
	for (int i=0; i<world.n_opponents; i++)
	{
		if (!candidates[i]) continue;
		MyVector2d target;
		double target_tolerance;
		int obs_flags = 0;
		//如果存在己方守门员
		if (world.trole_goalie >= 0)
		{
			obs_flags |= OBS_TEAMMATE(world.trole_goalie);
		}
		evaluation.aim(world, world.now, world.opponent_position(i),
		               world.our_goal_r, world.our_goal_l,
		               obs_flags, target, target_tolerance);
		if (best_id < 0 || target_tolerance > best)
		{
			best_id = i;
			best = target_tolerance;
		}
	}
	return best_id;
}

//寻找离对方门最近的对方机器人，排除对方守门员
int PlayAscii::orole_most_downfield(World &world, bool candidates[])
{
	int best_id = -1;
	double best = 0.0;
	//排除对方守门员
	if (world.orole_goalie >= 0)
	{
		candidates[world.orole_goalie] = false;
	}
	for (int i=0; i<world.n_opponents; i++)
	{
		if (!candidates[i]) continue;
		if (best_id < 0 || world.opponent_position(i).x > best)
		{
			best_id = i;
			best = world.opponent_position(i).x;
		}
	}
	return best_id;
}

//脚本解析用
//寻找离己方门最近的对方机器人，排除己方守门员
int PlayAscii::orole_most_upfield(World &world, bool candidates[])
{
	int best_id = -1;
	double best = 0.0;
	//orole_goalie 己方守门员
	if (world.orole_goalie >= 0)
	{
		candidates[world.orole_goalie] = false;
	}
	for (int i=0; i<world.n_opponents; i++)
	{
		if (!candidates[i]) continue;
		if (best_id < 0 || world.opponent_position(i).x < best)
		{
			best_id = i;
			best = world.opponent_position(i).x;
		}
	}
	return best_id;
}

//返回对方守门员编号
int PlayAscii::orole_goalie(World &world, bool candidates[])
{
	if (world.orole_goalie >= 0 && candidates[world.orole_goalie])
	{
		return world.orole_goalie;
	}
	return orole_most_upfield(world, candidates);
}

//寻找对方离球最近的机器人,排除守门员
int PlayAscii::orole_closest_to_ball(World &world, bool candidates[])
{
	int best_id = -1;
	double best = 0.0;
	//排除守门员
	if (world.orole_goalie >= 0)
	{
		candidates[world.orole_goalie] = false;
	}
	for (int i=0; i<world.n_opponents; i++)
	{
		if (!candidates[i]) continue;
		double d = (world.opponent_position(i) - world.ball_position()).length();
		if (best_id < 0 || d < best)
		{
			best_id = i;
			best = d;
		}
	}
	return best_id;
}

//在对方机器人中寻找能最快截断射球的
int PlayAscii::orole_closest_to_shot(World &world, bool candidates[])
{
	int best_id = -1;
	double best = 0.0;
	if (world.orole_goalie >= 0) candidates[world.orole_goalie] = false;
	MyVector2d ball = world.ball_position();
	MyVector2d goal = world.their_goal;
	for (int i=0; i<world.n_opponents; i++)
	{
		if (!candidates[i]) continue;
		//求两直线段距离
		double d = distance_seg_to_seg(ball, goal,
		                               world.opponent_position(i),
		                               world.opponent_position(i));
		if (best_id < 0 || d < best)
		{
			best_id = i;
			best = d;
		}
	}
	return best_id;
}

//寻找对手中最好的传球手
int PlayAscii::orole_best_pass(World &world, bool candidates[])
{
	int best_id = -1;
	double best = 0.0;
	if (world.orole_goalie >= 0) candidates[world.orole_goalie] = false;
	for (int i=0; i<world.n_opponents; i++)
	{
		if (!candidates[i]) continue;
		MyVector2d target;
		double target_tolerance;
		int obs_flags = 0;
		//如果存在己方守门员
		if (world.trole_goalie >= 0)
		{
			obs_flags |= OBS_TEAMMATE(world.trole_goalie);
		}
		evaluation.aim(world, world.now, world.opponent_position(i),
		               world.our_goal_r, world.our_goal_l,
		               obs_flags, target, target_tolerance);
		//弧度=0.2442,相当于角度=14度
		//如果角度太小,就忽略
		if (target_tolerance < 0.2442) continue;
		//寻找离球最近的
		double d = (world.opponent_position(i) - world.ball_position()).length();
		if (best_id < 0 || d < best)
		{
			best_id = i;
			best = d;
		}
	}
	return best_id;
}

//条件解析
WorldPredicateConjunct &PlayAscii::parsePredicate(const char *string, int &n)
{
	static WorldPredicateConjunct pred;
	pred.clear();
	while (string[n] != '\n' && string[n] != '\0')
	{
		char *word;
		int negate;
		double argument = 0;
		bool paren = false;
		n += Parse::skipWS(string + n, " \t");
		if (string[n] == '{')
		{
			n++;
			paren = true;
		}
		n += Parse::pWord(string + n, &word);
		n += Parse::skipWS(string + n, " \t");
		if (paren && string[n] != '}') n += Parse::pDouble(string + n, argument);
		n += Parse::skipWS(string + n, " \t}");
		negate = (word[0] == '!') ? 1 : 0;
		if (strcmp(word + negate, "offense") == 0)
			pred.add(&pred_offense, negate, argument);
		else if (strcmp(word + negate, "defense") == 0)
			pred.add(&pred_defense, negate, argument);
		else if (strcmp(word + negate, "their_ball") == 0)
			pred.add(&pred_defense, negate, argument);
		else if (strcmp(word + negate, "our_ball") == 0)
			pred.add(&pred_defense, negate, argument);
		else if (strcmp(word + negate, "loose_ball") == 0)
			pred.add(&pred_defense, negate, argument);
		else if (strcmp(word + negate, "their_side") == 0)
			pred.add(&pred_defense, negate, argument);
		else if (strcmp(word + negate, "our_side") == 0)
			pred.add(&pred_defense, negate, argument);
		else if (strcmp(word + negate, "midfield") == 0)
			pred.add(&pred_defense, negate, argument);
		else if (strcmp(word + negate, "our_kickoff") == 0)
			pred.add(&pred_our_kickoff, negate, argument);
		else if (strcmp(word + negate, "their_kickoff") == 0)
			pred.add(&pred_their_kickoff, negate, argument);
		else if (strcmp(word + negate, "our_freekick") == 0)
			pred.add(&pred_our_freekick, negate, argument);
		else if (strcmp(word + negate, "their_freekick") == 0)
			pred.add(&pred_their_freekick, negate, argument);
		else if (strcmp(word + negate, "our_penalty") == 0)
			pred.add(&pred_our_penalty, negate, argument);
		else if (strcmp(word + negate, "their_penalty") == 0)
			pred.add(&pred_their_penalty, negate, argument);
		else if (strcmp(word + negate, "ball_x_gt") == 0)
			pred.add(&pred_ball_x_gt, negate, argument);
		else if (strcmp(word + negate, "ball_x_lt") == 0)
			pred.add(&pred_ball_x_lt, negate, argument);
		else if (strcmp(word + negate, "ball_absy_gt") == 0)
			pred.add(&pred_ball_absy_gt, negate, argument);
		else if (strcmp(word + negate, "ball_absy_lt") == 0)
			pred.add(&pred_ball_absy_lt, negate, argument);
		else if (strcmp(word + negate, "in_our_corner") == 0)
			pred.add(&pred_in_our_corner, negate, argument);
		else if (strcmp(word + negate, "in_their_corner") == 0)
			pred.add(&pred_in_their_corner, negate, argument);
		else if (strcmp(word + negate, "n_opponents_our_side") == 0)
			pred.add(&pred_nopponents_our_side, negate, argument);
		else
		{
			fprintf(stderr, "ERROR: PlayAscii could not parse predicate, %s... ignoring.\n", word + negate);
		}
		delete [] word;
	}
	return pred;
}

//
OpponentRole PlayAscii::parseORole(const char *string, int &n)
{
	OpponentRole o = NULL;
	int id;
	char *word;
	n += Parse::skipWS(string + n, " \t");
	n += Parse::pInt(string + n, id); // Ignored, but syntactically pretty.
	n += Parse::pWord(string + n, &word);
	if (strcmp(word, "best_shot") == 0) o = &orole_best_shot;//寻找对方机器人中最有威胁的射手
	else if (strcmp(word, "most_downfield") == 0) o = &orole_most_downfield;
	else if (strcmp(word, "most_upfield") == 0) o = &orole_most_upfield;
	else if (strcmp(word, "closest_to_ball") == 0) o = &orole_closest_to_ball;
	else if (strcmp(word, "closest_to_shot") == 0) o = &orole_closest_to_ball;
	else if (strcmp(word, "best_pass") == 0) o = &orole_closest_to_ball;
	else if (strcmp(word, "goalie") == 0) o = &orole_goalie;
	else
	{
		fprintf(stderr, "ERROR: PlayAscii could not parse opponent role, %s... ignoring.\n", word);
		printErrorLine(string, n - 1);
	}
	delete [] word;
	return o;
}
