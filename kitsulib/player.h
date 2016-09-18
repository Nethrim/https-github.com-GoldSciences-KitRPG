#include "Character.h"

#ifndef __PLAYER_H__98213640926348273649827364987234698234__
#define __PLAYER_H__98213640926348273649827364987234698234__

namespace klib
{
	// Squads should be indices to the army
	struct SSquad {
#define MAX_AGENT_SLOTS 12
		static const int32_t		Size = MAX_AGENT_SLOTS;

		int16_t						Agents			[MAX_AGENT_SLOTS] = {-1, -1, -1, -1, -1, -1, -1, -1};
		int16_t						TargetAgents	[MAX_AGENT_SLOTS] = {-1, -1, -1, -1, -1, -1, -1, -1};
		SCellCoord					TargetPositions	[MAX_AGENT_SLOTS] = {};
		int32_t						MovesLeft		[MAX_AGENT_SLOTS] = {};
	
		inline void					Clear(int32_t index) {
			if(index == -1)
				memset(Agents, -1, sizeof(int16_t)*klib::size(Agents));
			else
				Agents[index] = -1;
		};
	};

	struct SPlayerSelection	// can be AI or human.
	{
		int16_t						PlayerSquad	;
		int16_t						PlayerUnit	;
		int16_t						TargetSquad	;
		int16_t						TargetUnit	;
	};

	struct SPlayer	// can be AI or human.
	{
		int32_t						Money				= 25000;
		SCharacterResearch			CompletedResearch	= {};
		SCharacterEquip				MaxResearch			= {};
		SCharacterInventory			Inventory			= {};
		SSquad						Squad				= SSquad();
		int32_t						Team				= 0;
		SPlayerSelection			Selection			= {0, 0, -1, -1};

		::std::vector<CCharacter>	Army				= {};
		::std::string				Name				= "Kasparov";

		bool						IsAlive()	const
		{
			for(size_t iAgent = 0; iAgent < size(Squad.Agents); iAgent++)
				if(Squad.Agents[iAgent] != -1 && Army[Squad.Agents[iAgent]].Points.LifeCurrent.Health > 0)
					return true;

			return false;
		}

		bool						SelectNextAgent()
		{
			uint32_t count = 0;
			Selection.PlayerUnit = (Selection.PlayerUnit + 1) % size(Squad.Agents);
			while(Squad.Agents[Selection.PlayerUnit] == -1 || Army[Squad.Agents[Selection.PlayerUnit]].Points.LifeCurrent.Health <= 0) 
			{
				Selection.PlayerUnit = (Selection.PlayerUnit + 1) % size(Squad.Agents);
				if(++count == size(Squad.Agents))
					return false;
			}
			return true;
		};


		bool						SelectPreviousAgent()
		{
			uint32_t count = 0;

			--Selection.PlayerUnit;
			if(Selection.PlayerUnit < 0) 
				Selection.PlayerUnit = ((int16_t)size(Squad.Agents))-1;

			while(Squad.Agents[Selection.PlayerUnit] == -1 || Army[Squad.Agents[Selection.PlayerUnit]].Points.LifeCurrent.Health <= 0)
			{
				--Selection.PlayerUnit;
				if(Selection.PlayerUnit < 0) 
					Selection.PlayerUnit = ((int16_t)size(Squad.Agents))-1;

				if(++count == size(Squad.Agents))
					return false;
			}
			return true;
		}
	};
} // namespace

#endif // __PLAYER_H__98213640926348273649827364987234698234__
