#define NOMINMAX

#include "draw.h"
#include "menus.h"
#include "StageProp.h"

#include <algorithm>

using namespace klib;

template<size_t _Width, size_t _Depth>
void boardToDisplay(const STacticalBoard& board, SWeightedDisplay<_Width, _Depth>& target, const SPlayerSelection& selection)
{
	for(uint32_t z=0; z<_Depth; z++)
		for(uint32_t x=0; x<_Width; x++)
		{
			if(board.Entities.Agents.Cells[z][x].PlayerIndex != -1)	
			{ 
				int32_t agentIndex = board.Entities.Agents.Cells[z][x].AgentIndex;
				target.Screen.Cells[z][x] = std::to_string(agentIndex+1)[0]; 
				if(board.Entities.Agents.Cells[z][x].PlayerIndex == PLAYER_USER) 
					target.TextAttributes.Cells[z][x] |= (selection.PlayerUnit == agentIndex) ? COLOR_CYAN : COLOR_DARKBLUE; 
				else if(board.Entities.Agents.Cells[z][x].PlayerIndex == PLAYER_ENEMY) 
					target.TextAttributes.Cells[z][x] |= (selection.TargetUnit == agentIndex) ? COLOR_RED : COLOR_DARKRED; 
				else //if(board.Entities.Agents.Cells[z][x].PlayerIndex == 1) 
					target.TextAttributes.Cells[z][x] |= COLOR_GRAY; 
			}
			else if(board.Entities.Coins	.Cells[z][x] != 0)					{ target.Screen.Cells[z][x] = '$'; target.TextAttributes.Cells[z][x] |= COLOR_ORANGE	; } 
			else if(board.Entities.Props	.Cells[z][x].Definition  != -1)		{ target.Screen.Cells[z][x] = definitionsStageProp[board.Entities.Props	.Cells[z][x].Definition].Name[0]; target.TextAttributes.Cells[z][x] |= COLOR_BLACK; } 
			else if(board.Terrain.Topology	.Cells[z][x].Sharp > 1)				{ target.Screen.Cells[z][x] = -78; target.TextAttributes.Cells[z][x] |= COLOR_GRAY; } 
			//else if(target.Screen.Cells[z][x] == ' ' || target.Screen.Cells[z][x] == -78) 
			//{ target.Screen.Cells[z][x] = -78; target.TextAttributes.Cells[z][x] |= COLOR_DARKGREEN	; } 

		}
}

template<size_t _Width, size_t _Depth>
void drawTacticalInfo(STacticalInfo& tacticalInfo, SWeightedDisplay<_Width, _Depth>& target, const SPlayerSelection& selection)
{
	uint16_t gridColor = COLOR_DARKGREEN << 4;
	for(uint32_t z=0; z<_Depth; ++z)
		valueToGrid(target.TextAttributes, z, 0, LEFT, &gridColor, 1, _Width);

	boardToDisplay(tacticalInfo.Board, target, selection);
}

