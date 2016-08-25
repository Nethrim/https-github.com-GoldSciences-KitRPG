#include "GameBase.h"

#include <string>

#ifndef __CHARACTER_H__928346902734698713649875239841__
#define __CHARACTER_H__928346902734698713649875239841__

namespace klib
{
#pragma pack(push, 1)

	class CCharacter : public SCharacter
	{
	public:
		std::string			Name			= "Unnamed";

		CCharacter() = default;
		CCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SEntityEffect characterEffect, SEntityStatus characterStatus, const std::string& name)
			:SCharacter	(characterType, maxHP, hitChance, attack, coins, characterEffect, characterStatus)
			,Name		(name)
			{};

		int		Save(FILE* fp)	const;
		int		Load(FILE* fp);
	};

#pragma pack(pop)
} // namespace

#endif // __CHARACTER_H__928346902734698713649875239841__