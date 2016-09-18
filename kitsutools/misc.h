#include <cstdint>

#ifndef __MISC_H__982143092634987160987328097642398__
#define __MISC_H__982143092634987160987328097642398__

namespace ktools
{
#pragma pack(push, 1)

	template <typename _T, size_t _Size> 
	static inline constexpr size_t size(const _T (&staticArray)[_Size]) { return _Size; }

	enum ALIGN : uint8_t
	{	LEFT	= 0x1
	,	RIGHT	= 0x2
	,	CENTER	= LEFT | RIGHT
	,	TOP		= 0x4
	,	BOTTOM	= 0x8
	,	VCENTER	= TOP | BOTTOM
	};

	// This function copies an amount of _TCell values into a rectangle of _TCells 
	// with rectangleTopLeft specifying the origin of the rectangle 
	// and width and height specifying the sizes from there (with Y axis being up to down most likely)
	template<typename _TCell>
	int32_t valueToRect( _TCell* rectangleTopLeft, size_t width, size_t height, int32_t offsetLine, int32_t offsetColumn, ALIGN align, const _TCell* values, int32_t valueCount, int32_t repeat=0 )
	{
		if(align & RIGHT) {	// testing for RIGHT bit would tell us that this might be aligned to either RIGHT or CENTER.
			int32_t offsetX = (int32_t) ((align == CENTER) ? (width>>1)-(valueCount>>1)+offsetColumn : (int32_t)(width)-(valueCount)-offsetColumn); 
			return valueToRect(rectangleTopLeft, width, height, offsetLine, offsetX, LEFT, values, valueCount, repeat);
		}
		else if(offsetLine < (int32_t)height) 
		{
			int32_t linearCellOffset	= (int32_t)(offsetLine*width+offsetColumn);
			int32_t remainingColumns	= (int32_t)width-offsetColumn;
			int32_t maxPrintSize		= (valueCount < remainingColumns) ? valueCount : remainingColumns;
			memcpy(&rectangleTopLeft[linearCellOffset], values, maxPrintSize*sizeof(_TCell));
			if(repeat > 0)
				valueToRect( rectangleTopLeft, width, height, offsetLine, offsetColumn+valueCount, align, values, valueCount, --repeat );
		}

		return offsetColumn;
	}


#pragma pack(pop)
}

#endif // __MISC_H__982143092634987160987328097642398__