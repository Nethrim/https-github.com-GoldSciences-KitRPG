#include "Game.h"
#include "Weapon.h"
#include "Armor.h"
#include "Profession.h"
#include "Menu.h"

template <typename _TEquipClass, typename _TInventory, typename _TResearched, size_t _SizeInventory, size_t _SizeResearched, size_t _SizeDefinitions>
void research
	( klib::SEquipContainer<_TInventory, _SizeInventory>& equipInventory 
	, klib::SEquipContainer<_TResearched, _SizeResearched>& researchedList
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
	static const int32_t			maxItemCount = 256;
	static char						menuItemText[maxItemCount]	= {}; 
	static klib::SMenuItem<int32_t>	menuItems	[maxItemCount]	= {}; 

	int32_t menuItemCount=0; 
	int32_t duplicatedSamples=0;
	for( uint32_t i=0; i<equipInventory.Count; ++i ) 
	{
		int32_t value = 0;
		const char* stringLeft="Left error", *stringRight="Right error";
		if(bIsModifier)
		{
			if( -1 == researchedList.FindElement(equipInventory.Slots[i].Modifier) )  
			{ 
				stringLeft	= definitionsTable[equipInventory.Slots[i].Modifier].Name.c_str();
				stringRight	= itemFormat.c_str();
				value = equipInventory.Slots[i].Modifier;
			}
			else 
				continue;
		} 
		else
		{
			if( -1 == researchedList.FindElement(equipInventory.Slots[i].Index) )  
			{
				stringRight	= definitionsTable[equipInventory.Slots[i].Index].Name.c_str();
				stringLeft	= itemFormat.c_str();
				value = equipInventory.Slots[i].Index; 
			}
			else 
				continue;
		}

		sprintf_s(menuItemText, stringLeft, stringRight);

		bool bRequiresInserting = true;
		if(bIsProgressive) {
			if(bIsModifier) {
				if(value > adventurerMaxEquip.Modifier)
				{
					printf("%s can't be %s because it's grade %u and your max grade researched is %u.\n", menuItemText, verbSimplePast.c_str(), value, adventurerMaxEquip.Modifier);
					continue;
				}
			}
			else
			{
				if(value > adventurerMaxEquip.Index)
				{
					printf("%s can't be %s because it's grade %u and your max grade researched is %u.\n", menuItemText, verbSimplePast.c_str(), value, adventurerMaxEquip.Index);
					continue;
				}
			}
		}

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
		adventurerMaxEquip.Modifier		= std::max(adventurerMaxEquip.Modifier, (int16_t)selectedValue+1); 
		printf("%s has been %s!\n", menuItemText, verbSimplePast.c_str()); 
	}
	else
	{
		adventurerMaxEquip.Index		= std::max(adventurerMaxEquip.Index, (int16_t)selectedValue+1); 
		printf("%s %s has been %s!\n", definitionsTable[selectedValue].Name.c_str(), lowercaseName.c_str(), verbSimplePast.c_str()); 
	}
};


void  researchWeaponDefinition		(klib::CCharacter& adventurer) { research(adventurer.Weapons		, adventurer.ResearchedDefinitionsWeapon		, klib::weaponDefinitions		, adventurer.MaxWeapon		, false	, true, "%s Blueprint"	, "weapon sample"			, "blueprint"	, "research", "researching"	, "researched"	); }
void  researchArmorDefinition		(klib::CCharacter& adventurer) { research(adventurer.Armors			, adventurer.ResearchedDefinitionsArmor			, klib::armorDefinitions		, adventurer.MaxArmor		, false	, true, "%s Design"		, "armor sample"			, "design"		, "research", "researching"	, "researched"	); }
void  researchProfessionDefinition	(klib::CCharacter& adventurer) { research(adventurer.Professions	, adventurer.ResearchedDefinitionsProfession	, klib::professionDefinitions	, adventurer.MaxProfession	, false	, false, "%s Mastery"	, "profession techniques"	, "profession"	, "learn"	, "learning"	, "learned"		); }
void  researchWeaponModifier		(klib::CCharacter& adventurer) { research(adventurer.Weapons		, adventurer.ResearchedModifiersWeapon			, klib::weaponModifiers			, adventurer.MaxWeapon		, true	, false, "Science"		, "science project"			, "project"		, "study"	, "studying"	, "mastered"	); }
void  researchArmorModifier			(klib::CCharacter& adventurer) { research(adventurer.Armors			, adventurer.ResearchedModifiersArmor			, klib::armorModifiers			, adventurer.MaxArmor		, true	, false, "Technology"	, "technology development"	, "design"		, "develop"	, "developing"	, "developed"	); }
void  researchProfessionModifier	(klib::CCharacter& adventurer) { research(adventurer.Professions	, adventurer.ResearchedModifiersProfession		, klib::professionModifiers		, adventurer.MaxProfession	, true	, true, "Rank"			, "rank achievement"		, "achievement"	, "achieve"	, "achieving"	, "achieved"	); }
