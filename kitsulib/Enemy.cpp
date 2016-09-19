#include "Enemy.h"

#include "Item.h"
#include "Misc.h"
#include "Weapon.h"
#include "Armor.h"
#include "Profession.h"
#include "Vehicle.h"
#include "Accessory.h"
#include "Facility.h"
#include "StageProp.h"

#include <algorithm>

void klib::setupAgent(const CCharacter& adventurer, CCharacter& currentEnemy, int32_t enemyType, int32_t teamId)
{
	currentEnemy.Goods.Inventory.Items.AddElement({1, 1, 1});

	for(int32_t i=1; i<enemyType; ++i)
		currentEnemy.Goods.Inventory.Items.AddElement({ 1+int16_t(rand()%(ktools::size(itemDescriptions)-1)), int16_t(1+rand()%ktools::size(itemModifiers)), int16_t(rand()%ktools::size(itemGrades)) });

	int32_t twoEnemyType = enemyType*2;
	currentEnemy.CurrentEquip.Weapon		.Definition	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	definitionsWeapon		)-1 ));	
	currentEnemy.CurrentEquip.Armor			.Definition	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	definitionsArmor		)-1 ));	
	currentEnemy.CurrentEquip.Accessory		.Definition	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	definitionsAccessory	)-1 ));	
	currentEnemy.CurrentEquip.Vehicle		.Definition	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	definitionsVehicle		)-1 ));
	currentEnemy.CurrentEquip.Facility		.Definition	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	definitionsFacility		)-1 ));
	currentEnemy.CurrentEquip.Profession	.Definition	= (int16_t)(1+(rand()%(ktools::size( definitionsProfession	)-1)));							
	currentEnemy.CurrentEquip.StageProp		.Definition	= (int16_t)(1+(rand()%(ktools::size( definitionsStageProp	)-1)));							
	//											  
	currentEnemy.CurrentEquip.Weapon		.Modifier	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	modifiersWeapon		)-1 ));	
	currentEnemy.CurrentEquip.Armor			.Modifier	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	modifiersArmor		)-1 ));	
	currentEnemy.CurrentEquip.Accessory		.Modifier	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	modifiersAccessory	)-1 ));	
	currentEnemy.CurrentEquip.Vehicle		.Modifier	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	modifiersVehicle	)-1 ));
	currentEnemy.CurrentEquip.Facility		.Modifier	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	modifiersFacility	)-1 ));
	currentEnemy.CurrentEquip.Profession	.Modifier	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	modifiersProfession	)-1 ));
	currentEnemy.CurrentEquip.StageProp		.Modifier	= rand() %	(1 + std::min( twoEnemyType, (int32_t)ktools::size(	modifiersStageProp	)-1 ));
