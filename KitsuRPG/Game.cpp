#include "Game.h"

#include "Item.h"
#include "Enemy.h"
#include "Profession.h"
#include "Armor.h"
#include "Weapon.h"

#include "Menu.h"

void tavern(CCharacter& adventurer)
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	static const SMenuItem<int> tavernOptions[] =
	{ { 1, "Rest"						}
	, { 2, "Look for a mercenary job"	}
	, { 3, "Go for a drink"				}
	, { 4, "Show inventory"				}
	, { 5, "Display score"				}
	, { 6, "Exit game"					}
	};

	while (true)  // If the last action didn't go well we cancel the loop and exit the game.
	{
		int tavernChoice = displayMenu("You wonder about what to do next..", tavernOptions);

			 if( 1 == tavernChoice )	{	rest(adventurer);			}	// Rest and ask again for the action.
		else if( 2 == tavernChoice )	{	mercenaryJob(adventurer);	}	// Go for a mercenary job and ask again for action once it's done
		else if( 3 == tavernChoice )	{	bar(adventurer);			}	// Go to the shop and ask again for action once it's done.
		else if( 4 == tavernChoice )	{	showInventory(adventurer);	}	// Display the inventory and coins and ask again for action once it's done.
		else if( 5 == tavernChoice )	{	displayScore(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 6 == tavernChoice )	{	break;						}	// Exit the main loop, which effectively closes the game.
	}
}

void mercenaryJob(CCharacter& adventurer)
{
	static const size_t enemyCount = size(enemyDefinitions);
	SMenuItem<uint32_t> jobOptions[enemyCount];
	for(uint32_t i=0, count = enemyCount-1; i<count; ++i)
	{
		jobOptions[i].ReturnValue	= i+1;
		jobOptions[i].Text			= "Level " + std::to_string(i+1);
	}
	jobOptions[enemyCount-1].ReturnValue	= enemyCount;
	jobOptions[enemyCount-1].Text			= "Back to tavern";

	const uint32_t enemyType = displayMenu("You decide to enroll for a mercenary job", jobOptions);

	// Set bCombat to true and the enemy type for executing the combat logic.
	bool bCombat = false;

	if(enemyCount == enemyType)	// This option cancels the loop which causes to exit to the tavern.
		printf("Welcome back, %s.\n", adventurer.Name.c_str());
	else {
		printf("You challenge a %s.\n", enemyDefinitions[enemyType].Name.c_str()); 
		combat(adventurer, enemyType);
	}
}

void bar(CCharacter& adventurer)
{
	printf("\nDo you want to buy some drinks?\n\n");

	static const size_t descriptionCount = size(itemDescriptions);
	static SMenuItem<uint32_t> itemOptions[descriptionCount];
	static const int initialized = initializeItemMenu(itemOptions);

	char menuTitle[128] = {};
	while (true)	// break the loop to leave the shop
	{
		sprintf_s(menuTitle, "You have %u coins", adventurer.Points.Coins);
		const uint32_t indexItem = displayMenu(menuTitle, itemOptions);
		if( indexItem == descriptionCount ) {
			printf("You leave the bar.\n");
			break;
		}
		else 
		{
			int itemPrice	= itemDescriptions[indexItem].Price;	// Get a copy of this value because we use it very often.

			// Check first for conditions that prevent from acquiring the item
			if(adventurer.Points.Coins < itemPrice)
				printf("You can't afford to buy that! Choose something else...\n");
			else if(addItem(adventurer.Inventory, indexItem))	// addItem() returns false if the inventory is full.
			{
				printf("You spend %u coins buying %s.\n", itemPrice, itemDescriptions[indexItem].Name.c_str());
				adventurer.Points.Coins		-= itemPrice;
				adventurer.Score.MoneySpent	+= itemPrice;
			}
			else
				printf("Not enough space in inventory!\n");
		}
	}
	showInventory(adventurer);
}

