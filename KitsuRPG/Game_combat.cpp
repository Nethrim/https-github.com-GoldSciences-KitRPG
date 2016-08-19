#include "Game.h"

#include "Profession.h"
#include "Weapon.h"
#include "Item.h"
#include "Enemy.h"
#include "Armor.h"

#include "Menu.h"

bool useItems(CCharacter& adventurer, CCharacter& target);	// While in combat, displays a list of the available items to use.

bool escape(const std::string& escaperName, SCharacterScore& escaperScore)
{
	printf("%s tries to escape!\n", escaperName.c_str());
	if ((rand() % 100) < 30) {
		escaperScore.EscapesSucceeded++;
		printf("%s fled from the battle!", escaperName.c_str());
		return true;
	}

	escaperScore.EscapesFailed++;
	printf("%s failed to escape!\n\n", escaperName.c_str());
	return false;
}

void assignDrops(CCharacter& winner, CCharacter& loser)
{
	printf("%s is dead!\n", loser.Name.c_str());
	int drop = rand() % (std::max(1,loser.Points.Coins>>2));

	if(loser.Type == CHARACTER_TYPE_ENEMY)
		drop = loser.Points.Coins-drop;

	printf("\n%s dropped %u coins!!\n", loser.Name.c_str(), drop);
	winner.Points.Coins += drop;
	loser.Points.Coins	-= drop;
	for(uint32_t i=0; i<loser.Inventory.ItemCount; i++) 
		if(rand()%2) 
		{
			const SInventorySlot& itemDrop = loser.Inventory.Slots[i];
			std::string itemDropName = getItemName(itemDrop.Item);
			if(klib::addItem(winner.Inventory, itemDrop.Item)) {
				printf("\n%s dropped %s!!\n", loser.Name.c_str(), itemDropName.c_str());
			}
			else {
				printf("%s can't pick up %s by %s because the inventory is full!\n", winner.Name.c_str(), itemDropName.c_str(), loser.Name.c_str());
			}

			klib::removeItem(loser.Inventory, i, loser.Name);
		}

	SWeapon oldWinnerWeapon		= winner.Weapon;
	std::string loserWeaponName = getWeaponName(loser.Weapon);
	if(loser.Weapon.Index > winner.Weapon.Index)
	{
		if(rand()%2)
		{
			printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(),loserWeaponName.c_str(), loser.Weapon.Level, loser.Name.c_str());
			winner.Weapon			= loser.Weapon;
			loser.Weapon.Index		= rand() % (oldWinnerWeapon.Index+1);
			loser.Weapon.Modifier	= rand() % (oldWinnerWeapon.Modifier+1);
			loser.Weapon.Level		= 1+(rand() % (oldWinnerWeapon.Level+1));
			if(loser.Weapon.Index)
				printf("%s recovers a used %s from the battlefield.\n", loser.Name.c_str(), getWeaponName(loser.Weapon).c_str());
		}
		else
		{
			printf("%s dropped by %s is too damaged to be recovered.\n", loserWeaponName.c_str(), loser.Name.c_str());
			loser.Weapon.Index		= 0;
			loser.Weapon.Modifier	= 0;
			loser.Weapon.Level		= 1;
		}
	}
	else if(loser.Weapon.Index == winner.Weapon.Index && loser.Weapon.Modifier > winner.Weapon.Modifier)
	{
		if(rand()%2)
		{
			printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(), loserWeaponName.c_str(), loser.Weapon.Level, loser.Name.c_str());
			winner.Weapon.Modifier	= loser.Weapon.Modifier;
			winner.Weapon.Level		= loser.Weapon.Level;
			loser.Weapon.Index		= rand() % (oldWinnerWeapon.Index+1);
			loser.Weapon.Modifier	= rand() % (oldWinnerWeapon.Modifier+1);
			loser.Weapon.Level		= 1+(rand() % (oldWinnerWeapon.Level));
			if(loser.Weapon.Index)
				printf("%s recovers a used %s from the battlefield.\n", loser.Name.c_str(), getWeaponName(loser.Weapon).c_str());
		}
		else
			printf("%s doesn't get to recover %s from %s.\n", winner.Name.c_str(), loserWeaponName.c_str(), loser.Name.c_str());
	}

	SArmor oldWinnerArmor		= winner.Armor;
	std::string loserArmorName = getArmorName(loser.Armor);
	if(loser.Armor.Index > winner.Armor.Index)
	{
		if(rand()%2)
		{
			printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(), loserArmorName.c_str(), loser.Armor.Level, loser.Name.c_str());
			winner.Armor			= loser.Armor;
			loser.Armor.Index		= rand() % (oldWinnerArmor.Index+1);
			loser.Armor.Modifier	= rand() % (oldWinnerArmor.Modifier+1);
			loser.Armor.Level		= 1+(rand() % (oldWinnerArmor.Level));
			if(loser.Armor.Index)
				printf("%s recovers a used %s from the battlefield.\n", loser.Name.c_str(), getArmorName(loser.Armor).c_str());
		}
		else
		{
			printf("%s dropped by %s is too damaged to be recovered.\n", loserArmorName.c_str(), loser.Name.c_str());
			loser.Armor.Index		= 0;
			loser.Armor.Modifier	= 0;
			loser.Armor.Level		= 1;
		}
	}
	else if(loser.Armor.Index == winner.Armor.Index && loser.Armor.Modifier > winner.Armor.Modifier)
	{
		if(rand()%2)
		{
			printf("%s recovers %s level %u from %s.\n", winner.Name.c_str(), loserArmorName.c_str(), loser.Armor.Level, loser.Name.c_str());
			winner.Armor.Modifier	= loser.Armor.Modifier;
			winner.Armor.Level		= loser.Armor.Level;
			loser.Armor.Index		= rand() % (oldWinnerArmor.Index+1);
			loser.Armor.Modifier	= rand() % (oldWinnerArmor.Modifier+1);
			loser.Armor.Level		= 1+(rand() % (oldWinnerArmor.Level));
			if(loser.Armor.Index)
				printf("%s recovers a used %s from the battlefield.\n", loser.Name.c_str(), getArmorName(loser.Armor).c_str());
		}
		else
			printf("%s doesn't get to recover %s from %s.\n", winner.Name.c_str(), loserArmorName.c_str(), loser.Name.c_str());
	}

	//winner.Points.MaxLife.HP += winner.Points.Attack;
	winner.Profession.Level++;
	winner.Armor.Level++;
	winner.Weapon.Level++;

	winner.Score.BattlesWon++;
	winner.Score.EnemiesKilled++;
	winner.Score.MoneyEarned += drop;

	loser.Score.BattlesLost++;

}

