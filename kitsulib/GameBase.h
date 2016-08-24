#include "Inventory.h"
#include "CombatPoints.h"
#include "CombatStatus.h"
#include "Entity.h"
#include "EntityPoints.h"

#include <cstdint>
#include <cstdio>
#include <algorithm>

#ifndef __GAMEBASE_H__98236498027346892734689273__
#define __GAMEBASE_H__98236498027346892734689273__

namespace klib
{
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

	struct SCharacterPointsMultipliers
	{
		SLifePointsMultiplier	LifeMax;
		SLifePointsMultiplier	LifeCurrent;
		SAttackPointsMultiplier	Attack;
		double					Coins;

		inline constexpr SCharacterPointsMultipliers	operator *	(const int32_t level)	const {
			return{ LifeMax*level, LifeCurrent*level, Attack*level, Coins*level };
		}
	};

	struct SCharacterPoints
	{
		SLifePoints			LifeMax;
		SLifePoints			LifeCurrent;
		SAttackPoints		Attack;
		int32_t				Coins;
		SEntityFlags		Flags;

		inline constexpr SCharacterPoints	operator *	(const SCharacterPointsMultipliers& other)	const	{ 
			return { LifeMax*other.LifeMax, LifeCurrent*other.LifeCurrent, Attack*other.Attack, (int32_t)(Coins*std::max(1.000001, other.Coins)), Flags }; 
		}

		inline constexpr SCharacterPoints	operator +	(const SCharacterPoints& other)				const	{ 
			return { LifeMax+other.LifeMax, LifeCurrent+other.LifeCurrent, Attack+other.Attack, Coins+other.Coins, Flags|other.Flags };//Effect | other.Effect, Status | other.Status, Tech | other.Tech }; 
		}

		void Print() const
		{
			printf("- Max Life:\n");
			LifeMax.Print();
			printf("- Current Life:\n");
			LifeCurrent.Print();
			printf("- Attack Points:\n");
			Attack.Print();
			printf("- Coins                     : %i.\n"		, (int32_t)		Coins				);
			Flags.Print();
		};
	};


	typedef SCharacterPoints SBonusTurns;

	struct SCombatBonus
	{
		SCharacterPoints	Points		= { {}, {}, {}, 0, {} };	// these are points that are calculated during combat depending on equipment or item consumption.
		SBonusTurns			TurnsLeft	= { {}, {}, {}, 0, {} };	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.

		void				NextTurn() {
			if( 0 < TurnsLeft.LifeMax.Health		)	{	if( 0 == --TurnsLeft.LifeMax.Health		)	Points.LifeMax.Health		= 0;	} else if( 0 > TurnsLeft.LifeMax.Health		)	{	if( 0 == ++TurnsLeft.LifeMax.Health		)	Points.LifeMax.Health		= 0;	} 
			if( 0 < TurnsLeft.LifeMax.Mana			)	{	if( 0 == --TurnsLeft.LifeMax.Mana		)	Points.LifeMax.Mana			= 0;	} else if( 0 > TurnsLeft.LifeMax.Mana		)	{	if( 0 == ++TurnsLeft.LifeMax.Mana		)	Points.LifeMax.Mana			= 0;	} 
			if( 0 < TurnsLeft.LifeMax.Shield		)	{	if( 0 == --TurnsLeft.LifeMax.Shield		)	Points.LifeMax.Shield		= 0;	} else if( 0 > TurnsLeft.LifeMax.Shield		)	{	if( 0 == ++TurnsLeft.LifeMax.Shield		)	Points.LifeMax.Shield		= 0;	} 
//
			if( 0 < TurnsLeft.LifeCurrent.Health	)	{	if( 0 == --TurnsLeft.LifeCurrent.Health	)	Points.LifeCurrent.Health	= 0;	} else if( 0 > TurnsLeft.LifeCurrent.Health	)	{	if( 0 == ++TurnsLeft.LifeCurrent.Health	)	Points.LifeCurrent.Health	= 0;	} 
			if( 0 < TurnsLeft.LifeCurrent.Mana		)	{	if( 0 == --TurnsLeft.LifeCurrent.Mana	)	Points.LifeCurrent.Mana		= 0;	} else if( 0 > TurnsLeft.LifeCurrent.Mana	)	{	if( 0 == ++TurnsLeft.LifeCurrent.Mana	)	Points.LifeCurrent.Mana		= 0;	} 
			if( 0 < TurnsLeft.LifeCurrent.Shield	)	{	if( 0 == --TurnsLeft.LifeCurrent.Shield	)	Points.LifeCurrent.Shield	= 0;	} else if( 0 > TurnsLeft.LifeCurrent.Shield	)	{	if( 0 == ++TurnsLeft.LifeCurrent.Shield	)	Points.LifeCurrent.Shield	= 0;	} 
//
			if( 0 < TurnsLeft.Attack.Damage			)	{	if( 0 == --TurnsLeft.Attack.Damage		)	Points.Attack.Damage		= 0;	} else if( 0 > TurnsLeft.Attack.Damage		)	{	if( 0 == ++TurnsLeft.Attack.Damage		)	Points.Attack.Damage		= 0;	} 
			if( 0 < TurnsLeft.Attack.Hit			)	{	if( 0 == --TurnsLeft.Attack.Hit			)	Points.Attack.Hit			= 0;	} else if( 0 > TurnsLeft.Attack.Hit			)	{	if( 0 == ++TurnsLeft.Attack.Hit			)	Points.Attack.Hit			= 0;	} 
			if( 0 < TurnsLeft.Coins					)	{	if( 0 == --TurnsLeft.Coins				)	Points.Coins				= 0;	} else if( 0 > TurnsLeft.Coins				)	{	if( 0 == ++TurnsLeft.Coins				)	Points.Coins				= 0;	} 
		};
	};

#define MAX_COMBAT_STATUS	16
	struct SCombatStatus
	{
		uint32_t			Count	= 0;
		COMBAT_STATUS		Status		[MAX_COMBAT_STATUS]	= {};
		int8_t				TurnsLeft	[MAX_COMBAT_STATUS]	= {};
		int32_t				GetStatusTurns(const COMBAT_STATUS status)
		{
			int32_t turns=0;
			for(uint32_t i=0; i<Count; ++i)
				if(status == (Status[i] & status))
					turns += TurnsLeft[i];
			return turns;
		};
		void				NextTurn() {
			for(uint32_t i=0; i<Count; ++i) {
				if(0 < TurnsLeft[i] && 0 == --TurnsLeft[i])
				{
					Status		[i]	=	Status		[--Count];
					TurnsLeft	[i]	=	TurnsLeft	[Count];
				}
				else if(0 > TurnsLeft[i] && 0 == ++TurnsLeft[i])
				{
					Status		[i]	=	Status		[--Count];
					TurnsLeft	[i]	=	TurnsLeft	[Count];
				}
			}

		};
	};

