#include "GameBase.h"

#include <string>

#ifndef __CHARACTER_H__928346902734698713649875239841__
#define __CHARACTER_H__928346902734698713649875239841__

namespace klib
{
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

	SCharacterPoints calculateFinalPoints(const CCharacter& character);

	bool addStatus(SCombatStatus& characterStatus, COMBAT_STATUS statusType, int32_t turnCount);
	void rest(CCharacter& adventurerPoints);	// Take a nap and recover HP up to MaxHP.
} // namespace

#endif // __CHARACTER_H__928346902734698713649875239841__