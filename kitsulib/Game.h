#include "Character.h"
#include "Tile.h"
#include "menus.h"

#ifndef __GAME_H__91827309126391263192312312354__
#define __GAME_H__91827309126391263192312312354__

namespace klib
{

	template<size_t _Width, size_t _Depth>
	struct SWeightedDisplay {
		SGrid<char ,  _Width, _Depth>	Screen			= {};
		SGrid<float, _Width, _Depth>	DisplayWeights	= {};
		SGrid<float, _Width, _Depth>	Speed			= {};
		SGrid<float, _Width, _Depth>	SpeedTarget		= {};

		static const uint32_t			Width			= (uint32_t)_Width;
		static const uint32_t			Depth			= (uint32_t)_Depth;

		void							Clear()
		{
			clearGrid(Screen			, ' ');
			clearGrid(DisplayWeights	, 0.0f);
			clearGrid(Speed				, 0.0f);
			clearGrid(SpeedTarget		, 0.0f);
		};
	};

#define GAME_MAP_DEPTH	48
#define GAME_MAP_WIDTH	(GAME_MAP_DEPTH*2)

	typedef SGameTiles<GAME_MAP_WIDTH, GAME_MAP_DEPTH>									STacticalTiles		;
	typedef SWeightedDisplay<STacticalTiles::Width, STacticalTiles::Depth>				STacticalDisplay	;
	typedef	SWeightedDisplay<DEFAULT_ASCII_SCREEN_WIDTH, DEFAULT_ASCII_SCREEN_HEIGHT>	SGlobalDisplay		;
	typedef SWeightedDisplay<STacticalTiles::Width, STacticalTiles::Depth>				SPostEffectDisplay	;
	typedef SGrid<char, SGlobalDisplay::Width, SGlobalDisplay::Depth>					SMenuDisplay		;

	// Squads should be indices to the army
	struct SGameSquad {
#define MAX_AGENT_SLOTS 16
		static const int32_t	Size = MAX_AGENT_SLOTS;

		int16_t					Agents[MAX_AGENT_SLOTS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	
		inline void				Clear(int32_t index) {
			if(index == -1)
				memset(Agents, -1, sizeof(int16_t)*klib::size(Agents));
			else
				Agents[index] = -1;
		};
	};

	struct SGameArmy {
		std::vector<klib::CCharacter>	Player;
		std::vector<klib::CCharacter>	Enemy;
	};

	struct SGame
	{
		bool									bRunning			= true;
		bool									bStarted			= false;

		SGameState								State				= {GAME_STATE_MENU_MAIN,};
		SGameState								PreviousState		= {GAME_STATE_MENU_MAIN,};

		std::string								UserMessage			= "";

		klib::SCharacterEquip					PlayerMaxResearch	= {};
		klib::SCharacterEquip					EnemyMaxResearch	= {};
		klib::SCharacterInventory				PlayerInventory		= {};
		klib::SCharacterInventory				EnemyInventory		= {};

		int32_t									SelectedPlayerSquad	= -1;
		int32_t									SelectedPlayerUnit	= -1;
		int32_t									SelectedTargetSquad	= -1;
		int32_t									SelectedTargetUnit	= -1;

		klib::SCharacterResearch				PlayerResearch		= {};

		SGameArmy								GameArmies			= {};
		SGameSquad								PlayerSquads		= SGameSquad();
		SGameSquad								EnemySquads			= SGameSquad();;

		STacticalTiles							TacticalTiles		= {};

		STacticalDisplay						TacticalDisplay		= {};
		SPostEffectDisplay						PostEffectDisplay	= {};
		SGlobalDisplay							GlobalDisplay		= {};
		SMenuDisplay							MenuDisplay			= {};

		SInput									FrameInput			= {};
		STimer									FrameTimer			= {};

		char									SlowMessage[256]	= {'_',};

		int16_t									Seed				= 0;

		std::string								PlayerName			= "Anonymous";
	};

	void initGame(SGame& instanceGame);
	void resetGame(SGame& instanceGame);
	void showMenu(SGame& instanceGame);
	void initTacticalMap(SGame& instanceGame);

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

} // namespace 

#endif // __GAME_H__91827309126391263192312312354__