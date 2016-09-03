#include <cstdint>

#ifndef __TIMER_H__2390847012398470912837409812734__
#define __TIMER_H__2390847012398470912837409812734__

#pragma pack(push, 1)
namespace klib
{
	template<typename _ValueType>
	struct SAccumulator
	{
		_ValueType	Value;
		_ValueType	MaxValue;

		bool		Accumulate( _ValueType amount ){
			if( Value >= MaxValue )
				return true;
			
			Value += amount;
			if( Value > MaxValue )
				Value = MaxValue;
			
			return MaxValue == Value;
		}

		bool		Deplete( _ValueType amount ){
			if( Value == 0 )
				return true;
			
			Value += amount*-1;
			if( Value < 0 )
				Value = 0;
			
			return 0 == Value;
		}
	};

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
};

#pragma pack(pop)

#endif // __TIMER_H__2390847012398470912837409812734__