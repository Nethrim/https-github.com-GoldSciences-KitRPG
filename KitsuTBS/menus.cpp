#define NOMINMAX

#include "Game.h"
#include "draw.h"
#include "Profession.h"
#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "Facility.h"
#include "Vehicle.h"

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

#define MAX_AGENT_ROWS 2
#define MAX_AGENT_COLUMNS 3
#define MAX_AGENT_SLOTS (MAX_AGENT_ROWS*MAX_AGENT_COLUMNS)

void displayEmptySlot(int32_t offsetY, int32_t offsetX, int32_t agentIndex)
{
	game::lineToScreen(offsetY		, offsetX, game::LEFT, "-- Agent #%u: %-36.36s", agentIndex, "Open position");
}

void displayAgentSlot(int32_t offsetY, int32_t offsetX, int32_t agentIndex, klib::CCharacter& character)
{
	game::lineToScreen(offsetY		, offsetX, game::LEFT, "-- Agent #%u: %-36.36s", agentIndex, character.Name.c_str());
	game::lineToScreen(offsetY+2	, offsetX, game::LEFT, "%-24.24s: %-24.24s"	, "Class"				, klib::getProfessionName	(character.CurrentEquip.Profession	).c_str());
	game::lineToScreen(offsetY+3	, offsetX, game::LEFT, "%-24.24s: %-24.24s"	, "Weapon"				, klib::getWeaponName		(character.CurrentEquip.Weapon		).c_str());
	game::lineToScreen(offsetY+4	, offsetX, game::LEFT, "%-24.24s: %-24.24s"	, "Armor"				, klib::getArmorName		(character.CurrentEquip.Armor		).c_str());
	game::lineToScreen(offsetY+5	, offsetX, game::LEFT, "%-24.24s: %-24.24s"	, "Accessory"			, klib::getAccessoryName	(character.CurrentEquip.Accessory	).c_str());
	game::lineToScreen(offsetY+6	, offsetX, game::LEFT, "%-24.24s: %-24.24s"	, "Vehicle"				, klib::getVehicleName		(character.CurrentEquip.Vehicle		).c_str());
	game::lineToScreen(offsetY+7	, offsetX, game::LEFT, "%-24.24s: %-24.24s"	, "Building assigned"	, klib::getFacilityName		(character.CurrentEquip.Facility	).c_str());
}

template <size_t _Size>
void drawSquadSlots(SGame& instanceGame, GAME_STATE returnValue, std::string (&out_agentNames)[_Size])
{
	game::clearASCIIBackBuffer(' ');
	STacticalDisplay<instanceGame.TacticalDisplay.Width, instanceGame.TacticalDisplay.Depth>& display = instanceGame.TacticalDisplay;
	static const int32_t slotWidth	= display.Width / MAX_AGENT_COLUMNS;
	static const int32_t slotHeight	= display.Depth / MAX_AGENT_ROWS;
	std::string agentNames[MAX_AGENT_ROWS][MAX_AGENT_COLUMNS] = {};
	for(int32_t y=0, countY=MAX_AGENT_ROWS; y<countY; ++y) {
		for(int32_t x=0, countX=MAX_AGENT_COLUMNS; x<countX; ++x) {
			const int32_t linearIndex = y*MAX_AGENT_COLUMNS+x;
			if(linearIndex < (int32_t)instanceGame.PlayerSquad.size())
				agentNames[y][x] = instanceGame.PlayerSquad[linearIndex].Name;
			else
				agentNames[y][x] = "Position available.";
			out_agentNames[linearIndex] = agentNames[y][x];
		}
	}


	for(int32_t y=0, countY=MAX_AGENT_ROWS; y<countY; ++y) {
		for(int32_t x=0, countX=MAX_AGENT_COLUMNS; x<countX; ++x) {
			int32_t linearIndex = y*countX+x;
			if(linearIndex < (int32_t)instanceGame.PlayerSquad.size())
				displayAgentSlot(TACTICAL_DISPLAY_YPOS+25*y, 1+53*x, linearIndex+1, instanceGame.PlayerSquad[linearIndex]);
			else											 
				displayEmptySlot(TACTICAL_DISPLAY_YPOS+25*y, 1+53*x, linearIndex+1);
		}
	}
}

GAME_STATE drawSquadSetupMenu(SGame& instanceGame, GAME_STATE returnValue)
{
	int32_t optionCount = (int32_t)instanceGame.PlayerArmy.size();
	std::string agentNames[MAX_AGENT_ROWS*MAX_AGENT_COLUMNS] = {};

	drawSquadSlots(instanceGame, returnValue, agentNames);

	klib::SMenuItem<int32_t> menuItems[] = 
	{	{0, "%s"}
	,	{1, "%s"}
	,	{2, "%s"}
	,	{3, "%s"}
	,	{4, "%s"}
	,	{5, "%s"}
	};

	for(uint32_t i=0, count=(uint32_t)size(menuItems); i<count; i++) {
		char buffer[128];
		if(i < instanceGame.PlayerSquad.size()) {
			sprintf_s(buffer, menuItems[i].Text.c_str(), instanceGame.PlayerSquad[i].Name.c_str());
			menuItems[i].Text = buffer;
		}
		else
			menuItems[i].Text = "Empty slot";
	}

	int32_t result = drawMenu(instanceGame.UserMessage = "Squad Setup", menuItems, instanceGame.FrameInput, 6);
	if(6 == result)
		return GAME_MENU_CONTROL_CENTER;

	if( result < 0 || result > 5 )
		return GAME_MENU_SQUAD_SETUP_MAIN;

	if( result < ((int32_t)instanceGame.PlayerSquad.size()) && 0 == instanceGame.FrameInput.Keys[VK_LSHIFT] )
		return GAME_MENU_EQUIP_MAIN;

	return GAME_MENU_EQUIP_CHARACTER;
}

void showMenu(SGame& instanceGame)	{
	
	int32_t newAction=-1;
	switch(instanceGame.CurrentMenu) {
	case	GAME_MENU_MAIN							:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Main Menu"				, optionsMain			, instanceGame.FrameInput, GAME_EXIT				, "Exit game", true));	break;
	case	GAME_MENU_CONTROL_CENTER				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Control Center"			, optionsControlCenter	, instanceGame.FrameInput, GAME_MENU_MAIN			));	break;
	case	GAME_MENU_SQUAD_SETUP_MAIN				:	;	instanceGame.UserMessage	= "Squad Setup"; newAction = processMenuReturn(instanceGame, drawSquadSetupMenu(instanceGame, GAME_MENU_SQUAD_SETUP_MAIN	));	break;
	//case	GAME_MENU_SQUAD_SETUP_MAIN				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Squad Select"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_CONSTRUCTION_MAIN				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Construction"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_MAIN			));	break;
	case	GAME_MENU_EQUIP_MAIN					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equipment"				, optionsEquip			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
	case	GAME_MENU_EQUIP_CHARACTER				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Agent seletion"			, optionsMain			, instanceGame.FrameInput, GAME_MENU_CONTROL_CENTER	));	break;
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
