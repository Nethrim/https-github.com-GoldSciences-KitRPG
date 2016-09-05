#define NOMINMAX

#include "Game.h"

#include "Enemy.h"

#include <ctime>

using namespace klib;

void klib::initTacticalMap(SGame& instanceGame)
{
	const uint32_t	terrainWidth = instanceGame.TacticalTiles.Terrain.Topology.Width, 
					terrainDepth = instanceGame.TacticalTiles.Terrain.Topology.Depth;

	instanceGame.TacticalTiles.Clear();
	fillCellsFromNoise(instanceGame.TacticalTiles.Terrain.Topology, {1,0}, (int32_t)instanceGame.Seed, {0, 0});
	fillCellsFromNoise(instanceGame.TacticalTiles.Terrain.Topology, {0,1}, (int32_t)instanceGame.Seed+3, {0, 0});
	fillCellsFromNoise(instanceGame.TacticalTiles.Terrain.Topology, {0,10}, (int32_t)instanceGame.Seed+6, {0, 0});
	fillCellsFromNoise(instanceGame.TacticalTiles.Terrain.Topology, {0,25}, (int32_t)instanceGame.Seed+9, {0, 0});
	//fillCellsFromNoise(instanceGame.TacticalTiles.Terrain.Topology, {0,0}, (int32_t)instanceGame.Seed+5, {0, 0});
	
	int8_t*				cellsOcclusion	= &instanceGame.TacticalTiles.Terrain.Occlusion.Cells[0][0];
	STopologyHeight*	cellsHeight		= &instanceGame.TacticalTiles.Terrain.Topology.Cells[0][0];
	for(uint32_t i=0, count = terrainDepth*terrainWidth; i<count; i++) {
		cellsOcclusion[i] = cellsHeight[i].Sharp + cellsHeight[i].Smooth;
	};
}

void klib::resetGame(SGame& instanceGame)
{
	initGame(instanceGame);
	klib::clearASCIIBackBuffer(' ');

	instanceGame.PostEffectDisplay									.Clear();
	instanceGame.TacticalDisplay									.Clear();
	instanceGame.GlobalDisplay										.Clear();
	instanceGame.TacticalTiles										.Clear();
	clearGrid(instanceGame.MenuDisplay);

	// Set up a nice prompt 
	uint32_t screenWidth  =	klib::getASCIIBackBufferWidth(),
			 screenHeight =	klib::getASCIIBackBufferHeight();

	klib::lineToScreen(screenHeight/2-1, 0, klib::CENTER, "Enter your name:");

	static const HANDLE hConsoleOut	= GetStdHandle( STD_OUTPUT_HANDLE );
	COORD cursorPos = {(SHORT)screenWidth/2-5, (SHORT)screenHeight/2};
	SetConsoleCursorPosition( hConsoleOut, cursorPos );
	SetConsoleDisplayMode(hConsoleOut, CONSOLE_FULLSCREEN_MODE, 0);
	klib::presentASCIIBackBuffer();

	static const HANDLE hConsoleIn	= GetStdHandle( STD_INPUT_HANDLE );
	FlushConsoleInputBuffer(hConsoleIn);
	getline(std::cin, instanceGame.PlayerName);

	instanceGame.bStarted = true;
}

// Sets up initial equipment and items for the player to carry or wear.
void klib::initGame(SGame& instanceGame)
{
	srand((int32_t)time(0));

	instanceGame.bStarted = false;

	instanceGame.Seed = rand();

	initTacticalMap(instanceGame);

	instanceGame.TacticalDisplay.Clear();
	instanceGame.PostEffectDisplay.Clear();
	instanceGame.GlobalDisplay.Clear();

	fillCellsFromNoise(instanceGame.PostEffectDisplay.Screen, (char)19, instanceGame.Seed);
	drawGridBorders(instanceGame.PostEffectDisplay.Screen, '@');
	
	resetCursorString(instanceGame.SlowMessage);

	instanceGame.GameArmies.Player	.clear();
	instanceGame.GameArmies.Enemy	.clear();

	instanceGame.PlayerSquads		.Clear(-1);
	instanceGame.EnemySquads		.Clear(-1);

	instanceGame.GameArmies.Player	.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	instanceGame.GameArmies.Enemy	.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	
	klib::CCharacter& adventurer		= instanceGame.GameArmies.Player[0];
	adventurer.CurrentEquip.Weapon		= {rand()%20	, rand()%11	, rand()%20};
	adventurer.CurrentEquip.Accessory	= {rand()%5		, rand()%10	, rand()%20};
	adventurer.CurrentEquip.Armor		= {rand()%10	, rand()%2	, rand()%20};
	adventurer.CurrentEquip.Profession	= {rand()%10	, rand()%8	, rand()%20};
	adventurer.CurrentEquip.Vehicle		= {rand()%10	, rand()%5	, rand()%20};
	adventurer.CurrentEquip.Facility	= {rand()%5		, rand()%2	, rand()%20};
	adventurer.CurrentEquip.StageProp	= {0, 0, 0};

	for(uint32_t i=1; i<10; i++) {
		instanceGame.GameArmies.Player	.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
		instanceGame.GameArmies.Enemy	.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);

		klib::setupEnemy(instanceGame.GameArmies.Player[i-1], instanceGame.GameArmies.Player[i]	, rand()%klib::size(klib::enemyDefinitions));
		klib::setupEnemy(instanceGame.GameArmies.Player[i]	, instanceGame.GameArmies.Enemy[i]	, rand()%klib::size(klib::enemyDefinitions));

		instanceGame.GameArmies.Player[i].CurrentEquip.Accessory	.Level	= rand()%50;
		instanceGame.GameArmies.Player[i].CurrentEquip.Armor		.Level	= rand()%50; 
		instanceGame.GameArmies.Player[i].CurrentEquip.Weapon		.Level	= rand()%50; 
		instanceGame.GameArmies.Player[i].CurrentEquip.Vehicle		.Level	= rand()%50; 
		instanceGame.GameArmies.Player[i].CurrentEquip.Facility		.Level	= rand()%50; 
		instanceGame.GameArmies.Player[i].CurrentEquip.Profession	.Level	= rand()%50; 
		instanceGame.GameArmies.Player[i].CurrentEquip.StageProp	.Level	= rand()%50; 
	}

	for(uint32_t i=0; i<4; i++) {
		instanceGame.PlayerSquads	.Agents[0] = 3;
		instanceGame.PlayerSquads	.Agents[1] = 2;
		instanceGame.PlayerSquads	.Agents[2] = 1;
		instanceGame.PlayerSquads	.Agents[3] = 0;
		instanceGame.EnemySquads	.Agents[0] = 3;
		instanceGame.EnemySquads	.Agents[1] = 2;
		instanceGame.EnemySquads	.Agents[2] = 1;
		instanceGame.EnemySquads	.Agents[3] = 0;
	}

};