void determineOutcome(CCharacter& adventurer, CCharacter& enemy, uint32_t enemyType)
{
		// Determine the outcome of the battle and give rewards if applicable.
	if (adventurer.Points.CurrentLife.HP <= 0) 
		assignDrops(enemy, adventurer);
	if (enemy.Points.CurrentLife.HP <= 0)
		assignDrops(adventurer, enemy);
}

enum TURN_ACTION
{	TURN_ACTION_ATTACK
,	TURN_ACTION_INVENTORY
,	TURN_ACTION_SKILL
,	TURN_ACTION_RUN
};

enum TURN_OUTCOME
{	TURN_OUTCOME_CONTINUE
,	TURN_OUTCOME_ESCAPE
,	TURN_OUTCOME_CANCEL
};

bool useSkills(CCharacter& attacker, CCharacter& target)
{
	printf("\n");
	printf("Skills are not implemented yet.\n");
	return false;
}

TURN_OUTCOME characterTurn(TURN_ACTION combatOption, CCharacter& attacker, CCharacter& target)
{
	// If the action is valid then we execute it and break the current while() so the attack turn executes.
	TURN_OUTCOME outcome = TURN_OUTCOME_CANCEL;
	if(TURN_ACTION_ATTACK == combatOption)
		klib::attack(attacker, target);
	else if(TURN_ACTION_INVENTORY == combatOption) { 
		if( !useItems(attacker, target) )
			outcome = TURN_OUTCOME_CONTINUE;
	}	// useItems requires to receive the current enemy as a parameter in order to modify its health if we use a grenade and hit.
	else if(TURN_ACTION_SKILL == combatOption) { 
		if( !useSkills(attacker, target) )
			outcome = TURN_OUTCOME_CONTINUE;
	}	// useItems requires to receive the current enemy as a parameter in order to modify its health if we use a grenade and hit.
	else if(TURN_ACTION_RUN == combatOption) {
		if( escape(attacker.Name, attacker.Score) )
			outcome = TURN_OUTCOME_ESCAPE; // Escape: if we succeed we just exit this combat() function, otherwise cancel this loop and execute the enemy turn.
	}

	if(outcome == TURN_OUTCOME_CANCEL && target.Points.CurrentLife.HP > 0 && attacker.Points.CurrentLife.HP > 0)
		klib::applyTurnStatusAndBonusesAndSkipTurn(attacker);

	return outcome;
}

