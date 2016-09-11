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
		currentEnemy.Inventory.Items.AddElement({ 1+int16_t(rand()%(size(itemDescriptions)-1)), int16_t(1+rand()%size(itemModifiers)), int16_t(rand()%size(itemGrades)) });

	int32_t twoEnemyType = enemyType*2;
	currentEnemy.CurrentEquip.Weapon		.Definition	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	definitionsWeapon		)-1 );	
	currentEnemy.CurrentEquip.Armor			.Definition	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	definitionsArmor		)-1 );	
	currentEnemy.CurrentEquip.Accessory		.Definition	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	definitionsAccessory	)-1 );	
	currentEnemy.CurrentEquip.Vehicle		.Definition	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	definitionsVehicle		)-1 );
	currentEnemy.CurrentEquip.Facility		.Definition	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	definitionsFacility		)-1 );
	currentEnemy.CurrentEquip.Profession	.Definition	= (int16_t)(1+(rand()%(size( definitionsProfession	)-1)));							
	currentEnemy.CurrentEquip.StageProp		.Definition	= (int16_t)(1+(rand()%(size( definitionsStageProp	)-1)));							
	//											  
	currentEnemy.CurrentEquip.Weapon		.Modifier	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	modifiersWeapon		)-1 );	
	currentEnemy.CurrentEquip.Armor			.Modifier	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	modifiersArmor		)-1 );	
	currentEnemy.CurrentEquip.Accessory		.Modifier	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	modifiersAccessory	)-1 );	
	currentEnemy.CurrentEquip.Vehicle		.Modifier	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	modifiersVehicle	)-1 );
	currentEnemy.CurrentEquip.Facility		.Modifier	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	modifiersFacility	)-1 );
	currentEnemy.CurrentEquip.Profession	.Modifier	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	modifiersProfession	)-1 );
	currentEnemy.CurrentEquip.StageProp		.Modifier	= rand() %	1 + std::min( twoEnemyType, (int32_t)size(	modifiersStageProp	)-1 );
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

	const SEntityPoints finalEnemyPoints	= calculateFinalPoints(currentEnemy);
	currentEnemy.Points.LifeCurrent			= finalEnemyPoints.LifeMax;
}