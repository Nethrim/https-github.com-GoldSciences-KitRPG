#include "Game.h"

#include "Weapon.h"
#include "Armor.h"
#include "Profession.h"
#include "Vehicle.h"

#include "Menu.h"

//#define DISABLE_RESEARCH_REQUIREMENTS

template <typename _TEquipClass, typename _TInventory, typename _TResearched, size_t _SizeInventory, size_t _SizeResearched, size_t _SizeDefinitions>
void research
	( klib::SEntityContainer<_TInventory, _SizeInventory>& equipInventory 
	, klib::SEntityContainer<_TResearched, _SizeResearched>& researchedList
	, const _TEquipClass (&definitionsTable)[_SizeDefinitions]
	, _TInventory& adventurerMaxEquip
	, bool bIsModifier
	, bool bIsProgressive
	, const std::string& itemFormat
	, const std::string& sourceName
	, const std::string& lowercaseName
	, const std::string& verbPresentPerfect
	, const std::string& verbPresentContinuous
	, const std::string& verbSimplePast
	) 
{ 
	if(researchedList.Count >= _SizeDefinitions-1) {
		printf("You've done %s all %ss available (%u?)!\n", verbPresentContinuous.c_str(), sourceName.c_str(), researchedList.Count);
		return;
	}

	static const int32_t			maxItemCount = 256;
	static char						menuItemText[maxItemCount]	= {}; 
	static klib::SMenuItem<int32_t>	menuItems	[maxItemCount]	= {}; 

	adventurerMaxEquip.Modifier	= std::max(adventurerMaxEquip.Modifier	, (int16_t)1);
	adventurerMaxEquip.Index	= std::max(adventurerMaxEquip.Index		, (int16_t)1);
	adventurerMaxEquip.Level	= std::max(adventurerMaxEquip.Level		, (int16_t)1);

	int32_t menuItemCount=0; 
	int32_t duplicatedSamples=0;
	for( uint32_t i=0; i<equipInventory.Count; ++i ) 
	{
		int32_t value = 0;
		const char* stringLeft="Left error", *stringRight="Right error";
		if(bIsModifier)
		{
			if( -1 == researchedList.FindElement(equipInventory.Slots[i].Entity.Modifier) )  
			{ 
				stringLeft	= definitionsTable[equipInventory.Slots[i].Entity.Modifier].Name.c_str();
				stringRight	= itemFormat.c_str();
				value = equipInventory.Slots[i].Entity.Modifier;
			}
			else 
				continue;
		} 
		else
		{
			if( -1 == researchedList.FindElement(equipInventory.Slots[i].Entity.Index) )  
			{
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
					printf("%s can't be %s because it's grade %u and your max research grade allowed for is %u.\n", menuItemText, verbSimplePast.c_str(), value, adventurerMaxEquip.Modifier);
					continue;
				}
			}
			else
			{
				if(value > adventurerMaxEquip.Index) {
					printf("%s can't be %s because it's grade %u and your max research grade allowed for is %u.\n", menuItemText, verbSimplePast.c_str(), value, adventurerMaxEquip.Index);
					continue;
				}
			}
		}
#endif

		if(!bRequiresInserting)
			continue;

		for(int32_t i=0; i<menuItemCount; ++i)
			if(menuItems[i].ReturnValue == value)
			{
				printf("You seem to have an extra %s for the next %s %s.\n", menuItemText, lowercaseName.c_str(), verbPresentPerfect.c_str() );
				bRequiresInserting = false;
				break;
			}

		if(bRequiresInserting)
			menuItems[menuItemCount++] = { value, menuItemText };
	}

	if( 0 == menuItemCount ) { 
		printf("You don't have any %s available to %s yet!\n", sourceName.c_str(), verbPresentPerfect.c_str()); 
		return; 
	} 
	
	menuItems[menuItemCount] = {maxItemCount, "Back to tavern"}; 
	menuItemCount++; 
	
	sprintf_s(menuItemText, "Select %s to %s", lowercaseName.c_str(), verbPresentPerfect.c_str());

	int32_t selectedValue = klib::displayMenu(menuItemText, menuItems, menuItemCount); 
	
	if(maxItemCount == selectedValue) { 
		printf("You exit the labs.\n"); 
		return; 
	} 
	
	if(bIsModifier)
	{
		sprintf_s(menuItemText, definitionsTable[selectedValue].Name.c_str(), lowercaseName.c_str());
		printf("You start %s %s.\n", verbPresentContinuous.c_str(), menuItemText); 
	}
	else
		printf("You start %s %s %s.\n", verbPresentContinuous.c_str(), definitionsTable[selectedValue].Name.c_str(), lowercaseName.c_str()); 

	researchedList.AddElement((int32_t)selectedValue); 

	if(bIsModifier)
	{
		adventurerMaxEquip.Modifier		= std::max(adventurerMaxEquip.Modifier, (int16_t)(selectedValue+1)); 
		printf("%s has been %s!\n", menuItemText, verbSimplePast.c_str()); 
	}
	else
	{
		adventurerMaxEquip.Index		= std::max(adventurerMaxEquip.Index, (int16_t)(selectedValue+1)); 
		printf("%s %s has been %s!\n", definitionsTable[selectedValue].Name.c_str(), lowercaseName.c_str(), verbSimplePast.c_str()); 
	}
	research
		( equipInventory 
		, researchedList
		, definitionsTable
		, adventurerMaxEquip
		, bIsModifier
		, bIsProgressive
		, itemFormat
		, sourceName
		, lowercaseName
		, verbPresentPerfect
		, verbPresentContinuous
		, verbSimplePast
		);
};


