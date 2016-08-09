#define NOMINMAX

#include <iostream>
#include <time.h>
#include <windows.h>
#include <algorithm>
#include "Misc.h"
#include "Enemy.h"
#include "Player.h"

CPlayer adventurer (CT_PLAYER, 200, 6, 50, 100, "Anonymous"); //MaxHP, ATk, HitChance, Coins.

// This struct holds a counter for every action that we want to record mostly for score purposes.
struct SGameCounters {
	int BattlesWon			= 0;
	int TurnsPlayed			= 0;

	int EnemiesKilled		= 0;
	int DamageDealt			= 0;
	int DamageTaken			= 0;

	int EscapesSucceeded	= 0;
	int EscapesFailed		= 0;

	int MoneyEarned			= 0;
	int MoneySpent			= 0;

	int AttacksHit			= 0;
	int AttacksMissed		= 0;
	int AttacksReceived		= 0;
	int AttacksAvoided		= 0;

	int PotionsUsed			= 0;
	int GrenadesUsed		= 0;

} GlobalGameCounters;

void tavern();
void rest();
void mercenaryJob();
void bar();
void displayScore();
void showInventory();
bool useItems(CCharacter& enemy);

void main()
{	
	srand((unsigned int)time(NULL));

	printf("Welcome Stranger!! who are you?\n");
	printf("My name is: \n");
	getline(std::cin, adventurer.Name);
	std::cout << "\nSo, " << adventurer.Name << "... What brings you here?\n";

	tavern();	// Tavern is the main loop of our game. Exiting it means we quit the game.

	printf("\n-- Game Over! --\n\n");
	displayScore();

	system("PAUSE");
}

// This function returns the damage dealt to the target
int attack(const CCharacter& attacker, CCharacter& target)
{
	// Calculate success from the hit chance and apply damage to target or just print the miss message.
	int damageDealt = 0;

	if ((rand() % 100) < attacker.Points.Hit)
	{
		damageDealt = attacker.Points.Attack+(rand()%10);
		target.Points.HP -= damageDealt;
		printf("%s hits %s for: %u.\n", attacker.Name.c_str(), target.Name.c_str(), damageDealt);
	}
	else 
		printf("%s misses the attack!\n", attacker.Name.c_str());

	return damageDealt;
};

