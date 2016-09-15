#include <cstdint>
#include <cstdio>

#ifndef __COMBATPOINTS_H__928374092873409238650160213__
#define __COMBATPOINTS_H__928374092873409238650160213__

namespace klib
{
#pragma pack(push, 1)

	struct SLifePointsMultiplier
	{
		double	Health	;
		double	Mana	;
		double	Shield	;

		inline constexpr SLifePointsMultiplier	operator *	(const int32_t level)	const	{ 
			return 
				{ Health	*level 
				, Mana		*level 
				, Shield	*level 
			}; 
		}

		void Print() const
		{
			printf("Health .....: %f.\n",	Health	);
			printf("Mana -------: %f.\n",	Mana	);
			printf("Shield .....: %f.\n",	Shield	);
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

		inline constexpr SLifePoints	operator -	(const SLifePoints& other)				const	{ 
			return { Health-other.Health, Mana-other.Mana, Shield-other.Shield }; 
		}

		inline constexpr SLifePoints	operator *	(const SLifePointsMultiplier& other)	const	{ 
			return	{ Health	? ((1 < (Health	*	other.Health	)) ?	(int32_t)(Health	*	other.Health	): 1) : 0
					, Mana		? ((1 < (Mana	*	other.Mana		)) ?	(int32_t)(Mana		*	other.Mana		): 1) : 0
					, Shield	? ((1 < (Shield	*	other.Shield	)) ?	(int32_t)(Shield	*	other.Shield	): 1) : 0
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
			printf("Health .....: %i.\n",	Health	);
			printf("Mana -------: %i.\n",	Mana	);
			printf("Shield .....: %i.\n",	Shield	);
		}
	};

	struct SSpeedPointsMultiplier
	{
		double			Attack;
		double			Movement;
		double			Reflexes;

		constexpr SSpeedPointsMultiplier	operator *	(const int32_t level)	const	{ 
			return { Attack*level, Movement*level, Reflexes*level }; 
		}

		void Print() const
		{
			printf("Attack .....: %f.\n",	Attack		);
			printf("Movement ---: %f.\n",	Movement	);
			printf("Reflexes ...: %f.\n",	Reflexes	);
		}
	};

	struct SSpeedPoints
	{
		int32_t			Attack	;
		int32_t			Movement;
		int32_t			Reflexes;

		constexpr SSpeedPoints	operator +	(const SSpeedPoints& other)			const	{ 
			return { Attack+other.Attack, Movement+other.Movement, Reflexes+other.Reflexes }; 
		}

		constexpr SSpeedPoints	operator -	(const SSpeedPoints& other)			const	{ 
			return { Attack-other.Attack, Movement-other.Movement, Reflexes-other.Reflexes }; 
		}

		constexpr SSpeedPoints	operator *	(const SSpeedPointsMultiplier& other)	const	{ 
			return	{ Attack	? ((1 < (Attack		*	other.Attack	)) ?	(int32_t)(Attack	*	other.Attack	): 1) : 0
					, Movement	? ((1 < (Movement	*	other.Movement	)) ?	(int32_t)(Movement	*	other.Movement	): 1) : 0
					, Reflexes	? ((1 < (Reflexes	*	other.Reflexes	)) ?	(int32_t)(Reflexes	*	other.Reflexes	): 1) : 0
				}; 
		}

		SSpeedPoints&					operator +=	(const SSpeedPoints& other)					{ 
			Attack += other.Attack; Movement += other.Movement; Reflexes += other.Reflexes; return *this; 
		}

		void Print() const
		{
			printf("Attack .....: %i.\n",	Attack	);
			printf("Movement ---: %i.\n",	Movement);
			printf("Reflexes ...: %i.\n",	Reflexes);
		}
	};

	struct SAttackPointsMultiplier
	{
		double					Hit			;
		double					Damage		;
		SSpeedPointsMultiplier	Speed		;
		double					Absorption	;
		SLifePointsMultiplier	DirectDamage	;

		constexpr SAttackPointsMultiplier	operator *	(const int32_t level)	const	{ 
			return { Hit*level, Damage*level, Speed*level, Absorption*level, DirectDamage*level }; 
		}

		void Print() const
		{
			printf("Hit ........: %f.\n",	Hit			);
			printf("Damage -----: %f.\n",	Damage		);
			printf("Absorption -: %f.\n",	Absorption	);
			printf("- Speed Points:.\n");
			Speed.Print();
			printf("- Extra Point Damage:\n");
			DirectDamage.Print();
		}
	};

	struct SAttackPoints
	{
		int32_t			Hit				;
		int32_t			Damage			;
		SLifePoints		DirectDamage	;
		int32_t			Absorption		;
		SSpeedPoints	Speed			;

		constexpr SAttackPoints	operator +	(const SAttackPoints& other)			const	{ 
			return { Hit+other.Hit, Damage+other.Damage, DirectDamage+other.DirectDamage, Absorption+other.Absorption, Speed+other.Speed };
		}
		constexpr SAttackPoints	operator -	(const SAttackPoints& other)			const	{ 
			return { Hit-other.Hit, Damage-other.Damage, DirectDamage-other.DirectDamage, Absorption-other.Absorption, Speed-other.Speed };
		}

		constexpr SAttackPoints	operator *	(const SAttackPointsMultiplier& other)	const	{ 
			return	{ Hit			? ((1 < (Hit		*	other.Hit			)) ?	(int32_t)(Hit			*	other.Hit			): 1) : 0
					, Damage		? ((1 < (Damage		*	other.Damage		)) ?	(int32_t)(Damage		*	other.Damage		): 1) : 0
					, DirectDamage*other.DirectDamage
					, Absorption	? ((1 < (Absorption	*	other.Absorption	)) ?	(int32_t)(Absorption	*	other.Absorption	): 1) : 0
					, Speed*other.Speed
				}; 
		}

		SAttackPoints&					operator +=	(const SAttackPoints& other) { 
			Hit			+= other.Hit; 
			Damage		+= other.Damage; 
			DirectDamage += other.DirectDamage; 
			Absorption	+= other.Absorption; 
			Speed		+= other.Speed; 
			return *this; 
		}

		void Print() const
		{
			printf("Hit ........: %i.\n",	Hit			);
			printf("Damage .....: %i.\n",	Damage		);
			printf("Absorption -: %%%i.\n",	Absorption	);
			printf("- Speed Points:.\n");
			Speed.Print();
			printf("- Extra Point Damage:\n");
			DirectDamage.Print();
		}
	};

#pragma pack(pop)
} //namespace

#endif // __COMBATPOINTS_H__928374092873409238650160213__
