#include "Game.h"

#include "menus.h"

#define NOMINMAX

#include <crtdbg.h>
#include <Windows.h>

#include <algorithm>

void credits()
{
	game::clearASCIIBackBuffer( ' ' );
	int32_t offset = (game::getASCIIBackBufferHeight()>>1)-4;
	game::lineToScreen(offset++, 0, game::CENTER, "%-30.30s: %16.16s", "Programming"						, "Dreadwords."	);
	game::lineToScreen(offset++, 0, game::CENTER, "%-30.30s: %16.16s", "Design"								, "Dreadwords."	);
	game::lineToScreen(offset++, 0, game::CENTER, "%-30.30s: %16.16s", "Management"							, "Dreadwords."	);
	game::lineToScreen(offset++, 0, game::CENTER, "%-30.30s: %16.16s", "Art Direction"						, "Dreadwords."	);
	game::lineToScreen(offset++, 0, game::CENTER, "%-30.30s: %16.16s", "Storyline Direction"				, "Dreadwords."	);
	game::lineToScreen(offset++, 0, game::CENTER, "%-30.30s: %16.16s", "Graphics"							, "Dreadwords."	);
	game::lineToScreen(offset++, 0, game::CENTER, "%-30.30s: %16.16s", "Owner"								, "Dreadwords."	);
	game::lineToScreen(offset++, 0, game::CENTER, "%-30.30s: %16.16s", "Loosely based on homework code by"	, "Nethrim."	);
	game::lineToScreen(offset+=4, 0, game::CENTER, "Press ESC to go back.");
	//drawGame(gameObject);
	game::presentASCIIBackBuffer();

	while(0 == GetAsyncKeyState(VK_ESCAPE))
		continue;
}

void drawStage(SGame&)	{
};

