#include "Game.h"

#include "Weapon.h"
#include "Item.h"
#include "Enemy.h"
#include "Armor.h"

#include "Menu.h"

bool useItems(CCharacter& adventurer, CCharacter& target);	// While in combat, displays a list of the available items to use.

// This function returns the damage dealt to the target
int attack(CCharacter& attacker, CCharacter& target)
{
	// Calculate success from the hit chance and apply damage to target or just print the miss message.
	int32_t damageDealt = 0;
	const CWeapon& weaponDefinition = weaponDefinitions[attacker.Weapon];

	SCharacterPoints attackerPoints = calculateFinalPoints(attacker);

	bool bIsBlind = attacker.CombatStatus.GetStatusTurns(STATUS_TYPE_BLIND) > 0;
	if(bIsBlind)
		printf("Blindness causes %s to have %u hit chance for this turn.\n", attacker.Name.c_str(), attackerPoints.Hit/2);

	int finalChance = (bIsBlind ? attackerPoints.Hit/2 : attackerPoints.Hit);
	if(target.CombatStatus.GetStatusTurns(STATUS_TYPE_STUN))
	{
		printf("As %s is stunned, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Hit);
		finalChance	+= attackerPoints.Hit;
	}
	if(target.CombatStatus.GetStatusTurns(STATUS_TYPE_BLIND))
	{
		printf("As %s is blind, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Hit/2);
		finalChance	+= attackerPoints.Hit/2;
	}

	if ((rand() % 100) < finalChance )
	{
		damageDealt = attackerPoints.Attack+(rand()%((attackerPoints.Attack)/10+1));
		printf("%s hits %s for: %u.\n", attacker.Name.c_str(), target.Name.c_str(), damageDealt);

		int remainingShield = target.Shield-damageDealt;
		if(target.Shield)
		{
			printf("%s's shield absorbs %u damage.\n", target.Name.c_str(), std::min(target.Shield, damageDealt));
			target.Shield = std::max(0, remainingShield);
			printf("%s's remaining shield is now %u.\n", target.Name.c_str(), target.Shield);
		}
		if(remainingShield < 0)	// only apply damage to health if the shield didn't absorb all the damage.
			target.Points.HP += remainingShield;

		attacker.Points.Coins	+= weaponDefinition.Points.Coins;
		attacker.Points.HP		+= weaponDefinition.Points.HP;
		attacker.Points.HP		= std::min(attacker.Points.HP, attackerPoints.MaxHP);
		if(weaponDefinition.Effect == WEAPON_EFFECT_LEECH)
		{
			int actualHPGained = std::min(damageDealt, attacker.Points.MaxHP-attacker.Points.HP);
			if(actualHPGained)
			{
				printf("%s gains %u HP from %s.\n", attacker.Name.c_str(), actualHPGained, weaponDefinition.Name.c_str());
				attacker.Points.HP		+= actualHPGained;
			}
		}
		if(weaponDefinition.Status)
		{
			if(rand()%2)
			{
				std::string text;
				int turns=1;
				switch(weaponDefinition.Status)
				{
				default: text = "An unknown effect was applied to %s for %u turns.\n";break;
				case STATUS_TYPE_STUN:		turns = 2; text = "%s will be stunned for the next %u turns.\n"	;break;
				case STATUS_TYPE_BLIND:		turns = 2; text = "%s will be blind for the next %u turns.\n"	;break;
				case STATUS_TYPE_BLEEDING:	turns = 2; text = "%s will bleed for the next %u turns.\n"		;break;
				}
				printf(text.c_str(), target.Name.c_str(), turns-1);
				addStatus(target.CombatStatus, weaponDefinition.Status, 1);
			}
		}

		if(weaponDefinition.Points.Coins)
			printf("%s gives %s %u Coins.\n", weaponDefinition.Name.c_str(), attacker.Name.c_str(), weaponDefinition.Points.Coins);
		if(weaponDefinition.Points.HP)
			printf("%s gives %s %u Health Points.\n", weaponDefinition.Name.c_str(), attacker.Name.c_str(), weaponDefinition.Points.HP);
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

void determineOutcome(CCharacter& adventurer, CCharacter& enemy, uint32_t enemyType)
{
		// Determine the outcome of the battle and give rewards if applicable.
	if (adventurer.Points.HP <= 0) 
		printf("You are dead!\n");
	else if (enemy.Points.HP <= 0)
	{
		printf("The %s is dead!\n", enemy.Name.c_str());
		int drop = enemy.Points.Coins + (rand() % 20);
		printf("\nThe enemy dropped %u coins!!\n", drop);
		adventurer.Points.Coins = adventurer.Points.Coins + drop;
		for(uint32_t i=0; i<enemy.Inventory.ItemCount; i++) 
			if(rand()%2) 
			{
				const SInventorySlot& itemDrop = enemy.Inventory.Slots[i];
				if(addItem(adventurer.Inventory, itemDrop.ItemIndex))
					printf("\nThe enemy dropped %s!!\n", itemDescriptions[itemDrop.ItemIndex].Name.c_str());
				else
					printf("You can't pick up %s by %s because the inventory is full!\n", itemDescriptions[itemDrop.ItemIndex].Name.c_str(), enemy.Name.c_str());
			}

		if(enemy.Weapon > adventurer.Weapon)
		{
			if(rand()%2)
			{
				printf("You recover %s from %s.\n", weaponDefinitions[enemy.Weapon].Name.c_str(), enemy.Name.c_str());
				adventurer.Weapon = enemy.Weapon;
			}
			else
				printf("%s dropped by %s is too damaged to be recovered.\n", weaponDefinitions[enemy.Weapon].Name.c_str(), enemy.Name.c_str());
		}

		if(enemy.Armor > adventurer.Armor)
		{
			if(rand()%2)
			{
				printf("You recover %s from %s.\n", armorDefinitions[enemy.Armor].Name.c_str(), enemy.Name.c_str());
				adventurer.Armor = enemy.Armor;
			}
			else
				printf("%s dropped by %s is too damaged to be recovered.\n", armorDefinitions[enemy.Armor].Name.c_str(), enemy.Name.c_str());
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

void applyTurnStatus(CCharacter& character)
{
	int amount=0;
	const SCharacterPoints finalPoints = calculateFinalPoints(character);
	for(uint32_t i=0; i<character.CombatStatus.Count; ++i)
	{
		switch(character.CombatStatus.Status[i])
		{
		case STATUS_TYPE_BLEEDING:	amount = std::max(1, finalPoints.MaxHP/20); character.Points.HP -= amount; printf("%s is bleeding and loses %u HP.\n", character.Name.c_str(), amount); break;
		//case STATUS_TYPE_STUN:		break;
		//case STATUS_TYPE_BLIND:		break;
		}
	}
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
		}
}

TURN_OUTCOME playerTurn(CCharacter& adventurer, CCharacter& currentEnemy)
{
	static const SMenuItem combatOptions[] =
	{ { TURN_ACTION_ATTACK		, "Attack"		}
	, { TURN_ACTION_INVENTORY	, "Inventory"	}
	, { TURN_ACTION_RUN			, "Run"			}
	};

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;

	SCharacterPoints playerPoints = calculateFinalPoints(adventurer);
	SCharacterPoints enemyPoints = calculateFinalPoints(currentEnemy);
	while (turnOutcome == TURN_OUTCOME_CONTINUE)	// this while() process the input for this turn until the user enters a valid choice and then exits to the outer loop for executing the attack turn.
	{
		printf("\n-- %s HP is: %u. Shield: %u.\nHit Chance: %u.\nAttack: %u.\nHit Chance Bonus Turns: %u.\nAttack Bonus Turns: %u.\nWeapon: %s.\nArmor: %s.\n",  adventurer	.Name.c_str(), adventurer	.Points.HP,	adventurer	.Shield,	 playerPoints	.Hit, playerPoints	.Attack,	adventurer		.CombatBonus.TurnsLeft.Hit,adventurer	.CombatBonus.TurnsLeft.Attack,	weaponDefinitions[adventurer	.Weapon].Name.c_str(), armorDefinitions[adventurer		.Armor].Name.c_str());
		printStatuses(adventurer);
		printf("\n-- %s HP is: %u. Shield: %u.\nHit Chance: %u.\nAttack: %u.\nHit Chance Bonus Turns: %u.\nAttack Bonus Turns: %u.\nWeapon: %s.\nArmor: %s.\n",  currentEnemy	.Name.c_str(), currentEnemy	.Points.HP,	currentEnemy.Shield,	 enemyPoints	.Hit, enemyPoints	.Attack,	currentEnemy	.CombatBonus.TurnsLeft.Hit,currentEnemy	.CombatBonus.TurnsLeft.Attack,	weaponDefinitions[currentEnemy	.Weapon].Name.c_str(), armorDefinitions[currentEnemy	.Armor].Name.c_str());
		printStatuses(currentEnemy);

		const TURN_ACTION actionChoice = (TURN_ACTION)displayMenu("It's your turn to make a move", combatOptions);
		turnOutcome = characterTurn(actionChoice, adventurer, currentEnemy);
	}
	return turnOutcome;
}

TURN_ACTION resolveAI(CCharacter& enemy, CCharacter& adventurer)
{
	TURN_ACTION action = TURN_ACTION_ATTACK;
	if(enemy.Inventory.ItemCount)
		action = (rand()%2) ? action : TURN_ACTION_INVENTORY;
	else if(enemy.Points.HP <= (enemy.Points.MaxHP/9) && 0 == (rand()%7))	// chance of escape attempt if health is less than 11%.
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

void setupEnemy(CCharacter& currentEnemy, uint32_t enemyType)
{
	addItem( currentEnemy.Inventory, 1 );
	for(uint32_t i=1; i<enemyType; ++i)
		addItem( currentEnemy.Inventory, 1+(rand()%(size(itemDescriptions)-1)) );

	currentEnemy.Weapon	= (rand()%(enemyType*enemyType)) % (size(weaponDefinitions)-1);
	currentEnemy.Armor	= std::min(rand()%(enemyType*4), size(armorDefinitions)-1);
	currentEnemy.Shield = armorDefinitions[currentEnemy.Armor].Shield;

	SCharacterPoints finalEnemyPoints = calculateFinalPoints(currentEnemy);
	currentEnemy.Points.HP = finalEnemyPoints.MaxHP;
}

void applyCombatBonus(CCharacter& character, const SCharacterPoints& combatBonus, const std::string& sourceName)
{
	
	if(combatBonus.HP)
	{
		if(combatBonus.HP > 0 && (character.Points.HP >= calculateFinalPoints(character).MaxHP)) {}
		else 
		{
			if(combatBonus.HP > 0)
				printf("%s gains %u HP from %s.\n", character.Name.c_str(), combatBonus.HP, sourceName.c_str());
			else if( combatBonus.HP < 0)
				printf("%s loses %u HP from %s.\n", character.Name.c_str(), combatBonus.HP*-1, sourceName.c_str());
			character.Points.HP		+= combatBonus.HP;
		}
	}
	if(combatBonus.Coins)
	{
		if(combatBonus.Coins > 0)
			printf("%s gains %u Coins from %s.\n", character.Name.c_str(), combatBonus.Coins, sourceName.c_str());
		else if( combatBonus.Coins < 0)
			printf("%s loses %u Coins from %s.\n", character.Name.c_str(), combatBonus.Coins*-1, sourceName.c_str());
		character.Points.Coins	+= combatBonus.Coins;
	}
};


void applyArmorBonus(CCharacter& character)
{
	const CArmor& armorDefinition = armorDefinitions[character.Armor];
	applyCombatBonus(character, armorDefinition.Points, armorDefinition.Name);
};

//5736	// gasty.bellino@gmail.com
void combat(CCharacter& adventurer, uint32_t enemyType)
{
	CCharacter currentEnemy = enemyDefinitions[enemyType];	// Request the enemy data.
	setupEnemy(currentEnemy, enemyType);

	adventurer.CombatStatus.Count	= 0;	// We need to clear the combat status before starting the combat.
	adventurer.Shield				= armorDefinitions[adventurer.Armor].Shield;

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while(combatContinues(turnOutcome, adventurer.Points.HP, currentEnemy.Points.HP))	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		adventurer.Score.TurnsPlayed++;
		currentEnemy.Score.TurnsPlayed++;

		if(adventurer.CombatStatus.GetStatusTurns(STATUS_TYPE_STUN))
		{
			printf("%s is stunned and loses his turn!\n", adventurer.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
		}
		else
			turnOutcome = playerTurn(adventurer, currentEnemy);

		if(turnOutcome != TURN_OUTCOME_CONTINUE) {
			if(turnOutcome != TURN_OUTCOME_ESCAPE)
			{
				applyTurnStatus(adventurer);
				applyCombatBonus(adventurer, adventurer.CombatBonus.Points, "Turn Combat Bonus");
				applyArmorBonus(adventurer);
			}
			adventurer.CombatBonus.NextTurn();
			adventurer.CombatStatus.NextTurn();
		}

		if(!combatContinues(turnOutcome, adventurer.Points.HP, currentEnemy.Points.HP))
			break;

		// Execute enemy attack turn
		if(currentEnemy.CombatStatus.GetStatusTurns(STATUS_TYPE_STUN))
		{
			printf("%s is stunned and loses his turn!\n", currentEnemy.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
		}
		else
			turnOutcome = enemyTurn(currentEnemy, adventurer);

		if(turnOutcome != TURN_OUTCOME_CONTINUE) {
			if(turnOutcome != TURN_OUTCOME_ESCAPE)
			{
				applyTurnStatus(currentEnemy);
				applyCombatBonus(currentEnemy, currentEnemy.CombatBonus.Points, "Turn Combat Bonus");
				applyArmorBonus(currentEnemy);
			}
			currentEnemy.CombatBonus.NextTurn();
			currentEnemy.CombatStatus.NextTurn();
		}

	}

	determineOutcome(adventurer, currentEnemy, enemyType);
}

void usePotion(const CItem& itemDescription, CCharacter& potionDrinker) 
{
	int itemEffectValue;
	const int itemGrade = std::max(1, itemDescription.Grade);
	
	const std::string& drinkerName	= potionDrinker.Name;
	SCharacterPoints& drinkerPoints	= potionDrinker.Points;
	SCombatBonus& drinkerBonus		= potionDrinker.CombatBonus;
	SCharacterPoints finalPoints;

	switch(itemDescription.Property)
	{
	case PROPERTY_TYPE_HEALTH:
		printf("%s starts feeling better...\n", drinkerName.c_str());
		finalPoints = calculateFinalPoints(potionDrinker);
		itemEffectValue = (finalPoints.MaxHP/5+1)+(rand()%(finalPoints.MaxHP/10));
		itemEffectValue *= itemDescription.Grade;
		drinkerPoints.HP += itemEffectValue;
		drinkerPoints.HP = std::min(drinkerPoints.HP, finalPoints.MaxHP);
		printf("The potion heals %s for %u! %s now has %u HP.\n", drinkerName.c_str(), itemEffectValue, drinkerName.c_str(), drinkerPoints.HP);
		break;
	case PROPERTY_TYPE_STRENGTH:
		printf("%s starts feeling stronger...\n", drinkerName.c_str());
		itemEffectValue = 3*itemGrade;
		itemEffectValue += rand()%(itemGrade*2);
		drinkerBonus.Points.Attack		+= itemEffectValue;
		if(0 == drinkerBonus.TurnsLeft.Attack)
			drinkerBonus.TurnsLeft.Attack = 1;
		drinkerBonus.TurnsLeft.Attack	+= itemGrade;
		finalPoints = calculateFinalPoints(potionDrinker);
		printf("The potion gives %s %u Attack points for %u turns. %s now has %u Attack points for the next %u turns.\n", drinkerName.c_str(), itemEffectValue, itemGrade, drinkerName.c_str(), finalPoints.Attack, drinkerBonus.TurnsLeft.Attack-1);
		break;
	case PROPERTY_TYPE_HIT:
		printf("%s starts feeling faster...\n", drinkerName.c_str());
		itemEffectValue = 10*itemGrade;
		itemEffectValue += ((rand()%itemGrade)+1)*5;
		drinkerBonus.Points.Hit		+= itemEffectValue;
		if(0 == drinkerBonus.TurnsLeft.Hit)
			drinkerBonus.TurnsLeft.Hit = 1;
		drinkerBonus.TurnsLeft.Hit	+= itemGrade;
		finalPoints = calculateFinalPoints(potionDrinker);
		printf("The potion gives %s %u Hit chance points for %u turns. %s now has %u Hit chance points for the next %u turns.\n", drinkerName.c_str(), itemEffectValue, itemGrade, drinkerName.c_str(), finalPoints.Hit, drinkerBonus.TurnsLeft.Hit-1);
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

void useGrenade(const CItem& itemDescription, CCharacter& thrower, CCharacter& target) 
{
	// Currently the hit chance for all the grenade types are calculated with the same formula.
	int	lotteryRange	= 60+(10*itemDescription.Grade);	// calculate hit chance from item grade
	int	lotteryResult	= rand()%100;

	int itemEffectValue;
	int itemEffectValueSelf;
	ATTACK_TARGET hitTarget = ATTACK_TARGET_MISS;

	SCharacterPoints finalPoints;

	printf("%s throws %s to %s.\n", thrower.Name.c_str(), itemDescription.Name.c_str(), target.Name.c_str());
	switch(itemDescription.Property)
	{
	case PROPERTY_TYPE_BLAST:

		if(lotteryResult == lotteryRange)
		{
			finalPoints = calculateFinalPoints(thrower);
			itemEffectValueSelf	= int(finalPoints.MaxHP*(0.2f*itemDescription.Grade)); // this is the total damage points applied to the enemy or player
			thrower.Points.HP -= itemEffectValueSelf/2;	// this is the resulting hp after applying the damage 

			hitTarget = ATTACK_TARGET_SELF;
			printf("%s throws the grenade too close...\n"		
				"The grenade explodes near %s doing %u damage!\n", thrower.Name.c_str(), thrower.Name.c_str(), itemEffectValueSelf);
		}
		else if( lotteryResult == (lotteryRange-1) )
		{
			finalPoints = calculateFinalPoints(target);
			itemEffectValue	= int(finalPoints.MaxHP*(0.2f*itemDescription.Grade)); // this is the total damage points applied to the enemy or player
			target.Points.HP -= (itemEffectValue>>1);		// this is the resulting hp after applying the damage 

			finalPoints = calculateFinalPoints(thrower);
			itemEffectValueSelf	= int(finalPoints.MaxHP*(0.2f*itemDescription.Grade)); // this is the total damage points applied to the enemy or player
			thrower.Points.HP -= (itemEffectValue>>1);	// this is the resulting hp after applying the damage 

			hitTarget = (ATTACK_TARGET)(ATTACK_TARGET_SELF | ATTACK_TARGET_OTHER);
			printf("%s doesn't throw the grenade far enough so %s receives %u damage but also %s receives %u damage.\n", thrower.Name.c_str(), target.Name.c_str(), itemEffectValue, thrower.Name.c_str(), itemEffectValueSelf);
		}
		else if( lotteryResult < lotteryRange )
		{
			finalPoints = calculateFinalPoints(target);
			itemEffectValue	= int(finalPoints.MaxHP*(0.2f*itemDescription.Grade)); // this is the total damage points applied to the enemy or player
			target.Points.HP -= itemEffectValue;	// this is the resulting hp after applying the damage 

			hitTarget = ATTACK_TARGET_OTHER;
			printf("The grenade hits the target doing %u damage.\n", itemEffectValue);
		}
		else
			printf("%s throws the grenade too far away.\n", thrower.Name.c_str());

		break;
	case PROPERTY_TYPE_SMOKE:
	case PROPERTY_TYPE_PIERCING:
	default:
		printf("Grenade type not implemented!");
	};

	if(hitTarget & ATTACK_TARGET_OTHER) {
		thrower	.Score.DamageDealt += itemEffectValue; 
		target	.Score.DamageTaken += itemEffectValue; 
	}
	
	if(hitTarget & ATTACK_TARGET_SELF)  {
		thrower	.Score.DamageDealt += itemEffectValueSelf; 
		thrower	.Score.DamageTaken += itemEffectValueSelf; 
	}

	thrower.Score.GrenadesUsed++;
}

void executeItem(uint32_t indexInventory, CCharacter& user, CCharacter& target) {

	const CItem& itemDescription = itemDescriptions[user.Inventory.Slots[indexInventory].ItemIndex];
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

	user.Inventory.Slots[indexInventory].ItemCount--;
	if( user.Inventory.Slots[indexInventory].ItemCount )
		printf("\n%s has %u %s left.\n", user.Name.c_str(), user.Inventory.Slots[indexInventory].ItemCount, itemName.c_str());
	else 
	{
		user.Inventory.Slots[indexInventory] = user.Inventory.Slots[--user.Inventory.ItemCount];
		printf("\n%s ran out of %s.\n", user.Name.c_str(), itemName.c_str());
	}
}

// This function returns true if an item was used or false if the menu was exited without doing anything.
bool useItems(CCharacter& user, CCharacter& target)
{
	bool bUsedItem = false;
	uint32_t indexInventory = ~0U;
	CItem itemDescription;
	static const size_t inventorySize = size(user.Inventory.Slots);
	if(0 == user.Inventory.ItemCount)
	{
		printf("%s has no items in the inventory.\n", user.Name.c_str());
		return false;
	}

	if(user.Type == CHARACTER_TYPE_PLAYER) 
	{
		while(true)
		{
			printf("- Type %u to close your inventory.\n", (uint32_t)(inventorySize+1));
			showInventory(user);

			indexInventory = (uint32_t)(getNumericInput()-1);

			if(indexInventory == inventorySize) // exit option
				break;
			else if(indexInventory >= user.Inventory.ItemCount)	// invalid index means it's an invalid option
				printf("Invalid answer. Answer again...\n");
			else if (user.Inventory.Slots[indexInventory].ItemCount <= 0)
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
		indexInventory = (uint32_t)(rand() % user.Inventory.ItemCount);	// this should be improved.
		
		// Only use potions if we have less than 80% HP
		if	 ( ITEM_TYPE_POTION		!= itemDescriptions[user.Inventory.Slots[indexInventory].ItemIndex].Type
			|| PROPERTY_TYPE_HEALTH != itemDescriptions[user.Inventory.Slots[indexInventory].ItemIndex].Property
			|| user.Points.HP < ((user.Points.MaxHP+user.CombatBonus.Points.MaxHP)*.7f)
			)
			bUsedItem = true;
	}

	const SCharacterPoints finalPoints = calculateFinalPoints(user);

	if(bUsedItem)
	{
		const CItem& itemDescription = itemDescriptions[user.Inventory.Slots[indexInventory].ItemIndex];
		if( ITEM_TYPE_POTION == itemDescription.Type 
		 && PROPERTY_TYPE_HEALTH == itemDescription.Property 
		 && user.Points.HP == finalPoints.MaxHP)
		{
			bUsedItem = false;
			printf("Your HP is full!");
		}
		else
			executeItem(indexInventory, user, target);
	}
	
	return bUsedItem;
}