#include "text.h"

#include <algorithm>

#ifndef __ASCIISCREEN_H__20983472903740928374234__
#define __ASCIISCREEN_H__20983472903740928374234__

#define DEFAULT_ASCII_SCREEN_HEIGHT	80
#define DEFAULT_ASCII_SCREEN_WIDTH	((uint32_t)(DEFAULT_ASCII_SCREEN_HEIGHT*2.6f))

namespace klib
{
	void initASCIIScreen();

	char* getASCIIBackBuffer( void );
	const char* getASCIIFrontBuffer( void );

	uint16_t* getASCIIColorBackBuffer( void );
	const uint16_t* getASCIIColorFrontBuffer( void );

	uint32_t getASCIIBackBufferWidth( void );
	uint32_t getASCIIBackBufferHeight( void );

	//int32_t setASCIIBackBufferWidth( uint32_t backBufferWidth );
	//int32_t setASCIIBackBufferHeight( uint32_t backBufferHeight );

	void swapASCIIBuffers( void );
	void clearASCIIBackBuffer( int value, uint16_t colorValue=0x1 );
	void presentASCIIFrontBuffer( void );
	void presentASCIIBackBuffer( void );

	template <typename... _Args>
	void lineToScreen( int32_t offsetLine, int32_t offsetColumn, ALIGN align, const char* format, _Args... args ) {
		lineToRect(getASCIIBackBuffer(), (size_t)getASCIIBackBufferWidth(), (size_t)getASCIIBackBufferHeight(), offsetLine, offsetColumn, align, format, args... );
	}
};

#endif // __ASCIISCREEN_H__20983472903740928374234__
