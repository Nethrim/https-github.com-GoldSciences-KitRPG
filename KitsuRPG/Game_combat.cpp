#include "Game.h"

#include "Weapon.h"
#include "Item.h"
#include "Enemy.h"
#include "Armor.h"

#include "Menu.h"

bool useItems(CCharacter& adventurer, CCharacter& target);	// While in combat, displays a list of the available items to use.

// Returns final passthrough damage (not blocked by shield) to use by vampire effect.
int32_t applyShieldableDamage(CCharacter& target, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName)
{
	//int32_t scaledDamage		= damageDealt		*100;
	//int32_t scaledAbsorption	= absorptionRate	*100;

	const double absorptionFraction = absorptionRate ? (0.01*absorptionRate) : 0.0;
	int shieldedDamage		= (int)(damageDealt * absorptionFraction);
	int passthroughDamage	= (int)(damageDealt * (1.0-absorptionFraction));
	int totalDamage			= shieldedDamage+passthroughDamage;
	
	int finalPassthroughDamage = 0;

	printf("Absorption rate: %%%f. Shielded damage: %u. Passthrough damage: %u. Expected sum: %u. Actual sum: %u.\n", (float)(absorptionFraction*100), shieldedDamage, passthroughDamage, damageDealt, totalDamage);
	if( totalDamage < damageDealt )
	{
		int errorDamage = damageDealt-totalDamage;
		if( 0 == absorptionRate )
		{
			printf("%u damage error will be applied to the health.\n", errorDamage);
			passthroughDamage += errorDamage;
		}
		else
		{
			printf("%u damage error will be applied to the shield.\n", errorDamage);
			shieldedDamage += errorDamage;
		}
	}

	if(shieldedDamage)
	{
		int remainingShield = target.Shield-shieldedDamage;
		if(target.Shield)
		{
			printf("%s's shield absorbs %u damage from %s.\n", target.Name.c_str(), std::min(target.Shield, shieldedDamage), sourceName.c_str());
			target.Shield = std::max(0, remainingShield);
			if(remainingShield < 0)
				printf("%s's shield ran out allowing %u damage from %s to pass through.\n", target.Name.c_str(), remainingShield*-1, sourceName.c_str());
			else
				printf("%s's remaining shield is now %u.\n", target.Name.c_str(), target.Shield);
		}
		if(remainingShield < 0)	// only apply damage to health if the shield didn't absorb all the damage.
		{
			finalPassthroughDamage	= remainingShield*-1;
			printf("%s's wasn't protect against %u shieldable damage from %s.\n", target.Name.c_str(), finalPassthroughDamage, sourceName.c_str());
			target.Points.HP += remainingShield;
		}
	}

	if(passthroughDamage)
		printf("%s receives %u passthrough damage from %s.\n", target.Name.c_str(), passthroughDamage, sourceName.c_str());
	finalPassthroughDamage	+= passthroughDamage;

	target.Points.HP -= passthroughDamage;

	return finalPassthroughDamage;
}

static inline int32_t applyShieldableDamage(CCharacter& target, int32_t damageDealt, const std::string& sourceName) {
	return applyShieldableDamage(target, damageDealt, armorDefinitions[target.Armor].Absorption, sourceName);
}

