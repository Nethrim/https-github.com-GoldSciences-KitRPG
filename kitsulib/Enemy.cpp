#include "Enemy.h"

#include "Item.h"
#include "Misc.h"
#include "Weapon.h"
#include "Armor.h"
#include "Profession.h"
#include "Vehicle.h"
#include "Accessory.h"
#include "Facility.h"

void klib::setupEnemy(const CCharacter& adventurer, CCharacter& currentEnemy, int32_t enemyType)
{
	currentEnemy.Inventory.Items.AddElement({1, 1, 1});
	for(int32_t i=1; i<enemyType; ++i)
		currentEnemy.Inventory.Items.AddElement({ 1+int16_t(rand()%(size(itemDescriptions)-1)), int16_t(1+rand()%size(itemModifiers)), int16_t(rand()%size(itemGrades)) });

	int16_t twoEnemyType = enemyType*2;
	currentEnemy.CurrentEquip.Weapon		.Index		= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsWeapon		)-2) ? twoEnemyType : (int16_t)size(	definitionsWeapon		)-2));	
	currentEnemy.CurrentEquip.Armor			.Index		= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsArmor		)-2) ? twoEnemyType : (int16_t)size(	definitionsArmor		)-2));	
	currentEnemy.CurrentEquip.Accessory		.Index		= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsAccessory	)-2) ? twoEnemyType : (int16_t)size(	definitionsAccessory	)-2));	
	currentEnemy.CurrentEquip.Vehicle		.Index		= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsVehicle		)-2) ? twoEnemyType : (int16_t)size(	definitionsVehicle		)-2));
	currentEnemy.CurrentEquip.Facility		.Index		= 1+rand()%(1+((twoEnemyType < (int16_t)size(	definitionsFacilities	)-2) ? twoEnemyType : (int16_t)size(	definitionsFacilities	)-2));
	currentEnemy.CurrentEquip.Profession	.Index		= (int16_t)(1+(rand()%(size(definitionsProfession)-1)));							
	//											  
	currentEnemy.CurrentEquip.Weapon		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersWeapon			)-2) ? twoEnemyType : (int16_t)size(	modifiersWeapon			)-2));	
	currentEnemy.CurrentEquip.Armor			.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersArmor			)-2) ? twoEnemyType : (int16_t)size(	modifiersArmor			)-2));	
	currentEnemy.CurrentEquip.Accessory		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersAccessory		)-2) ? twoEnemyType : (int16_t)size(	modifiersAccessory		)-2));	
	currentEnemy.CurrentEquip.Vehicle		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersVehicle		)-2) ? twoEnemyType : (int16_t)size(	modifiersVehicle		)-2));	
	currentEnemy.CurrentEquip.Facility		.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersFacilities		)-2) ? twoEnemyType : (int16_t)size(	modifiersFacilities		)-2));	
	currentEnemy.CurrentEquip.Profession	.Modifier	= 1+rand()%(1+((twoEnemyType < (int16_t)size(	modifiersProfession		)-2) ? twoEnemyType : (int16_t)size(	modifiersProfession		)-2));	
//
	currentEnemy.CurrentEquip.Weapon		.Level		= (0 > (adventurer.CurrentEquip.Weapon		.Level-1)) ? 0 : adventurer.CurrentEquip.Weapon		.Level;
	currentEnemy.CurrentEquip.Armor			.Level		= (0 > (adventurer.CurrentEquip.Armor		.Level-1)) ? 0 : adventurer.CurrentEquip.Armor		.Level;
	currentEnemy.CurrentEquip.Profession	.Level		= (0 > (adventurer.CurrentEquip.Profession	.Level-1)) ? 0 : adventurer.CurrentEquip.Profession	.Level;
	currentEnemy.CurrentEquip.Vehicle		.Level		= (0 > (adventurer.CurrentEquip.Vehicle		.Level-1)) ? 0 : adventurer.CurrentEquip.Vehicle	.Level;

	//
	currentEnemy.MaxEquip.Weapon			.Index		= currentEnemy.CurrentEquip.Weapon		.Index;
	currentEnemy.MaxEquip.Armor				.Index		= currentEnemy.CurrentEquip.Armor		.Index;
	currentEnemy.MaxEquip.Profession		.Index		= currentEnemy.CurrentEquip.Profession	.Index;
	currentEnemy.MaxEquip.Vehicle			.Index		= currentEnemy.CurrentEquip.Vehicle		.Index;

	currentEnemy.MaxEquip.Weapon			.Modifier	= currentEnemy.CurrentEquip.Weapon		.Modifier;
	currentEnemy.MaxEquip.Armor				.Modifier	= currentEnemy.CurrentEquip.Armor		.Modifier;
	currentEnemy.MaxEquip.Profession		.Modifier	= currentEnemy.CurrentEquip.Profession	.Modifier;
	currentEnemy.MaxEquip.Vehicle			.Modifier	= currentEnemy.CurrentEquip.Vehicle		.Modifier;

	currentEnemy.MaxEquip.Weapon			.Level		= currentEnemy.CurrentEquip.Weapon		.Level;
	currentEnemy.MaxEquip.Armor				.Level		= currentEnemy.CurrentEquip.Armor		.Level;
	currentEnemy.MaxEquip.Profession		.Level		= currentEnemy.CurrentEquip.Profession	.Level;
	currentEnemy.MaxEquip.Vehicle			.Level		= currentEnemy.CurrentEquip.Vehicle		.Level;

	SEntityPoints finalEnemyPoints			= calculateFinalPoints(currentEnemy);
	currentEnemy.Points.LifeCurrent			= finalEnemyPoints.LifeMax;
}