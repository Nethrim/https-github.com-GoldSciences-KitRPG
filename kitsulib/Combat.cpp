#include "Combat.h"
#include "Armor.h"
#include "Weapon.h"
#include "Profession.h"
#include "Item.h"

int32_t klib::applyShieldableDamage(CCharacter& target, int32_t damageDealt, const std::string& sourceName) {
	return klib::applyShieldableDamage(target, damageDealt, getArmorAbsorption(target.Armor), sourceName);
}

// Returns final passthrough damage (not blocked by shield) to use by vampire and reflect effects.
int32_t klib::applyShieldableDamage(CCharacter& target, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName)
{
	if(0 >= target.Points.CurrentLife.HP)	// This character is already dead
		return 0;

	const std::string	targetArmorName		= getArmorName	(target.Armor);
	SCharacterPoints	targetFinalPoints	= calculateFinalPoints(target);
	const int32_t		targetArmorShield	= targetFinalPoints.MaxLife.Shield;
	
	// Impenetrable armors always have 60% extra absorption rate.
	if(targetFinalPoints.DefendEffect & DEFEND_EFFECT_IMPENETRABLE)
	{
		if(target.Points.CurrentLife.Shield)
		{
			absorptionRate += 60;
			printf("%s damage absorption rate for %s is raised to %%%u because of the impenetrable property.\n", targetArmorName.c_str(), sourceName.c_str(), absorptionRate);
		}
	}
	else
	{	
		// If the armor is not impenetrable, the absorption rate is affected by the shield damage.
		printf("%s damage absorption rate for %s is %%%u.\n", targetArmorName.c_str(), sourceName.c_str(), absorptionRate);
		if(target.Points.CurrentLife.Shield) 
			absorptionRate = absorptionRate ? std::max((int32_t)(absorptionRate*(target.Points.CurrentLife.Shield/(double)targetArmorShield)), 1) : 0;

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
		int remainingShield = target.Points.CurrentLife.Shield-shieldedDamage;
		if(target.Points.CurrentLife.Shield)
		{
			printf("%s's shield absorbs %u damage from %s.\n", target.Name.c_str(), std::min(target.Points.CurrentLife.Shield, shieldedDamage), sourceName.c_str());
			target.Points.CurrentLife.Shield = std::max(0, remainingShield);
			if(remainingShield < 0)
				printf("%s's shield ran out allowing some damage from %s to pass through.\n", target.Name.c_str(), sourceName.c_str());
			else
				printf("%s's remaining shield is now %u.\n", target.Name.c_str(), target.Points.CurrentLife.Shield);
		}
		if(remainingShield < 0)	// only apply damage to health if the shield didn't absorb all the damage.
		{
			finalPassthroughDamage	= remainingShield*-1;
			printf("%s's wasn't protected against %u shieldable damage from %s.\n", target.Name.c_str(), finalPassthroughDamage, sourceName.c_str());
			target.Points.CurrentLife.HP += remainingShield;
		}
	}

	if(passthroughDamage)
		printf("%s receives %u passthrough damage from %s.\n", target.Name.c_str(), passthroughDamage, sourceName.c_str());
	finalPassthroughDamage	+= passthroughDamage;

	target.Points.CurrentLife.HP -= passthroughDamage;

	return finalPassthroughDamage;
}

