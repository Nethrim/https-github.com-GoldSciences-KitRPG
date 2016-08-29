#include <cstdint>

#include <algorithm>


#ifndef __ASCIISCREEN_H__20983472903740928374234__
#define __ASCIISCREEN_H__20983472903740928374234__

#define DEFAULT_ASCII_SCREEN_HEIGHT	80
#define DEFAULT_ASCII_SCREEN_WIDTH	DEFAULT_ASCII_SCREEN_HEIGHT*2

namespace game
{
	void initASCIIScreen();

	char* getASCIIBackBuffer( void );
	const char* getASCIIFrontBuffer( void );

	uint32_t getASCIIBackBufferWidth( void );
	uint32_t getASCIIBackBufferHeight( void );

	int32_t setASCIIBackBufferWidth( uint32_t backBufferWidth );
	int32_t setASCIIBackBufferHeight( uint32_t backBufferHeight );

	void swapASCIIBuffers( void );
	void clearASCIIBackBuffer( int value );
	void presentASCIIFrontBuffer( void );
	void presentASCIIBackBuffer( void );

	enum ALIGN
	{	LEFT	= 0x1
	,	RIGHT	= 0x2
	,	CENTER	= 0x3
	,	TOP		= 0x4
	,	BOTTOM	= 0x8
	,	VCENTER	= TOP | BOTTOM
	};

	template <typename... _Args>
	void lineToScreen( int32_t offsetLine, int32_t offsetColumn, ALIGN bCenter, const char* format, _Args... args )
	{
		char precookStr[1024] = {};
		uint32_t precookLen = sprintf_s(precookStr, format, args...);
		if( 0 == precookLen )
			return;

		if(bCenter == CENTER)
		{
			int32_t offsetX = (game::getASCIIBackBufferWidth()>>1)-(precookLen>>1)+offsetColumn;
			lineToScreen(offsetLine, offsetX, game::LEFT, precookStr);
		}
		else if(bCenter == RIGHT)
		{
			int32_t offsetX = (game::getASCIIBackBufferWidth())-(precookLen)-offsetColumn;
			lineToScreen(offsetLine, offsetX, game::LEFT, precookStr);
		}
		else
		{
			char* screen = game::getASCIIBackBuffer();
			int32_t cursorOffset = offsetLine*game::getASCIIBackBufferWidth()+offsetColumn;
			int32_t maxPrintSize = std::min(precookLen, getASCIIBackBufferWidth()-offsetColumn);
			memcpy(&screen[cursorOffset], precookStr, precookLen);
		}
	}
};

#endif // __ASCIISCREEN_H__20983472903740928374234__
