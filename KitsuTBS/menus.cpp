#define NOMINMAX

#include "Game.h"
#include "draw.h"

#include <algorithm>

GAME_STATE processMenuReturn(SGame& instanceGame, GAME_STATE returnValue)
{
	switch(returnValue) { 
	case GAME_MENU_MAIN:
	default: return returnValue;
	}
};

GAME_STATE processMenuReturn(SGame& instanceGame, TURN_ACTION returnValue)
{
	switch(returnValue) { 
	case TURN_ACTION_CONTINUE:
	default: return GAME_MENU_ACTION_MAIN;
	}
};

GAME_STATE drawSquadSelectMenu(SGame& instanceGame, GAME_STATE returnValue)
{
	int32_t optionCount = instanceGame.PlayerArmy.Count;
#define MAX_AGENT_SLOTS 6
	std::string line;
	for(int32_t i=0, count=MAX_AGENT_SLOTS; i<count; ++i) {
		game::lineToScreen(instanceGame.TacticalDisplay.Depth+2+TACTICAL_DISPLAY_YPOS, 0, game::CENTER, "Agent #%u", i);
		char formatted[128] = {};
		sprintf_s(formatted, " %32.32s ", instanceGame.PlayerSquad.Slots[i].Entity.Name.c_str());
		line += formatted;
	}

	klib::SMenuItem<klib::CCharacter*> menuItems[64];
	switch(returnValue) { 
	case GAME_MENU_MAIN:
	default: 
		break;
	}


	return returnValue;
}

void showMenu(SGame& instanceGame)	{
	
	int32_t newAction=-1;
	switch(instanceGame.CurrentMenu) {
	case	GAME_MENU_MAIN							:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Main Menu"				, optionsMain			, instanceGame.FrameInput, GAME_EXIT				, "Exit game", true));	break;
	case	GAME_MENU_CONTROL_CENTER				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Control Center"			, optionsControlCenter	, instanceGame.FrameInput, GAME_MENU_MAIN			));	break;
	case	GAME_MENU_SQUAD_SELECT_MAIN				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Squad Select"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_CONSTRUCTION_MAIN				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Construction"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_MAIN			));	break;
	case	GAME_MENU_EQUIP_MAIN					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equipment"				, optionsEquip			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_EQUIP_CHARACTER				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equip character"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_EQUIP_WEAPON					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equip weapon"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_EQUIP_ACCESSORY				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equip accessory"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_EQUIP_ARMOR					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equip armor"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_EQUIP_VEHICLE					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equip vehicle"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_EQUIP_FACILITY				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equip facility"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_ACTION_MAIN					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Action"					, optionsCombatTurn		, instanceGame.FrameInput, TURN_ACTION_CONTINUE		));	break;
	case	GAME_MENU_SENSE_MAIN					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sense"					, optionsSense			, instanceGame.FrameInput, GAME_MENU_ACTION_MAIN	));	break;
	case	GAME_MENU_RESEARCH						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research"				, optionsResearch		, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_RESEARCH_CHARACTER			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research profession"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_WEAPON				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research weapon"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_ACCESSORY			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research accessory"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_ARMOR				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research armor"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_VEHICLE				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research vehicle"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_FACILITY				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research facility"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_MODIFIER_CHARACTER	:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research rank"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_MODIFIER_WEAPON		:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research science"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_MODIFIER_ACCESSORY	:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research technique"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_MODIFIER_ARMOR		:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research technology"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_MODIFIER_VEHICLE		:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research mechanics"		, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_RESEARCH_MODIFIER_FACILITY	:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research construction"	, optionsMain			, instanceGame.FrameInput, GAME_MENU_RESEARCH		));	break;
	case	GAME_MENU_INSPECT_MAIN					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Inspect"					, optionsInspect		, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_BUY_MAIN						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy"						, optionsBuy			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_BUY_ITEM						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy items"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	//case	GAME_MENU_BUY_EQUIP_MAIN				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy equipment"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_BUY_EQUIP_CHARACTER			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy character"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_BUY_EQUIP_WEAPON				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy weapon"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_BUY_EQUIP_ACCESSORY			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy accessory"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_BUY_EQUIP_ARMOR				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy armor"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_BUY_EQUIP_VEHICLE				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy vehicle"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_BUY_EQUIP_FACILITY			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy facility"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_SELL_MAIN						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell"					, optionsSell			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_SELL_ITEM						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell items"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	//case	GAME_MENU_SELL_EQUIP_MAIN				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell equipment"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_SELL_EQUIP_CHARACTER			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell character"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_SELL_EQUIP_WEAPON				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell weapon"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_SELL_EQUIP_ACCESSORY			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell accessory"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_SELL_EQUIP_ARMOR				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell armor"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_SELL_EQUIP_VEHICLE			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell vehicle"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_SELL_EQUIP_FACILITY			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell facility"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_USE_ITEM						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Use item"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_ACTION_MAIN	));	break;
	case	GAME_MENU_USE_SKILL						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Use skill"				, optionsMain			, instanceGame.FrameInput, GAME_MENU_ACTION_MAIN	));	break;
	case	GAME_MENU_OPTIONS						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Options"					, optionsMain			, instanceGame.FrameInput, GAME_MENU_MAIN			));	break;
	case	GAME_MENU_OPTIONS_SCREEN				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Screen Options"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_OPTIONS		));	break;
	case	GAME_MENU_OPTIONS_HOTKEYS				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Hotkeys"					, optionsMain			, instanceGame.FrameInput, GAME_MENU_OPTIONS		));	break;
	case	GAME_START_MISSION						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Start mission"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_CREDITS							:	if(instanceGame.FrameInput.Keys[VK_ESCAPE]) newAction=GAME_MENU_MAIN; break;
	case	GAME_EXIT								:	instanceGame.	UserMessage = "Exiting game..."	;	instanceGame.bRunning = false; break;
	default:
		newAction=-1;
		game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::RIGHT, "%s.", "Unrecognized game state!!");
	}			

	if(newAction > -1)
		instanceGame.CurrentMenu = (GAME_STATE)newAction;

};
