#ifndef __MISC_H__982143092634987160987328097642398__
#define __MISC_H__982143092634987160987328097642398__

namespace klib
{
#pragma pack(push, 1)

	template <typename _T, size_t _Size> 
	inline constexpr size_t size(const _T (&staticArray)[_Size]) { return _Size; }

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
			
			return false;
		}
	};
#pragma pack(pop)
}

#endif // __MISC_H__982143092634987160987328097642398__