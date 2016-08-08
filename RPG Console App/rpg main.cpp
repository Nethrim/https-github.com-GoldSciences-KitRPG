#include <iostream>
#include <stdio.h>
#include <time.h>   
#include <windows.h>
#include <string>
#include "Enemy.h"
#include "Player.h"

using namespace std;

string name;
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

	cout << "Welcome Stranger!! who are you?\n";
	cout << "My name is: \n";
	getline(cin, name);
	cout << "So " << name << "...What's bring you here?\n";

	while (game)
	{	
		tavern();				
	}

	cout << "\nGame Over\n" << endl;
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
			cout << endl;
			cout << "Select Action:" << endl;
			cout << endl;
			cout << "1-Attack 2-Inventory 3-Run" << endl;
			cout << endl;			
			char action = getchar();
			getchar();
			switch (action)
			{
			case '1':
				cout << "You decide to attack!" << endl;				
				combatVal = true;
				break;
			case '2':
				useItems();
				combatVal = true;
				break;
			case '3':
				cout << "You are trying to escape!" << endl;
				cout << endl;
				if ((rand() % 100) <30)
				{
					cout << "You escaped from combat!" << endl;
					cout << endl;
					tavern();
				}
				else {
					cout << "You failed to escape!" << endl;
					cout << endl;
				}
				
				combatVal = true;
				break;
			default:
				cout << "answer again" << endl;
				combatVal = false;
				continue;
			}
		}
				
		if ((rand() % 100) < currentEnemy.getEnemChit() )
		{
			int enemyDamage = currentEnemy.getEnemAttack()+(rand()%10);
			cout << "The " << currentEnemy.getEnemName() << " hits you for: " << enemyDamage << endl;

			adventurer.setPlayerHp(adventurer.getPlayerHp() - enemyDamage);
			if (adventurer.getPlayerHp() <= 0) 
			{ 
				std::cout << "Your HP is: 0" << endl; 
				std::cout << endl;
				std::cout << "You are dead!" << endl;
			}
			else {
				std::cout << "Your HP is: " << adventurer.getPlayerHp() << endl;
			}
		}
		else {
			std::cout << "The " << currentEnemy.getEnemName() << " miss the attack!" << endl;
			std::cout << "Your HP is: " << adventurer.getPlayerHp() << endl;
			
		}
		if (adventurer.getPlayerHp() < 1) { break; }

		if ((rand() % 100) < adventurer.getPlayerChit())
		{
			int playerDamage = adventurer.getPlayerAttack()+(rand()%10);
			std::cout << "You hit for: " << playerDamage << endl;
			currentEnemy.setEnemHp(currentEnemy.getEnemHp() - playerDamage);
			if (currentEnemy.getEnemHp() <= 0)
			{
				std::cout << "The "<< currentEnemy.getEnemName() <<" HP is: 0" << endl;
				std::cout << endl;
				std::cout << "The " << currentEnemy.getEnemName() <<" is dead" << endl;
			}
			else {
				std::cout << "The " << currentEnemy.getEnemName() << " HP is : " << currentEnemy.getEnemHp() << endl;
			}
		}
		else {
			std::cout << "You miss the attack!" << endl;
			std::cout << "The " << currentEnemy.getEnemName() << " HP is : " << currentEnemy.getEnemHp() << endl;
		};

		if (adventurer.getPlayerHp() <= 0){	game = !game; break;}
		if (currentEnemy.getEnemHp() <= 0)
		{
			int drop = currentEnemy.getEnemDrop() + (rand() % 20);
			std::cout << endl;			
			std::cout << "The enemy has drop " << drop << " coins!!" << endl;
			std::cout << endl;
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
		std::cout << "1- for Rest. 2-for a Mercenary Job. 3-for a drink. " << endl;
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
			std::cout << "answer again" << endl;
			valid = false;
			continue;
		}
	}
}

void rest()
{
	std::cout << "rest" << endl;
	adventurer.setPlayerHp(maxHP);
	std::cout << "Your HP is: " << adventurer.getPlayerHp() << endl;
}

void mercenaryJob()
{
	std::cout << "mercenary job" << endl;
	bool difValid = false;
	while (!difValid)
	{
		if (adventurer.getPlayerHp() <= 0) { game = !game; break; }
		std::cout << "1-Easy Job. 2-Medium Job. 3-Hard Job. 4-Back to the Tabern." << endl;
		char mercenaryDif = getchar();
		getchar();
		switch (mercenaryDif)
		{
		case '1':
			std::cout << "Wolf fight" << endl;
			combat(WOLF);
			break;
		case '2':
			std::cout << "Raider fight" << endl;
			combat(RAIDER);
			break;
		case '3':
			printf("Soldier fight");
			combat(SOLDIER);
			break;
		case '4':
			std::cout << "Welcome again, " << name << endl;
			tavern();
			break;
		default:
			std::cout << "answer again" << endl;
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
			const string itemName = itemDescriptions[idItem].Name;
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
	cout << endl;
	showInventory();
}

void showInventory()
{
	cout << "Your inventory: " << endl;
	cout << endl;
	cout << "Your have " << adventurer.getPlayerCoins() << " coins" << endl;
	cout << endl;
	int i;
	for (i = 0; i < adventurer.itemCount; i++)
	{
		cout << i + 1 << " - "<< adventurer.inventory[i].Name << ": " << adventurer.inventory[i].Count;
		cout << endl;
	}
	
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
		cout << endl;
		char item = getchar();
		getchar();

		indexItem = item - '1';
		if(indexItem < 0) {
			cout << "answer again" << endl;
			continue;
		}
		else if(indexItem == MAX_INVENTORY_SLOTS) {	// exit 
			break;
		}
		else if( indexItem > adventurer.itemCount ) {
			cout << "answer again" << endl;
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
