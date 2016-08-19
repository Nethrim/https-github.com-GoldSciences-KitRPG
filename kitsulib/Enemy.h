#include "Character.h"

#ifndef __ENEMY_H__923409234902364897234__
#define __ENEMY_H__923409234902364897234__

namespace klib
{
	static const CCharacter enemyDefinitions[] =	
	//	type,					MaxHP,	Hit,	Atk,	dropCoins,		name.
	{	{CHARACTER_TYPE_ENEMY,	1,		1,		1,		1,			"Unknown"	}
	,	{CHARACTER_TYPE_ENEMY,	1,		50,		1,		1,			"Bug"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	2,		50,		1,		10,			"Wolf"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	3,		50,		2,		20,			"Raider"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	4,		45,		3,		30,			"Soldier"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	5,		45,		4,		40,			"Knight"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	6,		50,		6,		60,			"T-Rex"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	7,		55,		7,		75,			"Dragon"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	8,		55,		8,		85,			"Behemot"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	9,		60,		9,		90,			"Jean Grey"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	10,		60,		10,		100,		"Cthulhu"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	11,		60,		13,		150,		"Thor"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	12,		70,		11,		150,		"Zeus"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	13,		60,		11,		150,		"Anhur"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	,	{CHARACTER_TYPE_ENEMY,	14,		70,		15,		250,		"Death"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
	};

	void setupEnemy(CCharacter& adventurer, CCharacter& currentEnemy, uint32_t enemyType);
}	// namespace
#endif // __ENEMY_H__923409234902364897234__