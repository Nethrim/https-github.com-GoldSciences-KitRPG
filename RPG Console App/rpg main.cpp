#include <iostream>
#include <stdio.h>
#include <time.h>   
#include <windows.h>
#include <string>
#include "Enemy.h"
#include "Player.h"

using namespace std;
enemy wolf		  (100,  5, 1000,    "Wolf", 10); //HP, Atk, HitChance, name, dropCoins.
enemy raider	  (150,  8, 1000,  "Raider", 50); //HP, Atk, HitChance, name, dropCoins.
enemy soldier	  (250,  50, 1000, "Soldier",100); //HP, Atk, HitChance, name, dropCoins.
player adventurer (200, 10, 1000, 0); //HP, ATk, HitChance, Coins.
bool game = true;
bool valid;
bool difValid;
bool drinkVal;
bool combatVal;
bool itemVal;
const int MAX_ITEMS = 3;
string inventory[MAX_ITEMS];
string name;
int numItems = 0;
int maxHP = 200;
char drinks;
char action;
char item;
char tabernSwitch;
char mercenaryDif;
void tavern();
void rest();
void mercenaryJob();
void drink();
void showInventory();
void useItems();
    
void main()
{	
	srand(time(NULL));
	inventory[numItems++] = "EMPTY";
	inventory[numItems++] = "EMPTY";
	inventory[numItems++] = "EMPTY";
	cout << "Welcome Stranger!! who are you ?" << endl;
	cout << "My name is: " << endl;
	getline(cin, name);
	cout << "So " << name << "...What's bring you here?" << endl;

	while (game)
	{	
		tavern();				
	}

	cout << "\nGame Over\n" << endl;
	system("PAUSE");
}

enum ENEMY_TYPE
{
	WOLF,
	RAIDER,
	SOLDIER
};

void combat(ENEMY_TYPE enemyType)
{
	enemy currentEnemy;

	switch(enemyType) 
	{
	case WOLF:		currentEnemy.setEnemChit(wolf.getEnemChit	());	currentEnemy.setEnemAttack(wolf.getEnemAttack	());	currentEnemy.setEnemHp(wolf.getEnemHp());		currentEnemy.setEnemName(wolf.getEnemName	());	currentEnemy.setEnemDrop(wolf.getEnemDrop	()); break;
	case RAIDER:	currentEnemy.setEnemChit(raider.getEnemChit ());	currentEnemy.setEnemAttack(raider.getEnemAttack ());	currentEnemy.setEnemHp(raider.getEnemHp());		currentEnemy.setEnemName(raider.getEnemName ());	currentEnemy.setEnemDrop(raider.getEnemDrop	()); break;
	case SOLDIER:	currentEnemy.setEnemChit(soldier.getEnemChit());	currentEnemy.setEnemAttack(soldier.getEnemAttack());	currentEnemy.setEnemHp(soldier.getEnemHp());	currentEnemy.setEnemName(soldier.getEnemName());	currentEnemy.setEnemDrop(soldier.getEnemDrop()); break;
	}

	while (adventurer.getPlayerHp() >= 0 && currentEnemy.getEnemHp() >= 0)
	{	
		combatVal = false;
		while (!combatVal)
		{
			cout << endl;
			cout << "Select Action:" << endl;
			cout << endl;
			cout << "1-Atack 2-Inventory 3-Run" << endl;
			cout << endl;			
			action = getchar();
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

		int hitDice = (1 + (rand() % 10));

		if (hitDice < currentEnemy.getEnemChit())
		{
			int enemyDamage = currentEnemy.getEnemAttack()+(rand()%10);
			cout << "The " << currentEnemy.getEnemName() << " hits you for: " << enemyDamage << endl;
			adventurer.setPlayerHp(adventurer.getPlayerHp() - enemyDamage);
			cout << "Your HP is: " << adventurer.getPlayerHp() << endl;
		}
		else {
			cout << "The " << currentEnemy.getEnemName() << " miss the attack!" << endl;
			cout << "Your HP is: " << adventurer.getPlayerHp();
		}
		if (adventurer.getPlayerHp() < 1) { break; }

		int playerDice = (1 + (rand() % 10));

		if (playerDice < adventurer.getPlayerChit())
		{
			int playerDamage = adventurer.getPlayerAttack()+(rand()%10);
			cout << "You hit for: " << playerDamage << endl;
			currentEnemy.setEnemHp(currentEnemy.getEnemHp() - playerDamage);
			cout << "The " << currentEnemy.getEnemName() << " HP is : " << currentEnemy.getEnemHp() << endl;
		}
		else {
			cout << "You miss the attack!" << endl;
			cout << "The " << currentEnemy.getEnemName() << " HP is : " << currentEnemy.getEnemHp() << endl;
		};

		if (adventurer.getPlayerHp() <= 0){	game = !game; break;}
		if (currentEnemy.getEnemHp() <= 0)
		{
			int drop = currentEnemy.getEnemDrop() + (rand() % 20);
			cout << endl;			
			cout << "The enemy has drop " << drop << " coins!!" << endl;
			cout << endl;
			adventurer.setPlayerCoins(adventurer.getPlayerCoins() + drop);
		}
	}
}

void tavern()
{
	valid = false;
	
	while (!valid)
	{
		if (adventurer.getPlayerHp() <= 0) { game = !game; break; }
		cout << "1- for Rest. 2-for a Mercenary Job. 3-for a drink. " << endl;
		tabernSwitch = getchar();
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
			cout << "answer again" << endl;
			valid = false;
			continue;
		}
	}
}

