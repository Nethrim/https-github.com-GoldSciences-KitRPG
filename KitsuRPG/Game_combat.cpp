#include "Game.h"
#include "Menu.h"

bool useItems(CCharacter& adventurer, CCharacter& target);	// While in combat, displays a list of the available items to use.

// This function returns the damage dealt to the target
int attack(const CCharacter& attacker, CCharacter& target)
{
	// Calculate success from the hit chance and apply damage to target or just print the miss message.
	int32_t damageDealt = 0;

	if ((rand() % 100) < (attacker.Points.Hit+attacker.CombatBonus.Points.Hit) )
	{
		damageDealt = attacker.Points.Attack+attacker.CombatBonus.Points.Attack+(rand()%(attacker.Points.Attack/10+1));
		target.Points.HP -= damageDealt;
		printf("%s hits %s for: %u.\n", attacker.Name.c_str(), target.Name.c_str(), damageDealt);
	}
	else 
		printf("%s misses the attack!\n", attacker.Name.c_str());

	return damageDealt;
};

void attackTurn(CCharacter& attacker, CCharacter& target)
{
	if(attacker.Type == CHARACTER_TYPE_PLAYER)
		printf("You decide to attack!\n"); 

	const int damageDealt = attack(attacker, target);

	if(damageDealt) {
		attacker	.Score.DamageDealt += damageDealt;
		attacker	.Score.AttacksHit++;
		target		.Score.DamageTaken += damageDealt;
		target		.Score.AttacksReceived++;
	}
	else  {
		attacker	.Score.AttacksMissed++;
		target		.Score.AttacksAvoided++;
	}

}

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

void determineOutcome(CCharacter& adventurer, CCharacter& enemy, ENEMY_TYPE enemyType)
{
		// Determine the outcome of the battle and give rewards if applicable.
	if (adventurer.Points.HP <= 0) 
		printf("You are dead!\n");
	else if (enemy.Points.HP <= 0)
	{
		printf("The %s is dead!\n", enemy.Name.c_str());
		int drop = enemy.Points.Coins + (rand() % 20);
		printf("\nThe enemy dropped %u coins!!\n\n", drop);
		adventurer.Points.Coins = adventurer.Points.Coins + drop;
		for(uint32_t i=0; i<enemy.ItemCount; i++) 
			if(rand()%2) 
			{
				const SInventorySlot& itemDrop = enemy.Inventory[i];
				if(addItem(adventurer, itemDrop.Description))
					printf("\nThe enemy dropped %s!!\n\n", itemDrop.Description.Name.c_str());
				else
					printf("You can't pick up %s by %s because the inventory is full!", itemDrop.Description.Name.c_str(), enemy.Name.c_str());
			}

		adventurer.Points.MaxHP += enemyType;

		adventurer.Score.BattlesWon++;
		adventurer.Score.EnemiesKilled++;
		adventurer.Score.MoneyEarned += drop;
	}
}

enum TURN_ACTION
{	TURN_ACTION_ATTACK
,	TURN_ACTION_INVENTORY
,	TURN_ACTION_RUN
};

enum TURN_OUTCOME
{	TURN_OUTCOME_CONTINUE
,	TURN_OUTCOME_ESCAPE
,	TURN_OUTCOME_CANCEL
};

TURN_OUTCOME characterTurn(TURN_ACTION combatOption, CCharacter& attacker, CCharacter& target)
{
	// If the action is valid then we execute it and break the current while() so the attack turn executes.
	TURN_OUTCOME outcome = TURN_OUTCOME_CANCEL;
	if(TURN_ACTION_ATTACK == combatOption)
		attackTurn(attacker, target);
	else if(TURN_ACTION_INVENTORY == combatOption) { 
		if( !useItems(attacker, target) )
			outcome = TURN_OUTCOME_CONTINUE;
	}	// useItems requires to receive the current enemy as a parameter in order to modify its health if we use a grenade and hit.
	else if(TURN_ACTION_RUN == combatOption) {
		if( escape(attacker.Name, attacker.Score) )
			outcome = TURN_OUTCOME_ESCAPE; // Escape: if we succeed we just exit this combat() function, otherwise cancel this loop and execute the enemy turn.
	}

	if(outcome != TURN_OUTCOME_CONTINUE) 
	{
		if(outcome == TURN_OUTCOME_ESCAPE)
			attacker.CombatBonus = {};
		else
			attacker.CombatBonus.NextTurn();
	}

	return outcome;
}

TURN_OUTCOME playerTurn(CCharacter& adventurer, CCharacter& currentEnemy)
{
	static const SMenuItem combatOptions[] =
	{ { TURN_ACTION_ATTACK		, "Attack"		}
	, { TURN_ACTION_INVENTORY	, "Inventory"	}
	, { TURN_ACTION_RUN			, "Run"			}
	};

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while (turnOutcome == TURN_OUTCOME_CONTINUE)	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
	{
		printf("\n-- Your HP is: %u.\n", adventurer.Points.HP);
		printf("-- %s HP is: %u.\n", currentEnemy.Name.c_str(), currentEnemy.Points.HP);

		const TURN_ACTION actionChoice = (TURN_ACTION)displayMenu("It's your turn to make a move", combatOptions);
		turnOutcome = characterTurn(actionChoice, adventurer, currentEnemy);
	}
	return turnOutcome;
}

