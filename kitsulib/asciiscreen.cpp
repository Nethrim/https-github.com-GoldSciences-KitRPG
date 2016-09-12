#include "asciiscreen.h"
#include "color.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <Windows.h>

static char* _backBuffer			= 0;
static char* _frontBuffer			= 0;
static uint16_t* _colorBackBuffer	= 0;
static uint16_t* _colorFrontBuffer	= 0;

static const int _backBufferWidth	= DEFAULT_ASCII_SCREEN_WIDTH;
static const int _backBufferHeight	= DEFAULT_ASCII_SCREEN_HEIGHT;

class CGBackBufferDeleter
{
public:
	~CGBackBufferDeleter()
	{
		if( _backBuffer )	delete( _backBuffer );
		if( _frontBuffer )	delete( _frontBuffer );
		if( _colorBackBuffer	)	delete( _colorBackBuffer	);
		if( _colorFrontBuffer	)	delete( _colorFrontBuffer	);
	}
} bbDeleter;

char* klib::getASCIIBackBuffer( void ) {
	if( 0 == _backBuffer )
		clearASCIIBackBuffer(' ', COLOR_WHITE);
	return _backBuffer;
};

const char* klib::getASCIIFrontBuffer( void ) {
	return _frontBuffer;
};

uint16_t* klib::getASCIIColorBackBuffer( void ) {
	return _colorBackBuffer;
};

const uint16_t* klib::getASCIIColorFrontBuffer( void ) {
	return _colorFrontBuffer;
};

uint32_t klib::getASCIIBackBufferWidth( void ) {
	return _backBufferWidth;
};

uint32_t klib::getASCIIBackBufferHeight( void ) {
	return _backBufferHeight;
};

void klib::swapASCIIBuffers( void ) {
	char* bB			= _backBuffer;
	_backBuffer			= _frontBuffer;
	_frontBuffer		= bB;

	uint16_t* cBB		= _colorBackBuffer;
	_colorBackBuffer	= _colorFrontBuffer;
	_colorFrontBuffer	= cBB;
};


void klib::presentASCIIBackBuffer( void )
{
	swapASCIIBuffers();
	presentASCIIFrontBuffer();
};

void klib::presentASCIIFrontBuffer( void )
{
#if defined( WIN32 )
	const HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo = {};
    GetConsoleScreenBufferInfo( hConsoleOut, &csbiInfo );
	COORD   Coords = {0, csbiInfo.dwSize.Y-_backBufferHeight};
	char* myLine=0;
	DWORD dummy=0;

    COORD    Home = { 0, 0 };
	//static WORD curColor = 0;
	if( _frontBuffer ) {
		WriteConsoleOutputAttribute( hConsoleOut, _colorFrontBuffer, _backBufferWidth*_backBufferHeight, Coords, &dummy );
		WriteConsoleOutputCharacterA( hConsoleOut, _frontBuffer, _backBufferWidth*_backBufferHeight, Coords, &dummy );
	}
#else
	if( _frontBuffer )
	{
		_frontBuffer[(_backBufferWidth*_backBufferHeight)] = 0;
		fprintf(stdout, "\n%s", _frontBuffer );
	}
#endif
};

void klib::clearASCIIBackBuffer( int value, uint16_t colorValue )
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
		_colorBackBuffer = (uint16_t*)malloc( sizeof( uint16_t )*_backBufferWidth*_backBufferHeight+1 );	
	}

	if( _backBuffer)
		memset(_backBuffer, value, sizeof( char )*_backBufferWidth*_backBufferHeight+1);

	if(_colorBackBuffer)
		memset(_colorBackBuffer, colorValue, sizeof( uint16_t )*_backBufferWidth*_backBufferHeight+1);

};

void klib::initASCIIScreen()
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
	csbiInfo.ColorTable[0 ]		= COLOR_0 ;
	csbiInfo.ColorTable[1 ]		= COLOR_1 ;
	csbiInfo.ColorTable[2 ]		= COLOR_2 ;
	csbiInfo.ColorTable[3 ]		= COLOR_3 ;
	csbiInfo.ColorTable[4 ]		= COLOR_4 ;
	csbiInfo.ColorTable[5 ]		= COLOR_5 ;
	csbiInfo.ColorTable[6 ]		= COLOR_6 ;
	csbiInfo.ColorTable[7 ]		= COLOR_7 ;
	csbiInfo.ColorTable[8 ]		= COLOR_8 ;
	csbiInfo.ColorTable[9 ]		= COLOR_9 ;
	csbiInfo.ColorTable[10]		= COLOR_10;
	csbiInfo.ColorTable[11]		= COLOR_11;
	csbiInfo.ColorTable[12]		= COLOR_12;
	csbiInfo.ColorTable[13]		= COLOR_13;
	csbiInfo.ColorTable[14]		= COLOR_14;
	csbiInfo.ColorTable[15]		= COLOR_15;
	csbiInfo.wAttributes		= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;

	SetConsoleScreenBufferInfoEx( hConsoleOut, &csbiInfo );
	//klib::clearASCIIBackBuffer(' ');
	//klib::presentASCIIBackBuffer();
	//klib::clearASCIIBackBuffer(' ');
	//klib::presentASCIIBackBuffer();
}
