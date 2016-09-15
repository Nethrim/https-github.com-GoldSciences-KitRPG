#include "Combat.h"
#include "Armor.h"
#include "Weapon.h"
#include "Profession.h"
#include "Item.h"
#include "Vehicle.h"

#include <algorithm>

using namespace klib;

SLifePoints klib::applyShieldableDamage(CCharacter& target, int32_t damageDealt, const std::string& sourceName) {
	return applyShieldableDamage(target, damageDealt, getArmorAbsorption(target.CurrentEquip.Armor), sourceName);
}

// Returns final passthrough damage (not blocked by shield) to use by vampire and reflect effects.
SLifePoints klib::applyShieldableDamage(CCharacter& target, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName)
{
	if(0 >= target.Points.LifeCurrent.Health)	// This character is already dead
		return {};

	const std::string		targetArmorName		= getArmorName	(target.CurrentEquip.Armor);
	const SEntityPoints		targetFinalPoints	= calculateFinalPoints(target);
	const SEntityFlags		targetFinalFlags	= calculateFinalFlags(target);
	const int32_t			targetArmorShield	= targetFinalPoints.LifeMax.Shield;
	
	// Impenetrable armors always have 60% extra absorption rate.
	if(targetFinalFlags.Effect.Defend & DEFEND_EFFECT_IMPENETRABLE)
	{
		if(target.Points.LifeCurrent.Shield) {
			absorptionRate += 60;
			printf("%s damage absorption rate for %s is raised to %%%u because of the impenetrable property.\n", targetArmorName.c_str(), sourceName.c_str(), absorptionRate);
		}
	}
	else
	{	
		// If the armor is not impenetrable, the absorption rate is affected by the shield damage.
		printf("%s damage absorption rate for %s is %%%u.\n", targetArmorName.c_str(), sourceName.c_str(), absorptionRate);
		double shieldDivisor = (targetArmorShield > 1) ? targetArmorShield : 1;
		if(target.Points.LifeCurrent.Shield) 
		{
			double possibleAbsorptionRate =  absorptionRate*(target.Points.LifeCurrent.Shield/shieldDivisor);
			absorptionRate	= absorptionRate ? 
				((possibleAbsorptionRate > 1) ? (int32_t)possibleAbsorptionRate : 1) : 0;
		}
		else
			absorptionRate = 0;

		printf("%s final damage absorption rate taking deterioration into account is %%%u.\n", targetArmorName.c_str(), absorptionRate);
	}

	absorptionRate = (absorptionRate < 100) ? absorptionRate : 100;
	const double	absorptionFraction	= 0.01*absorptionRate;
	int shieldedDamage		= (int)(damageDealt * absorptionFraction);
	int passthroughDamage	= (int)(damageDealt * ((0.0 > 1.0-absorptionFraction) ? 0.0 : 1.0-absorptionFraction ));
	int totalDamage			= shieldedDamage+passthroughDamage;
	
	printf("Shielded damage: %u. Passthrough damage: %u. Expected sum: %u. Actual sum: %u. Absorption ratio: %%%u.\n", shieldedDamage, passthroughDamage, damageDealt, totalDamage, absorptionRate);
	if( totalDamage < damageDealt )	// because of the lack of rounding when casting to integer, a difference of one may be found after calculating the health-shield proportion.
	{
		int errorDamage = damageDealt-totalDamage;
		
		// if we have no health or the absorption rate is disabled we apply that error to the health. Otherwise apply it to the shield
		if(0 == absorptionRate || 0 == target.Points.LifeCurrent.Shield) {
			printf("%u damage error will be applied to the health.\n", errorDamage);
			passthroughDamage += errorDamage;
		}
		else {
			printf("%u damage error will be applied to the shield.\n", errorDamage);
			shieldedDamage += errorDamage;
		}
	}

	int finalPassthroughDamage = 0;

	// if damage has been inflicted to the shield, apply the damage and redirect the difference to the health if the damage to the shield was higher than what it could resist.
	if(shieldedDamage)
	{
		int remainingShield = target.Points.LifeCurrent.Shield-shieldedDamage;
		if(target.Points.LifeCurrent.Shield)
		{
			int32_t maxShieldedDamage = (target.Points.LifeCurrent.Shield < shieldedDamage) ? target.Points.LifeCurrent.Shield : shieldedDamage;
			printf("%s's shield absorbs %u damage from %s.\n", target.Name.c_str(), maxShieldedDamage, sourceName.c_str());
			target.Points.LifeCurrent.Shield -= maxShieldedDamage;
			if(remainingShield < 0)
				printf("%s's shield ran out allowing some damage from %s to pass through.\n", target.Name.c_str(), sourceName.c_str());
			else
				printf("%s's remaining shield is now %u.\n", target.Name.c_str(), target.Points.LifeCurrent.Shield);
		}

		if(remainingShield < 0)	// only apply damage to health if the shield didn't absorb all the damage.
		{
			finalPassthroughDamage	= remainingShield*-1;
			printf("%s's was hurt by %u shieldable damage from %s for which there was no protection.\n", target.Name.c_str(), finalPassthroughDamage, sourceName.c_str());
			target.Points.LifeCurrent.Health += remainingShield;
		}
	}

	if(passthroughDamage)
		printf("%s receives %u passthrough damage from %s.\n", target.Name.c_str(), passthroughDamage, sourceName.c_str());
	finalPassthroughDamage	+= passthroughDamage;

	target.Points.LifeCurrent.Health -= passthroughDamage;

	return { finalPassthroughDamage, damageDealt-finalPassthroughDamage };
}


