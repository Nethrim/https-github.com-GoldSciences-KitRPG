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


SGameState processMenuReturn(SGame& instanceGame, SGameState returnValue)
{
	switch(returnValue.State) { 
	case GAME_STATE_MENU_MAIN:
	default: return returnValue;
	}
};

SGameState processMenuReturn(SGame& instanceGame, TURN_ACTION returnValue)
{
	switch(returnValue) { 
	case TURN_ACTION_CONTINUE:
	default: return {GAME_STATE_MENU_ACTION, };
	}
};

SGameState drawWelcome(SGame& instanceGame, SGameState returnValue)
{
	static char message[256] = {'_',0};

	int32_t mesLen = strlen(message);
	if(mesLen && memcmp(message, instanceGame.UserMessage.c_str(), std::min(mesLen-1, (int32_t)instanceGame.UserMessage.size())))
	{	
		memset(message, 0, mesLen);
		message[0] = ' ';
	}

	instanceGame.NextTick	+= (float)instanceGame.FrameTimer.LastTimeSeconds;

	if(instanceGame.NextTick > 0.05f)
	{
		instanceGame.TickCount++;
		if(mesLen-1 < (int32_t)instanceGame.UserMessage.size()){
			message[mesLen-1]	= instanceGame.UserMessage[mesLen-1];
			message[mesLen] = '_';
			instanceGame.NextTick = 0.0f;
		}
		else if(0 == (instanceGame.TickCount % 100))
		{
			if(message[mesLen-1] == '_')
				message[mesLen-1] = ' ';
			else
				message[mesLen-1] = '_';
		}
	}

	game::lineToScreen(game::getASCIIBackBufferHeight()/2-1, game::getASCIIBackBufferWidth()/2-instanceGame.UserMessage.size()/2, game::LEFT, message);

	if( mesLen-1 == instanceGame.UserMessage.size() )
		return drawMenu(instanceGame.UserMessage = "Control Center", optionsControlCenter, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN,}, {GAME_STATE_WELCOME_COMMANDER,}, false, "Exit this menu");

	return returnValue;
};


#define MAX_AGENT_ROWS 2
#define MAX_AGENT_COLUMNS 3
#define MAX_AGENT_SLOTS (MAX_AGENT_ROWS*MAX_AGENT_COLUMNS)

void displayEmptySlot(int32_t offsetY, int32_t offsetX, int32_t agentIndex)
{
	game::lineToScreen(offsetY		, offsetX, game::LEFT, "-- Agent #%u: %-36.36s", agentIndex, "Open position");
}

