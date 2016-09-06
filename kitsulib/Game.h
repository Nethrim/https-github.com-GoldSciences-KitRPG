#include "Character.h"
#include "Tile.h"
#include "menus.h"

#ifndef __GAME_H__91827309126391263192312312354__
#define __GAME_H__91827309126391263192312312354__

namespace klib
{

	template<size_t _Width, size_t _Depth>
	struct SWeightedDisplay {
		SGrid<char		, _Width, _Depth>	Screen			= {};
		SGrid<uint16_t	, _Width, _Depth>	TextAttributes	= {};
		SGrid<float		, _Width, _Depth>	DisplayWeights	= {};
		SGrid<float		, _Width, _Depth>	Speed			= {};
		SGrid<float		, _Width, _Depth>	SpeedTarget		= {};

		static const uint32_t		Width			= (uint32_t)_Width;
		static const uint32_t		Depth			= (uint32_t)_Depth;

		void						Clear()
		{
			clearGrid(Screen			, ' ');
			clearGrid(TextAttributes	, (uint16_t)1);
			clearGrid(DisplayWeights	, 0.0f);
			clearGrid(Speed				, 0.0f);
			clearGrid(SpeedTarget		, 0.0f);
		};
	};

#define GAME_MAP_DEPTH	48
#define GAME_MAP_WIDTH	(GAME_MAP_DEPTH*2)

	typedef SGameTiles<GAME_MAP_WIDTH, GAME_MAP_DEPTH>									STacticalBoard		;
	typedef	SWeightedDisplay<DEFAULT_ASCII_SCREEN_WIDTH, DEFAULT_ASCII_SCREEN_HEIGHT>	SGlobalDisplay		;
	typedef SWeightedDisplay<STacticalBoard::Width, STacticalBoard::Depth>				STacticalDisplay	;
	typedef SWeightedDisplay<STacticalBoard::Width, STacticalBoard::Depth>				SPostEffectDisplay	;
	typedef SGrid<char, SGlobalDisplay::Width, SGlobalDisplay::Depth>					SMenuDisplay		;

	// Squads should be indices to the army
	struct SGameSquad {
#define MAX_AGENT_SLOTS 16
		static const int32_t		Size = MAX_AGENT_SLOTS;

		int16_t						Agents[MAX_AGENT_SLOTS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	
		inline void					Clear(int32_t index) {
			if(index == -1)
				memset(Agents, -1, sizeof(int16_t)*klib::size(Agents));
			else
				Agents[index] = -1;
		};
	};

	struct SPlayerSelection	// can be AI or human.
	{
		int16_t						SelectedPlayerSquad	;
		int16_t						SelectedPlayerUnit	;
		int16_t						SelectedTargetSquad	;
		int16_t						SelectedTargetUnit	;
	};

	struct SPlayer	// can be AI or human.
	{
		int32_t						PlayerMoney			= 0;
		SCharacterResearch			CompletedResearch	= {};
		SCharacterEquip				MaxResearch			= {};
		SCharacterInventory			Inventory			= {};
		SGameSquad					Squad				= SGameSquad();
		std::vector<CCharacter>		Army				= {};

		SPlayerSelection			Selection			= {-1, -1, -1, -1};
		std::string					Name				= "Anonymous";
	};

	struct SGame
	{
		bool						bRunning			= true;
		bool						bStarted			= false;

		SGameState					State				= {GAME_STATE_MENU_MAIN,};
		SGameState					PreviousState		= {GAME_STATE_MENU_MAIN,};

		SPlayer						Player, Enemy;

		STacticalBoard				TacticalBoard		= {};

		std::string					UserMessage			= "";

		STacticalDisplay			TacticalDisplay		= {};
		SPostEffectDisplay			PostEffectDisplay	= {};
		SGlobalDisplay				GlobalDisplay		= {};
		SMenuDisplay				MenuDisplay			= {};

		SInput						FrameInput			= {};
		STimer						FrameTimer			= {};

		char						SlowMessage[256]	= {'_',};

		int16_t						Seed				= 0;
	};

	void initGame(SGame& instanceGame);
	void resetGame(SGame& instanceGame);
	void showMenu(SGame& instanceGame);
	void initTacticalMap(SGame& instanceGame);


} // namespace 

#endif // __GAME_H__91827309126391263192312312354__