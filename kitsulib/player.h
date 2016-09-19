#include "Character.h"
#include "CharacterTile.h"
#include "genum.h"

#ifndef __PLAYER_H__98213640926348273649827364987234698234__
#define __PLAYER_H__98213640926348273649827364987234698234__

namespace klib
{
	enum PLAYER_ANTAGONISM	: uint8_t
	{
		PLAYER_ANTAGONISM_USER		= 0,
		PLAYER_ANTAGONISM_ENEMY		= 1,
		PLAYER_ANTAGONISM_NEUTRAL	= 2,
		PLAYER_ANTAGONISM_ALLY		= 3,
	};

	// Squads should be indices to the army
	struct SSquad 
	{
#define MAX_AGENT_SLOTS 12
		static const int32_t		Size = MAX_AGENT_SLOTS;

		int16_t						Agents			[MAX_AGENT_SLOTS] = {-1, -1, -1, -1, -1, -1, -1, -1};
		SCharacterTile				TargetAgents	[MAX_AGENT_SLOTS] = {-1, -1, -1, -1, -1, -1, -1, -1};
		SCellCoord					TargetPositions	[MAX_AGENT_SLOTS] = {};
		int32_t						MovesLeft		[MAX_AGENT_SLOTS] = {};
	
		inline void					Clear(int32_t index) {
			if(index == -1)
				memset(Agents, -1, sizeof(int16_t)*ktools::size(Agents));
			else
				Agents[index] = -1;
		};
	};

#pragma pack(push, 1)
	struct SPlayerSelection	// can be AI or human.
	{
		int16_t						PlayerSquad	;
		int16_t						PlayerUnit	;
		int16_t						TargetSquad	;
		int16_t						TargetUnit	;
	};
#pragma pack(pop)

	GDEFINE_ENUM_TYPE(PLAYER_PROPERTY, uint8_t);	// I added these so network messages can talk about the player members
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 1, MONEY				);
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 2, RESEARCH_COMPLETED	);
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 3, RESEARCH_MAX			);
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 4, INVENTORY			);
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 5, SQUAD				);
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 6, TEAM					);
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 7, SELECTION			);
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 8, ARMY					);
	GDEFINE_ENUM_VALUE(PLAYER_PROPERTY, 9, NAME					);

	struct SPlayer	// can be AI or human.
	{
		int32_t						Money				= 25000;
		SCharacterEquip				MaxResearch			= {};
		SCharacterGoods				Goods				= {};
		SSquad						Squad				= SSquad();
		int32_t						Team				= 0;
		SPlayerSelection			Selection			= {0, 0, -1, -1};
		::std::vector<CCharacter>	Army				= {};
		::god::glabel				Name				= "Kasparov";

		bool						IsAlive()	const
		{
			for(size_t iAgent = 0; iAgent < ktools::size(Squad.Agents); iAgent++)
				if(Squad.Agents[iAgent] != -1 && Army[Squad.Agents[iAgent]].Points.LifeCurrent.Health > 0)
					return true;

			return false;
		}

		bool						SelectNextAgent()
		{
			uint32_t count = 0;
			Selection.PlayerUnit = (Selection.PlayerUnit + 1) % ktools::size(Squad.Agents);
			while(Squad.Agents[Selection.PlayerUnit] == -1 || Army[Squad.Agents[Selection.PlayerUnit]].Points.LifeCurrent.Health <= 0) 
			{
				Selection.PlayerUnit = (Selection.PlayerUnit + 1) % ktools::size(Squad.Agents);
				if(++count == ktools::size(Squad.Agents))
					return false;
			}
			return true;
		};


		bool						SelectPreviousAgent()
		{
			uint32_t count = 0;

			--Selection.PlayerUnit;
			if(Selection.PlayerUnit < 0) 
				Selection.PlayerUnit = ((int16_t)ktools::size(Squad.Agents))-1;

			while(Squad.Agents[Selection.PlayerUnit] == -1 || Army[Squad.Agents[Selection.PlayerUnit]].Points.LifeCurrent.Health <= 0)
			{
				--Selection.PlayerUnit;
				if(Selection.PlayerUnit < 0) 
					Selection.PlayerUnit = ((int16_t)ktools::size(Squad.Agents))-1;

				if(++count == ktools::size(Squad.Agents))
					return false;
			}
			return true;
		}
	};
} // namespace

#endif // __PLAYER_H__98213640926348273649827364987234698234__
