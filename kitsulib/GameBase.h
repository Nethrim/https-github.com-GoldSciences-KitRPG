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

	struct SCharacterPoints
	{
		SEntityPoints		Points;
		SEntityFlags		Flags;

		inline constexpr SCharacterPoints	operator *	(const SEntityPointsMultiplier& other)	const	{ 
			return { Points*other, Flags }; 
		}

		inline constexpr SCharacterPoints	operator +	(const SCharacterPoints& other)				const	{ 
			return { Points+other.Points, Flags|other.Flags };//Effect | other.Effect, Status | other.Status, Tech | other.Tech }; 
		}

		void Print() const
		{
			Points.Print();
			Flags.Print();
		};
	};

	typedef SEntityPoints SEntityPointsSturns;
	struct SCombatBonus
	{
		SEntityPoints		Points				= { };	// these are points that are calculated during combat depending on equipment or item consumption.
		SEntityPointsSturns	TurnsLeftPoints		= { };	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.
		SEntityFlags		Flags				= { };	// these are points that are calculated during combat depending on equipment or item consumption.
		SEntityFlagsTurns	TurnsLeftFlags		= { };	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.

		void				NextTurn() {
			if( 0 < TurnsLeftPoints.LifeMax.Health		)	{	if( 0 == --TurnsLeftPoints.LifeMax.Health		)	Points.LifeMax.Health		= 0;	} else if( 0 > TurnsLeftPoints.LifeMax.Health		)	{	if( 0 == ++TurnsLeftPoints.LifeMax.Health		)	Points.LifeMax.Health		= 0;	} 
			if( 0 < TurnsLeftPoints.LifeMax.Mana		)	{	if( 0 == --TurnsLeftPoints.LifeMax.Mana			)	Points.LifeMax.Mana			= 0;	} else if( 0 > TurnsLeftPoints.LifeMax.Mana			)	{	if( 0 == ++TurnsLeftPoints.LifeMax.Mana			)	Points.LifeMax.Mana			= 0;	} 
			if( 0 < TurnsLeftPoints.LifeMax.Shield		)	{	if( 0 == --TurnsLeftPoints.LifeMax.Shield		)	Points.LifeMax.Shield		= 0;	} else if( 0 > TurnsLeftPoints.LifeMax.Shield		)	{	if( 0 == ++TurnsLeftPoints.LifeMax.Shield		)	Points.LifeMax.Shield		= 0;	} 
//	
			if( 0 < TurnsLeftPoints.LifeCurrent.Health	)	{	if( 0 == --TurnsLeftPoints.LifeCurrent.Health	)	Points.LifeCurrent.Health	= 0;	} else if( 0 > TurnsLeftPoints.LifeCurrent.Health	)	{	if( 0 == ++TurnsLeftPoints.LifeCurrent.Health	)	Points.LifeCurrent.Health	= 0;	} 
			if( 0 < TurnsLeftPoints.LifeCurrent.Mana	)	{	if( 0 == --TurnsLeftPoints.LifeCurrent.Mana		)	Points.LifeCurrent.Mana		= 0;	} else if( 0 > TurnsLeftPoints.LifeCurrent.Mana		)	{	if( 0 == ++TurnsLeftPoints.LifeCurrent.Mana		)	Points.LifeCurrent.Mana		= 0;	} 
			if( 0 < TurnsLeftPoints.LifeCurrent.Shield	)	{	if( 0 == --TurnsLeftPoints.LifeCurrent.Shield	)	Points.LifeCurrent.Shield	= 0;	} else if( 0 > TurnsLeftPoints.LifeCurrent.Shield	)	{	if( 0 == ++TurnsLeftPoints.LifeCurrent.Shield	)	Points.LifeCurrent.Shield	= 0;	} 
//	
			if( 0 < TurnsLeftPoints.Attack.Damage		)	{	if( 0 == --TurnsLeftPoints.Attack.Damage		)	Points.Attack.Damage		= 0;	} else if( 0 > TurnsLeftPoints.Attack.Damage		)	{	if( 0 == ++TurnsLeftPoints.Attack.Damage		)	Points.Attack.Damage		= 0;	} 
			if( 0 < TurnsLeftPoints.Attack.Hit			)	{	if( 0 == --TurnsLeftPoints.Attack.Hit			)	Points.Attack.Hit			= 0;	} else if( 0 > TurnsLeftPoints.Attack.Hit			)	{	if( 0 == ++TurnsLeftPoints.Attack.Hit			)	Points.Attack.Hit			= 0;	} 
			if( 0 < TurnsLeftPoints.Attack.Speed		)	{	if( 0 == --TurnsLeftPoints.Attack.Speed			)	Points.Attack.Speed			= 0;	} else if( 0 > TurnsLeftPoints.Attack.Speed			)	{	if( 0 == ++TurnsLeftPoints.Attack.Speed			)	Points.Attack.Speed			= 0;	} 
			if( 0 < TurnsLeftPoints.Attack.Absorption	)	{	if( 0 == --TurnsLeftPoints.Attack.Absorption	)	Points.Attack.Absorption	= 0;	} else if( 0 > TurnsLeftPoints.Attack.Absorption	)	{	if( 0 == ++TurnsLeftPoints.Attack.Absorption	)	Points.Attack.Absorption	= 0;	} 
			if( 0 < TurnsLeftPoints.Coins				)	{	if( 0 == --TurnsLeftPoints.Coins				)	Points.Coins				= 0;	} else if( 0 > TurnsLeftPoints.Coins				)	{	if( 0 == ++TurnsLeftPoints.Coins				)	Points.Coins				= 0;	} 
//
			if( 0 < TurnsLeftFlags.Effect	.Attack		)	{	if( 0 == --TurnsLeftFlags.Effect	.Attack		)	Flags.Effect	.Attack		= ATTACK_EFFECT_NONE		;} else if( 0 > TurnsLeftFlags.Effect	.Attack		)	{	if( 0 == ++TurnsLeftFlags.Effect	.Attack		)	Flags.Effect	.Attack		= ATTACK_EFFECT_NONE		;	} 
			if( 0 < TurnsLeftFlags.Effect	.Defend		)	{	if( 0 == --TurnsLeftFlags.Effect	.Defend		)	Flags.Effect	.Defend		= DEFEND_EFFECT_NONE		;} else if( 0 > TurnsLeftFlags.Effect	.Defend		)	{	if( 0 == ++TurnsLeftFlags.Effect	.Defend		)	Flags.Effect	.Defend		= DEFEND_EFFECT_NONE		;	} 
			if( 0 < TurnsLeftFlags.Effect	.Passive	)	{	if( 0 == --TurnsLeftFlags.Effect	.Passive	)	Flags.Effect	.Passive	= PASSIVE_EFFECT_NONE		;} else if( 0 > TurnsLeftFlags.Effect	.Passive	)	{	if( 0 == ++TurnsLeftFlags.Effect	.Passive	)	Flags.Effect	.Passive	= PASSIVE_EFFECT_NONE		;	} 
			if( 0 < TurnsLeftFlags.Status	.Inflict	)	{	if( 0 == --TurnsLeftFlags.Status	.Inflict	)	Flags.Status	.Inflict	= COMBAT_STATUS_NONE		;} else if( 0 > TurnsLeftFlags.Status	.Inflict	)	{	if( 0 == ++TurnsLeftFlags.Status	.Inflict	)	Flags.Status	.Inflict	= COMBAT_STATUS_NONE		;	} 
			if( 0 < TurnsLeftFlags.Status	.Immunity	)	{	if( 0 == --TurnsLeftFlags.Status	.Immunity	)	Flags.Status	.Immunity	= COMBAT_STATUS_NONE		;} else if( 0 > TurnsLeftFlags.Status	.Immunity	)	{	if( 0 == ++TurnsLeftFlags.Status	.Immunity	)	Flags.Status	.Immunity	= COMBAT_STATUS_NONE		;	} 
			if( 0 < TurnsLeftFlags.Tech		.Tech		)	{	if( 0 == --TurnsLeftFlags.Tech		.Tech		)	Flags.Tech		.Tech		= ENTITY_TECHNOLOGY_UNKNOWN	;} else if( 0 > TurnsLeftFlags.Tech		.Tech		)	{	if( 0 == ++TurnsLeftFlags.Tech		.Tech		)	Flags.Tech		.Tech		= ENTITY_TECHNOLOGY_UNKNOWN	;	} 
			if( 0 < TurnsLeftFlags.Tech		.Grade		)	{	if( 0 == --TurnsLeftFlags.Tech		.Grade		)	Flags.Tech		.Grade		= ENTITY_GRADE_ILLUSION		;} else if( 0 > TurnsLeftFlags.Tech		.Grade		)	{	if( 0 == ++TurnsLeftFlags.Tech		.Grade		)	Flags.Tech		.Grade		= ENTITY_GRADE_ILLUSION		;	} 
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
		SCharacterResearchGroup		Weapon		= {};
		SCharacterResearchGroup		Armor		= {};
		SCharacterResearchGroup		Profession	= {};
		SCharacterResearchGroup		Vehicle		= {};
	};

	// POD to store all the character data that is not a string or object.
	struct SCharacter
	{
		CHARACTER_TYPE				Type				= CHARACTER_TYPE_UNKNOWN;
		SCharacterPoints			Points				= { {{10,0,0}, {10,0,0}, {50,1,1,1}, 10} };	// These are the base character points.

		SCombatBonus				CombatBonus			= SCombatBonus			();
		SCombatStatus				CombatStatus		= SCombatStatus			();
		SCharacterScore				Score				= SCharacterScore		();	

		SWeapon						CurrentWeapon		= {0,0,1};	// Index, ModifierIndex, Level
		SArmor						CurrentArmor		= {0,0,1};	// Index, ModifierIndex, Level
		SProfession					CurrentProfession	= {0,0,1};	// Index, ModifierIndex, Level
		SVehicle					CurrentVehicle		= {0,0,1};	// Index, ModifierIndex, Level

		SWeapon						MaxWeapon			= {0,0,1};	// Index, ModifierIndex, Level
		SArmor						MaxArmor			= {0,0,1};	// Index, ModifierIndex, Level
		SProfession					MaxProfession		= {0,0,1};	// Index, ModifierIndex, Level
		SVehicle					MaxVehicle			= {0,0,1};	// Index, ModifierIndex, Level

		SInventoryItems				Inventory			= {};
		SInventoryWeapons			Weapons				= {};
		SInventoryArmors			Armors				= {};
		SInventoryProfessions		Professions			= {};
		SInventoryVehicles			Vehicles			= {};

		SCharacterResearch			Researched;

		void						UnloadWeapon		()	{	Weapons		.AddElement(CurrentWeapon		);	CurrentWeapon		= {0,0,1};	};
		void						UnloadArmor			()	{	Armors		.AddElement(CurrentArmor		);	CurrentArmor		= {0,0,1};	};
		void						UnloadProfession	()	{	Professions	.AddElement(CurrentProfession	);	CurrentProfession	= {0,0,1};	};
		void						UnloadVehicle		()	{	Vehicles	.AddElement(CurrentVehicle		);	CurrentVehicle		= {0,0,1};	};
		void						EquipWeapon			(size_t slotIndex);
		void						EquipArmor			(size_t slotIndex);
		void						EquipProfession		(size_t slotIndex);
		void						EquipVehicle		(size_t slotIndex);
		bool						DidLoseTurn() {
			return 0 < CombatStatus.GetStatusTurns(COMBAT_STATUS_STUN	) 
				|| 0 < CombatStatus.GetStatusTurns(COMBAT_STATUS_SLEEP	) 
				|| 0 < CombatStatus.GetStatusTurns(COMBAT_STATUS_FROZEN	);
		};

		constexpr SCharacter() = default;
		constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, SEntityEffect characterEffect, SEntityStatus characterStatus ) 
			:Type				(characterType)
			,Points				({{{maxHP}, {maxHP}, {hitChance, attack}, coins}, {characterEffect, characterStatus}})
			,CombatBonus		(SCombatBonus	())
			,CombatStatus		(SCombatStatus	())
			,Score				(SCharacterScore())
			,CurrentWeapon		({0,0,1})
			,CurrentArmor		({0,0,1})
			,CurrentProfession	({0,0,1})
			,MaxWeapon			({0,0,1})
			,MaxArmor			({0,0,1})
			,MaxProfession		({0,0,1})
			,Inventory			({})
			,Weapons			({})
			,Armors				({})
			,Professions		({})
			,Vehicles			({})
		{};

		int32_t						Save(FILE* fp)	const;
		int32_t						Load(FILE* fp);
	};

	static SCharacterPoints applyMultipliers(const SCharacterPoints& definitionPoints, const SCharacterPoints& modifierPoints, const SEntityPointsMultiplier& multipliers ) {
		SCharacterPoints finalPoints = {};
		finalPoints				= (definitionPoints + modifierPoints)*multipliers;
		return finalPoints;
	}

#pragma pack(pop)

} // namespace

#endif // __GAMEBASE_H__98236498027346892734689273__
