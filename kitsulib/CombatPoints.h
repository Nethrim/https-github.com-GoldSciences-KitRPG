#include <cstdint>

#include <algorithm>

#ifndef __COMBATPOINTS_H__928374092873409238650160213__
#define __COMBATPOINTS_H__928374092873409238650160213__

namespace klib
{
	struct SLifePointsMultiplier
	{
		double	HP		;
		double	Mana	;
		double	Shield	;

		inline constexpr SLifePointsMultiplier	operator *	(const int32_t level)	const	{ 
			return { HP*level, Mana*level, Shield*level }; 
		}
	};

	struct SCombatPointsMultiplier
	{
		double	Hit		;
		double	Damage	;

		inline constexpr SCombatPointsMultiplier	operator *	(const int32_t level)	const	{ 
			return { Hit*level, Damage*level }; 
		}
	};

	struct SLifePoints
	{
		int32_t	HP		;
		int32_t	Mana	;
		int32_t	Shield	;

		inline constexpr SLifePoints	operator +	(const SLifePoints& other)				const	{ 
			return { HP+other.HP, Mana+other.Mana, Shield+other.Shield }; 
		}

		inline constexpr SLifePoints	operator *	(const SLifePointsMultiplier& other)	const	{ 
			return { 
				(int32_t)(	HP		*	std::max(	1.000001, other.HP		)), 
				(int32_t)(	Mana	*	std::max(	1.000001, other.Mana	)), 
				(int32_t)(	Shield	*	std::max(	1.000001, other.Shield	))}; 
		}

		SLifePoints&					operator +=	(const SLifePoints& other)						{ 
			HP		+= other.HP; 
			Mana	+= other.Mana; 
			Shield	+= other.Shield; 
			return *this; 
		}

		void Print() const
		{
			printf("HP     : %i.\n",	HP		);
			printf("Mana   : %i.\n",	Mana	);
			printf("Shield : %i.\n",	Shield	);
		}
	};

	struct SCombatPoints
	{
		int32_t	Hit		;
		int32_t	Damage	;

		inline constexpr SCombatPoints	operator +	(const SCombatPoints& other)			const	{ 
			return {Hit+other.Hit, Damage+other.Damage}; 
		}

		inline constexpr SCombatPoints	operator *	(const SCombatPointsMultiplier& other)	const	{ 
			return { (int32_t)(Hit*std::max(1.000001, other.Hit)), (int32_t)(Damage*std::max(1.0001, other.Damage))}; 
		}

		SCombatPoints&					operator +=	(const SCombatPoints& other)					{ 
			Hit += other.Hit; Damage += other.Damage; return *this; 
		}

		void Print() const
		{
			printf("Hit    : %i.\n",	Hit		);
			printf("Damage : %i.\n",	Damage	);
		}
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

} //namespace

#endif // __COMBATPOINTS_H__928374092873409238650160213__
