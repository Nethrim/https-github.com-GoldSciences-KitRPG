#pragma once

#include "Item.h"
#include "Misc.h"

// This struct holds a counter for every action that we want to record mostly for score purposes.
struct SCharacterScore 
{
	uint64_t BattlesWon			= 0;
	uint64_t TurnsPlayed		= 0;

	uint64_t EnemiesKilled		= 0;
	uint64_t DamageDealt		= 0;
	uint64_t DamageTaken		= 0;

	uint64_t EscapesSucceeded	= 0;
	uint64_t EscapesFailed		= 0;

	uint64_t MoneyEarned		= 0;
	uint64_t MoneySpent			= 0;

	uint64_t AttacksHit			= 0;
	uint64_t AttacksMissed		= 0;
	uint64_t AttacksReceived	= 0;
	uint64_t AttacksAvoided		= 0;

	uint64_t PotionsUsed		= 0;
	uint64_t GrenadesUsed		= 0;
};

struct SInventorySlot
{
	uint32_t	ItemIndex;
	uint32_t	ItemCount;
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

#define MAX_INVENTORY_SLOTS 9
struct SCharacterInventory
{
	uint32_t			ItemCount					= 0;
	SInventorySlot		Slots[MAX_INVENTORY_SLOTS]	= {};
};

class CCharacter
{
public:
	CHARACTER_TYPE		Type							= CHARACTER_TYPE_UNKNOWN;
	SCharacterPoints	Points							= {10, 10, 50, 1, 10};	// These are the base character points.
	std::string			Name							= "Unnamed";
	SCombatBonus		CombatBonus						= {};
	SCharacterInventory	Inventory						= {};
	SCharacterScore		Score							= {};	

	CCharacter() = default;
	CCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, const std::string& name)
		:Type(characterType)
		,Points({maxHP, maxHP, hitChance, attack, coins})
		,Name(name)
		{};
};

void rest(SCharacterPoints& adventurerPoints);	// Take a nap and recover HP up to MaxHP.
bool addItem(SCharacterInventory& adventurer, uint32_t itemIndex);
void showInventory(const CCharacter& adventurer);