SGameState drawTacticalScreen(SGame& instanceGame, const SGameState& returnState)
{
	SPlayer&		player	= instanceGame.Players[PLAYER_USER];

	static bool		tacticalEnded = true;
	
	static SAccumulator<double>	keyAccum = {0.0, 0.4};

	if(tacticalEnded) 
	{
		klib::initTacticalMap(instanceGame);
		tacticalEnded = false;
		player.Selection.PlayerUnit = player.Selection.PlayerSquad = 0;
		player.Selection.TargetUnit = player.Selection.TargetSquad = -1;
	}
	else if(0 > player.Selection.PlayerUnit || player.Selection.PlayerUnit >= size(player.Squad.Agents)) 
	{
		player.Selection.PlayerUnit		= 0;
		player.Selection.PlayerSquad	= 0;
		while(player.Squad.Agents[player.Selection.PlayerUnit] == -1 || player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Points.LifeCurrent.Health <= 0)
			player.Selection.PlayerUnit = (player.Selection.PlayerUnit + 1) % size(player.Squad.Agents);
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

	STacticalInfo& tacticalInfo = instanceGame.TacticalInfo;
	STacticalDisplay& tacticalDisplay = instanceGame.TacticalDisplay;
	drawTacticalInfo(tacticalInfo, tacticalDisplay, player.Selection);

	// Need to construct menu title

	std::string menuTitle = "Mission Over";
	if( player.Selection.PlayerUnit != -1 && player.Squad.Agents[player.Selection.PlayerUnit] != -1 && GAME_SUBSTATE_CHARACTER != instanceGame.State.Substate)
		menuTitle = "Agent #" + std::to_string(player.Selection.PlayerUnit+1) + ": " + player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Name;
	else if(player.Selection.PlayerUnit != -1)
		menuTitle = "Agent #" + std::to_string(player.Selection.PlayerUnit+1);

	klib::SGlobalDisplay& globalDisplay = instanceGame.GlobalDisplay;
	clearGrid(globalDisplay.Screen, ' ');

	static std::string selectedTile = "";
	static char messageSlow[256] = {'_',};
	static uint16_t messageColor = COLOR_DARKGREEN;
	int32_t tacticalDisplayX	= (globalDisplay.Width>>1)	- (tacticalDisplay.Width>>1);
	int32_t tacticalDisplayStop = TACTICAL_DISPLAY_POSY		+ (tacticalDisplay.Depth);

	int32_t mouseX = instanceGame.FrameInput.MouseX;
	int32_t mouseY = instanceGame.FrameInput.MouseY;
	bool bDrawText = false;
	if( mouseX >= tacticalDisplayX		&& mouseX < (int32_t)(tacticalDisplayX+tacticalDisplay.Width)
	 && mouseY >= TACTICAL_DISPLAY_POSY && mouseY < tacticalDisplayStop
	)
	{
		int32_t boardX	= mouseX-tacticalDisplayX;
		int32_t boardZ	= mouseY-TACTICAL_DISPLAY_POSY;
		int32_t playerIndex	= tacticalInfo.Board.Entities.Agents.Cells[boardZ][boardX].PlayerIndex;
		int32_t agentIndex	= tacticalInfo.Board.Entities.Agents.Cells[boardZ][boardX].AgentIndex;
		if(	playerIndex	!= -1 && agentIndex != -1 )
		{
			SPlayer& player	= instanceGame.Players[playerIndex];
			selectedTile	= player.Army[player.Squad.Agents[agentIndex]].Name;
			messageColor	= (playerIndex == PLAYER_USER) ? COLOR_CYAN : COLOR_RED;
			if(0 != instanceGame.FrameInput.MouseButtons[0]) 
			{
				if(playerIndex == PLAYER_USER) 
					player.Selection.PlayerUnit = agentIndex;
				else
					player.Selection.TargetUnit = agentIndex;
			}
			bDrawText		= true;
		}
		else if(tacticalInfo.Board.Entities.Coins.Cells[boardZ][boardX])
		{
			selectedTile	= "Coins: " + std::to_string(tacticalInfo.Board.Entities.Coins.Cells[boardZ][boardX]);
			messageColor	= COLOR_ORANGE;
			bDrawText		= true;
		}
		else if(tacticalInfo.Board.Entities.Props.Cells[boardZ][boardX].Definition != -1)
		{
			selectedTile	= definitionsStageProp[tacticalInfo.Board.Entities.Props.Cells[boardZ][boardX].Definition].Name;
			messageColor	= COLOR_GRAY;
			bDrawText		= true;
		}
		else if(tacticalInfo.Board.Terrain.Topology.Cells[boardZ][boardX].Sharp > 1)
		{
			selectedTile	= "Terrain too high: " + std::to_string(tacticalInfo.Board.Terrain.Topology.Cells[boardZ][boardX].Sharp);
			messageColor	= COLOR_GRAY;
			bDrawText		= true;
		}

		getMessageSlow(messageSlow, selectedTile, instanceGame.FrameTimer.LastTimeSeconds*4);
	}

	if(bDrawText) {
		int32_t actualX = lineToGrid(globalDisplay.Screen, tacticalDisplayStop+3, 0, CENTER, messageSlow);
		valueToGrid(globalDisplay.TextAttributes, tacticalDisplayStop+3, actualX, LEFT, &messageColor, 1, (int32_t)selectedTile.size() );
	}
	else {
		int32_t actualX = lineToGrid(globalDisplay.Screen, tacticalDisplayStop+3, 0, CENTER, "                                  ");
	}

 	TURN_ACTION selectedAction = drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuTitle, optionsCombatTurn, instanceGame.FrameInput, TURN_ACTION_MENUS, TURN_ACTION_CONTINUE);
	
	if(selectedAction == TURN_ACTION_MENUS)
		return {GAME_STATE_WELCOME_COMMANDER};
	else if(selectedAction != TURN_ACTION_CONTINUE)
		instanceGame.UserError = "This function isn't available!";

	return returnState;
}

