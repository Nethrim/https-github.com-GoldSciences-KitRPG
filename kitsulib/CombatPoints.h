#include <cstdint>

#include <algorithm>

#ifndef __COMBATPOINTS_H__928374092873409238650160213__
#define __COMBATPOINTS_H__928374092873409238650160213__

namespace klib
{
	struct SLifePointsMultiplier
	{
		double	Health	;
		double	Mana	;
		double	Shield	;

		inline constexpr SLifePointsMultiplier	operator *	(const int32_t level)	const	{ 
			return { Health*level, Mana*level, Shield*level }; 
		}

		void Print() const
		{
			printf("Health.: %f.\n",	Health	);
			printf("Mana---: %f.\n",	Mana	);
			printf("Shield.: %f.\n",	Shield	);
		}
	};

	struct SAttackPointsMultiplier
	{
		double	Hit			;
		double	Damage		;
		double	Speed		;
		double	Absorption	;

		inline constexpr SAttackPointsMultiplier	operator *	(const int32_t level)	const	{ 
			return { Hit*level, Damage*level, Speed*level, Absorption*level }; 
		}

		void Print() const
		{
			printf("Hit........: %f.\n",	Hit			);
			printf("Damage-----: %f.\n",	Damage		);
			printf("Speed......: %f.\n",	Speed		);
			printf("Absorption-: %f.\n",	Absorption	);
		}
	};

	struct SLifePoints
	{
		int32_t	Health	;
		int32_t	Mana	;
		int32_t	Shield	;

		inline constexpr SLifePoints	operator +	(const SLifePoints& other)				const	{ 
			return { Health+other.Health, Mana+other.Mana, Shield+other.Shield }; 
		}

		inline constexpr SLifePoints	operator *	(const SLifePointsMultiplier& other)	const	{ 
			return { (int32_t)(	Health	*	std::max(	1.000001, other.Health	)), 
					 (int32_t)(	Mana	*	std::max(	1.000001, other.Mana	)), 
					 (int32_t)(	Shield	*	std::max(	1.000001, other.Shield	)) 
			}; 
		}

		SLifePoints&					operator +=	(const SLifePoints& other)						{ 
			Health	+= other.Health; 
			Mana	+= other.Mana; 
			Shield	+= other.Shield; 
			return *this; 
		}

		void Print() const
		{
			printf("Health.: %i.\n",	Health	);
			printf("Mana---: %i.\n",	Mana	);
			printf("Shield.: %i.\n",	Shield	);
		}
	};

	struct SAttackPoints
	{
		int32_t		Hit			;
		int32_t		Damage		;
		int32_t		Speed		;
		int32_t		Absorption	;

		inline constexpr SAttackPoints	operator +	(const SAttackPoints& other)			const	{ 
			return {Hit+other.Hit, Damage+other.Damage, Speed+other.Speed, Absorption+other.Absorption}; 
		}

		inline constexpr SAttackPoints	operator *	(const SAttackPointsMultiplier& other)	const	{ 
			return { (int32_t)(Hit*std::max(1.000001, other.Hit)), (int32_t)(Damage*std::max(1.0001, other.Damage)), (int32_t)(Damage*std::max(1.0001, other.Damage))}; 
		}

		SAttackPoints&					operator +=	(const SAttackPoints& other)					{ 
			Hit += other.Hit; Damage += other.Damage; return *this; 
		}

		void Print() const
		{
			printf("Hit........: %i.\n",	Hit		);
			printf("Damage-----: %i.\n",	Damage	);
			printf("Speed......: %i.\n",	Damage	);
			printf("Absorption-: %%%i.\n",	Damage	);
		}
	};

} //namespace

#endif // __COMBATPOINTS_H__928374092873409238650160213__