COMBAT_STATUS klib::applyAttackStatus(CCharacter& target, COMBAT_STATUS weaponStatus, int32_t absorbChance, int32_t turnCount, const std::string& sourceName)
{
	if(COMBAT_STATUS_NONE == weaponStatus || 0 >= target.Points.LifeCurrent.Health)
		return COMBAT_STATUS_NONE;

	const std::string	targetArmorName			= getArmorName(target.CurrentEquip.Armor);

	COMBAT_STATUS appliedStatus = COMBAT_STATUS_NONE;

	SEntityFlags targetFinalFlags = calculateFinalFlags(target);
	if((targetFinalFlags.Effect.Defend & DEFEND_EFFECT_IMPENETRABLE) && target.Points.LifeCurrent.Shield)
	{
		absorbChance = (absorbChance > 30) ? absorbChance*2 : 60;
		printf("%s absorb chance of status by %s is modified to %%%u because of the impenetrable property of %s.\n", target.Name.c_str(), sourceName.c_str(), absorbChance, targetArmorName.c_str());
	}
	
	for(int i=0; i<MAX_COMBAT_STATUS_COUNT; i++)
	{
		COMBAT_STATUS bitStatus =  (COMBAT_STATUS)(1<<i);
		if(0 == (bitStatus & weaponStatus) )
			continue;

		std::string text = getStringFromBit(bitStatus);

		if(bitStatus & targetFinalFlags.Status.Immunity)
		{
			printf("%s is immune to %s!\n", target.Name.c_str(), text.c_str());
			continue;
		}

		if((rand()%100) < absorbChance)
		{
			printf("%s absorbs \"%s\" inflicted by %s with %%%u absorb chance.\n", targetArmorName.c_str(), text.c_str(), sourceName.c_str(), absorbChance);
			continue;
		}
		
		addStatus(target.ActiveBonus.Status, bitStatus, turnCount);
		appliedStatus = (COMBAT_STATUS)(appliedStatus|bitStatus);

		printf("%s got inflicted \"%s\" status from %s that will last for the next %u turns.\n", target.Name.c_str(), text.c_str(), sourceName.c_str(), turnCount);
	}
	return appliedStatus;
}

COMBAT_STATUS klib::applyAttackStatus(CCharacter& target, COMBAT_STATUS weaponStatus, int32_t turnCount, const std::string& sourceName)
{
	if(COMBAT_STATUS_NONE == weaponStatus || 0 >= target.Points.LifeCurrent.Health)
		return COMBAT_STATUS_NONE;

	const int32_t		targetArmorAbsorption	= getArmorAbsorption(target.CurrentEquip.Armor);
	const std::string	targetArmorName			= getArmorName(target.CurrentEquip.Armor);
	const SEntityPoints		targetFinalPoints		= calculateFinalPoints(target);
	const int32_t		targetArmorShield		= targetFinalPoints.LifeMax.Shield;

	COMBAT_STATUS appliedStatus = COMBAT_STATUS_NONE;
	
	int32_t absorbChance;

	double absorptionRatio = std::max(0.0, (target.Points.LifeCurrent.Shield/(double)targetArmorShield))/2.0;
	absorbChance = 50+(int32_t)(absorptionRatio*targetArmorAbsorption);
	absorbChance = std::min(absorbChance, 100);

	printf("%s status absorb chance after absorption calculation is %%%u.\n", target.Name.c_str(), absorbChance);
	return applyAttackStatus(target, weaponStatus, absorbChance, turnCount, sourceName);
}