void deployAgents
	( SPlayer&	player
	, const int32_t	playerIndex
	, const SGrid<STopologyHeight, STacticalBoard::Width, STacticalBoard::Depth>	& terrainTopology	 
	, SEntityTiles<STacticalBoard::Width, STacticalBoard::Depth>					& terrainEntities	
	, int32_t seed
	)
{
	const uint32_t	terrainWidth = terrainTopology.Width, 
					terrainDepth = terrainTopology.Depth;

	int32_t rangeX	= terrainWidth / 5;
	int32_t rangeZ	= terrainDepth / 5;

	for(uint32_t iAgent=0, agentCount = (uint32_t)klib::size(player.Squad.Agents); iAgent<agentCount; iAgent++)
	{
		if(player.Squad.Agents[iAgent] != -1)
		{
			if(player.Army[player.Squad.Agents[iAgent]].Points.LifeCurrent.Health > 0)
			{
				SCellCoord agentPosition;
				agentPosition.x = 1+(int32_t)(rangeX * noiseNormal(iAgent, seed)	);
				agentPosition.z = 1+(int32_t)(rangeZ * noiseNormal(iAgent, seed<<8)	);
				if(playerIndex == PLAYER_ENEMY) 
				{
					agentPosition.x += terrainWidth-rangeX-2;
					agentPosition.z += terrainDepth-rangeZ-2;
				}


				while( terrainTopology			.Cells	[agentPosition.z][agentPosition.x].Sharp		>	1
					|| terrainEntities.Agents	.Cells	[agentPosition.z][agentPosition.x].PlayerIndex	!= -1
					|| terrainEntities.Props	.Cells	[agentPosition.z][agentPosition.x].Definition	!= -1 
					)
				{
					agentPosition.x = 1+(int32_t)(rangeX * noiseNormal((1+iAgent)*agentPosition.z, seed+rangeZ)	);
					agentPosition.z = 1+(int32_t)(rangeZ * noiseNormal(((1+iAgent)<<16)*agentPosition.x, seed<<8)	);
					if(playerIndex == PLAYER_ENEMY) 
					{
						agentPosition.x += terrainWidth-rangeX-2;
						agentPosition.z += terrainDepth-rangeZ-2;
					}
				}

				player.Army[player.Squad.Agents[iAgent]].Position = agentPosition;
				terrainEntities.Agents.Cells[agentPosition.z][agentPosition.x].PlayerIndex	= playerIndex;
				terrainEntities.Agents.Cells[agentPosition.z][agentPosition.x].SquadIndex	= 0;
				terrainEntities.Agents.Cells[agentPosition.z][agentPosition.x].AgentIndex	= iAgent;
			}
		};
	}
}

