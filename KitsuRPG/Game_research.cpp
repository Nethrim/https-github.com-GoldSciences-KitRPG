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
	, const std::string& postFix
	, const std::string& sourceName
	, const std::string& lowercaseName
	, const std::string& verbPresentPerfect
	, const std::string& verbPresentContinuous
	, const std::string& verbSimplePast
	) 
{ 
	static char menuItemText[256] = {}; 

	static const size_t maxItemCount = 256;


	static klib::SMenuItem<int32_t> menuItems[maxItemCount]; 
	int32_t menuItemCount=0; 
	for( uint32_t i=0; i<equipInventory.Count; ++i ) 
	{
		if(bIsModifier)
		{
			if( -1 == researchedList.FindElement(equipInventory.Slots[i].Modifier) )  
			{ 
				sprintf_s(menuItemText, definitionsTable[equipInventory.Slots[i].Modifier].Name.c_str(), postFix.c_str());
				menuItems[menuItemCount] = { equipInventory.Slots[i].Modifier, menuItemText }; menuItemCount++; 
			}
		} 
		else
		{
			if( -1 == researchedList.FindElement(equipInventory.Slots[i].Index) )  
			{
				sprintf_s(menuItemText, postFix.c_str(), definitionsTable[equipInventory.Slots[i].Index].Name.c_str()); 
				menuItems[menuItemCount] = { equipInventory.Slots[i].Index, menuItemText }; menuItemCount++; 
			}
		}
	}

	if( 0 == menuItemCount ) { 
		printf("You don't have any %s avaiable to %s yet!\n", sourceName.c_str(), verbPresentPerfect.c_str()); 
		return; 
	} 
	
	menuItems[menuItemCount] = {(int16_t)maxItemCount, "Back to tavern"}; 
	menuItemCount++; 
	
	sprintf_s(menuItemText, "Select %s to %s", lowercaseName.c_str(), verbPresentPerfect.c_str());

	int32_t selectedValue = klib::displayMenu(menuItemText, menuItems, menuItemCount); 
	
	if(maxItemCount == selectedValue) { 
		printf("You exit the labs.\n"); 
		return; 
	} 
	
	static char activityName[64] = {}; 

	if(bIsModifier)
	{
		sprintf_s(activityName, definitionsTable[selectedValue].Name.c_str(), lowercaseName.c_str());
		printf("You start %s %s.\n", verbPresentContinuous.c_str(), activityName); 
	}
	else
		printf("You start %s %s %s.\n", verbPresentContinuous.c_str(), definitionsTable[selectedValue].Name.c_str(), lowercaseName.c_str()); 

	researchedList.AddElement((int32_t)selectedValue); 

	if(bIsModifier)
	{
		adventurerMaxEquip.Modifier		= std::max(adventurerMaxEquip.Modifier, (int16_t)selectedValue); 
		printf("%s has been %s!\n", activityName, verbSimplePast.c_str()); 
	}
	else
	{
		adventurerMaxEquip.Index		= std::max(adventurerMaxEquip.Index, (int16_t)selectedValue); 
		printf("%s %s has been %s!\n", definitionsTable[selectedValue].Name.c_str(), lowercaseName.c_str(), verbSimplePast.c_str()); 
	}
};


void  researchWeaponDefinition		(klib::CCharacter& adventurer) { research(adventurer.Weapons		, adventurer.ResearchedDefinitionsWeapon		, klib::weaponDefinitions		, adventurer.MaxWeapon		, false	, "%s Blueprint"	, "weapon sample"			, "blueprint"	, "research", "researching"	, "researched"	); }
void  researchArmorDefinition		(klib::CCharacter& adventurer) { research(adventurer.Armors			, adventurer.ResearchedDefinitionsArmor			, klib::armorDefinitions		, adventurer.MaxArmor		, false	, "%s Design"		, "armor sample"			, "design"		, "research", "researching"	, "researched"	); }
void  researchProfessionDefinition	(klib::CCharacter& adventurer) { research(adventurer.Professions	, adventurer.ResearchedDefinitionsProfession	, klib::professionDefinitions	, adventurer.MaxProfession	, false	, "%s Mastery"		, "profession techniques"	, "profession"	, "learn"	, "learning"	, "learned"		); }
void  researchWeaponModifier		(klib::CCharacter& adventurer) { research(adventurer.Weapons		, adventurer.ResearchedModifiersWeapon			, klib::weaponModifiers			, adventurer.MaxWeapon		, true	, "Science"			, "science project"			, "project"		, "study"	, "studying"	, "mastered"	); }
void  researchArmorModifier			(klib::CCharacter& adventurer) { research(adventurer.Armors			, adventurer.ResearchedModifiersArmor			, klib::armorModifiers			, adventurer.MaxArmor		, true	, "Technology"		, "technology development"	, "design"		, "develop"	, "developing"	, "developed"	); }
void  researchProfessionModifier	(klib::CCharacter& adventurer) { research(adventurer.Professions	, adventurer.ResearchedModifiersProfession		, klib::professionModifiers		, adventurer.MaxProfession	, true	, "Rank"			, "rank"					, "profession"	, "achieve"	, "achieving"	, "achieved"	); }
