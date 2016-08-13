#define NOMINMAX

#include "Game.h"

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
	getline(std::cin, gameInstance.Adventurer.Name);
	std::cout << "\nSo, " << gameInstance.Adventurer.Name << "... What brings you here?\n";

	tavern(gameInstance.Adventurer);	// Tavern is the main loop of our game. Exiting it means we quit the game.

	printf("\n-- Game Over! --\n\n");
	displayScore(gameInstance.Adventurer);

	printf("Press ENTER to exit.");
	getline(std::cin, gameInstance.Adventurer.Name);
}
