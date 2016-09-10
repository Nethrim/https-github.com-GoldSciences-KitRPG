#include "Entity.h"

#ifndef __ENTITYIMPL_H__928374092634923746298374__
#define __ENTITYIMPL_H__928374092634923746298374__


namespace klib
{
#pragma pack(push, 1)
	struct SItem		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static constexpr SEntityPointsMultiplier getMultipliers()
		{
			return	{ {.125, .125, .125}												//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	
					, {.125, .125, .125}												//	x2:	 .1, , .1, } 		
					, {.125, .125, {.125, .125, .125}, .125, {.125, .125, .125} }		//	x2:	.25, .25,
					, 0.125																//	x4:	.0625}	{.0625,	.0625}	.0625, 
					, 0.85																//	x1:	0.85 
					};
		}
	};

	struct SWeapon		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static constexpr SEntityPointsMultiplier getMultipliers()
		{
			return	{ {.125, .1, .0625}													//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	
					, {.125, .0625, .0625}												//	x2:	 .1, , .1, } 		
					, {.125, .25, {.125, .1, .125}, .0625, {.125, .125, .125} }			//	x2:	.25, .25,
					, 0.125																//	x4:	.0625}	{.0625,	.0625}	.0625, 
					, 0.85																//	x1:	0.85 
					};
		}
	};

	struct SArmor		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static constexpr SEntityPointsMultiplier getMultipliers()
		{																					 
			return	{ {.125, .1, .25}														//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	
					, {.125, .1, .25}														//	x2:	 .1, , .1, } 		
					, {.0625, .0625, {.125, .0625, .125}, .125, {.125, .0625, .125} }		//	x2:	.25, .25,
					, 0.125																	//	x4:	.0625}	{.0625,	.0625}	.0625, 
					, 0.85																	//	x1:	0.85 
					};
		}
	};

	struct SProfession		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static constexpr SEntityPointsMultiplier getMultipliers()
		{
			return	{ {.25, .125, .0625}												//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	
					, {.25, .125, .0625}												//	x2:	 .1, , .1, } 
					, {.125, .125, {.0625, .125, .0625}, .125, {.125, .1, .1} }			//	x2:	.25, .25,
					, 0.125																//	x4:	.0625}	{.0625,	.0625}	.0625, 
					, 0.85																//	x1:	0.85 
					};
		}
	};

	struct SAccessory		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static constexpr SEntityPointsMultiplier getMultipliers()
		{
			return	{ {.125, .25, .0625}												//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	 
					, {.125, .25, .0625}												//	x2:	 .1, , .1, } 
					, {.125, .1, {.0625, .0625, .125}, .1, {.125, .125, .125} }			//	x2:	.25, .25,
					, 0.125																//	x4:	.0625}	{.0625,	.0625}	.0625, 
					, 0.85																//	x1:	0.85 
					};																
		}																		
	};																			
																				
	struct SVehicle		: public SEntity										
	{ 																			
		using SEntity::SEntity; 												
		static constexpr SEntityPointsMultiplier getMultipliers()
		{
			return	{ {.0625, .0625, .25}												//	x8:	{.125, .125, .125}{.125, {.125,  {.125, .125} 0.125	 
					, {.0625, .0625, .25}												//	x2:	 .1, , .1, } 
					, {.125, .125, {.125, .125, .125}, .125, {.125, .1, .125} }			//	x2:	.25, .25,
					, 0.1																//	x4:	.0625}	{.0625,	.0625}	.0625, 
					, 0.85																//	x1:	0.85 
					};																
		}
	};

	struct SStageProp		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static constexpr SEntityPointsMultiplier getMultipliers()
		{
			return	{ {.25, .25, .25}
					, {.25, .25, .25}
					, {.25, .25, {.25, .25, .25}, .25, {.25, .25, .25} }
					, 0.125
					, 0.85																//	x1:	0.85 
					};
		}
	};

	struct SFacility		: public SEntity	
	{ 
		using SEntity::SEntity; 
		static constexpr SEntityPointsMultiplier getMultipliers()
		{
			return	{ {.25, .25, .25}
					, {.25, .25, .25}
					, {.25, .25, {.25, .25, .25}, .25, {.25, .25, .25} }
					, 0.125
					, 0.85																//	x1:	0.85 
					};
		}
	};


#define MAX_INVENTORY_SLOTS 32
	typedef SEntityContainer<SItem			, MAX_INVENTORY_SLOTS>	SInventoryItems;
	typedef SEntityContainer<SProfession	, MAX_INVENTORY_SLOTS>	SInventoryProfession;
	typedef SEntityContainer<SWeapon		, MAX_INVENTORY_SLOTS>	SInventoryWeapon;
	typedef SEntityContainer<SArmor			, MAX_INVENTORY_SLOTS>	SInventoryArmor;
	typedef SEntityContainer<SAccessory		, MAX_INVENTORY_SLOTS>	SInventoryAccessory;
	typedef SEntityContainer<SVehicle		, MAX_INVENTORY_SLOTS>	SInventoryVehicle;
	typedef SEntityContainer<SStageProp		, MAX_INVENTORY_SLOTS>	SInventoryStageProp;
	typedef SEntityContainer<SFacility		, MAX_INVENTORY_SLOTS>	SInventoryFacility;

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