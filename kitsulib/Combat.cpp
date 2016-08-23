#include "Combat.h"
#include "Armor.h"
#include "Weapon.h"
#include "Profession.h"
#include "Item.h"

using namespace klib;

SLifePoints klib::applyShieldableDamage(CCharacter& target, int32_t damageDealt, const std::string& sourceName) {
	return applyShieldableDamage(target, damageDealt, getArmorAbsorption(target.CurrentArmor), sourceName);
}

// Returns final passthrough damage (not blocked by shield) to use by vampire and reflect effects.
SLifePoints klib::applyShieldableDamage(CCharacter& target, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName)
{
	if(0 >= target.Points.CurrentLife.Health)	// This character is already dead
		return {};

	const std::string	targetArmorName		= getArmorName	(target.CurrentArmor);
	SCharacterPoints	targetFinalPoints	= calculateFinalPoints(target);
	const int32_t		targetArmorShield	= targetFinalPoints.MaxLife.Shield;
	
	// Impenetrable armors always have 60% extra absorption rate.
	if(targetFinalPoints.Effect.Defend & DEFEND_EFFECT_IMPENETRABLE)
	{
		if(target.Points.CurrentLife.Shield) {
			absorptionRate += 60;
			printf("%s damage absorption rate for %s is raised to %%%u because of the impenetrable property.\n", targetArmorName.c_str(), sourceName.c_str(), absorptionRate);
		}
	}
	else
	{	
		// If the armor is not impenetrable, the absorption rate is affected by the shield damage.
		printf("%s damage absorption rate for %s is %%%u.\n", targetArmorName.c_str(), sourceName.c_str(), absorptionRate);
		if(target.Points.CurrentLife.Shield) 
			absorptionRate = absorptionRate ? std::max((int32_t)(absorptionRate*(target.Points.CurrentLife.Shield/(double)std::max(targetArmorShield, 1))), 1) : 0;
		else
			absorptionRate = 0;

		printf("%s final damage absorption rate taking deterioration into account is %%%u.\n", targetArmorName.c_str(), absorptionRate);
	}

	absorptionRate = std::min(absorptionRate, 100);
	const double	absorptionFraction	= 0.01*absorptionRate;
	int shieldedDamage		= (int)(damageDealt * absorptionFraction);
	int passthroughDamage	= (int)(damageDealt * std::max(0.0, 1.0-absorptionFraction));
	int totalDamage			= shieldedDamage+passthroughDamage;
	
	printf("Shielded damage: %u. Passthrough damage: %u. Expected sum: %u. Actual sum: %u. Absorption ratio: %%%u.\n", shieldedDamage, passthroughDamage, damageDealt, totalDamage, absorptionRate);
	if( totalDamage < damageDealt )
	{
		int errorDamage = damageDealt-totalDamage;
		if(0 == absorptionRate || 0 == target.Points.CurrentLife.Shield)
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
			target.Points.CurrentLife.Health += remainingShield;
		}
	}

	if(passthroughDamage)
		printf("%s receives %u passthrough damage from %s.\n", target.Name.c_str(), passthroughDamage, sourceName.c_str());
	finalPassthroughDamage	+= passthroughDamage;

	target.Points.CurrentLife.Health -= passthroughDamage;

	return { finalPassthroughDamage, damageDealt-finalPassthroughDamage };
}


