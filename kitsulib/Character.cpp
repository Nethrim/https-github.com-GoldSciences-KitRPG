#include "Character.h"

#include "Weapon.h"
#include "Item.h"
#include "Armor.h"
#include "Profession.h"

#include "Misc.h"

#include <algorithm>

using namespace klib;

void klib::rest(CCharacter& character)
{
	SCharacterPoints finalPoints	= calculateFinalPoints(character);
	character.Points.CurrentLife	= finalPoints.MaxLife	;
	printf("\nYou decide to get some rest.\n");
	character.Points.CurrentLife.Print();
}

SCharacterPoints klib::calculateFinalPoints(const CCharacter& character)
{
	SCharacterPoints result = {};
	const SCharacterPoints 		weaponPoints		= klib::getWeaponPoints		(character.CurrentWeapon);
	const SCharacterPoints 		armorPoints			= klib::getArmorPoints		(character.CurrentArmor);
	const SCharacterPoints 		professionPoints	= klib::getProfessionPoints	(character.CurrentProfession);

	result.MaxLife		= character.CombatBonus.Points.MaxLife		+ weaponPoints.MaxLife		+ armorPoints.MaxLife		+ professionPoints.MaxLife		+	character.Points.MaxLife;
	result.Attack		= character.CombatBonus.Points.Attack		+ weaponPoints.Attack		+ armorPoints.Attack		+ professionPoints.Attack		+	character.Points.Attack;
	result.CurrentLife	= character.CombatBonus.Points.CurrentLife	+ weaponPoints.CurrentLife	+ armorPoints.CurrentLife	+ professionPoints.CurrentLife;
	result.Coins		= character.CombatBonus.Points.Coins		+ weaponPoints.Coins		+ armorPoints.Coins			+ professionPoints.Coins;

	result.AttackEffect		= (ATTACK_EFFECT	)( character.CombatBonus.Points.AttackEffect	| weaponPoints.AttackEffect		| armorPoints.AttackEffect		| professionPoints.AttackEffect		|	character.Points.AttackEffect	);
	result.DefendEffect		= (DEFEND_EFFECT	)( character.CombatBonus.Points.DefendEffect	| weaponPoints.DefendEffect		| armorPoints.DefendEffect		| professionPoints.DefendEffect		|	character.Points.DefendEffect	);
	result.PassiveEffect	= (PASSIVE_EFFECT	)( character.CombatBonus.Points.PassiveEffect	| weaponPoints.PassiveEffect	| armorPoints.PassiveEffect		| professionPoints.PassiveEffect	|	character.Points.PassiveEffect	);
	result.StatusInflict	= (COMBAT_STATUS		)( character.CombatBonus.Points.StatusInflict	| weaponPoints.StatusInflict	| armorPoints.StatusInflict		| professionPoints.StatusInflict	|	character.Points.StatusInflict	);
	result.StatusImmunity	= (COMBAT_STATUS		)( character.CombatBonus.Points.StatusImmunity	| weaponPoints.StatusImmunity	| armorPoints.StatusImmunity	| professionPoints.StatusImmunity	|	character.Points.StatusImmunity	);

	return result;
};

bool klib::addStatus(SCombatStatus& characterStatus, COMBAT_STATUS statusType, int32_t turnCount)
{
	for(int i=0, count=MAX_COMBAT_STATUS_COUNT; i<count; ++i)
	{
		COMBAT_STATUS bitStatus =  (COMBAT_STATUS)(1<<i);
		if(0 == (bitStatus & statusType))
			continue;

		bool bFound = false;
		// look in the inventory for the name so we just increment the counter instead of adding the item
		for(int i=0, count = characterStatus.Count; i<count; i++) {
			if(bitStatus == characterStatus.Status[i]) {
				characterStatus.TurnsLeft[i] += turnCount;
				bFound = true;
				break;
			}
		}

		if(bFound)
			continue;

		if(characterStatus.Count >= size(characterStatus.Status))
			return false;
		else
		{
			// If we didn't return yet it means that the item was not found and we need to add it to the inventory.
			characterStatus.Status		[characterStatus.Count] = bitStatus;
			characterStatus.TurnsLeft	[characterStatus.Count] = turnCount;
			characterStatus.Count++;
		}
	}
	return true;
}

