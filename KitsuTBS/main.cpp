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
	while(true)
	{
		if(GetAsyncKeyState(VK_ESCAPE))
			break;
	}

	return 0;
}