#include "Item.h"
#include "Combat.h"

using namespace klib;

bool potionRestore(const CCharacter& potionDrinker, int32_t potionGrade, int32_t maxPoints, int32_t& currentPoints, const std::string& pointName)
{
	if(maxPoints <= currentPoints)
	{
		printf("Your %s is full!", pointName.c_str());
		return false;
	}
	int32_t itemEffectValue = (int32_t)(maxPoints*.2+1)+(rand()%std::max(1, maxPoints/10));
	itemEffectValue *= potionGrade;
	itemEffectValue = std::min(itemEffectValue, maxPoints-currentPoints);
	currentPoints += itemEffectValue;
	SCharacterPoints finalPoints	= calculateFinalPoints(potionDrinker);
	printf("The potion restores %u %s to %s! %s now has %u %s.\n", 
		itemEffectValue, pointName.c_str(), potionDrinker.Name.c_str(), potionDrinker.Name.c_str(), currentPoints, pointName.c_str());
	return true;
}

bool potionAttackBonus(const CCharacter& potionDrinker, int32_t potionGrade, int32_t& currentPoints, int32_t& turnsLeft, const std::string& pointName)
{
	int32_t itemEffectValue = 5*potionGrade;
	itemEffectValue += rand() % std::max(2, itemEffectValue>>2);

	currentPoints		+= itemEffectValue;
	if(0 == turnsLeft)
		turnsLeft = 1;
	turnsLeft	+= potionGrade;

	SCharacterPoints finalPoints	= calculateFinalPoints(potionDrinker);
	printf("The potion gives %s %u %s points for %u turns. %s now has %u %s points for the next %u turns.\n", 
		potionDrinker.Name.c_str(), itemEffectValue, pointName.c_str(), potionGrade, potionDrinker.Name.c_str(), finalPoints.Attack.Damage, pointName.c_str(), turnsLeft-1);

	return true;
}

bool klib::usePotion(const SItem& itemPotion, CCharacter& potionDrinker) 
{
	if(0 == itemPotion.Modifier) {
		printf("The prop potion drank by %s doesn't seem to taste very well...\n", potionDrinker.Name.c_str());
		return true;
	}

	const CItem& itemDescription = itemDefinitions[itemPotion.Index];

	bool bUsedItem = false;

	const int itemGrade = itemPotion.Modifier;
	
	const std::string& drinkerName	= potionDrinker.Name;
	SCharacterPoints& drinkerPoints	= potionDrinker.Points;
	SCombatBonus& drinkerBonus		= potionDrinker.CombatBonus;
	SCharacterPoints finalPoints	= calculateFinalPoints(potionDrinker);

	switch(itemDescription.Property)
	{
	case PROPERTY_TYPE_HEALTH:
		bUsedItem = potionRestore(potionDrinker, itemGrade, finalPoints.MaxLife.HP, drinkerPoints.CurrentLife.HP, "HP");
		break;
	case PROPERTY_TYPE_MANA:
		bUsedItem = potionRestore(potionDrinker, itemGrade, finalPoints.MaxLife.Mana, drinkerPoints.CurrentLife.Mana, "Mana");
		break;
	case PROPERTY_TYPE_SHIELD:
		bUsedItem = potionRestore(potionDrinker, itemGrade, finalPoints.MaxLife.Shield, drinkerPoints.CurrentLife.Shield, "Shield");
		break;
	case PROPERTY_TYPE_STRENGTH:
		bUsedItem = potionAttackBonus(potionDrinker, itemGrade, drinkerBonus.Points.Attack.Damage, drinkerBonus.TurnsLeft.Attack.Damage, "Damage");;
		break;
	case PROPERTY_TYPE_HIT:
		bUsedItem = potionAttackBonus(potionDrinker, itemGrade, drinkerBonus.Points.Attack.Hit, drinkerBonus.TurnsLeft.Attack.Hit, "Hit");
		break;
	default:
		printf("Potion type not implemented!");
	};

	if(bUsedItem)
		potionDrinker.Score.PotionsUsed++;

	return bUsedItem;
}