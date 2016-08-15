#define NOMINMAX

#include "Game.h"
#include "Armor.h"

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

	static const SMenuItem playAgainMenu[]
	{	{ 1,	"Yes"	}
	,	{ 0,	"No"	}
	};

	bool bPlayAgain = true;
	while(bPlayAgain)
	{
		adventurer			= CCharacter(CHARACTER_TYPE_PLAYER,	10, 50, 1, 100, Name);
		adventurer.Weapon	= rand()%4;
		adventurer.Armor	= rand()%4;
		adventurer.Shield	= armorDefinitions[adventurer.Armor].Shield;

		SCharacterPoints finalPoints = calculateFinalPoints(adventurer);
		adventurer.Points.HP = finalPoints.MaxHP;

		std::cout << "\nSo, " << adventurer.Name << "... What brings you here?\n";
		tavern(adventurer);	// Tavern is the main menu of our game.

		printf("\n-- Game Over! --\n");
		displayScore(adventurer);

		bPlayAgain = displayMenu("Play again? ..", playAgainMenu) ? true : false;
	}
}
