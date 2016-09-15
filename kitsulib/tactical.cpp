#define NOMINMAX

#include "draw.h"
#include "menus.h"
#include "StageProp.h"

#include <algorithm>
#include <time.h>

using namespace klib;

template<size_t _Width, size_t _Depth>
void boardToDisplay(const STacticalBoard& board, SWeightedDisplay<_Width, _Depth>& target, PLAYER_INDEX playerIndex, const SPlayerSelection& selection)
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
					target.TextAttributes.Cells[z][x] |= ((playerIndex == PLAYER_ENEMY) ? (selection.TargetUnit == agentIndex) : (selection.PlayerUnit == agentIndex)) ? COLOR_CYAN : COLOR_DARKBLUE; 
				else if(board.Entities.Agents.Cells[z][x].PlayerIndex == PLAYER_ENEMY) 
					target.TextAttributes.Cells[z][x] |= ((playerIndex == PLAYER_ENEMY) ? (selection.PlayerUnit == agentIndex) : (selection.TargetUnit == agentIndex)) ? COLOR_RED : COLOR_DARKRED; 
				else //if(board.Entities.Agents.Cells[z][x].PlayerIndex == 1) 
					target.TextAttributes.Cells[z][x] |= COLOR_GRAY; 
			}
			else if(board.Entities.Coins	.Cells[z][x] != 0)					{ target.Screen.Cells[z][x] = '$'; target.TextAttributes.Cells[z][x] |= bSwap ? COLOR_DARKYELLOW : COLOR_ORANGE; } 
			else if(board.Entities.Props	.Cells[z][x].Definition  != -1)		{ target.Screen.Cells[z][x] = definitionsStageProp[board.Entities.Props	.Cells[z][x].Definition].Name[0]; target.TextAttributes.Cells[z][x] |= COLOR_BLACK; } 
			else if(board.Terrain.Topology	.Cells[z][x].Sharp >= 2)			{ target.Screen.Cells[z][x] = -78; target.TextAttributes.Cells[z][x] |= COLOR_BLACK; } 
			else if(board.Terrain.Topology	.Cells[z][x].Sharp >= 1)			{ target.Screen.Cells[z][x] = -78; target.TextAttributes.Cells[z][x] = COLOR_DARKGREEN; } 
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
void drawTacticalInfo(STacticalInfo& tacticalInfo, SWeightedDisplay<_Width, _Depth>& target, PLAYER_INDEX playerIndex, const SPlayerSelection& selection)
{
	uint16_t gridColor = COLOR_DARKGREEN << 4;
	char empty = ' ';
	for(uint32_t z=0; z<_Depth; ++z) {
		valueToGrid(target.TextAttributes, z, 0, LEFT, &gridColor, 1, _Width);
		valueToGrid(target.Screen, z, 0, LEFT, &empty, 1, _Width);
	}

	boardToDisplay(tacticalInfo.Board, target, playerIndex, selection);
}

void initTacticalGame(SGame& instanceGame)
{
	SPlayer&		player	= instanceGame.Players[PLAYER_USER];

	klib::initTacticalMap(instanceGame);
	instanceGame.bTactical		= true;
	player.Selection.PlayerUnit	= player.Selection.PlayerSquad = 0;
	player.Selection.TargetUnit	= player.Selection.TargetSquad = -1;
	instanceGame.CurrentPlayer	= PLAYER_USER;
}

void getTileInfoFromMouse(int32_t mouseX, int32_t mouseY, int32_t tacticalDisplayX)
{
}


