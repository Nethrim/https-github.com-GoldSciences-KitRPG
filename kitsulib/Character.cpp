#include "Character.h"

#include "Weapon.h"
#include "Item.h"
#include "Armor.h"
#include "Profession.h"
#include "Vehicle.h"

#include "Misc.h"

using namespace klib;

void klib::rest(SCharacter& character)
{
	const SEntityPoints finalPoints	= calculateFinalPoints(character);
	character.Points.LifeCurrent	= finalPoints.LifeMax	;
	printf("\nYou decide to get some rest.\n");
	character.Points.LifeCurrent.Print();
}

SEntityPoints klib::calculateFinalPoints(const SCharacter& character)
{
	SEntityPoints result = {};
	const SEntityPoints	weaponPoints		= klib::getWeaponPoints		(character.CurrentEquip.Weapon);
	const SEntityPoints	armorPoints			= klib::getArmorPoints		(character.CurrentEquip.Armor);
	const SEntityPoints	professionPoints	= klib::getProfessionPoints	(character.CurrentEquip.Profession);
	const SEntityPoints	vehiclePoints		= klib::getVehiclePoints	(character.CurrentEquip.Vehicle);

	result.LifeMax		= character.ActiveBonus.Points.Points.LifeMax		+ weaponPoints.LifeMax		+ armorPoints.LifeMax		+ vehiclePoints.LifeMax		+ professionPoints.LifeMax		+	character.Points.LifeMax;
	result.Attack		= character.ActiveBonus.Points.Points.Attack		+ weaponPoints.Attack		+ armorPoints.Attack		+ vehiclePoints.Attack		+ professionPoints.Attack		+	character.Points.Attack;
	result.LifeCurrent	= character.ActiveBonus.Points.Points.LifeCurrent	+ weaponPoints.LifeCurrent	+ armorPoints.LifeCurrent	+ vehiclePoints.LifeCurrent	+ professionPoints.LifeCurrent;
	result.Coins		= character.ActiveBonus.Points.Points.Coins			+ weaponPoints.Coins		+ armorPoints.Coins			+ vehiclePoints.Coins		+ professionPoints.Coins;
	result.Price		= character.ActiveBonus.Points.Points.Price			+ weaponPoints.Price		+ armorPoints.Price			+ vehiclePoints.Price		+ professionPoints.Price;

	return result;
};

SEntityFlags klib::calculateFinalFlags(const SCharacter& character)
{
	SEntityFlags result = {};
	const SEntityFlags	weaponFlags			= klib::getWeaponFlags		(character.CurrentEquip.Weapon);
	const SEntityFlags	armorFlags			= klib::getArmorFlags		(character.CurrentEquip.Armor);
	const SEntityFlags	professionFlags		= klib::getProfessionFlags	(character.CurrentEquip.Profession);
	const SEntityFlags	vehicleFlags		= klib::getVehicleFlags		(character.CurrentEquip.Vehicle);
	result	= character.ActiveBonus.Points.Flags | weaponFlags | armorFlags | vehicleFlags | professionFlags | character.Flags;

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


void SCharacter::EquipWeapon		(size_t slotIndex)	{	
	
	equipEntityIfResearched
		( slotIndex
		, Inventory.Weapons
		, Researched.Weapon
		, definitionsWeapon
		, modifiersWeapon
		, CurrentEquip.Weapon
		, "You can't access to %s weapons until you have researched them!\n"	
		, "You can't access to %s weapons until you have researched them!\n"	
		, "You store %s level %u in your arsenal.\n"
		, "You equip %s level %u.\n"
		, "Science"
		);

}

void SCharacter::EquipArmor		(size_t slotIndex)	{	
	
	equipEntityIfResearched
		( slotIndex
		, Inventory.Armors
		, Researched.Armor
		, definitionsArmor
		, modifiersArmor
		, CurrentEquip.Armor
		, "You can't access to %s armors until you have researched them!\n"		
		, "You can't access to %s armors until you have researched them!\n"		
		, "You leave %s level %u in your wardrobe.\n"		
		, "You equip %s level %u.\n"
		, "Technology"
		);

}

void SCharacter::EquipProfession		(size_t slotIndex)	{	
	
	equipEntityIfResearched
		( slotIndex
		, Inventory.Professions
		, Researched.Profession
		, definitionsProfession
		, modifiersProfession
		, CurrentEquip.Profession
		, "You can't access to %s professions until you have researched them!\n"	
		, "You can't access to %s professions until you have researched them!\n"	
		, "You take a long vacation and forget the details about the %s level %u profession.\n"
		, "You get up to date with your profession as %s level %u.\n"
		, "Rank"
		);

}

void SCharacter::EquipVehicle		(size_t slotIndex)	{	
	
	equipEntityIfResearched
		( slotIndex
		, Inventory.Vehicles
		, Researched.Vehicle
		, definitionsVehicle
		, modifiersVehicle
		, CurrentEquip.Vehicle
		, "You can't access to %s vehicles until you have researched them!\n"
		, "You can't access to %s vehicles until you have researched them!\n"
		, "You get out of your %s level %u.\n"
		, "You get into your recently painted %s level %u.\n"
		, "Mechanics"
		);
}

int	SCharacter::Save(FILE* fp)	const
{	
	if(0 == fp) {	
		printf("Cannot save to file!"	); 
		return -1; 
	}	
	if( 1 != fwrite	(this, 1, sizeof(SCharacter), fp) )				
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
	if( 1 != fread	(&loadedCharacter, 1, sizeof(SCharacter), fp) ) { 
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
	char name[128] = {};
	if(nameLen)
		if( 1 != fread(name, 1, (nameLen < 127) ? nameLen : 127, fp) )
		{
			printf("Failed to load character name!");
			return -1;
		}
	Name = name;
	return 0; 
};
