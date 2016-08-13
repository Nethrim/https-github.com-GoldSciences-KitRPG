#pragma once

#include "Character.h"

enum ENEMY_TYPE : uint8_t
{	ENEMY_TYPE_UNKNOWN	= 0
,	ENEMY_TYPE_WOLF		= 1
,	ENEMY_TYPE_RAIDER	= 2
,	ENEMY_TYPE_SOLDIER	= 3
,	ENEMY_TYPE_COUNT	= 4
};

static const CCharacter& getEnemyDefinition(ENEMY_TYPE enemyType)
{
	static const CCharacter invalidEnemy	(CHARACTER_TYPE_ENEMY, 250+(rand()%50), 8+(rand()%5), 80+(rand()%20), 100+(rand()%20), "1nd3x 32202 (a character that spawns due to a bug)");

	static const CCharacter enemyTable[] =	{	{CHARACTER_TYPE_ENEMY,	250+(rand()%50), 8+(rand()%5), 80+(rand()%20), 100+(rand()%20), "Unknown (a character that spawns due to a bug)"}
											,	{CHARACTER_TYPE_ENEMY,	10,		1,	50,	10,		"Wolf"		} //type, MaxHP, Atk, HitChance, dropCoins, name.
											,	{CHARACTER_TYPE_ENEMY,	15,		2,	50,	20,		"Raider"	} //type, MaxHP, Atk, HitChance, dropCoins, name.
											,	{CHARACTER_TYPE_ENEMY,	20,		3,	45,	30,		"Soldier"	} //type, MaxHP, Atk, HitChance, dropCoins, name.
											};
	if(enemyType >= size(enemyTable) )
		return invalidEnemy;
	else 
		return enemyTable[enemyType];
}