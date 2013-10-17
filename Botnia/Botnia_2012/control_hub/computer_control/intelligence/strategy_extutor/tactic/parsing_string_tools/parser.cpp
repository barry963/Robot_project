// parse.cc
//
// Utilities for parsing strings and files.
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

#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/parsing_string_tools/parser.h"

int Parse::skipWS(const char *string, const char *ws)
{
	return strspn(string, ws);
}

int Parse::skipLine(const char *string)
{
	return strcspn(string, "\n") + 1;
}

int Parse::pInt(const char *string, int &x)
{
	char *end;
	x = strtol(string, &end, 0);
	return (end - string);
}

int Parse::pDouble(const char *string, double &x)
{
	char *end;
	x = strtod(string, &end);
	return (end - string);
}

int Parse::pChar(const char *string, char &x)
{
	int n = Parse::skipWS(string);
	x = string[n];
	return n+1;
}

int Parse::pWord(const char *string, char *x, int n)
{
	int s = Parse::skipWS(string);
	int l = strcspn(string + s, " \n\t");
	strncpy(x, string + s, MIN(l, n));
	x[MIN(l, n)] = '\0';
	return s + l;
}

int Parse::pWord(const char *string, char **x)
{
	int s = Parse::skipWS(string);
	int l = strcspn(string + s, " \n\t");
	*x = new char[l + 1];
	strncpy(*x, string + s, l);
	(*x)[l] = '\0';
	return s + l;
}

int Parse::pLine(const char *string, char **x)
{
	int s = Parse::skipWS(string);
	int l = strcspn(string + s, "\n");
	*x = new char[l + 1];
	strncpy(*x, string + s, l);
	(*x)[l] = '\0';
	return s + l;
}

int Parse::pBool(const char *string, bool &x)
{
	static char w[2];
	int n;
	n = pWord(string, w, 2);
	if (w[0] == 'f' || w[0] == 'F' || w[0] == '0') x = false;
	else x = true;
	return n;
}

int Parse::pVector2d(const char *string, MyVector2d &x)
{
	int n = 0;
	n += skipWS(string + n, " \n\t{");
	n += pDouble(string + n, x.x);
	n += skipWS(string + n, " \n\t,");
	n += pDouble(string + n, x.y);
	n += skipWS(string + n, " \n\t}");
	return n;
}

int Parse::pTCoordinate(const char *string, BCoordinate &x)
{
	int n = 0;
	n += skipWS(string + n, " \n\t{");
	BCoordinate::stype s = BCoordinate::SAbsolute;
	BCoordinate::otype o = BCoordinate::OAbsolute;
	bool d = false;
	if (string[n] == 'A')
	{
		s = BCoordinate::SAbsolute;
		n++;
	}
	else if (string[n] == 'B')
	{
		s = BCoordinate::SBall;
		n++;
	}
	else if (string[n] == 'S')
	{
		s = BCoordinate::SStrong;
		n++;
	}
	else if (string[n] == 'O')
	{
		s = BCoordinate::SBallOrStrong;
		n++;
	}
	else if (string[n] == 'G')
	{
		s = BCoordinate::SGui;
		n++;
	}
	if (string[n] == 'A')
	{
		o = BCoordinate::OAbsolute;
		n++;
	}
	else if (string[n] == 'B')
	{
		o = BCoordinate::OBall;
		n++;
	}
	if (string[n] == 'd')
	{
		d = true;
		n++;
	}
	MyVector2d v;
	n += pVector2d(string + n, v);
	x = BCoordinate(v, s, o, d);
	return n;
}

int Parse::pTCoordinateDir(const char *string, BCoordinate &x)
{
	int n = 0;
	n += skipWS(string + n, " \n\t{");
	BCoordinate::stype s = BCoordinate::SAbsolute;
	BCoordinate::otype o = BCoordinate::OAbsolute;
	bool d = false;
	if (string[n] == 'A')
	{
		s = BCoordinate::SAbsolute;
		n++;
	}
	else if (string[n] == 'B')
	{
		s = BCoordinate::SBall;
		n++;
	}
	else if (string[n] == 'S')
	{
		s = BCoordinate::SStrong;
		n++;
	}
	else if (string[n] == 'O')
	{
		s = BCoordinate::SBallOrStrong;
		n++;
	}
	else if (string[n] == 'G')
	{
		s = BCoordinate::SGui;
		n++;
	}
	if (string[n] == 'A')
	{
		o = BCoordinate::OAbsolute;
		n++;
	}
	else if (string[n] == 'B')
	{
		o = BCoordinate::OBall;
		n++;
	}
	if (string[n] == 'd')
	{
		d = true;
		n++;
	}
	double dir;
	n += pDouble(string + n, dir);
	x = BCoordinate(MyVector2d(1, 0).rotate(dir), s, o, d);
	return n;
}

int Parse::pTRegion(const char *string, BRegion &x)
{
	int n = 0;
	n += skipWS(string + n, " \n\t{");
	if (string[n] == 'R')
	{
		BCoordinate p1, p2;
		double radius;
		n++;
		n += pTCoordinate(string + n, p1);
		n += pTCoordinate(string + n, p2);
		n += pDouble(string + n, radius);
		x = BRegion(p1, p2, radius);
	}
	else if (string[n] == 'C')
	{
		BCoordinate center;
		double radius;
		n++;
		n += pTCoordinate(string + n, center);
		n += pDouble(string + n, radius);
		x = BRegion(center, radius);
	}
	else return 0;
	return n;
}

