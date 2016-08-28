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
			static const SEntityPointsMultiplier multipliers =						//	x1:	0.85 		
				{ {.125, .125, .125}												//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	
				, {.125, .125, .125}												//	x2:	 .1, , .1, } 		
				, {.125, .125, {.125, .125, .125}, .125, {.125, .125, .125} }		//	x2:	.25, .25,
				, 0.125																//	x4:	.0625}	{.0625,	.0625}	.0625, 
				, 0.85
				};
			return multipliers;
		}
	};

	struct SWeapon		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers =						//	x1:	0.85 		
				{ {.125, .1, .0625}													//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	
				, {.125, .0625, .0625}												//	x2:	 .1, , .1, } 		
				, {.125, .25, {.125, .1, .125}, .0625, {.125, .125, .125} }			//	x2:	.25, .25,
				, 0.125																//	x4:	.0625}	{.0625,	.0625}	.0625, 
				, 0.85
				};
			return multipliers;
		}
	};

	struct SArmor		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static SEntityPointsMultiplier getMultipliers()										
		{																					 
			static const SEntityPointsMultiplier multipliers = 						//	x1:	0.85 		
				{ {.125, .1, .25}													//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	
				, {.125, .1, .25}													//	x2:	 .1, , .1, } 		
				, {.0625, .0625, {.125, .0625, .125}, .125, {.125, .0625, .125} }	//	x2:	.25, .25,
				, 0.125																//	x4:	.0625}	{.0625,	.0625}	.0625, 
				, 0.85
				};
			return multipliers;
		}
	};

	struct SProfession		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers =					//	x1:	0.85 
				{ {.25, .125, .0625}											//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	
				, {.25, .125, .0625}											//	x2:	 .1, , .1, } 
				, {.125, .125, {.0625, .125, .0625}, .125, {.125, .1, .1} }		//	x2:	.25, .25,
				, 0.125															//	x4:	.0625}	{.0625,	.0625}	.0625, 
				, 0.85
				};
			return multipliers;
		}
	};

	struct SAccessory		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers =					//	x1:	0.85 
				{ {.125, .25, .0625}											//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	 
				, {.125, .25, .0625}											//	x2:	 .1, , .1, } 
				, {.125, .1, {.0625, .0625, .125}, .1, {.125, .125, .125} }		//	x2:	.25, .25,
				, 0.125															//	x4:	.0625}	{.0625,	.0625}	.0625, 
				, 0.85															
				};																
			return multipliers;													
		}																		
	};																			
																				
	struct SVehicle		: public SEntity										
	{ 																			
		using SEntity::SEntity; 												
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers =					//	x1:	0.85 
				{ {.0625, .0625, .25}											//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	 
				, {.0625, .0625, .25}											//	x2:	 .1, , .1, } 
				, {.125, .125, {.125, .125, .125}, .125, {.125, .1, .125} }		//	x2:	.25, .25,
				, 0.1															//	x4:	.0625}	{.0625,	.0625}	.0625, 
				, 0.85															
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
				{ {.25, .25, .25}
				, {.25, .25, .25}
				, {.25, .25, {.25, .25, .25}, .25, {.25, .25, .25} }
				, 0.125
				, 0.85
				};
			return multipliers;
		}
	};

	struct SFacility		: public SEntity	
	{ 
		using SEntity::SEntity; 
		SEntityPointsMultiplier getMultipliers() const 
		{
			static const SEntityPointsMultiplier multipliers = 
				{ {.25, .25, .25}
				, {.25, .25, .25}
				, {.25, .25, {.25, .25, .25}, .25, {.25, .25, .25} }
				, 0.125
				, 0.85
				};
			return multipliers;
		}
	};


#define MAX_INVENTORY_SLOTS 32
	typedef SEntityContainer<SItem			, MAX_INVENTORY_SLOTS>	SInventoryItems;
	typedef SEntityContainer<SProfession	, MAX_INVENTORY_SLOTS>	SInventoryProfessions;
	typedef SEntityContainer<SWeapon		, MAX_INVENTORY_SLOTS>	SInventoryWeapons;
	typedef SEntityContainer<SArmor			, MAX_INVENTORY_SLOTS>	SInventoryArmors;
	typedef SEntityContainer<SAccessory		, MAX_INVENTORY_SLOTS>	SInventoryAccessories;
	typedef SEntityContainer<SVehicle		, MAX_INVENTORY_SLOTS>	SInventoryVehicles;
	typedef SEntityContainer<SStageProp		, MAX_INVENTORY_SLOTS>	SInventoryStageProps;
	typedef SEntityContainer<SFacility		, MAX_INVENTORY_SLOTS>	SInventoryFacilities;

	typedef struct SEntityRecord<SItem			>	CRecordItem			;
	typedef struct SEntityRecord<SProfession	>	CRecordProfession	;
	typedef struct SEntityRecord<SWeapon		>	CRecordWeapon		;
	typedef struct SEntityRecord<SArmor			>	CRecordArmor		;
	typedef struct SEntityRecord<SAccessory		>	CRecordAccessory	;
	typedef struct SEntityRecord<SVehicle		>	CRecordVehicle		;
	typedef struct SEntityRecord<SStageProp		>	CRecordStageProp	;
	typedef struct SEntityRecord<SFacility		>	CRecordFacility		;


#pragma pack(pop)
} // namespace

#endif //__ENTITYIMPL_H__928374092634923746298374__