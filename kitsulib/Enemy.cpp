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

	currentEnemy.CurrentWeapon		.Index		= std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	definitionsWeapon		));
	currentEnemy.CurrentArmor		.Index		= std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	definitionsArmor		));
	currentEnemy.CurrentProfession	.Index		= std::min((int16_t)(rand()%(enemyType*2+4)), (int16_t)size(	definitionsProfession	));

	currentEnemy.CurrentWeapon		.Modifier	= std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	modifiersWeapon			));
	currentEnemy.CurrentArmor		.Modifier	= std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	modifiersArmor			));
	currentEnemy.CurrentProfession	.Modifier	= std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	modifiersProfession		));

	currentEnemy.CurrentWeapon		.Level		= std::max(1, (adventurer.CurrentWeapon		.Level-1+(rand()%3)));
	currentEnemy.CurrentArmor		.Level		= std::max(1, (adventurer.CurrentArmor		.Level-1+(rand()%3)));
	currentEnemy.CurrentProfession	.Level		= std::max(1, (adventurer.CurrentProfession	.Level-1+(rand()%3)));

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