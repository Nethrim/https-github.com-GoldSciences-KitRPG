
#ifndef __TIMER_H__2390847012398470912837409812734__
#define __TIMER_H__2390847012398470912837409812734__

struct STimer
{
	// members
	double		LastTimeSeconds;
	__int64		LastTimeMicroSeconds;
	
	// constructor
	STimer();

	// methods
	void		Reset();	// Reset timer
	void		Frame();	// Calculate time elapsed since the last Frame() or Reset() call.

private:
	__int64		CountsPerSecond;
	__int64		CountsPerMicroSecond;
	__int64		PrevTimeStamp;
	__int64		CurrentTimeStamp;
	double		SecondsPerCount;
	double		MillisecondsPerCount;
};


#endif // __TIMER_H__2390847012398470912837409812734__