void combat(CHARACTER_TYPE enemyType)
{
	CCharacter currentEnemy = getEnemyDefinition(enemyType);	// Request the enemy data.

	while (adventurer.Points.HP > 0 && currentEnemy.Points.HP > 0)	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		GlobalGameCounters.TurnsPlayed++;

		while (true)	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
		{
			printf("\n-- Your HP is: %u.\n", adventurer.Points.HP);
			printf("-- %s HP is: %u.\n", currentEnemy.Name.c_str(), currentEnemy.Points.HP);
			printf("Select action:\n"
				"1: Attack.\n"
				"2: Inventory.\n"
				"3: Run.\n\n"
			);

			unsigned int actionChoice = getNumericInput();
			
			// If the action is valid then we execute it and break the current while() so the attack turn executes.
			if(1 == actionChoice) { 
				printf("You decide to attack!\n"); 

				int damageDealt = attack(adventurer, currentEnemy);
				if(damageDealt) {
					GlobalGameCounters.DamageDealt += damageDealt;
					GlobalGameCounters.AttacksHit++;
				}
				else 
					GlobalGameCounters.AttacksMissed++;

				break; 
			}
			else if(2 == actionChoice) { 
				if( useItems(currentEnemy) )	// If no items were used we ask again for user input. 
					break; 
				else
					continue;
			}	// useItems requires to receive the current enemy as a parameter in order to modify its health if we use a grenade and hit.
			else if(3 == actionChoice) { // Escape: if we succeed we just exit this combat() function, otherwise cancel this loop and execute the attack turn.
				printf("You try to escape!\n\n");
				if ((rand() % 100) < 30) {
					printf("You escaped from combat!\n");
					GlobalGameCounters.EscapesSucceeded++;
					return;
				}

				GlobalGameCounters.EscapesFailed++;
				printf("You failed to escape!\n");
				break;
			}
			else {	// We do nothing on invalid action so the while() just executes the next interation of input request.
				printf("Invalid action.\n");
			}
		}

		if (adventurer.Points.HP <= 0  || currentEnemy.Points.HP <= 0)	// We do this check because player actions may kill one of the combatants and in that case we need to exit this loop.
			break;	// Cancel the combat loop to exit combat.

		// Execute enemy attack turn
		int damageDealt = attack(currentEnemy, adventurer);
		if(damageDealt) {
			GlobalGameCounters.DamageTaken += damageDealt;
			GlobalGameCounters.AttacksReceived++;
		}
		else
			GlobalGameCounters.AttacksAvoided++;
	} 

	if (adventurer.Points.HP <= 0) 
		std::cout << "You are dead!\n";
	else if (currentEnemy.Points.HP <= 0)
	{
		printf("The %s is dead!\n", currentEnemy.Name.c_str());
		int drop = currentEnemy.Points.Coins + (rand() % 20);
		std::cout << "\nThe enemy dropped " << drop << " coins!!\n\n";
		adventurer.Points.Coins = adventurer.Points.Coins + drop;

		switch(currentEnemy.Type) {
		case CT_SOLDIER	:	adventurer.Points.MaxHP += 2;
		case CT_RAIDER	:	adventurer.Points.MaxHP++;
		case CT_WOLF	:	adventurer.Points.MaxHP++;
		}

		GlobalGameCounters.BattlesWon++;
		GlobalGameCounters.EnemiesKilled++;
		GlobalGameCounters.MoneyEarned += drop;
	}
}

void tavern()
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	while (adventurer.Points.HP > 0) // If the mercenary job didn't go well we cancel the loop and exit the game.
	{
		printf(	"\n-- You wonder about what to do next...\n");
		printf(	"Select your next action:\n");
		printf(	"1: Rest.\n"
				"2: Look for a mercenary job.\n"
				"3: Go for a drink.\n"
				"4: Show inventory.\n"
				"5: Display score.\n"
				"6: Exit game.\n\n"
		);

		unsigned int tavernChoice = getNumericInput();

			 if( 1 == tavernChoice )	{	rest();				}	// Rest and ask again for the action.
		else if( 2 == tavernChoice )	{	mercenaryJob();		}	// Go for a mercenary job and ask again for action once it's done
		else if( 3 == tavernChoice )	{	bar();				}	// Go to the shop and ask again for action once it's done.
		else if( 4 == tavernChoice )	{	showInventory();	}	// Display the inventory and coins and ask again for action once it's done.
		else if( 5 == tavernChoice )	{	displayScore();		}	// Display score and player points and ask again for action once it's done.
		else if( 6 == tavernChoice )	{	break;				}	// Exit the main loop, which effectively closes the game.
		else {	// Enter here if we didn't recognize the option. Print the error message and ask again for input.
			printf("Invalid answer. Answer again...\n");
		};
	}
}

void rest()
{
	printf("\nYou decide to get some rest.\n");
	adventurer.Points.HP = adventurer.Points.MaxHP;
	printf("Your HP is: %u.\n", adventurer.Points.HP);
}

