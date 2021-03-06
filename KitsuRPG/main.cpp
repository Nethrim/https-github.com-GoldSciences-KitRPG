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

#include "GameMenu.h"

#include <iostream>
#include <algorithm>
#include <time.h>

// Sets up initial equipment and items for the player to carry or wear.
void initPlayerCharacter(klib::CCharacter& adventurer, const std::string& name);

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
	std::string adventurerName;
	getline(std::cin, adventurerName);

	klib::CCharacter* pAdventurer = new klib::CCharacter(klib::CHARACTER_TYPE_PLAYER,	4, 50, 1, 100, {1,4}, {}, {klib::COMBAT_STATUS_NONE, klib::COMBAT_STATUS_STUN}, adventurerName);
	klib::CCharacter& adventurer = *pAdventurer;

	static const klib::SMenuItem<int32_t> playAgainMenu[]
	{	{ 1,	"Yes"	}
	,	{ 0,	"No"	}
	};

	bool bPlayAgain = true;

	while(bPlayAgain)
	{
		initPlayerCharacter(adventurer, adventurerName);

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
void createPlayerCharacter(klib::CCharacter& adventurer, const std::string& name)
{
	klib::CCharacter* tempadventurer = new klib::CCharacter(klib::CHARACTER_TYPE_PLAYER, 4, 50, 1, 100, {1,4}, {}, {klib::COMBAT_STATUS_NONE, klib::COMBAT_STATUS_STUN}, name);
	adventurer = *tempadventurer;
	delete(tempadventurer);
}

//
void initPlayerCharacter(klib::CCharacter& adventurer, const std::string& name)
{
	createPlayerCharacter(adventurer, name);
	klib::SCharacterEquip& currentEquip = adventurer.CurrentEquip;
	klib::SCharacterEquip& maxEquip		= adventurer.MaxResearch;
#if defined(POWER_START)
	currentEquip.Weapon		.Index		= (int16_t)std::min(3U, size(klib::definitionsWeapon		)-1);
	currentEquip.Accessory	.Index		= (int16_t)std::min(3U, size(klib::definitionsAccessory		)-1);
	currentEquip.Armor		.Index		= (int16_t)std::min(3U, size(klib::definitionsArmor			)-1);
	currentEquip.Profession	.Index		= (int16_t)std::min(3U, size(klib::definitionsProfession	)-1);
	currentEquip.Vehicle	.Index		= (int16_t)std::min(3U, size(klib::definitionsVehicle		)-1);
	currentEquip.Facility	.Index		= (int16_t)std::min(3U, size(klib::definitionsFacility	)-1);

	currentEquip.Weapon		.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersWeapon			)-1);
	currentEquip.Accessory	.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersAccessory		)-1);
	currentEquip.Armor		.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersArmor			)-1);
	currentEquip.Profession	.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersProfession		)-1);
	currentEquip.Vehicle	.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersVehicle			)-1);
	currentEquip.Facility	.Modifier	= (int16_t)std::min(3U,	size(klib::modifiersFacility		)-1);

	currentEquip.Weapon		.Level		= 10;
	currentEquip.Accessory	.Level		= 10;
	currentEquip.Armor		.Level		= 10;
	currentEquip.Profession	.Level		= 10;
	currentEquip.Vehicle	.Level		= 10;
	currentEquip.Facility	.Level		= 10;
#else
	memset(&currentEquip, 0, sizeof(klib::SCharacterEquip));
	memset(&maxEquip	, 0, sizeof(klib::SCharacterEquip));
#endif

	adventurer.Goods.Inventory.Weapon		.AddElement({1,1,1});
	adventurer.Goods.Inventory.Accessory	.AddElement({1,1,1});
	adventurer.Goods.Inventory.Armor		.AddElement({1,1,1});
	adventurer.Goods.Inventory.Profession	.AddElement({adventurer.CurrentEquip.Profession.Definition,1,10});
	adventurer.Goods.Inventory.Vehicle		.AddElement({1,1,1});
	adventurer.Goods.Inventory.Facility		.AddElement({1,1,1});

	int16_t i = 5;
	adventurer.Goods.Inventory.Weapon		.AddElement(	{	rand()%(int16_t)ktools::size(klib::definitionsWeapon		), rand()%(int16_t)ktools::size(klib::modifiersWeapon		), ++i	});
	adventurer.Goods.Inventory.Accessory	.AddElement(	{	rand()%(int16_t)ktools::size(klib::definitionsAccessory		), rand()%(int16_t)ktools::size(klib::modifiersAccessory	), ++i	});
	adventurer.Goods.Inventory.Armor		.AddElement(	{	rand()%(int16_t)ktools::size(klib::definitionsArmor			), rand()%(int16_t)ktools::size(klib::modifiersArmor		), ++i	});
	adventurer.Goods.Inventory.Profession	.AddElement(	{	rand()%(int16_t)ktools::size(klib::definitionsProfession	), rand()%(int16_t)ktools::size(klib::modifiersProfession	), ++i	});
	adventurer.Goods.Inventory.Vehicle		.AddElement(	{	rand()%(int16_t)ktools::size(klib::definitionsVehicle		), rand()%(int16_t)ktools::size(klib::modifiersVehicle		), ++i	});
	adventurer.Goods.Inventory.Facility		.AddElement(	{	rand()%(int16_t)ktools::size(klib::definitionsFacility		), rand()%(int16_t)ktools::size(klib::modifiersFacility	), ++i	});

	adventurer.Goods.Inventory.Items		.AddElement({1,1,1});
	for(int32_t i=1; i<3; ++i)
		adventurer.Goods.Inventory.Items	.AddElement({ 1+int16_t(rand()%(ktools::size(klib::itemDescriptions)-1)), int16_t(1+rand()%ktools::size(klib::itemModifiers)), int16_t(rand()%ktools::size(klib::itemGrades)) });

	const klib::SEntityPoints finalPoints	= klib::calculateFinalPoints(adventurer);
	adventurer.Points.LifeCurrent			= finalPoints.LifeMax;
}
