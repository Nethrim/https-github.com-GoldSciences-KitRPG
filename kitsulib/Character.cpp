#include "Character.h"

#include "Weapon.h"
#include "Item.h"
#include "Armor.h"
#include "Profession.h"
#include "Vehicle.h"

#include "Misc.h"

#include <algorithm>

using namespace klib;

void klib::rest(CCharacter& character)
{
	SEntityPoints finalPoints	= calculateFinalPoints(character);
	character.Points.LifeCurrent	= finalPoints.LifeMax	;
	printf("\nYou decide to get some rest.\n");
	character.Points.LifeCurrent.Print();
}

SEntityPoints klib::calculateFinalPoints(const CCharacter& character)
{
	SEntityPoints result = {};
	const SEntityPoints	weaponPoints		= klib::getWeaponPoints		(character.CurrentWeapon);
	const SEntityPoints	armorPoints			= klib::getArmorPoints		(character.CurrentArmor);
	const SEntityPoints	professionPoints	= klib::getProfessionPoints	(character.CurrentProfession);
	const SEntityPoints	vehiclePoints		= klib::getVehiclePoints	(character.CurrentVehicle);

	result.LifeMax		= character.CombatBonus.Points.LifeMax		+ weaponPoints.LifeMax		+ armorPoints.LifeMax		+ vehiclePoints.LifeMax		+ professionPoints.LifeMax		+	character.Points.LifeMax;
	result.Attack		= character.CombatBonus.Points.Attack		+ weaponPoints.Attack		+ armorPoints.Attack		+ vehiclePoints.Attack		+ professionPoints.Attack		+	character.Points.Attack;
	result.LifeCurrent	= character.CombatBonus.Points.LifeCurrent	+ weaponPoints.LifeCurrent	+ armorPoints.LifeCurrent	+ vehiclePoints.LifeCurrent	+ professionPoints.LifeCurrent;
	result.Coins		= character.CombatBonus.Points.Coins		+ weaponPoints.Coins		+ armorPoints.Coins			+ vehiclePoints.Coins		+ professionPoints.Coins;

	return result;
};