void SCharacter::EquipWeapon		(size_t slotIndex)	{	if(slotIndex < Weapons		.Count)	{	bool bCancel = false; if( -1 == ResearchedDefinitionsWeapon		.FindElement(	Weapons		.Slots[slotIndex].Index))	{ bCancel = true; printf("You can't access to %s weapons until you have researched them!\n"		, definitionsWeapon		[Weapons		.Slots[slotIndex].Index].Name.c_str()); } if( -1 == ResearchedModifiersWeapon		.FindElement(	Weapons		.Slots[slotIndex].Modifier))	{ bCancel = true; static char itemText[128] = {}; sprintf_s(itemText, modifiersWeapon		[Weapons		.Slots[slotIndex].Modifier].Name.c_str(), "Science"		); printf("You can't access to %s weapons until you have researched them!\n"		, itemText); }	if(bCancel) return; printf("You store %s level %u in your arsenal.\n"												, klib::getWeaponName		(CurrentWeapon		).c_str(), CurrentWeapon		.Level); UnloadWeapon		();	CurrentWeapon		= Weapons		.Slots[slotIndex];	Weapons		.RemoveElement(slotIndex);	printf("You equip %s level %u.\n"							, klib::getWeaponName		(CurrentWeapon		).c_str(), CurrentWeapon	.Level	); };	};
void SCharacter::EquipArmor			(size_t slotIndex)	{	if(slotIndex < Armors		.Count)	{	bool bCancel = false; if( -1 == ResearchedDefinitionsArmor		.FindElement(	Armors		.Slots[slotIndex].Index))	{ bCancel = true; printf("You can't access to %s armors until you have researched them!\n"		, definitionsArmor		[Armors			.Slots[slotIndex].Index].Name.c_str()); } if( -1 == ResearchedModifiersArmor		.FindElement(	Armors		.Slots[slotIndex].Modifier))	{ bCancel = true; static char itemText[128] = {}; sprintf_s(itemText, modifiersArmor		[Armors			.Slots[slotIndex].Modifier].Name.c_str(), "Technology"	); printf("You can't access to %s armors until you have researched them!\n"			, itemText); }	if(bCancel) return; printf("You leave %s level %u in your wardrobe.\n"												, klib::getArmorName		(CurrentArmor		).c_str(), CurrentArmor			.Level); UnloadArmor		();	CurrentArmor		= Armors		.Slots[slotIndex];	Armors		.RemoveElement(slotIndex);	printf("You equip %s level %u.\n"							, klib::getArmorName		(CurrentArmor		).c_str(), CurrentArmor		.Level	); };	};
void SCharacter::EquipProfession	(size_t slotIndex)	{	if(slotIndex < Professions	.Count)	{	bool bCancel = false; if( -1 == ResearchedDefinitionsProfession	.FindElement(	Professions	.Slots[slotIndex].Index))	{ bCancel = true; printf("You can't access to %s professions until you have researched them!\n"	, definitionsProfession	[Professions	.Slots[slotIndex].Index].Name.c_str()); } if( -1 == ResearchedModifiersProfession	.FindElement(	Professions	.Slots[slotIndex].Modifier))	{ bCancel = true; static char itemText[128] = {}; sprintf_s(itemText, modifiersProfession	[Professions	.Slots[slotIndex].Modifier].Name.c_str(), "Rank"		); printf("You can't access to %s professions until you have researched them!\n"	, itemText); }	if(bCancel) return; printf("You take a long vacation and forget the details about the %s level %u profession.\n"	, klib::getProfessionName	(CurrentProfession	).c_str(), CurrentProfession	.Level); UnloadProfession	();	CurrentProfession	= Professions	.Slots[slotIndex];	Professions	.RemoveElement(slotIndex);	printf("You get up to date with your profession as %s level %u.\n"	, klib::getProfessionName	(CurrentProfession	).c_str(), CurrentProfession.Level	); };	};


int	SCharacter::Save(FILE* fp)	const
{	
	if(0 == fp) {	
		printf("Cannot save to file!"	); 
		return -1; 
	}	
	if( 1 != fwrite	(this, sizeof(SCharacter), 1, fp) )				
	{ 
		printf( "Cannot save to file!"	); 
		return -1; 
	}; 
	return 0; 
};

int	SCharacter::Load(FILE* fp)	{	
	if(0 == fp) {	
		printf("Cannot load from file!"	); 
		return -1; 
	} 
		
	SCharacter loadedCharacter; 
	if( 1 != fread	(&loadedCharacter, sizeof(SCharacter), 1, fp) ) { 
		printf( "Cannot load from file!"	); 
		return -1; 
	}; 
		
	*this = loadedCharacter; 
	return 0; 
};

int	CCharacter::Save(FILE* fp)	const 
{	
	SCharacter::Save(fp);
	uint8_t nameLen = (uint8_t)Name.size();
	if( 1 != fwrite(&nameLen, 1, 1, fp) )
	{
		printf("Failed to save character data!");
		return -1;
	}
	if(nameLen)
		if( 1 != fwrite(Name.c_str(), 1, nameLen, fp) )
		{
			printf("Failed to load character name!");
			return -1;
		}
	return 0; 
};

int	CCharacter::Load(FILE* fp)	{	
	SCharacter::Load(fp); 
	uint8_t nameLen = 0;
	if( 1 != fread(&nameLen, 1, 1, fp) )
	{
		printf("Failed to load character data!");
		return -1;
	}
	char name[128] = {0,};
	if(nameLen)
		if( 1 != fread(name, 1, std::min(nameLen, (uint8_t)127), fp) )
		{
			printf("Failed to load character name!");
			return -1;
		}
	Name = name;
	return 0; 
};
