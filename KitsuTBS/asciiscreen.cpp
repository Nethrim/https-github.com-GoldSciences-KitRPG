#include "asciiscreen.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <Windows.h>

static char* _backBuffer	= 0;
static char* _frontBuffer	= 0;

static const int _backBufferWidth	= DEFAULT_ASCII_SCREEN_WIDTH;
static const int _backBufferHeight	= DEFAULT_ASCII_SCREEN_HEIGHT;

class CGBackBufferDeleter
{
public:
	~CGBackBufferDeleter()
	{
		if( _backBuffer )	delete( _backBuffer );
		if( _frontBuffer )	delete( _frontBuffer );
	}
} bbDeleter;

char* game::getASCIIBackBuffer( void )
{
	return _backBuffer;
};

const char* game::getASCIIFrontBuffer( void )
{
	return _frontBuffer;
};

uint32_t game::getASCIIBackBufferWidth( void )
{
	return _backBufferWidth;
};

uint32_t game::getASCIIBackBufferHeight( void )
{
	return _backBufferHeight;
};

void game::swapASCIIBuffers( void )
{
	char* bB		= _backBuffer;
	char* fB		= _frontBuffer;

	_backBuffer		= fB;
	_frontBuffer	= bB;
};


void game::initASCIIScreen()
{
	static const HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_SCREEN_BUFFER_INFOEX csbiInfo = {};
	GetConsoleScreenBufferInfoEx( hConsoleOut, &csbiInfo );
	//csbiInfo.dwCursorPosition.Y = csbiInfo.dwSize.Y-1;
	csbiInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	//csbiInfo.wAttributes
	csbiInfo.dwSize.X = _backBufferWidth;
	csbiInfo.dwSize.Y = _backBufferHeight;
	csbiInfo.dwMaximumWindowSize.X	= _backBufferWidth;
	csbiInfo.dwMaximumWindowSize.Y	= _backBufferHeight;
	csbiInfo.bFullscreenSupported	= TRUE;
	csbiInfo.srWindow.Top		= 10;
	csbiInfo.srWindow.Left		= 10;
	csbiInfo.srWindow.Right		= 800;
	csbiInfo.srWindow.Bottom	= 600;
	csbiInfo.ColorTable[0]		= 0x00000000;
	csbiInfo.ColorTable[1]		= 0x00FFFFFF;
	csbiInfo.ColorTable[2]		= 0x00FF0000;
	csbiInfo.ColorTable[3]		= 0x0000FF00;
	csbiInfo.ColorTable[4]		= 0x000000FF;
	csbiInfo.ColorTable[5]		= 0x00FFFFFF;//0x0000FFFF;
	csbiInfo.ColorTable[6]		= 0x00FF00FF;
	csbiInfo.ColorTable[7]		= 0x00FFFF00;
	csbiInfo.ColorTable[8]		= 0x00000FFF;
	csbiInfo.ColorTable[9]		= 0x0000FFF0;
	csbiInfo.ColorTable[10]		= 0x000FFF00;
	csbiInfo.ColorTable[11]		= 0x00FFF000;
	csbiInfo.ColorTable[12]		= 0x00FF000F;
	csbiInfo.ColorTable[13]		= 0x00F000FF;
	csbiInfo.ColorTable[14]		= 0x0000F0F0;
	csbiInfo.ColorTable[15]		= 0x00FFFFFF;//0x00F000F0;
	csbiInfo.wAttributes		= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

	SetConsoleScreenBufferInfoEx( hConsoleOut, &csbiInfo );
	game::clearASCIIBackBuffer(' ');
	game::presentASCIIBackBuffer();
	game::clearASCIIBackBuffer(' ');
	game::presentASCIIBackBuffer();
}


void game::clearASCIIBackBuffer( int value )
{
	if( 0 == _backBuffer )
	{
		static const HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
		CONSOLE_SCREEN_BUFFER_INFOEX csbiInfo = {};
		GetConsoleScreenBufferInfoEx( hConsoleOut, &csbiInfo );
		csbiInfo.dwCursorPosition.Y = csbiInfo.dwSize.Y-1;
		//csbiInfo.dwCursorPosition.X = 0;
		SetConsoleScreenBufferInfoEx( hConsoleOut, &csbiInfo );
		DWORD dummy=0;
		COORD Home = { 0, 0 };
		FillConsoleOutputCharacter( hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy );
		//csbiInfo.dwSize.X = 
		_backBuffer = (char*)malloc( sizeof( char )*_backBufferWidth*_backBufferHeight+1 );	// We're going to draw our map in this array. the assignment of empty brackets = {} initializes all chars in the array to 0
		if( 0 == _backBuffer )
			throw("damn");
	}
	if( _backBuffer)
	{
		memset(_backBuffer, value, sizeof( char )*_backBufferWidth*_backBufferHeight+1);

		static const HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
		CONSOLE_SCREEN_BUFFER_INFOEX csbiInfo = {};
		GetConsoleScreenBufferInfoEx( hConsoleOut, &csbiInfo );
		csbiInfo.dwCursorPosition.Y = csbiInfo.dwSize.Y-1;
		//csbiInfo.dwCursorPosition.Y = 0;
		//csbiInfo.dwCursorPosition.X = 0;
		//csbiInfo.dwCursorPosition.X = 0;
		SetConsoleScreenBufferInfoEx( hConsoleOut, &csbiInfo );
	}

};

void game::presentASCIIFrontBuffer( void )
{
#if defined( WIN32 )
	static const HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo = {};
    GetConsoleScreenBufferInfo( hConsoleOut, &csbiInfo );
	COORD   Coords = {0, csbiInfo.dwSize.Y-_backBufferHeight};
	char* myLine=0;
	DWORD dummy=0;

    COORD    Home = { 0, 0 };
	if( _frontBuffer )
		WriteConsoleOutputCharacterA( hConsoleOut, _frontBuffer, _backBufferWidth*_backBufferHeight, Coords, &dummy );
#else
	if( _frontBuffer )
	{
		_frontBuffer[(_backBufferWidth*_backBufferHeight)] = 0;
		fprintf(stdout, "\n%s", _frontBuffer );
	}
#endif
};

void game::presentASCIIBackBuffer( void )
{
	swapASCIIBuffers();
	presentASCIIFrontBuffer();
};