// This function returns the damage dealt to the target
int attack(CCharacter& attacker, CCharacter& target)
{
	// Calculate success from the hit chance and apply damage to target or just print the miss message.
	int32_t damageDealt = 0;
	const CWeapon& weaponDefinition = weaponDefinitions[attacker.Weapon];
	const CWeaponModifier& weaponModifier = weaponModifiers[attacker.WeaponModifier];
	STATUS_TYPE finalWeaponStatus = (STATUS_TYPE)(weaponModifier.Status | weaponDefinition.Status);
	WEAPON_EFFECT finalWeaponEffect = (WEAPON_EFFECT)(weaponModifier.Effect | weaponDefinition.Effect);
	int finalWeaponCoins	= weaponDefinition.Points.Coins	+	weaponModifier.Points.Coins	;
	int finalWeaponHP		= weaponDefinition.Points.HP	+	weaponModifier.Points.HP	;

	const std::string weaponName = getWeaponName(attacker.Weapon, attacker.WeaponModifier);

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

		int finalPassthroughDamage = applyShieldableDamage(target, damageDealt, attacker.Name);

		if(finalWeaponCoins)
			printf("%s gives %s %u Coins.\n", weaponName.c_str(), attacker.Name.c_str(), finalWeaponCoins);
		attacker.Points.Coins	+= finalWeaponCoins;

		if(finalWeaponHP)
			printf("%s gives %s %u Health Points.\n", weaponName.c_str(), attacker.Name.c_str(), finalWeaponHP);
		attacker.Points.HP		+= finalWeaponHP;

		attacker.Points.HP		= std::min(attacker.Points.HP, attackerPoints.MaxHP);

		if(finalWeaponEffect & WEAPON_EFFECT_LEECH)
		{
			int actualHPGained = std::min(finalPassthroughDamage, attackerPoints.MaxHP-attacker.Points.HP);
			if(actualHPGained > 0)
				printf("%s drains %u HP from %s.\n", attacker.Name.c_str(), actualHPGained, weaponName.c_str());
			else if(actualHPGained < 0)
				printf("%s loses %u HP from %s.\n", attacker.Name.c_str(), actualHPGained, weaponName.c_str());
			attacker.Points.HP		+= actualHPGained;
		}

		if(finalWeaponEffect & WEAPON_EFFECT_STEAL)
		{
			int actualCoinsGained = std::min(finalPassthroughDamage, target.Points.Coins);
			target.Points.Coins -= actualCoinsGained;
			if(actualCoinsGained > 0)
				printf("%s steals %u Coins from %s.\n", attacker.Name.c_str(), actualCoinsGained, weaponName.c_str());
			else if(actualCoinsGained < 0)
				printf("%s drops %u Coins from %s.\n", attacker.Name.c_str(), actualCoinsGained, weaponName.c_str());
			attacker.Points.Coins	+= actualCoinsGained;
		}

		for(int i=0; i<MAX_STATUS_COUNT; i++)
		{
			STATUS_TYPE bitStatus =  (STATUS_TYPE)(1<<i);
			if(0 == (bitStatus & finalWeaponStatus) || (rand()%2))
				continue;

			std::string text;
			int turns = 2;
				 
			switch(1<<i) {
			case STATUS_TYPE_STUN		:	text = "%s will be stunned by %s for the next %u turns.\n"	; break;
			case STATUS_TYPE_BLIND		:	text = "%s will be blind by %s for the next %u turns.\n"	; break;
			case STATUS_TYPE_BLEEDING	:	text = "%s will bleed by %s for the next %u turns.\n"		; break;
			case STATUS_TYPE_BURN		:	text = "%s will burn by %s for the next %u turns.\n"		; break;
			case STATUS_TYPE_POISON		:	text = "%s will be poisoned by %s for the next %u turns.\n"	; break;
			default:						text = "An unknown status was added to the character.\n"	; break;
			}
			
			addStatus(target.CombatStatus, bitStatus, 1);
			printf(text.c_str(), target.Name.c_str(), weaponName.c_str(), turns-1);
		}


	}
	else 
		printf("%s misses the attack!\n", attacker.Name.c_str());

	return damageDealt;
};

