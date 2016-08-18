#include "Game.h"

#include "Profession.h"
#include "Weapon.h"
#include "Item.h"
#include "Enemy.h"
#include "Armor.h"

#include "Menu.h"

bool useItems(CCharacter& adventurer, CCharacter& target);	// While in combat, displays a list of the available items to use.

// Returns final passthrough damage (not blocked by shield) to use by vampire effect.
int32_t applyShieldableDamage(CCharacter& target, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName)
{
	if(0 >= target.Points.HP)	// This character is already dead
		return 0;

	const std::string	targetArmorName		= getArmorName	(target.Armor);
	const int32_t		targetArmorShield	= getArmorShield(target.Armor);
	
	// Impenetrable armors always have 100% 
	if(getArmorEffect(target.Armor) & ARMOR_EFFECT_IMPENETRABLE)
	{
		if(target.Points.Shield)
		{
			absorptionRate += 60;
			printf("%s damage absorption rate for %s is raised to %%%u because of the impenetrable property.\n", targetArmorName.c_str(), sourceName.c_str(), absorptionRate);
		}
	}
	else
	{	
		// If the armor is not impenetrable, the absorption rate is affected by the shield damage.
		printf("%s damage absorption rate for %s is %%%u.\n", targetArmorName.c_str(), sourceName.c_str(), absorptionRate);
		if(target.Points.Shield) 
			absorptionRate = absorptionRate ? std::max((int32_t)(absorptionRate*(target.Points.Shield/(double)targetArmorShield)), 1) : 0;

		printf("%s final damage absorption rate taking deterioration into account is %%%u.\n", targetArmorName.c_str(), absorptionRate);
	}
	absorptionRate = std::min(absorptionRate, 100);
	const double	absorptionFraction	= absorptionRate ? (0.01*absorptionRate) : 0.0;
	int shieldedDamage		= (int)(damageDealt * absorptionFraction);
	int passthroughDamage	= (int)(damageDealt * std::max(0.0, 1.0-absorptionFraction));
	int totalDamage			= shieldedDamage+passthroughDamage;
	
	printf("Shielded damage: %u. Passthrough damage: %u. Expected sum: %u. Actual sum: %u. Absorption ratio: %%%u.\n", shieldedDamage, passthroughDamage, damageDealt, totalDamage, absorptionRate);
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

	int finalPassthroughDamage = 0;

	if(shieldedDamage)
	{
		int remainingShield = target.Points.Shield-shieldedDamage;
		if(target.Points.Shield)
		{
			printf("%s's shield absorbs %u damage from %s.\n", target.Name.c_str(), std::min(target.Points.Shield, shieldedDamage), sourceName.c_str());
			target.Points.Shield = std::max(0, remainingShield);
			if(remainingShield < 0)
				printf("%s's shield ran out allowing some damage from %s to pass through.\n", target.Name.c_str(), sourceName.c_str());
			else
				printf("%s's remaining shield is now %u.\n", target.Name.c_str(), target.Points.Shield);
		}
		if(remainingShield < 0)	// only apply damage to health if the shield didn't absorb all the damage.
		{
			finalPassthroughDamage	= remainingShield*-1;
			printf("%s's wasn't protected against %u shieldable damage from %s.\n", target.Name.c_str(), finalPassthroughDamage, sourceName.c_str());
			target.Points.HP += remainingShield;
		}
	}

	if(passthroughDamage)
		printf("%s receives %u passthrough damage from %s.\n", target.Name.c_str(), passthroughDamage, sourceName.c_str());
	finalPassthroughDamage	+= passthroughDamage;

	target.Points.HP -= passthroughDamage;

	return finalPassthroughDamage;
}

