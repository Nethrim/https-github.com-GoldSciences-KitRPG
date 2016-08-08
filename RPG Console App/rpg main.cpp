#include <iostream>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "Enemy.h"
#include "Player.h"

std::string name;
int maxHP = 200;

CPlayer adventurer (CT_PLAYER, 200, 6, 50, 50); //MaxHP, ATk, HitChance, Coins.

struct SGameCounters {
	int DamageDealt			= 0;
	int DamageTaken			= 0;

	int EnemiesKilled		= 0;
	int PotionsUsed			= 0;
	int BattlesWon			= 0;
	int MoneyEarned			= 0;
	int MoneySpent			= 0;
	int TurnsPlayed			= 0;

	int AttacksHit			= 0;
	int AttacksMissed		= 0;
	int AttacksReceived		= 0;
	int AttacksAvoided		= 0;

	int EscapesSucceeded	= 0;
	int EscapesFailed		= 0;
} GlobalGameCounters;


void tavern();
void rest();
void mercenaryJob();
void drink();
void showInventory();
void useItems();

void main()
{	
	srand((unsigned int)time(NULL));
	for(int item = 0; item < MAX_INVENTORY_SLOTS; item++) {
		adventurer.inventory[item].Name  = "EMPTY";
		adventurer.inventory[item].Count = 0;
	}

	printf("Welcome Stranger!! who are you?\n");
	printf("My name is: \n");
	getline(std::cin, name);
	std::cout << "So, " << name << "... What brings you here?\n";

	tavern();	// Tavern is the main loop of our game. Exiting it means we quit the game.

	printf("\nGame Over!\n\n");
	printf("Player statistics:\n"
   "Damage Dealt		: %u.\n"
   "Damage Taken		: %u.\n"
   "Enemies Killed		: %u.\n"
   "Potions Used		: %u.\n"
   "Battles Won			: %u.\n"
   "Money Earned		: %u.\n"
   "Money Spent			: %u.\n"
   "Turns Played		: %u.\n"
   "Attacks Hit			: %u.\n"
   "Attacks Missed		: %u.\n"
   "Attacks Received	: %u.\n"
   "Attacks Avoided		: %u.\n"
   "Escapes Succeeded	: %u.\n"
   "Escapes Failed		: %u.\n"
	, GlobalGameCounters.DamageDealt		
	, GlobalGameCounters.DamageTaken		
	, GlobalGameCounters.EnemiesKilled	
	, GlobalGameCounters.PotionsUsed		
	, GlobalGameCounters.BattlesWon		
	, GlobalGameCounters.MoneyEarned		
	, GlobalGameCounters.MoneySpent		
	, GlobalGameCounters.TurnsPlayed		
	, GlobalGameCounters.AttacksHit		
	, GlobalGameCounters.AttacksMissed	
	, GlobalGameCounters.AttacksReceived	
	, GlobalGameCounters.AttacksAvoided	
	, GlobalGameCounters.EscapesSucceeded	
	, GlobalGameCounters.EscapesFailed	
	);


	system("PAUSE");
}


