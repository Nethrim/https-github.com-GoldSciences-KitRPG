#define NOMINMAX

#include "draw.h"
#include "menus.h"
#include "StageProp.h"

#include <algorithm>
#include <time.h>

using namespace klib;

template<size_t _Width, size_t _Depth>
void boardToDisplay(const STacticalBoard& board, SWeightedDisplay<_Width, _Depth>& target, const SPlayerSelection& selection)
{
	static bool bSwap = false;
	for(uint32_t z=0; z<_Depth; z++)
		for(uint32_t x=0; x<_Width; x++)
		{
			if(board.Entities.Agents.Cells[z][x].PlayerIndex != -1)	
			{ 
				int32_t agentIndex = board.Entities.Agents.Cells[z][x].AgentIndex;
				target.Screen.Cells[z][x] = std::to_string(agentIndex+1)[0]; 
				if(board.Entities.Agents.Cells[z][x].PlayerIndex == PLAYER_USER) 
					target.TextAttributes.Cells[z][x] |= (selection.PlayerUnit != agentIndex) ? COLOR_DARKBLUE : COLOR_CYAN; 
				else if(board.Entities.Agents.Cells[z][x].PlayerIndex == PLAYER_ENEMY) 
					target.TextAttributes.Cells[z][x] |= (selection.TargetUnit == agentIndex) ? COLOR_RED : COLOR_DARKRED; 
				else //if(board.Entities.Agents.Cells[z][x].PlayerIndex == 1) 
					target.TextAttributes.Cells[z][x] |= COLOR_GRAY; 
			}
			else if(board.Entities.Coins	.Cells[z][x] != 0)					{ target.Screen.Cells[z][x] = '$'; target.TextAttributes.Cells[z][x] |= bSwap ? COLOR_DARKYELLOW : COLOR_ORANGE; } 
			else if(board.Entities.Props	.Cells[z][x].Definition  != -1)		{ target.Screen.Cells[z][x] = definitionsStageProp[board.Entities.Props	.Cells[z][x].Definition].Name[0]; target.TextAttributes.Cells[z][x] |= COLOR_BLACK; } 
			else if(board.Terrain.Topology	.Cells[z][x].Sharp >= 2)			{ target.Screen.Cells[z][x] = -78; target.TextAttributes.Cells[z][x] |= COLOR_BLACK; } 
			else if(board.Terrain.Topology	.Cells[z][x].Sharp >= 1)				{ target.Screen.Cells[z][x] = -78; target.TextAttributes.Cells[z][x] = COLOR_DARKGREEN; } 
			bSwap = !bSwap;
		}
	static STimer animationTimer;
	static SAccumulator<double> animationAccum = {0.0, 1.0};

	animationTimer.Frame();
	if( animationAccum.Accumulate(animationTimer.LastTimeSeconds) )
	{
		bSwap = !bSwap;
		animationAccum.Value = 0;
	};
}

template<size_t _Width, size_t _Depth>
void drawTacticalInfo(STacticalInfo& tacticalInfo, SWeightedDisplay<_Width, _Depth>& target, const SPlayerSelection& selection)
{
	uint16_t gridColor = COLOR_DARKGREEN << 4;
	char empty = ' ';
	for(uint32_t z=0; z<_Depth; ++z) {
		valueToGrid(target.TextAttributes, z, 0, LEFT, &gridColor, 1, _Width);
		valueToGrid(target.Screen, z, 0, LEFT, &empty, 1, _Width);
	}

	boardToDisplay(tacticalInfo.Board, target, selection);
}

