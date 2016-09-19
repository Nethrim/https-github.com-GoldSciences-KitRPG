#define NOMINMAX

#include "Game.h"

#include "Enemy.h"
#include "Accessory.h"
#include "Armor.h"
#include "Weapon.h"
#include "Profession.h"
#include "Vehicle.h"
#include "Facility.h"
#include "StageProp.h"
#include "Item.h"

#include <ctime>

using namespace klib;


void prompt(std::string& userInput, const std::string& displayText)
{
	ktools::clearASCIIBackBuffer(' ', 5);

	// Set up a nice prompt 
	uint32_t screenWidth  =	ktools::getASCIIBackBufferWidth(),
			 screenHeight =	ktools::getASCIIBackBufferHeight();

	klib::lineToRect(ktools::getASCIIBackBuffer(), screenWidth, screenHeight, (screenHeight>>1)-1, 0, ktools::CENTER, displayText.c_str());

	ktools::presentASCIIBackBuffer();

	static const HANDLE hConsoleOut	= GetStdHandle( STD_OUTPUT_HANDLE );
	COORD cursorPos = {((SHORT)screenWidth>>1)-5, (SHORT)screenHeight>>1};
	SetConsoleCursorPosition( hConsoleOut, cursorPos );
	SetConsoleTextAttribute(hConsoleOut, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	SetConsoleDisplayMode(hConsoleOut, CONSOLE_FULLSCREEN_MODE, 0);
	static const HANDLE hConsoleIn	= GetStdHandle( STD_INPUT_HANDLE );
	FlushConsoleInputBuffer(hConsoleIn);
	getline(std::cin, userInput);
}

void klib::resetGame(SGame& instanceGame)
{
	initGame(instanceGame);

	ktools::clearASCIIBackBuffer(' ', 5);

	// Set up a nice prompt 
	std::string playerName;
	prompt(playerName, "Enter your name:");
	std::string password;
	prompt(password, "Enter password:");

	instanceGame.Players[PLAYER_USER].Name = playerName;
	gbit_set(instanceGame.Flags, GAME_FLAGS_STARTED);
	gbit_clear(instanceGame.Flags, GAME_FLAGS_TACTICAL);
}

// Sets up initial equipment and items for the player to carry or wear.
void klib::initGame(SGame& instanceGame)
{
	srand((int32_t)time(0));

	gbit_clear(instanceGame.Flags, GAME_FLAGS_STARTED	);
	gbit_clear(instanceGame.Flags, GAME_FLAGS_TACTICAL	);

	instanceGame.Seed = time(0);

	resetCursorString(instanceGame.SlowMessage);

	SPlayer& player	= instanceGame.Players[PLAYER_USER ]	= SPlayer();
	SPlayer& enemy	= instanceGame.Players[PLAYER_ENEMY]	= SPlayer();

	player	.Army.push_back(klib::enemyDefinitions[1+rand()%(ktools::size(klib::enemyDefinitions)-1)]);
	enemy	.Army.push_back(klib::enemyDefinitions[1+rand()%(ktools::size(klib::enemyDefinitions)-1)]);

	instanceGame.Antagonisms[PLAYER_USER ]	= PLAYER_ANTAGONISM_USER	;
	instanceGame.Antagonisms[PLAYER_ENEMY]	= PLAYER_ANTAGONISM_ENEMY	;
	
	klib::CCharacter& adventurer		= player.Army[0];
	adventurer.CurrentEquip.Weapon		= {25, (int16_t)(rand()%ktools::size(klib::modifiersWeapon		)), 20};
	adventurer.CurrentEquip.Accessory	= {(int16_t)(rand()%ktools::size(klib::definitionsAccessory		)), (int16_t)(rand()%ktools::size(klib::modifiersAccessory	)), 1+rand()%10};
	adventurer.CurrentEquip.Armor		= {(int16_t)(rand()%ktools::size(klib::definitionsArmor			)), (int16_t)(rand()%ktools::size(klib::modifiersArmor		)), 1+rand()%10};
	adventurer.CurrentEquip.Profession	= {(int16_t)(rand()%ktools::size(klib::definitionsProfession	)), (int16_t)(rand()%ktools::size(klib::modifiersProfession	)), 1+rand()%10};
	adventurer.CurrentEquip.Vehicle		= {(int16_t)(rand()%ktools::size(klib::definitionsVehicle		)), (int16_t)(rand()%ktools::size(klib::modifiersVehicle	)), 1+rand()%10};
	adventurer.CurrentEquip.Facility	= {(int16_t)(rand()%ktools::size(klib::definitionsFacility		)), (int16_t)(rand()%ktools::size(klib::modifiersFacility	)), 1+rand()%10};
	adventurer.CurrentEquip.StageProp	= {(int16_t)(rand()%ktools::size(klib::definitionsStageProp		)), (int16_t)(rand()%ktools::size(klib::modifiersStageProp	)), 1+rand()%10};

	for(uint32_t i=1; i<8; i++) 
	{
		player	.Army.push_back(klib::enemyDefinitions[1+rand()%(ktools::size(klib::enemyDefinitions)-1)]);
		enemy	.Army.push_back(klib::enemyDefinitions[1+rand()%(ktools::size(klib::enemyDefinitions)-1)]);

		klib::setupAgent(player.Army[i-1]	, player	.Army[i]	, rand()%ktools::size(klib::enemyDefinitions), 0);
		klib::setupAgent(player.Army[i]		, enemy		.Army[i]	, rand()%ktools::size(klib::enemyDefinitions), 1);

		player.Army[i].CurrentEquip.Accessory	.Level	= 1+rand()%10;
		player.Army[i].CurrentEquip.Armor		.Level	= 1+rand()%10; 
		player.Army[i].CurrentEquip.Weapon		.Level	= 1+rand()%10; 
		player.Army[i].CurrentEquip.Vehicle		.Level	= 1+rand()%10; 
		player.Army[i].CurrentEquip.Facility	.Level	= 1+rand()%10; 
		player.Army[i].CurrentEquip.Profession	.Level	= 1+rand()%10; 
		player.Army[i].CurrentEquip.StageProp	.Level	= 1+rand()%10; 
	}

	player	.Squad.Clear(-1);
	enemy	.Squad.Clear(-1);

	for(uint32_t i=0; i<4; i++) {
		player	.Squad.Agents[i] = 3-i;
		enemy	.Squad.Agents[i] = i;
	}

	instanceGame.TacticalDisplay	.Clear();
	instanceGame.PostEffectDisplay	.Clear();
	instanceGame.GlobalDisplay		.Clear();
	instanceGame.TacticalInfo		.Clear();

	initTacticalMap(instanceGame);

	gbit_set(instanceGame.Flags, GAME_FLAGS_RUNNING	);

};