void displayAgentSlot(int32_t offsetY, int32_t offsetX, int32_t agentIndex, klib::CCharacter& character, bool bAddFieldNames=false)
{
	std::string nameAndLevelText;
	if( bAddFieldNames ) {
		nameAndLevelText = character.Name												;	game::lineToScreen(offsetY		, offsetX, game::LEFT, "-- Agent #%u: %-24.24s (Lv. %i)", agentIndex		, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getProfessionName	(character.CurrentEquip.Profession	);	game::lineToScreen(offsetY+2	, offsetX, game::LEFT, "%-9.9s: %-26.26s (Lv. %i)"	, "Class"				, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getWeaponName		(character.CurrentEquip.Weapon		);	game::lineToScreen(offsetY+3	, offsetX, game::LEFT, "%-9.9s: %-26.26s (Lv. %i)"	, "Weapon"				, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
		nameAndLevelText = klib::getArmorName		(character.CurrentEquip.Armor		);	game::lineToScreen(offsetY+4	, offsetX, game::LEFT, "%-9.9s: %-26.26s (Lv. %i)"	, "Armor"				, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
		nameAndLevelText = klib::getAccessoryName	(character.CurrentEquip.Accessory	);	game::lineToScreen(offsetY+5	, offsetX, game::LEFT, "%-9.9s: %-26.26s (Lv. %i)"	, "Accessory"			, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
		nameAndLevelText = klib::getVehicleName		(character.CurrentEquip.Vehicle		);	game::lineToScreen(offsetY+6	, offsetX, game::LEFT, "%-9.9s: %-26.26s (Lv. %i)"	, "Vehicle"				, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
		//nameAndLevelText = klib::getFacilityName	(character.CurrentEquip.Facility	);	game::lineToScreen(offsetY+7	, offsetX, game::LEFT, "%-9.9s: %-26.26s (Lv. %i)"	, "Building assigned"	, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
	}
	else {
		nameAndLevelText = character.Name												;	game::lineToScreen(offsetY		, offsetX, game::LEFT, "-- %-21.21s (Lv. %i)"	, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getProfessionName	(character.CurrentEquip.Profession	);	game::lineToScreen(offsetY+2	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Profession	.Level);
		nameAndLevelText = klib::getWeaponName		(character.CurrentEquip.Weapon		);	game::lineToScreen(offsetY+3	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Weapon		.Level);
		nameAndLevelText = klib::getArmorName		(character.CurrentEquip.Armor		);	game::lineToScreen(offsetY+4	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Armor		.Level);
		nameAndLevelText = klib::getAccessoryName	(character.CurrentEquip.Accessory	);	game::lineToScreen(offsetY+5	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Accessory	.Level);
		nameAndLevelText = klib::getVehicleName		(character.CurrentEquip.Vehicle		);	game::lineToScreen(offsetY+6	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Vehicle		.Level);
		//nameAndLevelText = klib::getFacilityName	(character.CurrentEquip.Facility	);	game::lineToScreen(offsetY+7	, offsetX, game::LEFT, "%-24.24s (Lv. %i)"		, nameAndLevelText.c_str(), character.CurrentEquip.Facility		.Level);
	}
}

template <size_t _Size>
void drawSquadSlots(SGame& instanceGame, SGameState returnValue, std::string (&out_agentNames)[_Size])
{
	game::clearASCIIBackBuffer(' ');
	STacticalDisplay<instanceGame.GlobalDisplay.Width, instanceGame.GlobalDisplay.Depth>& display = instanceGame.GlobalDisplay;
	static const int32_t slotWidth	= display.Width / MAX_AGENT_COLUMNS;
	static const int32_t slotHeight	= 7;// display.Depth / (MAX_AGENT_ROWS);

	for(int32_t y=0, countY=MAX_AGENT_ROWS; y<countY; ++y)
		for(int32_t x=0, countX=MAX_AGENT_COLUMNS; x<countX; ++x) 
		{
			int32_t linearIndex = y*countX+x;
			if(linearIndex < (int32_t)instanceGame.PlayerSquad.size())
				displayAgentSlot(TACTICAL_DISPLAY_YPOS+slotHeight*2*y, 1+slotWidth*x, linearIndex+1, instanceGame.PlayerSquad[linearIndex], true);
			else											 
				displayEmptySlot(TACTICAL_DISPLAY_YPOS+slotHeight*2*y, 1+slotWidth*x, linearIndex+1);
		}
}

SGameState drawSquadSetupMenu(SGame& instanceGame, SGameState returnValue)
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

	for(uint32_t i=0, count=(uint32_t)size(menuItems); i<count; i++) 
	{
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
		return {GAME_STATE_WELCOME_COMMANDER};

	if( result < 0 || result > 5 )
		return {GAME_STATE_MENU_SQUAD_SETUP};

	if( result < ((int32_t)instanceGame.PlayerSquad.size()) && 0 == instanceGame.FrameInput.Keys[VK_LSHIFT] )
		return {GAME_STATE_MENU_EQUIPMENT};

	return {GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_CHARACTER};
}

void showMenu(SGame& instanceGame)	{
	
	SGameState newAction={(GAME_STATE_EX)-1, };
	switch(instanceGame.CurrentState.State) {
	case	GAME_STATE_MENU_MAIN					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Main Menu"				, optionsMain			, instanceGame.FrameInput, {GAME_STATE_EXIT					}, instanceGame.CurrentState, true, "Exit game"));	break;
	case	GAME_STATE_MENU_CONTROL_CENTER			:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Control Center"			, optionsControlCenter	, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN			}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_SQUAD_SETUP				:	;	instanceGame.UserMessage	= "Squad Setup"; newAction = processMenuReturn(instanceGame, drawSquadSetupMenu(instanceGame, { GAME_STATE_MENU_SQUAD_SETUP	} ));	break;
	case	GAME_STATE_MENU_EQUIPMENT				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equipment"				, optionsEquip			, instanceGame.FrameInput, {GAME_STATE_MENU_SQUAD_SETUP		}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_RESEARCH				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research"				, optionsResearch		, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_OPTIONS					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Options"					, optionsMain			, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN			}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_START_MISSION				:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Start mission"			, optionsMain			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_BUY						:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy"						, optionsBuy			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_SELL					:	;	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell"					, optionsSell			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_WELCOME_COMMANDER			:	;	instanceGame.UserMessage	= "Welcome back commander."; newAction = processMenuReturn(instanceGame, drawWelcome(instanceGame, {GAME_STATE_WELCOME_COMMANDER}));	break;
	case	GAME_STATE_CREDITS						:	if(instanceGame.FrameInput.Keys[VK_ESCAPE]) newAction={GAME_STATE_MENU_MAIN}; else newAction = instanceGame.CurrentState; break;
	case	GAME_STATE_EXIT							:	instanceGame.	UserMessage = "Exiting game..."	;	instanceGame.bRunning = false; newAction =instanceGame.CurrentState; break;
	default:
		newAction.State = (GAME_STATE_EX)-1;
		game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::RIGHT, "%s.", "Unrecognized game state!!");
	}			

	if(newAction.State == GAME_STATE_MENU_MAIN && instanceGame.CurrentState.State != GAME_STATE_MENU_MAIN )
	{
		fillDisplayValueFromNoise(instanceGame.TacticalDisplay, 19, (int32_t)(instanceGame.FrameTimer.FramesLastSecond*10));
		drawDisplayBorders(instanceGame.TacticalDisplay, '@'); 
	}
	else if(newAction.State == GAME_STATE_MENU_RESEARCH && instanceGame.CurrentState.State != GAME_STATE_MENU_RESEARCH)
	{
		clearDisplay(instanceGame.TacticalDisplay);
	}
	else if(newAction.State == GAME_STATE_WELCOME_COMMANDER && instanceGame.CurrentState.State != GAME_STATE_WELCOME_COMMANDER)
	{
		clearDisplay(instanceGame.TacticalDisplay);
	}

	if(newAction.State >= 0)
		instanceGame.CurrentState = newAction;

};