COMBAT_STATUS klib::applyAttackStatus(CCharacter& target, COMBAT_STATUS weaponStatus, int32_t absorbChance, int32_t turnCount, const std::string& sourceName)
{
	if(COMBAT_STATUS_NONE == weaponStatus || 0 >= target.Points.CurrentLife.Health)
		return COMBAT_STATUS_NONE;

	const std::string	targetArmorName			= getArmorName(target.CurrentArmor);

	COMBAT_STATUS appliedStatus = COMBAT_STATUS_NONE;

	SCharacterPoints targetFinalPoints = calculateFinalPoints(target);
	if((targetFinalPoints.Effect.Defend & DEFEND_EFFECT_IMPENETRABLE) && target.Points.CurrentLife.Shield)
	{
		absorbChance = absorbChance ? std::max(60, absorbChance*2) : 60;
		printf("%s absorb chance of status by %s is modified to %%%u because of the impenetrable property of %s.\n", target.Name.c_str(), sourceName.c_str(), absorbChance, targetArmorName.c_str());
	}
	
	for(int i=0; i<MAX_COMBAT_STATUS_COUNT; i++)
	{
		COMBAT_STATUS bitStatus =  (COMBAT_STATUS)(1<<i);
		if(0 == (bitStatus & weaponStatus) )
			continue;

		std::string text = getStringFromBit(bitStatus);

		if(bitStatus & targetFinalPoints.Status.Immunity)
		{
			printf("%s is immune to %s!\n", target.Name.c_str(), text.c_str());
			continue;
		}

		if((rand()%100) < absorbChance)
		{
			printf("%s absorbs \"%s\" inflicted by %s with %%%u absorb chance.\n", targetArmorName.c_str(), text.c_str(), sourceName.c_str(), absorbChance);
			continue;
		}
		
		addStatus(target.CombatStatus, bitStatus, turnCount);
		appliedStatus = (COMBAT_STATUS)(appliedStatus|bitStatus);

		printf("%s got inflicted \"%s\" status from %s that will last for the next %u turns.\n", target.Name.c_str(), text.c_str(), sourceName.c_str(), turnCount);
	}
	return appliedStatus;
}

COMBAT_STATUS klib::applyAttackStatus(CCharacter& target, COMBAT_STATUS weaponStatus, int32_t turnCount, const std::string& sourceName)
{
	if(COMBAT_STATUS_NONE == weaponStatus || 0 >= target.Points.CurrentLife.Health)
		return COMBAT_STATUS_NONE;

	const int32_t		targetArmorAbsorption	= getArmorAbsorption(target.CurrentArmor);
	const std::string	targetArmorName			= getArmorName(target.CurrentArmor);
	SCharacterPoints	targetFinalPoints		= calculateFinalPoints(target);
	const int32_t		targetArmorShield		= targetFinalPoints.MaxLife.Shield;

	COMBAT_STATUS appliedStatus = COMBAT_STATUS_NONE;
	
	int32_t absorbChance;

	double absorptionRatio = std::max(0.0, (target.Points.CurrentLife.Shield/(double)targetArmorShield))/2.0;
	absorbChance = 50+(int32_t)(absorptionRatio*targetArmorAbsorption);
	absorbChance = std::min(absorbChance, 100);

	printf("%s status absorb chance after absorption calculation is %%%u.\n", target.Name.c_str(), absorbChance);
	return applyAttackStatus(target, weaponStatus, absorbChance, turnCount, sourceName);
}

int32_t klib::applyArmorReflect(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName) 
{
	SCharacterPoints targetFinalPoints = calculateFinalPoints(targetReflecting);
		if( 0 == damageDealt 
		||	0 == (targetFinalPoints.Effect.Defend & DEFEND_EFFECT_REFLECT) 
		||	0 >= targetReflecting.Points.CurrentLife.Shield 
		||	0 >= attacker.Points.CurrentLife.Health
		||	0 >= targetReflecting.Points.CurrentLife.Health
		)
		return 0;

	printf("\n");
	const std::string	targetArmorName			= getArmorName	(targetReflecting.CurrentArmor);

	if(damageDealt > 0)
		printf("%s reflects %u damage from %s with %s.\n", targetReflecting.Name.c_str(), damageDealt, sourceName.c_str(), targetArmorName.c_str());
	else if(damageDealt < 0)
		printf("%s reflects %u health from %s with %s.\n", targetReflecting.Name.c_str(), damageDealt, sourceName.c_str(), targetArmorName.c_str());

	SLifePoints finalDamage	= applyShieldableDamage(attacker, damageDealt, targetArmorName);
	// If the attacker was killed by the reflect we need to avoid reflecting from her armor.
	if(finalDamage.Shield && 0 < attacker.Points.CurrentLife.Health)
	{
		SCharacterPoints	attackerFinalPoints = calculateFinalPoints(attacker);
		DEFEND_EFFECT		attackerArmorEffect = attackerFinalPoints.Effect.Defend;
		if((attackerArmorEffect & DEFEND_EFFECT_REFLECT) && attacker.Points.CurrentLife.Shield)
		{
			const std::string	attackerArmorName		= getArmorName	(attacker.CurrentArmor);
			printf("%s causes a recursive reflection with %s dealing %u damage.\n", attackerArmorName.c_str(), targetArmorName.c_str(), damageDealt);
			applyArmorReflect(targetReflecting, attacker, finalDamage.Shield, targetArmorName);
		}
	}

	return finalDamage.Health;
}

