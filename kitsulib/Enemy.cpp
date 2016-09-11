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

void klib::setupAgent(const CCharacter& adventurer, CCharacter& currentEnemy, int32_t enemyType, int32_t teamId)
{
	currentEnemy.Inventory.Items.AddElement({1, 1, 1});

	for(int32_t i=1; i<enemyType; ++i)
		currentEnemy.Inventory.Items.AddElement({ 1+int16_t(rand()%(size(itemDescriptions)-1)), int16_t(1+rand()%size(itemModifiers)), int16_t(rand()%size(itemGrades)) });

	int16_t twoEnemyType = enemyType*2;
	currentEnemy.CurrentEquip.Weapon		.Definition	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsWeapon		)-2) ? twoEnemyType : (int16_t)size(	definitionsWeapon		)-2));	
	currentEnemy.CurrentEquip.Armor			.Definition	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsArmor		)-2) ? twoEnemyType : (int16_t)size(	definitionsArmor		)-2));	
	currentEnemy.CurrentEquip.Accessory		.Definition	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsAccessory	)-2) ? twoEnemyType : (int16_t)size(	definitionsAccessory	)-2));	
	currentEnemy.CurrentEquip.Vehicle		.Definition	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsVehicle		)-2) ? twoEnemyType : (int16_t)size(	definitionsVehicle		)-2));
	currentEnemy.CurrentEquip.Facility		.Definition	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsFacility	)-2) ? twoEnemyType : (int16_t)size(	definitionsFacility	)-2));
	currentEnemy.CurrentEquip.Profession	.Definition	= (int16_t)(1+(rand()%(size(definitionsProfession	)-1)));							
	currentEnemy.CurrentEquip.StageProp		.Definition	= (int16_t)(1+(rand()%(size(definitionsStageProp	)-1)));							
	//											  
	currentEnemy.CurrentEquip.Weapon		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersWeapon			)-2) ? twoEnemyType : (int16_t)size(	modifiersWeapon			)-2));	
	currentEnemy.CurrentEquip.Armor			.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersArmor			)-2) ? twoEnemyType : (int16_t)size(	modifiersArmor			)-2));	
	currentEnemy.CurrentEquip.Accessory		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersAccessory		)-2) ? twoEnemyType : (int16_t)size(	modifiersAccessory		)-2));	
	currentEnemy.CurrentEquip.Vehicle		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersVehicle		)-2) ? twoEnemyType : (int16_t)size(	modifiersVehicle		)-2));	
	currentEnemy.CurrentEquip.Facility		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersFacility		)-2) ? twoEnemyType : (int16_t)size(	modifiersFacility		)-2));	
	currentEnemy.CurrentEquip.Profession	.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersProfession		)-2) ? twoEnemyType : (int16_t)size(	modifiersProfession		)-2));	
	currentEnemy.CurrentEquip.StageProp		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersStageProp		)-2) ? twoEnemyType : (int16_t)size(	modifiersStageProp		)-2));	
//
	currentEnemy.CurrentEquip.Weapon		.Level		= (0 > (adventurer.CurrentEquip.Weapon		.Level-1)) ? 0 : adventurer.CurrentEquip.Weapon		.Level;
	currentEnemy.CurrentEquip.Armor			.Level		= (0 > (adventurer.CurrentEquip.Armor		.Level-1)) ? 0 : adventurer.CurrentEquip.Armor		.Level;
	currentEnemy.CurrentEquip.Accessory		.Level		= (0 > (adventurer.CurrentEquip.Accessory	.Level-1)) ? 0 : adventurer.CurrentEquip.Accessory	.Level;
	currentEnemy.CurrentEquip.Vehicle		.Level		= (0 > (adventurer.CurrentEquip.Vehicle		.Level-1)) ? 0 : adventurer.CurrentEquip.Vehicle	.Level;
	currentEnemy.CurrentEquip.Facility		.Level		= (0 > (adventurer.CurrentEquip.Facility	.Level-1)) ? 0 : adventurer.CurrentEquip.Facility	.Level;
	currentEnemy.CurrentEquip.Profession	.Level		= (0 > (adventurer.CurrentEquip.Profession	.Level-1)) ? 0 : adventurer.CurrentEquip.Profession	.Level;
	currentEnemy.CurrentEquip.StageProp		.Level		= (0 > (adventurer.CurrentEquip.StageProp	.Level-1)) ? 0 : adventurer.CurrentEquip.StageProp	.Level;

	// 
	currentEnemy.MaxEquip = currentEnemy.CurrentEquip;

	const SEntityPoints finalEnemyPoints	= calculateFinalPoints(currentEnemy);
	currentEnemy.Points.LifeCurrent			= finalEnemyPoints.LifeMax;
}