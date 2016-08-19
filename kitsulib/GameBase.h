#include "Inventory.h"
#include "CombatPoints.h"

#include <cstdint>
#include <cstdio>
#include <algorithm>

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

enum ATTACK_EFFECT : uint16_t
{	ATTACK_EFFECT_NONE
,	ATTACK_EFFECT_LEECH	= 0x01
,	ATTACK_EFFECT_STEAL	= 0x02
};

enum DEFEND_EFFECT : uint16_t
{	DEFEND_EFFECT_NONE			= 0
,	DEFEND_EFFECT_REFLECT		= 0x01
,	DEFEND_EFFECT_IMPENETRABLE	= 0x04
};

enum PASSIVE_EFFECT : uint16_t
{	PASSIVE_EFFECT_NONE			= 0
,	PASSIVE_EFFECT_RECHARGE		= 0x02
};

struct SCharacterPoints
{
	SLifePoints		MaxLife;
	SLifePoints		CurrentLife;
	SCombatPoints	Attack;
	int32_t			Coins;
	ATTACK_EFFECT	AttackEffect;
	DEFEND_EFFECT	DefendEffect;
	PASSIVE_EFFECT	PassiveEffect;
	STATUS_TYPE		StatusInflict;
	STATUS_TYPE		StatusImmunity;

	inline constexpr SCharacterPoints	operator *	(const SCharacterPointsMultipliers& other)	const	{ 
		return { MaxLife*other.MaxLife, CurrentLife*other.CurrentLife, Attack*other.Attack, (int32_t)(Coins*std::max(1.000001, other.Coins)), AttackEffect, DefendEffect, PassiveEffect, StatusInflict, StatusImmunity }; 
	}
	inline constexpr SCharacterPoints	operator +	(const SCharacterPoints& other)				const	{ 
		return { MaxLife+other.MaxLife, CurrentLife+other.CurrentLife, Attack+other.Attack, Coins+other.Coins, (ATTACK_EFFECT)(AttackEffect | other.AttackEffect), (DEFEND_EFFECT)(DefendEffect | other.DefendEffect), (PASSIVE_EFFECT)(PassiveEffect | other.PassiveEffect), (STATUS_TYPE)(StatusInflict | other.StatusInflict), (STATUS_TYPE)(StatusImmunity | other.StatusImmunity) }; 
	}
	void Print() const
	{
		printf("- Max Life:\n");
		MaxLife.Print();
		printf("- Current Life:\n");
		CurrentLife.Print();
		printf("- Attack Points:\n");
		Attack.Print();
		printf("- Coins: %i.\n",	Coins	);
	};
};


typedef SCharacterPoints SBonusTurns;

struct SCombatBonus
{
	SCharacterPoints	Points		= { {0, 0, 0}, {0, 0, 0}, {0, 0}, 0, ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_NONE, STATUS_TYPE_NONE, STATUS_TYPE_NONE };	// these are points that are calculated during combat depending on equipment or item consumption.
	SBonusTurns			TurnsLeft	= { {0, 0, 0}, {0, 0, 0}, {0, 0}, 0, ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_NONE, STATUS_TYPE_NONE, STATUS_TYPE_NONE };	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.

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


enum CHARACTER_TYPE : uint32_t
{	CHARACTER_TYPE_UNKNOWN
,	CHARACTER_TYPE_NPC
,	CHARACTER_TYPE_PLAYER
,	CHARACTER_TYPE_ENEMY
,	CHARACTER_TYPE_ALLY
,	CHARACTER_TYPE_ADMIN
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

static SCharacterPoints applyMultipliers(const SCharacterPoints& definitionPoints, const SCharacterPoints& modifierPoints, const SCharacterPointsMultipliers& multipliers ) {
	SCharacterPoints finalPoints = {};
	finalPoints				= (definitionPoints + modifierPoints)*multipliers;
	return finalPoints;
}

#pragma pack(pop)

#endif // __GAMEBASE_H__98236498027346892734689273__
