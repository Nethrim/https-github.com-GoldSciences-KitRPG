#include "Misc.h"
#include "EntityPoints.h"

#ifndef __ENTITY_H__38924092634721346098170219783096__
#define __ENTITY_H__38924092634721346098170219783096__

namespace klib
{
#pragma pack(push, 1)

	struct SEntity
	{
		int16_t Index		= 0;
		int16_t Modifier	= 0;
		int16_t Level		= 1;

		inline constexpr SEntity(const SEntity& other)	= default;
		inline constexpr SEntity(int16_t index=0, int16_t modifier=0, int16_t level=1)
			: Index		(index		)
			, Modifier	(modifier	)
			, Level		(level		)
		{};

		inline SEntity& operator =(const SEntity& other){ Index = other.Index; Modifier = other.Modifier; Level = other.Level; return *this; };
		inline constexpr bool operator	==(const SEntity& other) const { return Index == other.Index && Modifier == other.Modifier && Level == other.Level;  };

		void Print() const
		{
			printf("Index ......: %i.\n",	Index		);
			printf("Modifier ---: %i.\n",	Modifier	);
			printf("Level ......: %i.\n",	Level		);
		}
	};

	template<typename _EntityType>
	struct SEntitySlot
	{
		uint32_t	Count	= 0;
		_EntityType	Entity	= {};

		inline SEntitySlot& operator =(const SEntitySlot& other){ Count = other.Count; Entity = other.Entity; return *this; };
	};

	template<typename _EntityType, size_t _Size>
	struct SEntityContainer
	{

		uint32_t						Count			= 0;
		SEntitySlot<_EntityType>		Slots[_Size]	= {};

		inline constexpr				SEntityContainer() = default;
		inline constexpr				SEntityContainer(const SEntityContainer& other) = default;

		template<size_t _Size>
		inline							SEntityContainer(size_t slotCount, const _EntityType (&entitySlots)[_Size])
			:Count(0)
		{
			for(size_t slotCount = (_Size < slotCount) ? _Size : slotCount; Count<slotCount; Count++) {
				Slots[Count] = entitySlots[Count];
			}
		};

		inline constexpr int32_t		GetMaxSize() const { return _Size; };

		inline bool						AddElement(const _EntityType& element) { 
			// look in the inventory for the name so we just increment the counter instead of adding the item
			for(uint32_t i=0, count = (_Size < Count) ? _Size : Count; i<count; i++) {
				if(element == Slots[i].Entity) {
					Slots[i].Count++;
					return true;
				}
			}

			if(Count >= size(Slots))
				return false;
			else
			{
				// If we didn't return yet it means that the item was not found and we need to add it to the inventory.
				Slots[Count].Entity	= element;
				Slots[Count].Count	= 1;
				Count++;
				return true;
			}
		};

		inline uint32_t					DecreaseEntity(size_t index) { 
			if( --Slots[index].Count )
				return Slots[index].Count;

			Slots[index] = Slots[--Count];
			return 0; 
		};

		inline int32_t					FindElement(const _EntityType& element) const { 
			for(uint32_t i=0, count=(_Size < Count) ? _Size : Count; i<count; ++i)
				if(Slots[i].Entity == element)
					return i;
			return -1;
		};
	};


	template<typename _EntityType>
	struct SEntityRecord 
	{
		SEntityPoints	Points	;
		SEntityFlags	Flags	;
		std::string		Name	;

		SEntityRecord(SEntityPoints	points	
					 ,SEntityFlags	flags	
					 ,const std::string&	name	
					)
			:Points(points)
			,Flags (flags )
			,Name  (name  )
		{}
	};

#pragma pack(pop)
};

#endif //__ENTITY_H__38924092634721346098170219783096__