void mercenaryJob()
{
	printf("\nYou decide to enroll for a mercenary job.\n");
	while (true)
	{
		printf(	"1: Easy Job.\n"
				"2: Medium Job.\n"
				"3: Hard Job.\n"
				"4: Back to the tavern.\n\n");

		unsigned int mercenaryDif = getNumericInput();

		// Set bCombat to true and the enemy type for executing the combat logic.
		bool bCombat = false;
		CHARACTER_TYPE enemyType = CT_UNKNOWN;	

			 if(1 == mercenaryDif)	{ bCombat = true;	enemyType	= CT_WOLF;		}
		else if(2 == mercenaryDif)	{ bCombat = true;	enemyType	= CT_RAIDER;	}
		else if(3 == mercenaryDif)	{ bCombat = true;	enemyType	= CT_SOLDIER;	}
		else if(4 == mercenaryDif)	{ // This option cancels the loop which causes to exit to the tavern.
			std::cout << "Welcome back, " << adventurer.Name << ".\n";
			break;
		}
		else {
			printf("Invalid answer. Answer again...\n");
		}

		if( bCombat ) {
			printf("You challenge a %s.\n", getEnemyDefinition(enemyType).Name.c_str()); 
			combat(enemyType);
			break;	// Just exit this function after the combat ends and automatically go back to the tavern.
		}
	}
}

void addItem(const SItem& itemDescription)
{
	// look in the inventory for the name so we just increment the counter instead of adding the item
	for(int i=0, count = adventurer.itemCount; i<count; i++) {
		if(itemDescription.Name == adventurer.inventory[i].Description.Name) {
			adventurer.inventory[i].Count++;
			return;
		}
	}

	// If we didn't return yet it means that the item was not found and we need to add it to the inventory.
	adventurer.inventory[adventurer.itemCount].Description	= itemDescription;
	adventurer.inventory[adventurer.itemCount].Count		= 1;
	adventurer.itemCount++;
}

void bar()
{
	printf("\nDo you want to buy some drinks?\n\n");

	static const int descriptionCount = getDescriptionCount(itemDescriptions);
	while (true)	// break the loop to leave the shop
	{
		printf("-- You have %u coins.\n", adventurer.Points.Coins);
		printf("Type %u to leave the bar.\n\n", descriptionCount+1);
		for(int i=0; i<descriptionCount; i++)	// Print available items
			printf("%u: Buy %s for %u coins.\n", i+1, itemDescriptions[i].Name.c_str(), itemDescriptions[i].Price);
		printf("\n");

		unsigned int indexItem = getNumericInput()-1;
		
		if( indexItem == descriptionCount ) {
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
			else if(adventurer.itemCount == getInventorySize(adventurer.inventory))
				printf("You don't have enough space in your inventory.\n");
			else {	// If we didn't find a reason for canceling the buy, add the item to the player's inventory and do the money calculations.
				addItem(itemDescriptions[indexItem]);
				printf("You spend %u coins buying %s.\n", itemPrice, itemDescriptions[indexItem].Name.c_str());
				adventurer.Points.Coins			-= itemPrice;
				GlobalGameCounters.MoneySpent	+= itemPrice;
			}
		}
	}
	showInventory();
}

void showInventory()
{
	printf("\n-- Your inventory --\n\n");
	printf("-- You look at your wallet and count %u coins.\n\n", adventurer.Points.Coins);
	if(adventurer.itemCount) {
		printf("You look at the remaining supplies in your backpack...\n");
		for (unsigned int i = 0; i < adventurer.itemCount; i++)
			printf("%u: x%.2u %s.\n", i + 1, adventurer.inventory[i].Count, adventurer.inventory[i].Description.Name.c_str());
		printf("\n");
	}
}

