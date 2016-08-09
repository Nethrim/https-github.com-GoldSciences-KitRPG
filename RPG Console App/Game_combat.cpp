#include "Game.h"
#include "Menu.h"

bool useItems(CGame& gameInstance);	// While in combat, displays a list of the available items to use.

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

void combat(CGame& gameInstance, CHARACTER_TYPE enemyType)
{
	gameInstance.CurrentEnemy = getEnemyDefinition(enemyType);	// Request the enemy data.

	CCharacter& currentEnemy = gameInstance.CurrentEnemy;
	CPlayer& adventurer = gameInstance.Adventurer;
	SGameCounters& gameCounters = gameInstance.Statisticts;

	static const SMenuItem combatOptions[] =
	{ { 1, "Attack"		}
	, { 2, "Inventory"	}
	, { 3, "Run"		}
	};

	while (adventurer.Points.HP > 0 && currentEnemy.Points.HP > 0)	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		gameCounters.TurnsPlayed++;

		while (true)	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
		{
			printf("\n-- Your HP is: %u.\n", adventurer.Points.HP);
			printf("-- %s HP is: %u.\n", currentEnemy.Name.c_str(), currentEnemy.Points.HP);

			int actionChoice = displayMenu("It's your turn to make a move", combatOptions);
			
			// If the action is valid then we execute it and break the current while() so the attack turn executes.
			if(1 == actionChoice) { 
				printf("You decide to attack!\n"); 

				int damageDealt = attack(adventurer, currentEnemy);
				if(damageDealt) {
					gameCounters.DamageDealt += damageDealt;
					gameCounters.AttacksHit++;
				}
				else 
					gameCounters.AttacksMissed++;

				break; 
			}
			else if(2 == actionChoice) { 
				if( useItems(gameInstance) )	// If no items were used we ask again for user input. 
					break; 
				else
					continue;
			}	// useItems requires to receive the current enemy as a parameter in order to modify its health if we use a grenade and hit.
			else if(3 == actionChoice) { // Escape: if we succeed we just exit this combat() function, otherwise cancel this loop and execute the attack turn.
				printf("You try to escape!\n\n");
				if ((rand() % 100) < 30) {
					printf("You escaped from combat!\n");
					gameCounters.EscapesSucceeded++;
					return;
				}

				gameCounters.EscapesFailed++;
				printf("You failed to escape!\n");
				break;
			}
		}

		if (adventurer.Points.HP <= 0  || currentEnemy.Points.HP <= 0)	// We do this check because player actions may kill one of the combatants and in that case we need to exit this loop.
			break;	// Cancel the combat loop to exit combat.

		// Execute enemy attack turn
		int damageDealt = attack(currentEnemy, adventurer);
		if(damageDealt) {
			gameCounters.DamageTaken += damageDealt;
			gameCounters.AttacksReceived++;
		}
		else
			gameCounters.AttacksAvoided++;
	} 

	// Determine the outcome of the battle and give rewards if applicable.
	if (adventurer.Points.HP <= 0) 
		printf("You are dead!\n");
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

		gameCounters.BattlesWon++;
		gameCounters.EnemiesKilled++;
		gameCounters.MoneyEarned += drop;
	}
}

// This function returns true if an item was used or false if the menu was exited without doing anything.
bool useItems(CGame& gameInstance)
{
	CPlayer& adventurer = gameInstance.Adventurer;
	SGameCounters& gameCounters = gameInstance.Statisticts;

	bool bUsedItem = false;
	unsigned int indexItem = ~0U;
	SItem itemDescription;
	while(true)
	{
		printf("- Type %u to close your inventory.\n", getInventorySize(adventurer.inventory)+1);
		showInventory(adventurer);

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
			gameCounters.PotionsUsed++;
			break;

		case IT_GRENADE:
			printf("You throw that shit to the enemy.\n");
			itemEffectValue = int(gameInstance.CurrentEnemy.Points.MaxHP*(0.2f*itemDescription.Grade)); // this is the total damage points applied to the enemy or player

			lotteryRange = 60+(10*itemDescription.Grade);
			lotteryResult = rand()%100;
			if( lotteryResult < lotteryRange)
			{
				gameInstance.CurrentEnemy.Points.HP -= itemEffectValue;	// this is the resulting hp after applying the damage 
				gameCounters.DamageDealt += itemEffectValue;
				printf("The grenade hits the enemy doing %u damage.\n", itemEffectValue);
			}
			else if( lotteryResult == lotteryRange )
			{
				adventurer.Points.HP -= itemEffectValue/2;	// this is the resulting hp after applying the damage 
				gameCounters.DamageTaken += itemEffectValue;
				printf("You throw the grenade too close...\n"		
					"The grenade explodes near you hurting you by %u damage!\n", itemEffectValue);
			}
			else
				printf("You throw the grenade too far away.\n");

			gameCounters.GrenadesUsed++;
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