#define NOMINMAX

#include "Game.h"
#include "Armor.h"
#include "Weapon.h"
#include "Profession.h"
#include "Enemy.h"
#include "Item.h"
#include "Vehicle.h"
#include "Accessory.h"
#include "Facility.h"

#include "Menu.h"

#include <iostream>
#include <algorithm>
#include <time.h>

// Sets up initial equipment and items for the player to carry or wear.
void initPlayerCharacter(klib::CCharacter& adventurer);

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

	printf("Welcome Stranger!! who are you?\n");
	printf("My name is: \n");
	std::string Name;
	getline(std::cin, Name);

	klib::CCharacter* pAdventurer = new klib::CCharacter(klib::CHARACTER_TYPE_PLAYER,	4, 50, 1, 100, {}, {klib::COMBAT_STATUS_NONE, klib::COMBAT_STATUS_STUN}, Name);
	klib::CCharacter& adventurer = *pAdventurer;

	static const klib::SMenuItem<int32_t> playAgainMenu[]
	{	{ 1,	"Yes"	}
	,	{ 0,	"No"	}
	};

	bool bPlayAgain = true;

	while(bPlayAgain)
	{
		initPlayerCharacter(adventurer);

		std::cout << "\nSo, " << adventurer.Name << "... What brings you here?\n";
		tavern(adventurer);	// Tavern is the main menu of our game.

		printf("\n-- Game Over! --\n");
		displayScore(adventurer.Score);

		bPlayAgain = displayMenu("Play again? ..", playAgainMenu) ? true : false;
	}

	if( pAdventurer )
		delete(pAdventurer);
}

