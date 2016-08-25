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

		adventurer.CurrentEquip.Weapon		.Index		= 0;
		adventurer.CurrentEquip.Armor		.Index		= 0;
		adventurer.CurrentEquip.Profession	.Index		= 0;
		adventurer.CurrentEquip.Vehicle		.Index		= 0;

		adventurer.CurrentEquip.Weapon		.Modifier	= 0;
		adventurer.CurrentEquip.Armor		.Modifier	= 0;
		adventurer.CurrentEquip.Profession	.Modifier	= 0;
		adventurer.CurrentEquip.Vehicle		.Modifier	= 0;

		adventurer.CurrentEquip.Weapon		.Level		= 1;
		adventurer.CurrentEquip.Armor		.Level		= 1;
		adventurer.CurrentEquip.Profession	.Level		= 1;
		adventurer.CurrentEquip.Vehicle		.Level		= 1;

		adventurer.MaxEquip.Weapon			.Index		= adventurer.CurrentEquip.Weapon		.Index;		
		adventurer.MaxEquip.Armor			.Index		= adventurer.CurrentEquip.Armor			.Index;		
		adventurer.MaxEquip.Profession		.Index		= adventurer.CurrentEquip.Profession	.Index;		
		adventurer.MaxEquip.Vehicle			.Index		= adventurer.CurrentEquip.Vehicle		.Index;

		adventurer.MaxEquip.Weapon			.Modifier	= adventurer.CurrentEquip.Weapon		.Modifier;
		adventurer.MaxEquip.Armor			.Modifier	= adventurer.CurrentEquip.Armor			.Modifier;
		adventurer.MaxEquip.Profession		.Modifier	= adventurer.CurrentEquip.Profession	.Modifier;
		adventurer.MaxEquip.Vehicle			.Modifier	= adventurer.CurrentEquip.Vehicle		.Modifier;

		adventurer.MaxEquip.Weapon			.Level		= adventurer.CurrentEquip.Weapon		.Level;
		adventurer.MaxEquip.Armor			.Level		= adventurer.CurrentEquip.Armor			.Level;
		adventurer.MaxEquip.Profession		.Level		= adventurer.CurrentEquip.Profession	.Level;
		adventurer.MaxEquip.Vehicle			.Level		= adventurer.CurrentEquip.Vehicle		.Level;

		adventurer.Inventory.Weapons		.AddElement({1,1,1});
		adventurer.Inventory.Armors			.AddElement({1,1,1});
		adventurer.Inventory.Professions	.AddElement({adventurer.CurrentEquip.Profession.Index,1,1});
		adventurer.Inventory.Vehicles		.AddElement({1,1,1});

		adventurer.Inventory.Items			.AddElement({1,1,1});
		for(int32_t i=1; i<3; ++i)
			adventurer.Inventory.Items.AddElement({ 1+int16_t(rand()%(klib::size(klib::itemDescriptions)-1)), int16_t(1+rand()%klib::size(klib::itemModifiers)), int16_t(rand()%klib::size(klib::itemGrades)) });

		klib::SEntityPoints finalPoints		= klib::calculateFinalPoints(adventurer);
		adventurer.Points.LifeCurrent		= finalPoints.LifeMax;

		std::cout << "\nSo, " << adventurer.Name << "... What brings you here?\n";
		tavern(adventurer);	// Tavern is the main menu of our game.

		printf("\n-- Game Over! --\n");
		displayScore(adventurer.Score);

		bPlayAgain = displayMenu("Play again? ..", playAgainMenu) ? true : false;
	}
}