void  researchWeaponDefinition		(klib::CCharacter& adventurer) { research(adventurer.Weapons		, adventurer.Researched.Weapon		.Definitions	, klib::definitionsWeapon		, adventurer.MaxWeapon		, false	, true, "%s Blueprint"		, "weapon sample"			, "blueprint"	, "research", "researching"	, "researched"	); }
void  researchArmorDefinition		(klib::CCharacter& adventurer) { research(adventurer.Armors			, adventurer.Researched.Armor		.Definitions	, klib::definitionsArmor		, adventurer.MaxArmor		, false	, true, "%s Design"			, "armor sample"			, "design"		, "research", "researching"	, "researched"	); }
void  researchProfessionDefinition	(klib::CCharacter& adventurer) { research(adventurer.Professions	, adventurer.Researched.Profession	.Definitions	, klib::definitionsProfession	, adventurer.MaxProfession	, false	, false, "%s Mastery"		, "profession technique"	, "profession"	, "learn"	, "learning"	, "learned"		); }
void  researchWeaponModifier		(klib::CCharacter& adventurer) { research(adventurer.Weapons		, adventurer.Researched.Weapon		.Modifiers		, klib::modifiersWeapon			, adventurer.MaxWeapon		, true	, false, "Science"			, "science project"			, "project"		, "study"	, "studying"	, "mastered"	); }
void  researchArmorModifier			(klib::CCharacter& adventurer) { research(adventurer.Armors			, adventurer.Researched.Armor		.Modifiers		, klib::modifiersArmor			, adventurer.MaxArmor		, true	, false, "Technology"		, "technology development"	, "design"		, "develop"	, "developing"	, "developed"	); }
void  researchProfessionModifier	(klib::CCharacter& adventurer) { research(adventurer.Professions	, adventurer.Researched.Profession	.Modifiers		, klib::modifiersProfession		, adventurer.MaxProfession	, true	, true, "Rank"				, "rank achievement"		, "achievement"	, "achieve"	, "achieving"	, "achieved"	); }

void  researchVehicleDefinition		(klib::CCharacter& adventurer) { research(adventurer.Vehicles		, adventurer.Researched.Vehicle	.Definitions		, klib::definitionsVehicle		, adventurer.MaxVehicle		, false	, false, "%s Development"	, "vehicle"		, "vehicle"		, "develop"		, "developing"	, "developed"		); }
void  researchVehicleModifier		(klib::CCharacter& adventurer) { research(adventurer.Vehicles		, adventurer.Researched.Vehicle	.Modifiers			, klib::modifiersVehicle		, adventurer.MaxVehicle		, true	, true, "Mechanics"			, "mechanics"	, "mechanics"	, "engineer"	, "engineering"	, "engineered"		); }
