#include "Enemy.h"
#include "Item.h"
#include "Misc.h"
#include "Weapon.h"
#include "Armor.h"
#include "Profession.h"

#include <algorithm>

void klib::setupEnemy(CCharacter& adventurer, CCharacter& currentEnemy, uint32_t enemyType)
{
	addItem( currentEnemy.Inventory, {1, 1, 1} );
	for(uint32_t i=1; i<enemyType; ++i)
		addItem( currentEnemy.Inventory, { 1+(rand()%((int32_t)size(itemDescriptions)-1)), 1+rand()%(int32_t)size(itemModifiers), rand()%(int32_t)size(itemGrades) } );

	currentEnemy.CurrentWeapon		.Index		= std::max((int16_t)0, std::min	((int16_t)	(adventurer.CurrentWeapon		.Index-2	+(rand()%5)), (int16_t)(size(weaponDefinitions		)-1)));
	currentEnemy.CurrentArmor		.Index		= std::max((int16_t)0, std::min	((int16_t)	(adventurer.CurrentArmor		.Index-2	+(rand()%5)), (int16_t)(size(armorDefinitions		)-1)));
	currentEnemy.CurrentProfession	.Index		= std::max((int16_t)0, std::min	((int16_t)	(adventurer.CurrentProfession	.Index-2	+(rand()%5)), (int16_t)(size(professionDefinitions	)-1)));
	currentEnemy.CurrentWeapon		.Modifier	= std::max((int16_t)0, std::min	((int16_t)	(adventurer.CurrentWeapon		.Modifier-2	+(rand()%5)), (int16_t)(size(weaponModifiers		)-1)));
	currentEnemy.CurrentArmor		.Modifier	= std::max((int16_t)0, std::min	((int16_t)	(adventurer.CurrentArmor		.Modifier-2	+(rand()%5)), (int16_t)(size(armorModifiers			)-1)));
	currentEnemy.CurrentProfession	.Modifier	= std::max((int16_t)0, std::min	((int16_t)	(adventurer.CurrentProfession	.Modifier-2	+(rand()%5)), (int16_t)(size(professionModifiers	)-1)));

	currentEnemy.CurrentWeapon		.Level		= std::max(1, (adventurer.CurrentWeapon		.Level-2+(rand()%5)));
	currentEnemy.CurrentArmor		.Level		= std::max(1, (adventurer.CurrentArmor		.Level-2+(rand()%5)));
	currentEnemy.CurrentProfession	.Level		= std::max(1, (adventurer.CurrentProfession	.Level-2+(rand()%5)));

	currentEnemy.MaxWeapon		.Index		= currentEnemy.CurrentWeapon		.Index;		
	currentEnemy.MaxArmor		.Index		= currentEnemy.CurrentArmor			.Index;		
	currentEnemy.MaxProfession	.Index		= currentEnemy.CurrentProfession	.Index;		
	currentEnemy.MaxWeapon		.Modifier	= currentEnemy.CurrentWeapon		.Modifier;
	currentEnemy.MaxArmor		.Modifier	= currentEnemy.CurrentArmor			.Modifier;
	currentEnemy.MaxProfession	.Modifier	= currentEnemy.CurrentProfession	.Modifier;
	currentEnemy.MaxWeapon		.Level		= currentEnemy.CurrentWeapon		.Level;
	currentEnemy.MaxArmor		.Level		= currentEnemy.CurrentArmor			.Level;
	currentEnemy.MaxProfession	.Level		= currentEnemy.CurrentProfession	.Level;

	SCharacterPoints finalEnemyPoints = calculateFinalPoints(currentEnemy);
	currentEnemy.Points.CurrentLife			= finalEnemyPoints.MaxLife;
}