void attackTurn(CCharacter& attacker, CCharacter& target)
{
	if(attacker.Type == CHARACTER_TYPE_PLAYER)
		printf("You decide to attack %s!\n", target.Name.c_str()); 

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

void assignDrops(CCharacter& winner, CCharacter& loser)
{
	printf("%s is dead!\n", loser.Name.c_str());
	int drop = loser.Points.Coins - (rand() % (1+loser.Points.Coins/3));
	printf("\n%s dropped %u coins!!\n", loser.Name.c_str(), drop);
	winner.Points.Coins += drop;
	loser.Points.Coins	-= drop;
	for(uint32_t i=0; i<loser.Inventory.ItemCount; i++) 
		if(rand()%2) 
		{
			const SInventorySlot& itemDrop = loser.Inventory.Slots[i];
			if(addItem(winner.Inventory, itemDrop.ItemIndex))
				printf("\n%s dropped %s!!\n", loser.Name.c_str(), itemDescriptions[itemDrop.ItemIndex].Name.c_str());
			else
				printf("%s can't pick up %s by %s because the inventory is full!\n", winner.Name.c_str(), itemDescriptions[itemDrop.ItemIndex].Name.c_str(), loser.Name.c_str());
			loser.Inventory.Slots[i] = loser.Inventory.Slots[--loser.Inventory.ItemCount];
		}

	if(loser.Weapon > winner.Weapon)
	{
		if(rand()%2)
		{
			printf("%s recovers %s from %s.\n", winner.Name.c_str(), getWeaponName(loser.Weapon, loser.WeaponModifier).c_str(), loser.Name.c_str());
			winner.Weapon			= loser.Weapon;
			winner.WeaponModifier	= loser.WeaponModifier;
			loser.Weapon			= rand() % (winner.Weapon+1);
			loser.WeaponModifier	= rand() % (winner.WeaponModifier+1);
		}
		else
		{
			printf("%s dropped by %s is too damaged to be recovered.\n", getWeaponName(loser.Weapon, loser.WeaponModifier).c_str(), loser.Name.c_str());
			loser.Weapon			= 0;
			loser.WeaponModifier	= 0;
		}
	}

	if(loser.Armor > winner.Armor)
	{
		if(rand()%2)
		{
			printf("%s recovers %s from %s.\n", winner.Name.c_str(), getArmorName(loser.Armor, loser.ArmorModifier).c_str(), loser.Name.c_str());
			winner.Armor			= loser.Armor;
			winner.ArmorModifier	= loser.ArmorModifier;
			loser.Armor				= rand() % (winner.Armor+1);
			loser.ArmorModifier		= rand() % (winner.ArmorModifier+1);
		}
		else
		{
			printf("%s dropped by %s is too damaged to be recovered.\n", getArmorName(loser.Armor, loser.ArmorModifier).c_str(), loser.Name.c_str());
			loser.Armor			= 0;
			loser.ArmorModifier	= 0;
		}
	}

	winner.Points.MaxHP += winner.Points.Attack;

	winner.Score.BattlesWon++;
	winner.Score.EnemiesKilled++;
	winner.Score.MoneyEarned += drop;

	loser.Score.BattlesLost++;

}

void determineOutcome(CCharacter& adventurer, CCharacter& enemy, uint32_t enemyType)
{
		// Determine the outcome of the battle and give rewards if applicable.
	if (adventurer.Points.HP <= 0) 
		assignDrops(enemy, adventurer);
	if (enemy.Points.HP <= 0)
		assignDrops(adventurer, enemy);
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
		case STATUS_TYPE_BLEEDING:	amount = std::max(1, finalPoints.MaxHP/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, 0, "bleeding");	break;
		case STATUS_TYPE_POISON:	amount = std::max(1, finalPoints.MaxHP/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, 0, "poisoning");	break;
		case STATUS_TYPE_BURN:		amount = std::max(1, finalPoints.MaxHP/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, armorDefinitions[character.Armor].Absorption, "burning");	break;
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
		case STATUS_TYPE_BURN:		printf("%s is burning for the next %u turn(s).\n"	, character.Name.c_str(), character.CombatStatus.TurnsLeft[i]);	break;
		case STATUS_TYPE_POISON:	printf("%s is poisoned for the next %u turn(s).\n"	, character.Name.c_str(), character.CombatStatus.TurnsLeft[i]);	break;
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
		printf("\n-- %s HP is: %u. Shield: %u.\nHit Chance: %u.\nAttack: %u.\nHit Chance Bonus Turns: %u.\nAttack Bonus Turns: %u.\nWeapon: %s.\nArmor: %s.\n",  adventurer	.Name.c_str(), adventurer	.Points.HP,	adventurer	.Shield,	 playerPoints	.Hit, playerPoints	.Attack,	adventurer		.CombatBonus.TurnsLeft.Hit,adventurer	.CombatBonus.TurnsLeft.Attack,		getWeaponName(adventurer.Weapon, adventurer.WeaponModifier).c_str(), getArmorName(adventurer.Armor, adventurer.ArmorModifier).c_str());
		printStatuses(adventurer);
		printf("\n-- %s HP is: %u. Shield: %u.\nHit Chance: %u.\nAttack: %u.\nHit Chance Bonus Turns: %u.\nAttack Bonus Turns: %u.\nWeapon: %s.\nArmor: %s.\n",  currentEnemy	.Name.c_str(), currentEnemy	.Points.HP,	currentEnemy.Shield,	 enemyPoints	.Hit, enemyPoints	.Attack,	currentEnemy	.CombatBonus.TurnsLeft.Hit,currentEnemy	.CombatBonus.TurnsLeft.Attack,	getWeaponName(currentEnemy.Weapon, currentEnemy.WeaponModifier).c_str(), getArmorName(currentEnemy.Armor, currentEnemy.ArmorModifier).c_str());
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

	currentEnemy.Weapon			= rand() % size(weaponDefinitions	);//	std::min(enemyType*2, size(weaponDefinitions));
	currentEnemy.Armor			= rand() % size(armorDefinitions	);//	std::min(enemyType*2, size(armorDefinitions));
	currentEnemy.WeaponModifier	= rand() % size(weaponModifiers		);//	std::min(enemyType*2, size(weaponModifiers));
	currentEnemy.ArmorModifier	= rand() % size( armorModifiers		);//	std::min(enemyType*2, size( armorModifiers));
	currentEnemy.Shield			= armorDefinitions[currentEnemy.Armor].Shield+armorModifiers[currentEnemy.ArmorModifier].Shield;

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
	applyCombatBonus(character, armorDefinitions[character.Armor].Points, getArmorName(character.Armor, character.ArmorModifier));
	applyCombatBonus(character, armorModifiers[character.ArmorModifier].Points, getArmorName(character.Armor, character.ArmorModifier));
};

//5736	// gasty.bellino@gmail.com
void combat(CCharacter& adventurer, uint32_t enemyType)
{
	CCharacter currentEnemy = enemyDefinitions[enemyType];	// Request the enemy data.
	setupEnemy(currentEnemy, enemyType);

	adventurer.CombatStatus.Count	= 0;	// We need to clear the combat status before starting the combat.
	adventurer.Shield				= armorDefinitions[adventurer.Armor].Shield+armorModifiers[adventurer.ArmorModifier].Shield;

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

		if(!combatContinues(turnOutcome, adventurer.Points.HP, currentEnemy.Points.HP))
			break;

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

		if(!combatContinues(turnOutcome, adventurer.Points.HP, currentEnemy.Points.HP))
			break;

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

STATUS_TYPE getGrenadeStatusFromProperty(PROPERTY_TYPE grenadeProperty)
{
	STATUS_TYPE result = STATUS_TYPE_NONE;
	switch(grenadeProperty) {
	case PROPERTY_TYPE_STUN		:		result = STATUS_TYPE_STUN		; break;
	case PROPERTY_TYPE_SMOKE	:		result = STATUS_TYPE_BLIND		; break;
	case PROPERTY_TYPE_PIERCING	:		result = STATUS_TYPE_BLEEDING	; break;
	case PROPERTY_TYPE_FIRE		:		result = STATUS_TYPE_BURN		; break;
	case PROPERTY_TYPE_POISON	:		result = STATUS_TYPE_POISON		; break;
	//case PROPERTY_TYPE_FREEZE	:		result = STATUS_TYPE_; break;
	//case PROPERTY_TYPE_			:		result = STATUS_TYPE_; break;
	}
	return result;
}

void useGrenade(const CItem& itemDescription, CCharacter& thrower, CCharacter& target) 
{
	// Currently the hit chance for all the grenade types are calculated with the same formula.
	int	lotteryRange	= 60+(10*itemDescription.Grade);	// calculate hit chance from item grade
	int	lotteryResult	= rand()%100;

	SCharacterPoints 
		finalPointsThrower	= calculateFinalPoints(thrower), 
		finalPointsTarget	= calculateFinalPoints(target);

	int itemEffectValue				= int(finalPointsTarget.MaxHP*(0.2f*itemDescription.Grade));
	int itemEffectValueSelf			= int(finalPointsThrower.MaxHP*(0.2f*itemDescription.Grade)) >> 1;

	int itemEffectValueReduced		= int(finalPointsTarget.MaxHP*(0.1f*itemDescription.Grade));
	int itemEffectValueReducedSelf	= int(finalPointsThrower.MaxHP*(0.1f*itemDescription.Grade)) >> 1;

	ATTACK_TARGET hitTarget = ATTACK_TARGET_MISS;

	printf("%s throws %s to %s.\n", thrower.Name.c_str(), itemDescription.Name.c_str(), target.Name.c_str());
	bool bAddStatus = false;


	PROPERTY_TYPE	grenadeProperty = itemDescription.Property;
	STATUS_TYPE		grenadeStatus = getGrenadeStatusFromProperty(grenadeProperty);
	switch(grenadeProperty)
	{
	case PROPERTY_TYPE_SMOKE:
	case PROPERTY_TYPE_STUN:
		if( lotteryResult < lotteryRange )
		{
			addStatus(target.CombatStatus, grenadeStatus, 1+itemDescription.Grade);
			switch(grenadeProperty) {
			case PROPERTY_TYPE_SMOKE	: printf("The grenade hits the target leaving %s blind for %u turns.\n"		, target.Name.c_str(), 1+itemDescription.Grade);	  break;
			case PROPERTY_TYPE_STUN		: printf("The grenade hits the target leaving %s stunned for %u turns.\n"	, target.Name.c_str(), 1+itemDescription.Grade); break;
			}
		}
		else
			printf("%s throws the grenade too far away.\n", thrower.Name.c_str());

		break;

	case PROPERTY_TYPE_PIERCING:
	case PROPERTY_TYPE_FIRE:
	case PROPERTY_TYPE_POISON:
		itemEffectValueSelf = itemEffectValueReducedSelf;
		itemEffectValue		= itemEffectValueReduced;
		bAddStatus			= true;
	case PROPERTY_TYPE_BLAST:
		if(lotteryResult == lotteryRange)
		{
			applyShieldableDamage(thrower, itemEffectValueSelf, itemDescription.Name);
			if(bAddStatus)
				addStatus(thrower.CombatStatus, grenadeStatus, 1*itemDescription.Grade);

			hitTarget = ATTACK_TARGET_SELF;
			printf("%s throws the grenade too close...\n"		
				"The grenade explodes near %s doing %u damage!\n", thrower.Name.c_str(), thrower.Name.c_str(), itemEffectValueSelf);
		}
		else if( lotteryResult == (lotteryRange-1) )
		{
			applyShieldableDamage(target,	itemEffectValue		>> 1, itemDescription.Name);
			applyShieldableDamage(thrower,	itemEffectValueSelf	>> 1, itemDescription.Name);
			if(bAddStatus)
			{
				addStatus(target.CombatStatus	, grenadeStatus, 2*itemDescription.Grade);
				addStatus(thrower.CombatStatus	, grenadeStatus, 1*itemDescription.Grade);
			}

			hitTarget = (ATTACK_TARGET)(ATTACK_TARGET_SELF | ATTACK_TARGET_OTHER);
			printf("%s doesn't throw the grenade far enough so %s receives %u damage but also %s receives %u damage.\n", thrower.Name.c_str(), target.Name.c_str(), itemEffectValue, thrower.Name.c_str(), itemEffectValueSelf);
		}
		else if( lotteryResult < lotteryRange )
		{
			applyShieldableDamage(target, itemEffectValue, itemDescription.Name);
			if(bAddStatus)
				addStatus(target.CombatStatus, grenadeStatus, (uint32_t)(3.6f*itemDescription.Grade));

			hitTarget = ATTACK_TARGET_OTHER;
			printf("The grenade hits the target doing %u damage.\n", itemEffectValue);
		}
		else
			printf("%s throws the grenade too far away.\n", thrower.Name.c_str());

		if(hitTarget & ATTACK_TARGET_OTHER) {
			thrower	.Score.DamageDealt += itemEffectValue; 
			target	.Score.DamageTaken += itemEffectValue; 
		}
	
		if(hitTarget & ATTACK_TARGET_SELF)  {
			thrower	.Score.DamageDealt += itemEffectValueSelf; 
			thrower	.Score.DamageTaken += itemEffectValueSelf; 
		}
		break;
	default:
		printf("Grenade type not implemented!");
	};


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