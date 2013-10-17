/*
 * hires_timer.cpp
 */

#include "hires_timer.h"
#include <cstddef>

StrategyTimer::StrategyTimer()
{
	//LARGE_INTEGERs and __int64s are interchangable
//	QueryPerformanceFrequency( (LARGE_INTEGER *) &freq );
//        //The function using StrategyTimer must check the frequency for 0;
//	//initialize these
//	QueryPerformanceCounter( (LARGE_INTEGER *) &startTime );
//	QueryPerformanceCounter( (LARGE_INTEGER *) &endTime );
//	QueryPerformanceCounter( (LARGE_INTEGER *) &lastLoopBeginTime );


//        int clock_getres(clockid_t clock_id, struct timespec *res);
//        int clock_gettime(clockid_t clock_id, struct timespec *tp);
//        int clock_settime(clockid_t clock_id, const struct timespec *tp);
}
double StrategyTimer::MarkStartTime()
{
    gettimeofday( &tv_start,NULL);
    return (double)(tv_start.tv_sec+tv_start.tv_usec);
}

double StrategyTimer::MarkEndTime()
{
    gettimeofday( &tv_stop, NULL );
    return (double)(tv_stop.tv_sec+tv_stop.tv_usec);
}
//void StrategyTimer::markStartTime()
//{
//	QueryPerformanceCounter( (LARGE_INTEGER *) &startTime );
//}

//void StrategyTimer::markEndTime()
//{
//	QueryPerformanceCounter( (LARGE_INTEGER *) &endTime );
//}

//// after markStartTime and markEndTime have been called,
//// you can call getInterval to get the elapsed time;
//double StrategyTimer::GetInterval() const
//{
//        return (double)(tv_stop.tv_sec - tv_start.tv_sec)*1000 + (double)(tv_stop.tv_usec - tv_start.tv_usec)/1000.0;
//}
double StrategyTimer::GetInterval_ms() const
{
        return (double)(tv_stop.tv_sec - tv_start.tv_sec)*1000 + (double)(tv_stop.tv_usec - tv_start.tv_usec)/1000.0;
}

//double StrategyTimer::getElapsedTime() const
//{
//        int64_t currentTime;
//	QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
//	return (double(currentTime - startTime)) / freq;
//}

//double StrategyTimer::getElapsedAndReset() const
//{
//        int64_t lastStartTime = startTime;
//	QueryPerformanceCounter( (LARGE_INTEGER *) &startTime );
//	return (double(startTime - lastStartTime)) / freq;
//}


////delays the current thread by "interval" milliseconds
//// from the current time
//void StrategyTimer::Delay_ms(double interval) const
//{
//        int64_t beginTime, currentTime;
//	QueryPerformanceCounter( (LARGE_INTEGER *) &beginTime );
//	QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
//	double timeSoFar_ms = (double(currentTime - beginTime)) * 1000 / freq;
//	if (timeSoFar_ms < (interval*.8 - 10))
//	{
//		//Sleep conservatively:  if we'll be waiting for a while, let other processes do stuff;
//		// but when we get close to the deadline wake up and start spinning so we get accurate performance.
//		Sleep( (int)(interval*.8 - 10 - timeSoFar_ms) );
//	}
//        int64_t intervalInTicks = int64_t(freq * interval / 1000);
//	do
//	{
//		QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
//	}
//	while ( (currentTime - beginTime) <= intervalInTicks );
//}

////delays the processor until at least "interval" ms have elapsed since the
//// last call to ensureLoopInterval_ms or since the StrategyTimer was initialized
////
////Returns the amount of time (in ms) between the start of this call and the
//// end of the previous  (i.e. the amount of time spent doing useful work
//// between calls to ensureLoopInterval_ms).
//double StrategyTimer::ensureLoopInterval_ms(double interval)
//{
//        int64_t currentTime;
//	QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
//	double timeSinceLastCall = (double(currentTime - lastLoopBeginTime)) * 1000 / freq;
//	//====== Let other processes do stuff ======
//	double timeSoFar_ms = (double(currentTime - lastLoopBeginTime)) * 1000 / freq;
//	if (timeSoFar_ms < (interval*.8 - 10))
//	{
//		//Sleep conservatively:  if we'll be waiting for a while, let other processes do stuff;
//		// but when we get close to the deadline wake up and start spinning so we get accurate performance.
//		Sleep( (int)(interval*.8 - 10 - timeSoFar_ms) );
//	}
//	//====== Now spin until time is up. ========
//        int64_t intervalInTicks = int64_t(freq * interval / 1000);
//	do
//	{
//		QueryPerformanceCounter( (LARGE_INTEGER *) &currentTime );
//	}
//	while ( (currentTime - lastLoopBeginTime) <= intervalInTicks );
//	lastLoopBeginTime = currentTime;
//	return timeSinceLastCall;
//}

