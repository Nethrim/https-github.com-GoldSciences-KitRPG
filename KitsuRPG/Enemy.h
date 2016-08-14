#pragma once

#include "Character.h"

enum ENEMY_TYPE : uint8_t
{	ENEMY_TYPE_UNKNOWN	= 0
,	ENEMY_TYPE_WOLF		= 1
,	ENEMY_TYPE_RAIDER	= 2
,	ENEMY_TYPE_SOLDIER	= 3
,	ENEMY_TYPE_KNIGHT	= 4
,	ENEMY_TYPE_DRAGON	= 5
};

static const CCharacter enemyDefinitions[] =	{	{CHARACTER_TYPE_ENEMY,	1,	1,	1,	1,		"Unknown"	}
												,	{CHARACTER_TYPE_ENEMY,	10,	50,	1,	10,		"Wolf"		} //type, MaxHP, Atk, HitChance, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	15,	50,	2,	20,		"Raider"	} //type, MaxHP, Atk, HitChance, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	20,	45,	3,	30,		"Soldier"	} //type, MaxHP, Atk, HitChance, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	25,	45,	4,	40,		"Knight"	} //type, MaxHP, Atk, HitChance, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	50,	55,	7,	75,		"Dragon"	} //type, MaxHP, Atk, HitChance, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	60,	55,	10,	90,		"Behemot"	} //type, MaxHP, Atk, HitChance, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	80,	60,	15,	100,	"Cthulhu"	} //type, MaxHP, Atk, HitChance, dropCoins, name.
												};

static const CCharacter& getEnemyDefinition(ENEMY_TYPE enemyType)
{
	static const CCharacter invalidEnemy	(CHARACTER_TYPE_ENEMY, 250+(rand()%50), 8+(rand()%5), 80+(rand()%20), 100+(rand()%20), "1nd3x 32202 (a character that spawns due to a bug)");

	if(enemyType >= size(enemyDefinitions) )
		return invalidEnemy;
	else 
		return enemyDefinitions[enemyType];
}