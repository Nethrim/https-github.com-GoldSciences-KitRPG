#include "Character.h"

#include <crtdbg.h>
#include <Windows.h>


#define DEATH_STATUS_INFLICT	((klib::STATUS_TYPE)(klib::STATUS_TYPE_BLIND|klib::STATUS_TYPE_STUN|klib::STATUS_TYPE_BLEEDING|klib::STATUS_TYPE_BURN|klib::STATUS_TYPE_FREEZING|klib::STATUS_TYPE_POISON|klib::STATUS_TYPE_PETRIFY))
#define DEATH_STATUS_IMMUNITY	((klib::STATUS_TYPE)(klib::STATUS_TYPE_BLIND|klib::STATUS_TYPE_BLEEDING|klib::STATUS_TYPE_BURN|klib::STATUS_TYPE_FROZEN|klib::STATUS_TYPE_POISON))


int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
	int tmp = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF);
	// Clear the upper 16 bits and OR in the desired freqency
	tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;

	// Set the new bits
	_CrtSetDbgFlag(tmp);
#endif
	klib::CCharacter characters[] = 
	{	{	klib::CHARACTER_TYPE_ENEMY,		1,		1,		1,		1	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_NONE		,	"Unknown"	}
	,	{	klib::CHARACTER_TYPE_PLAYER,	4,		50,		1,		100	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_STUN		,	"Anonymous"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		1,		50,		1,		1	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_BLIND		,	"Bug"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		2,		50,		1,		10	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_BURN		,	"Wolf"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		3,		50,		2,		20	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_BURN		,	"Raider"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		4,		45,		3,		30	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_STUN		,	"Soldier"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		5,		45,		4,		40	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_BLEEDING	,	"Knight"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		6,		50,		6,		60	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_POISON	,	"T-Rex"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		7,		55,		7,		75	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_POISON	,	"Dragon"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		8,		55,		8,		85	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_NONE		,	"Behemot"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		9,		60,		9,		90	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_NONE		,	"Jean Grey"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		10,		60,		10,		100	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_NONE		,	"Cthulhu"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		11,		60,		13,		150	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_STUN		,	"Thor"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		12,		70,		11,		150	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_BLIND		,	"Zeus"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		13,		60,		11,		150	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	klib::STATUS_TYPE_NONE		,	klib::STATUS_TYPE_BLEEDING	,	"Anhur"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		14,		70,		15,		250	,	klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE	,	klib::PASSIVE_EFFECT_NONE	,	DEATH_STATUS_INFLICT		,	DEATH_STATUS_IMMUNITY		,	"Death"		}
	};
	while(true)
	{
		if(GetAsyncKeyState(VK_ESCAPE))
			break;
	}

	return 0;
}