void printStatuses(const CCharacter& character)
{
	for(uint32_t i=0; i<character.CombatStatus.Count; ++i)
		switch(character.CombatStatus.Status[i])
		{
		case STATUS_TYPE_BLIND:		printf("%s is blind for the next %u turn(s).\n"		, character.Name.c_str(), character.CombatStatus.TurnsLeft[i]);	break;
		case STATUS_TYPE_BLEEDING:	printf("%s is bleeding for the next %u turn(s).\n"	, character.Name.c_str(), character.CombatStatus.TurnsLeft[i]);	break;
		case STATUS_TYPE_STUN:		printf("%s is stunned for the next %u turn(s).\n"	, character.Name.c_str(), character.CombatStatus.TurnsLeft[i]);	break;
		case STATUS_TYPE_BURN:		printf("%s is burning for the next %u turn(s).\n"	, character.Name.c_str(), character.CombatStatus.TurnsLeft[i]);	break;
		case STATUS_TYPE_POISON:	printf("%s is poisoned for the next %u turn(s).\n"	, character.Name.c_str(), character.CombatStatus.TurnsLeft[i]);	break;
		}
}

TURN_OUTCOME playerTurn(CCharacter& adventurer, CCharacter& currentEnemy)
{
	static const SMenuItem<TURN_ACTION> combatOptions[] =
	{ { TURN_ACTION_ATTACK		, "Attack"		}
	, { TURN_ACTION_INVENTORY	, "Inventory"	}
	, { TURN_ACTION_SKILL		, "Skills"		}
	, { TURN_ACTION_RUN			, "Run"			}
	};

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;

	SCharacterPoints playerPoints = calculateFinalPoints(adventurer);
	SCharacterPoints enemyPoints = calculateFinalPoints(currentEnemy);
	while (turnOutcome == TURN_OUTCOME_CONTINUE)	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
	{
		printf("\n----------------------- %s is a %s level %u.\nWeapon: %s level %u.\nArmor: %s level %u.\nHit Chance Bonus Turns: %u.\nAttack Bonus Turns: %u.\n",  
			adventurer		.Name.c_str(), getProfessionName(adventurer.Profession).c_str(),	adventurer		.Profession.Level,	
			getWeaponName(adventurer.Weapon).c_str(),	adventurer	.Weapon.Level,	
			getArmorName (adventurer.Armor ).c_str(),	adventurer	.Armor.Level, 
			adventurer	.CombatBonus.TurnsLeft.Attack.Hit,	
			adventurer	.CombatBonus.TurnsLeft.Attack.Damage	);
		
		printf("-------------- Max points:\n");
		playerPoints.MaxLife.Print();
		printf("-------------- Current points:\n");
		adventurer.Points.CurrentLife.Print();
		playerPoints.Attack.Print();
		printStatuses(adventurer);
		
		printf("\n----------------------- %s is a %s level %u.\nWeapon: %s level %u.\nArmor: %s level %u.\nHit Chance Bonus Turns: %u.\nAttack Bonus Turns: %u.\n",  
			currentEnemy	.Name.c_str(), getProfessionName(currentEnemy.Profession).c_str(),	currentEnemy	.Profession.Level,	
			getWeaponName(currentEnemy.Weapon).c_str(),	currentEnemy	.Weapon.Level,	
			getArmorName (currentEnemy.Armor ).c_str(),	currentEnemy	.Armor.Level, 
			currentEnemy	.CombatBonus.TurnsLeft.Attack.Hit,	
			currentEnemy	.CombatBonus.TurnsLeft.Attack.Damage	);
		
		printf("-------------- Max points:\n");
		enemyPoints.MaxLife.Print();
		printf("-------------- Current points:\n");
		currentEnemy.Points.CurrentLife.Print();
		enemyPoints.Attack.Print();
		printStatuses(currentEnemy);

		const TURN_ACTION actionChoice = displayMenu("It's your turn to make a move", combatOptions);
		turnOutcome = characterTurn(actionChoice, adventurer, currentEnemy);
	}
	return turnOutcome;
}

TURN_ACTION resolveAI(CCharacter& enemy, CCharacter& adventurer)
{
	TURN_ACTION action = TURN_ACTION_ATTACK;
	if(enemy.Inventory.ItemCount)
		action = (rand()%2) ? action : TURN_ACTION_INVENTORY;
	else if(enemy.Points.CurrentLife.HP <= (enemy.Points.MaxLife.HP/9) && 0 == (rand()%7))	// chance of escape attempt if health is less than 11%.
		action = TURN_ACTION_RUN;

	return action;
}

TURN_OUTCOME enemyTurn(CCharacter& enemy, CCharacter& adventurer)
{
	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while (turnOutcome == TURN_OUTCOME_CONTINUE) {	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
		const TURN_ACTION actionChoice = (TURN_ACTION)resolveAI(enemy, adventurer);
		turnOutcome = characterTurn(actionChoice, enemy, adventurer);
	}
	return turnOutcome;
}

bool combatContinues(TURN_OUTCOME turnOutcome, int adventurerHP, int enemyHP)
{
	if( TURN_OUTCOME_ESCAPE == turnOutcome || 0 >= adventurerHP || 0 >= enemyHP )
		return false;
	return true;
}