TURN_ACTION resolveAI(CCharacter& enemy, CCharacter& adventurer)
{
	TURN_ACTION action = TURN_ACTION_ATTACK;
	if(enemy.ItemCount)
		action = (rand()%2) ? action : TURN_ACTION_INVENTORY;
	else if(enemy.Points.HP <= (enemy.Points.MaxHP/9) && 0 == (rand()%9))	// 11 % chance of escape attempt.
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
void combat(CCharacter& adventurer, ENEMY_TYPE enemyType)
{
	CCharacter currentEnemy = getEnemyDefinition(enemyType);	// Request the enemy data.

	addItem( currentEnemy, itemDescriptions[0] );
	for(size_t i=(size_t)ENEMY_TYPE_WOLF; i<enemyType; ++i)
		addItem( currentEnemy, itemDescriptions[rand()%getDescriptionCount(itemDescriptions)] );

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while(combatContinues(turnOutcome, adventurer.Points.HP, currentEnemy.Points.HP))	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		adventurer.Score.TurnsPlayed++;
		currentEnemy.Score.TurnsPlayed++;

		turnOutcome = playerTurn(adventurer, currentEnemy);
		if(!combatContinues(turnOutcome, adventurer.Points.HP, currentEnemy.Points.HP))
			break;

		// Execute enemy attack turn
		turnOutcome = enemyTurn(currentEnemy, adventurer);
	} 

	determineOutcome(adventurer, currentEnemy, enemyType);
}

void usePotion(const SItem& itemDescription, CCharacter& potionDrinker) 
{
	int itemEffectValue;
	int totalMaxHP=potionDrinker.Points.MaxHP+potionDrinker.CombatBonus.Points.MaxHP;
	const int itemGrade = std::max(1, itemDescription.Grade);
	
	const std::string& drinkerName	= potionDrinker.Name;
	SCharacterPoints& drinkerPoints	= potionDrinker.Points;
	SCombatBonus& drinkerBonus		= potionDrinker.CombatBonus;
	

	switch(itemDescription.Property)
	{
	case PROPERTY_TYPE_HEALTH:
		printf("%s starts feeling better...\n", drinkerName.c_str());
		itemEffectValue = (totalMaxHP/5+1)+(rand()%(totalMaxHP/10));
		itemEffectValue *= itemDescription.Grade;
		drinkerPoints.HP += itemEffectValue;
		drinkerPoints.HP = std::min(drinkerPoints.HP, totalMaxHP);
		printf("The potion heals %s for %u! %s now has %u HP.\n", drinkerName.c_str(), itemEffectValue, drinkerName.c_str(), drinkerPoints.HP);
		break;
	case PROPERTY_TYPE_STRENGTH:
		printf("%s starts feeling stronger...\n", drinkerName.c_str());
		itemEffectValue = 3*itemGrade;
		itemEffectValue += 3+(rand()%itemGrade);
		drinkerBonus.Points.Attack		+= itemEffectValue;
		drinkerBonus.TurnsLeft.Attack	+= 1+itemGrade;
		printf("The potion gives %s %u Attack points for %u turns. %s now has %u Attack points for the next %u turns.\n", drinkerName.c_str(), itemEffectValue, itemGrade, drinkerName.c_str(), drinkerPoints.Attack+drinkerBonus.Points.Attack, drinkerBonus.TurnsLeft.Attack-1);
		break;
	case PROPERTY_TYPE_HIT:
		printf("%s starts feeling faster...\n", drinkerName.c_str());
		itemEffectValue = 5*itemGrade;
		itemEffectValue += ((rand()%itemGrade)+1)*5;
		drinkerBonus.Points.Hit		+= itemEffectValue;
		drinkerBonus.TurnsLeft.Hit	+= 1+itemGrade;
		printf("The potion gives %s %u Hit chance points for %u turns. %s now has %u Hit chance points for the next %u turns.\n", drinkerName.c_str(), itemEffectValue, itemGrade, drinkerName.c_str(), drinkerPoints.Hit+drinkerBonus.Points.Hit, drinkerBonus.TurnsLeft.Hit-1);
		break;
	default:
		printf("Potion type not implemented!");
	};

	potionDrinker.Score.PotionsUsed++;
}

enum ATTACK_TARGET
{	ATTACK_TARGET_MISS
,	ATTACK_TARGET_SELF
,	ATTACK_TARGET_OTHER
};

