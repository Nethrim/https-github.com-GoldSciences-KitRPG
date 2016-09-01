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

uint32_t drawMenu_tickCount		= 0;
float drawMenu_tickTime			= 0.0f;
double drawMenu_lastKeyPress	= 0.5;
STimer drawMenu_timer;
klib::SAccumulator<double> drawMenu_accumulator = {0, 0.6};

using namespace klib;

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
	const std::string textToPrint = "Welcome back commander " + instanceGame.PlayerName + ".";

	bool bDonePrinting = drawMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
	if ( bDonePrinting )
		return drawMenu(instanceGame.UserMessage = "Control Center", optionsControlCenter, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN,}, {GAME_STATE_WELCOME_COMMANDER,}, false, "Exit this menu");

	return returnValue;
};

SGameState drawSquadSetupMenu(SGame& instanceGame, const SGameState& returnValue);

void handleSubstateChange(SGame& instanceGame, const SGameState& newState, const SGameState& prevState) {}
void handleStateChange(SGame& instanceGame, const SGameState& newState, const SGameState& prevState)
{
	switch(newState.State)
	{
	case GAME_STATE_MENU_MAIN:
		fillCellsFromNoise(instanceGame.TacticalDisplay.Screen, (char)19, (int32_t)(instanceGame.FrameTimer.FramesLastSecond*10));
		drawGridBorders(instanceGame.TacticalDisplay.Screen, '@'); 
		break;


	case GAME_STATE_START_MISSION:
		klib::initTacticalMap(instanceGame);
		break;

	case GAME_STATE_WELCOME_COMMANDER:
		if(newState.Substate == GAME_SUBSTATE_RESET)
			resetGame(instanceGame);

	case GAME_STATE_MENU_SQUAD_SETUP:
	case GAME_STATE_CREDITS:
	case GAME_STATE_MENU_RESEARCH:
	default:
		clearASCIIBackBuffer(' ');
		clearGrid(instanceGame.TacticalDisplay.Screen);
		resetCursorString(instanceGame.SlowMessage);
		break;

	}
}

void klib::showMenu(SGame& instanceGame) {
	
	SGameState newAction={(GAME_STATE_EX)-1, };

	switch(instanceGame.State.State) {
	case GAME_STATE_MENU_MAIN			:	
		if( instanceGame.bStarted )
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.UserMessage = "Main Menu"	,  optionsMainInGame, instanceGame.FrameInput, {GAME_STATE_EXIT}, instanceGame.State, true, "Exit game"));	
		else																			  
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.UserMessage = "Main Menu"	,  optionsMain, instanceGame.FrameInput, {GAME_STATE_EXIT}, instanceGame.State, true, "Exit game"));	

		break;

	case GAME_STATE_MENU_CONTROL_CENTER	:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Control Center"	, optionsControlCenter	, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN			}, instanceGame.State ));	break;
	case GAME_STATE_MENU_EQUIPMENT		:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Arsenal"			, optionsEquip			, instanceGame.FrameInput, {GAME_STATE_MENU_SQUAD_SETUP		}, instanceGame.State ));	break;
	case GAME_STATE_MENU_RESEARCH		:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research Center"	, optionsResearch		, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.State ));	break;
	case GAME_STATE_MENU_OPTIONS		:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Options"			, optionsConfig			, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN			}, instanceGame.State ));	break;
	case GAME_STATE_START_MISSION		:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Start mission"	, optionsMainInGame		, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.State ));	break;
	case GAME_STATE_MENU_BUY			:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy"				, optionsBuy			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.State ));	break;
	case GAME_STATE_MENU_SELL			:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell"			, optionsSell			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.State ));	break;
	case GAME_STATE_MENU_SQUAD_SETUP	:	instanceGame.UserMessage	= "Squad Setup"; newAction = processMenuReturn(instanceGame, drawSquadSetupMenu(instanceGame, { GAME_STATE_MENU_SQUAD_SETUP	} ));	break;
	case GAME_STATE_WELCOME_COMMANDER	:	instanceGame.UserMessage	= "Welcome back commander."; newAction = processMenuReturn(instanceGame, drawWelcome(instanceGame, {GAME_STATE_WELCOME_COMMANDER}));	break;
	case GAME_STATE_CREDITS				:	
		if(instanceGame.FrameInput.Keys[VK_ESCAPE]) 
			newAction={GAME_STATE_MENU_MAIN}; 
		else 
			newAction = instanceGame.State; 

		break;
	case GAME_STATE_EXIT				:	
		instanceGame.UserMessage = "Exiting game...";	
		instanceGame.bRunning = false; 
		newAction = instanceGame.State; 
		break;

	default:
		newAction.State = (GAME_STATE_EX)-1;
		klib::lineToScreen(klib::getASCIIBackBufferHeight()-2, 1, klib::RIGHT, "%s.", "Unrecognized game state!!");
	}			

	instanceGame.PreviousState = instanceGame.State;

	//
	if(newAction.State != instanceGame.State.State) {
		instanceGame.State			= newAction;
		handleStateChange(instanceGame, instanceGame.State, instanceGame.PreviousState);
	}
	else if(newAction.Substate != instanceGame.State.Substate) {
		instanceGame.State.Substate	= newAction.Substate;
		handleSubstateChange(instanceGame, instanceGame.State, instanceGame.PreviousState);
	}
};