//5736	// gasty.bellino@gmail.com
void combat(CCharacter& adventurer, uint32_t enemyType)
{
	if(adventurer.Points.CurrentLife.HP <= 1)
	{
		printf("You don't have enough health to engage in combat. Please go rest and come back later.\n");
		return;
	}

	CCharacter currentEnemy = klib::enemyDefinitions[enemyType];	// Request the enemy data.
	klib::setupEnemy(adventurer, currentEnemy, enemyType);

	adventurer.CombatStatus.Count	= 0;	// We need to clear the combat status before starting the combat.

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while(combatContinues(turnOutcome, adventurer.Points.CurrentLife.HP, currentEnemy.Points.CurrentLife.HP))	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		adventurer.Score.TurnsPlayed++;
		currentEnemy.Score.TurnsPlayed++;

		if(adventurer.CombatStatus.GetStatusTurns(STATUS_TYPE_STUN))
		{
			printf("%s is stunned and loses his turn!\n", adventurer.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
			klib::applyTurnStatusAndBonusesAndSkipTurn(adventurer);
		}
		else
			turnOutcome = playerTurn(adventurer, currentEnemy);

		if(!combatContinues(turnOutcome, adventurer.Points.CurrentLife.HP, currentEnemy.Points.CurrentLife.HP))
			break;

		// Execute enemy attack turn
		if(currentEnemy.CombatStatus.GetStatusTurns(STATUS_TYPE_STUN))
		{
			printf("%s is stunned and loses his turn!\n", currentEnemy.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
			klib::applyTurnStatusAndBonusesAndSkipTurn(currentEnemy);
		}
		else
			turnOutcome = enemyTurn(currentEnemy, adventurer);
	}

	determineOutcome(adventurer, currentEnemy, enemyType);
}


// This function returns true if an item was used or false if the menu was exited without doing anything.
bool useItems(CCharacter& user, CCharacter& target)
{
	bool bUsedItem = false;
	uint32_t indexInventory = ~0U;
	static const size_t inventorySize = size(user.Inventory.Slots);
	if(0 == user.Inventory.ItemCount)
	{
		printf("%s has no items in the inventory.\n", user.Name.c_str());
		return false;
	}

	SMenuItem<uint32_t> itemOptions[MAX_INVENTORY_SLOTS+1];
	char itemOption[128] = {};
	for(uint32_t i=0; i<user.Inventory.ItemCount; ++i)
	{
		std::string itemName = getItemName(user.Inventory.Slots[i].Item);
		sprintf_s(itemOption, "- x%.2u %s", user.Inventory.Slots[i].ItemCount, itemName.c_str());
		itemOptions[i].ReturnValue	= i;
 		itemOptions[i].Text			= itemOption;
	}
	itemOptions[user.Inventory.ItemCount].ReturnValue	= user.Inventory.ItemCount;
	itemOptions[user.Inventory.ItemCount].Text			= "Back to combat options";

	if(user.Type == CHARACTER_TYPE_PLAYER) 
	{
		indexInventory = displayMenu(user.Inventory.ItemCount+1, "Select an item to use", itemOptions);

		if(indexInventory == user.Inventory.ItemCount) // exit option
			bUsedItem = false;
		else if (user.Inventory.Slots[indexInventory].ItemCount <= 0)
			printf("You don't have anymore of that. Use something else...\n"); 
		else {
			// if we reached here it means that the input was valid so we select the description and exit the loop
			bUsedItem = true;
		}
	}
	else // not a player so execute choice by AI
	{
		indexInventory = (uint32_t)(rand() % user.Inventory.ItemCount);	// this should be improved.
		const CItem& itemDescription = itemDefinitions[user.Inventory.Slots[indexInventory].Item.Index];
		
		// Only use potions if we have less than 80% HP
		if	 ( ITEM_TYPE_POTION		!= itemDescription.Type
			|| PROPERTY_TYPE_HEALTH != itemDescription.Property
			|| user.Points.CurrentLife.HP < ((user.Points.MaxLife.HP+user.CombatBonus.Points.MaxLife.HP)*.7f)
			)
			bUsedItem = true;
	}

	if(bUsedItem)
	{
		const SCharacterPoints finalPoints = calculateFinalPoints(user);
		const CItem& itemDescription = itemDefinitions[user.Inventory.Slots[indexInventory].Item.Index];
		if( ITEM_TYPE_POTION == itemDescription.Type 
		 && PROPERTY_TYPE_HEALTH == itemDescription.Property 
		 && user.Points.CurrentLife.HP == finalPoints.MaxLife.HP)
		{
			bUsedItem = false;
			printf("Your HP is full!");
		}
		else
			klib::executeItem(indexInventory, user, target);
	}
	
	return bUsedItem;
}