STATUS_TYPE klib::applyAttackStatus(CCharacter& target, STATUS_TYPE weaponStatus, int32_t absorbChance, int32_t turnCount, const std::string& sourceName)
{
	if(weaponStatus == STATUS_TYPE_NONE)
		return STATUS_TYPE_NONE;

	const std::string	targetArmorName			= getArmorName(target.Armor);

	STATUS_TYPE appliedStatus = STATUS_TYPE_NONE;

	SCharacterPoints targetFinalPoints = calculateFinalPoints(target);
	if((targetFinalPoints.DefendEffect & DEFEND_EFFECT_IMPENETRABLE) && target.Points.CurrentLife.Shield)
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

STATUS_TYPE klib::applyAttackStatus(CCharacter& target, STATUS_TYPE weaponStatus, int32_t turnCount, const std::string& sourceName)
{
	if(weaponStatus == STATUS_TYPE_NONE)
		return STATUS_TYPE_NONE;

	const int32_t		targetArmorAbsorption	= getArmorAbsorption(target.Armor);
	const std::string	targetArmorName			= getArmorName(target.Armor);
	SCharacterPoints	targetFinalPoints		= calculateFinalPoints(target);
	const int32_t		targetArmorShield		= targetFinalPoints.MaxLife.Shield;


	STATUS_TYPE appliedStatus = STATUS_TYPE_NONE;
	
	int32_t absorbChance;

	double absorptionRatio = std::max(0.0, (target.Points.CurrentLife.Shield/(double)targetArmorShield))/2.0;
	absorbChance = 50+(int32_t)(absorptionRatio*targetArmorAbsorption);
	absorbChance = std::min(absorbChance, 100);

	printf("%s status absorb chance after absorption calculation is %%%u.\n", target.Name.c_str(), absorbChance);
	return applyAttackStatus(target, weaponStatus, absorbChance, turnCount, sourceName);
}

int32_t klib::applyArmorReflect(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName) {
	
	SCharacterPoints targetFinalPoints = calculateFinalPoints(targetReflecting);
	if( 0 == damageDealt || 0 == (targetFinalPoints.DefendEffect & DEFEND_EFFECT_REFLECT))
		return 0;

	const std::string	targetArmorName			= getArmorName	(targetReflecting.Armor);
	const std::string	attackerArmorName		= getArmorName	(attacker.Armor);
	if(damageDealt > 0)
		printf("%s reflects %u damage from %s with %s.\n", targetReflecting.Name.c_str(), damageDealt, sourceName.c_str(), targetArmorName.c_str());
	else if(damageDealt < 0)
		printf("%s reflects %u health from %s with %s.\n", targetReflecting.Name.c_str(), damageDealt, sourceName.c_str(), targetArmorName.c_str());
	int32_t finalPassthroughDamage = klib::applyShieldableDamage(attacker, damageDealt, targetArmorName);
	int reflectedDamage = damageDealt-finalPassthroughDamage;
	if(reflectedDamage)
	{
		SCharacterPoints attackerFinalPoints = calculateFinalPoints(attacker);
		DEFEND_EFFECT attackerArmorEffect = attackerFinalPoints.DefendEffect;
		if(attackerArmorEffect & DEFEND_EFFECT_REFLECT)
		{
			printf("\n%s causes a recursive reflection with %s.\n", attackerArmorName.c_str(), targetArmorName.c_str());
			applyArmorReflect(targetReflecting, attacker, reflectedDamage, attackerArmorName);
		}
	}

	return finalPassthroughDamage;
}


// This function returns the damage dealt to the target
int klib::attack(CCharacter& attacker, CCharacter& target)
{
	// Calculate success from the hit chance and apply damage to target or just print the miss message.
	int32_t damageDealt = 0;

	const std::string	attackerWeaponName	= getWeaponName(attacker.Weapon);
	SCharacterPoints	attackerPoints		= calculateFinalPoints(attacker);

	bool bIsBlind = attacker.CombatStatus.GetStatusTurns(STATUS_TYPE_BLIND) > 0;

	int finalChance = attackerPoints.Attack.Hit;
	if(bIsBlind)
		printf("Blindness causes %s to have %u hit chance for this turn.\n", attacker.Name.c_str(), attackerPoints.Attack.Hit >>= 1);

	if(target.CombatStatus.GetStatusTurns(STATUS_TYPE_STUN))
	{
		printf("As %s is stunned, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Attack.Hit);
		finalChance	+= attackerPoints.Attack.Hit;
	}
	else if(target.CombatStatus.GetStatusTurns(STATUS_TYPE_BLIND))
	{
		printf("As %s is blind, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Attack.Hit/2);
		finalChance	+= attackerPoints.Attack.Hit/2;
	}

	if ((rand() % 100) < finalChance )
	{

		damageDealt = attackerPoints.Attack.Damage+(rand()%((attackerPoints.Attack.Damage)/10+1));
		printf("%s hits %s for: %u.\n", attacker.Name.c_str(), target.Name.c_str(), damageDealt);

		int finalPassthroughDamage = klib::applyShieldableDamage(target, damageDealt, attacker.Name);

		// Apply combat bonuses from weapon for successful hits.
		const SCharacterPoints attackerWeaponPoints = getWeaponPoints(attacker.Weapon);
		applyCombatBonus(attacker, attackerWeaponPoints, attackerWeaponName);

		// Apply weapon effects for successful hits.
		ATTACK_EFFECT attackerWeaponEffect = attackerPoints.AttackEffect;
		if(attackerWeaponEffect & ATTACK_EFFECT_LEECH)
		{
			int actualHPGained = std::min(finalPassthroughDamage, attackerPoints.MaxLife.HP-attacker.Points.CurrentLife.HP);
			if(actualHPGained > 0)
				printf("%s drains %u HP from %s with %s.\n", attacker.Name.c_str(), actualHPGained, target.Name.c_str(), attackerWeaponName.c_str());
			else if(actualHPGained < 0)
				printf("%s loses %u HP from %s with %s.\n", attacker.Name.c_str(), actualHPGained, target.Name.c_str(), attackerWeaponName.c_str());
			attacker.Points.CurrentLife.HP		+= actualHPGained;
		}

		if(attackerWeaponEffect & ATTACK_EFFECT_STEAL)
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
		klib::applyArmorReflect(attacker, target, shieldedDamage, attackerWeaponName);

		// Apply attack statuses.
		int turns = 1;
		klib::applyAttackStatus(target, attackerPoints.StatusInflict, turns, attackerWeaponName);
	}
	else 
		printf("%s misses the attack!\n", attacker.Name.c_str());

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

	return damageDealt;
};

void klib::applyCombatBonus(CCharacter& character, const SCharacterPoints& combatBonus, const std::string& sourceName)
{
	if(0 >= character.Points.CurrentLife.HP)	// This character is already dead
		return;

	if(combatBonus.CurrentLife.HP)
	{
		if(combatBonus.CurrentLife.HP > 0 && character.Points.CurrentLife.HP < calculateFinalPoints(character).MaxLife.HP)
		{
			int finalHPAdded = std::min(combatBonus.CurrentLife.HP, calculateFinalPoints(character).MaxLife.HP-character.Points.CurrentLife.HP);
			printf("%s gains %u HP from %s.\n", character.Name.c_str(), finalHPAdded, sourceName.c_str());
			character.Points.CurrentLife.HP		+= finalHPAdded;
		}
		else if(combatBonus.CurrentLife.HP < 0 && character.Points.CurrentLife.HP)
		{
			int finalHPRemoved = std::min(-combatBonus.CurrentLife.HP, character.Points.CurrentLife.HP);
			printf("%s loses %u HP from %s.\n", character.Name.c_str(), finalHPRemoved, sourceName.c_str());
			character.Points.CurrentLife.HP			-= finalHPRemoved;
		}
	}

	if(combatBonus.CurrentLife.Shield)
	{
		if(combatBonus.CurrentLife.Shield > 0 && character.Points.CurrentLife.Shield < calculateFinalPoints(character).MaxLife.Shield)
		{
			int finalShieldAdded = std::min(combatBonus.CurrentLife.Shield, calculateFinalPoints(character).MaxLife.Shield-character.Points.CurrentLife.Shield);
			printf("%s gains %u Shield from %s.\n", character.Name.c_str(), finalShieldAdded, sourceName.c_str());
			character.Points.CurrentLife.Shield		+= finalShieldAdded;
		}
		else if(combatBonus.CurrentLife.Shield < 0 && character.Points.CurrentLife.Shield)
		{
			int finalShieldRemoved = std::min(-combatBonus.CurrentLife.Shield, character.Points.CurrentLife.Shield);
			printf("%s loses %u Shield from %s.\n", character.Name.c_str(), finalShieldRemoved, sourceName.c_str());
			character.Points.CurrentLife.Shield			-= finalShieldRemoved;
		}
	}

	if(combatBonus.CurrentLife.Mana)
	{
		if(combatBonus.CurrentLife.Mana > 0 && character.Points.CurrentLife.Mana < calculateFinalPoints(character).MaxLife.Mana)
		{
			int finalManaAdded = std::min(combatBonus.CurrentLife.Mana, calculateFinalPoints(character).MaxLife.Mana-character.Points.CurrentLife.Mana);
			printf("%s gains %u Mana from %s.\n", character.Name.c_str(), finalManaAdded, sourceName.c_str());
			character.Points.CurrentLife.Mana		+= finalManaAdded;
		}
		else if(combatBonus.CurrentLife.Mana < 0 && character.Points.CurrentLife.Mana)
		{
			int finalManaRemoved = std::min(-combatBonus.CurrentLife.Mana, character.Points.CurrentLife.Mana);
			printf("%s loses %u Mana from %s.\n", character.Name.c_str(), finalManaRemoved, sourceName.c_str());
			character.Points.CurrentLife.Mana			-= finalManaRemoved;
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

void klib::applyArmorEffect(CCharacter& character)
{
	if(0 >= character.Points.CurrentLife.HP)	// This character is already dead
		return;

	const std::string armorName = getArmorName(character.Armor);

	SCharacterPoints	targetFinalPoints	= calculateFinalPoints(character);
	int32_t				totalArmorShield	= getArmorPoints(character.Armor).MaxLife.Shield;

	if((targetFinalPoints.PassiveEffect & PASSIVE_EFFECT_RECHARGE) && character.Points.CurrentLife.Shield < totalArmorShield) {
		int32_t shieldToAdd		= totalArmorShield/20;
		shieldToAdd				= std::max(1, std::min(shieldToAdd, totalArmorShield-character.Points.CurrentLife.Shield));
		character.Points.CurrentLife.Shield	+= shieldToAdd;
		printf("%s recharges by %u.\n", armorName.c_str(), shieldToAdd);
	};
};


void klib::applyTurnStatus(CCharacter& character)
{
	if(0 >= character.Points.CurrentLife.HP)	// This character is already dead
		return;

	int amount=0;
	const SCharacterPoints finalPoints = calculateFinalPoints(character);
	for(uint32_t i=0; i<character.CombatStatus.Count; ++i)
	{
		switch(character.CombatStatus.Status[i])
		{
		case STATUS_TYPE_BLEEDING:	amount = std::max(1, finalPoints.MaxLife.HP/20); if( amount > 0 ) character.Score.DamageTaken += amount; klib::applyShieldableDamage(character, amount, 0, "bleeding");	break;
		case STATUS_TYPE_POISON:	amount = std::max(1, finalPoints.MaxLife.HP/20); if( amount > 0 ) character.Score.DamageTaken += amount; klib::applyShieldableDamage(character, amount, 0, "poisoning");	break;
		case STATUS_TYPE_BURN:		amount = std::max(1, finalPoints.MaxLife.HP/20); if( amount > 0 ) character.Score.DamageTaken += amount; klib::applyShieldableDamage(character, amount, getArmorAbsorption(character.Armor), "burning");	break;
		//case STATUS_TYPE_STUN:		break;
		//case STATUS_TYPE_BLIND:		break;
		}
	}
};

void klib::applyTurnStatusAndBonusesAndSkipTurn(CCharacter& character)
{
	if(0 >= character.Points.CurrentLife.HP)	// This character is already dead
		return;

	printf("\n");
	applyTurnStatus(character);
	klib::applyCombatBonus(character, character.CombatBonus.Points, "Turn Combat Bonus");
	klib::applyCombatBonus(character, getProfessionPoints(character.Profession), getProfessionName(character.Profession).c_str());
	klib::applyCombatBonus(character, getArmorPoints(character.Armor), getArmorName(character.Armor));
	klib::applyArmorEffect(character);
	character.CombatBonus.NextTurn();
	character.CombatStatus.NextTurn();
	printf("\n");
}

void klib::executeItem(uint32_t indexInventory, CCharacter& user, CCharacter& target) {

	const SItem& item = user.Inventory.Slots[indexInventory].Item;
	std::string itemName = getItemName(item);

	printf("\n%s uses: %s.\n\n", user.Name.c_str(), itemName.c_str());
	switch( itemDefinitions[item.Index].Type )
	{
	case ITEM_TYPE_POTION:
		klib::usePotion(item, user);
		break;

	case ITEM_TYPE_GRENADE:
		klib::useGrenade(item, user, target);
		break;

	default:
		printf("This item type does nothing yet... But we still remove it from your inventory!\n");
	}

	klib::removeItem(user.Inventory, indexInventory, user.Name);
}