int32_t klib::applyArmorReflect(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName) 
{
	const SEntityPoints targetFinalPoints = calculateFinalPoints(targetReflecting);
	const SEntityFlags targetFinalFlags = calculateFinalFlags(targetReflecting);

		if( 0 == damageDealt 
		||	0 == (targetFinalFlags.Effect.Defend & DEFEND_EFFECT_REFLECT) 
		||	0 >= targetReflecting.Points.LifeCurrent.Shield 
		||	0 >= attacker.Points.LifeCurrent.Health
		||	0 >= targetReflecting.Points.LifeCurrent.Health
		)
		return 0;

	printf("\n");
	const std::string	targetArmorName			= getArmorName	(targetReflecting.CurrentEquip.Armor);

	if(damageDealt > 0)
		printf("%s reflects %u damage from %s with %s.\n", targetReflecting.Name.c_str(), damageDealt, sourceName.c_str(), targetArmorName.c_str());
	else if(damageDealt < 0)
		printf("%s reflects %u health from %s with %s.\n", targetReflecting.Name.c_str(), damageDealt, sourceName.c_str(), targetArmorName.c_str());

	SLifePoints finalDamage	= applyShieldableDamage(attacker, damageDealt, targetArmorName);
	// If the attacker was killed by the reflect we need to avoid reflecting from her armor.
	if(finalDamage.Shield && 0 < attacker.Points.LifeCurrent.Health)
	{
		SEntityFlags		attackerFinalFlags = calculateFinalFlags(attacker);

		DEFEND_EFFECT		attackerArmorEffect = attackerFinalFlags.Effect.Defend;
		if((attackerArmorEffect & DEFEND_EFFECT_REFLECT) && attacker.Points.LifeCurrent.Shield)
		{
			const std::string	attackerArmorName		= getArmorName	(attacker.CurrentEquip.Armor);
			printf("%s causes a recursive reflection with %s dealing %u damage.\n", attackerArmorName.c_str(), targetArmorName.c_str(), damageDealt);
			applyArmorReflect(targetReflecting, attacker, finalDamage.Shield, targetArmorName);
		}
	}

	return finalDamage.Health;
}

void applyWeaponLeech(ATTACK_EFFECT testEffectBit, ATTACK_EFFECT attackerWeaponEffect, int32_t finalPassthroughDamage, int32_t maxPoints, int32_t& currentPoints, const std::string& attackerName, const std::string& targetName, const std::string& attackerWeaponName, const std::string& pointName, const std::string& gainVerb, const std::string& loseVerb )
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

SLifePoints applyUnshieldableDamage(CCharacter& attacker, CCharacter& target, const SLifePoints& damageDealt)
{
	const SLifePoints finalDamage = 
	{	(	target.Points.LifeCurrent.Health	< damageDealt.Health	) ? target.Points.LifeCurrent.Health	: damageDealt.Health
	,	(	target.Points.LifeCurrent.Mana		< damageDealt.Mana		) ? target.Points.LifeCurrent.Mana		: damageDealt.Mana
	,	(	target.Points.LifeCurrent.Shield	< damageDealt.Shield	) ? target.Points.LifeCurrent.Shield	: damageDealt.Shield
	};

	if( finalDamage.Health	)	printf("%s does %i direct damage to %s's %s.\n", attacker.Name.c_str(), finalDamage.Health	, target.Name.c_str(), "Health"	);
	if( finalDamage.Mana	)	printf("%s does %i direct damage to %s's %s.\n", attacker.Name.c_str(), finalDamage.Mana	, target.Name.c_str(), "Mana"	);
	if( finalDamage.Shield	)	printf("%s does %i direct damage to %s's %s.\n", attacker.Name.c_str(), finalDamage.Shield	, target.Name.c_str(), "Shield"	);

	target.Points.LifeCurrent.Health	-= finalDamage.Health	;
	target.Points.LifeCurrent.Mana		-= finalDamage.Mana		;
	target.Points.LifeCurrent.Shield	-= finalDamage.Shield	;
	return finalDamage;
}

