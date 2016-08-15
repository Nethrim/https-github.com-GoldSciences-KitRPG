#define NOMINMAX

#include "Game.h"
#include "Menu.h"

#include <iostream>
#include <time.h>

// This function seeds the rand() and enters the tavern() after initializing the player.
// If the player leaves the tavern() it means the game was requested to close. 
// After leaving the tavern() we display the score of the player.
void main()
{	
	srand((unsigned int)time(NULL));

	CGame gameInstance;	// This class instance will contain all our game data.

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
		gameInstance.Adventurer			= CCharacter(CHARACTER_TYPE_PLAYER,	10, 50, 1, 100, Name);
		gameInstance.Adventurer.Weapon	= rand()%4;
		SCharacterPoints finalPoints = calculateFinalPoints(gameInstance.Adventurer);
		gameInstance.Adventurer.Points.HP = finalPoints.MaxHP;

		std::cout << "\nSo, " << gameInstance.Adventurer.Name << "... What brings you here?\n";
		tavern(gameInstance.Adventurer);	// Tavern is the main menu of our game.

		printf("\n-- Game Over! --\n");
		displayScore(gameInstance.Adventurer);

		bPlayAgain = displayMenu("Play again? ..", playAgainMenu) ? true : false;
	}
}