void displayScore(const CCharacter& adventurer) 
{
	const SCharacterPoints& points = adventurer.Points;
	const SCharacterScore& gameCounters		= adventurer.Score;
	const SCharacterPoints finalPoints		= calculateFinalPoints(adventurer);
	const SCharacterPoints weaponPoints		= getWeaponPoints(adventurer.Weapon);
	const SCharacterPoints armorPoints		= getArmorPoints(adventurer.Armor);
	const SCharacterPoints professionPoints	= getProfessionPoints(adventurer.Profession);

	printf("\n-- %s final points:\n", adventurer.Name.c_str());
	printf("\n- Max Life:\n");
	finalPoints.MaxLife.Print();
	printf("\n- Current Life:\n");
	points.CurrentLife.Print();
	printf("\n- Attack:\n");
	finalPoints.Attack.Print();

	printf("\n-- %s base character points:\n", adventurer.Name.c_str());
	printf("\n- Max Life:\n");
	points.MaxLife.Print();
	//printf("\n- Current Life:\n");
	//points.CurrentLife.Print();
	printf("\n- Attack:\n");
	points.Attack.Print();

	printf("\n-- %s is a %s level %u:\n", adventurer.Name.c_str(), getProfessionName(adventurer.Profession).c_str(), adventurer.Profession.Level);
	printf("\n- Max Life:\n");
	professionPoints.MaxLife.Print();
	printf("\n- Current Life:\n");
	professionPoints.CurrentLife.Print();
	printf("\n- Attack:\n");
	professionPoints.Attack.Print();

	printf("\n-- %s is wearing %s level %u:\n", adventurer.Name.c_str(), getArmorName(adventurer.Armor).c_str(), adventurer.Armor.Level);
	printf("\n- Max Life:\n");
	armorPoints.MaxLife.Print();
	printf("\n- Current Life:\n");
	armorPoints.CurrentLife.Print();
	printf("\n- Attack:\n");
	armorPoints.Attack.Print();

	printf("\n-- %s is carrying %s level %u:\n", adventurer.Name.c_str(), getWeaponName(adventurer.Weapon).c_str(), adventurer.Weapon.Level);
	printf("\n- Max Life:\n");
	weaponPoints.MaxLife.Print();
	printf("\n- Current Life:\n");
	weaponPoints.CurrentLife.Print();
	printf("\n- Attack:\n");
	weaponPoints.Attack.Print();

	printf("\n-- Player statistics:\n\n"
		"Battles Won         : %llu\n"
		"Battles Lost        : %llu\n"
		"Turns Played        : %llu\n"
		"--------------------\n"
		"Enemies Killed      : %llu\n"
		"Damage Dealt        : %llu\n"
		"Damage Taken        : %llu\n"
		"--------------------\n"
		"Escapes Succeeded   : %llu\n"
		"Escapes Failed      : %llu\n"
		"--------------------\n"
		"Money Earned        : %llu\n"
		"Money Spent         : %llu\n"
		"--------------------\n"
		"Attacks Hit         : %llu\n"
		"Attacks Missed      : %llu\n"
		"Attacks Received    : %llu\n"
		"Attacks Avoided     : %llu\n"
		"--------------------\n"
		"Potions Used        : %llu\n"
		"Grenades Used       : %llu\n\n"
		, gameCounters.BattlesWon			
		, gameCounters.BattlesLost
		, gameCounters.TurnsPlayed			
	
		, gameCounters.EnemiesKilled		
		, gameCounters.DamageDealt			
		, gameCounters.DamageTaken			
	
		, gameCounters.EscapesSucceeded	
		, gameCounters.EscapesFailed		
	
		, gameCounters.MoneyEarned			
		, gameCounters.MoneySpent			

		, gameCounters.AttacksHit			
		, gameCounters.AttacksMissed		
		, gameCounters.AttacksReceived		
		, gameCounters.AttacksAvoided		

		, gameCounters.PotionsUsed			
		, gameCounters.GrenadesUsed		
	);
};