SLifePoints klib::applySuccessfulHit(CCharacter& attacker, CCharacter& target, int32_t damage, bool bAddStatus, COMBAT_STATUS grenadeStatus, int32_t statusTurns, const std::string& sourceName) {
	return applySuccessfulHit(attacker, target, damage, getArmorAbsorption(target.CurrentEquip.Armor), bAddStatus, grenadeStatus, statusTurns, sourceName);
}

SLifePoints klib::applySuccessfulHit(CCharacter& attacker, CCharacter& target, int32_t damage, int32_t absorptionRate, bool bAddStatus, COMBAT_STATUS grenadeStatus, int32_t statusTurns, const std::string& sourceName)
{
	SLifePoints finalDamage = klib::applyShieldableDamage(target, damage, sourceName);
	klib::applyArmorReflect(attacker, target, finalDamage.Shield, sourceName);
	
	const SLifePoints& directDamage = calculateFinalPoints(attacker).Attack.DirectDamage;
	applyUnshieldableDamage(attacker, target, directDamage);

	// Clear sleep on successful hit.
	if(finalDamage.Health || finalDamage.Shield || finalDamage.Mana) {
		for(uint32_t i=0; i < target.ActiveBonus.Status.Count; ++i)
			if( target.ActiveBonus.Status.Status[i] == COMBAT_STATUS_SLEEP )
			{
				if(target.Points.LifeCurrent.Health < 0)
					printf("Sweet Dreams, %s!\n", target.Name.c_str());
				else
					printf("%s awakes from his induced nap!\n", target.Name.c_str());
				target.ActiveBonus.Status.TurnsLeft[i] = 0;
			}
	}

	if(bAddStatus)
		applyAttackStatus(target, grenadeStatus, statusTurns, sourceName);

	return finalDamage;
}

void klib::applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, const std::string& sourceName) {
	applySuccessfulWeaponHit(attacker, targetReflecting, damageDealt, getArmorAbsorption(targetReflecting.CurrentEquip.Armor), sourceName);
}

void klib::applyWeaponLeechEffects(CCharacter& attacker, CCharacter& targetReflecting, const SLifePoints& finalDamage, const std::string& sourceName) 
{
	SEntityPoints			attackerPoints			= calculateFinalPoints(attacker);
	SEntityFlags			attackerFlags			= calculateFinalFlags(attacker);
	ATTACK_EFFECT			attackerWeaponEffect	= attackerFlags.Effect.Attack;
	applyWeaponLeech(ATTACK_EFFECT_LEECH_HEALTH	, attackerFlags.Effect.Attack, finalDamage.Health	, attackerPoints.LifeMax.Health	, attacker.Points.LifeCurrent.Health	, attacker.Name	, targetReflecting.Name	, sourceName, "Health", "drains", "loses" );

	attackerPoints			= calculateFinalPoints(attacker);
	attackerFlags			= calculateFinalFlags(attacker);
	applyWeaponLeech(ATTACK_EFFECT_LEECH_MANA	, attackerFlags.Effect.Attack, finalDamage.Mana		, attackerPoints.LifeMax.Mana	, attacker.Points.LifeCurrent.Mana		, attacker.Name	, targetReflecting.Name	, sourceName, "Mana", "drains", "loses" );

	attackerPoints			= calculateFinalPoints(attacker);
	attackerFlags			= calculateFinalFlags(attacker);
	applyWeaponLeech(ATTACK_EFFECT_LEECH_SHIELD	, attackerFlags.Effect.Attack, finalDamage.Shield	, attackerPoints.LifeMax.Shield	, attacker.Points.LifeCurrent.Shield	, attacker.Name	, targetReflecting.Name	, sourceName, "Shield", "steals", "gives" );

	attackerPoints			= calculateFinalPoints(attacker);
	attackerFlags			= calculateFinalFlags(attacker);
	applyWeaponLeech(ATTACK_EFFECT_STEAL		, attackerFlags.Effect.Attack, finalDamage.Health+finalDamage.Shield+finalDamage.Mana	
		, 0x7FFFFFFF, attacker.Points.Coins, attacker.Name	, targetReflecting.Name	, sourceName, "Coins", "steals", "drops" );
}