void applyTurnStatus(CCharacter& character)
{
	if(0 >= character.Points.HP)	// This character is already dead
		return;

	int amount=0;
	const SCharacterPoints finalPoints = calculateFinalPoints(character);
	for(uint32_t i=0; i<character.CombatStatus.Count; ++i)
	{
		switch(character.CombatStatus.Status[i])
		{
		case STATUS_TYPE_BLEEDING:	amount = std::max(1, finalPoints.MaxHP/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, 0, "bleeding");	break;
		case STATUS_TYPE_POISON:	amount = std::max(1, finalPoints.MaxHP/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, 0, "poisoning");	break;
		case STATUS_TYPE_BURN:		amount = std::max(1, finalPoints.MaxHP/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, getArmorAbsorption(character.Armor), "burning");	break;
		//case STATUS_TYPE_STUN:		break;
		//case STATUS_TYPE_BLIND:		break;
		}
	}
};

void applyCombatBonus(CCharacter& character, const SCharacterPoints& combatBonus, const std::string& sourceName)
{
	if(0 >= character.Points.HP)	// This character is already dead
		return;

	if(combatBonus.HP)
	{
		if(combatBonus.HP > 0 && character.Points.HP < calculateFinalPoints(character).MaxHP)
		{
			int finalHPAdded = std::min(combatBonus.HP, calculateFinalPoints(character).MaxHP-character.Points.HP);
			printf("%s gains %u HP from %s.\n", character.Name.c_str(), finalHPAdded, sourceName.c_str());
			character.Points.HP		+= finalHPAdded;
		}
		else if(combatBonus.HP < 0 && character.Points.HP)
		{
			int finalHPRemoved = std::min(-combatBonus.HP, character.Points.HP);
			printf("%s loses %u HP from %s.\n", character.Name.c_str(), finalHPRemoved, sourceName.c_str());
			character.Points.HP			-= finalHPRemoved;
		}
	}
	if(combatBonus.Coins)
	{
		if(combatBonus.Coins > 0)
			printf("%s gains %u Coins from %s.\n", character.Name.c_str(), combatBonus.Coins, sourceName.c_str());
		else if(combatBonus.Coins < 0 && character.Points.Coins)
			printf("%s loses %u Coins from %s.\n", character.Name.c_str(), combatBonus.Coins*-1, sourceName.c_str());
		character.Points.Coins	+= combatBonus.Coins;
	}
};

void applyArmorEffect(CCharacter& character)
{
	if(0 >= character.Points.HP)	// This character is already dead
		return;

	const std::string armorName = getArmorName(character.Armor);
	ARMOR_EFFECT	armorBaseEffect			= armorDefinitions	[character.Armor.Index]		.Effect;
	ARMOR_EFFECT	armorModifierEffect		= armorModifiers	[character.Armor.Modifier]	.Effect;
	int32_t			armorBaseShield			= armorDefinitions	[character.Armor.Index]		.Shield;
	int32_t			armorModifierShield		= armorModifiers	[character.Armor.Modifier]	.Shield;
	int32_t			totalArmorShield			= getArmorShield(character.Armor);
	if((armorBaseEffect & ARMOR_EFFECT_RECHARGE) && character.Points.Shield < totalArmorShield) {
		int32_t shieldToAdd		= totalArmorShield/20;
		shieldToAdd				= std::max(1, std::min(shieldToAdd, totalArmorShield-character.Points.Shield));
		character.Points.Shield	+= shieldToAdd;
		printf("%s recharges by %u.\n", armorName.c_str(), shieldToAdd);
	};
	if((armorModifierEffect & ARMOR_EFFECT_RECHARGE) && character.Points.Shield < totalArmorShield) {
		int32_t shieldToAdd		= totalArmorShield/20;
		shieldToAdd				= std::max(1, std::min(shieldToAdd, totalArmorShield-character.Points.Shield));
		character.Points.Shield	+= shieldToAdd;
		printf("%s recharges by %u.\n", armorName.c_str(), shieldToAdd);
	};
};

