#define NOMINMAX

#include "Agent_helper.h"
#include "draw.h"


using namespace klib;

#define MAX_AGENT_ROWS		2
#define MAX_AGENT_COLUMNS	3

void drawSquadSlots(SGame& instanceGame, const SGameState& returnValue)
{
	SGlobalDisplay& display = instanceGame.GlobalDisplay;
	static const int32_t slotWidth		= display.Width / MAX_AGENT_COLUMNS;
	static const int32_t slotRowSpace	= 28;// display.Depth / (MAX_AGENT_ROWS);

	static const int32_t offsetYBase = TACTICAL_DISPLAY_YPOS-2;

	SPlayer& player = instanceGame.Player;

	int32_t playerOffset = (player.Selection.PlayerUnit != -1) ? std::min(std::max(0, player.Selection.PlayerUnit-1), SGameSquad::Size-6) : 0;

	for(int32_t y=0, countY=MAX_AGENT_ROWS; y<countY; ++y)
	{
		for(int32_t x=0, countX=MAX_AGENT_COLUMNS; x<countX; ++x) 
		{
			int32_t linearIndex			= y*countX+x;
			int32_t agentIndexOffset	= linearIndex+playerOffset;
			
			if(agentIndexOffset < SGameSquad::Size) {
				if( player.Squad.Agents[agentIndexOffset] != -1 )
					displayAgentSlot(display, TACTICAL_DISPLAY_YPOS+slotRowSpace*y, 1+slotWidth*x, agentIndexOffset+1, player.Army[player.Squad.Agents[agentIndexOffset]], true);
				else											 
					displayEmptySlot(display, TACTICAL_DISPLAY_YPOS+slotRowSpace*y, 1+slotWidth*x, agentIndexOffset+1);
			}
		}
	}
}

SGameState drawSquadSetupMenu(SGame& instanceGame, const SGameState& returnValue)
{
	drawSquadSlots(instanceGame, returnValue);

	SPlayer& player = instanceGame.Player;

	static SMenuItem<int32_t> menuItems[SGameSquad::Size] = {};
	static int32_t maxNameLen = 0;
	for(uint32_t i=0, count=(uint32_t)size(menuItems); i<count; i++) 
	{
		menuItems[i].ReturnValue = i;

		char buffer[128];
		if(player.Squad.Agents[i] != -1) {
			maxNameLen = std::max(maxNameLen, sprintf_s(buffer, "Agent #%u: %s", i+1, instanceGame.Player.Army[player.Squad.Agents[i]].Name.c_str()));
			menuItems[i].Text = buffer;
		}
		else {
			maxNameLen = std::max(maxNameLen, sprintf_s(buffer, "Agent #%u: Empty slot", i+1));
			menuItems[i].Text = buffer;
		}
	}

	int32_t result = drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], "Squad setup", menuItems, instanceGame.FrameInput, SGameSquad::Size, -1, std::max(24, maxNameLen+4));
	if(SGameSquad::Size == result)
		return {GAME_STATE_WELCOME_COMMANDER};

	if( result < 0 || result >= SGameSquad::Size )
		return {GAME_STATE_MENU_SQUAD_SETUP};

	player.Selection.PlayerUnit = result;

	if( player.Squad.Agents[result] != -1 && 0 == instanceGame.FrameInput.Keys[VK_LSHIFT] ) {
		return {GAME_STATE_MENU_EQUIPMENT};
	}

	return {GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_CHARACTER};
}