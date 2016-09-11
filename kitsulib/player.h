#include "Character.h"

#ifndef __PLAYER_H__98213640926348273649827364987234698234__
#define __PLAYER_H__98213640926348273649827364987234698234__

namespace klib
{
	// Squads should be indices to the army
	struct SSquad {
#define MAX_AGENT_SLOTS 8
		static const int32_t		Size = MAX_AGENT_SLOTS;

		int16_t						Agents[MAX_AGENT_SLOTS] = {-1, -1, -1, -1, -1, -1, -1, -1};
	
		inline void					Clear(int32_t index) {
			if(index == -1)
				memset(Agents, -1, sizeof(int16_t)*klib::size(Agents));
			else
				Agents[index] = -1;
		};
	};

	struct SPlayerSelection	// can be AI or human.
	{
		int16_t						PlayerSquad	;
		int16_t						PlayerUnit	;
		int16_t						TargetSquad	;
		int16_t						TargetUnit	;
	};

	struct SPlayer	// can be AI or human.
	{
		int32_t						Money				= 5000;//0x6FFFFFFF;
		SCharacterResearch			CompletedResearch	= {};
		SCharacterEquip				MaxResearch			= {};
		SCharacterInventory			Inventory			= {};
		SSquad					Squad				= SSquad();
		std::vector<CCharacter>		Army				= {};
		int32_t						Team				= 0;

		SPlayerSelection			Selection			= {-1, -1, -1, -1};
		std::string					Name				= "Kasparov";
	};
} // namespace

#endif // __PLAYER_H__98213640926348273649827364987234698234__