void applyTurnStatusAndBonusesAndSkipTurn(CCharacter& character)
{
	if(0 >= character.Points.HP)	// This character is already dead
		return;

	printf("\n");
	applyTurnStatus(character);
	applyCombatBonus(character, character.CombatBonus.Points, "Turn Combat Bonus");
	applyCombatBonus(character, getProfessionPoints(character.Profession), getProfessionName(character.Profession).c_str());
	applyCombatBonus(character, getArmorPoints(character.Armor), getArmorName(character.Armor));
	applyArmorEffect(character);
	character.CombatBonus.NextTurn();
	character.CombatStatus.NextTurn();
	printf("\n");
}


static inline int32_t applyShieldableDamage(CCharacter& target, int32_t damageDealt, const std::string& sourceName) {
	return applyShieldableDamage(target, damageDealt, getArmorAbsorption(target.Armor), sourceName);
}

static int32_t applyArmorReflect(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName) {
	
	if( 0 == damageDealt || 0 == (getArmorEffect(targetReflecting.Armor) & ARMOR_EFFECT_REFLECT))
		return 0;

	const std::string	targetArmorName			= getArmorName	(targetReflecting.Armor);
	const std::string	attackerArmorName		= getArmorName	(attacker.Armor);
	if(damageDealt > 0)
		printf("%s reflects %u damage from %s with %s.\n", targetReflecting.Name.c_str(), damageDealt, sourceName.c_str(), targetArmorName.c_str());
	else if(damageDealt < 0)
		printf("%s reflects %u health from %s with %s.\n", targetReflecting.Name.c_str(), damageDealt, sourceName.c_str(), targetArmorName.c_str());
	int32_t finalPassthroughDamage = applyShieldableDamage(attacker, damageDealt, targetArmorName);
	int reflectedDamage = damageDealt-finalPassthroughDamage;
	if(reflectedDamage)
	{
		ARMOR_EFFECT attackerArmorEffect = (ARMOR_EFFECT)getArmorEffect(attacker.Armor);
		if(attackerArmorEffect & ARMOR_EFFECT_REFLECT)
		{
			printf("\n%s causes a recursive reflection with %s.\n", attackerArmorName.c_str(), targetArmorName.c_str());
			applyArmorReflect(targetReflecting, attacker, reflectedDamage, attackerArmorName);
		}
	}

	return finalPassthroughDamage;
}

STATUS_TYPE applyAttackStatus(CCharacter& target, STATUS_TYPE weaponStatus, int32_t absorbChance, int32_t turnCount, const std::string& sourceName)
{
	if(weaponStatus == STATUS_TYPE_NONE)
		return STATUS_TYPE_NONE;

	const std::string	targetArmorName			= getArmorName(target.Armor);

	STATUS_TYPE appliedStatus = STATUS_TYPE_NONE;

	if((getArmorEffect(target.Armor) & ARMOR_EFFECT_IMPENETRABLE) && target.Points.Shield)
	{
		if(absorbChance)
			absorbChance = std::max(60, absorbChance*2);
		else
			absorbChance = 60;
		printf("%s absorb chance of status by %s is modified to %%%u because of the impenetrable property of %s.\n", target.Name.c_str(), sourceName.c_str(), absorbChance, targetArmorName.c_str());
	}
	
	for(int i=0; i<MAX_STATUS_COUNT; i++)
	{
		STATUS_TYPE bitStatus =  (STATUS_TYPE)(1<<i);
		if(0 == (bitStatus & weaponStatus) )
			continue;

		std::string text;
		if((rand()%100) < absorbChance)
		{
			switch(bitStatus) {
			case STATUS_TYPE_STUN		:	text = "Stun"			;	break;
			case STATUS_TYPE_BLIND		:	text = "Blind"			;	break;
			case STATUS_TYPE_BLEEDING	:	text = "Bleeding"		;	break;
			case STATUS_TYPE_BURN		:	text = "Burn"			;	break;
			case STATUS_TYPE_POISON		:	text = "Poison"			;	break;
			default:						text = "Unknown Status"	;	break;
			}

			printf("%s absorbs \"%s\" inflicted by %s with %%%u absorb chance.\n", targetArmorName.c_str(), text.c_str(), sourceName.c_str(), absorbChance);
			continue;
		}

		addStatus(target.CombatStatus, bitStatus, turnCount);
		appliedStatus = (STATUS_TYPE)(appliedStatus|bitStatus);

		switch(bitStatus) {
		case STATUS_TYPE_STUN		:	text = "%s will be stunned by %s for the next %u turns.\n"	; break;
		case STATUS_TYPE_BLIND		:	text = "%s will be blind by %s for the next %u turns.\n"	; break;
		case STATUS_TYPE_BLEEDING	:	text = "%s will bleed by %s for the next %u turns.\n"		; break;
		case STATUS_TYPE_BURN		:	text = "%s will burn by %s for the next %u turns.\n"		; break;
		case STATUS_TYPE_POISON		:	text = "%s will be poisoned by %s for the next %u turns.\n"	; break;
		default:						text = "An unknown status was added to the character.\n"	; break;
		}
			
		printf(text.c_str(), target.Name.c_str(), sourceName.c_str(), turnCount);
	}
	return appliedStatus;
}

