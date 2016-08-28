#include "Game.h"

#include "menus.h"

#define NOMINMAX

#include <crtdbg.h>
#include <Windows.h>

#include <algorithm>
#include <ctime>

double noise1D(uint32_t x, uint64_t Seed=15731)
{
	x = (x<<13) ^ x;
	return ( 1.0 - ( (x * (x * x * Seed + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}


double noise2D(uint32_t x, uint32_t y, uint32_t nWidth, uint64_t Seed=15731 )
{
	//uint32_t n = x + (y * xWidth);
	x += (y * nWidth);
	x = (x<<13) ^ x;
	return ( 1.0 - ( (x * (x * x * Seed + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

double noise3D( uint32_t x, uint32_t y, uint32_t z, uint32_t nWidth, uint32_t nHeight, uint64_t Seed=15731 )
{
	uint32_t n = x+(y*nWidth+(z*nHeight*nWidth));
	n = (n<<13) ^ n;
	return ( 1.0 - ( (n*(n * n * Seed + 789221) + 1376312589 ) & 0x7fffffff) / 1073741824.0);    
	//return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}


GAME_STATE drawCredits()
{
	static double offset = (double)game::getASCIIBackBufferHeight();// (game::getASCIIBackBufferHeight()>>1)-4;
	if((offset+1) >= 0 && (offset+1) < (double)game::getASCIIBackBufferHeight()) game::lineToScreen((int32_t)(offset+1), 0, game::CENTER, "%-34.34s: %13.13s", "Programming"						, "Kitsu."		);
	if((offset+2) >= 0 && (offset+2) < (double)game::getASCIIBackBufferHeight()) game::lineToScreen((int32_t)(offset+2), 0, game::CENTER, "%-34.34s: %13.13s", "Everything else"					, "Kitsu."		);
	if((offset+3) >= 0 && (offset+3) < (double)game::getASCIIBackBufferHeight()) game::lineToScreen((int32_t)(offset+3), 0, game::CENTER, "%-34.34s: %13.13s", "Loosely based on homework code by"	, "Nethrim."	);
	if((offset+6) >= 0 && (offset+6) < (double)game::getASCIIBackBufferHeight()) game::lineToScreen((int32_t)(offset+6), 0, game::CENTER, "Press ESC to go back.");
	
	offset -= 0.01;

	if( offset <= -5 )
		offset = game::getASCIIBackBufferHeight()+5;
	
	return GAME_CREDITS;
}

void drawStage(SGame& instancedGame)	{

	int32_t offset = 5;
	for(uint32_t z=0, maxZ=TACTICAL_MAP_DEPTH; z<maxZ; ++z) {
		char row[TACTICAL_MAP_WIDTH+1] = {};
		memcpy(row, instancedGame.TacticalMap.Tiles[z], TACTICAL_MAP_WIDTH);
		game::lineToScreen(offset++, 0, game::CENTER, "%s", row	);
	}
};

void showMenu(SGame& instanceGame)	{
	
	int32_t newAction=-1;
	switch(instanceGame.CurrentMenu) {
	case	GAME_MENU_MAIN							:	instanceGame.	UserMessage = "Main Menu"				;	newAction = drawMenu(	"Main Menu"				, optionsMain			, instanceGame.FrameInput, GAME_EXIT				, "Exit game");	break;
	case	GAME_MENU_CONTROL_CENTER				:	instanceGame.	UserMessage = "Control Center"			;	newAction = drawMenu(	"Control Center"		, optionsControlCenter	, instanceGame.FrameInput, GAME_MENU_MAIN			);	break;
	case	GAME_MENU_SQUAD_SELECT_MAIN				:	instanceGame.	UserMessage = "Squad Select"			;	newAction = drawMenu(	"Squad Select"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_CONSTRUCTION_MAIN				:	instanceGame.	UserMessage = "Construction"			;	newAction = drawMenu(	"Construction"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_MAIN			);	break;
	case	GAME_MENU_EQUIP_MAIN					:	instanceGame.	UserMessage = "Equipment"				;	newAction = drawMenu(	"Equipment"				, optionsEquip			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_EQUIP_CHARACTER				:	instanceGame.	UserMessage = "Equip character"			;	newAction = drawMenu(	"Equip character"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_EQUIP_WEAPON					:	instanceGame.	UserMessage = "Equip weapon"			;	newAction = drawMenu(	"Equip weapon"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_EQUIP_ACCESSORY				:	instanceGame.	UserMessage = "Equip accessory"			;	newAction = drawMenu(	"Equip accessory"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_EQUIP_ARMOR					:	instanceGame.	UserMessage = "Equip armor"				;	newAction = drawMenu(	"Equip armor"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_EQUIP_VEHICLE					:	instanceGame.	UserMessage = "Equip vehicle"			;	newAction = drawMenu(	"Equip vehicle"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_EQUIP_FACILITY				:	instanceGame.	UserMessage = "Equip facility"			;	newAction = drawMenu(	"Equip facility"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
//	case	GAME_MENU_ACTION_MAIN					:	instanceGame.	UserMessage = "Action"					;	newAction = drawMenu(	"Action"				, optionsCombatTurn		, instanceGame.FrameInput, GAME_MENU_MAIN			);	break;
	case	GAME_MENU_SENSE_MAIN					:	instanceGame.	UserMessage = "Sense"					;	newAction = drawMenu(	"Sense"					, optionsSense			, instanceGame.FrameInput, GAME_MENU_ACTION_MAIN	);	break;
	case	GAME_MENU_RESEARCH						:	instanceGame.	UserMessage = "Research"				;	newAction = drawMenu(	"Research"				, optionsResearch		, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_RESEARCH_CHARACTER			:	instanceGame.	UserMessage = "Research profession"		;	newAction = drawMenu(	"Research profession"	, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_WEAPON				:	instanceGame.	UserMessage = "Research weapon"			;	newAction = drawMenu(	"Research weapon"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_ACCESSORY			:	instanceGame.	UserMessage = "Research accessory"		;	newAction = drawMenu(	"Research accessory"	, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_ARMOR				:	instanceGame.	UserMessage = "Research armor"			;	newAction = drawMenu(	"Research armor"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_VEHICLE				:	instanceGame.	UserMessage = "Research vehicle"		;	newAction = drawMenu(	"Research vehicle"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_FACILITY				:	instanceGame.	UserMessage = "Research facility"		;	newAction = drawMenu(	"Research facility"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_CHARACTER	:	instanceGame.	UserMessage = "Research rank"			;	newAction = drawMenu(	"Research rank"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_WEAPON		:	instanceGame.	UserMessage = "Research science"		;	newAction = drawMenu(	"Research science"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_ACCESSORY	:	instanceGame.	UserMessage = "Research technique"		;	newAction = drawMenu(	"Research technique"	, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_ARMOR		:	instanceGame.	UserMessage = "Research technology"		;	newAction = drawMenu(	"Research technology"	, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_VEHICLE		:	instanceGame.	UserMessage = "Research mechanics"		;	newAction = drawMenu(	"Research mechanics"	, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_FACILITY	:	instanceGame.	UserMessage = "Research construction"	;	newAction = drawMenu(	"Research construction"	, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		);	break;
	case	GAME_MENU_INSPECT_MAIN					:	instanceGame.	UserMessage = "Inspect"					;	newAction = drawMenu(	"Inspect"				, optionsInspect		, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_BUY_MAIN						:	instanceGame.	UserMessage = "Buy"						;	newAction = drawMenu(	"Buy"					, optionsBuy			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_BUY_ITEM						:	instanceGame.	UserMessage = "Buy items"				;	newAction = drawMenu(	"Buy items"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	//case	GAME_MENU_BUY_EQUIP_MAIN				:	instanceGame.	UserMessage = "Buy equipment"			;	newAction = drawMenu(	"Buy equipment"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_BUY_EQUIP_CHARACTER			:	instanceGame.	UserMessage = "Buy character"			;	newAction = drawMenu(	"Buy character"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_BUY_EQUIP_WEAPON				:	instanceGame.	UserMessage = "Buy weapon"				;	newAction = drawMenu(	"Buy weapon"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_BUY_EQUIP_ACCESSORY			:	instanceGame.	UserMessage = "Buy accessory"			;	newAction = drawMenu(	"Buy accessory"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_BUY_EQUIP_ARMOR				:	instanceGame.	UserMessage = "Buy armor"				;	newAction = drawMenu(	"Buy armor"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_BUY_EQUIP_VEHICLE				:	instanceGame.	UserMessage = "Buy vehicle"				;	newAction = drawMenu(	"Buy vehicle"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_BUY_EQUIP_FACILITY			:	instanceGame.	UserMessage = "Buy facility"			;	newAction = drawMenu(	"Buy facility"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_SELL_MAIN						:	instanceGame.	UserMessage = "Sell"					;	newAction = drawMenu(	"Sell"					, optionsSell			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_SELL_ITEM						:	instanceGame.	UserMessage = "Sell items"				;	newAction = drawMenu(	"Sell items"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	//case	GAME_MENU_SELL_EQUIP_MAIN				:	instanceGame.	UserMessage = "Sell equipment"			;	newAction = drawMenu(	"Sell equipment"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_SELL_EQUIP_CHARACTER			:	instanceGame.	UserMessage = "Sell character"			;	newAction = drawMenu(	"Sell character"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_SELL_EQUIP_WEAPON				:	instanceGame.	UserMessage = "Sell weapon"				;	newAction = drawMenu(	"Sell weapon"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_SELL_EQUIP_ACCESSORY			:	instanceGame.	UserMessage = "Sell accessory"			;	newAction = drawMenu(	"Sell accessory"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_SELL_EQUIP_ARMOR				:	instanceGame.	UserMessage = "Sell armor"				;	newAction = drawMenu(	"Sell armor"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_SELL_EQUIP_VEHICLE			:	instanceGame.	UserMessage = "Sell vehicle"			;	newAction = drawMenu(	"Sell vehicle"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_SELL_EQUIP_FACILITY			:	instanceGame.	UserMessage = "Sell facility"			;	newAction = drawMenu(	"Sell facility"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_MENU_USE_ITEM						:	instanceGame.	UserMessage = "Use item"				;	newAction = drawMenu(	"Use item"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_ACTION_MAIN	);	break;
	case	GAME_MENU_USE_SKILL						:	instanceGame.	UserMessage = "Use skill"				;	newAction = drawMenu(	"Use skill"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_ACTION_MAIN	);	break;
	case	GAME_MENU_OPTIONS						:	instanceGame.	UserMessage = "Options"					;	newAction = drawMenu(	"Options"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_MAIN			);	break;
	case	GAME_MENU_OPTIONS_SCREEN				:	instanceGame.	UserMessage = "Screen Options"			;	newAction = drawMenu(	"Screen Options"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_OPTIONS		);	break;
	case	GAME_MENU_OPTIONS_HOTKEYS				:	instanceGame.	UserMessage = "Hotkeys"					;	newAction = drawMenu(	"Hotkeys"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_OPTIONS		);	break;
	case	GAME_START_MISSION						:	instanceGame.	UserMessage = "Start mission"			;	newAction = drawMenu(	"Start mission"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	);	break;
	case	GAME_CREDITS							:	drawCredits();	if(instanceGame.FrameInput.Keys[VK_ESCAPE]) newAction=GAME_MENU_MAIN; break;
	case	GAME_EXIT								:	instanceGame.	UserMessage = "Exiting game..."	;	instanceGame.bRunning = false; break;
	default:
		newAction=-1;
		game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::RIGHT, "%s.", "Unrecognized game state!!");
	}			

	if(newAction > -1)
		instanceGame.CurrentMenu = (GAME_STATE)newAction;


	//uint32_t userChoice = drawMenu(klib::size(optionsMain), "Welcome, dude", optionsMain);
	//game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::LEFT, "                                         ", optionsMain[userChoice].Text.c_str());
	//switch(userChoice) {
	//case 0:	game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::LEFT, "Selection result: %s.", optionsMain[userChoice].Text.c_str());	break;
	//case 1:	game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::RIGHT, "Selection result: %s.", optionsMain[userChoice].Text.c_str());	break;
	//case 2:	game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::LEFT, "Selection result: %s.", optionsMain[userChoice].Text.c_str());	break;
	//case 3:	
	//	bRunning=false; 
	//	break;
	//};

};

void drawAndPresentGame( SGame& instanceGame )
{
	static STimer frameMeasure;

	game::clearASCIIBackBuffer( ' ' );

	game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::RIGHT, "%s.", instanceGame.UserMessage.c_str());
	game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::LEFT, "sizeof(SGame): %u.", sizeof(SGame));
	
	if(instanceGame.CurrentMenu != GAME_CREDITS)
		drawStage(instanceGame);
	showMenu(instanceGame);

	frameMeasure.Frame();
	game::lineToScreen(1, 1, game::LEFT, "Frame time: %.5f seconds.", frameMeasure.LastTimeSeconds);
	//game::lineToScreen(2, 1, game::LEFT, "Frames per second: %f.", frameMeasure.LastTimeSeconds);

	
	game::presentASCIIBackBuffer();
}; 	// 

// Use this function to draw our game data
void draw( SGame& instancedGame ) // 
{
 	drawAndPresentGame(instancedGame);
};


void options(SGame& gameInstance)
{
}

int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
	int tmp = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF);
	// Clear the upper 16 bits and OR in the desired freqency
	tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;
	// Set the new bits
	_CrtSetDbgFlag(tmp);
#endif

	game::initASCIIScreen();
	
	SGame* pInstancedGame = new SGame;
	SGame& instancedGame = *pInstancedGame;
	srand((int32_t)time(0));

	int32_t seed = rand();

	for(uint32_t z=0, maxZ=TACTICAL_MAP_DEPTH; z<maxZ; ++z)
		for(uint32_t x=0, maxX=TACTICAL_MAP_WIDTH; x<maxX; ++x) {
			instancedGame.TacticalMap.Tiles[z][x] = (int(noise2D(x, z, TACTICAL_MAP_WIDTH, seed)*12)) ? ' ' : 21;
		}

	for(uint32_t z=0, maxZ=TACTICAL_MAP_DEPTH; z<maxZ; ++z)	{ instancedGame.TacticalMap.Tiles[z][0] = '#'; instancedGame.TacticalMap.Tiles[z][TACTICAL_MAP_WIDTH-1] = '#';}
	for(uint32_t x=0, maxX=TACTICAL_MAP_WIDTH; x<maxX; ++x) { instancedGame.TacticalMap.Tiles[0][x] = '#'; instancedGame.TacticalMap.Tiles[TACTICAL_MAP_DEPTH-1][x] = '#';}


	while(instancedGame.bRunning)
	{
		pollInput(instancedGame.FrameInput);
		//processMenuInput(instancedGame, instancedGame.FrameInput);
		draw(instancedGame);
	}

	if(pInstancedGame)
		delete(pInstancedGame);

	return 0;
}

void processInput(SInput& input, SGame& gameInstance)
{
}