SEntityFlags klib::calculateFinalFlags(const CCharacter& character)
{
	SEntityFlags result = {};
	const SEntityFlags	weaponFlags			= klib::getWeaponFlags		(character.CurrentWeapon);
	const SEntityFlags	armorFlags			= klib::getArmorFlags		(character.CurrentArmor);
	const SEntityFlags	professionFlags		= klib::getProfessionFlags	(character.CurrentProfession);
	const SEntityFlags	vehicleFlags		= klib::getVehicleFlags		(character.CurrentVehicle);
	result	= character.CombatBonus.Flags | weaponFlags | armorFlags | vehicleFlags | professionFlags | character.Flags;

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

void SCharacter::EquipWeapon		(size_t slotIndex)	{	if(slotIndex < Weapons		.Count)	{	bool bCancel = false; if( -1 == Researched.Weapon		.Definitions.FindElement(	Weapons		.Slots[slotIndex].Entity.Index))	{ bCancel = true; printf("You can't access to %s weapons until you have researched them!\n"		, definitionsWeapon		[Weapons		.Slots[slotIndex].Entity.Index].Name.c_str()); } if( -1 == Researched.Weapon		.Modifiers.FindElement(Weapons		.Slots[slotIndex].Entity.Modifier))	{ bCancel = true; static char itemText[128] = {}; sprintf_s(itemText, modifiersWeapon		[Weapons		.Slots[slotIndex].Entity.Modifier].Name.c_str(), "Science"		); printf("You can't access to %s weapons until you have researched them!\n"		, itemText); }	if(bCancel) return; printf("You store %s level %u in your arsenal.\n"												, klib::getWeaponName		(CurrentWeapon		).c_str(), CurrentWeapon		.Level); UnloadWeapon		();	CurrentWeapon		= Weapons		.Slots[slotIndex].Entity;	Weapons		.DecreaseEntity(slotIndex);	printf("You equip %s level %u.\n"									, klib::getWeaponName		(CurrentWeapon		).c_str(), CurrentWeapon		.Level	); };	};
void SCharacter::EquipArmor			(size_t slotIndex)	{	if(slotIndex < Armors		.Count)	{	bool bCancel = false; if( -1 == Researched.Armor		.Definitions.FindElement(	Armors		.Slots[slotIndex].Entity.Index))	{ bCancel = true; printf("You can't access to %s armors until you have researched them!\n"		, definitionsArmor		[Armors			.Slots[slotIndex].Entity.Index].Name.c_str()); } if( -1 == Researched.Armor			.Modifiers.FindElement(Armors		.Slots[slotIndex].Entity.Modifier))	{ bCancel = true; static char itemText[128] = {}; sprintf_s(itemText, modifiersArmor		[Armors			.Slots[slotIndex].Entity.Modifier].Name.c_str(), "Technology"	); printf("You can't access to %s armors until you have researched them!\n"			, itemText); }	if(bCancel) return; printf("You leave %s level %u in your wardrobe.\n"												, klib::getArmorName		(CurrentArmor		).c_str(), CurrentArmor			.Level); UnloadArmor		();	CurrentArmor		= Armors		.Slots[slotIndex].Entity;	Armors		.DecreaseEntity(slotIndex);	printf("You equip %s level %u.\n"									, klib::getArmorName		(CurrentArmor		).c_str(), CurrentArmor			.Level	); };	};
void SCharacter::EquipProfession	(size_t slotIndex)	{	if(slotIndex < Professions	.Count)	{	bool bCancel = false; if( -1 == Researched.Profession	.Definitions.FindElement(	Professions	.Slots[slotIndex].Entity.Index))	{ bCancel = true; printf("You can't access to %s professions until you have researched them!\n"	, definitionsProfession	[Professions	.Slots[slotIndex].Entity.Index].Name.c_str()); } if( -1 == Researched.Profession	.Modifiers.FindElement(Professions	.Slots[slotIndex].Entity.Modifier))	{ bCancel = true; static char itemText[128] = {}; sprintf_s(itemText, modifiersProfession	[Professions	.Slots[slotIndex].Entity.Modifier].Name.c_str(), "Rank"			); printf("You can't access to %s professions until you have researched them!\n"	, itemText); }	if(bCancel) return; printf("You take a long vacation and forget the details about the %s level %u profession.\n"	, klib::getProfessionName	(CurrentProfession	).c_str(), CurrentProfession	.Level); UnloadProfession	();	CurrentProfession	= Professions	.Slots[slotIndex].Entity;	Professions	.DecreaseEntity(slotIndex);	printf("You get up to date with your profession as %s level %u.\n"	, klib::getProfessionName	(CurrentProfession	).c_str(), CurrentProfession	.Level	); };	};
void SCharacter::EquipVehicle		(size_t slotIndex)	{	if(slotIndex < Vehicles		.Count)	{	bool bCancel = false; if( -1 == Researched.Vehicle		.Definitions.FindElement(	Vehicles	.Slots[slotIndex].Entity.Index))	{ bCancel = true; printf("You can't access to %s vehicles until you have researched them!\n"	, definitionsVehicle	[Vehicles		.Slots[slotIndex].Entity.Index].Name.c_str()); } if( -1 == Researched.Vehicle		.Modifiers.FindElement(Vehicles		.Slots[slotIndex].Entity.Modifier))	{ bCancel = true; static char itemText[128] = {}; sprintf_s(itemText, modifiersVehicle		[Vehicles		.Slots[slotIndex].Entity.Modifier].Name.c_str(), "Mechanics"	); printf("You can't access to %s vehicles until you have researched them!\n"		, itemText); }	if(bCancel) return; printf("You get out of your %s level %u.\n"														, klib::getVehicleName		(CurrentVehicle		).c_str(), CurrentVehicle		.Level); UnloadVehicle		();	CurrentVehicle		= Vehicles		.Slots[slotIndex].Entity;	Vehicles	.DecreaseEntity(slotIndex);	printf("You get into your recently painted %s level %u.\n"			, klib::getVehicleName		(CurrentVehicle		).c_str(), CurrentVehicle		.Level	); };	};


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