bool moveStep(SPlayer& player, PLAYER_INDEX playerIndex, int32_t agentIndex, STacticalBoard& board, SCellCoord& agentPosition)
{
	if(agentPosition == player.Squad.TargetPositions[player.Selection.PlayerUnit])
		return player.Squad.MovesLeft[agentIndex] <= 0;	// I added this just in case but currently there is no situation in which this function is called when the agent is in the target position already.

	SGrid<SCharacterTile, STacticalBoard::Width, STacticalBoard::Depth>& terrainAgents = board.Entities.Agents;
	SCharacterTile cellValue = terrainAgents.Cells[agentPosition.z][agentPosition.x];

	SCellCoord initialPosition = agentPosition;

	if( player.Squad.TargetPositions[player.Selection.PlayerUnit].z > agentPosition.z 
		&&	board.IsTileAvailable( agentPosition.x, agentPosition.z+1 )
		&&	player.Squad.MovesLeft[agentIndex]
	)
	{
		agentPosition.z++;
		player.Squad.MovesLeft[agentIndex]--;
	}
	else if( player.Squad.TargetPositions[player.Selection.PlayerUnit].z < agentPosition.z 
		&&	board.IsTileAvailable( agentPosition.x, agentPosition.z-1 )
		&&	player.Squad.MovesLeft[agentIndex]
	)
	{
		agentPosition.z--;
		player.Squad.MovesLeft[agentIndex]--;
	}

	if( player.Squad.TargetPositions[player.Selection.PlayerUnit].x > agentPosition.x 
		&&	board.IsTileAvailable( agentPosition.x+1, agentPosition.z )
		&&	player.Squad.MovesLeft[agentIndex]
	)
	{
		agentPosition.x++;
		player.Squad.MovesLeft[agentIndex]--;
	}
	else if( player.Squad.TargetPositions[player.Selection.PlayerUnit].x < agentPosition.x 
		&&	board.IsTileAvailable( agentPosition.x-1, agentPosition.z )
		&&	player.Squad.MovesLeft[agentIndex]
	)
	{
		agentPosition.x--;
		player.Squad.MovesLeft[agentIndex]--;
	}

	bool bArrived = true;
	if( initialPosition == agentPosition )	// If we didn't move yet is because we're stuck.
	{
		bArrived = false;

		// The following noise gives epilepsy to the agents when they get stuck against a wall.
		if(rand()%2)
			agentPosition.x += rand()%3 - 1;
		else
			agentPosition.z += rand()%3 - 1;
	}
	else 
	{
		// Check if there's money here and pick it up.
		player.Army[player.Squad.Agents[agentIndex]].Points.Coins += board.Entities.Coins.Cells[agentPosition.z][agentPosition.x]/2;
		player.Money += board.Entities.Coins.Cells[agentPosition.z][agentPosition.x]/2;
		board.Entities.Coins.Cells[agentPosition.z][agentPosition.x] = 0;
	}

	if( terrainAgents.Cells[initialPosition.z][initialPosition.x].AgentIndex == player.Selection.PlayerUnit 
	 && terrainAgents.Cells[initialPosition.z][initialPosition.x].PlayerIndex == playerIndex )
		terrainAgents.Cells[initialPosition.z][initialPosition.x] = {-1, -1, -1};
	
	if( terrainAgents.Cells[agentPosition.z][agentPosition.x].AgentIndex == -1 
	 && terrainAgents.Cells[agentPosition.z][agentPosition.x].PlayerIndex == -1 )
		terrainAgents.Cells[agentPosition.z][agentPosition.x] = {(int8_t)playerIndex, (int8_t)player.Selection.PlayerSquad, (int8_t)player.Selection.PlayerUnit};

	return (bArrived && agentPosition == player.Squad.TargetPositions[player.Selection.PlayerUnit]);
}