//
void initPlayerCharacter(klib::CCharacter& adventurer)
{
	adventurer.CurrentEquip.Weapon		.Index		= (int16_t)std::min(3U, size(klib::definitionsWeapon		)-1);
	adventurer.CurrentEquip.Accessory	.Index		= (int16_t)std::min(3U, size(klib::definitionsAccessory	)-1);
	adventurer.CurrentEquip.Armor		.Index		= (int16_t)std::min(3U, size(klib::definitionsArmor		)-1);
	adventurer.CurrentEquip.Profession	.Index		= (int16_t)std::min(3U, size(klib::definitionsProfession	)-1);
	adventurer.CurrentEquip.Vehicle		.Index		= (int16_t)std::min(3U, size(klib::definitionsVehicle		)-1);
	adventurer.CurrentEquip.Facility	.Index		= (int16_t)std::min(3U, size(klib::definitionsFacilities	)-1);

	adventurer.CurrentEquip.Weapon		.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersWeapon		)-1);
	adventurer.CurrentEquip.Accessory	.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersAccessory	)-1);
	adventurer.CurrentEquip.Armor		.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersArmor		)-1);
	adventurer.CurrentEquip.Profession	.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersProfession	)-1);
	adventurer.CurrentEquip.Vehicle		.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersVehicle		)-1);
	adventurer.CurrentEquip.Facility	.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersFacilities	)-1);

	adventurer.CurrentEquip.Weapon		.Level		= 10;
	adventurer.CurrentEquip.Accessory	.Level		= 10;
	adventurer.CurrentEquip.Armor		.Level		= 10;
	adventurer.CurrentEquip.Profession	.Level		= 10;
	adventurer.CurrentEquip.Vehicle		.Level		= 10;
	adventurer.CurrentEquip.Facility	.Level		= 10;

	adventurer.MaxEquip.Weapon			.Index		= adventurer.CurrentEquip.Weapon		.Index;		
	adventurer.MaxEquip.Armor			.Index		= adventurer.CurrentEquip.Armor			.Index;		
	adventurer.MaxEquip.Accessory		.Index		= adventurer.CurrentEquip.Accessory		.Index;
	adventurer.MaxEquip.Profession		.Index		= adventurer.CurrentEquip.Profession	.Index;		
	adventurer.MaxEquip.Vehicle			.Index		= adventurer.CurrentEquip.Vehicle		.Index;
	adventurer.MaxEquip.Facility		.Index		= adventurer.CurrentEquip.Facility		.Index;

	adventurer.MaxEquip.Weapon			.Modifier	= adventurer.CurrentEquip.Weapon		.Modifier;		
	adventurer.MaxEquip.Armor			.Modifier	= adventurer.CurrentEquip.Armor			.Modifier;		
	adventurer.MaxEquip.Accessory		.Modifier	= adventurer.CurrentEquip.Accessory		.Modifier;
	adventurer.MaxEquip.Profession		.Modifier	= adventurer.CurrentEquip.Profession	.Modifier;		
	adventurer.MaxEquip.Vehicle			.Modifier	= adventurer.CurrentEquip.Vehicle		.Modifier;
	adventurer.MaxEquip.Facility		.Modifier	= adventurer.CurrentEquip.Facility		.Modifier;

	adventurer.MaxEquip.Weapon			.Level		= adventurer.CurrentEquip.Weapon		.Level;		
	adventurer.MaxEquip.Armor			.Level		= adventurer.CurrentEquip.Armor			.Level;		
	adventurer.MaxEquip.Accessory		.Level		= adventurer.CurrentEquip.Accessory		.Level;
	adventurer.MaxEquip.Profession		.Level		= adventurer.CurrentEquip.Profession	.Level;		
	adventurer.MaxEquip.Vehicle			.Level		= adventurer.CurrentEquip.Vehicle		.Level;
	adventurer.MaxEquip.Facility		.Level		= adventurer.CurrentEquip.Facility		.Level;

	adventurer.Inventory.Weapons		.AddElement({1,1,1});
	adventurer.Inventory.Accessories	.AddElement({1,1,1});
	adventurer.Inventory.Armors			.AddElement({1,1,1});
	adventurer.Inventory.Professions	.AddElement({adventurer.CurrentEquip.Profession.Index,1,20});
	adventurer.Inventory.Vehicles		.AddElement({1,1,1});
	adventurer.Inventory.Facilities		.AddElement({1,1,1});

	adventurer.Inventory.Weapons		.AddElement({2,2,2});
	adventurer.Inventory.Accessories	.AddElement({2,2,2});
	adventurer.Inventory.Armors			.AddElement({2,2,2});
	adventurer.Inventory.Professions	.AddElement({3,4,1});
	adventurer.Inventory.Vehicles		.AddElement({2,2,2});
	adventurer.Inventory.Facilities		.AddElement({2,2,2});

	int16_t i = 5;
	adventurer.Inventory.Weapons		.AddElement({	std::min(++i, (int16_t)(size(klib::definitionsWeapon		)-1)), std::min(++i, (int16_t)(size(klib::modifiersWeapon		)-1)), ++i});
	adventurer.Inventory.Accessories	.AddElement({	std::min(++i, (int16_t)(size(klib::definitionsAccessory		)-1)), std::min(++i, (int16_t)(size(klib::modifiersAccessory	)-1)), ++i});
	adventurer.Inventory.Armors			.AddElement({	std::min(++i, (int16_t)(size(klib::definitionsArmor			)-1)), std::min(++i, (int16_t)(size(klib::modifiersArmor		)-1)), ++i});
	adventurer.Inventory.Professions	.AddElement({	std::min(++i, (int16_t)(size(klib::definitionsProfession	)-1)), std::min(++i, (int16_t)(size(klib::modifiersProfession	)-1)), ++i});
	adventurer.Inventory.Vehicles		.AddElement({	std::min(++i, (int16_t)(size(klib::definitionsVehicle		)-1)), std::min(++i, (int16_t)(size(klib::modifiersVehicle		)-1)), ++i});
	adventurer.Inventory.Facilities		.AddElement({	std::min(++i, (int16_t)(size(klib::definitionsFacilities	)-1)), std::min(++i, (int16_t)(size(klib::modifiersFacilities	)-1)), ++i});

	adventurer.Inventory.Items			.AddElement({1,1,1});
	for(int32_t i=1; i<3; ++i)
		adventurer.Inventory.Items.AddElement({ 1+int16_t(rand()%(klib::size(klib::itemDescriptions)-1)), int16_t(1+rand()%klib::size(klib::itemModifiers)), int16_t(rand()%klib::size(klib::itemGrades)) });

	const klib::SEntityPoints finalPoints	= klib::calculateFinalPoints(adventurer);
	adventurer.Points.LifeCurrent			= finalPoints.LifeMax;
}
