#include "Character.h"

#ifndef __ENEMY_H__923409234902364897234__
#define __ENEMY_H__923409234902364897234__

static const CCharacter enemyDefinitions[] =	{	{CHARACTER_TYPE_ENEMY,	0x01,		1,		1,		1,		"Unknown"	}
												,	{CHARACTER_TYPE_ENEMY,	0x04,		50,		1,		10,		"Wolf"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x08,		50,		2,		20,		"Raider"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x10,		45,		3,		30,		"Soldier"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x14,		45,		4,		40,		"Knight"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x18,		50,		6,		60,		"T-Rex"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x20,		55,		7,		75,		"Dragon"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x24,		55,		8,		85,		"Behemot"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x28,		60,		9,		90,		"Jean Grey"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x30,		60,		10,		100,	"Cthulhu"	}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x34,		60,		13,		150,	"Thor"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x38,		70,		11,		150,	"Zeus"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x40,		60,		11,		150,	"Anhur"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												,	{CHARACTER_TYPE_ENEMY,	0x44,		70,		15,		250,	"Death"		}	//type, MaxHP, HitChance, Atk, dropCoins, name.
												};

#endif // __ENEMY_H__923409234902364897234__