void klib::applySuccessfulWeaponHit(CCharacter& attacker, CCharacter& targetReflecting, int32_t damageDealt, int32_t absorptionRate, const std::string& sourceName) 
{
	if(calculateFinalFlags(targetReflecting).Effect.Defend & DEFEND_EFFECT_BLIND)
		applyAttackStatus(targetReflecting, COMBAT_STATUS_BLIND, 1, getArmorName(targetReflecting.CurrentEquip.Armor));

	if( 0 == damageDealt )
		return;

	SEntityPoints		attackerPoints	= calculateFinalPoints(attacker);
	SEntityFlags		attackerFlags	= calculateFinalFlags(attacker);
	SLifePoints			finalDamage 	= applySuccessfulHit(attacker, targetReflecting, damageDealt, absorptionRate, attackerFlags.Status.Inflict != COMBAT_STATUS_NONE, attackerFlags.Status.Inflict, 1, sourceName);

	// Apply combat bonuses from weapon for successful hits.
	const SEntityPoints attackerWeaponPoints = getWeaponPoints(attacker.CurrentEquip.Weapon);
	applyCombatBonus(attacker, attackerWeaponPoints, sourceName);
	//attackerPoints			= calculateFinalPoints(attacker);
	//attackerFlags			= calculateFinalFlags(attacker);
	//printf("\n");

	// Apply weapon effects for successful hits.
	applyWeaponLeechEffects(attacker, targetReflecting, finalDamage, sourceName);
}

