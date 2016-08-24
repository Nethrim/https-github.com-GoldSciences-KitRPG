#define NOMINMAX

#include "Game.h"
#include "Armor.h"
#include "Weapon.h"
#include "Profession.h"
#include "Enemy.h"
#include "Item.h"
#include "Vehicle.h"

#include "Menu.h"

#include <iostream>
#include <time.h>

// This function seeds the rand() and enters the tavern() after initializing the player.
// If the player leaves the tavern() it means the game was requested to close. 
// After leaving the tavern() we display the score of the player.
void main()
{	
#if defined(DEBUG) || defined(_DEBUG)
	int tmp = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF);
	// Clear the upper 16 bits and OR in the desired freqency
	tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;

	// Set the new bits
	_CrtSetDbgFlag(tmp);
#endif

	srand((unsigned int)time(NULL));

	klib::CCharacter adventurer;	// This class instance will contain all our game data.

	printf("Welcome Stranger!! who are you?\n");
	printf("My name is: \n");
	std::string Name;
	getline(std::cin, Name);

	static const klib::SMenuItem<int32_t> playAgainMenu[]
	{	{ 1,	"Yes"	}
	,	{ 0,	"No"	}
	};

	bool bPlayAgain = true;
	while(bPlayAgain)
	{
		adventurer							= klib::CCharacter(klib::CHARACTER_TYPE_PLAYER,	4, 50, 1, 100, {}, {klib::COMBAT_STATUS_NONE, klib::COMBAT_STATUS_STUN}, Name);

		adventurer.	CurrentWeapon		.Index		= 0;
		adventurer.	CurrentArmor		.Index		= 0;
		adventurer.	CurrentProfession	.Index		= 0;//(int16_t)(rand() % size(klib::definitionsProfession	));
		adventurer.	CurrentVehicle		.Index		= (int16_t)(rand() % size(klib::definitionsVehicle	));
		adventurer.	CurrentWeapon		.Modifier	= 0;
		adventurer.	CurrentArmor		.Modifier	= 0;
		adventurer.	CurrentProfession	.Modifier	= 0;
		adventurer.	CurrentVehicle		.Modifier	= 0;
		adventurer.	CurrentWeapon		.Level		= 1;
		adventurer.	CurrentArmor		.Level		= 1;
		adventurer.	CurrentProfession	.Level		= 1;
		adventurer.	CurrentVehicle		.Level		= 1;

		adventurer.MaxWeapon			.Index		= adventurer.CurrentWeapon		.Index;		
		adventurer.MaxArmor				.Index		= adventurer.CurrentArmor		.Index;		
		adventurer.MaxProfession		.Index		= adventurer.CurrentProfession	.Index;		
		adventurer.MaxWeapon			.Modifier	= adventurer.CurrentWeapon		.Modifier;
		adventurer.MaxArmor				.Modifier	= adventurer.CurrentArmor		.Modifier;
		adventurer.MaxProfession		.Modifier	= adventurer.CurrentProfession	.Modifier;
		adventurer.MaxWeapon			.Level		= adventurer.CurrentWeapon		.Level;
		adventurer.MaxArmor				.Level		= adventurer.CurrentArmor		.Level;
		adventurer.MaxProfession		.Level		= adventurer.CurrentProfession	.Level;

		adventurer.Weapons				.AddElement({1,1,1});
		adventurer.Armors				.AddElement({1,1,1});
		adventurer.Professions			.AddElement({adventurer.CurrentProfession.Index,1,1});
		adventurer.Vehicles				.AddElement({1,1,1});

		adventurer.Inventory			.AddElement({1,1,1});
		for(int32_t i=1; i<3; ++i)
			adventurer.Inventory.AddElement({ 1+int16_t(rand()%(klib::size(klib::itemDescriptions)-1)), int16_t(1+rand()%klib::size(klib::itemModifiers)), int16_t(rand()%klib::size(klib::itemGrades)) });

		klib::SCharacterPoints finalPoints		= klib::calculateFinalPoints(adventurer);
		adventurer.Points.Points.LifeCurrent	= finalPoints.Points.LifeMax;

		std::cout << "\nSo, " << adventurer.Name << "... What brings you here?\n";
		tavern(adventurer);	// Tavern is the main menu of our game.

		printf("\n-- Game Over! --\n");
		displayScore(adventurer.Score);

		bPlayAgain = displayMenu("Play again? ..", playAgainMenu) ? true : false;
	}
}