STATUS_TYPE applyAttackStatus(CCharacter& target, STATUS_TYPE weaponStatus, int32_t turnCount, const std::string& sourceName)
{
	if(weaponStatus == STATUS_TYPE_NONE)
		return STATUS_TYPE_NONE;

	const int32_t		targetArmorAbsorption	= getArmorAbsorption(target.Armor);
	const std::string	targetArmorName			= getArmorName(target.Armor);
	const int32_t		targetArmorShield		= getArmorShield(target.Armor);
	const ARMOR_EFFECT	targetArmorEffect		= getArmorEffect(target.Armor);

	STATUS_TYPE appliedStatus = STATUS_TYPE_NONE;
	
	int32_t absorbChance;

	//double absorptionRatio = std::max(0.0, ((target.Shield+target.Points.HP)/(double)(targetArmorShield+calculateFinalPoints(target).MaxHP)));
	double absorptionRatio = std::max(0.0, (target.Points.Shield/(double)targetArmorShield))/2.0;
	absorbChance = 50+(int32_t)(absorptionRatio*100);
	absorbChance = std::min(absorbChance, 100);

	printf("%s status absorb chance after absorption calculation is %%%u.\n", target.Name.c_str(), absorbChance);
	return applyAttackStatus(target, weaponStatus, absorbChance, turnCount, sourceName);
}