SGameState drawTacticalScreen(SGame& instanceGame, const SGameState& returnState)
{
	SPlayer&		player	= instanceGame.Players[PLAYER_USER];

	
	static SAccumulator<double>	keyAccum = {0.0, 0.4};

	if(false == instanceGame.bTactical)
	{
		klib::initTacticalMap(instanceGame);
		instanceGame.bTactical = true;
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
			SPlayer& boardPlayer	= instanceGame.Players[playerIndex];
			selectedTile	= boardPlayer.Army[boardPlayer.Squad.Agents[agentIndex]].Name;
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
		else if(tacticalInfo.Board.Terrain.Topology.Cells[boardZ][boardX].Sharp >= 2)
		{
			selectedTile	= "Full cover terrain height: " + std::to_string(tacticalInfo.Board.Terrain.Topology.Cells[boardZ][boardX].Sharp);
			messageColor	= COLOR_GRAY;
			bDrawText		= true;
		}
		else if(tacticalInfo.Board.Terrain.Topology.Cells[boardZ][boardX].Sharp >= 1)
		{
			selectedTile	= "Partial cover terrain height: " + std::to_string(tacticalInfo.Board.Terrain.Topology.Cells[boardZ][boardX].Sharp);
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
	printfToGrid(globalDisplay.Screen, tacticalDisplayStop+3, tacticalDisplayX	+1, LEFT, "%i, %i", mouseX-tacticalDisplayX, mouseY-TACTICAL_DISPLAY_POSY);

	const SPlayer& enemy = instanceGame.Players[PLAYER_ENEMY];
	if(player.Selection.TargetUnit != -1 && enemy.Squad.Agents[player.Selection.TargetUnit] != -1 && enemy.Army[enemy.Squad.Agents[player.Selection.TargetUnit]].Points.LifeCurrent.Health > 0) {
		selectedTile = "Target: " + enemy.Army[enemy.Squad.Agents[player.Selection.TargetUnit]].Name;
		int32_t actualX = lineToGrid(globalDisplay.Screen, tacticalDisplayStop+3, tacticalDisplayX+1, RIGHT, selectedTile.c_str());
		valueToGrid(globalDisplay.TextAttributes, tacticalDisplayStop+3, actualX, LEFT, &(messageColor = COLOR_RED), 1, (int32_t)selectedTile.size());
	}

	SCellCoord& agentPosition = player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Position;
	SEntityTiles<STacticalBoard::Width, STacticalBoard::Depth>& terrainEntities = instanceGame.TacticalInfo.Board.Entities;
	if( player.Squad.TargetPositions[player.Selection.PlayerUnit] == agentPosition )
	{
		if(0 != instanceGame.FrameInput.MouseButtons[4]) 
		{
			int32_t tacticalMouseX = mouseX-tacticalDisplayX;
			int32_t tacticalMouseY = mouseY-TACTICAL_DISPLAY_POSY;
			if( tacticalMouseX >= 0 && tacticalMouseX < STacticalBoard::Width
			 &&	tacticalMouseY >= 0 && tacticalMouseY < STacticalBoard::Depth
			 &&	terrainEntities.Agents.Cells[tacticalMouseY][tacticalMouseX].AgentIndex == -1
			 &&	terrainEntities.Props .Cells[tacticalMouseY][tacticalMouseX].Definition	== -1
			 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[tacticalMouseY][tacticalMouseX].Sharp	< 1
			 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[tacticalMouseY][tacticalMouseX].Smooth	< 1
			)
				player.Squad.TargetPositions[player.Selection.PlayerUnit] = {tacticalMouseX, agentPosition.y, tacticalMouseY};
		}
	}
	else
	{
		SGrid<SCharacterTile, STacticalBoard::Width, STacticalBoard::Depth>& terrainAgents = terrainEntities.Agents;
		SCharacterTile cellValue = terrainAgents.Cells[agentPosition.z][agentPosition.x];
		//terrainAgents.Cells[agentPosition.z][agentPosition.x].PlayerIndex	= -1;
		//terrainAgents.Cells[agentPosition.z][agentPosition.x].SquadIndex	= 0;
		//terrainAgents.Cells[agentPosition.z][agentPosition.x].AgentIndex	= -1;

		SCellCoord initialPosition = agentPosition;

		if( player.Squad.TargetPositions[player.Selection.PlayerUnit].z > agentPosition.z 
		 && terrainEntities.Agents.Cells[agentPosition.z+1][agentPosition.x].AgentIndex == -1 
		 &&	terrainEntities.Props .Cells[agentPosition.z+1][agentPosition.x].Definition	== -1
		 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[agentPosition.z+1][agentPosition.x].Sharp	< 1
		 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[agentPosition.z+1][agentPosition.x].Smooth	< 1
		)
			agentPosition.z++;
		else if( player.Squad.TargetPositions[player.Selection.PlayerUnit].z < agentPosition.z 
		 && terrainEntities.Agents.Cells[agentPosition.z-1][agentPosition.x].AgentIndex == -1 
		 &&	terrainEntities.Props .Cells[agentPosition.z-1][agentPosition.x].Definition	== -1
		 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[agentPosition.z-1][agentPosition.x].Sharp	< 1
		 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[agentPosition.z-1][agentPosition.x].Smooth	< 1
		)
			agentPosition.z--;

		if( player.Squad.TargetPositions[player.Selection.PlayerUnit].x > agentPosition.x 
		 && terrainEntities.Agents.Cells[agentPosition.z][agentPosition.x+1].AgentIndex == -1 
		 &&	terrainEntities.Props .Cells[agentPosition.z][agentPosition.x+1].Definition	== -1
		 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[agentPosition.z][agentPosition.x+1].Sharp	< 1
		 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[agentPosition.z][agentPosition.x+1].Smooth	< 1
		)
			agentPosition.x++;
		else if( player.Squad.TargetPositions[player.Selection.PlayerUnit].x < agentPosition.x 
		 && terrainEntities.Agents.Cells[agentPosition.z][agentPosition.x-1].AgentIndex == -1 
		 &&	terrainEntities.Props .Cells[agentPosition.z][agentPosition.x-1].Definition	== -1
		 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[agentPosition.z][agentPosition.x-1].Sharp	< 1
		 &&	instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[agentPosition.z][agentPosition.x-1].Smooth	< 1
		)
			agentPosition.x--;

		if( initialPosition == agentPosition ) {
			if(rand()%2)
				agentPosition.x += rand()%3 - 1;
			else
				agentPosition.z += rand()%3 - 1;
		}

		if( terrainAgents.Cells[initialPosition.z][initialPosition.x].AgentIndex == player.Selection.PlayerUnit 
		 && terrainAgents.Cells[initialPosition.z][initialPosition.x].PlayerIndex == PLAYER_USER )
			terrainAgents.Cells[initialPosition.z][initialPosition.x] = {-1, -1, -1};
		if(terrainAgents.Cells[agentPosition.z][agentPosition.x].AgentIndex == -1 && terrainAgents.Cells[agentPosition.z][agentPosition.x].PlayerIndex == -1 )
			terrainAgents.Cells[agentPosition.z][agentPosition.x] = {PLAYER_USER, (int8_t)player.Selection.PlayerSquad, (int8_t)player.Selection.PlayerUnit};
	}


	if( player.Selection.PlayerUnit != -1 
	 && player.Squad.Agents[player.Selection.PlayerUnit] != -1 
	 && player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Points.LifeCurrent.Health > 0
	 && player.Squad.TargetPositions[player.Selection.PlayerUnit] != player.Army[player.Squad.Agents[player.Selection.PlayerUnit]].Position
	) 
	{
		selectedTile = "Target Position: " + std::to_string(player.Squad.TargetPositions[player.Selection.PlayerUnit].x) + ", " + std::to_string(player.Squad.TargetPositions[player.Selection.PlayerUnit].z);
		int32_t actualX = lineToGrid(globalDisplay.Screen, tacticalDisplayStop+2, tacticalDisplayX+1, RIGHT, selectedTile.c_str());
		valueToGrid(globalDisplay.TextAttributes, tacticalDisplayStop+2, actualX, LEFT, &(messageColor = COLOR_CYAN), 1, (int32_t)selectedTile.size());
	}

 	TURN_ACTION selectedAction = drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuTitle, optionsCombatTurn, instanceGame.FrameInput, TURN_ACTION_MENUS, TURN_ACTION_CONTINUE);
	
	if(selectedAction == TURN_ACTION_MENUS)
		return {GAME_STATE_WELCOME_COMMANDER};
	else if(selectedAction != TURN_ACTION_CONTINUE)
		instanceGame.UserError = "This function isn't available!";

	bool bIsAliveUser = false, bIsAliveEnemy = false;
	for(size_t iAgent = 0; iAgent < size(player.Squad.Agents); iAgent++)
		if(player.Squad.Agents[iAgent] != -1 && player.Army[player.Squad.Agents[iAgent]].Points.LifeCurrent.Health > 0)
		{	
			bIsAliveUser = true;
			break;
		}

	for(size_t iAgent = 0; iAgent < size(player.Squad.Agents); iAgent++)
		if(enemy.Squad.Agents[iAgent] != -1 && enemy.Army[enemy.Squad.Agents[iAgent]].Points.LifeCurrent.Health > 0)
		{	
			bIsAliveEnemy = true;
			break;
		}

	if(bIsAliveUser && bIsAliveEnemy) // If players have agents still alive we just continue in the tactical screen. Otherwise go back to main screen.
		return returnState;

	instanceGame.bTactical = false;
	// Should we give rewards before exiting?
	return {GAME_STATE_WELCOME_COMMANDER};
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
					agentPosition.x = 1+(int32_t)(rangeX * noiseNormal((1+iAgent)*agentPosition.z*(iAgent+agentPosition.x), seed+rangeZ)	);
					agentPosition.z = 1+(int32_t)(rangeZ * noiseNormal(((1+iAgent)<<16)*agentPosition.x, seed = (int32_t)time(0))	);
					if(playerIndex == PLAYER_ENEMY) 
					{
						agentPosition.x += terrainWidth-rangeX-2;
						agentPosition.z += terrainDepth-rangeZ-2;
					}
				}

				player.Army[player.Squad.Agents[iAgent]].Position = agentPosition;
				player.Squad.TargetPositions[iAgent] = agentPosition;
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
	fillCellsFromNoise(terrainTopology, {0,1},	(int32_t)time(0), {0, 0}, 50);
	fillCellsFromNoise(terrainTopology, {0,2},	(int32_t)time(0)<<8, {0, 0}, 50);
	fillCellsFromNoise(terrainTopology, {0,3},	(int32_t)time(0)<<16, {0, 0}, 50);
	
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

