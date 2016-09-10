#include "CombatPoints.h"
#include "CombatStatus.h"

#ifndef __ENTITYPOINTS_H__9238409723640921876409234__
#define __ENTITYPOINTS_H__9238409723640921876409234__

namespace klib
{
#pragma pack(push, 1)

	struct SEntityPointsMultiplier 
	{
		SLifePointsMultiplier		LifeMax				;
		SLifePointsMultiplier		LifeCurrent			;
		SAttackPointsMultiplier		Attack				;
		double						Coins				;
		double						PriceBuy			;
		double						CostMaintenance		;

		inline constexpr SEntityPointsMultiplier	operator *	(const int32_t level)	const	{ 
			return { LifeMax*level, LifeCurrent*level, Attack*level, Coins*level, PriceBuy*level, CostMaintenance*level }; 
		}

		void Print() const
		{
			printf("- Max Life:\n");
			LifeMax.Print();
			printf("- Current Life:\n");
			LifeCurrent.Print();
			printf("- Attack Points:\n");
			Attack.Print();
			printf("- Coins...........: %f.\n", Coins);
			printf("- Buy Price.......: %f.\n", PriceBuy);
			printf("- Maintenace Cost.: %f.\n", CostMaintenance);
		}
	};

	struct SEntityPoints 
	{
		SLifePoints		LifeMax			;
		SLifePoints		LifeCurrent		;
		SAttackPoints	Attack			;
		int32_t			Coins			;
		int32_t			PriceBuy		;
		int32_t			CostMaintenance	;
	
		void Print() const
		{
			printf("- Max Life:\n");
			LifeMax.Print();
			printf("- Current Life:\n");
			LifeCurrent.Print();
			printf("- Attack Points:\n");
			Attack.Print();
			printf("- Coins.............: %i.\n", (int32_t)Coins);
			printf("- Buy Price.........: %i.\n", (int32_t)PriceBuy);
			printf("- Maintenance Cost..: %i.\n", (int32_t)CostMaintenance);
		}

		inline constexpr SEntityPoints	operator +	(const SEntityPoints& other)				const	{ 
			return { LifeMax+other.LifeMax, LifeCurrent+other.LifeCurrent, Attack+other.Attack, Coins+other.Coins, PriceBuy+other.PriceBuy, CostMaintenance+other.CostMaintenance };
		}

		inline constexpr SEntityPoints	operator -	(const SEntityPoints& other)				const	{ 
			return { LifeMax-other.LifeMax, LifeCurrent-other.LifeCurrent, Attack-other.Attack, Coins-other.Coins, PriceBuy-other.PriceBuy, CostMaintenance-other.CostMaintenance };
		}

		inline constexpr SEntityPoints		operator *	(const SEntityPointsMultiplier& other)	const	{ 
			return	{ LifeMax					*	other.LifeMax		
					, LifeCurrent				*	other.LifeCurrent	
					, Attack					*	other.Attack	
					, (int32_t)(Coins			*	other.Coins)	
					, (int32_t)(PriceBuy		*	other.PriceBuy)
					, (int32_t)(CostMaintenance	*	other.CostMaintenance)
					};
		}
	};

	struct SEntityFlags {
		SEntityEffect	Effect;
		SEntityStatus	Status;
		SEntityGrade	Tech;

		inline constexpr SEntityFlags operator | (const SEntityFlags& other) const {
			return { Effect | other.Effect, Status | other.Status, Tech | other.Tech };
		};

		void Print() const
		{
			Effect.Print();
			Status.Print();
			Tech.Print();
		}
	};

	struct SEntityCombo
	{
		SEntityPoints	Points	;
		SEntityFlags	Flags	;

		inline constexpr SEntityCombo	operator *	(const SEntityPointsMultiplier& other)	const	{ 
			return { Points*other, Flags }; 
		}

		inline constexpr SEntityCombo	operator +	(const SEntityCombo& other)				const	{ 
			return { Points+other.Points,  Flags|other.Flags };//Effect | other.Effect, Status | other.Status, Tech | other.Tech }; 
		}

		void Print() const
		{
			printf("- Status Points:\n");
			Points.Print();
			printf("- Status Flags:\n");
			Flags.Print();
		}
	};

#pragma pack(pop)
} // klib

#endif // __ENTITYPOINTS_H__9238409723640921876409234__