// This function returns the damage dealt to the target
int attack(CCharacter& attacker, CCharacter& target)
{
	// Calculate success from the hit chance and apply damage to target or just print the miss message.
	int32_t damageDealt = 0;
	const SCharacterPoints attackerWeaponPoints = getWeaponPoints(attacker.Weapon);

	STATUS_TYPE			attackerWeaponStatus	= (STATUS_TYPE)		getWeaponStatus(attacker.Weapon);
	WEAPON_EFFECT		attackerWeaponEffect	= (WEAPON_EFFECT)	getWeaponEffect(attacker.Weapon);
	ARMOR_EFFECT		targetArmorEffect		= (ARMOR_EFFECT)	getArmorEffect(target.Armor);
	const std::string	attackerWeaponName		= getWeaponName	(attacker.Weapon);
	const std::string	targetArmorName			= getArmorName	(target.Armor);

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

		// Apply combat bonuses from weapon for successful hits.
		applyCombatBonus(attacker, attackerWeaponPoints, attackerWeaponName);

		// Apply weapon effects for successful hits.
		if(attackerWeaponEffect & WEAPON_EFFECT_LEECH)
		{
			int actualHPGained = std::min(finalPassthroughDamage, attackerPoints.MaxHP-attacker.Points.HP);
			if(actualHPGained > 0)
				printf("%s drains %u HP from %s with %s.\n", attacker.Name.c_str(), actualHPGained, target.Name.c_str(), attackerWeaponName.c_str());
			else if(actualHPGained < 0)
				printf("%s loses %u HP from %s with %s.\n", attacker.Name.c_str(), actualHPGained, target.Name.c_str(), attackerWeaponName.c_str());
			attacker.Points.HP		+= actualHPGained;
		}

		if(attackerWeaponEffect & WEAPON_EFFECT_STEAL)
		{
			int actualCoinsGained = std::min(finalPassthroughDamage, target.Points.Coins);
			target.Points.Coins -= actualCoinsGained;
			if(actualCoinsGained > 0)
				printf("%s steals %u Coins from %s with %s.\n", attacker.Name.c_str(), actualCoinsGained, target.Name.c_str(), attackerWeaponName.c_str());
			else if(actualCoinsGained < 0)
				printf("%s drops %u Coins from %s with %s.\n", attacker.Name.c_str(), actualCoinsGained, target.Name.c_str(), attackerWeaponName.c_str());
			attacker.Points.Coins	+= actualCoinsGained;
		}

		// Call applyArmorReflect which will cancel automatically if reflect is not available in target's armor.
		int shieldedDamage = damageDealt-finalPassthroughDamage;
		applyArmorReflect(attacker, target, shieldedDamage, attackerWeaponName);

		// Apply attack statuses.
		int turns = 1;
		applyAttackStatus(target, attackerWeaponStatus, turns, attackerWeaponName);
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
	else if(loser.Weapon.Index == winner.Weapon.Index && (loser.Weapon.Modifier > winner.Weapon.Modifier && loser.Weapon.Level > winner.Weapon.Level))
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
	else if(loser.Armor.Index == winner.Armor.Index && (loser.Armor.Modifier > winner.Armor.Modifier && loser.Armor.Level > winner.Armor.Level))
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

	//winner.Points.MaxHP += winner.Points.Attack;
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
	if (adventurer.Points.HP <= 0) 
		assignDrops(enemy, adventurer);
	if (enemy.Points.HP <= 0)
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
		attackTurn(attacker, target);
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

	if(outcome == TURN_OUTCOME_CANCEL && target.Points.HP > 0 && attacker.Points.HP > 0)
		applyTurnStatusAndBonusesAndSkipTurn(attacker);

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
		printf("\n-- %s is a %s level %u.\nWeapon: %s level %u.\nArmor: %s level %u.\nHit Chance Bonus Turns: %u.\nAttack Bonus Turns: %u.\n",  
			adventurer		.Name.c_str(), getProfessionName(adventurer.Profession).c_str(),	adventurer		.Profession.Level,	
			getWeaponName(adventurer.Weapon).c_str(),	adventurer	.Weapon.Level,	
			getArmorName (adventurer.Armor ).c_str(),	adventurer	.Armor.Level, 
			adventurer	.CombatBonus.TurnsLeft.Hit,	
			adventurer		.CombatBonus.TurnsLeft.Attack	);
		printf("- Base points:\n");
		adventurer.Points.Print();
		printf("- Bonus points:\n");
		playerPoints.Print();
		printStatuses(adventurer);
		printf("\n-- %s is a %s level %u.\nWeapon: %s level %u.\nArmor: %s level %u.\nHit Chance Bonus Turns: %u.\nAttack Bonus Turns: %u.\n",  
			currentEnemy	.Name.c_str(), getProfessionName(currentEnemy.Profession).c_str(),	currentEnemy	.Profession.Level,	
			getWeaponName(currentEnemy.Weapon).c_str(),	currentEnemy	.Weapon.Level,	
			getArmorName (currentEnemy.Armor ).c_str(),	currentEnemy	.Armor.Level, 
			currentEnemy	.CombatBonus.TurnsLeft.Hit,	
			currentEnemy	.CombatBonus.TurnsLeft.Attack	);
		printf("- Base points:\n");
		currentEnemy.Points.Print();
		printf("- Bonus points:\n");
		enemyPoints.Print();
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

void setupEnemy(CCharacter& adventurer, CCharacter& currentEnemy, uint32_t enemyType)
{
	addItem( currentEnemy.Inventory, 1 );
	for(uint32_t i=1; i<enemyType; ++i)
		addItem( currentEnemy.Inventory, 1+(rand()%(size(itemDescriptions)-1)) );

	currentEnemy.Weapon		.Index		= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(weaponDefinitions		)));
	currentEnemy.Armor		.Index		= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(armorDefinitions		)));
	currentEnemy.Profession	.Index		= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(professionDefinitions	)));

	currentEnemy.Weapon		.Modifier	= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(weaponModifiers		)));
	currentEnemy.Armor		.Modifier	= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(armorModifiers			)));
	currentEnemy.Profession	.Modifier	= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(professionModifiers	)));

	currentEnemy.Weapon		.Level		= std::max( uint32_t(adventurer.Weapon		.Level*.8),	1U+(rand() %	std::max(1U, uint32_t(adventurer.Weapon		.Level*.11))));
	currentEnemy.Armor		.Level		= std::max( uint32_t(adventurer.Armor		.Level*.8),	1U+(rand() %	std::max(1U, uint32_t(adventurer.Armor		.Level*.11))));
	currentEnemy.Profession	.Level		= std::max( uint32_t(adventurer.Profession	.Level*.8),	1U+(rand() %	std::max(1U, uint32_t(adventurer.Profession	.Level*.11))));


	currentEnemy.Points.Shield				= getArmorShield(currentEnemy.Armor);

	SCharacterPoints finalEnemyPoints = calculateFinalPoints(currentEnemy);
	currentEnemy.Points.HP = finalEnemyPoints.MaxHP;
}


