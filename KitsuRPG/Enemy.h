#include "Character.h"

#ifndef __ENEMY_H__923409234902364897234__
#define __ENEMY_H__923409234902364897234__

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

#endif // __ENEMY_H__923409234902364897234__