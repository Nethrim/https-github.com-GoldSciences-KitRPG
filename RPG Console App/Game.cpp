#include "Game.h"
#include "Menu.h"

void tavern(CGame& gameInstance)
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

	while (gameInstance.Adventurer.Points.HP > 0)  // If the last action didn't go well we cancel the loop and exit the game.
	{
		int tavernChoice = displayMenu("You wonder about what to do next..", tavernOptions);

			 if( 1 == tavernChoice )	{	rest(gameInstance.Adventurer.Points);	}	// Rest and ask again for the action.
		else if( 2 == tavernChoice )	{	mercenaryJob(gameInstance);				}	// Go for a mercenary job and ask again for action once it's done
		else if( 3 == tavernChoice )	{	bar(gameInstance);						}	// Go to the shop and ask again for action once it's done.
		else if( 4 == tavernChoice )	{	showInventory(gameInstance.Adventurer);	}	// Display the inventory and coins and ask again for action once it's done.
		else if( 5 == tavernChoice )	{	displayScore(gameInstance);				}	// Display score and player points and ask again for action once it's done.
		else if( 6 == tavernChoice )	{	break;									}	// Exit the main loop, which effectively closes the game.
	}
}

void mercenaryJob(CGame& gameInstance)
{
	static const SMenuItem jobOptions[] =
	{ { 1, "Easy Job"			}
	, { 2, "Medium Job"			}
	, { 3, "Hard Job"			}
	, { 4, "Back to the tavern"	}
	};

	int mercenaryDifficulty = displayMenu("You decide to enroll for a mercenary job.", jobOptions);

	// Set bCombat to true and the enemy type for executing the combat logic.
	bool bCombat = false;
	CHARACTER_TYPE enemyType = CT_UNKNOWN;	

		 if(1 == mercenaryDifficulty)	{ bCombat = true;	enemyType	= CT_WOLF;		}
	else if(2 == mercenaryDifficulty)	{ bCombat = true;	enemyType	= CT_RAIDER;	}
	else if(3 == mercenaryDifficulty)	{ bCombat = true;	enemyType	= CT_SOLDIER;	}
	else if(4 == mercenaryDifficulty)	{ // This option cancels the loop which causes to exit to the tavern.
		std::cout << "Welcome back, " << gameInstance.Adventurer.Name << ".\n";
		return;
	}

	if( bCombat ) {
		printf("You challenge a %s.\n", getEnemyDefinition(enemyType).Name.c_str()); 
		combat(gameInstance, enemyType);
	}
}

void bar(CGame& gameInstance)
{
	printf("\nDo you want to buy some drinks?\n\n");

	static const int descriptionCount = getDescriptionCount(itemDescriptions);
	while (true)	// break the loop to leave the shop
	{
		printf("-- You have %u coins.\n", gameInstance.Adventurer.Points.Coins);
		printf("Type %u to leave the bar.\n\n", descriptionCount+1);
		for(int i=0; i<descriptionCount; i++)	// Print available items
			printf("%u: Buy %s for %u coins.\n", i+1, itemDescriptions[i].Name.c_str(), itemDescriptions[i].Price);
		printf("\n");

		unsigned int indexItem = getNumericInput()-1;
		
		if( indexItem == descriptionCount ) {
			printf("You leave the bar.\n");
			break;
		}
		else if(indexItem >= descriptionCount)
			printf("You can't buy something that doesn't exist!\n");
		else 
		{
			int itemPrice	= itemDescriptions[indexItem].Price;	// Get a copy of this value because we use it very often.

			// Check first for conditions that prevent from acquiring the item
			if(gameInstance.Adventurer.Points.Coins < itemPrice)
				printf("You can't afford to buy that! Choose something else...\n");
			else if(gameInstance.Adventurer.itemCount == getInventorySize(gameInstance.Adventurer.inventory))
				printf("You don't have enough space in your inventory.\n");
			else {	// If we didn't find a reason for canceling the buy, add the item to the player's inventory and do the money calculations.
				addItem(gameInstance.Adventurer, itemDescriptions[indexItem]);
				printf("You spend %u coins buying %s.\n", itemPrice, itemDescriptions[indexItem].Name.c_str());
				gameInstance.Adventurer.Points.Coins			-= itemPrice;
				gameInstance.Statisticts.MoneySpent	+= itemPrice;
			}
		}
	}
	showInventory(gameInstance.Adventurer);
}

void displayScore(const CGame& gameInstance) 
{
	const SCharacterPoints& points = gameInstance.Adventurer.Points;
	const SGameCounters& gameCounters = gameInstance.Statisticts;

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
		"Battles Won         : %u\n"
		"Turns Played        : %u\n"
		"--------------------\n"
		"Enemies Killed      : %u\n"
		"Damage Dealt        : %u\n"
		"Damage Taken        : %u\n"
		"--------------------\n"
		"Escapes Succeeded   : %u\n"
		"Escapes Failed      : %u\n"
		"--------------------\n"
		"Money Earned        : %u\n"
		"Money Spent         : %u\n"
		"--------------------\n"
		"Attacks Hit         : %u\n"
		"Attacks Missed      : %u\n"
		"Attacks Received    : %u\n"
		"Attacks Avoided     : %u\n"
		"--------------------\n"
		"Potions Used        : %u\n"
		"Grenades Used       : %u\n\n"
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
