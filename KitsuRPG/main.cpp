#define NOMINMAX

#include "Game.h"
#include "Armor.h"
#include "Weapon.h"

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
		adventurer					= CCharacter(CHARACTER_TYPE_PLAYER,	30, 50, 5, 100, Name);
		adventurer.Weapon	.Index		= (int16_t)size(weaponDefinitions	)-1;//(rand() % size(weaponDefinitions	));	//%(size(weaponDefinitions)/5);
		adventurer.Armor	.Index		= (int16_t)size(armorDefinitions	)-1;//(rand() % size(armorDefinitions	));	//%(size(armorDefinitions)/5);
		adventurer.Weapon	.Modifier	= (int16_t)size(weaponModifiers		)-1;//(rand() % size(weaponModifiers	));	//%(size(weaponModifiers)/2);
		adventurer.Armor	.Modifier	= (int16_t)size(armorModifiers		)-1;//(rand() % size(armorModifiers	));
		adventurer.Shield				= getArmorShield(adventurer.Armor);

		SCharacterPoints finalPoints = calculateFinalPoints(adventurer);
		adventurer.Points.HP = finalPoints.MaxHP;

		std::cout << "\nSo, " << adventurer.Name << "... What brings you here?\n";
		tavern(adventurer);	// Tavern is the main menu of our game.

		printf("\n-- Game Over! --\n");
		displayScore(adventurer);

		bPlayAgain = displayMenu("Play again? ..", playAgainMenu) ? true : false;
	}
}