void applyWeaponSteal(ATTACK_EFFECT testEffectBit, ATTACK_EFFECT attackerWeaponEffect, int32_t finalPassthroughDamage, int32_t maxPoints, int32_t& currentPoints, const std::string& attackerName, const std::string& targetName, const std::string& attackerWeaponName, const std::string& pointName, const std::string& gainVerb, const std::string& loseVerb )
{
	if(attackerWeaponEffect & testEffectBit)
	{
		int32_t actualHPGained = std::min((int32_t)finalPassthroughDamage, maxPoints-std::max(0, currentPoints));
		if(actualHPGained > 0)
			printf("%s %s %u %s from %s with %s.\n", attackerName.c_str(), gainVerb.c_str(), actualHPGained, pointName.c_str(), targetName.c_str(), attackerWeaponName.c_str());
		else if(actualHPGained < 0)
			printf("%s %s %u %s to %s with %s.\n", attackerName.c_str(), loseVerb.c_str(),   actualHPGained, pointName.c_str(), targetName.c_str(), attackerWeaponName.c_str());
		currentPoints		+= actualHPGained;
	}
}

SLifePoints klib::applySuccessfulHit(CCharacter& attacker, CCharacter& target, int32_t damage, bool bAddStatus, COMBAT_STATUS grenadeStatus, int32_t statusTurns, const std::string& sourceName) {
	return applySuccessfulHit(attacker, target, damage, getArmorAbsorption(target.CurrentArmor), bAddStatus, grenadeStatus, statusTurns, sourceName);
}

SLifePoints klib::applySuccessfulHit(CCharacter& attacker, CCharacter& target, int32_t damage, int32_t absorptionRate, bool bAddStatus, COMBAT_STATUS grenadeStatus, int32_t statusTurns, const std::string& sourceName)
{
	SLifePoints finalDamage = klib::applyShieldableDamage(target, damage, sourceName);
	klib::applyArmorReflect(attacker, target, finalDamage.Shield, sourceName);
	
	// Clear sleep on successful hit.
	if(finalDamage.Health || finalDamage.Shield || finalDamage.Mana) {
		for(uint32_t i=0; i < target.CombatStatus.Count; ++i)
			if( target.CombatStatus.Status[i] == COMBAT_STATUS_SLEEP )
			{
				printf("%s awakes from his induced nap!\n", target.Name.c_str());
				target.CombatStatus.TurnsLeft[i] = 0;
			}
	}

	if(bAddStatus)
		applyAttackStatus(target, grenadeStatus, statusTurns, sourceName);

	return finalDamage;
}

void klib::applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName) {
	applySuccessfulWeaponHit(attacker, targetReflecting, damageDealt, getArmorAbsorption(targetReflecting.CurrentArmor), sourceName);
}

