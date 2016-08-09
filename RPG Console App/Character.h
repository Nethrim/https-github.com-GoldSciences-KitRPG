#pragma once

#include <string>

enum CHARACTER_TYPE : unsigned int
{	CT_UNKNOWN
,	CT_NPC
,	CT_PLAYER
,	CT_WOLF
,	CT_RAIDER
,	CT_SOLDIER
};

struct SCharacterPoints
{
	int MaxHP	;
	int	HP		;
	int	Attack	; 
	int	Hit		; 
	int	Coins	;
};

class CCharacter
{
public:
	CHARACTER_TYPE		Type	= CT_UNKNOWN;
	SCharacterPoints	Points	= {100, 100, 5, 50, 100};
	std::string			Name	= "Unnamed";

	CCharacter() = default;
	CCharacter(CHARACTER_TYPE characterType, int maxHP, int attack, int hitChance, int coins, const std::string& name)
		:Type(characterType)
		,Points({maxHP, maxHP, attack, hitChance, coins})
		,Name(name)
		{};
};

void rest(SCharacterPoints& adventurerPoints);	// Take a nap and recover HP up to MaxHP.
