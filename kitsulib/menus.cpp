#define NOMINMAX

#include "Game.h"
#include "draw.h"

#include "credits.h"

#include <algorithm>

klib::STimer drawMenu_timer;
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

SGameState drawSquadSetupMenu(SGame& instanceGame, const SGameState& returnValue);
SGameState drawResearch(SGame& instanceGame, const SGameState& returnState);
SGameState drawWelcome(SGame& instanceGame, const SGameState& returnValue);

void handleSubstateChange(SGame& instanceGame, const SGameState& newState, const SGameState& prevState) {
	clearASCIIBackBuffer(' ');
	clearGrid(instanceGame.TacticalDisplay.Screen);
	clearGrid(instanceGame.GlobalDisplay.Screen);
	clearGrid(instanceGame.PostEffectDisplay.Screen);
	clearGrid(instanceGame.MenuDisplay);
	resetCursorString(instanceGame.SlowMessage);
}

void handleStateChange(SGame& instanceGame, const SGameState& newState, const SGameState& prevState)
{
	clearASCIIBackBuffer(' ');
	clearGrid(instanceGame.TacticalDisplay.Screen);
	clearGrid(instanceGame.GlobalDisplay.Screen);
	clearGrid(instanceGame.PostEffectDisplay.Screen);
	clearGrid(instanceGame.MenuDisplay);
	resetCursorString(instanceGame.SlowMessage);

	switch(newState.State)
	{
	case GAME_STATE_MENU_MAIN:
		fillCellsFromNoise(instanceGame.PostEffectDisplay.Screen, (char)19, (int32_t)(instanceGame.FrameTimer.FramesLastSecond*10));
		drawGridBorders(instanceGame.PostEffectDisplay.Screen, '@'); 
		instanceGame.UserMessage = "Main Menu";
		break;

	case GAME_STATE_START_MISSION:
		instanceGame.UserMessage = "Start mission";
		klib::initTacticalMap(instanceGame);
		break;

	case GAME_STATE_WELCOME_COMMANDER:
		if(newState.Substate == GAME_SUBSTATE_RESET) {
			instanceGame.UserMessage = "Welcome commander";
			resetGame(instanceGame);
			clearGrid(instanceGame.PostEffectDisplay.Screen);
		}
		else
			instanceGame.UserMessage = "Welcome back commander";

	case GAME_STATE_MENU_SQUAD_SETUP:	instanceGame.UserMessage = "Squad Setup";	break;
	case GAME_STATE_CREDITS:			instanceGame.UserMessage = "Credits";		break;
	case GAME_STATE_MENU_RESEARCH:		instanceGame.UserMessage = "Research";		break;
	default:
		break;

	}
}

void klib::showMenu(SGame& instanceGame) {

	SGameState newAction=instanceGame.State;

	static const SMenu<SGameState, size(optionsMain			)> menuMain			(optionsMain		,  {GAME_STATE_EXIT					},	"Main Menu"			, 20, true, "Exit game");
	static const SMenu<SGameState, size(optionsMainInGame	)> menuMainInGame	(optionsMainInGame	,  {GAME_STATE_EXIT					},	"Main Menu"			, 20, true, "Exit game");
	static const SMenu<SGameState, size(optionsEquip		)> menuEquip		(optionsEquip		,  {GAME_STATE_MENU_SQUAD_SETUP		},	"Arsenal"			);
	static const SMenu<SGameState, size(optionsConfig		)> menuConfig		(optionsConfig		,  {GAME_STATE_MENU_MAIN			},	"Options"			, 26);
	static const SMenu<SGameState, size(optionsMainInGame	)> menuStartMission	(optionsMainInGame	,  {GAME_STATE_WELCOME_COMMANDER	},	"Start mission"		);
	static const SMenu<SGameState, size(optionsBuy			)> menuBuy			(optionsBuy			,  {GAME_STATE_WELCOME_COMMANDER	},	"Buy"				);
	static const SMenu<SGameState, size(optionsSell			)> menuSell			(optionsSell		,  {GAME_STATE_WELCOME_COMMANDER	},	"Sell"				);

	switch(instanceGame.State.State) {
	case GAME_STATE_MENU_MAIN			:	
		if( instanceGame.bStarted )
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen, menuMainInGame, instanceGame.FrameInput, instanceGame.State));	
		else																			  
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen, menuMain, instanceGame.FrameInput, instanceGame.State));	

		break;

	case GAME_STATE_MENU_EQUIPMENT		:	instanceGame.UserMessage	= menuEquip			.Title; newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen,	menuEquip		, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_MENU_OPTIONS		:	instanceGame.UserMessage	= menuConfig		.Title; newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen,	menuConfig		, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_START_MISSION		:	instanceGame.UserMessage	= menuMainInGame	.Title; newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen,	menuMainInGame	, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_MENU_BUY			:	instanceGame.UserMessage	= menuBuy			.Title; newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen,	menuBuy			, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_MENU_SELL			:	instanceGame.UserMessage	= menuSell			.Title; newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen,	menuSell		, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_MENU_SQUAD_SETUP	:	instanceGame.UserMessage	= "Squad Setup"			; newAction = processMenuReturn(instanceGame, drawSquadSetupMenu	(instanceGame, instanceGame.State));	break;
	case GAME_STATE_WELCOME_COMMANDER	:	instanceGame.UserMessage	= "Welcome Commander"	; newAction = processMenuReturn(instanceGame, drawWelcome			(instanceGame, instanceGame.State));	break;
	case GAME_STATE_MENU_RESEARCH		:	instanceGame.UserMessage	= "Research Center"		; newAction = processMenuReturn(instanceGame, drawResearch			(instanceGame, instanceGame.State));	break;
	
	case GAME_STATE_CREDITS				:	
		if(instanceGame.FrameInput.Keys[VK_ESCAPE]) 
			newAction={GAME_STATE_MENU_MAIN}; 
		//else
		//	newAction = drawCredits(instanceGame.FrameTimer.LastTimeSeconds, namesSpecialThanks, instanceGame.State); 

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
