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

double drawMenu_lastKeyPress = 0.5;
STimer drawMenu_timer;

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
	static const std::string textToPrint = "Welcome back commander " + instanceGame.PlayerName + ".";

	bool bDonePrinting = drawMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.NextTick, instanceGame.TickCount, instanceGame.FrameTimer.LastTimeSeconds);
	if ( bDonePrinting ) {
		return drawMenu(instanceGame.UserMessage = "Control Center", optionsControlCenter, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN,}, {GAME_STATE_WELCOME_COMMANDER,}, false, "Exit this menu");
	}

	return returnValue;
};

SGameState drawSquadSetupMenu(SGame& instanceGame, SGameState returnValue);

void showMenu(SGame& instanceGame)	{
	
	SGameState newAction={(GAME_STATE_EX)-1, };
	switch(instanceGame.State.State) {
	case	GAME_STATE_MENU_MAIN					:	
		if( instanceGame.bStarted )
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.UserMessage = "Main Menu"	,  optionsMainInGame, instanceGame.FrameInput, {GAME_STATE_EXIT}, instanceGame.State, true, "Exit game"));	
		else																			  
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.UserMessage = "Main Menu"	,  optionsMain, instanceGame.FrameInput, {GAME_STATE_EXIT}, instanceGame.State, true, "Exit game"));	
		break;

	case	GAME_STATE_MENU_CONTROL_CENTER			:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Control Center"			, optionsControlCenter	, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN			}, instanceGame.State ));	break;
	case	GAME_STATE_MENU_SQUAD_SETUP				:	instanceGame.UserMessage	= "Squad Setup"; newAction = processMenuReturn(instanceGame, drawSquadSetupMenu(instanceGame, { GAME_STATE_MENU_SQUAD_SETUP	} ));	break;
	case	GAME_STATE_MENU_EQUIPMENT				:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equipment"				, optionsEquip			, instanceGame.FrameInput, {GAME_STATE_MENU_SQUAD_SETUP		}, instanceGame.State ));	break;
	case	GAME_STATE_MENU_RESEARCH				:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research"				, optionsResearch		, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.State ));	break;
	case	GAME_STATE_MENU_OPTIONS					:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Options"					, optionsConfig			, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN			}, instanceGame.State ));	break;
	case	GAME_STATE_START_MISSION				:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Start mission"			, optionsMainInGame		, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.State ));	break;
	case	GAME_STATE_MENU_BUY						:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy"						, optionsBuy			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.State ));	break;
	case	GAME_STATE_MENU_SELL					:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell"					, optionsSell			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.State ));	break;
	case	GAME_STATE_WELCOME_COMMANDER			:	instanceGame.UserMessage	= "Welcome back commander."; newAction = processMenuReturn(instanceGame, drawWelcome(instanceGame, {GAME_STATE_WELCOME_COMMANDER}));	break;
	case	GAME_STATE_CREDITS						:	
		if(instanceGame.FrameInput.Keys[VK_ESCAPE]) 
			newAction={GAME_STATE_MENU_MAIN}; 
		else newAction = instanceGame.State; 
		break;
	case	GAME_STATE_EXIT							:	instanceGame.	UserMessage = "Exiting game..."	;	instanceGame.bRunning = false; newAction = instanceGame.State; break;
	default:
		newAction.State = (GAME_STATE_EX)-1;
		game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::RIGHT, "%s.", "Unrecognized game state!!");
	}			

	if(newAction.State == GAME_STATE_MENU_MAIN && instanceGame.State.State != GAME_STATE_MENU_MAIN )
	{
		fillDisplayValueFromNoise(instanceGame.TacticalDisplay, 19, (int32_t)(instanceGame.FrameTimer.FramesLastSecond*10));
		drawDisplayBorders(instanceGame.TacticalDisplay, '@'); 
	}
	else if(newAction.State == GAME_STATE_MENU_RESEARCH && instanceGame.State.State != GAME_STATE_MENU_RESEARCH)
	{
		clearDisplay(instanceGame.TacticalDisplay);
		instanceGame.SlowMessage[0] = '_'; instanceGame.SlowMessage[1] = 0;
	}
	else if(newAction.State == GAME_STATE_CREDITS && instanceGame.State.State != GAME_STATE_CREDITS)
	{
		clearDisplay(instanceGame.TacticalDisplay);
		instanceGame.SlowMessage[0] = '_'; instanceGame.SlowMessage[1] = 0;
	}
	if(newAction.State == GAME_STATE_MENU_SQUAD_SETUP && instanceGame.State.State != GAME_STATE_MENU_SQUAD_SETUP)
	{
		game::clearASCIIBackBuffer(' ');
		clearDisplay(instanceGame.GlobalDisplay);
		instanceGame.SlowMessage[0] = '_'; instanceGame.SlowMessage[1] = 0;
	}
	else if(newAction.State == GAME_STATE_WELCOME_COMMANDER && instanceGame.State.State != GAME_STATE_WELCOME_COMMANDER)
	{
		instanceGame.SlowMessage[0] = '_'; instanceGame.SlowMessage[1] = 0;
		if(newAction.Substate == GAME_SUBSTATE_RESET) {
			initGame(instanceGame);
		}
		clearDisplay(instanceGame.TacticalDisplay);
	}

	if(newAction.State >= 0) {
		instanceGame.State = newAction;
	}

};