//
	currentEnemy.CurrentEquip.Weapon		.Level		= (1 > (adventurer.CurrentEquip.Weapon		.Level-1)) ? 1 : adventurer.CurrentEquip.Weapon		.Level;
	currentEnemy.CurrentEquip.Armor			.Level		= (1 > (adventurer.CurrentEquip.Armor		.Level-1)) ? 1 : adventurer.CurrentEquip.Armor		.Level;
	currentEnemy.CurrentEquip.Accessory		.Level		= (1 > (adventurer.CurrentEquip.Accessory	.Level-1)) ? 1 : adventurer.CurrentEquip.Accessory	.Level;
	currentEnemy.CurrentEquip.Vehicle		.Level		= (1 > (adventurer.CurrentEquip.Vehicle		.Level-1)) ? 1 : adventurer.CurrentEquip.Vehicle	.Level;
	currentEnemy.CurrentEquip.Facility		.Level		= (1 > (adventurer.CurrentEquip.Facility	.Level-1)) ? 1 : adventurer.CurrentEquip.Facility	.Level;
	currentEnemy.CurrentEquip.Profession	.Level		= (1 > (adventurer.CurrentEquip.Profession	.Level-1)) ? 1 : adventurer.CurrentEquip.Profession	.Level;
	currentEnemy.CurrentEquip.StageProp		.Level		= (1 > (adventurer.CurrentEquip.StageProp	.Level-1)) ? 1 : adventurer.CurrentEquip.StageProp	.Level;

	// 
	currentEnemy.MaxResearch = currentEnemy.CurrentEquip;
	currentEnemy.Goods.CompletedResearch = SCharacterResearch();
	currentEnemy.Goods.CompletedResearch.Weapon		.Definitions.AddElement(currentEnemy.CurrentEquip.Weapon		.Definition);
	currentEnemy.Goods.CompletedResearch.Armor		.Definitions.AddElement(currentEnemy.CurrentEquip.Armor			.Definition);	
	currentEnemy.Goods.CompletedResearch.Accessory	.Definitions.AddElement(currentEnemy.CurrentEquip.Accessory		.Definition);	
	currentEnemy.Goods.CompletedResearch.Vehicle	.Definitions.AddElement(currentEnemy.CurrentEquip.Vehicle		.Definition);
	currentEnemy.Goods.CompletedResearch.Facility	.Definitions.AddElement(currentEnemy.CurrentEquip.Facility		.Definition);	
	currentEnemy.Goods.CompletedResearch.Profession	.Definitions.AddElement(currentEnemy.CurrentEquip.Profession	.Definition);
	currentEnemy.Goods.CompletedResearch.StageProp	.Definitions.AddElement(currentEnemy.CurrentEquip.StageProp		.Definition);	

	currentEnemy.Goods.CompletedResearch.Weapon		.Modifiers.AddElement(currentEnemy.CurrentEquip.Weapon		.Modifier);
	currentEnemy.Goods.CompletedResearch.Armor		.Modifiers.AddElement(currentEnemy.CurrentEquip.Armor		.Modifier);	
	currentEnemy.Goods.CompletedResearch.Accessory	.Modifiers.AddElement(currentEnemy.CurrentEquip.Accessory	.Modifier);	
	currentEnemy.Goods.CompletedResearch.Vehicle	.Modifiers.AddElement(currentEnemy.CurrentEquip.Vehicle		.Modifier);
	currentEnemy.Goods.CompletedResearch.Facility	.Modifiers.AddElement(currentEnemy.CurrentEquip.Facility	.Modifier);	
	currentEnemy.Goods.CompletedResearch.Profession	.Modifiers.AddElement(currentEnemy.CurrentEquip.Profession	.Modifier);
	currentEnemy.Goods.CompletedResearch.StageProp	.Modifiers.AddElement(currentEnemy.CurrentEquip.StageProp	.Modifier);	

	currentEnemy.Goods.CompletedResearch.Weapon		.MaxResearch = currentEnemy.CurrentEquip.Weapon		;
	currentEnemy.Goods.CompletedResearch.Armor		.MaxResearch = currentEnemy.CurrentEquip.Armor		;	
	currentEnemy.Goods.CompletedResearch.Accessory	.MaxResearch = currentEnemy.CurrentEquip.Accessory	;	
	currentEnemy.Goods.CompletedResearch.Vehicle	.MaxResearch = currentEnemy.CurrentEquip.Vehicle	;
	currentEnemy.Goods.CompletedResearch.Facility	.MaxResearch = currentEnemy.CurrentEquip.Facility	;	
	currentEnemy.Goods.CompletedResearch.Profession	.MaxResearch = currentEnemy.CurrentEquip.Profession	;
	currentEnemy.Goods.CompletedResearch.StageProp	.MaxResearch = currentEnemy.CurrentEquip.StageProp	;	

	currentEnemy.Points.CostMaintenance		= twoEnemyType;

	const SEntityPoints finalEnemyPoints	= calculateFinalPoints(currentEnemy);
	currentEnemy.Points.LifeCurrent			= finalEnemyPoints.LifeMax;
}