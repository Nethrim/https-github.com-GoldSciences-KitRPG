#include "align.h"

#include <string>

#ifndef __TEXT_H__923649827346982746982346298734623__
#define __TEXT_H__923649827346982746982346298734623__

namespace klib
{
	template<typename _TCell, typename... _Args>
	int32_t lineToRect( _TCell* rectangle, size_t width, size_t height, int32_t offsetLine, int32_t offsetColumn, ALIGN align, const char* format, _Args... args )
	{
		char precookStr[1024] = {};
		int32_t precookLen = sprintf_s(precookStr, format, args...);
		//if( 0 == precookLen )
		//	return 0;

		if(align == CENTER) {
			int32_t offsetX = (int32_t)(width>>1)-(precookLen>>1)+offsetColumn;
			return lineToRect(rectangle, width, height, offsetLine, offsetX, LEFT, precookStr);
		}
		else if(align == RIGHT) {
			int32_t offsetX =(int32_t)(width)-(precookLen)-offsetColumn;
			return lineToRect(rectangle, width, height, offsetLine, offsetX, LEFT, precookStr);
		}
		else if(offsetLine < (int32_t)height) 
		{
			int32_t cursorOffset	= (int32_t)(offsetLine*width+offsetColumn);
			int32_t remainingRows	= (int32_t)width-offsetColumn;
			int32_t maxPrintSize	= (precookLen < remainingRows) ? precookLen : remainingRows;
			memcpy(&rectangle[cursorOffset], precookStr, maxPrintSize*sizeof(_TCell));
		}

		return offsetColumn;
	}

	// returns true if done printing all the text.
	template <size_t _Size> 
	bool getMessageSlow(char (&message)[_Size], const char* textToPrint, size_t sizeToPrint, double lastFrameSeconds)
	{
		static float nextTick=0.0f;
		static uint32_t tickCount=0;

		int32_t mesLen = (int32_t)strlen(message);
		nextTick	+= (float)lastFrameSeconds;

		if(nextTick > 0.025f)
		{
			tickCount++;
			if(mesLen-1 < (int32_t)sizeToPrint)
			{
				message[mesLen-1]	= textToPrint[mesLen-1];
				message[mesLen]		= '_';
				message[mesLen+1]	= 0;
				nextTick = 0.0f;
			}
			else if(0 == (tickCount % 100))
			{
				if(message[mesLen-1] == '_')
					message[mesLen-1] = ' ';
				else
					message[mesLen-1] = '_';
			}
		}

		return ( mesLen-1 == sizeToPrint );
	}

	template <size_t _Size> 
	static inline bool getMessageSlow(char (&message)[_Size], const std::string& textToPrint, double lastFrameSeconds) {
		return getMessageSlow(message, textToPrint.c_str(), textToPrint.size(), lastFrameSeconds);
	}

	template <size_t _ArraySize>
	void resetCursorString(char (&textContainer)[_ArraySize]) {
		textContainer[textContainer[1] = 0] = '_';
	}

} // namespace


#endif // __TEXT_H__923649827346982746982346298734623__
