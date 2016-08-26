#include "EntityHelper.h"
#include "EntityImpl.h"

#ifndef __CHARACTERPOINTS_H__0298734092864380972346234__
#define __CHARACTERPOINTS_H__0298734092864380972346234__

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

	typedef SEntityPoints SEntityPointsSturns;
	struct SCombatBonus
	{
		SEntityPoints		Points				= { };	// these are points that are calculated during combat depending on equipment or item consumption.
		SEntityPointsSturns	TurnsLeftPoints		= { };	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.
		SEntityFlags		Flags				= { };	// these are points that are calculated during combat depending on equipment or item consumption.
		SEntityFlagsTurns	TurnsLeftFlags		= { };	// these are the amount of turns for which each bonus is valid. On each turn it should decrease by one and clear the bonus to zero when this counter reaches zero.

		void				NextTurn() {
			if( 0 < TurnsLeftPoints.LifeMax.Health				)	{	if( 0 == --TurnsLeftPoints.LifeMax.Health				)	Points.LifeMax.Health				= 0;	} else if( 0 > TurnsLeftPoints.LifeMax.Health					)	{	if( 0 == ++TurnsLeftPoints.LifeMax.Health				)	Points.LifeMax.Health				= 0;	} 
			if( 0 < TurnsLeftPoints.LifeMax.Mana				)	{	if( 0 == --TurnsLeftPoints.LifeMax.Mana					)	Points.LifeMax.Mana					= 0;	} else if( 0 > TurnsLeftPoints.LifeMax.Mana						)	{	if( 0 == ++TurnsLeftPoints.LifeMax.Mana					)	Points.LifeMax.Mana					= 0;	} 
			if( 0 < TurnsLeftPoints.LifeMax.Shield				)	{	if( 0 == --TurnsLeftPoints.LifeMax.Shield				)	Points.LifeMax.Shield				= 0;	} else if( 0 > TurnsLeftPoints.LifeMax.Shield					)	{	if( 0 == ++TurnsLeftPoints.LifeMax.Shield				)	Points.LifeMax.Shield				= 0;	} 
//	
			if( 0 < TurnsLeftPoints.LifeCurrent.Health			)	{	if( 0 == --TurnsLeftPoints.LifeCurrent.Health			)	Points.LifeCurrent.Health			= 0;	} else if( 0 > TurnsLeftPoints.LifeCurrent.Health				)	{	if( 0 == ++TurnsLeftPoints.LifeCurrent.Health			)	Points.LifeCurrent.Health			= 0;	} 
			if( 0 < TurnsLeftPoints.LifeCurrent.Mana			)	{	if( 0 == --TurnsLeftPoints.LifeCurrent.Mana				)	Points.LifeCurrent.Mana				= 0;	} else if( 0 > TurnsLeftPoints.LifeCurrent.Mana					)	{	if( 0 == ++TurnsLeftPoints.LifeCurrent.Mana				)	Points.LifeCurrent.Mana				= 0;	} 
			if( 0 < TurnsLeftPoints.LifeCurrent.Shield			)	{	if( 0 == --TurnsLeftPoints.LifeCurrent.Shield			)	Points.LifeCurrent.Shield			= 0;	} else if( 0 > TurnsLeftPoints.LifeCurrent.Shield				)	{	if( 0 == ++TurnsLeftPoints.LifeCurrent.Shield			)	Points.LifeCurrent.Shield			= 0;	} 
//	
			if( 0 < TurnsLeftPoints.Attack.Hit					)	{	if( 0 == --TurnsLeftPoints.Attack.Hit					)	Points.Attack.Hit					= 0;	} else if( 0 > TurnsLeftPoints.Attack.Hit						)	{	if( 0 == ++TurnsLeftPoints.Attack.Hit					)	Points.Attack.Hit					= 0;	} 
			if( 0 < TurnsLeftPoints.Attack.Damage				)	{	if( 0 == --TurnsLeftPoints.Attack.Damage				)	Points.Attack.Damage				= 0;	} else if( 0 > TurnsLeftPoints.Attack.Damage					)	{	if( 0 == ++TurnsLeftPoints.Attack.Damage				)	Points.Attack.Damage				= 0;	} 

			if( 0 < TurnsLeftPoints.Attack.Speed.Attack			)	{	if( 0 == --TurnsLeftPoints.Attack.Speed.Attack			)	Points.Attack.Speed.Attack			= 0;	} else if( 0 > TurnsLeftPoints.Attack.Speed.Attack				)	{	if( 0 == ++TurnsLeftPoints.Attack.Speed.Attack			)	Points.Attack.Speed.Attack			= 0;	} 
			if( 0 < TurnsLeftPoints.Attack.Speed.Movement		)	{	if( 0 == --TurnsLeftPoints.Attack.Speed.Movement		)	Points.Attack.Speed.Movement		= 0;	} else if( 0 > TurnsLeftPoints.Attack.Speed.Movement			)	{	if( 0 == ++TurnsLeftPoints.Attack.Speed.Movement		)	Points.Attack.Speed.Movement		= 0;	} 
			if( 0 < TurnsLeftPoints.Attack.Speed.Reflexes		)	{	if( 0 == --TurnsLeftPoints.Attack.Speed.Reflexes		)	Points.Attack.Speed.Reflexes		= 0;	} else if( 0 > TurnsLeftPoints.Attack.Speed.Reflexes			)	{	if( 0 == ++TurnsLeftPoints.Attack.Speed.Reflexes		)	Points.Attack.Speed.Reflexes		= 0;	} 

			if( 0 < TurnsLeftPoints.Attack.Absorption			)	{	if( 0 == --TurnsLeftPoints.Attack.Absorption			)	Points.Attack.Absorption			= 0;	} else if( 0 > TurnsLeftPoints.Attack.Absorption				)	{	if( 0 == ++TurnsLeftPoints.Attack.Absorption			)	Points.Attack.Absorption			= 0;	} 

			if( 0 < TurnsLeftPoints.Attack.ExtraDamage.Health	)	{	if( 0 == --TurnsLeftPoints.Attack.ExtraDamage.Health	)	Points.Attack.ExtraDamage.Health	= 0;	} else if( 0 > TurnsLeftPoints.Attack.ExtraDamage.Health		)	{	if( 0 == ++TurnsLeftPoints.Attack.ExtraDamage.Health	)	Points.Attack.ExtraDamage.Health	= 0;	} 
			if( 0 < TurnsLeftPoints.Attack.ExtraDamage.Mana		)	{	if( 0 == --TurnsLeftPoints.Attack.ExtraDamage.Mana		)	Points.Attack.ExtraDamage.Mana		= 0;	} else if( 0 > TurnsLeftPoints.Attack.ExtraDamage.Mana			)	{	if( 0 == ++TurnsLeftPoints.Attack.ExtraDamage.Mana		)	Points.Attack.ExtraDamage.Mana		= 0;	} 
			if( 0 < TurnsLeftPoints.Attack.ExtraDamage.Shield	)	{	if( 0 == --TurnsLeftPoints.Attack.ExtraDamage.Shield	)	Points.Attack.ExtraDamage.Shield	= 0;	} else if( 0 > TurnsLeftPoints.Attack.ExtraDamage.Shield		)	{	if( 0 == ++TurnsLeftPoints.Attack.ExtraDamage.Shield	)	Points.Attack.ExtraDamage.Shield	= 0;	} 
			if( 0 < TurnsLeftPoints.Coins						)	{	if( 0 == --TurnsLeftPoints.Coins						)	Points.Coins						= 0;	} else if( 0 > TurnsLeftPoints.Coins							)	{	if( 0 == ++TurnsLeftPoints.Coins						)	Points.Coins						= 0;	} 
//
			if( 0 < TurnsLeftFlags.Effect	.Attack				)	{	if( 0 == --TurnsLeftFlags.Effect	.Attack				)	Flags.Effect	.Attack		= ATTACK_EFFECT_NONE		;} else if( 0 > TurnsLeftFlags.Effect	.Attack		)	{	if( 0 == ++TurnsLeftFlags.Effect	.Attack		)	Flags.Effect	.Attack		= ATTACK_EFFECT_NONE		;	} 
			if( 0 < TurnsLeftFlags.Effect	.Defend				)	{	if( 0 == --TurnsLeftFlags.Effect	.Defend				)	Flags.Effect	.Defend		= DEFEND_EFFECT_NONE		;} else if( 0 > TurnsLeftFlags.Effect	.Defend		)	{	if( 0 == ++TurnsLeftFlags.Effect	.Defend		)	Flags.Effect	.Defend		= DEFEND_EFFECT_NONE		;	} 
			if( 0 < TurnsLeftFlags.Effect	.Passive			)	{	if( 0 == --TurnsLeftFlags.Effect	.Passive			)	Flags.Effect	.Passive	= PASSIVE_EFFECT_NONE		;} else if( 0 > TurnsLeftFlags.Effect	.Passive	)	{	if( 0 == ++TurnsLeftFlags.Effect	.Passive	)	Flags.Effect	.Passive	= PASSIVE_EFFECT_NONE		;	} 
			if( 0 < TurnsLeftFlags.Status	.Inflict			)	{	if( 0 == --TurnsLeftFlags.Status	.Inflict			)	Flags.Status	.Inflict	= COMBAT_STATUS_NONE		;} else if( 0 > TurnsLeftFlags.Status	.Inflict	)	{	if( 0 == ++TurnsLeftFlags.Status	.Inflict	)	Flags.Status	.Inflict	= COMBAT_STATUS_NONE		;	} 
			if( 0 < TurnsLeftFlags.Status	.Immunity			)	{	if( 0 == --TurnsLeftFlags.Status	.Immunity			)	Flags.Status	.Immunity	= COMBAT_STATUS_NONE		;} else if( 0 > TurnsLeftFlags.Status	.Immunity	)	{	if( 0 == ++TurnsLeftFlags.Status	.Immunity	)	Flags.Status	.Immunity	= COMBAT_STATUS_NONE		;	} 
			if( 0 < TurnsLeftFlags.Tech		.Tech				)	{	if( 0 == --TurnsLeftFlags.Tech		.Tech				)	Flags.Tech		.Tech		= ENTITY_TECHNOLOGY_UNKNOWN	;} else if( 0 > TurnsLeftFlags.Tech		.Tech		)	{	if( 0 == ++TurnsLeftFlags.Tech		.Tech		)	Flags.Tech		.Tech		= ENTITY_TECHNOLOGY_UNKNOWN	;	} 
			if( 0 < TurnsLeftFlags.Tech		.Grade				)	{	if( 0 == --TurnsLeftFlags.Tech		.Grade				)	Flags.Tech		.Grade		= ENTITY_GRADE_ILLUSION		;} else if( 0 > TurnsLeftFlags.Tech		.Grade		)	{	if( 0 == ++TurnsLeftFlags.Tech		.Grade		)	Flags.Tech		.Grade		= ENTITY_GRADE_ILLUSION		;	} 
		};
	};

#define MAX_COMBAT_STATUS	16
	struct SCombatStatus
	{
		uint32_t			Count	= 0;
		COMBAT_STATUS		Status		[MAX_COMBAT_STATUS]	= {};
		int8_t				TurnsLeft	[MAX_COMBAT_STATUS]	= {};

		int32_t				GetStatusTurns(const COMBAT_STATUS status){
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

	bool addStatus(SCombatStatus& characterStatus, COMBAT_STATUS statusType, int32_t turnCount);

	enum CHARACTER_TYPE : int32_t
	{	CHARACTER_TYPE_UNKNOWN
	,	CHARACTER_TYPE_PROP
	,	CHARACTER_TYPE_EQUIP
	,	CHARACTER_TYPE_NPC
	,	CHARACTER_TYPE_PLAYER
	,	CHARACTER_TYPE_ENEMY
	,	CHARACTER_TYPE_ALLY
	,	CHARACTER_TYPE_ADMIN
	};
#pragma pack(pop)
}

#endif __CHARACTERPOINTS_H__0298734092864380972346234__
