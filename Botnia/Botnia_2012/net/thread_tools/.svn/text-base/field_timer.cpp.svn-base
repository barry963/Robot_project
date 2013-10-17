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

/**    @file    - fied_timer.cpp
  *
  *    @class   -
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 10/18/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */

#include "field_timer.h"



#ifndef GETTICKCOUNT
#define GETTICKCOUNT

long GetTickCount()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
#endif

//typedef union Large_Integer_
//{
//    int64_t QuadPart;
//}LARGE_INTEGER;
int64_t litmp;
int64_t QPart1,QPart2;
double dfMinus, dfFreq, dfTim;
static const unsigned usec_per_sec = 1000000;
static const unsigned usec_per_msec = 1000;

/* Helpful conversion constants. */


/* These functions are written to match the win32
   signatures and behavior as closely as possible.
*/
bool QueryPerformanceFrequency(int64_t *frequency)
{
    /* Sanity check. */
    assert(frequency != NULL);

    /* gettimeofday reports to microsecond accuracy. */
    *frequency = usec_per_sec;

    return true;
}

bool QueryPerformanceCounter(int64_t *performance_count)
{
    struct timeval time;


    /* Sanity check. */
    assert(performance_count != NULL);

    /* Grab the current time. */
    gettimeofday(&time, NULL);
    *performance_count = time.tv_usec + /* Microseconds. */
            time.tv_sec * usec_per_sec; /* Seconds. */

    return true;
}


// porting time function to limux
// LARGE_INTEGER is a 64 bits union




void TimerInit()
{
    QueryPerformanceFrequency(&litmp);
    //    dfFreq = (double)litmp.QuadPart;
    dfFreq = (double)litmp;
    QueryPerformanceCounter(&litmp);
    //    QPart1 = litmp.QuadPart;
    QPart1 = litmp;
}

double GetTimeSec()
{
    QueryPerformanceCounter(&litmp);
    //    QPart2 = litmp.QuadPart;
    QPart2 = litmp;
    dfMinus = (double)(QPart2-QPart1);
    return dfMinus / dfFreq;
}

// it seems under windows it is milli sec
void Sleep(double minisec)
{
    usleep((int)(minisec * 1E3));
}

