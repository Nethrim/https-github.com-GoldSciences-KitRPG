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

struct SLifePoints
{
	int	HP		;
	int	Mana	;
	int	Shield	;

	inline constexpr SLifePoints	operator +	(const SLifePoints& other) const	{ return {HP+other.HP, Mana+other.Mana, Shield+other.Shield}; }
	SLifePoints&					operator +=	(const SLifePoints& other)			{ HP += other.HP; Mana += other.Mana; Shield += other.Shield; return *this; }

	void Print() const
	{
		printf("HP     : %u.\n",	HP		);
		printf("Mana   : %u.\n",	Mana	);
		printf("Shield : %u.\n",	Shield	);
	}
};

struct SCombatPoints
{
	int	Hit		;
	int	Damage	;

	constexpr SCombatPoints	operator +	(const SCombatPoints& other) const	{ return {Hit+other.Hit, Damage+other.Damage}; }
	SCombatPoints&			operator +=	(const SCombatPoints& other)		{ Hit += other.Hit; Damage += other.Damage; return *this; }

	void Print() const
	{
		printf("Hit    : %u.\n",	Hit		);
		printf("Damage : %u.\n",	Damage	);
	}
};

struct SCharacterPoints
{
	SLifePoints		MaxLife;
	SLifePoints		CurrentLife;
	SCombatPoints	Attack;
	int				Coins;

	void Print() const
	{
		printf("\n- Max Life:\n");
		MaxLife.Print();
		printf("\n- Current Life:\n");
		CurrentLife.Print();
		printf("\n- Attack Points:\n");
		Attack.Print();
		printf("\n- Coins: %u.\n",	Coins	);
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
	SCharacterPoints	Points		= { {0, 0, 0}, {0, 0, 0}, {0, 0}, 0};	// these are points that are calculated during combat depending on equipment or item consumption.
	SBonusTurns			TurnsLeft	= { {0, 0, 0}, {0, 0, 0}, {0, 0}, 0};	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.

	void				NextTurn() {
		if( 0 >= --TurnsLeft.MaxLife.HP			)	TurnsLeft.MaxLife.HP		=	Points.MaxLife.HP		= 0;
		if( 0 >= --TurnsLeft.MaxLife.Mana		)	TurnsLeft.MaxLife.Mana		=	Points.MaxLife.Mana		= 0;
		if( 0 >= --TurnsLeft.MaxLife.Shield		)	TurnsLeft.MaxLife.Shield	=	Points.MaxLife.Shield	= 0;

		if( 0 >= --TurnsLeft.CurrentLife.HP		)	TurnsLeft.CurrentLife.HP		=	Points.CurrentLife.HP		= 0;
		if( 0 >= --TurnsLeft.CurrentLife.Mana	)	TurnsLeft.CurrentLife.Mana		=	Points.CurrentLife.Mana		= 0;
		if( 0 >= --TurnsLeft.CurrentLife.Shield	)	TurnsLeft.CurrentLife.Shield	=	Points.CurrentLife.Shield	= 0;

		if( 0 >= --TurnsLeft.Attack.Damage		)	TurnsLeft.Attack.Damage		=	Points.Attack.Damage	= 0;
		if( 0 >= --TurnsLeft.Attack.Hit			)	TurnsLeft.Attack.Hit		=	Points.Attack.Hit		= 0;
		if( 0 >= --TurnsLeft.Coins				)	TurnsLeft.Coins				=	Points.Coins			= 0;
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
,	STATUS_TYPE_FREEZE		= 0x20
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
,	CHARACTER_TYPE_ADMIN
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
	SCharacterPoints	Points			= { {10, 0, 0}, {10, 0,	0}, {50, 1}, 10};	// These are the base character points.
	SCombatBonus		CombatBonus		= {};
	SCombatStatus		CombatStatus	= {};
	SCharacterInventory	Inventory		= {};
	SCharacterScore		Score			= {};	
	SWeapon				Weapon			= {0,0,1};	// Index, ModifierIndex, Level
	SArmor				Armor			= {0,0,1};	// Index, ModifierIndex, Level
	SProfession			Profession		= {0,0,1};	// Index, ModifierIndex, Level

	constexpr SCharacter() = default;
	constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins) 
		:Type			(characterType)
		,Points			({{maxHP, 0, 0}, {maxHP, 0, 0}, {hitChance, attack}, coins})
		,CombatBonus	({})
		,CombatStatus	({})
		,Inventory		({})
		,Score			({})
		,Weapon			({0,0,1})
		,Armor			({0,0,1})
		,Profession		({0,0,1})
		//,Shield			(0)
	{};

	int		Save(FILE* fp)	const;
	int		Load(FILE* fp);
};

#pragma pack(pop)

#endif // __GAMEBASE_H__98236498027346892734689273__