// This function returns the damage dealt to the target
bool klib::attack(CCharacter& attacker, CCharacter& target)
{
	// Calculate success from the hit chance and apply damage to target or just print the miss message.
	int32_t damageDealt = 0;

	const std::string	attackerWeaponName	= getWeaponName(attacker.CurrentEquip.Weapon);
	SEntityPoints	attackerPoints		= calculateFinalPoints(attacker);

	bool bIsBlind = 0 < attacker.ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_BLIND);

	if(0 < attacker.ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_BLACKOUT) && (getWeaponFlags(attacker.CurrentEquip.Weapon).Tech.Tech & ENTITY_TECHNOLOGY_DIGITAL)) {
		printf("This weapon was disabled by an electromagnetic pulse.\n");
		return false;
	}

	int finalChance = attackerPoints.Attack.Hit;
	if(bIsBlind)
		printf("Blindness causes %s to have %u hit chance for this turn.\n", attacker.Name.c_str(), attackerPoints.Attack.Hit >>= 1);

	if(0 < target.ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_STUN)) {
		printf("As %s is stunned, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Attack.Hit>>1);
		finalChance	+= attackerPoints.Attack.Hit>>1;
	}
	else if(0 < target.ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_SLEEP)) {
		printf("As %s is asleep, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Attack.Hit/3);
		finalChance	+= attackerPoints.Attack.Hit/3;
	}
	else if(0 < target.ActiveBonus.Status.GetStatusTurns(COMBAT_STATUS_BLIND)) {
		printf("As %s is blind, %s gains %u hit chance for this turn.\n", target.Name.c_str(), attacker.Name.c_str(), attackerPoints.Attack.Hit>>2);
		finalChance	+= attackerPoints.Attack.Hit>>2;
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

void klib::applyCombatBonus(CCharacter& character, const SEntityPoints& combatBonus, const std::string& sourceName)
{
	if(0 >= character.Points.LifeCurrent.Health)	// This character is already dead
		return;

	SEntityPoints characterPoints = calculateFinalPoints(character);

	int finalHPAdded = std::min(combatBonus.LifeCurrent.Health, std::max(0, characterPoints.LifeMax.Health-character.Points.LifeCurrent.Health));
	applyTurnBonus(character.Points.LifeCurrent.Health, characterPoints.LifeMax.Health, finalHPAdded, character.Name, "Health", sourceName);
	
	finalHPAdded = std::min(combatBonus.LifeCurrent.Mana, std::max(0, characterPoints.LifeMax.Mana-character.Points.LifeCurrent.Mana));
	applyTurnBonus(character.Points.LifeCurrent.Mana, characterPoints.LifeMax.Mana, finalHPAdded, character.Name, "Mana", sourceName);
	
	finalHPAdded = std::min(combatBonus.LifeCurrent.Shield, std::max(0, characterPoints.LifeMax.Shield-character.Points.LifeCurrent.Shield));
	applyTurnBonus(character.Points.LifeCurrent.Shield, characterPoints.LifeMax.Shield, finalHPAdded, character.Name, "Shield", sourceName);

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
	SEntityPoints	characterFinalPoints	= calculateFinalPoints(character);

	applyRegenBonus(PASSIVE_EFFECT_LIFE_REGEN		,	equipmentEffects,	characterFinalPoints.LifeMax.Health	,	character.Points.LifeCurrent.Health	, "Health"	, sourceName);
	applyRegenBonus(PASSIVE_EFFECT_MANA_REGEN		,	equipmentEffects,	characterFinalPoints.LifeMax.Mana	,	character.Points.LifeCurrent.Mana	, "Mana"	, sourceName);
	applyRegenBonus(PASSIVE_EFFECT_SHIELD_REPAIR	,	equipmentEffects,	characterFinalPoints.LifeMax.Shield	,	character.Points.LifeCurrent.Shield	, "Shield"	, sourceName);
};

void klib::applyTurnStatus(CCharacter& character)
{
	if(0 >= character.Points.LifeCurrent.Health)	// This character is already dead
		return;

	int amount=0;
	const SEntityPoints finalPoints = calculateFinalPoints(character);
	for(uint32_t i=0; i<character.ActiveBonus.Status.Count; ++i)
	{
		switch(character.ActiveBonus.Status.Status[i])
		{
		case COMBAT_STATUS_BLEEDING	:	amount = std::max(1, finalPoints.LifeMax.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, 0, "bleeding");	break;
		case COMBAT_STATUS_POISON	:	amount = std::max(1, finalPoints.LifeMax.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, 0, "poisoning");	break;
		case COMBAT_STATUS_BURN		:	amount = std::max(1, finalPoints.LifeMax.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, getArmorAbsorption(character.CurrentEquip.Armor), "burning");	break;
		case COMBAT_STATUS_FREEZING	:	amount = std::max(1, finalPoints.LifeMax.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, getArmorAbsorption(character.CurrentEquip.Armor), "freezing");	break;
		case COMBAT_STATUS_SHOCK	:	amount = std::max(1, finalPoints.LifeMax.Health/20); if( amount > 0 ) character.Score.DamageTaken += amount; applyShieldableDamage(character, amount, getArmorAbsorption(character.CurrentEquip.Armor), "shocked");	break;
		//case COMBAT_STATUS_STUN	:		break;
		//case COMBAT_STATUS_BLIND	:	break;
		}
	}
};

void klib::applyTurnStatusAndBonusesAndSkipTurn(CCharacter& character)
{
	if(0 >= character.Points.LifeCurrent.Health)	// This character is already dead
		return;

	printf("\n");
	applyTurnStatus		(character);
	
	applyCombatBonus	(character, character.ActiveBonus.Points.Points, "Turn Combat Bonus");
	applyCombatBonus	(character, getProfessionPoints	(character.CurrentEquip.Profession	), getProfessionName(character.CurrentEquip.Profession).c_str());
	applyCombatBonus	(character, getArmorPoints		(character.CurrentEquip.Armor			), getArmorName		(character.CurrentEquip.Armor));
	applyCombatBonus	(character, getVehiclePoints	(character.CurrentEquip.Vehicle		), getVehicleName	(character.CurrentEquip.Vehicle));

	applyPassive		(character, getArmorFlags		(character.CurrentEquip.Armor			).Effect.Passive	, getArmorName		(character.CurrentEquip.Armor			));
	applyPassive		(character, getProfessionFlags	(character.CurrentEquip.Profession	).Effect.Passive	, getProfessionName	(character.CurrentEquip.Profession	));
	applyPassive		(character, getWeaponFlags		(character.CurrentEquip.Weapon		).Effect.Passive	, getWeaponName		(character.CurrentEquip.Weapon		));
	applyPassive		(character, getVehicleFlags		(character.CurrentEquip.Vehicle		).Effect.Passive	, getVehicleName	(character.CurrentEquip.Vehicle		));

	character.ActiveBonus.Points	.NextTurn();
	character.ActiveBonus.Status	.NextTurn();
	printf("\n");
}

bool klib::executeItem(int32_t indexInventory, CCharacter& user, CCharacter& target) {

	const SItem& item = user.Inventory.Items.Slots[indexInventory].Entity;
	std::string itemName = getItemName(item);

	bool bUsedItem = false;

	printf("\n%s uses: %s.\n\n", user.Name.c_str(), itemName.c_str());
	switch( itemDescriptions[item.Definition].Type )
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
		user.Inventory.Items.DecreaseEntity(indexInventory);

	return bUsedItem;
}
