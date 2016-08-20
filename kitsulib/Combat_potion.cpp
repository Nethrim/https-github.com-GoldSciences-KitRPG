#include "Item.h"
#include "Combat.h"

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

bool klib::usePotion(const SItem& itemPotion, CCharacter& potionDrinker) 
{
	if(0 == itemPotion.Modifier) {
		printf("The prop potion drank by %s doesn't seem to taste very well...\n", potionDrinker.Name.c_str());
		return true;
	}

	int32_t itemEffectValue;
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
		printf("%s starts feeling stronger...\n", drinkerName.c_str());
		itemEffectValue = 3*itemGrade;
		itemEffectValue += rand()%std::max(1,itemGrade*2);
		drinkerBonus.Points.Attack.Damage		+= itemEffectValue;
		if(0 == drinkerBonus.TurnsLeft.Attack.Damage)
			drinkerBonus.TurnsLeft.Attack.Damage = 1;
		drinkerBonus.TurnsLeft.Attack.Damage	+= itemGrade;
		finalPoints	= calculateFinalPoints(potionDrinker);
		printf("The potion gives %s %u Attack points for %u turns. %s now has %u Attack points for the next %u turns.\n", drinkerName.c_str(), itemEffectValue, itemGrade, drinkerName.c_str(), finalPoints.Attack.Damage, drinkerBonus.TurnsLeft.Attack.Damage-1);
		bUsedItem = true;
		break;
	case PROPERTY_TYPE_HIT:
		printf("%s starts feeling faster...\n", drinkerName.c_str());
		itemEffectValue = 10*itemGrade;
		itemEffectValue += ((rand()%std::max(1,itemGrade))+1)*5;
		drinkerBonus.Points.Attack.Hit		+= itemEffectValue;
		if(0 == drinkerBonus.TurnsLeft.Attack.Hit)
			drinkerBonus.TurnsLeft.Attack.Hit = 1;
		drinkerBonus.TurnsLeft.Attack.Hit	+= itemGrade;
		finalPoints	= calculateFinalPoints(potionDrinker);
		printf("The potion gives %s %u Hit chance points for %u turns. %s now has %u Hit chance points for the next %u turns.\n", drinkerName.c_str(), itemEffectValue, itemGrade, drinkerName.c_str(), finalPoints.Attack.Hit, drinkerBonus.TurnsLeft.Attack.Hit-1);
		bUsedItem = true;
		break;
	default:
		printf("Potion type not implemented!");
	};

	if(bUsedItem)
		potionDrinker.Score.PotionsUsed++;

	return bUsedItem;
}