void showMenu(SGame& instanceGame)	{
	
	int32_t newAction=-1;
	switch(instanceGame.CurrentMenu) {
	case	GAME_MENU_MAIN							:	instanceGame.	UserMessage = "Main Menu"				;	newAction = drawMenu(	"Main Menu"				, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_CONTROL_CENTER				:	instanceGame.	UserMessage = "Control Center"			;	newAction = drawMenu(	"Control Center"		, optionsControlCenter	, instanceGame.FrameInput);	break;
	case	GAME_MENU_SQUAD_SELECT_MAIN				:	instanceGame.	UserMessage = "Squad Select"			;	newAction = drawMenu(	"Squad Select"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_CONSTRUCTION_MAIN				:	instanceGame.	UserMessage = "Construction"			;	newAction = drawMenu(	"Construction"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_EQUIP_MAIN					:	instanceGame.	UserMessage = "Equipment"				;	newAction = drawMenu(	"Equipment"				, optionsEquip			, instanceGame.FrameInput);	break;
	case	GAME_MENU_EQUIP_CHARACTER				:	instanceGame.	UserMessage = "Equip character"			;	newAction = drawMenu(	"Equip character"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_EQUIP_WEAPON					:	instanceGame.	UserMessage = "Equip weapon"			;	newAction = drawMenu(	"Equip weapon"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_EQUIP_ACCESSORY				:	instanceGame.	UserMessage = "Equip accessory"			;	newAction = drawMenu(	"Equip accessory"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_EQUIP_ARMOR					:	instanceGame.	UserMessage = "Equip armor"				;	newAction = drawMenu(	"Equip armor"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_EQUIP_VEHICLE					:	instanceGame.	UserMessage = "Equip vehicle"			;	newAction = drawMenu(	"Equip vehicle"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_EQUIP_FACILITY				:	instanceGame.	UserMessage = "Equip facility"			;	newAction = drawMenu(	"Equip facility"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_ACTION_MAIN					:	instanceGame.	UserMessage = "Action"					;	newAction = drawMenu(	"Action"				, optionsCombatTurn		, instanceGame.FrameInput);	break;
	case	GAME_MENU_SENSE_MAIN					:	instanceGame.	UserMessage = "Sense"					;	newAction = drawMenu(	"Sense"					, optionsSense			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH						:	instanceGame.	UserMessage = "Research"				;	newAction = drawMenu(	"Research"				, optionsResearch		, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_CHARACTER			:	instanceGame.	UserMessage = "Research profession"		;	newAction = drawMenu(	"Research profession"	, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_WEAPON				:	instanceGame.	UserMessage = "Research weapon"			;	newAction = drawMenu(	"Research weapon"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_ACCESSORY			:	instanceGame.	UserMessage = "Research accessory"		;	newAction = drawMenu(	"Research accessory"	, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_ARMOR				:	instanceGame.	UserMessage = "Research armor"			;	newAction = drawMenu(	"Research armor"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_VEHICLE				:	instanceGame.	UserMessage = "Research vehicle"		;	newAction = drawMenu(	"Research vehicle"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_FACILITY				:	instanceGame.	UserMessage = "Research facility"		;	newAction = drawMenu(	"Research facility"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_CHARACTER	:	instanceGame.	UserMessage = "Research rank"			;	newAction = drawMenu(	"Research rank"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_WEAPON		:	instanceGame.	UserMessage = "Research science"		;	newAction = drawMenu(	"Research science"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_ACCESSORY	:	instanceGame.	UserMessage = "Research technique"		;	newAction = drawMenu(	"Research technique"	, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_ARMOR		:	instanceGame.	UserMessage = "Research technology"		;	newAction = drawMenu(	"Research technology"	, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_VEHICLE		:	instanceGame.	UserMessage = "Research mechanics"		;	newAction = drawMenu(	"Research mechanics"	, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_RESEARCH_MODIFIER_FACILITY	:	instanceGame.	UserMessage = "Research construction"	;	newAction = drawMenu(	"Research construction"	, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_INSPECT_MAIN					:	instanceGame.	UserMessage = "Inspect"					;	newAction = drawMenu(	"Inspect"				, optionsInspect		, instanceGame.FrameInput);	break;
	case	GAME_MENU_BUY_MAIN						:	instanceGame.	UserMessage = "Buy"						;	newAction = drawMenu(	"Buy"					, optionsBuy			, instanceGame.FrameInput);	break;
	case	GAME_MENU_BUY_ITEM						:	instanceGame.	UserMessage = "Buy items"				;	newAction = drawMenu(	"Buy items"				, optionsMain			, instanceGame.FrameInput);	break;
	//case	GAME_MENU_BUY_EQUIP_MAIN				:	instanceGame.	UserMessage = "Buy equipment"			;	newAction = drawMenu(	"Buy equipment"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_BUY_EQUIP_CHARACTER			:	instanceGame.	UserMessage = "Buy character"			;	newAction = drawMenu(	"Buy character"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_BUY_EQUIP_WEAPON				:	instanceGame.	UserMessage = "Buy weapon"				;	newAction = drawMenu(	"Buy weapon"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_BUY_EQUIP_ACCESSORY			:	instanceGame.	UserMessage = "Buy accessory"			;	newAction = drawMenu(	"Buy accessory"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_BUY_EQUIP_ARMOR				:	instanceGame.	UserMessage = "Buy armor"				;	newAction = drawMenu(	"Buy armor"				, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_BUY_EQUIP_VEHICLE				:	instanceGame.	UserMessage = "Buy vehicle"				;	newAction = drawMenu(	"Buy vehicle"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_BUY_EQUIP_FACILITY			:	instanceGame.	UserMessage = "Buy facility"			;	newAction = drawMenu(	"Buy facility"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_SELL_MAIN						:	instanceGame.	UserMessage = "Sell"					;	newAction = drawMenu(	"Sell"					, optionsSell			, instanceGame.FrameInput);	break;
	case	GAME_MENU_SELL_ITEM						:	instanceGame.	UserMessage = "Sell items"				;	newAction = drawMenu(	"Sell items"			, optionsMain			, instanceGame.FrameInput);	break;
	//case	GAME_MENU_SELL_EQUIP_MAIN				:	instanceGame.	UserMessage = "Sell equipment"			;	newAction = drawMenu(	"Sell equipment"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_SELL_EQUIP_CHARACTER			:	instanceGame.	UserMessage = "Sell character"			;	newAction = drawMenu(	"Sell character"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_SELL_EQUIP_WEAPON				:	instanceGame.	UserMessage = "Sell weapon"				;	newAction = drawMenu(	"Sell weapon"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_SELL_EQUIP_ACCESSORY			:	instanceGame.	UserMessage = "Sell accessory"			;	newAction = drawMenu(	"Sell accessory"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_SELL_EQUIP_ARMOR				:	instanceGame.	UserMessage = "Sell armor"				;	newAction = drawMenu(	"Sell armor"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_SELL_EQUIP_VEHICLE			:	instanceGame.	UserMessage = "Sell vehicle"			;	newAction = drawMenu(	"Sell vehicle"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_SELL_EQUIP_FACILITY			:	instanceGame.	UserMessage = "Sell facility"			;	newAction = drawMenu(	"Sell facility"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_USE_ITEM						:	instanceGame.	UserMessage = "Use item"				;	newAction = drawMenu(	"Use item"				, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_USE_SKILL						:	instanceGame.	UserMessage = "Use skill"				;	newAction = drawMenu(	"Use skill"				, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_OPTIONS						:	instanceGame.	UserMessage = "Options"					;	newAction = drawMenu(	"Options"				, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_OPTIONS_SCREEN				:	instanceGame.	UserMessage = "Screen Options"			;	newAction = drawMenu(	"Screen Options"		, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_MENU_OPTIONS_HOTKEYS				:	instanceGame.	UserMessage = "Hotkeys"					;	newAction = drawMenu(	"Hotkeys"				, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_START_MISSION						:	instanceGame.	UserMessage = "Start mission"			;	newAction = drawMenu(	"Start mission"			, optionsMain			, instanceGame.FrameInput);	break;
	case	GAME_CREDITS							:	credits();	newAction = GAME_MENU_MAIN; break;
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
	game::clearASCIIBackBuffer( ' ' );

	game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::RIGHT, "%s.", instanceGame.UserMessage.c_str());
	
	drawStage(instanceGame);
	showMenu(instanceGame);
	
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

//
//
//
//
//enum GAME_STATE
//{	GAME_STATE_MAIN_MENU
//,	GAME_STATE_COMBAT
//,	GAME_STATE_EQUIP_MAIN
//,	GAME_STATE_EQUIP_CHARACTER
//,	GAME_STATE_EQUIP_WEAPON
//,	GAME_STATE_EQUIP_ACCESSORY
//,	GAME_STATE_EQUIP_ARMOR
//,	GAME_STATE_EQUIP_VEHICLE
//,	GAME_STATE_EQUIP_FACILITY
//,	GAME_STATE_SENSE_MAIN
//,	GAME_STATE_SENSE_CHARACTER
//,	GAME_STATE_SENSE_WEAPON
//,	GAME_STATE_SENSE_ACCESSORY
//,	GAME_STATE_SENSE_ARMOR
//,	GAME_STATE_SENSE_VEHICLE
//,	GAME_STATE_SENSE_FACILITY
//,	GAME_STATE_BUY
//,	GAME_STATE_SELL
//,	GAME_STATE_USEITEM
//,	GAME_STATE_USESKILL
//};
