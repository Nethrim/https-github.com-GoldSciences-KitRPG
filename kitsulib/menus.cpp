#define NOMINMAX

#include "Game.h"
#include "draw.h"

#include <algorithm>

SDrawMenuGlobals	drawMenu_globals = {{}, {0, 0.6}};

using namespace klib;

SGameState processMenuReturn(SGame& instanceGame, const SGameState& returnValue)
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

SGameState drawSquadSetupMenu	(SGame& instanceGame, const SGameState& returnValue);
SGameState drawResearch			(SGame& instanceGame, const SGameState& returnState);
SGameState drawWelcome			(SGame& instanceGame, const SGameState& returnValue);
SGameState drawBuy				(SGame& instanceGame, const SGameState& returnState);
SGameState drawEquip			(SGame& instanceGame, const SGameState& returnState);

void handleSubstateChange(SGame& instanceGame, const SGameState& newState, const SGameState& prevState) 
{
	clearASCIIBackBuffer(' ');
	instanceGame.GlobalDisplay		.Clear();
	instanceGame.TacticalDisplay	.Clear();	
	//instanceGame.PostEffectDisplay	.Clear();	
	clearGrid(instanceGame.MenuDisplay);

	switch(newState.State)
	{
	case GAME_STATE_MENU_OPTIONS:
	case GAME_STATE_MENU_SELL:
		instanceGame.UserError = "This function isn't available!";
	}

	//resetCursorString(instanceGame.SlowMessage);	we shuold leave this out unless it becomes a need. This is because it turns screen transitions into an annoyance.
}

void handleStateChange(SGame& instanceGame, const SGameState& newState, const SGameState& prevState)
{
	clearASCIIBackBuffer(' ');
	instanceGame.ClearDisplays();
	resetCursorString(instanceGame.SlowMessage);

	switch(newState.State)
	{
	case GAME_STATE_MENU_MAIN:
		fillCellsFromNoise(instanceGame.PostEffectDisplay.Screen, (char)19, (int32_t)(instanceGame.FrameTimer.FramesLastSecond*10));
		drawGridBorders(instanceGame.PostEffectDisplay.Screen, '@'); 
		instanceGame.StateMessage = "Main Menu";
		break;

	case GAME_STATE_START_MISSION:
		instanceGame.StateMessage = "Start mission";
		klib::initTacticalMap(instanceGame);
		break;

	case GAME_STATE_WELCOME_COMMANDER:
		if(newState.Substate == GAME_SUBSTATE_RESET) {
			instanceGame.StateMessage = "Welcome commander";
			resetGame(instanceGame);
			handleSubstateChange(instanceGame, newState, prevState);
		}
		else
			instanceGame.StateMessage = "Welcome back commander";

	case GAME_STATE_MENU_OPTIONS:		instanceGame.StateMessage = "Options";		
	case GAME_STATE_MENU_SQUAD_SETUP:	instanceGame.StateMessage = "Squad Setup";	break;
	case GAME_STATE_CREDITS:			instanceGame.StateMessage = "Credits";		break;
	case GAME_STATE_MENU_RESEARCH:		instanceGame.StateMessage = "Research";		break;
	default:
		break;
	}

	instanceGame.UserError = instanceGame.UserMessage = "";
}

void updateState(SGame& instanceGame, const SGameState& newState) 
{
	instanceGame.PreviousState = instanceGame.State;

	//
	if(newState.State != instanceGame.State.State) {
		instanceGame.State			= newState;
		handleStateChange(instanceGame, instanceGame.State, instanceGame.PreviousState);
	}
	else if(newState.Substate != instanceGame.State.Substate) {
		instanceGame.State.Substate	= newState.Substate;
		handleSubstateChange(instanceGame, instanceGame.State, instanceGame.PreviousState);
	}

}

void klib::showMenu(SGame& instanceGame) {

	SGameState newAction=instanceGame.State;

	static const SMenu<SGameState, size(optionsMain			)> menuMain			(optionsMain		,  {GAME_STATE_EXIT					},	"Main Menu"		, 20, true, "Exit game");
	static const SMenu<SGameState, size(optionsMainInGame	)> menuMainInGame	(optionsMainInGame	,  {GAME_STATE_EXIT					},	"Main Menu"		, 20, true, "Exit game");
	static const SMenu<SGameState, size(optionsConfig		)> menuConfig		(optionsConfig		,  {GAME_STATE_MENU_MAIN			},	"Options"		, 26);
	static const SMenu<SGameState, size(optionsMainInGame	)> menuStartMission	(optionsMainInGame	,  {GAME_STATE_WELCOME_COMMANDER	},	"Start mission"	);
	static const SMenu<SGameState, size(optionsSell			)> menuSell			(optionsSell		,  {GAME_STATE_WELCOME_COMMANDER	},	"Sell"			);

	klib::SGlobalDisplay& globalDisplay = instanceGame.GlobalDisplay;

	switch(instanceGame.State.State) {
	case GAME_STATE_MENU_MAIN			:	
		if( instanceGame.bStarted )
			newAction = processMenuReturn(instanceGame, drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuMainInGame, instanceGame.FrameInput, instanceGame.State));	
		else																			  
			newAction = processMenuReturn(instanceGame, drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuMain, instanceGame.FrameInput, instanceGame.State));	

		break;

	case GAME_STATE_MENU_OPTIONS		:	instanceGame.StateMessage	= menuConfig		.Title	;	newAction = processMenuReturn(instanceGame, drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuConfig		, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_START_MISSION		:	instanceGame.StateMessage	= menuMainInGame	.Title	;	newAction = processMenuReturn(instanceGame, drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuMainInGame	, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_MENU_SELL			:	instanceGame.StateMessage	= menuSell			.Title	;	newAction = processMenuReturn(instanceGame, drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuSell			, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_MENU_EQUIPMENT		:	instanceGame.StateMessage	= "Equipment Setup"			;	newAction = processMenuReturn(instanceGame, drawEquip			(instanceGame, instanceGame.State));	break;
	case GAME_STATE_MENU_SQUAD_SETUP	:	instanceGame.StateMessage	= "Squad Setup"				;	newAction = processMenuReturn(instanceGame, drawSquadSetupMenu	(instanceGame, instanceGame.State));	break;
	case GAME_STATE_WELCOME_COMMANDER	:	instanceGame.StateMessage	= "Welcome Commander"		;	newAction = processMenuReturn(instanceGame, drawWelcome			(instanceGame, instanceGame.State));	break;
	case GAME_STATE_MENU_RESEARCH		:	instanceGame.StateMessage	= "Research Center"			;	newAction = processMenuReturn(instanceGame, drawResearch		(instanceGame, instanceGame.State));	break;
	case GAME_STATE_MENU_BUY			:	instanceGame.StateMessage	= "Buy"						;	newAction = processMenuReturn(instanceGame, drawBuy				(instanceGame, instanceGame.State));	break;
	
	case GAME_STATE_CREDITS				:	
		if(instanceGame.FrameInput.Keys[VK_ESCAPE]) 
			newAction={GAME_STATE_MENU_MAIN}; 

		break;
	case GAME_STATE_EXIT				:	
		instanceGame.StateMessage = "Exiting game...";	
		instanceGame.bRunning = false; 
		newAction = instanceGame.State; 
		break;

	default:
		newAction.State = (GAME_STATE)-1;
		instanceGame.StateMessage = "Unrecognized game state!!";
	}			

	updateState(instanceGame, newAction);
};
