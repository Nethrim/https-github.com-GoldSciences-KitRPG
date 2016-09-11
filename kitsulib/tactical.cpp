#define NOMINMAX

#include "Game.h"
#include "menus.h"

#include <algorithm>

using namespace klib;

template<size_t _Width, size_t _Depth>
void boardToDisplay(STacticalBoard& board, SWeightedDisplay<_Width, _Depth>& target)
{
	for(uint32_t z=0; z<_Depth; z++)
		for(uint32_t x=0; x<_Width; x++)
		{
				 if(board.Entities.Agents	.Cells[z][x].PlayerIndex != -1)	
			{ target.Screen.Cells[z][x] = std::to_string(board.Entities.Agents.Cells[z][x].AgentIndex)[0]; target.TextAttributes.Cells[z][x] |= COLOR_DARKCYAN		; }
			else if(board.Entities.Coins	.Cells[z][x] != 0)				
			{ target.Screen.Cells[z][x] = '$'; target.TextAttributes.Cells[z][x] |= COLOR_ORANGE		; } 
			else if(board.Entities.Props	.Cells[z][x].Definition  != -1)	
			{ target.Screen.Cells[z][x] = '!'; target.TextAttributes.Cells[z][x] |= COLOR_DARKYELLOW	; } 
		}
}

template<size_t _Width, size_t _Depth>
void drawTacticalInfo(STacticalInfo& tacticalInfo, SWeightedDisplay<_Width, _Depth>& target)
{
	//for(size_t iEntity = 0, count = board.Entities; iEntity<count; iEntity++) 
	//{
	//	SGameTiles& tiles =  board.Entities[iEntity];
	//	tiles.
	//	//targetX = enti
	//}
	uint16_t gridColor = COLOR_DARKGREEN << 8;
	for(uint32_t z=0; z<_Depth; ++z)
		valueToGrid(target.TextAttributes, z, 0, LEFT, &gridColor, 1, _Width );

	boardToDisplay(tacticalInfo.Board, target);
}

SGameState drawTacticalScreen(SGame& instanceGame, const SGameState& returnState)
{
	SPlayer&		player	= instanceGame.Player;

	static bool		tacticalEnded = true;
	
	static SAccumulator<double>	keyAccum = {0.0, 0.6};

	if(tacticalEnded) 
	{
		klib::initTacticalMap(instanceGame);
		tacticalEnded = false;
		instanceGame.Player.Selection.PlayerUnit = player.Selection.PlayerSquad = 0;
		instanceGame.Player.Selection.TargetUnit = player.Selection.TargetSquad = -1;
	}
	else if(0 > player.Selection.PlayerUnit || player.Selection.PlayerUnit >= size(player.Squad.Agents)) 
	{
		player.Selection.PlayerUnit		= 0;
		player.Selection.PlayerSquad	= 0;
	}
	else
	{
		bool bDoneWaiting = keyAccum.Accumulate(instanceGame.FrameTimer.LastTimeSeconds) > 0.0;
		if(instanceGame.FrameInput.Keys[VK_TAB] && bDoneWaiting)
		{
			if(instanceGame.FrameInput.Keys[VK_SHIFT]) 
			{
				--player.Selection.PlayerUnit;
				if(player.Selection.PlayerUnit < 0) 
					player.Selection.PlayerUnit = ((int32_t)size(player.Squad.Agents))-1;
				while(player.Squad.Agents[player.Selection.PlayerUnit] == -1 || player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Points.LifeCurrent.Health <= 0)
				{
					--player.Selection.PlayerUnit;
					if(player.Selection.PlayerUnit < 0) 
						player.Selection.PlayerUnit = ((int32_t)size(player.Squad.Agents))-1;
				}
			}
			else 
			{
				player.Selection.PlayerUnit = (player.Selection.PlayerUnit + 1) % size(player.Squad.Agents);
				while(player.Squad.Agents[player.Selection.PlayerUnit] == -1 || player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Points.LifeCurrent.Health <= 0)
					player.Selection.PlayerUnit = (player.Selection.PlayerUnit + 1) % size(player.Squad.Agents);
			}
			keyAccum.Value = 0.0;
		}
	}

	drawTacticalInfo(instanceGame.TacticalInfo, instanceGame.TacticalDisplay);

	// Need to construct menu title

	std::string menuTitle = "Mission Over";
	if( player.Selection.PlayerUnit != -1 && player.Squad.Agents[player.Selection.PlayerUnit] != -1 && GAME_SUBSTATE_CHARACTER != instanceGame.State.Substate)
		menuTitle = "Agent #" + std::to_string(player.Selection.PlayerUnit+1) + ": " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name;
	else if(player.Selection.PlayerUnit != -1)
		menuTitle = "Agent #" + std::to_string(player.Selection.PlayerUnit+1);

	klib::SGlobalDisplay& globalDisplay = instanceGame.GlobalDisplay;
	
 	TURN_ACTION selectedAction = drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuTitle, optionsCombatTurn, instanceGame.FrameInput, TURN_ACTION_MENUS, TURN_ACTION_CONTINUE);
	
	if(selectedAction == TURN_ACTION_MENUS)
		return {GAME_STATE_WELCOME_COMMANDER};
	else if(selectedAction != TURN_ACTION_CONTINUE)
		instanceGame.UserError = "This function isn't available!";

	return returnState;
}
