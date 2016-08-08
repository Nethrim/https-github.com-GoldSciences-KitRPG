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

bool game = true;
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
	std::cout << "So " << name << "...What's bring you here?\n";

	while (game)
	{	
		tavern();				
	}

	printf("\nGame Over\n\n");
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
				std::cout << "You are trying to escape!\n\n";
				if ((rand() % 100) <30)
				{
					printf("You escaped from combat!\n");
					tavern();
				}
				else {
					printf("You failed to escape!\n");
				}
				
				combatVal = true;
				break;
			default:
				printf("Invalid action\n");
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
		if (adventurer.getPlayerHp() < 1) { break; }

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

		if (adventurer.getPlayerHp() <= 0){	game = !game; break;}
		if (currentEnemy.getEnemHp() <= 0)
		{
			int drop = currentEnemy.getEnemDrop() + (rand() % 20);
			std::cout << "\nThe enemy has drop " << drop << " coins!!\n\n";
			adventurer.setPlayerCoins(adventurer.getPlayerCoins() + drop);
		}
	} 
}

void tavern()
{
	bool valid = false;
	
	while (!valid)
	{
		if (adventurer.getPlayerHp() <= 0) { game = !game; break; }
		std::cout << "1- for Rest. 2-for a Mercenary Job. 3-for a drink.\n";
		char tabernSwitch = getchar();
		getchar();
		switch (tabernSwitch)
		{
		case '1':
			rest();
			valid = false;
			break;
		case '2':
			mercenaryJob();
			valid = true;
			break;
		case '3':
			drink();
			valid = false;
			break;
		default:
			std::cout << "Invalid answer. Answer again...\n";
			valid = false;
			continue;
		}
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
	bool difValid = false;
	while (!difValid)
	{
		if (adventurer.getPlayerHp() <= 0) { game = !game; break; }
		std::cout << "1-Easy Job. 2-Medium Job. 3-Hard Job. 4-Back to the Tabern.\n";
		char mercenaryDif = getchar();
		getchar();
		switch (mercenaryDif)
		{
		case '1':
			std::cout << "Wolf fight\n";
			combat(WOLF);
			break;
		case '2':
			std::cout << "Raider fight\n";
			combat(RAIDER);
			break;
		case '3':
			printf("Soldier fight");
			combat(SOLDIER);
			break;
		case '4':
			std::cout << "Welcome back, " << name << "\n";
			tavern();
			break;
		default:
			std::cout << "answer again\n";
		}
	}
}

void addItem(const std::string& itemName)
{
	for(int i=0, count = adventurer.itemCount; i<count; i++) {
		if(itemName == adventurer.inventory[i].Name) {
			adventurer.inventory[i].Count++;
			return;
		}
	}

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
		printf("- Type %u to exit the tavern.\n", MAX_ITEM_DESCRIPTIONS+1);
		for(int i=0; i<MAX_ITEM_DESCRIPTIONS; i++)
			printf("- Type %u to buy %s for %u coins.\n", i+1, itemDescriptions[i].Name.c_str(), itemDescriptions[i].Price);

		char drinks = getchar();
		getchar();
		int idItem = drinks - '1';
		
		if( idItem == MAX_ITEM_DESCRIPTIONS ) 
		{
			printf("You leave the tavern.\n");
			break;
		}
		else if(idItem >= 0 && idItem < MAX_ITEM_DESCRIPTIONS)
		{
			int itemPrice = itemDescriptions[idItem].Price;
			const std::string itemName = itemDescriptions[idItem].Name;
			if(adventurer.getPlayerCoins() < itemPrice) {
				printf("You can't afford to buy that!\n");
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
	while (true)
	{
		showInventory();
		printf("- Type %u to continue.\n", MAX_INVENTORY_SLOTS+1);
		char item = getchar();
		getchar();

		indexItem = item - '1';
		if(indexItem < 0) {
			std::cout << "answer again\n";
			continue;
		}
		else if(indexItem == MAX_INVENTORY_SLOTS) {	// exit 
			break;
		}
		else if( indexItem > adventurer.itemCount ) {
			std::cout << "answer again\n";
			continue;
		}

		itemName = adventurer.inventory[indexItem].Name;
		int itemCount = adventurer.inventory[indexItem].Count;

		if (itemCount <= 0) { 
			printf("You don't have anymore of that.\n"); 
			continue; 
		};

		if( itemName == "Small HP Potion" )			{ adventurer.setPlayerHp(adventurer.getPlayerHp()+ 10+(rand()%10)); printf("You feel slightly better.\n");		bUsedItem = true; break; }
		else if( itemName == "Medium HP Potion" )	{ adventurer.setPlayerHp(adventurer.getPlayerHp()+ 50+(rand()%10)); printf("You feel better.\n");				bUsedItem = true; break; }
		else if( itemName == "Large HP Potion" )	{ adventurer.setPlayerHp(adventurer.getPlayerHp()+100+(rand()%10)); printf("You feel incredibly better.\n");	bUsedItem = true; break; }
		else {
			printf("Unrecognized item found in inventory!\n");
			continue;
		}
	}

	if(bUsedItem)
	{
		printf("You used: %s.\n", itemName.c_str());
		adventurer.inventory[indexItem].Count--;
	}

}
