#pragma once

#include "Item.h"
#include "Weapon.h"
#include "Misc.h"
#include <algorithm>

#pragma pack(push, 1)

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

enum CHARACTER_TYPE : uint32_t
{	CHARACTER_TYPE_UNKNOWN
,	CHARACTER_TYPE_NPC
,	CHARACTER_TYPE_PLAYER
,	CHARACTER_TYPE_ENEMY
,	CHARACTER_TYPE_ALLY
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

#define MAX_COMBAT_STATUS	16

struct SCombatStatus
{
	uint32_t			Count	= 0;
	STATUS_TYPE			Status		[MAX_COMBAT_STATUS]	= {};
	int					TurnsLeft	[MAX_COMBAT_STATUS]	= {};
	int					GetStatusTurns(STATUS_TYPE status)
	{
		for(uint32_t i=0; i<Count; ++i)
			if(Status[i] == status)
				return TurnsLeft[i];
		return 0;
	};
	void				NextTurn() {
		for( uint32_t i=0; i<Count; ++i)
			if(TurnsLeft[i] && 0 == --TurnsLeft[i])
			{
				Status		[i]	=	Status		[--Count];
				TurnsLeft	[i]	=	TurnsLeft	[Count];
			}
	};
};

#define MAX_INVENTORY_SLOTS 9

struct SCharacterInventory
{
	uint32_t			ItemCount					= 0;
	SInventorySlot		Slots[MAX_INVENTORY_SLOTS]	= {};
};

// POD to store all the character data that is not a string or object.
struct SCharacter
{
	CHARACTER_TYPE		Type			= CHARACTER_TYPE_UNKNOWN;
	SCharacterPoints	Points			= {10, 10, 50, 1, 10};	// These are the base character points.
	SCombatBonus		CombatBonus		= {};
	SCombatStatus		CombatStatus	= {};
	SCharacterInventory	Inventory		= {};
	SCharacterScore		Score			= {};	
	uint32_t			Weapon			= 0;

	constexpr SCharacter() = default;
	constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins) 
		:Type			(characterType)
		,Points			({maxHP, maxHP, hitChance, attack, coins})
		,CombatBonus	({})
		,CombatStatus	({})
		,Inventory		({})
		,Score			({})
		,Weapon			(0)
	{};

	int		Save(FILE* fp)	const;
	int		Load(FILE* fp);
};

#pragma pack(pop)

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



static SCharacterPoints calculateFinalPoints(const CCharacter& character)
{
	SCharacterPoints result;
	const SWeapon& weaponDefinition = weaponDefinitions[character.Weapon];

	result.MaxHP	= character.Points.MaxHP			+	weaponDefinition.Points.MaxHP	+	character.CombatBonus.Points.MaxHP	;
	result.HP		=										weaponDefinition.Points.HP		+	character.CombatBonus.Points.HP		;
	result.Hit		= character.Points.Hit				+	weaponDefinition.Points.Hit		+	character.CombatBonus.Points.Hit	;
	result.Attack	= character.Points.Attack			+	weaponDefinition.Points.Attack	+	character.CombatBonus.Points.Attack	;
	result.Coins	=										weaponDefinition.Points.Coins	+	character.CombatBonus.Points.Coins	;

	return result;
};

bool addItem(SCharacterInventory& adventurer, uint32_t itemIndex);
bool addStatus(SCombatStatus& characterStatus, STATUS_TYPE statusType, uint32_t turnCount);
void rest(CCharacter& adventurerPoints);	// Take a nap and recover HP up to MaxHP.
void showInventory(const SCharacter& adventurer);