void klib::applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName) 
{
	if(calculateFinalPoints(targetReflecting).Effect.Defend & DEFEND_EFFECT_BLIND)
		applyAttackStatus(targetReflecting, COMBAT_STATUS_BLIND, 1, getArmorName(targetReflecting.CurrentArmor));

	if( 0 == damageDealt )
		return;

	SCharacterPoints	attackerPoints	= calculateFinalPoints(attacker);
	SLifePoints			finalDamage 	= applySuccessfulHit(attacker, targetReflecting, damageDealt, absorptionRate, attackerPoints.Status.Inflict != COMBAT_STATUS_NONE, attackerPoints.Status.Inflict, 1, sourceName);

	// Apply combat bonuses from weapon for successful hits.
	const SCharacterPoints attackerWeaponPoints = getWeaponPoints(attacker.CurrentWeapon);
	applyCombatBonus(attacker, attackerWeaponPoints, sourceName);
	attackerPoints			= calculateFinalPoints(attacker);
	printf("\n");
	// Apply weapon effects for successful hits.
	ATTACK_EFFECT		attackerWeaponEffect	= attackerPoints.Effect.Attack;
	applyWeaponSteal(ATTACK_EFFECT_LEECH_HEALTH	, attackerWeaponEffect, finalDamage.Health	, attackerPoints.MaxLife.Health	, attacker.Points.CurrentLife.Health	, attacker.Name	, targetReflecting.Name, sourceName, "Health", "drains", "loses" );
	applyWeaponSteal(ATTACK_EFFECT_LEECH_MANA	, attackerWeaponEffect, finalDamage.Mana	, attackerPoints.MaxLife.Mana	, attacker.Points.CurrentLife.Mana		, attacker.Name	, targetReflecting.Name, sourceName, "Mana", "drains", "loses" );
	applyWeaponSteal(ATTACK_EFFECT_LEECH_SHIELD	, attackerWeaponEffect, finalDamage.Shield	, attackerPoints.MaxLife.Shield	, attacker.Points.CurrentLife.Shield	, attacker.Name	, targetReflecting.Name, sourceName, "Shield", "steals", "gives" );
	applyWeaponSteal(ATTACK_EFFECT_STEAL		, attackerWeaponEffect, finalDamage.Health	, 0x7FFFFFFF					, attacker.Points.Coins					, attacker.Name	, targetReflecting.Name	, sourceName, "Coins", "steals", "drops" );
}

