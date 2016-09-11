#define NOMINMAX

#include "Game.h"

#include "Enemy.h"

#include <ctime>

using namespace klib;

void klib::initTacticalMap(SGame& instanceGame)
{
	const uint32_t	terrainWidth = instanceGame.TacticalInfo.Board.Terrain.Topology.Width, 
					terrainDepth = instanceGame.TacticalInfo.Board.Terrain.Topology.Depth;

	instanceGame.TacticalInfo.Board.Clear();
	fillCellsFromNoise(instanceGame.TacticalInfo.Board.Terrain.Topology, {1,0},	(int32_t)instanceGame.Seed+0, {0, 0}, 50);
	fillCellsFromNoise(instanceGame.TacticalInfo.Board.Terrain.Topology, {0,1},	(int32_t)instanceGame.Seed+3, {0, 0}, 50);
	fillCellsFromNoise(instanceGame.TacticalInfo.Board.Terrain.Topology, {0,10}, (int32_t)instanceGame.Seed+6, {0, 0}, 50);
	fillCellsFromNoise(instanceGame.TacticalInfo.Board.Terrain.Topology, {0,25}, (int32_t)instanceGame.Seed+9, {0, 0}, 50);
	//fillCellsFromNoise(instanceGame.TacticalBoard.Terrain.Topology, {0,0}, (int32_t)instanceGame.Seed+5, {0, 0});
	
	int8_t*				cellsCollision	= &instanceGame.TacticalInfo.Board.Terrain.Collision.Cells[0][0];
	STopologyHeight*	cellsHeight		= &instanceGame.TacticalInfo.Board.Terrain.Topology.Cells[0][0];
	for(uint32_t i=0, count = terrainDepth*terrainWidth; i<count; i++) {
		cellsCollision[i] = cellsHeight[i].Sharp + cellsHeight[i].Smooth;
	};
}

void klib::resetGame(SGame& instanceGame)
{
	initGame(instanceGame);
	klib::clearASCIIBackBuffer(' ', 5);
	instanceGame.ClearDisplays();
	instanceGame.TacticalInfo.Board.Clear();

	// Set up a nice prompt 
	uint32_t screenWidth  =	klib::getASCIIBackBufferWidth(),
			 screenHeight =	klib::getASCIIBackBufferHeight();

	klib::lineToRect(getASCIIBackBuffer(), screenWidth, screenHeight, (screenHeight>>1)-1, 0, klib::CENTER, "Enter your name:");

	klib::presentASCIIBackBuffer();

	static const HANDLE hConsoleOut	= GetStdHandle( STD_OUTPUT_HANDLE );
	COORD cursorPos = {((SHORT)screenWidth>>1)-5, (SHORT)screenHeight>>1};
	SetConsoleCursorPosition( hConsoleOut, cursorPos );
	SetConsoleTextAttribute(hConsoleOut, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	SetConsoleDisplayMode(hConsoleOut, CONSOLE_FULLSCREEN_MODE, 0);
	static const HANDLE hConsoleIn	= GetStdHandle( STD_INPUT_HANDLE );
	FlushConsoleInputBuffer(hConsoleIn);
	getline(std::cin, instanceGame.Player.Name);

	instanceGame.bStarted = true;
}

// Sets up initial equipment and items for the player to carry or wear.
void klib::initGame(SGame& instanceGame)
{
	srand((int32_t)time(0));

	instanceGame.bStarted = false;

	instanceGame.Seed = rand();

	initTacticalMap(instanceGame);

	instanceGame.TacticalDisplay	.Clear();
	instanceGame.PostEffectDisplay	.Clear();
	instanceGame.GlobalDisplay		.Clear();

	fillCellsFromNoise(instanceGame.PostEffectDisplay.Screen, (char)19, instanceGame.Seed);
	drawGridBorders(instanceGame.PostEffectDisplay.Screen, '@');
	
	resetCursorString(instanceGame.SlowMessage);

	instanceGame.Player	= SPlayer();
	instanceGame.Enemy	= SPlayer();

	instanceGame.Player	.Army.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	instanceGame.Enemy	.Army.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	
	klib::CCharacter& adventurer		= instanceGame.Player.Army[0];
	adventurer.CurrentEquip.Weapon		= {rand()%20	, rand()%11	, 1+rand()%10};
	adventurer.CurrentEquip.Accessory	= {rand()%5		, rand()%10	, 1+rand()%10};
	adventurer.CurrentEquip.Armor		= {rand()%10	, rand()%2	, 1+rand()%10};
	adventurer.CurrentEquip.Profession	= {rand()%10	, rand()%8	, 1+rand()%10};
	adventurer.CurrentEquip.Vehicle		= {rand()%10	, rand()%5	, 1+rand()%10};
	adventurer.CurrentEquip.Facility	= {rand()%5		, rand()%2	, 1+rand()%10};
	adventurer.CurrentEquip.StageProp	= {0, 0, 0};

	for(uint32_t i=1; i<8; i++) 
	{
		instanceGame.Player	.Army.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
		instanceGame.Enemy	.Army.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);

		klib::setupAgent(instanceGame.Player.Army[i-1]	, instanceGame.Player	.Army[i]	, rand()%klib::size(klib::enemyDefinitions), 0);
		klib::setupAgent(instanceGame.Player.Army[i]	, instanceGame.Enemy	.Army[i]	, rand()%klib::size(klib::enemyDefinitions), 1);

		instanceGame.Player.Army[i].CurrentEquip.Accessory	.Level	= 1+rand()%10;
		instanceGame.Player.Army[i].CurrentEquip.Armor		.Level	= 1+rand()%10; 
		instanceGame.Player.Army[i].CurrentEquip.Weapon		.Level	= 1+rand()%10; 
		instanceGame.Player.Army[i].CurrentEquip.Vehicle	.Level	= 1+rand()%10; 
		instanceGame.Player.Army[i].CurrentEquip.Facility	.Level	= 1+rand()%10; 
		instanceGame.Player.Army[i].CurrentEquip.Profession	.Level	= 1+rand()%10; 
		instanceGame.Player.Army[i].CurrentEquip.StageProp	.Level	= 1+rand()%10; 
	}

	for(uint32_t i=0; i<4; i++) {
		instanceGame.Player	.Squad.Agents[i] = 3-i;
		instanceGame.Enemy	.Squad.Agents[i] = i;
	}

};