void useGrenade(const SItem& itemDescription, CCharacter& thrower, CCharacter& target) 
{
	// Currently the hit chance for all the grenade types are calculated with the same formula.
	int	lotteryRange	= 60+(10*itemDescription.Grade);	// calculate hit chance from item grade
	int	lotteryResult	= rand()%100;

	int itemEffectValue;
	ATTACK_TARGET hitTarget = ATTACK_TARGET_MISS;
	switch(itemDescription.Property)
	{
	case PROPERTY_TYPE_BLAST:
		printf("%s throws a grenade to %s.\n", thrower.Name.c_str(), target.Name.c_str());

		itemEffectValue	= int(target.Points.MaxHP*(0.2f*itemDescription.Grade)); // this is the total damage points applied to the enemy or player

		if(lotteryResult < lotteryRange)
		{
			target.Points.HP -= itemEffectValue;	// this is the resulting hp after applying the damage 
			hitTarget = ATTACK_TARGET_OTHER;
			printf("The grenade hits the target doing %u damage.\n", itemEffectValue);
		}
		else if( lotteryResult == lotteryRange )
		{
			thrower.Points.HP -= itemEffectValue/2;	// this is the resulting hp after applying the damage 
			hitTarget = ATTACK_TARGET_SELF;
			printf("%s throws the grenade too close...\n"		
				"The grenade explodes near %s doing %u damage!\n", thrower.Name.c_str(), thrower.Name.c_str(), itemEffectValue);
		}
		else
			printf("%s throws the grenade too far away.\n", thrower.Name.c_str());

		break;
	case PROPERTY_TYPE_PIERCING:
	default:
		printf("Grenade type not implemented!");
	};

	if(hitTarget & ATTACK_TARGET_OTHER) {
		thrower	.Score.DamageDealt += itemEffectValue; 
		target	.Score.DamageTaken += itemEffectValue; 
	}
	
	if(hitTarget & ATTACK_TARGET_SELF)  {
		thrower	.Score.DamageDealt += itemEffectValue; 
		thrower	.Score.DamageTaken += itemEffectValue; 
	}

	thrower.Score.GrenadesUsed++;
}

void executeItem(uint32_t indexItem, CCharacter& user, CCharacter& target) {

	SItem& itemDescription = user.Inventory[indexItem].Description;
	std::string itemName = itemDescription.Name;
	printf("\n%s uses: %s.\n\n", user.Name.c_str(), itemName.c_str());
	switch( itemDescription.Type )
	{
	case ITEM_TYPE_POTION:
		usePotion(itemDescription, user);
		break;

	case ITEM_TYPE_GRENADE:
		useGrenade(itemDescription, user, target);
		break;

	default:
		printf("This item type does nothing yet... But we still remove it from your inventory!\n");
	}

	user.Inventory[indexItem].Count--;
	if( user.Inventory[indexItem].Count )
		printf("\n%s has %u %s left.\n", user.Name.c_str(), user.Inventory[indexItem].Count, itemName.c_str());
	else 
	{
		user.Inventory[indexItem] = user.Inventory[--user.ItemCount];
		printf("\n%s ran out of %s.\n", user.Name.c_str(), itemName.c_str());
	}
}

// This function returns true if an item was used or false if the menu was exited without doing anything.
bool useItems(CCharacter& user, CCharacter& target)
{
	bool bUsedItem = false;
	uint32_t indexItem = ~0U;
	SItem itemDescription;
	static const size_t inventorySize = size(user.Inventory)+1;
	if(0 == user.ItemCount)
	{
		printf("%s has no items in the inventory.", user.Name.c_str());
		return false;
	}

	if(user.Type == CHARACTER_TYPE_PLAYER) 
	{
		while(true)
		{
			printf("- Type %u to close your inventory.\n", (uint32_t)(inventorySize));
			showInventory(user);

			indexItem = (uint32_t)(getNumericInput()-1);

			if(indexItem == (inventorySize-1)) // exit option
				break;
			else if(indexItem >= user.ItemCount)	// invalid index means it's an invalid option
				printf("Invalid answer. Answer again...\n");
			else if (user.Inventory[indexItem].Count <= 0)
				printf("You don't have anymore of that. Use something else...\n"); 
			else {
				// if we reached here it means that the input was valid so we select the description and exit the loop
				bUsedItem = true;
				break;
			}
		}
	}
	else // not a player so execute choice by AI
	{
		indexItem = (uint32_t)(rand() % user.ItemCount);	// this should be improved.
		
		// Only use potions if we have less than 80% HP
		if	 ( ITEM_TYPE_POTION != user.Inventory[indexItem].Description.Type
			|| PROPERTY_TYPE_HEALTH != user.Inventory[indexItem].Description.Property
			|| user.Points.HP < ((user.Points.MaxHP+user.CombatBonus.Points.MaxHP)*.8f)
			)
			bUsedItem = true;
	}

	if(bUsedItem)
		executeItem(indexItem, user, target);
	
	return bUsedItem;
}