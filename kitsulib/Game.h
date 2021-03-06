#include "Player.h"
#include "Tile.h"
#include "menus.h"

#define GODS_MTSUPPORT
#include "GCore_mutex.h"

#include <time.h>
#include <vector>

#ifndef __GAME_H__91827309126391263192312312354__
#define __GAME_H__91827309126391263192312312354__

namespace klib
{

#define GAME_MAP_DEPTH	36
#define GAME_MAP_WIDTH	(GAME_MAP_DEPTH*2)

	typedef SGameTiles<GAME_MAP_WIDTH, GAME_MAP_DEPTH>		STacticalBoard;
	
	struct STacticalInfo
	{
		STacticalBoard	Board;

		void Clear()
		{
			Board.Clear();
		}
	};

	template<size_t _Width, size_t _Depth>	struct SGridConsoleAttribute	: public SGrid<uint16_t	, _Width, _Depth>	{};
	template<size_t _Width, size_t _Depth>	struct SGridConsoleText			: public SGrid<char		, _Width, _Depth>	{};
	

	template<size_t _Width, size_t _Depth>
	struct SWeightedDisplay {
		SGridConsoleText		<_Width, _Depth>	Screen			= {};
		SGridConsoleAttribute	<_Width, _Depth>	TextAttributes	= {};
		SGrid<float		, _Width, _Depth>			DisplayWeights	= {};
		SGrid<float		, _Width, _Depth>			Speed			= {};
		SGrid<float		, _Width, _Depth>			SpeedTarget		= {};

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

	typedef	SWeightedDisplay<DEFAULT_ASCII_SCREEN_WIDTH, DEFAULT_ASCII_SCREEN_HEIGHT>	SGlobalDisplay		;
	typedef SWeightedDisplay<STacticalBoard::Width, STacticalBoard::Depth>				STacticalDisplay	;
	typedef SWeightedDisplay<STacticalBoard::Width, STacticalBoard::Depth>				SPostEffectDisplay	;
	typedef SGrid<char, SGlobalDisplay::Width, SGlobalDisplay::Depth>					SMenuDisplay		;

	enum PLAYER_INDEX	: uint8_t
	{
		PLAYER_USER		= 0,
		PLAYER_ENEMY	= 1,
		PLAYER_NEUTRAL	= 2,
		PLAYER_ALLY		= 3,

		PLAYER_REMOTE_0	= 4,
		PLAYER_REMOTE_1	= 5,
		PLAYER_REMOTE_2	= 6,
		PLAYER_REMOTE_3	= 7,
	}; 

	GDEFINE_ENUM_TYPE(GAME_FLAGS, uint8_t);
	GDEFINE_ENUM_VALUE(GAME_FLAGS, 0x01, RUNNING			);
	GDEFINE_ENUM_VALUE(GAME_FLAGS, 0x02, STARTED			);
	GDEFINE_ENUM_VALUE(GAME_FLAGS, 0x04, CONNECTED			);
	GDEFINE_ENUM_VALUE(GAME_FLAGS, 0x08, NETWORK_ENABLED	);
	GDEFINE_ENUM_VALUE(GAME_FLAGS, 0x10, TACTICAL			);
	GDEFINE_ENUM_VALUE(GAME_FLAGS, 0x20, TURN_BUSY			);

	//
	struct SFrameInfo
	{
		ktools::SInput	Input	= {};
		ktools::STimer	Timer	= {};
	};

	//
	struct SGame
	{
		// Game Flags tell us about the current state of the game.
		GAME_FLAGS					Flags				= (GAME_FLAGS)(GAME_FLAGS_NETWORK_ENABLED | GAME_FLAGS_TURN_BUSY);

		SGameState					State				= {GAME_STATE_MENU_MAIN,};
		SGameState					PreviousState		= {GAME_STATE_MENU_MAIN,};

		uint64_t					ServerTime			= 0;

		PLAYER_INDEX				CurrentPlayer		= (PLAYER_INDEX)-1;
		SPlayer						Players[8]			= {};
		PLAYER_ANTAGONISM			Antagonisms[8]		= {PLAYER_ANTAGONISM_USER, PLAYER_ANTAGONISM_ENEMY, PLAYER_ANTAGONISM_NEUTRAL, PLAYER_ANTAGONISM_ALLY, 
			PLAYER_ANTAGONISM_ENEMY, PLAYER_ANTAGONISM_ENEMY, PLAYER_ANTAGONISM_ENEMY, PLAYER_ANTAGONISM_ENEMY};

		ktools::SInput				FrameInput			= {};
		ktools::STimer				FrameTimer			= {};

		int64_t						Seed				= 0;

		// Tactical board.
		STacticalInfo				TacticalInfo		= {};

		// Displays.
		STacticalDisplay			TacticalDisplay		= {};
		SPostEffectDisplay			PostEffectDisplay	= {};
		SGlobalDisplay				GlobalDisplay		= {};
		SMenuDisplay				MenuDisplay			= {};

		// Feedback messages.
		std::string					StateMessage		= "";
		std::string					UserMessage			= "";
		std::string					UserError			= "";

		// For the special effect
		char						SlowMessage[256]	= {'_',};

		god::CGMutex				PlayerMutex			= {};
		god::CGMutex				ServerTimeMutex		= {};

		void ClearDisplays()
		{
			TacticalDisplay		.Clear();	
			GlobalDisplay		.Clear();	
			PostEffectDisplay	.Clear();	
			clearGrid(MenuDisplay);
		}
	};

	void initGame(SGame& instanceGame);
	void resetGame(SGame& instanceGame);
	void showMenu(SGame& instanceGame);
	void initTacticalMap(SGame& instanceGame);
	void drawTacticalMap(SGame& instanceGame, SPostEffectDisplay& target);


} // namespace 

#endif // __GAME_H__91827309126391263192312312354__