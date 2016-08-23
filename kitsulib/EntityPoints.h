#include "CombatPoints.h"
#include "CombatStatus.h"

#ifndef __ENTITYPOINTS_H__9238409723640921876409234__
#define __ENTITYPOINTS_H__9238409723640921876409234__

namespace klib
{
	struct SEntityPoints {
		SLifePoints		MaxLife;
		SLifePoints		CurrentLife;
		SAttackPoints	Attack;
		int32_t			Coins;
	};

	struct SEntityFlags {
		SEntityEffect		Effect;
		SEntityStatus		Status;
		SEntityGrade		Tech;
	};
} // klib

#endif // __ENTITYPOINTS_H__9238409723640921876409234__