SGameState drawTacticalScreen(SGame& instanceGame, const SGameState& returnState)
{
	SPlayer&		playerUser	= instanceGame.Players[PLAYER_USER];
	
	static SAccumulator<double>	keyAccum = {0.0, 0.4};

	if(false == instanceGame.bTactical)
	{
		initTacticalGame(instanceGame);
	}
	else if(-1 == playerUser.Selection.PlayerUnit || playerUser.Squad.Agents[playerUser.Selection.PlayerUnit] == -1 || playerUser.Army[playerUser.Squad.Agents[playerUser.Selection.PlayerUnit]].Points.LifeCurrent.Health <= 0) 
	{
		if(!playerUser.SelectNextAgent())
			return {GAME_STATE_WELCOME_COMMANDER};
	}
	else
	{
		bool bDoneWaiting = keyAccum.Accumulate(instanceGame.FrameTimer.LastTimeSeconds) > 0.0;
		if(instanceGame.FrameInput.Keys[VK_TAB] && bDoneWaiting)
		{
			if(instanceGame.FrameInput.Keys[VK_SHIFT])
			{
				if(!playerUser.SelectPreviousAgent())
					return {GAME_STATE_WELCOME_COMMANDER};
			}
			else if(!playerUser.SelectNextAgent())
				return {GAME_STATE_WELCOME_COMMANDER};

			keyAccum.Value = 0.0;
		}
	}

	int32_t mouseX = instanceGame.FrameInput.MouseX;
	int32_t mouseY = instanceGame.FrameInput.MouseY;
	klib::SGlobalDisplay& globalDisplay = instanceGame.GlobalDisplay;
	STacticalDisplay& tacticalDisplay = instanceGame.TacticalDisplay;
	int32_t tacticalDisplayX	= (globalDisplay.Width>>1)	- (tacticalDisplay.Width>>1);

	{ // the current player is only valid in this scope. After this code the current player can change 
		SPlayer& currentPlayer = instanceGame.Players[instanceGame.CurrentPlayer];
		SCellCoord& currentAgentPosition = currentPlayer.Army[currentPlayer.Squad.Agents[currentPlayer.Selection.PlayerUnit]].Position;

		if( currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit] == currentAgentPosition )
		{
			if(instanceGame.CurrentPlayer == PLAYER_USER) 
			{
				if(0 != instanceGame.FrameInput.MouseButtons[4]) 
				{
					const SEntityTiles<STacticalBoard::Width, STacticalBoard::Depth>& terrainEntities = instanceGame.TacticalInfo.Board.Entities;
					int32_t tacticalMouseX = mouseX-tacticalDisplayX;
					int32_t tacticalMouseY = mouseY-TACTICAL_DISPLAY_POSY;
					if( tacticalMouseX >= 0 && tacticalMouseX < STacticalBoard::Width
					 &&	tacticalMouseY >= 0 && tacticalMouseY < STacticalBoard::Depth
					 &&	instanceGame.TacticalInfo.Board.IsTileAvailable(tacticalMouseX, tacticalMouseY)
					)
						currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit] = {tacticalMouseX, currentAgentPosition.y, tacticalMouseY};
				}
			}
			else
			{
				currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit] = playerUser.Army[playerUser.Squad.Agents[playerUser.Selection.PlayerUnit]].Position; 
				currentPlayer.Selection.TargetSquad	= playerUser.Selection.PlayerSquad;
				currentPlayer.Selection.TargetUnit	= playerUser.Selection.PlayerUnit;

				while(!instanceGame.TacticalInfo.Board.IsTileAvailable(currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit].x, currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit].z)) 
				{
					if( rand()%2 )
						currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit].x += (rand()%2) ? 1 : -1;
					else
						currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit].z += (rand()%2) ? 1 : -1;
				}
			}
		}
		else
		{
			if( moveStep(currentPlayer, instanceGame.CurrentPlayer, currentPlayer.Selection.PlayerUnit, instanceGame.TacticalInfo.Board, currentAgentPosition) ) 
			{
				if (instanceGame.CurrentPlayer == PLAYER_ENEMY)
				{
					if(currentPlayer.Army[currentPlayer.Squad.Agents[currentPlayer.Selection.PlayerUnit]].Position == currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit])
						currentPlayer.SelectNextAgent();
				}
						
				//instanceGame.bTurnBusy		= false;
			}
			if(0 >= currentPlayer.Squad.MovesLeft[currentPlayer.Selection.PlayerUnit]) 
			{
				instanceGame.CurrentPlayer	= (instanceGame.CurrentPlayer == PLAYER_USER) ? PLAYER_ENEMY : PLAYER_USER;
				currentPlayer.Squad.MovesLeft[currentPlayer.Selection.PlayerUnit] = currentPlayer.Army[currentPlayer.Squad.Agents[currentPlayer.Selection.PlayerUnit]].Points.Attack.Speed.Movement;
			}
		}
	}

	STacticalInfo& tacticalInfo = instanceGame.TacticalInfo;
	drawTacticalInfo(tacticalInfo, tacticalDisplay, instanceGame.CurrentPlayer, instanceGame.Players[instanceGame.CurrentPlayer].Selection);

	clearGrid(globalDisplay.Screen, ' ');

	static std::string selectedTile = "";
	static char messageSlow[256] = {'_',};
	static uint16_t messageColor = COLOR_DARKGREEN;
	int32_t tacticalDisplayStop = TACTICAL_DISPLAY_POSY		+ (tacticalDisplay.Depth);

	bool bDrawText = false;
	if(instanceGame.CurrentPlayer == PLAYER_USER)
	{
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
						playerUser.Selection.PlayerUnit = agentIndex;
					else
						playerUser.Selection.TargetUnit = agentIndex;
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
	}
	else // not user, AI
	{
	}

	if(bDrawText) {
		int32_t actualX = lineToGrid(globalDisplay.Screen, tacticalDisplayStop+3, 0, CENTER, messageSlow);
		valueToGrid(globalDisplay.TextAttributes, tacticalDisplayStop+3, actualX, LEFT, &messageColor, 1, (int32_t)selectedTile.size() );
	}
	else {
		int32_t actualX = lineToGrid(globalDisplay.Screen, tacticalDisplayStop+3, 0, CENTER, "                                  ");
	}
	printfToGrid(globalDisplay.Screen, tacticalDisplayStop+3, tacticalDisplayX	+1, LEFT, "%i, %i", mouseX-tacticalDisplayX, mouseY-TACTICAL_DISPLAY_POSY);

	const SPlayer& currentEnemy		= (instanceGame.CurrentPlayer == PLAYER_USER) ? instanceGame.Players[PLAYER_ENEMY] : instanceGame.Players[PLAYER_USER];
	const SPlayer& currentPlayer	= instanceGame.Players[instanceGame.CurrentPlayer];
	if(currentPlayer.Selection.TargetUnit != -1 && currentEnemy.Squad.Agents[currentPlayer.Selection.TargetUnit] != -1 && currentEnemy.Army[currentEnemy.Squad.Agents[currentPlayer.Selection.TargetUnit]].Points.LifeCurrent.Health > 0) {
		selectedTile = "Target: " + currentEnemy.Army[currentEnemy.Squad.Agents[currentPlayer.Selection.TargetUnit]].Name;
		int32_t actualX = lineToGrid(globalDisplay.Screen, tacticalDisplayStop+3, tacticalDisplayX+1, RIGHT, selectedTile.c_str());
		valueToGrid(globalDisplay.TextAttributes, tacticalDisplayStop+3, actualX, LEFT, &(messageColor = COLOR_RED), 1, (int32_t)selectedTile.size());
	}

	if( currentPlayer.Selection.PlayerUnit != -1 
	 && currentPlayer.Squad.Agents[currentPlayer.Selection.PlayerUnit] != -1 
	 && currentPlayer.Army[currentPlayer.Squad.Agents[currentPlayer.Selection.PlayerUnit]].Points.LifeCurrent.Health > 0
	 && currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit] != currentPlayer.Army[currentPlayer.Squad.Agents[currentPlayer.Selection.PlayerUnit]].Position
	) 
	{
		selectedTile = "Target Position: " + std::to_string(currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit].x) + ", " + std::to_string(currentPlayer.Squad.TargetPositions[currentPlayer.Selection.PlayerUnit].z);
		int32_t actualX = lineToGrid(globalDisplay.Screen, tacticalDisplayStop+2, tacticalDisplayX+1, RIGHT, selectedTile.c_str());
		valueToGrid(globalDisplay.TextAttributes, tacticalDisplayStop+2, actualX, LEFT, &(messageColor = COLOR_CYAN), 1, (int32_t)selectedTile.size());
	}

 	// Need to construct menu title
	std::string menuTitle = "Mission Over";
	if( currentPlayer.Selection.PlayerUnit != -1 && currentPlayer.Squad.Agents[currentPlayer.Selection.PlayerUnit] != -1 && GAME_SUBSTATE_CHARACTER != instanceGame.State.Substate)
		menuTitle = "Agent #" + std::to_string(currentPlayer.Selection.PlayerUnit+1) + ": " + currentPlayer.Army[currentPlayer.Squad.Agents[currentPlayer.Selection.PlayerUnit]].Name;
	else if(currentPlayer.Selection.PlayerUnit != -1)
		menuTitle = "Agent #" + std::to_string(currentPlayer.Selection.PlayerUnit+1);

	TURN_ACTION selectedAction = drawMenu(globalDisplay.Screen, &globalDisplay.TextAttributes.Cells[0][0], menuTitle, optionsCombatTurn, instanceGame.FrameInput, TURN_ACTION_MENUS, TURN_ACTION_CONTINUE);
	
	if(selectedAction == TURN_ACTION_MENUS)
		return {GAME_STATE_WELCOME_COMMANDER};
	else if(selectedAction != TURN_ACTION_CONTINUE)
		instanceGame.UserError = "This function isn't available!";

	if(currentPlayer.IsAlive() && currentEnemy.IsAlive()) // If players have agents still alive we just continue in the tactical screen. Otherwise go back to main screen.
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

				while( terrainTopology			.Cells	[agentPosition.z][agentPosition.x].Sharp		>=	1
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

				player.Squad.MovesLeft[iAgent] = calculateFinalPoints(player.Army[player.Squad.Agents[iAgent]]).Attack.Speed.Movement;
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
			if(terrainTopology.Cells[z][x].Sharp < 1 && terrainTopology.Cells[z][x].Smooth < 1 && noise1 > 0.99)
				terrainEntities.Props.Cells[z][x].Definition = 1+(int16_t)(rand()%(size(definitionsStageProp)-1));
			else if(terrainTopology.Cells[z][x].Sharp < 1 && terrainTopology.Cells[z][x].Smooth < 1 && terrainEntities.Props.Cells[z][x].Definition == -1 && noise2 > 0.99)
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

	drawTacticalInfo(instanceGame.TacticalInfo, instanceGame.PostEffectDisplay, PLAYER_NEUTRAL, instanceGame.Players[PLAYER_NEUTRAL].Selection);
}

void klib::drawTacticalMap(SGame& instanceGame, SPostEffectDisplay& target)
{
	drawTacticalInfo(instanceGame.TacticalInfo, target, PLAYER_NEUTRAL, instanceGame.Players[PLAYER_NEUTRAL].Selection);
}

