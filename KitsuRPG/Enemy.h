#pragma once

#include "Character.h"

static const CCharacter enemyDefinitions[] =	{	{CHARACTER_TYPE_ENEMY,	1,		1,		1,		1,		"Unknown"	}
												,	{CHARACTER_TYPE_ENEMY,	10,		50,		1,		10,		"Wolf"		} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	15,		50,		2,		20,		"Raider"	} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	20,		45,		3,		30,		"Soldier"	} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	25,		45,		4,		40,		"Knight"	} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	40,		50,		6,		60,		"T-Rex"		} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	50,		55,		7,		75,		"Dragon"	} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	60,		55,		10,		85,		"Behemot"	} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	65,		60,		12,		90,		"Jean Grey"	} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	80,		60,		15,		100,	"Cthulhu"	} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	100,	00,		30,		150,	"Thor"		} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	100,	70,		20,		150,	"Zeus"		} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	200,	60,		20,		150,	"Anhur"		} //type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	300,	70,		30,		250,	"Death"		} //type, MaxHP, HitChance, Atk, dropCoins, name.
												};

static const CCharacter& getEnemyDefinition(uint32_t enemyType)
{
	static const CCharacter invalidEnemy	(CHARACTER_TYPE_ENEMY, 250+(rand()%50), 70+(rand()%5), 40+(rand()%20), 100+(rand()%20), "1nd3x 32202 (a character that spawns due to a bug)");

	if(enemyType >= size(enemyDefinitions) )
		return invalidEnemy;
	else 
		return enemyDefinitions[enemyType];
}