void rest()
{
	cout << "rest" << endl;
	adventurer.setPlayerHp(maxHP);
	cout << "Your HP is: " << adventurer.getPlayerHp() << endl;
}

void mercenaryJob()
{
	cout << "mercenary job" << endl;
	while (!difValid)
	{
		if (adventurer.getPlayerHp() <= 0) { game = !game; break; }
		cout << "1-Easy Job. 2-Medium Job. 3-Hard Job. 4-Back to the Tabern." << endl;
		mercenaryDif = getchar();
		getchar();
		switch (mercenaryDif)
		{
		case '1':
			cout << "Wolf fight" << endl;
			combat(WOLF);
			break;
		case '2':
			cout << "Raider fight" << endl;
			combat(RAIDER);
			break;
		case '3':
			cout << "Soldier fight" << endl;
			combat(SOLDIER);
			break;
		case '4':
			cout << "Welcome again " << name << endl;
			tavern();
			break;
		default:
			cout << "answer again" << endl;
			difValid = false;
		}
	}
}

void drink()
{
	drinkVal = false;
	while (!drinkVal)
	{
		cout << "Do you want to buy some drinks?" << endl;
		cout << endl;
		cout << "1- Small HP potion - 50 coins" << endl;
		cout << endl;
		cout << "2- Regular HP potion- 100 coins" << endl;
		cout << endl;
		cout << "3- Large HP potion- 150 coins" << endl;
		cout << endl;
		drinks = getchar();
		getchar();
		switch (drinks)
		{
		case '1':
			if (inventory[0] == "Small Hp potion")
			{
				cout << "You can't carry any more" << endl;
				drinkVal = true;
				break;
			}
			if (adventurer.getPlayerCoins() >= 50)
			{
				cout << "You bought a small HP potion" << endl;
				cout << endl;
				inventory[0] = "Small Hp potion";
				adventurer.setPlayerCoins(adventurer.getPlayerCoins() - 50);
			}
			else {
				cout << "Come back when you've got some money, scum!!";
				cout << endl;
			}
			drinkVal = true;
			break;
		case '2':
			if (inventory[1] == "Regular Hp potion")
			{
				cout << "You can't carry any more" << endl;
				drinkVal = true;
				break;
			}
			if (adventurer.getPlayerCoins() >= 100)
			{
				cout << "You bought a regular HP potion" << endl;
				cout << endl;
				inventory[1] = "Regular Hp potion";
				adventurer.setPlayerCoins(adventurer.getPlayerCoins() - 100);
			}else {
				cout << "Come back when you have some coins maggot!!";
				cout << endl;
			}
			drinkVal = true;
			break;
		case '3':
			if (inventory[2] == "Large Hp potion")
			{
				cout << "You can't carry any more" << endl;
				drinkVal = true;
				break;
			}
			if (adventurer.getPlayerCoins() >= 150)
			{
				cout << endl;
				cout << "You bought a large HP potion" << endl;
				inventory[2] = "Large Hp potion";
				adventurer.setPlayerCoins(adventurer.getPlayerCoins() - 150);
			}else {
				cout << "Come back when you have some coins maggot!!";
				cout << endl;
			}
			drinkVal = true;
			break;
		default:
			cout << "answer again" << endl;
			drinkVal = false;
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
	for (i = 0; i < numItems; i++)
	{
		cout<< i + 1<<"- "<< inventory[i] << endl;
		cout << endl;
	}
	
}

void useItems()
{
	cout << endl;
	cout << "Use an Item or press 4 to continue" << endl;
	cout << endl;
	itemVal = false;
	while (!itemVal)
	{
		showInventory();
		cout << "4- Continue" << endl;
		cout << endl;
		item = getchar();
		getchar();
		switch (item)
		{
		case '1':
			
			if (inventory[0] == "Small Hp potion")
			{
				cout << "You use a small HP potion" << endl;
				inventory[0] = "EMPTY";
				adventurer.setPlayerHp(adventurer.getPlayerHp()+10+(rand()%10));
				if (adventurer.getPlayerHp() > maxHP) { adventurer.setPlayerHp(maxHP); }
				itemVal = true;
			}
			else
			{
				cout << "It's empty !!" << endl;
				itemVal = false;
			}
			break;
		case '2':
			if (inventory[1] == "Regular Hp potion")
			{
				cout << "You use a regular HP potion" << endl;
				inventory[1] = "EMPTY";
				adventurer.setPlayerHp(adventurer.getPlayerHp()+50+(rand()%50));
				if (adventurer.getPlayerHp() > maxHP) { adventurer.setPlayerHp(maxHP); }
				itemVal = true;
			}
			else
			{
				cout << "It's empty !!" << endl;
				itemVal = false;
			}
			break;
		case '3':
			if (inventory[2] == "Large Hp potion")
			{
				cout << "You use a large HP potion" << endl;
				inventory[2] = "EMPTY";
				adventurer.setPlayerHp(adventurer.getPlayerHp()+100+(rand()%100));
				if (adventurer.getPlayerHp() > maxHP) {adventurer.setPlayerHp(maxHP);}
				itemVal = true;
			}
			else
			{
				cout << "It's empty !!" << endl;
				itemVal = false;
			}
			break;
		case '4':
			itemVal = true;
			break;
		default:
			cout << "answer again" << endl;
			itemVal = false;
			continue;
		}
	}
}



// calcular un chance hit balanceado.