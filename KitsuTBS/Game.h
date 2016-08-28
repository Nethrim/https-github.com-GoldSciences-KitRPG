#include "Character.h"
#include "menus.h"

#ifndef __GAME_H__91827309126391263192312312354__
#define __GAME_H__91827309126391263192312312354__

#define TACTICAL_DISPLAY_DEPTH	48
#define TACTICAL_DISPLAY_WIDTH	TACTICAL_DISPLAY_DEPTH*2

struct STacticalDisplay
{
	char						Screen			[TACTICAL_DISPLAY_DEPTH][TACTICAL_DISPLAY_WIDTH] = {};
	float						DisplayWeights	[TACTICAL_DISPLAY_DEPTH][TACTICAL_DISPLAY_WIDTH] = {};
	float						Speed			[TACTICAL_DISPLAY_DEPTH][TACTICAL_DISPLAY_WIDTH] = {};
	float						SpeedTarget		[TACTICAL_DISPLAY_DEPTH][TACTICAL_DISPLAY_WIDTH] = {};
	char						TargetX			[TACTICAL_DISPLAY_DEPTH][TACTICAL_DISPLAY_WIDTH] = {};
};

struct SGame
{
	bool						bRunning				= true;

	GAME_STATE					CurrentMenu				= GAME_MENU_MAIN;

	std::string					UserMessage				= "";

	klib::SCharacterInventory	PlayerInventory			= {};
	klib::SCharacterInventory	EnemyInventory			= {};

	klib::CCharacter			PlayerArmy		[512]	= {};
	klib::CCharacter			EnemyArmy		[512]	= {};
	klib::CCharacter			PlayerSquad		[32]	= {};
	klib::CCharacter			EnemySquad		[32]	= {};

	STacticalDisplay					TacticalMap				= {};

	SInput						FrameInput				= {};
	STimer						FrameTimer				= {};

	int16_t						Seed					= 0;


};


void drawState( SGame& instanceGame );

#endif // __GAME_H__91827309126391263192312312354__