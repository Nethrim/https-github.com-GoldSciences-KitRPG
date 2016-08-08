#pragma once

#include <string>

enum CHARACTER_TYPE : unsigned int
{	CT_NPC
,	CT_PLAYER
,	CT_WOLF
,	CT_RAIDER
,	CT_SOLDIER
,	CT_UNKNOWN
};

struct SCharacterPoints
{
	SCharacterPoints() = default;
	SCharacterPoints(int maxHp, int attack, int hit, int coins)
		:MaxHp	(maxHp	)
		,Hp		(maxHp	)
		,Attack	(attack	)
		,Hit	(hit	)	
		,Coins	(coins	)
		{};

	SCharacterPoints(int maxHp, int hp, int attack, int hit, int coins)
		:MaxHp	(maxHp	)
		,Hp		(hp		)
		,Attack	(attack	)
		,Hit	(hit	)	
		,Coins	(coins	)
		{};

	int MaxHp	= 100, 
		Hp		= 100, 
		Attack	= 5, 
		Hit		= 50, 
		Coins	= 100;
};

class CCharacter
{
public:
	CCharacter() = default;
	CCharacter(CHARACTER_TYPE characterType, int MaxHP, int Attack, int Hit, int Coins, const std::string& name="Unnamed")
		:Type(characterType)
		,Points(MaxHP, Attack, Hit, Coins)
		,Name(name)
		{};

	CHARACTER_TYPE		Type	= CT_UNKNOWN;
	SCharacterPoints	Points	= {100, 5, 50, 100};
	std::string			Name	= "Unnamed";
};