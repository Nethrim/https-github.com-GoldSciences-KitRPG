#include "Entity.h"

#ifndef __ENTITYIMPL_H__928374092634923746298374__
#define __ENTITYIMPL_H__928374092634923746298374__


namespace klib
{
#pragma pack(push, 1)
	struct SItem		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers = 
				{ {.1, .1, .25}
				, {.1, .1, .1}
				, {.1, .1, {.1, .1, .1}, .1, {.1, .1, .1} }
				, 0.125
				, 1.125
				};
			return multipliers;
		}
	};

	struct SWeapon		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers = 
				{ {.1, .1, .25}
				, {.1, .1, .1}
				, {.1, .1, {.1, .1, .1}, .1, {.1, .1, .1} }
				, 0.125
				, 1.125
				};
			return multipliers;
		}
	};

	struct SArmor		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static SEntityPointsMultiplier getMultipliers()
		{
			static const SEntityPointsMultiplier multipliers = 
				{ {.1, .1, .25}
				, {.1, .1, .1}
				, {.1, .1, {.1, .1, .1}, .1, {.1, .1, .1} }
				, 0.125
				, 1.125
				};
			return multipliers;
		}
	};

	struct SProfession		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers = 
				{ {.1, .1, .25}
				, {.1, .1, .1}
				, {.1, .1, {.1, .1, .1}, .1, {.1, .1, .1} }
				, 0.125
				, 1.125
				};
			return multipliers;
		}
	};

	struct SVehicle		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers = 
				{ {.1, .1, .25}
				, {.1, .1, .1}
				, {.1, .1, {.1, .1, .1}, .1, {.1, .1, .1} }
				, 0.125
				, 1.125
				};
			return multipliers;
		}
	};

	struct SStageProp		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers = 
				{ {.1, .1, .25}
				, {.1, .1, .1}
				, {.1, .1, {.1, .1, .1}, .1, {.1, .1, .1} }
				, 0.125
				, 1.125
				};
			return multipliers;
		}
	};


#define MAX_INVENTORY_SLOTS 512
	typedef SEntityContainer<SItem			, MAX_INVENTORY_SLOTS>	SInventoryItems;
	typedef SEntityContainer<SWeapon		, MAX_INVENTORY_SLOTS>	SInventoryWeapons;
	typedef SEntityContainer<SArmor			, MAX_INVENTORY_SLOTS>	SInventoryArmors;
	typedef SEntityContainer<SProfession	, MAX_INVENTORY_SLOTS>	SInventoryProfessions;
	typedef SEntityContainer<SVehicle		, MAX_INVENTORY_SLOTS>	SInventoryVehicles;
	typedef SEntityContainer<SStageProp		, MAX_INVENTORY_SLOTS>	SInventoryStageProps;

	typedef struct SEntityRecord<SArmor			>	CRecordArmor		;
	typedef struct SEntityRecord<SProfession	>	CRecordProfession	;
	typedef struct SEntityRecord<SWeapon		>	CRecordWeapon		;
	typedef struct SEntityRecord<SVehicle		>	CRecordVehicle		;
	typedef struct SEntityRecord<SStageProp		>	CRecordStageProp	;
	typedef struct SEntityRecord<SItem			>	CRecordItem			;

#pragma pack(pop)
} // namespace

#endif //__ENTITYIMPL_H__928374092634923746298374__