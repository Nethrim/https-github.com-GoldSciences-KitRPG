#define NOMINMAX

#include "Game.h"
#include "Armor.h"
#include "Weapon.h"
#include "Profession.h"
#include "Enemy.h"

#include "Menu.h"

#include <iostream>
#include <time.h>

// This function seeds the rand() and enters the tavern() after initializing the player.
// If the player leaves the tavern() it means the game was requested to close. 
// After leaving the tavern() we display the score of the player.
void main()
{	
	srand((unsigned int)time(NULL));

	CCharacter adventurer;	// This class instance will contain all our game data.

	printf("Welcome Stranger!! who are you?\n");
	printf("My name is: \n");
	std::string Name;
	getline(std::cin, Name);

	static const SMenuItem<uint32_t> playAgainMenu[]
	{	{ 1,	"Yes"	}
	,	{ 0,	"No"	}
	};

	bool bPlayAgain = true;
	while(bPlayAgain)
	{
		adventurer							= CCharacter(CHARACTER_TYPE_PLAYER,	10, 50, 1, 100, Name);

		adventurer.Weapon		.Index		= (int16_t)(rand() % size(weaponDefinitions		));//rand()%4;
		adventurer.Armor		.Index		= (int16_t)(rand() % size(armorDefinitions		));//rand()%4;
		adventurer.Profession	.Index		= (int16_t)(rand() % size(professionDefinitions	));

		adventurer.Weapon		.Modifier	= (int16_t)(rand() % size(weaponModifiers		));//rand()%4;
		adventurer.Armor		.Modifier	= (int16_t)(rand() % size(armorModifiers		));//rand()%4;
		adventurer.Profession	.Modifier	= (int16_t)(rand() % size(professionModifiers	));	

		adventurer.Weapon		.Level		= 1+(rand()%2);//+(int16_t)(rand()%size(enemyDefinitions));
		adventurer.Armor		.Level		= 1+(rand()%2);//+(int16_t)(rand()%size(enemyDefinitions));
		adventurer.Profession	.Level		= 1+(rand()%2);//+(int16_t)(rand()%size(enemyDefinitions));

		SCharacterPoints finalPoints			= calculateFinalPoints(adventurer);
		adventurer.Points.CurrentLife			= finalPoints.MaxLife;

		std::cout << "\nSo, " << adventurer.Name << "... What brings you here?\n";
		tavern(adventurer);	// Tavern is the main menu of our game.

		printf("\n-- Game Over! --\n");
		displayScore(adventurer.Score);

		bPlayAgain = displayMenu("Play again? ..", playAgainMenu) ? true : false;
	}
}
