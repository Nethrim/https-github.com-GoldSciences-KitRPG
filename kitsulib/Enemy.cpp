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

	currentEnemy.Weapon		.Index		= std::max((int16_t)0, std::min	((int16_t)	(adventurer.Weapon		.Index-2	+(rand()%5)), (int16_t)(size(weaponDefinitions		)-1)));
	currentEnemy.Armor		.Index		= std::max((int16_t)0, std::min	((int16_t)	(adventurer.Armor		.Index-2	+(rand()%5)), (int16_t)(size(armorDefinitions		)-1)));
	currentEnemy.Profession	.Index		= std::max((int16_t)0, std::min	((int16_t)	(adventurer.Profession	.Index-2	+(rand()%5)), (int16_t)(size(professionDefinitions	)-1)));
	currentEnemy.Weapon		.Modifier	= std::max((int16_t)0, std::min	((int16_t)	(adventurer.Weapon		.Modifier-2	+(rand()%5)), (int16_t)(size(weaponModifiers		)-1)));
	currentEnemy.Armor		.Modifier	= std::max((int16_t)0, std::min	((int16_t)	(adventurer.Armor		.Modifier-2	+(rand()%5)), (int16_t)(size(armorModifiers			)-1)));
	currentEnemy.Profession	.Modifier	= std::max((int16_t)0, std::min	((int16_t)	(adventurer.Profession	.Modifier-2	+(rand()%5)), (int16_t)(size(professionModifiers	)-1)));

	currentEnemy.Weapon		.Level		= std::max(0, (adventurer.Weapon		.Level-2+(rand()%5)));
	currentEnemy.Armor		.Level		= std::max(0, (adventurer.Armor			.Level-2+(rand()%5)));
	currentEnemy.Profession	.Level		= std::max(0, (adventurer.Profession	.Level-2+(rand()%5)));

	SCharacterPoints finalEnemyPoints = calculateFinalPoints(currentEnemy);
	currentEnemy.Points.CurrentLife			= finalEnemyPoints.MaxLife;
}