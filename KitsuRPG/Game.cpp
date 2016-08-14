#include "Game.h"
#include "Menu.h"

void tavern(CCharacter& adventurer)
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	static const SMenuItem tavernOptions[] =
	{ { 1, "Rest"						}
	, { 2, "Look for a mercenary job"	}
	, { 3, "Go for a drink"				}
	, { 4, "Show inventory"				}
	, { 5, "Display score"				}
	, { 6, "Exit game"					}
	};

	while (adventurer.Points.HP > 0)  // If the last action didn't go well we cancel the loop and exit the game.
	{
		int tavernChoice = displayMenu("You wonder about what to do next..", tavernOptions);

			 if( 1 == tavernChoice )	{	rest(adventurer.Points);	}	// Rest and ask again for the action.
		else if( 2 == tavernChoice )	{	mercenaryJob(adventurer);	}	// Go for a mercenary job and ask again for action once it's done
		else if( 3 == tavernChoice )	{	bar(adventurer);			}	// Go to the shop and ask again for action once it's done.
		else if( 4 == tavernChoice )	{	showInventory(adventurer);	}	// Display the inventory and coins and ask again for action once it's done.
		else if( 5 == tavernChoice )	{	displayScore(adventurer);	}	// Display score and player points and ask again for action once it's done.
		else if( 6 == tavernChoice )	{	break;									}	// Exit the main loop, which effectively closes the game.
	}
}

void mercenaryJob(CCharacter& adventurer)
{
	static const SMenuItem jobOptions[] =
	{ { ENEMY_TYPE_WOLF,					"Easy Job"				}
	, { ENEMY_TYPE_RAIDER,					"Medium Job"			}
	, { ENEMY_TYPE_SOLDIER,					"Hard Job"				}
	, { ENEMY_TYPE_KNIGHT,					"Very Hard Job"			}
	, { ENEMY_TYPE_DRAGON,					"Nightmare Job"			}
	, { (uint32_t)size(enemyDefinitions),	"Back to the tavern"	}
	};

	const ENEMY_TYPE enemyType = (ENEMY_TYPE)displayMenu("You decide to enroll for a mercenary job", jobOptions);

	// Set bCombat to true and the enemy type for executing the combat logic.
	bool bCombat = false;

	if(size(enemyDefinitions) == enemyType)	{ // This option cancels the loop which causes to exit to the tavern.
		std::cout << "Welcome back, " << adventurer.Name << ".\n";
		return;
	}
	else {
		printf("You challenge a %s.\n", getEnemyDefinition(enemyType).Name.c_str()); 
		combat(adventurer, enemyType);
	}
}

void bar(CCharacter& adventurer)
{
	printf("\nDo you want to buy some drinks?\n\n");

	static const size_t descriptionCount = size(itemDescriptions);
	while (true)	// break the loop to leave the shop
	{
		printf("-- You have %u coins.\n", adventurer.Points.Coins);
		printf("Type %u to leave the bar.\n\n", (uint32_t)(descriptionCount));
		for(size_t i=1; i<descriptionCount; i++)	// Print available items
			printf("%u: Buy %s for %u coins.\n", i, itemDescriptions[i].Name.c_str(), itemDescriptions[i].Price);
		printf("\n");

		const uint32_t indexItem = (uint32_t)getNumericInput();
		
		if( indexItem == (descriptionCount) ) {
			printf("You leave the bar.\n");
			break;
		}
		else if(indexItem > descriptionCount)
			printf("You can't buy something that doesn't exist!\n");
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
	const SCharacterScore& gameCounters = adventurer.Score;

	printf("\n-- Player points:\n\n"
		"Max HP     : %u.\n"
		"HP         : %u.\n"
		"Attack     : %u.\n"
		"Hit chance : %u.\n"
		"Coins      : %u.\n"
		, points.MaxHP     
		, std::max(points.HP, 0)
		, points.Attack     
		, points.Hit
		, points.Coins		
	);

	printf("\n-- Player statistics:\n\n"
		"Battles Won         : %llu\n"
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