//5736	// gasty.bellino@gmail.com
void combat(CCharacter& adventurer, uint32_t enemyType)
{
	CCharacter currentEnemy = enemyDefinitions[enemyType];	// Request the enemy data.
	setupEnemy(adventurer, currentEnemy, enemyType);

	adventurer.CombatStatus.Count	= 0;	// We need to clear the combat status before starting the combat.
	adventurer.Points.Shield		= getArmorShield(adventurer.Armor);

	TURN_OUTCOME turnOutcome = TURN_OUTCOME_CONTINUE;
	while(combatContinues(turnOutcome, adventurer.Points.HP, currentEnemy.Points.HP))	// This while() executes the attack turns, requesting for user input at the beginning of each turn.
	{	
		adventurer.Score.TurnsPlayed++;
		currentEnemy.Score.TurnsPlayed++;

		if(adventurer.CombatStatus.GetStatusTurns(STATUS_TYPE_STUN))
		{
			printf("%s is stunned and loses his turn!\n", adventurer.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
			applyTurnStatusAndBonusesAndSkipTurn(adventurer);
		}
		else
			turnOutcome = playerTurn(adventurer, currentEnemy);

		if(!combatContinues(turnOutcome, adventurer.Points.HP, currentEnemy.Points.HP))
			break;

		// Execute enemy attack turn
		if(currentEnemy.CombatStatus.GetStatusTurns(STATUS_TYPE_STUN))
		{
			printf("%s is stunned and loses his turn!\n", currentEnemy.Name.c_str());
			turnOutcome = TURN_OUTCOME_CANCEL;
			applyTurnStatusAndBonusesAndSkipTurn(currentEnemy);
		}
		else
			turnOutcome = enemyTurn(currentEnemy, adventurer);
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

	int32_t targetArmorAbsorption = getArmorAbsorption(target.Armor), finalPassthroughDamage = 0, reflectedDamage = 0;
	ARMOR_EFFECT attackerArmorEffect = (ARMOR_EFFECT)getArmorEffect(thrower.Armor);
	ARMOR_EFFECT targetArmorEffect = (ARMOR_EFFECT)getArmorEffect(target.Armor);

	PROPERTY_TYPE	grenadeProperty = itemDescription.Property;
	STATUS_TYPE		grenadeStatus = getGrenadeStatusFromProperty(grenadeProperty);
	const std::string targetArmorName = getArmorName(target.Armor);
	switch(grenadeProperty)
	{
	case PROPERTY_TYPE_SMOKE:
	case PROPERTY_TYPE_STUN:
		// Apply status with fixed 50% chance
		if( lotteryResult < lotteryRange )
			applyAttackStatus(target, grenadeStatus, 50, 1+itemDescription.Grade, itemDescription.Name);
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
			finalPassthroughDamage  = applyShieldableDamage(thrower, itemEffectValueSelf, itemDescription.Name);
			reflectedDamage			= itemEffectValueSelf - finalPassthroughDamage;
			applyArmorReflect(thrower, thrower, reflectedDamage, itemDescription.Name);

			if(bAddStatus)
				applyAttackStatus(thrower, grenadeStatus, 45, (uint32_t)(1*itemDescription.Grade), itemDescription.Name);

			hitTarget = ATTACK_TARGET_SELF;
			printf("%s throws the grenade too close...\n"		
				"The grenade explodes near %s doing %u damage!\n", thrower.Name.c_str(), thrower.Name.c_str(), itemEffectValueSelf);
		}
		else if( lotteryResult == (lotteryRange-1) )
		{
			finalPassthroughDamage  = applyShieldableDamage(target,	itemEffectValue		>> 1, itemDescription.Name);
			reflectedDamage			= (itemEffectValue>>1) - finalPassthroughDamage;
			applyArmorReflect(thrower, target, reflectedDamage, itemDescription.Name);

			finalPassthroughDamage  = applyShieldableDamage(thrower,	itemEffectValueSelf	>> 1, itemDescription.Name);
			reflectedDamage			= (itemEffectValueSelf>>1) - finalPassthroughDamage;
			applyArmorReflect(thrower, thrower, reflectedDamage, itemDescription.Name);

			if(bAddStatus)
			{
				applyAttackStatus(target,	grenadeStatus, 45, (uint32_t)(2*itemDescription.Grade), itemDescription.Name);
				applyAttackStatus(thrower,	grenadeStatus, 45, (uint32_t)(1*itemDescription.Grade), itemDescription.Name);
			}

			hitTarget = (ATTACK_TARGET)(ATTACK_TARGET_SELF | ATTACK_TARGET_OTHER);
			printf("%s doesn't throw the grenade far enough so %s receives %u damage but also %s receives %u damage.\n", thrower.Name.c_str(), target.Name.c_str(), itemEffectValue, thrower.Name.c_str(), itemEffectValueSelf);
		}
		else if( lotteryResult < lotteryRange )
		{
			finalPassthroughDamage  = applyShieldableDamage(target, itemEffectValue, itemDescription.Name);
			reflectedDamage			= itemEffectValue - finalPassthroughDamage;
			applyArmorReflect(thrower, target, reflectedDamage, itemDescription.Name);
			if(bAddStatus)
				applyAttackStatus(target, grenadeStatus, 50, (uint32_t)(3.6f*itemDescription.Grade), itemDescription.Name);

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

	SMenuItem<uint32_t> itemOptions[MAX_INVENTORY_SLOTS+1];
	char itemOption[128] = {};
	for(uint32_t i=0; i<user.Inventory.ItemCount; ++i)
	{
		sprintf_s(itemOption, "- x%.2u %s", user.Inventory.Slots[i].ItemCount, itemDescriptions[user.Inventory.Slots[i].ItemIndex].Name.c_str());
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