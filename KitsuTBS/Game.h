#include "Character.h"
#include "menus.h"

#ifndef __GAME_H__91827309126391263192312312354__
#define __GAME_H__91827309126391263192312312354__


template<size_t _Width, size_t _Depth>
struct STacticalDisplay
{
	static const uint32_t		Width			= (uint32_t)_Width;
	static const uint32_t		Depth			= (uint32_t)_Depth;

	char						Screen			[_Depth][_Width]= {};
	float						DisplayWeights	[_Depth][_Width]= {};
	float						Speed			[_Depth][_Width]= {};
	float						SpeedTarget		[_Depth][_Width]= {};
};


template<size_t _Width, size_t _Height>
void fillDisplayValueFromNoise( STacticalDisplay<_Width, _Height>& display, char value, int32_t seed, char clearValue = ' ') {
	for(uint32_t z=0, maxZ=_Height; z<maxZ; ++z)
		for(uint32_t x=0, maxX=_Width; x<maxX; ++x)
			display.Screen[z][x] = (int(noise2D(x, z, _Width, seed)*12)) ? clearValue : value;
}

template<size_t _Width, size_t _Height>
void clearDisplay( STacticalDisplay<_Width, _Height>& display, char clearValue = ' ') {
	memset(display.Screen, clearValue, _Height*_Width);
}

template<size_t _Width, size_t _Height>
void drawDisplayBorders( STacticalDisplay<_Width, _Height>& display, char value)
{
	for(uint32_t z=0, maxZ=_Height; z<maxZ; ++z){ display.Screen[z][0] = value; display.Screen[z][_Width-1] = value; }
	for(uint32_t x=0, maxX=_Width; x<maxX; ++x){ display.Screen[0][x] = value; display.Screen[_Height-1][x] = value; }
}


struct SGame
{
	bool						bRunning				= true;

	GAME_STATE					CurrentMenu				= GAME_MENU_MAIN;

	std::string					UserMessage				= "";

	klib::SCharacterInventory	PlayerInventory			= {};
	klib::SCharacterInventory	EnemyInventory			= {};

	int32_t						SelectedPlayerUnit		= 0;
	int32_t						SelectedTargetUnit		= 0;

	std::vector<klib::CCharacter>	PlayerArmy	= {};
	std::vector<klib::CCharacter>	EnemyArmy	= {};
	std::vector<klib::CCharacter>	PlayerSquad = {};
	std::vector<klib::CCharacter>	EnemySquad	= {};

#define TACTICAL_DISPLAY_DEPTH	48
#define TACTICAL_DISPLAY_WIDTH	TACTICAL_DISPLAY_DEPTH*2

	STacticalDisplay<TACTICAL_DISPLAY_WIDTH, TACTICAL_DISPLAY_DEPTH>			TacticalDisplay	= {};
	STacticalDisplay<DEFAULT_ASCII_SCREEN_WIDTH, DEFAULT_ASCII_SCREEN_HEIGHT>	GlobalDisplay	= {};

	SInput						FrameInput				= {};
	STimer						FrameTimer				= {};

	int16_t						Seed					= 0;
};


void drawState( SGame& instanceGame );
void showMenu(SGame& instanceGame);



static double noise1D(uint32_t x, uint64_t Seed=15731) {
	x = (x<<13) ^ x;
	return ( 1.0 - ( (x * (x * x * Seed + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

static double noise2D(uint32_t x, uint32_t y, uint32_t nWidth, uint64_t Seed=15731 ) {
	//uint32_t n = x + (y * xWidth);
	x += (y * nWidth);
	x = (x<<13) ^ x;
	return ( 1.0 - ( (x * (x * x * Seed + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

static double noise3D( uint32_t x, uint32_t y, uint32_t z, uint32_t nWidth, uint32_t nHeight, uint64_t Seed=15731 ) {
	uint32_t n = x+(y*nWidth+(z*nHeight*nWidth));
	n = (n<<13) ^ n;
	return ( 1.0 - ( (n*(n * n * Seed + 789221) + 1376312589 ) & 0x7fffffff) / 1073741824.0);    
	//return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

#endif // __GAME_H__91827309126391263192312312354__