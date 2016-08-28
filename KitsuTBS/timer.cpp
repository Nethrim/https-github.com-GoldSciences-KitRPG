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
	SecondsPerCount			= (1.0 / (CountsPerSecond));
	QueryPerformanceCounter( ( LARGE_INTEGER* )&PrevTimeStamp );		
	LastTimeMicroSeconds	= 0;
	LastTimeSeconds			= 0;

};

void STimer::Frame()
{
	QueryPerformanceCounter( ( LARGE_INTEGER* ) &CurrentTimeStamp );
	LastTimeMicroSeconds	= (CurrentTimeStamp - PrevTimeStamp)/CountsPerMicroSecond;
	LastTimeSeconds			= (CurrentTimeStamp - PrevTimeStamp) * SecondsPerCount;//LastTimeMicroSeconds*1000000;//(CurrentTimeStamp - PrevTimeStamp) * SecondsPerCount;
	PrevTimeStamp = CurrentTimeStamp;
};
