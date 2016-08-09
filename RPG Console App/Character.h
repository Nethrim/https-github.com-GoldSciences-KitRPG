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
	int MaxHP	= 100;
	int	HP		= 100;
	int	Attack	= 5; 
	int	Hit		= 50; 
	int	Coins	= 100;

	// The constructors help initializing values.
	SCharacterPoints() = default;
	SCharacterPoints(int maxHP, int attack, int hit, int coins)	// this constructor sets the current HP with the same value as the max HP. This is the most common use case.
		:MaxHP	(maxHP	)
		,HP		(maxHP	)
		,Attack	(attack	)
		,Hit	(hit	)	
		,Coins	(coins	)
		{};

	SCharacterPoints(int maxHP, int currentHP, int attack, int hit, int coins)	// This constuctor allows to provide a different CurrentHP value different from MaxHP.
		:MaxHP	(maxHP	)
		,HP		(currentHP)
		,Attack	(attack	)
		,Hit	(hit	)	
		,Coins	(coins	)
		{};
};

class CCharacter
{
public:
	CHARACTER_TYPE		Type	= CT_UNKNOWN;
	SCharacterPoints	Points	= {100, 5, 50, 100};
	std::string			Name	= "Unnamed";

	CCharacter() = default;
	CCharacter(CHARACTER_TYPE characterType, int maxHP, int attack, int hitChance, int coins, const std::string& name)
		:Type(characterType)
		,Points(maxHP, attack, hitChance, coins)
		,Name(name)
		{};
};