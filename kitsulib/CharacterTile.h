#include <cstdint>

#ifndef __CHARACTER_TILE_H__2986498276489273469827346982734__
#define __CHARACTER_TILE_H__2986498276489273469827346982734__

namespace klib
{
#pragma pack(push, 1)
	struct SCharacterTile 
	{
		int8_t		PlayerIndex	;
		int8_t		SquadIndex	;
		int8_t		AgentIndex	;
	};
#pragma pack(pop)
} // namespace

#endif // __CHARACTER_TILE_H__2986498276489273469827346982734__