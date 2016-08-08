#pragma once

#include "Character.h"

// Note that this class is exactly the same as CCharacter as it doesn't define anything new.
class CEnemy : public CCharacter
{
public:
	using CCharacter::CCharacter;	// This makes our CEnemy class to expose all the CCharacter constructors.
};

static CEnemy getEnemyDefinition(CHARACTER_TYPE enemyType)
{
	static const CEnemy wolf		(CT_WOLF,		100,	5,	70,	25,		"Wolf"		); //type, MaxHP, Atk, HitChance, dropCoins, name.
	static const CEnemy raider		(CT_RAIDER,		150,	10, 50,	50,		"Raider"	); //type, MaxHP, Atk, HitChance, dropCoins, name.
	static const CEnemy soldier		(CT_SOLDIER,	250,	8,	80,	100,	"Soldier"	); //type, MaxHP, Atk, HitChance, dropCoins, name.

	switch(enemyType) 
	{
	case CT_WOLF:		return wolf;	
	case CT_RAIDER:		return raider;	
	case CT_SOLDIER:	return soldier; 
	default:
		return CEnemy(CT_UNKNOWN, 250+(rand()%50), 8+(rand()%5), 80+(rand()%20), 100+(rand()%20), "Reptile (a character that spawns due to a bug)");
	}
}