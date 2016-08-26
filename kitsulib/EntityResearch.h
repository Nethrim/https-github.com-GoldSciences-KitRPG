#include "Entity.h"

#include "Menu.h"

#ifndef __ENTITYRESEARCH_H__92803472098473209847239084702934__
#define __ENTITYRESEARCH_H__92803472098473209847239084702934__

namespace klib
{
#pragma pack(push, 1)

	struct SResearchGroup
	{
		SEntityContainer<int16_t, 256>	Definitions;
		SEntityContainer<int16_t, 256>	Modifiers;
	};

	// This hell creates an user menu for the entity records available for research
	template <typename _TEquipClass, typename _TInventory, typename _TResearched, size_t _SizeInventory, size_t _SizeResearched, size_t _SizeDefinitions>
	void research
		( klib::SEntityContainer<_TInventory, _SizeInventory>& equipInventory 
		, klib::SEntityContainer<_TResearched, _SizeResearched>& researchedList
		, const _TEquipClass (&definitionsTable)[_SizeDefinitions]
		, _TInventory& adventurerMaxEquip
		, bool bIsModifier
		, bool bIsProgressive
		, const std::string& itemFormat
		, const std::string& allResearchComplete
		, const std::string& noResearchAvailable
		, const std::string& selectItemToResearch
		, const std::string& startResearching
		, const std::string& doneResearching
		) 
	{ 
		if(researchedList.Count >= _SizeDefinitions-1) {	// No more research items in the game.
			printf(allResearchComplete.c_str(), researchedList.Count);
			return;
		}

		// These variables are obviously to store the menuitems and compose the item menu text
		static const int32_t			maxItemCount = 256;
		static char						menuItemText[maxItemCount]	= {}; 
		static klib::SMenuItem<int32_t>	menuItems	[maxItemCount]	= {}; 

		adventurerMaxEquip.Modifier	= (adventurerMaxEquip.Modifier	> 1) ? adventurerMaxEquip.Modifier	: 1;
		adventurerMaxEquip.Index	= (adventurerMaxEquip.Index		> 1) ? adventurerMaxEquip.Index		: 1;
		adventurerMaxEquip.Level	= (adventurerMaxEquip.Level		> 1) ? adventurerMaxEquip.Level		: 1;

		int32_t menuItemCount=0; 
		int32_t duplicatedSamples=0;
		for( uint32_t i=0, count = equipInventory.Count; i<count; ++i ) {
			int32_t value = 0;
			const char* stringLeft=0, *stringRight=0;
			if(bIsModifier)
			{
				if( 0 != equipInventory.Slots[i].Entity.Modifier && -1 == researchedList.FindElement(equipInventory.Slots[i].Entity.Modifier) ) { 
					stringLeft	= definitionsTable[equipInventory.Slots[i].Entity.Modifier].Name.c_str();
					stringRight	= itemFormat.c_str();
					value = equipInventory.Slots[i].Entity.Modifier;
				}
				else 
					continue;
			} 
			else
			{
				if( 0 != equipInventory.Slots[i].Entity.Index && -1 == researchedList.FindElement(equipInventory.Slots[i].Entity.Index) ) {
					stringRight	= definitionsTable[equipInventory.Slots[i].Entity.Index].Name.c_str();
					stringLeft	= itemFormat.c_str();
					value = equipInventory.Slots[i].Entity.Index; 
				}
				else 
					continue;
			}

			sprintf_s(menuItemText, stringLeft, stringRight);

			bool bRequiresInserting = true;
	#ifndef DISABLE_RESEARCH_REQUIREMENTS
			if(bIsProgressive) {
				if(bIsModifier) 
				{
					if(value > adventurerMaxEquip.Modifier) {
						printf("%s can't be pursued because it's grade %u and your max research grade allowed for is %u.\n", menuItemText, value, adventurerMaxEquip.Modifier);
						continue;
					}
				}
				else
				{
					if(value > adventurerMaxEquip.Index) {
						printf("%s can't be pursued because it's grade %u and your max research grade allowed for is %u.\n", menuItemText, value, adventurerMaxEquip.Index);
						continue;
					}
				}
			}
	#endif

			if(!bRequiresInserting)
				continue;

			for(int32_t i=0; i<menuItemCount; ++i)
				if(menuItems[i].ReturnValue == value) {
					printf("You seem to have an additional research point for %s.\n", menuItemText);
					bRequiresInserting = false;
					break;
				}

			if(bRequiresInserting)
				menuItems[menuItemCount++] = { value, menuItemText };
		}

		if( 0 == menuItemCount ) { 
			printf("%s", noResearchAvailable.c_str()); 
			return; 
		} 
	
		menuItems[menuItemCount++] = {maxItemCount, "Back to tavern"}; 
	
		sprintf_s(menuItemText, "%s", selectItemToResearch.c_str());
		int32_t selectedValue = klib::displayMenu(menuItemText, menuItems, menuItemCount); 
	
		if(maxItemCount == selectedValue) { 
			printf("You exit the labs.\n"); 
			return; 
		} 
	
		if(bIsModifier) {
			sprintf_s(menuItemText, definitionsTable[selectedValue].Name.c_str(), itemFormat.c_str());
			printf(startResearching.c_str(), menuItemText); 
		}
		else
			printf(startResearching.c_str(), definitionsTable[selectedValue].Name.c_str()); 

		researchedList.AddElement((int32_t)selectedValue); 

		if(bIsModifier)
		{
			adventurerMaxEquip.Modifier		= std::max(adventurerMaxEquip.Modifier, (int16_t)(selectedValue+1)); 
			printf(doneResearching.c_str(), menuItemText); 
		}
		else
		{
			adventurerMaxEquip.Index		= std::max(adventurerMaxEquip.Index, (int16_t)(selectedValue+1)); 
			printf(doneResearching.c_str(), definitionsTable[selectedValue].Name.c_str()); 
		}
		research
			( equipInventory 
			, researchedList
			, definitionsTable
			, adventurerMaxEquip
			, bIsModifier
			, bIsProgressive
			, itemFormat
			, allResearchComplete
			, noResearchAvailable
			, selectItemToResearch
			, startResearching
			, doneResearching
			);
	};

#pragma pack(pop)
} // namespace


#endif // __ENTITYRESEARCH_H__92803472098473209847239084702934__
