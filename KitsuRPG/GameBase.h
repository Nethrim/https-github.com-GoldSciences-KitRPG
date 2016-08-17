#include <cstdint>
#include <cstdio>

#ifndef __GAMEBASE_H__98236498027346892734689273__
#define __GAMEBASE_H__98236498027346892734689273__

#pragma pack(push, 1)

// This struct holds a counter for every action that we want to record mostly for score purposes.
struct SCharacterScore 
{
	uint64_t BattlesWon			= 0;
	uint64_t BattlesLost		= 0;
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

struct SCharacterPoints
{
	int	MaxHP	;
	int	HP		;
	int	Hit		;
	int	Attack	;
	int	Coins	;

	void Print()
	{
		printf("MaxHP   : %u.\n",	MaxHP	);
		printf("HP      : %u.\n",	HP		);
		printf("Hit     : %u.\n",	Hit		);
		printf("Attack  : %u.\n",	Attack	);
		printf("Coins   : %u.\n",	Coins	);
	};
};

struct SInventorySlot
{
	uint32_t	ItemIndex;
	uint32_t	ItemCount;
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

#define MAX_STATUS_COUNT 8
enum STATUS_TYPE : uint16_t
{	STATUS_TYPE_NONE		= 0x00
,	STATUS_TYPE_BLIND		= 0x01
,	STATUS_TYPE_STUN		= 0x02
,	STATUS_TYPE_BLEEDING	= 0x04
,	STATUS_TYPE_BURN		= 0x08
,	STATUS_TYPE_POISON		= 0x10
};

#define MAX_COMBAT_STATUS	16

struct SCombatStatus
{
	uint32_t			Count	= 0;
	STATUS_TYPE			Status		[MAX_COMBAT_STATUS]	= {};
	int8_t				TurnsLeft	[MAX_COMBAT_STATUS]	= {};
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

#define MAX_INVENTORY_SLOTS 128

struct SCharacterInventory
{
	uint32_t			ItemCount					= 0;
	SInventorySlot		Slots[MAX_INVENTORY_SLOTS]	= {};
};

enum CHARACTER_TYPE : uint32_t
{	CHARACTER_TYPE_UNKNOWN
,	CHARACTER_TYPE_NPC
,	CHARACTER_TYPE_PLAYER
,	CHARACTER_TYPE_ENEMY
,	CHARACTER_TYPE_ALLY
};

enum PROFESSION
{	PROFESSION_CIVILIAN
,	PROFESSION_SWORDMAN
,	PROFESSION_ARCHER
,	PROFESSION_MAGE
};

struct SWeapon
{
	uint16_t Index;
	uint16_t Modifier;
	uint16_t Level;
};

struct SArmor
{
	uint16_t Index;
	uint16_t Modifier;
	uint16_t Level;
};

struct SProfession
{
	uint16_t Index;
	uint16_t Modifier;
	uint16_t Level;
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
	SWeapon				Weapon			= {0,0,1};	// Index, ModifierIndex, Level
	SArmor				Armor			= {0,0,1};	// Index, ModifierIndex, Level
	SProfession			Profession		= {0,0,1};	// Index, ModifierIndex, Level
	int32_t				Shield			= 0;	// Shield can be acquired from armor primarily but also from items and weapons.

	constexpr SCharacter() = default;
	constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins) 
		:Type			(characterType)
		,Points			({maxHP, maxHP, hitChance, attack, coins})
		,CombatBonus	({})
		,CombatStatus	({})
		,Inventory		({})
		,Score			({})
		,Weapon			({0,0,1})
		,Armor			({0,0,1})
		,Profession		({0,0,1})
		,Shield			(0)
	{};

	int		Save(FILE* fp)	const;
	int		Load(FILE* fp);
};

#pragma pack(pop)

#endif // __GAMEBASE_H__98236498027346892734689273__
