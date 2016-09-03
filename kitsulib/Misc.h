#ifndef __MISC_H__982143092634987160987328097642398__
#define __MISC_H__982143092634987160987328097642398__

namespace klib
{
#pragma pack(push, 1)

	template <typename _T, size_t _Size> 
	static inline constexpr size_t size(const _T (&staticArray)[_Size]) { return _Size; }


#pragma pack(pop)
}

#endif // __MISC_H__982143092634987160987328097642398__