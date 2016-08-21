#include "Enemy.h"
#include "Item.h"
#include "Misc.h"
#include "Weapon.h"
#include "Armor.h"
#include "Profession.h"

#include <algorithm>

void klib::setupEnemy(CCharacter& adventurer, CCharacter& currentEnemy, int32_t enemyType)
{
	addItem( currentEnemy.Inventory, {1, 1, 1} );
	for(int32_t i=1; i<enemyType; ++i)
		addItem( currentEnemy.Inventory, { 1+int16_t(rand()%(size(itemDescriptions)-1)), int16_t(1+rand()%size(itemModifiers)), int16_t(rand()%size(itemGrades)) } );

	currentEnemy.CurrentWeapon		.Index		= 1+rand()%(1+std::min(enemyType*2, (int16_t)size(	definitionsWeapon		)-2));	//std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	definitionsWeapon		));
	currentEnemy.CurrentArmor		.Index		= 1+rand()%(1+std::min(enemyType*2, (int16_t)size(	definitionsArmor		)-2));	//std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	definitionsArmor		));
	currentEnemy.CurrentProfession	.Index		= (int16_t)(1+(rand()%(size(definitionsProfession)-1)));								//std::min((int16_t)(rand()%(enemyType*2+4)), (int16_t)size(	definitionsProfession	));
	//											  
	currentEnemy.CurrentWeapon		.Modifier	= 1+rand()%(1+std::min(enemyType*2, (int16_t)size(	modifiersWeapon			)-2));	//std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	modifiersWeapon			));
	currentEnemy.CurrentArmor		.Modifier	= 1+rand()%(1+std::min(enemyType*2, (int16_t)size(	modifiersArmor			)-2));	//std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	modifiersArmor			));
	currentEnemy.CurrentProfession	.Modifier	= 1+rand()%(1+std::min(enemyType*2, (int16_t)size(	modifiersProfession		)-2));	//std::min((int16_t)(rand()%(enemyType*2+2)), (int16_t)size(	modifiersProfession		));
//
	currentEnemy.CurrentWeapon		.Level		= std::max(0, adventurer.CurrentWeapon		.Level-1); //std::max(1, (adventurer.CurrentWeapon		.Level-1+(rand()%3)));
	currentEnemy.CurrentArmor		.Level		= std::max(0, adventurer.CurrentArmor		.Level-1); //std::max(1, (adventurer.CurrentArmor		.Level-1+(rand()%3)));
	currentEnemy.CurrentProfession	.Level		= std::max(0, adventurer.CurrentProfession	.Level-1); //std::max(1, (adventurer.CurrentProfession	.Level-1+(rand()%3)));

	currentEnemy.MaxWeapon		.Index		= currentEnemy.CurrentWeapon		.Index;
	currentEnemy.MaxArmor		.Index		= currentEnemy.CurrentArmor			.Index;
	currentEnemy.MaxProfession	.Index		= currentEnemy.CurrentProfession	.Index;

	currentEnemy.MaxWeapon		.Modifier	= currentEnemy.CurrentWeapon		.Modifier;
	currentEnemy.MaxArmor		.Modifier	= currentEnemy.CurrentArmor			.Modifier;
	currentEnemy.MaxProfession	.Modifier	= currentEnemy.CurrentProfession	.Modifier;

	currentEnemy.MaxWeapon		.Level		= currentEnemy.CurrentWeapon		.Level;
	currentEnemy.MaxArmor		.Level		= currentEnemy.CurrentArmor			.Level;
	currentEnemy.MaxProfession	.Level		= currentEnemy.CurrentProfession	.Level;

	SCharacterPoints finalEnemyPoints	= calculateFinalPoints(currentEnemy);
	currentEnemy.Points.CurrentLife		= finalEnemyPoints.MaxLife;
}