	enum CHARACTER_TYPE : int32_t
	{	CHARACTER_TYPE_UNKNOWN
	,	CHARACTER_TYPE_NPC
	,	CHARACTER_TYPE_PLAYER
	,	CHARACTER_TYPE_ENEMY
	,	CHARACTER_TYPE_ALLY
	,	CHARACTER_TYPE_ADMIN
	};


	struct SCharacterResearchGroup
	{
		SEntityContainer<int16_t, 256>	Definitions;
		SEntityContainer<int16_t, 256>	Modifiers;
	};

	struct SCharacterResearch
	{
		SCharacterResearchGroup			Weapon		= {};
		SCharacterResearchGroup			Armor		= {};
		SCharacterResearchGroup			Profession	= {};
	};

	// POD to store all the character data that is not a string or object.
	struct SCharacter
	{
		CHARACTER_TYPE		Type				= CHARACTER_TYPE_UNKNOWN;
		SCharacterPoints	Points				= { {10, 0, 0}, {10, 0,	0}, {50, 1}, 10};	// These are the base character points.
		SCombatBonus		CombatBonus			= SCombatBonus	();
		SCombatStatus		CombatStatus		= SCombatStatus	();
		SCharacterInventory	Inventory			= SCharacterInventory();
		SCharacterScore		Score				= SCharacterScore();	
		SWeapon				CurrentWeapon		= {0,0,1};	// Index, ModifierIndex, Level
		SArmor				CurrentArmor		= {0,0,1};	// Index, ModifierIndex, Level
		SProfession			CurrentProfession	= {0,0,1};	// Index, ModifierIndex, Level

		SWeapon				MaxWeapon			= {0,0,1};	// Index, ModifierIndex, Level
		SArmor				MaxArmor			= {0,0,1};	// Index, ModifierIndex, Level
		SProfession			MaxProfession		= {0,0,1};	// Index, ModifierIndex, Level

		SEntityContainer<SWeapon		, 256>	Weapons		= {};
		SEntityContainer<SArmor			, 256>	Armors		= {};
		SEntityContainer<SProfession	, 256>	Professions	= {};

		SCharacterResearch						Researched;

		void				UnloadWeapon		()	{	Weapons.AddElement(CurrentWeapon);	};
		void				UnloadArmor			()	{	Armors.AddElement(CurrentArmor);	};
		void				UnloadProfession	()	{	Professions.AddElement(CurrentProfession);	};
		void				EquipWeapon			(size_t slotIndex);
		void				EquipArmor			(size_t slotIndex);
		void				EquipProfession		(size_t slotIndex);
		bool				DidLoseTurn() {
			return 0 < CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_STUN) || 0 < CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_SLEEP) || 0 < CombatStatus.GetStatusTurns(klib::COMBAT_STATUS_FROZEN);
		};

		constexpr SCharacter() = default;
		constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SEntityEffect characterEffect, SEntityStatus characterStatus ) 
			:Type				(characterType)
			,Points				({{maxHP}, {maxHP}, {hitChance, attack}, coins, {characterEffect, characterStatus}})
			,CombatBonus		(SCombatBonus	())
			,CombatStatus		(SCombatStatus	())
			,Inventory			(SCharacterInventory())
			,Score				(SCharacterScore())
			,CurrentWeapon		({0,0,1})
			,CurrentArmor		({0,0,1})
			,CurrentProfession	({0,0,1})
			,MaxWeapon			({0,0,1})
			,MaxArmor			({0,0,1})
			,MaxProfession		({0,0,1})
			,Weapons			({})
			,Armors				({})
			,Professions		({})
		{};

		int32_t		Save(FILE* fp)	const;
		int32_t		Load(FILE* fp);
	};

	static SCharacterPoints applyMultipliers(const SCharacterPoints& definitionPoints, const SCharacterPoints& modifierPoints, const SCharacterPointsMultipliers& multipliers ) {
		SCharacterPoints finalPoints = {};
		finalPoints				= (definitionPoints + modifierPoints)*multipliers;
		return finalPoints;
	}

#pragma pack(pop)

} // namespace

#endif // __GAMEBASE_H__98236498027346892734689273__
