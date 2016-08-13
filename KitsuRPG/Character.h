#pragma once

#include "Item.h"
#include "Misc.h"

// This struct holds a counter for every action that we want to record mostly for score purposes.
struct SCharacterScore {
	int BattlesWon			= 0;
	int TurnsPlayed			= 0;

	int EnemiesKilled		= 0;
	int DamageDealt			= 0;
	int DamageTaken			= 0;

	int EscapesSucceeded	= 0;
	int EscapesFailed		= 0;

	int MoneyEarned			= 0;
	int MoneySpent			= 0;

	int AttacksHit			= 0;
	int AttacksMissed		= 0;
	int AttacksReceived		= 0;
	int AttacksAvoided		= 0;

	int PotionsUsed			= 0;
	int GrenadesUsed		= 0;
};

struct SInventorySlot
{
	SItem		Description;
	uint32_t	Count;
};

enum CHARACTER_TYPE : unsigned int
{	CHARACTER_TYPE_UNKNOWN
,	CHARACTER_TYPE_NPC
,	CHARACTER_TYPE_PLAYER
,	CHARACTER_TYPE_ENEMY
,	CHARACTER_TYPE_ALLY
};

struct SCharacterPoints
{
	int	MaxHP	;
	int	HP		;
	int	Hit		;
	int	Attack	;
	int	Coins	;
};

typedef SCharacterPoints SBonusTurns;

#define MAX_INVENTORY_SLOTS 8

struct SCombatBonus
{
	SCharacterPoints	Points		= {0, 0, 0, 0, 0};	// these are points that are calculated during combat depending on equipment or item consumption.
	SBonusTurns			TurnsLeft	= {0, 0, 0, 0, 0};	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.

	void				NextTurn() {
		if( 0 >= --TurnsLeft.MaxHP	)	TurnsLeft.MaxHP		=	Points.MaxHP	= 0;
		if( 0 >= --TurnsLeft.HP		)	TurnsLeft.HP		=	Points.HP		= 0;
		if( 0 >= --TurnsLeft.Attack	)	TurnsLeft.Attack	=	Points.Attack	= 0;
		if( 0 >= --TurnsLeft.Hit	)	TurnsLeft.Hit		=	Points.Hit		= 0;
		if( 0 >= --TurnsLeft.Coins	)	TurnsLeft.Coins		=	Points.Coins	= 0;
	};
};

class CCharacter
{
public:
	CHARACTER_TYPE		Type							= CHARACTER_TYPE_UNKNOWN;
	SCharacterPoints	Points							= {10, 10, 1, 50, 50};	// These are the base character points.
	std::string			Name							= "Unnamed";
	uint32_t			ItemCount						= 0;
	SInventorySlot		Inventory[MAX_INVENTORY_SLOTS]	= {};
	SCombatBonus		CombatBonus						= {};
	SCharacterScore		Score							= {};			

	CCharacter() = default;
	CCharacter(CHARACTER_TYPE characterType, int maxHP, int attack, int hitChance, int coins, const std::string& name)
		:Type(characterType)
		,Points({maxHP, maxHP, hitChance, attack, coins})
		,Name(name)
		{};
};

void rest(SCharacterPoints& adventurerPoints);	// Take a nap and recover HP up to MaxHP.
bool addItem(CCharacter& adventurer, const SItem& itemDescription);
void showInventory(const CCharacter& adventurer);
