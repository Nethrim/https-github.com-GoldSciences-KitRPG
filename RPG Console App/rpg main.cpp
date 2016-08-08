#include <iostream>
#include <stdio.h>
#include <time.h>   
#include <windows.h>
#include <string>
#include "Enemy.h"
#include "Player.h"

std::string name;
int maxHP = 200;

CPlayer adventurer (200, 6, 50, 50); //HP, ATk, HitChance, Coins.

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

	tavern();				

	printf("\nGame Over!\n\n");
	system("PAUSE");
}


void combat(ENEMY_TYPE enemyType)
{
	CEnemy currentEnemy = getEnemyDefinition(enemyType);

	while (adventurer.getPlayerHp() > 0 && currentEnemy.getEnemHp() > 0)
	{	
		bool combatVal = false;
		while (!combatVal)
		{
			printf("\nSelect Action:\n");
			printf("1-Attack 2-Inventory 3-Run\n\n");
			char action = getchar();
			getchar();
			switch (action)
			{
			case '1':
				printf("You decide to attack!\n");
				combatVal = true;
				break;
			case '2':
				useItems();
				combatVal = true;
				break;
			case '3':
				std::cout << "You try to escape!\n\n";
				if ((rand() % 100) < 30)
				{
					printf("You escaped from combat!\n");
					return;
				}
				else {
					printf("You failed to escape!\n");
				}
				
				combatVal = true;
				break;
			default:
				printf("Invalid action.\n");
				combatVal = false;
				continue;
			}
		}
				
		if ((rand() % 100) < currentEnemy.getEnemChit() )
		{
			int enemyDamage = currentEnemy.getEnemAttack()+(rand()%10);
			printf("The %s hits you for %u.\n", currentEnemy.getEnemName().c_str(), enemyDamage);

			adventurer.setPlayerHp(adventurer.getPlayerHp() - enemyDamage);
			if (adventurer.getPlayerHp() <= 0) 
			{ 
				std::cout << "Your HP is: 0.\n\n";
				std::cout << "You are dead!\n";
			}
			else {
				printf("Your HP is: %u.\n", adventurer.getPlayerHp());
			}
		}
		else {
			std::cout << "The " << currentEnemy.getEnemName() << " misses the attack!\n";
			std::cout << "Your HP is: " << adventurer.getPlayerHp() << "\n";
			
		}

		if (adventurer.getPlayerHp() <= 0) {	
			break;
		}

		if ((rand() % 100) < adventurer.getPlayerChit())
		{
			int playerDamage = adventurer.getPlayerAttack()+(rand()%10);
			std::cout << "You hit for: " << playerDamage << "\n";
			currentEnemy.setEnemHp(currentEnemy.getEnemHp() - playerDamage);
			if (currentEnemy.getEnemHp() <= 0)
			{
				std::cout << "The "<< currentEnemy.getEnemName() <<" HP is: 0" << "\n\n";
				std::cout << "The " << currentEnemy.getEnemName() <<" is dead" << "\n";
			}
			else {
				std::cout << "The " << currentEnemy.getEnemName() << " HP is : " << currentEnemy.getEnemHp() << "\n";
			}
		}
		else {
			std::cout << "You miss the attack!\n";
			std::cout << "The " << currentEnemy.getEnemName() << " HP is : " << currentEnemy.getEnemHp() << "\n";
		};

		if (adventurer.getPlayerHp() <= 0) {	
			break;
		}
		if (currentEnemy.getEnemHp() <= 0)
		{
			int drop = currentEnemy.getEnemDrop() + (rand() % 20);
			std::cout << "\nThe enemy dropped " << drop << " coins!!\n\n";
			adventurer.setPlayerCoins(adventurer.getPlayerCoins() + drop);
		}
	} 
}

void tavern()
{
	while (true)
	{
		std::cout << "1- For Rest. 2- For a Mercenary Job. 3- For a drink. 4- Exit game.\n";
		char tavernChoice = getchar();
		getchar();
		if( '1' == tavernChoice )		{	rest();			}
		else if( '2' == tavernChoice )	{	mercenaryJob();	}
		else if( '3' == tavernChoice )	{	drink();		}
		else if( '4' == tavernChoice )	{	break;			}
		else {
			std::cout << "Invalid answer. Answer again...\n"; 
		};

		if (adventurer.getPlayerHp() <= 0) 
			break; 
	}
}

void rest()
{
	std::cout << "You decide to get some rest\n";
	adventurer.setPlayerHp(maxHP);
	std::cout << "Your HP is: " << adventurer.getPlayerHp() << "\n";
}

void mercenaryJob()
{
	std::cout << "You decide to enroll for a mercenary job.\n";
	while (true)
	{
		if (adventurer.getPlayerHp() <= 0) 
			break;

		printf("1- Easy Job. 2- Medium Job. 3- Hard Job. 4- Back to the tavern.\n");
		char mercenaryDif = getchar();
		getchar();
		if('1' == mercenaryDif)			{ printf("You challenge a %s.\n", getEnemyDefinition(WOLF)		.getEnemName().c_str()); combat(WOLF);		break; }
		else if('2' == mercenaryDif)	{ printf("You challenge a %s.\n", getEnemyDefinition(RAIDER)	.getEnemName().c_str()); combat(RAIDER);	break; }
		else if('3' == mercenaryDif)	{ printf("You challenge a %s.\n", getEnemyDefinition(SOLDIER)	.getEnemName().c_str()); combat(SOLDIER);	break; }
		else if('4' == mercenaryDif)	{
			std::cout << "Welcome back, " << name << ".\n";
			break;
		}
		else {
			printf("Invalid answer. Answer again...\n");
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
	while (true)
	{
		printf("You have %u coins.\n", adventurer.getPlayerCoins());
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
			if(adventurer.getPlayerCoins() < itemPrice) {
				printf("You can't afford to buy that! Choose something else...\n");
				continue;
			}
			addItem(itemName);
			printf("You spend %u coins in %s.\n", itemPrice, itemName.c_str());
			adventurer.setPlayerCoins(adventurer.getPlayerCoins() - itemPrice);
			break;
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
	std::cout << "You have " << adventurer.getPlayerCoins() << " coins.\n\n";
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

		if( itemName == "Small HP Potion" )			{ adventurer.setPlayerHp(adventurer.getPlayerHp()+ 10+(rand()%10)); printf("You feel slightly better.\n");		bUsedItem = true; break; }
		else if( itemName == "Medium HP Potion" )	{ adventurer.setPlayerHp(adventurer.getPlayerHp()+ 50+(rand()%10)); printf("You feel better.\n");				bUsedItem = true; break; }
		else if( itemName == "Large HP Potion" )	{ adventurer.setPlayerHp(adventurer.getPlayerHp()+100+(rand()%10)); printf("You feel incredibly better.\n");	bUsedItem = true; break; }
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
