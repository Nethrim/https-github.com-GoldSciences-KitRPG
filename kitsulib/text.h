#include "misc.h"

#include <string>

#ifndef __TEXT_H__923649827346982746982346298734623__
#define __TEXT_H__923649827346982746982346298734623__

namespace klib
{
	template<typename... _Args>
	int32_t lineToRect( char* rectangleTopLeft, size_t width, size_t height, int32_t offsetLine, int32_t offsetColumn, ALIGN align, const char* text, int32_t charCount = -1 )
	{
		int32_t textLen = (int32_t)strlen(text);
		if( textLen > charCount && charCount >= 0 )
			textLen = charCount;

		return valueToRect(rectangleTopLeft, width, height, offsetLine, offsetColumn, align, text, textLen);
	}

	template<typename... _Args>
	int32_t printfToRect( char* rectangleTopLeft, size_t width, size_t height, int32_t offsetLine, int32_t offsetColumn, ALIGN align, const char* format, _Args... args )
	{
		char precookStr[1024] = {};
		int32_t precookLen = sprintf_s(precookStr, format, args...);
		return valueToRect(rectangleTopLeft, width, height, offsetLine, offsetColumn, align, precookStr, precookLen);
	}

	// returns true if done printing all the text.
	template <size_t _Size> 
	bool getMessageSlow(char (&message)[_Size], const char* textToPrint, size_t sizeToPrint, double lastFrameSeconds)
	{
		static float nextTick=0.0f;
		static uint32_t tickCount=0;

		int32_t mesLen = (int32_t)strlen(message);
		mesLen = ((mesLen+1) > (_Size-1)) ? _Size-2 : mesLen;

		nextTick	+= (float)lastFrameSeconds;
		if(nextTick > 0.025f)
		{
			tickCount++;
			int32_t cursorIndex = mesLen-1;
			if(cursorIndex < (int32_t)sizeToPrint)
			{
				message[cursorIndex]	= textToPrint[cursorIndex];
				message[mesLen]			= '_';
				message[cursorIndex+2]	= 0;
				nextTick = 0.0f;
			}
			else if(0 == (tickCount % 20))
				message[cursorIndex] = (message[cursorIndex] == ' ') ? '_' : ' ';
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
