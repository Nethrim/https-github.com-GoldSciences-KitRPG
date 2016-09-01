#include <cstdint>

#ifndef __TIMER_H__2390847012398470912837409812734__
#define __TIMER_H__2390847012398470912837409812734__

#pragma pack(push, 1)

struct STimer
{
	// members
	double		LastTimeSeconds			= 0;
	int64_t		LastTimeMicroSeconds	= 0;
	double		FramesLastSecond		= 0;
	
	// constructor
	STimer();

	// methods
	void		Reset();	// Reset timer
	void		Frame();	// Calculate time elapsed since the last Frame() or Reset() call.

private:
	int64_t		CountsPerSecond			= 0;
	int64_t		CountsPerMicroSecond	= 0;
	int64_t		PrevTimeStamp			= 0;
	int64_t		CurrentTimeStamp		= 0;
	double		SecondsPerCount			= 0.0;
	double		MillisecondsPerCount	= 0.0;
	
	// The following are to get the FPS.
	double		FrameCounterSeconds		= 0.0;	
	int64_t		FramesThisSecond		= 0;
};

#pragma pack(pop)

#endif // __TIMER_H__2390847012398470912837409812734__