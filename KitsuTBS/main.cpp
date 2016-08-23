#include "Character.h"

#include <crtdbg.h>
#include <Windows.h>


#define DEATH_STATUS_INFLICT	((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLIND|klib::COMBAT_STATUS_STUN|klib::COMBAT_STATUS_BLEEDING|klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_FREEZING|klib::COMBAT_STATUS_POISON|klib::COMBAT_STATUS_PETRIFY))
#define DEATH_STATUS_IMMUNITY	((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLIND|klib::COMBAT_STATUS_BLEEDING|klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_FROZEN|klib::COMBAT_STATUS_POISON))


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
	{	{	klib::CHARACTER_TYPE_ENEMY,		1,		1,		1,		1	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_NONE		},	"Unknown"	}
	,	{	klib::CHARACTER_TYPE_PLAYER,	4,		50,		1,		100	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_STUN		},	"Anonymous"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		1,		50,		1,		1	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_BLIND		},	"Bug"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		2,		50,		1,		10	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_BURN		},	"Wolf"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		3,		50,		2,		20	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_BURN		},	"Raider"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		4,		45,		3,		30	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_STUN		},	"Soldier"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		5,		45,		4,		40	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_BLEEDING	},	"Knight"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		6,		50,		6,		60	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_POISON		},	"T-Rex"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		7,		55,		7,		75	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_POISON		},	"Dragon"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		8,		55,		8,		85	,	{}	,	{}																	,	"Behemot"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		9,		60,		9,		90	,	{}	,	{}																	,	"Jean Grey"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		10,		60,		10,		100	,	{}	,	{}																	,	"Cthulhu"	}
	,	{	klib::CHARACTER_TYPE_ENEMY,		11,		60,		13,		150	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_STUN		},	"Thor"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		12,		70,		11,		150	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_BLIND		},	"Zeus"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		13,		60,		11,		150	,	{}	,	{klib::COMBAT_STATUS_NONE		,	klib::COMBAT_STATUS_BLEEDING	},	"Anhur"		}
	,	{	klib::CHARACTER_TYPE_ENEMY,		14,		70,		15,		250	,	{}	,	{DEATH_STATUS_INFLICT			,	DEATH_STATUS_IMMUNITY			},	"Death"		}
	};
	while(true)
	{
		if(GetAsyncKeyState(VK_ESCAPE))
			break;
	}

	return 0;
}