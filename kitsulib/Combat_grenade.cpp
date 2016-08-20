#include "Combat.h"
#include "Item.h"
#include "Armor.h"

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

bool klib::useGrenade(const SItem& itemGrenade, CCharacter& thrower, CCharacter& target) 
{
	const CItem& itemDescription = itemDefinitions[itemGrenade.Index];

	if(0 == itemGrenade.Modifier) {
		printf("The prop grenade thrown by %s puffs in the air and quickly falls to the ground.\n", thrower.Name.c_str());
		return true;
	}

	const int itemGrade = itemGrenade.Modifier; //std::max(1, itemGrenade.Modifier);

	// Currently the hit chance for all the grenade types are calculated with the same formula.
	int	lotteryRange	= 60+(10*itemGrade);	// calculate hit chance from item grade
	int	lotteryResult	= rand()%100;

	SCharacterPoints 
		finalPointsThrower	= calculateFinalPoints(thrower), 
		finalPointsTarget	= calculateFinalPoints(target);

	int itemEffectValue				= int(finalPointsTarget .MaxLife.HP*(0.2f*itemGrade));
	int itemEffectValueSelf			= int(finalPointsThrower.MaxLife.HP*(0.2f*itemGrade)) >> 1;

	int itemEffectValueReduced		= int(finalPointsTarget .MaxLife.HP*(0.1f*itemGrade));
	int itemEffectValueReducedSelf	= int(finalPointsThrower.MaxLife.HP*(0.1f*itemGrade)) >> 1;

	ATTACK_TARGET hitTarget = ATTACK_TARGET_MISS;

	printf("%s throws %s to %s.\n", thrower.Name.c_str(), getItemName(itemGrenade).c_str(), target.Name.c_str());
	bool bAddStatus = false;

	int32_t targetArmorAbsorption = getArmorAbsorption(target.Armor), finalPassthroughDamage = 0, reflectedDamage = 0;
	DEFEND_EFFECT attackerArmorEffect	= finalPointsThrower.DefendEffect;
	DEFEND_EFFECT targetArmorEffect		= finalPointsTarget.DefendEffect;

	PROPERTY_TYPE	grenadeProperty = itemDescription.Property;
	STATUS_TYPE		grenadeStatus = getGrenadeStatusFromProperty(grenadeProperty);
	const std::string targetArmorName = getArmorName(target.Armor);
	switch(grenadeProperty)
	{
	case PROPERTY_TYPE_SMOKE:
	case PROPERTY_TYPE_STUN:
		// Apply status with fixed 50% chance
		if( lotteryResult < lotteryRange )
			applyAttackStatus(target, grenadeStatus, itemGrade ? 1+itemGrade : 0, itemDescription.Name);
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
			applySuccessfulHit(thrower, thrower, itemEffectValueSelf, bAddStatus, grenadeStatus, 1*itemGrade, getItemName(itemGrenade));

			hitTarget = ATTACK_TARGET_SELF;
			printf("%s throws the grenade too close...\n"		
				"The grenade explodes near %s doing %u damage!\n", thrower.Name.c_str(), thrower.Name.c_str(), itemEffectValueSelf);
		}
		else if( lotteryResult == (lotteryRange-1) )
		{
			applySuccessfulHit(thrower, target,		itemEffectValue		>> 1, bAddStatus, grenadeStatus, 2*itemGrade, getItemName(itemGrenade));
			applySuccessfulHit(thrower, thrower,	itemEffectValueSelf	>> 1, bAddStatus, grenadeStatus, 1*itemGrade, getItemName(itemGrenade));

			hitTarget = (ATTACK_TARGET)(ATTACK_TARGET_SELF | ATTACK_TARGET_OTHER);
			printf("%s doesn't throw the grenade far enough so %s receives %u damage but also %s receives %u damage.\n", thrower.Name.c_str(), target.Name.c_str(), itemEffectValue, thrower.Name.c_str(), itemEffectValueSelf);
		}
		else if( lotteryResult < lotteryRange )
		{
			applySuccessfulHit(thrower, target, itemEffectValue >> 1, bAddStatus, grenadeStatus, uint32_t(3.6*itemGrade), getItemName(itemGrenade));
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

	return true;
}
