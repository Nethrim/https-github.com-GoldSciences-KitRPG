#include "GameBase.h"

#include <string>

#ifndef __CHARACTER_H__928346902734698713649875239841__
#define __CHARACTER_H__928346902734698713649875239841__

class CCharacter : public SCharacter
{
public:
	std::string			Name			= "Unnamed";

	CCharacter() = default;
	CCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, const std::string& name)
		:SCharacter	(characterType, maxHP, hitChance, attack, coins)
		,Name		(name)
		{};

	int		Save(FILE* fp)	const;
	int		Load(FILE* fp);
};

SCharacterPoints calculateFinalPoints(const CCharacter& character);

bool addStatus(SCombatStatus& characterStatus, STATUS_TYPE statusType, uint32_t turnCount);
void rest(CCharacter& adventurerPoints);	// Take a nap and recover HP up to MaxHP.

namespace klib
{
} // namespace

#endif // __CHARACTER_H__928346902734698713649875239841__