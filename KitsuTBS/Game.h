#include "Character.h"
#include "menus.h"

#ifndef __GAME_H__91827309126391263192312312354__
#define __GAME_H__91827309126391263192312312354__

struct STacticalMap
{

#define TACTICAL_MAP_HEIGHT	32
#define TACTICAL_MAP_WIDTH	32

	const char	Tiles[TACTICAL_MAP_HEIGHT][TACTICAL_MAP_WIDTH] = {};
};


struct SGame
{
	bool				bRunning			= true;

	GAME_STATE			CurrentMenu			= GAME_MENU_MAIN;

	std::string			UserMessage			= "";

	klib::CCharacter	PlayerSquad	[32]	= {};
	klib::CCharacter	EnemySquad	[32]	= {};

	STacticalMap		TacticalMap			= {};

	SInput				FrameInput			= {};
};



#endif // __GAME_H__91827309126391263192312312354__