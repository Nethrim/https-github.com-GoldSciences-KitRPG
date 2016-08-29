#include "Game.h"

#include "menus.h"
#include "Enemy.h"

#define NOMINMAX

#include <crtdbg.h>
#include <Windows.h>

#include <algorithm>
#include <ctime>


// Sets up initial equipment and items for the player to carry or wear.
void initPlayerCharacters(klib::CCharacter& adventurer, const std::string& name);


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
void draw( SGame& instancedGame ) // 
{
 	drawAndPresentGame(instancedGame);
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
	SGame& instancedGame = *pInstancedGame;
	srand((int32_t)time(0));

	int32_t seed = rand();

	fillDisplayValueFromNoise(instancedGame.TacticalDisplay, 19, seed);
	drawDisplayBorders(instancedGame.TacticalDisplay, '@');
	
	instancedGame.PlayerArmy.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	instancedGame.PlayerArmy.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);

	instancedGame.PlayerSquad.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);
	instancedGame.PlayerSquad.push_back(klib::enemyDefinitions[1+rand()%(klib::size(klib::enemyDefinitions)-1)]);

	while(instancedGame.bRunning)
	{
		pollInput(instancedGame.FrameInput);
		//processMenuInput(instancedGame, instancedGame.FrameInput);
		draw(instancedGame);
	}

	if(pInstancedGame)
		delete(pInstancedGame);

	return 0;
}

//void processInput(SInput& input, SGame& gameInstance)
//{
//}
