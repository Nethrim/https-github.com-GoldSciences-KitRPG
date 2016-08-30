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

// returns true if done printing all the text.
template <size_t _Size>
bool getMessageSlow(char (&message)[_Size], const std::string& textToPrint, float& nextTick, uint32_t& tickCount, double lastFrameSeconds)
{
	int32_t mesLen = strlen(message);
	nextTick	+= (float)lastFrameSeconds;
	if(nextTick > 0.05f)
	{
		tickCount++;
		if(mesLen < (_Size-2))
		{
			if(mesLen-1 < (int32_t)textToPrint.size()){
				message[mesLen-1]	= textToPrint[mesLen-1];
				message[mesLen]		= '_';
				message[mesLen+1]	= 0;
				nextTick = 0.0f;
			}
			else if(0 == (tickCount % 100))
			{
				if(message[mesLen-1] == '_')
					message[mesLen-1] = ' ';
				else
					message[mesLen-1] = '_';
			}
		}
		else 
			return true;
	}

	return ( mesLen-1 == textToPrint.size() );
}

// returns true if done printing all the text.
template <size_t _Size>
bool drawMessageSlow(char (&message)[_Size], const std::string& textToPrint, float& nextTick, uint32_t& tickCount, double lastFrameSeconds)
{
	bool bDonePrinting = getMessageSlow(message, textToPrint, nextTick, tickCount, lastFrameSeconds);

	game::lineToScreen(game::getASCIIBackBufferHeight()/2-1, game::getASCIIBackBufferWidth()/2-textToPrint.size()/2, game::LEFT, message);
	return bDonePrinting;
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
	switch(instanceGame.CurrentState.State) {
	case	GAME_STATE_MENU_MAIN					:	
		if( instanceGame.bStarted )
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.UserMessage = "Main Menu"	,  optionsMainInGame, instanceGame.FrameInput, {GAME_STATE_EXIT}, instanceGame.CurrentState, true, "Exit game"));	
		else																			  
			newAction = processMenuReturn(instanceGame, drawMenu(instanceGame.UserMessage = "Main Menu"	,  optionsMain, instanceGame.FrameInput, {GAME_STATE_EXIT}, instanceGame.CurrentState, true, "Exit game"));	
		break;

	case	GAME_STATE_MENU_CONTROL_CENTER			:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Control Center"			, optionsControlCenter	, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN			}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_SQUAD_SETUP				:	instanceGame.UserMessage	= "Squad Setup"; newAction = processMenuReturn(instanceGame, drawSquadSetupMenu(instanceGame, { GAME_STATE_MENU_SQUAD_SETUP	} ));	break;
	case	GAME_STATE_MENU_EQUIPMENT				:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Equipment"				, optionsEquip			, instanceGame.FrameInput, {GAME_STATE_MENU_SQUAD_SETUP		}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_RESEARCH				:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Research"				, optionsResearch		, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_OPTIONS					:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Options"					, optionsConfig			, instanceGame.FrameInput, {GAME_STATE_MENU_MAIN			}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_START_MISSION				:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Start mission"			, optionsMainInGame		, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_BUY						:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Buy"						, optionsBuy			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_MENU_SELL					:	newAction = processMenuReturn(instanceGame, drawMenu(	instanceGame.UserMessage	= "Sell"					, optionsSell			, instanceGame.FrameInput, {GAME_STATE_WELCOME_COMMANDER	}, instanceGame.CurrentState ));	break;
	case	GAME_STATE_WELCOME_COMMANDER			:	instanceGame.UserMessage	= "Welcome back commander."; newAction = processMenuReturn(instanceGame, drawWelcome(instanceGame, {GAME_STATE_WELCOME_COMMANDER}));	break;
	case	GAME_STATE_CREDITS						:	if(instanceGame.FrameInput.Keys[VK_ESCAPE]) newAction={GAME_STATE_MENU_MAIN}; else newAction = instanceGame.CurrentState; break;
	case	GAME_STATE_EXIT							:	instanceGame.	UserMessage = "Exiting game..."	;	instanceGame.bRunning = false; newAction = instanceGame.CurrentState; break;
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
		if(newAction.Substate == GAME_SUBSTATE_RESET) {
			initGame(instanceGame);
		}
		clearDisplay(instanceGame.TacticalDisplay);
	}

	if(newAction.State >= 0) {
		instanceGame.CurrentState = newAction;
	}

};
