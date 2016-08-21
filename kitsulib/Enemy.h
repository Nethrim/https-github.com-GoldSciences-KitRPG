#include "Character.h"

#ifndef __ENEMY_H__923409234902364897234__
#define __ENEMY_H__923409234902364897234__

#define DEATH_STATUS_INFLICT	((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLIND|klib::COMBAT_STATUS_STUN|klib::COMBAT_STATUS_BLEEDING|klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_FREEZING|klib::COMBAT_STATUS_POISON|klib::COMBAT_STATUS_PETRIFY))
#define DEATH_STATUS_IMMUNITY	((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLIND|klib::COMBAT_STATUS_BLEEDING|klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_FROZEN|klib::COMBAT_STATUS_POISON))

namespace klib
{
	static const CCharacter enemyDefinitions[] =	
	//	type,					MaxHP,	Hit,	Atk,	dropCoins,	AttackEffect,	DefendEffect,			PassiveEffect,					Status Inflict			Status Immunity name.
	{	{CHARACTER_TYPE_ENEMY,	1,		1,		1,		1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_NONE		,	"Unknown"	}
	,	{CHARACTER_TYPE_ENEMY,	1,		50,		1,		1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_BLIND		,	"Bug"		}	
	,	{CHARACTER_TYPE_ENEMY,	2,		50,		1,		10	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_BURN		,	"Wolf"		}	
	,	{CHARACTER_TYPE_ENEMY,	3,		50,		2,		20	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_BURN		,	"Raider"	}	
	,	{CHARACTER_TYPE_ENEMY,	4,		45,		3,		30	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_STUN		,	"Soldier"	}	
	,	{CHARACTER_TYPE_ENEMY,	5,		45,		4,		40	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_BLEEDING	,	"Knight"	}	
	,	{CHARACTER_TYPE_ENEMY,	6,		50,		6,		60	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_POISON		,	"T-Rex"		}	
	,	{CHARACTER_TYPE_ENEMY,	7,		55,		7,		75	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_POISON		,	"Dragon"	}	
	,	{CHARACTER_TYPE_ENEMY,	8,		55,		8,		85	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_NONE		,	"Behemot"	}	
	,	{CHARACTER_TYPE_ENEMY,	9,		60,		9,		90	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_NONE		,	"Jean Grey"	}	
	,	{CHARACTER_TYPE_ENEMY,	10,		60,		10,		100	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_NONE		,	"Cthulhu"	}	
	,	{CHARACTER_TYPE_ENEMY,	11,		60,		13,		150	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_STUN		,	"Thor"		}	
	,	{CHARACTER_TYPE_ENEMY,	12,		70,		11,		150	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_BLIND		,	"Zeus"		}	
	,	{CHARACTER_TYPE_ENEMY,	13,		60,		11,		150	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE		,	COMBAT_STATUS_BLEEDING	,	"Anhur"		}	
	,	{CHARACTER_TYPE_ENEMY,	14,		70,		15,		250	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	DEATH_STATUS_INFLICT	,	DEATH_STATUS_IMMUNITY	,	"Death"		}	
	};

	void setupEnemy(CCharacter& adventurer, CCharacter& currentEnemy, int32_t enemyType);
}	// namespace
#endif // __ENEMY_H__923409234902364897234__