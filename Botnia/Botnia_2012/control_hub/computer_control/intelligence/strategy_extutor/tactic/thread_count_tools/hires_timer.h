#if 0
/*****************************************
*   hires_timer.h                        *
*                                        *
* A high-precision timer class based on  *
* QueryPerformanceCounter().             *
*                                        *
*****************************************/

#ifndef HIRES_TIMER_H
#define HIRES_TIMER_H


#include <inttypes.h>
#include <sys/time.h>
class StrategyTimer
{
public:
        StrategyTimer();

	//call this at least once before using the timer.  I'm pretty sure
	// all x86 machines support QueryPerformanceCounter(), but no guarantees
//        inline bool IsTimerSupported() const
//	{
//		return freq != 0;
//	}

        // after MarkStartTime and MarkEndTime have been called,
        // you can call GetInterval to get the elapsed time;
        double MarkStartTime();
        double MarkEndTime();

//        //GetInterval() retreives the time bewteen calls to MarkStartTime and MarkEndTime,
//        double GetInterval() const;	  //seconds
        double GetInterval_ms() const;//milliseconds

//        //GetElapsedTime() returns the amount of time that has passed since the last call to MarkStartTime.
//        double GetElapsedTime() const;// seconds

//	//like above, but also resets the start time
//        double GetElapsedAndReset() const;// milliseconds

//	//
//	// Delay routines:
//	//

//	//delays the current thread by "interval" milliseconds
//	// from the current time.
//        void Delay_ms(double interval) const;


	//call this during a loop (e.g. the gamepad)
	// to ensure that the loop will be executed exactly once every
	// "interval" milliseconds.
	//The first time this is called, it will delay until at least
        // "interval" ms have elapsed since the StrategyTimer was constructed.
	//
	//Returns the amount of time (in ms) between the start of this call and the
	// end of the previous  (i.e. the amount of time spent doing useful work
        // between calls to EnsureLoopInterval_ms).
//        double EnsureLoopInterval_ms(double interval);

private:
	//__int64 is a 64-bit signed integer in VC++
//        int64_t freq;
        struct timeval tv_start;
        struct timeval tv_stop;
};

#endif //HIRES_TIMER_H

#endif



#ifndef HIRES_TIMER_H
#define HIRES_TIMER_H


#include <inttypes.h>
#include <sys/time.h>
class StrategyTimer
{
public:
        StrategyTimer();
        double MarkStartTime();
        double MarkEndTime();
        double GetInterval_ms() const;//milliseconds

private:
        struct timeval tv_start;
        struct timeval tv_stop;
};

#endif