// This function returns true if an item was used or false if the menu was exited without doing anything.
bool useItems(CCharacter& enemy)
{
	bool bUsedItem = false;
	unsigned int indexItem = ~0U;
	SItem itemDescription;
	while(true)
	{
		printf("- Type %u to close your inventory.\n", getInventorySize(adventurer.inventory)+1);
		showInventory();

		indexItem = getNumericInput()-1;

		if(indexItem == getInventorySize(adventurer.inventory)) // exit option
			break;
		else if(indexItem >= adventurer.itemCount)	// invalid index means it's an invalid option
			printf("Invalid answer. Answer again...\n");
		else if (adventurer.inventory[indexItem].Count <= 0)
			printf("You don't have anymore of that. Use something else...\n"); 
		else {
			// if we reached here it means that the input was valid so we select the description and exit the loop
			itemDescription = adventurer.inventory[indexItem].Description;
			bUsedItem = true;
			break;
		}
	}

	if(bUsedItem) {
		int itemEffectValue = 0;
		int lotteryRange = 0;
		int lotteryResult = 0;

		std::string itemName = itemDescription.Name;
		printf("\nYou used: %s.\n\n", itemName.c_str());
		switch( itemDescription.Type )
		{
		case IT_POTION:
			printf("You start feeling better...\n");

			itemEffectValue = (10*itemDescription.Grade+(rand()%10))*itemDescription.Grade;
			adventurer.Points.HP += itemEffectValue;
			adventurer.Points.HP = std::min(adventurer.Points.HP, adventurer.Points.MaxHP);

			printf("The potion heals you for %u HP. You now have %u HP.\n", itemEffectValue, adventurer.Points.HP);
			GlobalGameCounters.PotionsUsed++;
			break;

		case IT_GRENADE:
			printf("You throw that shit to the enemy.\n");
			itemEffectValue = int(enemy.Points.MaxHP*(0.2f*itemDescription.Grade)); // this is the total damage points applied to the enemy or player

			lotteryRange = 60+(10*itemDescription.Grade);
			lotteryResult = rand()%100;
			if( lotteryResult < lotteryRange)
			{
				enemy.Points.HP -= itemEffectValue;	// this is the resulting hp after applying the damage 
				GlobalGameCounters.DamageDealt += itemEffectValue;
				printf("The grenade hits the enemy doing %u damage.\n", itemEffectValue);
			}
			else if( lotteryResult == lotteryRange )
			{
				adventurer.Points.HP -= itemEffectValue/2;	// this is the resulting hp after applying the damage 
				GlobalGameCounters.DamageTaken += itemEffectValue;
				printf("You throw the grenade too close...\n"		
					"The grenade explodes near you hurting you by %u damage!\n", itemEffectValue);
			}
			else
				printf("You throw the grenade too far away.\n");

			GlobalGameCounters.GrenadesUsed++;
			break;

		default:
			printf("This item type does nothing yet... But we still remove it from your inventory!\n");
		}

		adventurer.inventory[indexItem].Count--;
		if( adventurer.inventory[indexItem].Count )
			printf("\nYou have %u %s left.\n", adventurer.inventory[indexItem].Count, itemName.c_str());
		else 
		{
			adventurer.inventory[indexItem] = adventurer.inventory[--adventurer.itemCount];
			printf("\nYou ran out of %s.\n", itemName.c_str());
		}
	}
	
	return bUsedItem;
}

void displayScore() 
{
	printf("\n-- Player points:\n\n"
		"Max HP     : %u.\n"
		"HP         : %u.\n"
		"Attack     : %u.\n"
		"Hit chance : %u.\n"
		"Coins      : %u.\n"
		, adventurer.Points.MaxHP     
		, std::max(adventurer.Points.HP, 0)
		, adventurer.Points.Attack     
		, adventurer.Points.Hit
		, adventurer.Points.Coins		
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
		, GlobalGameCounters.BattlesWon			
		, GlobalGameCounters.TurnsPlayed			
	
		, GlobalGameCounters.EnemiesKilled		
		, GlobalGameCounters.DamageDealt			
		, GlobalGameCounters.DamageTaken			
	
		, GlobalGameCounters.EscapesSucceeded	
		, GlobalGameCounters.EscapesFailed		
	
		, GlobalGameCounters.MoneyEarned			
		, GlobalGameCounters.MoneySpent			

		, GlobalGameCounters.AttacksHit			
		, GlobalGameCounters.AttacksMissed		
		, GlobalGameCounters.AttacksReceived		
		, GlobalGameCounters.AttacksAvoided		

		, GlobalGameCounters.PotionsUsed			
		, GlobalGameCounters.GrenadesUsed		
	);
};
