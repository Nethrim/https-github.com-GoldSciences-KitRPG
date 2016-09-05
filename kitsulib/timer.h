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

		// The Accumulate and Deplete functions of SAccumulator won't prevent overflow when using negative numbers as arguments.
		// This is so we don't have unnecessary condition checks in places where we are going to operate always with positive values
		_ValueType		Accumulate( _ValueType amount ){
			if( (Value += amount) > MaxValue ) {
				_ValueType difference = Value-MaxValue;
				Value = MaxValue;
				return difference;
			}
			
			return 0;
		}

		// The Accumulate and Deplete functions of SAccumulator won't prevent overflow when using negative numbers as arguments.
		// This is so we don't have unnecessary condition checks in places where we know that we are going to operate always with positive values
		// 
		_ValueType		Deplete( _ValueType amount ){
			if( (Value -= amount) < 0 ) {
				_ValueType difference = Value;
				Value = 0;
				return difference;
			}
			
			return 0;
		}
	};

	//template<typename _ValueType>
	//struct SAccumulatorStatistics
	//{
	//	_ValueType	Accumulated;
	//	_ValueType	Depleted;
	//	uint64_t	TimesAccumulated;
	//	uint64_t	TimesDepleted;
	//
	//	inline _ValueType		Accumulate	( _ValueType amount ){ TimesAccumulated	++;	return Accumulated	+= amount; } //return amount; }
	//	inline _ValueType		Deplete		( _ValueType amount ){ TimesDepleted	++;	return Depleted		+= amount; } //return amount; }
	//};

	// This thing is to provide high-precision 
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
		double		CountsPerMicroSecond	= 0;
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