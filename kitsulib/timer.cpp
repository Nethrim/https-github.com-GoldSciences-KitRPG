#include "timer.h"
#include <windows.h>

STimer::STimer()
{ 
	Reset();
};

void STimer::Reset()
{
	QueryPerformanceFrequency( ( LARGE_INTEGER* )&CountsPerSecond );
	CountsPerMicroSecond	= CountsPerSecond/1000000;
	if( 0 == CountsPerMicroSecond )
		CountsPerMicroSecond = 1;
	SecondsPerCount			= (1.0 / (CountsPerSecond));
	QueryPerformanceCounter( ( LARGE_INTEGER* )&PrevTimeStamp );		
	LastTimeMicroSeconds	= 0;
	LastTimeSeconds			= 0;

	FramesLastSecond		= 0;
};

void STimer::Frame()
{
	QueryPerformanceCounter( ( LARGE_INTEGER* ) &CurrentTimeStamp );
	LastTimeMicroSeconds	= (CurrentTimeStamp - PrevTimeStamp)/CountsPerMicroSecond;
	LastTimeSeconds			= (CurrentTimeStamp - PrevTimeStamp) * SecondsPerCount;//LastTimeMicroSeconds*1000000;//(CurrentTimeStamp - PrevTimeStamp) * SecondsPerCount;
	PrevTimeStamp = CurrentTimeStamp;

	FramesThisSecond++;
	FrameCounterSeconds	+= LastTimeSeconds;

	int32_t totalFrames		= int32_t(FramesThisSecond/FrameCounterSeconds	);
	int32_t framesPerSecond	= int32_t(totalFrames/FrameCounterSeconds		);
	while(FrameCounterSeconds >= 1.0)
	{
		FramesLastSecond	= framesPerSecond;
		FrameCounterSeconds	-= 1.0f;
		FramesThisSecond	= 0;
	}
};