void combat(CHARACTER_TYPE enemyType)
{
	CEnemy currentEnemy = getEnemyDefinition(enemyType);	// Request the enemy data.

	while (true)	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		GlobalGameCounters.TurnsPlayed++;

		while (true)	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
		{
			printf("\nSelect Action:\n");
			printf("1-Attack 2-Inventory 3-Run\n\n");
			char action = getchar();
			getchar();
			
			// If the action is valid then we execute it and break the current while() so the attack turn executes.
				 if('1' == action) { printf("You decide to attack!\n"); break; }
			else if('2' == action) { useItems(); break; }
			else if('3' == action) { // Escape: if we succeed we just exit this combat() function, otherwise cancel this loop and execute the attack turn.
				std::cout << "You try to escape!\n\n";
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

		// Calculate the enemy hit chance and apply damage to player or just print the miss message.
		if ((rand() % 100) < currentEnemy.Points.Hit )
		{
			int enemyDamage = currentEnemy.Points.Attack+(rand()%10);
			printf("The %s hits you for: %u.\n", currentEnemy.Name.c_str(), enemyDamage);

			GlobalGameCounters.DamageTaken += enemyDamage;
			GlobalGameCounters.AttacksReceived++;

			adventurer.Points.Hp = adventurer.Points.Hp - enemyDamage;
			if (adventurer.Points.Hp <= 0) 
			{ 
				std::cout << "Your HP is: 0.\n\n";
				std::cout << "You are dead!\n";
				break;	// If the player is dead we exit the turn loop.
			}
		}
		else {
			GlobalGameCounters.AttacksAvoided++;
			std::cout << "The " << currentEnemy.Name << " misses the attack!\n";
		}

		printf("Your HP is: %u.\n", adventurer.Points.Hp);

		// Calculate the player hit chance and apply damage to enemy or just print the miss message.
		if ((rand() % 100) < adventurer.Points.Hit)
		{
			int playerDamage = adventurer.Points.Attack+(rand()%10);
			std::cout << "You hit for: " << playerDamage << "\n";
			currentEnemy.Points.Hp = currentEnemy.Points.Hp - playerDamage;
			
			GlobalGameCounters.DamageDealt += playerDamage;
			GlobalGameCounters.AttacksHit++;

			// Check if the enemy was killed. If it was, we cancel the turn loop after applying drops to the player.
			if (currentEnemy.Points.Hp <= 0)
			{
				std::cout << "The "<< currentEnemy.Name <<" HP is: 0\n\n";
				std::cout << "The " << currentEnemy.Name <<" is dead\n";
				int drop = currentEnemy.Points.Coins + (rand() % 20);
				std::cout << "\nThe enemy dropped " << drop << " coins!!\n\n";
				adventurer.Points.Coins = adventurer.Points.Coins + drop;

				GlobalGameCounters.BattlesWon++;
				GlobalGameCounters.EnemiesKilled++;
				GlobalGameCounters.MoneyEarned += drop;

				break;	// Cancel the combat loop to exit combat.
			}
		}
		else {
			GlobalGameCounters.AttacksMissed++;
			printf("You miss the attack!\n");
		};

		std::cout << "The " << currentEnemy.Name << " HP is: " << currentEnemy.Points.Hp << "\n";
	} 
}

void tavern()
{
	// This is the main loop of the game and queries for user input until the exit option is selected.
	while (true)
	{
		std::cout << "1- For Rest. 2- For a Mercenary Job. 3- For a drink. 4- Exit game.\n";
		char tavernChoice = getchar();
		getchar();

		if( '1' == tavernChoice )		{	rest();			}	// Rest and ask again for the action.
		else if( '2' == tavernChoice )	{	mercenaryJob();	}	// Go for a mercenary job and ask again for action once it's done
		else if( '3' == tavernChoice )	{	drink();		}	// Go to the shop and ask again for action once it's done.
		else if( '4' == tavernChoice )	{	break;			}	// Exit the main loop, which effectively closes the game
		else {	// Enter here if we didn't recognize the option. Print the error message and ask again for input.
			std::cout << "Invalid answer. Answer again...\n"; 
		};

		if (adventurer.Points.Hp <= 0) // If the mercenary job didn't go well we cancel the loop and exit the game.
			break; 
	}
}

void rest()
{
	printf("You decide to get some rest\n");
	adventurer.Points.Hp = adventurer.Points.MaxHp;
	std::cout << "Your HP is: " << adventurer.Points.Hp << "\n";
}

void mercenaryJob()
{
	printf("You decide to enroll for a mercenary job.\n");
	while (true)
	{
		printf("1- Easy Job. 2- Medium Job. 3- Hard Job. 4- Back to the tavern.\n");
		char mercenaryDif = getchar();
		getchar();

		// Set bCombat to true and the enemy type for executing the combat logic.
		bool bCombat = false;
		CHARACTER_TYPE enemyType = CT_UNKNOWN;	

		if('1' == mercenaryDif)			{ bCombat = true;	enemyType	= CT_WOLF;		}
		else if('2' == mercenaryDif)	{ bCombat = true;	enemyType	= CT_RAIDER;	}
		else if('3' == mercenaryDif)	{ bCombat = true;	enemyType	= CT_SOLDIER;	}
		else if('4' == mercenaryDif)	{ // This option cancels the loop which causes to exit to the tavern.
			std::cout << "Welcome back, " << name << ".\n";
			break;
		}
		else {
			printf("Invalid answer. Answer again...\n");
		}

		if( bCombat )
		{
			printf("You challenge a %s.\n", getEnemyDefinition(enemyType).Name.c_str()); 
			combat(enemyType);
			break;	// Just exit this function after the combat ends and automatically go back to the tavern.
		}
	}
}

void addItem(const std::string& itemName)
{
	// look in the inventory for the name so we just increment the counter instead of adding the item
	for(int i=0, count = adventurer.itemCount; i<count; i++) {
		if(itemName == adventurer.inventory[i].Name) {
			adventurer.inventory[i].Count++;
			return;
		}
	}

	// If we didn't return yet it means that the item was not found and we need to add it to the inventory.
	adventurer.inventory[adventurer.itemCount].Name		= itemName;
	adventurer.inventory[adventurer.itemCount].Count	= 1;
	adventurer.itemCount++;
}

void drink()
{
	printf("Do you want to buy some drinks?\n");
	while (true)	// break the loop for exiting the shop
	{
		printf("You have %u coins.\n", adventurer.Points.Coins);
		printf("- Type %u to exit the shop.\n", MAX_ITEM_DESCRIPTIONS+1);
		for(int i=0; i<MAX_ITEM_DESCRIPTIONS; i++)	// Print available items
			printf("- Type %u to buy %s for %u coins.\n", i+1, itemDescriptions[i].Name.c_str(), itemDescriptions[i].Price);

		char drinks = getchar();
		getchar();
		int idItem = drinks - '1';
		
		if( idItem == MAX_ITEM_DESCRIPTIONS ) 
		{
			printf("You leave the shop.\n");
			break;
		}
		else if(idItem >= 0 && idItem < MAX_ITEM_DESCRIPTIONS)
		{
			int itemPrice = itemDescriptions[idItem].Price;
			const std::string itemName = itemDescriptions[idItem].Name;
			if(adventurer.Points.Coins < itemPrice) {
				printf("You can't afford to buy that! Choose something else...\n");
				continue;
			}
			addItem(itemName);
			printf("You spend %u coins in %s.\n", itemPrice, itemName.c_str());
			adventurer.Points.Coins = adventurer.Points.Coins - itemPrice;

			GlobalGameCounters.MoneySpent += itemPrice;
			continue;
		}
		else
		{
			printf("You can't buy something that doesn't exist!\n");
			continue;
		}
	}
	std::cout << "\n";
	showInventory();
}

void showInventory()
{
	std::cout << "Your inventory:\n\n";
	std::cout << "You have " << adventurer.Points.Coins << " coins.\n\n";
	for (int i = 0; i < adventurer.itemCount; i++)
		std::cout << i + 1 << " - "<< adventurer.inventory[i].Name << ": " << adventurer.inventory[i].Count << "\n";
}

void useItems()
{
	printf("\nUse an Item or press %u to continue.\n\n", MAX_INVENTORY_SLOTS+1);

	bool bUsedItem = false;
	int indexItem = -1;
	std::string itemName;
	while(true)
	{
		showInventory();
		printf("- Type %u to continue.\n", MAX_INVENTORY_SLOTS+1);
		char item = getchar();
		getchar();

		indexItem = item - '1';
		if(indexItem == MAX_INVENTORY_SLOTS) // exit option
			break;
		else if(indexItem < 0 || indexItem >= adventurer.itemCount ) {	// invalid index means it's an invalid option
			std::cout << "Invalid answer. Answer again...\n";
			continue;
		}

		itemName = adventurer.inventory[indexItem].Name;

		if (adventurer.inventory[indexItem].Count <= 0) { 
			printf("You don't have anymore of that. Use something else...\n"); 
			continue; 
		};

		if( itemName == "Small HP Potion" )			{ adventurer.Points.Hp = (adventurer.Points.Hp+ 10+(rand()%10)); printf("You feel slightly better.\n");		bUsedItem = true; GlobalGameCounters.PotionsUsed++; break; }
		else if( itemName == "Medium HP Potion" )	{ adventurer.Points.Hp = (adventurer.Points.Hp+ 50+(rand()%10)); printf("You feel better.\n");				bUsedItem = true; GlobalGameCounters.PotionsUsed++; break; }
		else if( itemName == "Large HP Potion" )	{ adventurer.Points.Hp = (adventurer.Points.Hp+100+(rand()%10)); printf("You feel incredibly better.\n");	bUsedItem = true; GlobalGameCounters.PotionsUsed++; break; }
		else {
			printf("Unrecognized item found in inventory! Item name: %s.\n", itemName.c_str());
			continue;
		}
	}

	if(bUsedItem) {
		printf("You used: %s.\n", itemName.c_str());
		adventurer.inventory[indexItem].Count--;
	}
}
