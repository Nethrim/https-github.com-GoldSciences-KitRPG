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

void handleSubstateChange(SGame& instanceGame, const SGameState& newState, const SGameState& prevState) {
	clearASCIIBackBuffer(' ');
	instanceGame.TacticalDisplay	.Clear();	//	clearGrid(instanceGame.TacticalDisplay.Screen);
	instanceGame.GlobalDisplay		.Clear();	//	clearGrid(instanceGame.GlobalDisplay.Screen);
	instanceGame.PostEffectDisplay	.Clear();	//	clearGrid(instanceGame.PostEffectDisplay.Screen);
	clearGrid(instanceGame.MenuDisplay);
	//resetCursorString(instanceGame.SlowMessage);
}

void handleStateChange(SGame& instanceGame, const SGameState& newState, const SGameState& prevState)
{
	clearASCIIBackBuffer(' ');
	instanceGame.TacticalDisplay	.Clear();	//clearGrid(instanceGame.TacticalDisplay.Screen		);
	instanceGame.GlobalDisplay		.Clear();	//clearGrid(instanceGame.GlobalDisplay.Screen		);
	instanceGame.PostEffectDisplay	.Clear();	//clearGrid(instanceGame.PostEffectDisplay.Screen	);
	clearGrid(instanceGame.MenuDisplay);
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
			clearGrid(instanceGame.PostEffectDisplay.Screen);
		}
		else
			instanceGame.StateMessage = "Welcome back commander";

	case GAME_STATE_MENU_SQUAD_SETUP:	
		instanceGame.StateMessage = "Squad Setup";	break;
		//instanceGame.GlobalDisplay.Clear();
	case GAME_STATE_CREDITS:			
		instanceGame.StateMessage = "Credits";		
		//instanceGame.GlobalDisplay.Clear();
		break;
	case GAME_STATE_MENU_RESEARCH:		instanceGame.StateMessage = "Research";		break;
	default:
		break;

	}
}

void klib::showMenu(SGame& instanceGame) {

	SGameState newAction=instanceGame.State;

	static const SMenu<SGameState, size(optionsMain			)> menuMain			(optionsMain		,  {GAME_STATE_EXIT					},	"Main Menu"			, 20, true, "Exit game");
	static const SMenu<SGameState, size(optionsMainInGame	)> menuMainInGame	(optionsMainInGame	,  {GAME_STATE_EXIT					},	"Main Menu"			, 20, true, "Exit game");
	static const SMenu<SGameState, size(optionsConfig		)> menuConfig		(optionsConfig		,  {GAME_STATE_MENU_MAIN			},	"Options"			, 26);
	static const SMenu<SGameState, size(optionsMainInGame	)> menuStartMission	(optionsMainInGame	,  {GAME_STATE_WELCOME_COMMANDER	},	"Start mission"		);
	static const SMenu<SGameState, size(optionsSell			)> menuSell			(optionsSell		,  {GAME_STATE_WELCOME_COMMANDER	},	"Sell"				);

	switch(instanceGame.State.State) {
	case GAME_STATE_MENU_MAIN			:	
		if( instanceGame.bStarted )
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuMainInGame, instanceGame.FrameInput, instanceGame.State));	
		else																			  
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuMain, instanceGame.FrameInput, instanceGame.State));	

		break;

	case GAME_STATE_MENU_OPTIONS		:	instanceGame.StateMessage	= menuConfig		.Title	;	newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuConfig		, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_START_MISSION		:	instanceGame.StateMessage	= menuMainInGame	.Title	;	newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuMainInGame	, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_MENU_SELL			:	instanceGame.StateMessage	= menuSell			.Title	;	newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuSell		, instanceGame.FrameInput, instanceGame.State ));	break;
	case GAME_STATE_MENU_EQUIPMENT		:	instanceGame.StateMessage	= "Equipment Setup"			;	newAction = processMenuReturn(instanceGame, drawEquip			(instanceGame, instanceGame.State));	break;
	case GAME_STATE_MENU_SQUAD_SETUP	:	instanceGame.StateMessage	= "Squad Setup"				;	newAction = processMenuReturn(instanceGame, drawSquadSetupMenu	(instanceGame, instanceGame.State));	break;
	case GAME_STATE_WELCOME_COMMANDER	:	instanceGame.StateMessage	= "Welcome Commander"		;	newAction = processMenuReturn(instanceGame, drawWelcome			(instanceGame, instanceGame.State));	break;
	case GAME_STATE_MENU_RESEARCH		:	instanceGame.StateMessage	= "Research Center"			;	newAction = processMenuReturn(instanceGame, drawResearch		(instanceGame, instanceGame.State));	break;
	case GAME_STATE_MENU_BUY			:	instanceGame.StateMessage	= "Buy"						;	newAction = processMenuReturn(instanceGame, drawBuy				(instanceGame, instanceGame.State));	break;
	
	case GAME_STATE_CREDITS				:	
		if(instanceGame.FrameInput.Keys[VK_ESCAPE]) 
			newAction={GAME_STATE_MENU_MAIN}; 
		//else
		//	newAction = drawCredits(instanceGame.FrameTimer.LastTimeSeconds, namesSpecialThanks, instanceGame.State); 

		break;
	case GAME_STATE_EXIT				:	
		instanceGame.StateMessage = "Exiting game...";	
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
