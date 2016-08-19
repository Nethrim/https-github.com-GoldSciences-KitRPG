#include "Item.h"
#include "Combat.h"

void klib::usePotion(const SItem& itemPotion, CCharacter& potionDrinker) 
{
	int32_t itemEffectValue;
	const CItem& itemDescription = itemDefinitions[itemPotion.Index];

	if(0 == itemPotion.Modifier) {
		printf("The prop potion drank by %s doesn't seem to taste very well...\n", potionDrinker.Name.c_str());
		return;
	}

	const int itemGrade = itemPotion.Modifier;
	
	const std::string& drinkerName	= potionDrinker.Name;
	SCharacterPoints& drinkerPoints	= potionDrinker.Points;
	SCombatBonus& drinkerBonus		= potionDrinker.CombatBonus;
	SCharacterPoints finalPoints	= calculateFinalPoints(potionDrinker);

	switch(itemDescription.Property)
	{
	case PROPERTY_TYPE_HEALTH:
		printf("%s starts feeling better...\n", drinkerName.c_str());
		itemEffectValue = (int32_t)(finalPoints.MaxLife.HP*.2+1)+(rand()%std::max(1, finalPoints.MaxLife.HP/10));
		itemEffectValue *= itemGrade;
		drinkerPoints.CurrentLife.HP += itemEffectValue;
		drinkerPoints.CurrentLife.HP = std::min(drinkerPoints.CurrentLife.HP, finalPoints.MaxLife.HP);
		finalPoints	= calculateFinalPoints(potionDrinker);
		printf("The potion heals %s for %u! %s now has %u HP.\n", drinkerName.c_str(), itemEffectValue, drinkerName.c_str(), drinkerPoints.CurrentLife.HP);
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
		break;
	default:
		printf("Potion type not implemented!");
	};

	potionDrinker.Score.PotionsUsed++;
}