// This function returns the damage dealt to the target
bool klib::attack(CCharacter& attacker, CCharacter& target)
{
	// Calculate success from the hit chance and apply damage to target or just print the miss message.
	int32_t damageDealt = 0;

	const std::string	attackerWeaponName	= getWeaponName(attacker.CurrentWeapon);
	SCharacterPoints	attackerPoints		= calculateFinalPoints(attacker);

	bool bIsBlind = attacker.CombatStatus.GetStatusTurns(COMBAT_STATUS_BLIND) > 0;

	if(attacker.CombatStatus.GetStatusTurns(COMBAT_STATUS_BLACKOUT) && (getWeaponPoints(attacker.CurrentWeapon).Tech & EQUIP_TECHNOLOGY_DIGITAL)) {
		printf("This weapon was disabled by an electromagnetic pulse.\n");
		return false;
	}

	int finalChance = attackerPoints.Attack.Hit;
	if(bIsBlind)
		printf("Blindness causes %s to have %u hit chance for this turn.\n", attacker.Name.c_str(), attackerPoints.Attack.Hit >>= 1);

	if(target.CombatStatus.GetStatusTurns(COMBAT_STATUS_STUN)) {
		printf("As %s is stunned, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Attack.Hit);
		finalChance	+= attackerPoints.Attack.Hit;
	}
	else if(target.CombatStatus.GetStatusTurns(COMBAT_STATUS_SLEEP)) {
		printf("As %s is asleep, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Attack.Hit);
		finalChance	+= attackerPoints.Attack.Hit;
	}
	else if(target.CombatStatus.GetStatusTurns(COMBAT_STATUS_BLIND)) {
		printf("As %s is blind, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Attack.Hit/2);
		finalChance	+= attackerPoints.Attack.Hit/2;
	}

	if ((rand() % 100) < finalChance ) {
		damageDealt = attackerPoints.Attack.Damage+(rand()%((attackerPoints.Attack.Damage)/10+1));
		printf("%s hits %s for: %u.\n", attacker.Name.c_str(), target.Name.c_str(), damageDealt);
		applySuccessfulWeaponHit(attacker, target, damageDealt, attackerWeaponName);
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

	return true;
};

void applyTurnBonus(int32_t& characterCurrentPoint, const int32_t characterMaxPoint, const int32_t combatBonus, const std::string& characterName, const std::string& pointName, const std::string& sourceName)
{
	if(combatBonus > 0 && characterCurrentPoint < characterMaxPoint) {
		printf("%s gains %u %s from %s.\n", characterName.c_str(), combatBonus, pointName.c_str(), sourceName.c_str());
		characterCurrentPoint	+= combatBonus;
	}
	else if(combatBonus < 0 && characterCurrentPoint) {
		printf("%s loses %u %s from %s.\n", characterName.c_str(), combatBonus, pointName.c_str(), sourceName.c_str());
		characterCurrentPoint	-= combatBonus;
	}
}

void klib::applyCombatBonus(CCharacter& character, const SCharacterPoints& combatBonus, const std::string& sourceName)
{
	if(0 >= character.Points.CurrentLife.Health)	// This character is already dead
		return;

	SCharacterPoints characterPoints = calculateFinalPoints(character);

	int finalHPAdded = std::min(combatBonus.CurrentLife.Health, characterPoints.MaxLife.Health-character.Points.CurrentLife.Health);
	applyTurnBonus(character.Points.CurrentLife.Health, characterPoints.MaxLife.Health, finalHPAdded, character.Name, "Health", sourceName);
	
	finalHPAdded = std::min(combatBonus.CurrentLife.Mana, characterPoints.MaxLife.Mana-character.Points.CurrentLife.Mana);
	applyTurnBonus(character.Points.CurrentLife.Mana, characterPoints.MaxLife.Mana, finalHPAdded, character.Name, "Mana", sourceName);
	
	finalHPAdded = std::min(combatBonus.CurrentLife.Shield, characterPoints.MaxLife.Shield-character.Points.CurrentLife.Shield);
	applyTurnBonus(character.Points.CurrentLife.Shield, characterPoints.MaxLife.Shield, finalHPAdded, character.Name, "Shield", sourceName);

	if(combatBonus.Coins > 0)
		printf("%s gains %u Coins from %s.\n", character.Name.c_str(), combatBonus.Coins, sourceName.c_str());
	else if(combatBonus.Coins < 0 && character.Points.Coins)
		printf("%s loses %u Coins from %s.\n", character.Name.c_str(), combatBonus.Coins*-1, sourceName.c_str());
	character.Points.Coins	+= combatBonus.Coins;
};

void applyRegenBonus(PASSIVE_EFFECT testEffectBit, PASSIVE_EFFECT characterActiveEffects, int32_t maxPoints, int32_t& characterCurrentPoints, const std::string& pointName, const std::string& armorName)
{
	if((testEffectBit & characterActiveEffects) && (characterCurrentPoints < maxPoints)) {
		int32_t pointsToAdd		= maxPoints/20;
		pointsToAdd				= std::max(1, std::min(pointsToAdd, maxPoints-characterCurrentPoints));
		characterCurrentPoints	+= pointsToAdd;
		printf("%s regenerates %s by %i.\n", armorName.c_str(), pointName.c_str(), pointsToAdd);
	}
}


void applyPassive(CCharacter& character, PASSIVE_EFFECT equipmentEffects, const std::string& sourceName)
{
	SCharacterPoints	armorFinalPoints		= klib::getArmorPoints(character.CurrentArmor);
	SCharacterPoints	characterFinalPoints	= calculateFinalPoints(character);

	applyRegenBonus(PASSIVE_EFFECT_LIFE_REGEN		,	equipmentEffects,	characterFinalPoints.MaxLife.Health	,	character.Points.CurrentLife.Health	, "Health"	, sourceName);
	applyRegenBonus(PASSIVE_EFFECT_MANA_REGEN		,	equipmentEffects,	characterFinalPoints.MaxLife.Mana	,	character.Points.CurrentLife.Mana	, "Mana"	, sourceName);
	applyRegenBonus(PASSIVE_EFFECT_SHIELD_REPAIR	,	equipmentEffects,	characterFinalPoints.MaxLife.Shield	,	character.Points.CurrentLife.Shield	, "Shield"	, sourceName);
};

void klib::applyTurnStatus(CCharacter& character)
{
	if(0 >= character.Points.CurrentLife.Health)	// This character is already dead
		return;

	int amount=0;
	const SCharacterPoints finalPoints = calculateFinalPoints(character);
	for(uint32_t i=0; i<character.CombatStatus.Count; ++i)
	{
		switch(character.CombatStatus.Status[i])
		{
		case COMBAT_STATUS_BLEEDING	:	amount = std::max(1, finalPoints.MaxLife.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, 0, "bleeding");	break;
		case COMBAT_STATUS_POISON	:	amount = std::max(1, finalPoints.MaxLife.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, 0, "poisoning");	break;
		case COMBAT_STATUS_BURN		:	amount = std::max(1, finalPoints.MaxLife.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, getArmorAbsorption(character.CurrentArmor), "burning");	break;
		case COMBAT_STATUS_FREEZING	:	amount = std::max(1, finalPoints.MaxLife.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, getArmorAbsorption(character.CurrentArmor), "freezing");	break;
		case COMBAT_STATUS_SHOCK	:	amount = std::max(1, finalPoints.MaxLife.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, getArmorAbsorption(character.CurrentArmor), "shocked");	break;
		//case COMBAT_STATUS_STUN	:		break;
		//case COMBAT_STATUS_BLIND	:	break;
		}
	}
};

void klib::applyTurnStatusAndBonusesAndSkipTurn(CCharacter& character)
{
	if(0 >= character.Points.CurrentLife.Health)	// This character is already dead
		return;

	printf("\n");
	applyTurnStatus		(character);
	
	applyCombatBonus	(character, character.CombatBonus.Points, "Turn Combat Bonus");
	applyCombatBonus	(character, getProfessionPoints	(character.CurrentProfession	), getProfessionName(character.CurrentProfession).c_str());
	applyCombatBonus	(character, getArmorPoints		(character.CurrentArmor			), getArmorName		(character.CurrentArmor));

	applyPassive	(character, getArmorPoints		(character.CurrentArmor			).Effect.Passive, getArmorName		(character.CurrentArmor			));
	applyPassive	(character, getProfessionPoints	(character.CurrentProfession	).Effect.Passive, getProfessionName	(character.CurrentProfession	));
	applyPassive	(character, getWeaponPoints		(character.CurrentWeapon		).Effect.Passive, getWeaponName		(character.CurrentWeapon		));

	character.CombatBonus	.NextTurn();
	character.CombatStatus	.NextTurn();
	printf("\n");
}

bool klib::executeItem(int32_t indexInventory, CCharacter& user, CCharacter& target) {

	const SItem& item = user.Inventory.Slots[indexInventory].Item;
	std::string itemName = getItemName(item);

	bool bUsedItem = false;

	printf("\n%s uses: %s.\n\n", user.Name.c_str(), itemName.c_str());
	switch( itemDescriptions[item.Index].Type )
	{
	case ITEM_TYPE_POTION:
		bUsedItem = usePotion(item, user);
		break;

	case ITEM_TYPE_GRENADE:
		bUsedItem = useGrenade(item, user, target);
		break;

	default:
		printf("This item type does nothing yet... But we still remove it from your inventory!\n");
	}

	if(bUsedItem)
		removeItem(user.Inventory, indexInventory, user.Name);

	return bUsedItem;
}