void generateTopology
	( SGrid<STopologyHeight, STacticalBoard::Width, STacticalBoard::Depth>	& terrainTopology	 
	, SGrid<int8_t, STacticalBoard::Width, STacticalBoard::Depth>			& terrainCollision
	, int32_t seed
	)
{
	const uint32_t	terrainWidth = terrainTopology.Width, 
					terrainDepth = terrainTopology.Depth;

	//fillCellsFromNoise(terrainTopology, {2,0},	(int32_t)seed+0, {0, 0}, 50);
	//fillCellsFromNoise(terrainTopology, {0,1},	(int32_t)seed+3, {0, 0}, 50);
	fillCellsFromNoise(terrainTopology, {0,5},	(int32_t)seed+6, {0, 0}, 10);
	
	int8_t*				cellsCollision	= &terrainCollision	.Cells[0][0];
	STopologyHeight*	cellsHeight		= &terrainTopology	.Cells[0][0];
	for(uint32_t i=0, count = terrainDepth*terrainWidth; i<count; i++) {
		cellsCollision[i] = cellsHeight[i].Sharp + cellsHeight[i].Smooth;
	};
}


void populateProps
	( SGrid<STopologyHeight, STacticalBoard::Width, STacticalBoard::Depth>	& terrainTopology	 
	, SEntityTiles<STacticalBoard::Width, STacticalBoard::Depth>			& terrainEntities	
	, int32_t seed
	, int32_t maxCoins
	)
{
	const uint32_t	terrainWidth = terrainTopology.Width, 
					terrainDepth = terrainTopology.Depth;

	for(uint32_t z=0; z<terrainDepth; ++z)
		for(uint32_t x=0; x<terrainWidth; ++x)
		{
			double noise1 = noiseNormal(z*terrainDepth+x, seed);
			double noise2 = noiseNormal(z*terrainDepth+x, seed<<8);
			if(terrainTopology.Cells[z][x].Sharp <= 1 && noise1 > 0.99)
				terrainEntities.Props.Cells[z][x].Definition = 1+(int16_t)(rand()%(size(definitionsStageProp)-1));
			else if(terrainTopology.Cells[z][x].Sharp <= 1 && terrainEntities.Props.Cells[z][x].Definition == -1 && noise2 > 0.975)
				terrainEntities.Coins.Cells[z][x] = rand()%(1+maxCoins);
		}
}

void klib::initTacticalMap(SGame& instanceGame)
{
	instanceGame.TacticalInfo.Board.Clear();

	SGrid<STopologyHeight, STacticalBoard::Width, STacticalBoard::Depth>	& terrainTopology	= instanceGame.TacticalInfo.Board.Terrain.Topology;
	SGrid<int8_t, STacticalBoard::Width, STacticalBoard::Depth>				& terrainCollision	= instanceGame.TacticalInfo.Board.Terrain.Collision;
	SEntityTiles<STacticalBoard::Width, STacticalBoard::Depth>				& terrainEntities	= instanceGame.TacticalInfo.Board.Entities;
	int32_t seed = instanceGame.Seed;

	const SPlayer& enemy = instanceGame.Players[PLAYER_ENEMY];

	int32_t maxCoins = 0;
	for(uint32_t iAgent=0, count=(uint32_t)size(enemy.Squad.Agents); iAgent<count; ++iAgent)
	{
		if(enemy.Squad.Agents[iAgent] != -1)
		{
			const SEntityPoints agentPoints = calculateFinalPoints(enemy.Army[enemy.Squad.Agents[iAgent]]);
			maxCoins += agentPoints.CostMaintenance;
		}
	}

	generateTopology(terrainTopology, terrainCollision, seed);
	populateProps	(terrainTopology, terrainEntities , seed, maxCoins);

	deployAgents(instanceGame.Players[PLAYER_USER]	, 0, terrainTopology, terrainEntities, seed);
	deployAgents(instanceGame.Players[PLAYER_ENEMY]	, 1, terrainTopology, terrainEntities, seed*seed);
	drawTacticalInfo(instanceGame.TacticalInfo, instanceGame.PostEffectDisplay, instanceGame.Players[PLAYER_NEUTRAL].Selection);
}

void klib::drawTacticalMap(SGame& instanceGame, SPostEffectDisplay& target)
{
	drawTacticalInfo(instanceGame.TacticalInfo, target, instanceGame.Players[PLAYER_NEUTRAL].Selection);
}

