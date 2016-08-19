#include "Enemy.h"
#include "Item.h"
#include "Misc.h"
#include "Weapon.h"
#include "Armor.h"
#include "Profession.h"

void klib::setupEnemy(CCharacter& adventurer, CCharacter& currentEnemy, uint32_t enemyType)
{
	addItem( currentEnemy.Inventory, {1, 1} );
	for(uint32_t i=1; i<enemyType; ++i)
		addItem( currentEnemy.Inventory, { 1+(rand()%((int32_t)size(itemDefinitions)-1)), rand()%(int32_t)size(itemModifiers) } );

	currentEnemy.Weapon		.Index		= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(weaponDefinitions		)));
	currentEnemy.Armor		.Index		= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(armorDefinitions		)));
	currentEnemy.Profession	.Index		= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(professionDefinitions	)));

	currentEnemy.Weapon		.Modifier	= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(weaponModifiers		)));
	currentEnemy.Armor		.Modifier	= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(armorModifiers			)));
	currentEnemy.Profession	.Modifier	= uint16_t(rand()%	std::min(enemyType*2, (uint32_t)size(professionModifiers	)));

	currentEnemy.Weapon		.Level		= std::max( uint32_t(adventurer.Weapon		.Level*.8),	1U+(rand() %	std::max(1U, uint32_t(adventurer.Weapon		.Level*.11))));
	currentEnemy.Armor		.Level		= std::max( uint32_t(adventurer.Armor		.Level*.8),	1U+(rand() %	std::max(1U, uint32_t(adventurer.Armor		.Level*.11))));
	currentEnemy.Profession	.Level		= std::max( uint32_t(adventurer.Profession	.Level*.8),	1U+(rand() %	std::max(1U, uint32_t(adventurer.Profession	.Level*.11))));

	SCharacterPoints finalEnemyPoints = calculateFinalPoints(currentEnemy);
	currentEnemy.Points.CurrentLife			= finalEnemyPoints.MaxLife;
}