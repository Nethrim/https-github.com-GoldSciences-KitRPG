#include "Inventory.h"
#include "CombatPoints.h"
#include "CombatStatus.h"

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
		SLifePointsMultiplier	MaxLife;
		SLifePointsMultiplier	CurrentLife;
		SCombatPointsMultiplier	Attack;
		double					Coins;

		inline constexpr SCharacterPointsMultipliers	operator *	(const int32_t level)	const	{ 
			return { MaxLife*level, CurrentLife*level, Attack*level, Coins*level }; 
		}
	};

	struct SCharacterEffect
	{
		ATTACK_EFFECT		Attack;
		DEFEND_EFFECT		Defend;
		PASSIVE_EFFECT		Passive;

		inline constexpr SCharacterEffect operator | (const SCharacterEffect& other) const {
			return { (ATTACK_EFFECT)(Attack | other.Attack), (DEFEND_EFFECT)(Defend | other.Defend), (PASSIVE_EFFECT)(Passive | other.Passive) };
		};
	};
	
	struct SCharacterStatus
	{
		COMBAT_STATUS		Inflict;
		COMBAT_STATUS		Immunity;

		inline constexpr SCharacterStatus operator | (const SCharacterStatus& other) const {
			return { (COMBAT_STATUS)(Inflict | other.Inflict), (COMBAT_STATUS)(Immunity | other.Immunity) };
		};
	};

	struct SCharacterGrade
	{
		EQUIP_TECHNOLOGY	Tech;
		GRADE				Level;

		inline constexpr SCharacterGrade operator | (const SCharacterGrade& other) const {
			return { (EQUIP_TECHNOLOGY)(Tech | other.Tech), ::std::max(Level, other.Level) };
		}
	};

	struct SCharacterPoints
	{
		SLifePoints			MaxLife;
		SLifePoints			CurrentLife;
		SCombatPoints		Attack;
		int32_t				Coins;
		SCharacterEffect	Effect;
		SCharacterStatus	Status;
		SCharacterGrade		Tech;

		inline constexpr SCharacterPoints	operator *	(const SCharacterPointsMultipliers& other)	const	{ 
			return { MaxLife*other.MaxLife, CurrentLife*other.CurrentLife, Attack*other.Attack, (int32_t)(Coins*std::max(1.000001, other.Coins)), Effect, Status, Tech }; 
		}
		inline constexpr SCharacterPoints	operator +	(const SCharacterPoints& other)				const	{ 
			return { MaxLife+other.MaxLife, CurrentLife+other.CurrentLife, Attack+other.Attack, Coins+other.Coins, Effect | other.Effect, Status | other.Status, Tech | other.Tech }; 
		}
		void PrintStatusAndEffect() const
		{
			printf("- Flags for Attack Effect   : 0x%.04x.\n"	, (int32_t)	Effect.Attack		);
			printf("- Flags for Defend Effect   : 0x%.04x.\n"	, (int32_t)	Effect.Defend		);
			printf("- Flags for Passive Effect  : 0x%.04x.\n"	, (int32_t)	Effect.Passive	);
			printf("- Flags for Status Inflict  : 0x%.04x.\n"	, (int32_t)	Status.Inflict	);
			printf("- Flags for Status Immunity : 0x%.04x.\n"	, (int32_t)	Status.Immunity	);
		}

		void Print() const
		{
			printf("- Max Life:\n");
			MaxLife.Print();
			printf("- Current Life:\n");
			CurrentLife.Print();
			printf("- Attack Points:\n");
			Attack.Print();
			printf("- Coins                     : %i.\n"		, (int32_t)		Coins				);
			PrintStatusAndEffect();
		};
	};


	typedef SCharacterPoints SBonusTurns;

	struct SCombatBonus
	{
		SCharacterPoints	Points		= { {0, 0, 0}, {0, 0, 0}, {0, 0}, 0, ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_NONE, COMBAT_STATUS_NONE, COMBAT_STATUS_NONE };	// these are points that are calculated during combat depending on equipment or item consumption.
		SBonusTurns			TurnsLeft	= { {0, 0, 0}, {0, 0, 0}, {0, 0}, 0, ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_NONE, COMBAT_STATUS_NONE, COMBAT_STATUS_NONE };	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.

		void				NextTurn() {
			if( 0 >= --TurnsLeft.MaxLife.Health			)	TurnsLeft.MaxLife.Health		=	Points.MaxLife.Health		= 0;
			if( 0 >= --TurnsLeft.MaxLife.Mana		)	TurnsLeft.MaxLife.Mana		=	Points.MaxLife.Mana		= 0;
			if( 0 >= --TurnsLeft.MaxLife.Shield		)	TurnsLeft.MaxLife.Shield	=	Points.MaxLife.Shield	= 0;

			if( 0 >= --TurnsLeft.CurrentLife.Health		)	TurnsLeft.CurrentLife.Health		=	Points.CurrentLife.Health		= 0;
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
		COMBAT_STATUS		Status		[MAX_COMBAT_STATUS]	= {};
		int8_t				TurnsLeft	[MAX_COMBAT_STATUS]	= {};
		int					GetStatusTurns(COMBAT_STATUS status)
		{
			for(uint32_t i=0; i<Count; ++i)
				if(Status[i] & status)
					return TurnsLeft[i];
			return 0;
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

	struct SEquip
	{
		int16_t Index		= 0;
		int16_t Modifier	= 0;
		int16_t Level		= 1;
		inline constexpr SEquip() = default;
		inline constexpr SEquip(int16_t index, int16_t modifier, int16_t level=1)
			: Index		(index		)
			, Modifier	(modifier	)
			, Level		(level		)
		{};
	};

	struct SWeapon : public SEquip {
		using SEquip::SEquip;
	};

	struct SArmor : public SEquip {
		using SEquip::SEquip;
	};

	struct SProfession : public SEquip {
		using SEquip::SEquip;
	};

	template<typename _T, size_t _Size>
	struct SEquipContainer
	{
		uint32_t	Count			= 0;
		_T			Slots[_Size]	= {};

		inline constexpr int32_t	GetMaxSize() const { return _Size; };

		inline bool					AddElement(const _T& element) { 
			if(Count >= _Size)
				return false;
			Slots[Count++]	= element;
			return true; 
		};
		inline bool					RemoveElement(size_t index) { 
			if(index >= Count)
				return false;
			Slots[index]	= Slots[--Count];
			return true; 
		};
		inline int32_t				FindElement(const _T& element) { 
			for(uint32_t i=0, count=std::max((uint32_t)_Size, Count); i<count; i++)
				if(Slots[i] == element)
					return i;
			return -1;
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

	// POD to store all the character data that is not a string or object.
	struct SCharacter
	{
		CHARACTER_TYPE		Type				= CHARACTER_TYPE_UNKNOWN;
		SCharacterPoints	Points				= { {10, 0, 0}, {10, 0,	0}, {50, 1}, 10};	// These are the base character points.
		SCombatBonus		CombatBonus			= {};
		SCombatStatus		CombatStatus		= {};
		SCharacterInventory	Inventory			= {};
		SCharacterScore		Score				= {};	
		SWeapon				CurrentWeapon		= {0,0,1};	// Index, ModifierIndex, Level
		SArmor				CurrentArmor		= {0,0,1};	// Index, ModifierIndex, Level
		SProfession			CurrentProfession	= {0,0,1};	// Index, ModifierIndex, Level

		SWeapon				MaxWeapon			= {0,0,1};	// Index, ModifierIndex, Level
		SArmor				MaxArmor			= {0,0,1};	// Index, ModifierIndex, Level
		SProfession			MaxProfession		= {0,0,1};	// Index, ModifierIndex, Level

		SEquipContainer<SWeapon		, 256>	Weapons		= {};
		SEquipContainer<SArmor		, 256>	Armors		= {};
		SEquipContainer<SProfession	, 256>	Professions	= {};

		SEquipContainer<int16_t, 256>	ResearchedDefinitionsWeapon		= {};
		SEquipContainer<int16_t, 256>	ResearchedDefinitionsArmor		= {};
		SEquipContainer<int16_t, 256>	ResearchedDefinitionsProfession	= {};
		SEquipContainer<int16_t, 256>	ResearchedModifiersWeapon		= {};
		SEquipContainer<int16_t, 256>	ResearchedModifiersArmor		= {};
		SEquipContainer<int16_t, 256>	ResearchedModifiersProfession	= {};

		void				UnloadWeapon		()	{	Weapons.AddElement(CurrentWeapon);	};
		void				UnloadArmor			()	{	Armors.AddElement(CurrentArmor);	};
		void				UnloadProfession	()	{	Professions.AddElement(CurrentProfession);	};
		void				EquipWeapon			(size_t slotIndex);
		void				EquipArmor			(size_t slotIndex);
		void				EquipProfession		(size_t slotIndex);


		constexpr SCharacter() = default;
		constexpr SCharacter(CHARACTER_TYPE characterType, int maxHP, int hitChance, int attack, int coins, ATTACK_EFFECT attackEffect, DEFEND_EFFECT defendEffect, PASSIVE_EFFECT passiveEffect, COMBAT_STATUS inflictStatus, COMBAT_STATUS immunities) 
			:Type				(characterType)
			,Points				({{maxHP, 0, 0}, {maxHP, 0, 0}, {hitChance, attack}, coins, {attackEffect, defendEffect, passiveEffect}, {inflictStatus, immunities}, {}})
			,CombatBonus		({})
			,CombatStatus		({})
			,Inventory			({})
			,Score				({})
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
