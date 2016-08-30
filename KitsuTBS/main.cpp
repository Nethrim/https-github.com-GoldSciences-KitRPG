#include "Game.h"

#include "menus.h"
#include "Enemy.h"


#define NOMINMAX

#include <crtdbg.h>
#include <Windows.h>

#include <algorithm>
#include <ctime>

// Sets up initial equipment and items for the player to carry or wear.
void initGame(SGame& instanceGame)
{
	srand((int32_t)time(0));

	instanceGame.bStarted = true;

	int32_t seed = rand();

	fillDisplayValueFromNoise(instanceGame.TacticalDisplay, 19, seed);
	drawDisplayBorders(instanceGame.TacticalDisplay, '@');
	
	instanceGame.SlowMessage[0] = '_';
	instanceGame.SlowMessage[1] = 0;

	uint32_t screenWidth  =	game::getASCIIBackBufferWidth(),
			 screenHeight =	game::getASCIIBackBufferHeight();
	game::clearASCIIBackBuffer(' ');
	game::lineToScreen(screenHeight/2-1, 0, game::CENTER, "Enter your name:");

	static const HANDLE hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD cursorPos = {(SHORT)screenWidth/2-5, (SHORT)screenHeight/2};
	SetConsoleCursorPosition( hConsoleOut, cursorPos );

	game::presentASCIIBackBuffer();


	getline(std::cin, instanceGame.PlayerName);

	instanceGame.PlayerArmy.clear();
	instanceGame.PlayerSquad.clear();

	instanceGame.PlayerArmy.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	instanceGame.PlayerArmy.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);

	instanceGame.PlayerSquad.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	instanceGame.PlayerSquad.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	
	instanceGame.EnemyArmy.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	instanceGame.EnemyArmy.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);

	instanceGame.EnemyArmy.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	instanceGame.EnemyArmy.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
};


void drawAndPresentGame( SGame& instanceGame )
{
	static STimer frameMeasure;

	game::clearASCIIBackBuffer( ' ' );

	game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::RIGHT, "%s.", instanceGame.UserMessage.c_str());
	game::lineToScreen(game::getASCIIBackBufferHeight()-2, 1, game::LEFT, "sizeof(SGame): %u.", sizeof(SGame));

	drawState(instanceGame);
	showMenu(instanceGame);

	frameMeasure.Frame();
	instanceGame.FrameTimer.Frame();
	game::lineToScreen(1, 1, game::LEFT, "Frame time: %.5f seconds.", instanceGame.FrameTimer.LastTimeSeconds);
	game::lineToScreen(2, 1, game::LEFT, "Frames last second: %f.", instanceGame.FrameTimer.FramesLastSecond);

	
	game::presentASCIIBackBuffer();
}; 	// 

// Use this function to draw our game data
void draw( SGame& instanceGame ) // 
{
 	drawAndPresentGame(instanceGame);
};


void options(SGame& gameInstance)
{
}

int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
	int tmp = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF);
	// Clear the upper 16 bits and OR in the desired freqency
	tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;
	// Set the new bits
	_CrtSetDbgFlag(tmp);
#endif

	game::initASCIIScreen();
	
	SGame* pInstancedGame = new SGame;
	SGame& instanceGame = *pInstancedGame;

	fillDisplayValueFromNoise(instanceGame.TacticalDisplay, 19, rand());
	drawDisplayBorders(instanceGame.TacticalDisplay, '@');

	while(instanceGame.bRunning)
	{
		pollInput(instanceGame.FrameInput);
		draw(instanceGame);
	}

	if(pInstancedGame)
		delete(pInstancedGame);

	return 0;
}
