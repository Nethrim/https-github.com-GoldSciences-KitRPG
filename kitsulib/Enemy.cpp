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
	currentEnemy.Inventory.Items.AddElement({1, 1, 1});

	for(int32_t i=1; i<enemyType; ++i)
		currentEnemy.Inventory.Items.AddElement({ 1+int16_t(rand()%(ktools::size(itemDescriptions)-1)), int16_t(1+rand()%ktools::size(itemModifiers)), int16_t(rand()%ktools::size(itemGrades)) });

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
	currentEnemy.MaxEquip = currentEnemy.CurrentEquip;
	currentEnemy.Researched = SCharacterResearch();
	currentEnemy.Researched.Weapon		.Definitions.AddElement(currentEnemy.CurrentEquip.Weapon		.Definition);
	currentEnemy.Researched.Armor		.Definitions.AddElement(currentEnemy.CurrentEquip.Armor			.Definition);	
	currentEnemy.Researched.Accessory	.Definitions.AddElement(currentEnemy.CurrentEquip.Accessory		.Definition);	
	currentEnemy.Researched.Vehicle		.Definitions.AddElement(currentEnemy.CurrentEquip.Vehicle		.Definition);
	currentEnemy.Researched.Facility	.Definitions.AddElement(currentEnemy.CurrentEquip.Facility		.Definition);	
	currentEnemy.Researched.Profession	.Definitions.AddElement(currentEnemy.CurrentEquip.Profession	.Definition);
	currentEnemy.Researched.StageProp	.Definitions.AddElement(currentEnemy.CurrentEquip.StageProp		.Definition);	

	currentEnemy.Researched.Weapon		.Modifiers.AddElement(currentEnemy.CurrentEquip.Weapon		.Modifier);
	currentEnemy.Researched.Armor		.Modifiers.AddElement(currentEnemy.CurrentEquip.Armor		.Modifier);	
	currentEnemy.Researched.Accessory	.Modifiers.AddElement(currentEnemy.CurrentEquip.Accessory	.Modifier);	
	currentEnemy.Researched.Vehicle		.Modifiers.AddElement(currentEnemy.CurrentEquip.Vehicle		.Modifier);
	currentEnemy.Researched.Facility	.Modifiers.AddElement(currentEnemy.CurrentEquip.Facility	.Modifier);	
	currentEnemy.Researched.Profession	.Modifiers.AddElement(currentEnemy.CurrentEquip.Profession	.Modifier);
	currentEnemy.Researched.StageProp	.Modifiers.AddElement(currentEnemy.CurrentEquip.StageProp	.Modifier);	

	currentEnemy.Researched.Weapon		.MaxResearch = currentEnemy.CurrentEquip.Weapon		;
	currentEnemy.Researched.Armor		.MaxResearch = currentEnemy.CurrentEquip.Armor		;	
	currentEnemy.Researched.Accessory	.MaxResearch = currentEnemy.CurrentEquip.Accessory	;	
	currentEnemy.Researched.Vehicle		.MaxResearch = currentEnemy.CurrentEquip.Vehicle	;
	currentEnemy.Researched.Facility	.MaxResearch = currentEnemy.CurrentEquip.Facility	;	
	currentEnemy.Researched.Profession	.MaxResearch = currentEnemy.CurrentEquip.Profession	;
	currentEnemy.Researched.StageProp	.MaxResearch = currentEnemy.CurrentEquip.StageProp	;	

	currentEnemy.Points.CostMaintenance		= twoEnemyType;

	const SEntityPoints finalEnemyPoints	= calculateFinalPoints(currentEnemy);
	currentEnemy.Points.LifeCurrent			= finalEnemyPoints.LifeMax;
}