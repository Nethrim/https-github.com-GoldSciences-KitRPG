#include "Character.h"

#include "Weapon.h"
#include "Item.h"
#include "Armor.h"
#include "Profession.h"
#include "Vehicle.h"
#include "Facility.h"
#include "Accessory.h"

#include "Misc.h"

using namespace klib;

SEntityPoints klib::calculateFinalPoints(const SCharacter& character)
{
	const SEntityPoints		weaponPoints		= klib::getWeaponPoints		(character.CurrentEquip.Weapon);
	const SEntityPoints		accessoryPoints		= klib::getAccessoryPoints	(character.CurrentEquip.Accessory);
	const SEntityPoints		armorPoints			= klib::getArmorPoints		(character.CurrentEquip.Armor);
	const SEntityPoints		professionPoints	= klib::getProfessionPoints	(character.CurrentEquip.Profession);
	const SEntityPoints		vehiclePoints		= klib::getVehiclePoints	(character.CurrentEquip.Vehicle);
	const SEntityPoints		facilityPoints		= klib::getFacilityPoints	(character.CurrentEquip.Facility);
	const SEntityPoints&	bonusPoints			= character.ActiveBonus.Points.Points;

	// Currently, SEntityPoints::Coins and SEntityPoints::LifeCurrent values of the equipment are used in a different 
	// way from the character's points so we avoid adding the character points to the result for these two.
	SEntityPoints result	= bonusPoints + weaponPoints + accessoryPoints + facilityPoints + armorPoints + vehiclePoints + professionPoints;
	result.LifeMax			= result.LifeMax			+ character.Points.LifeMax;	// 
	result.Attack			= result.Attack				+ character.Points.Attack;	// 
	result.PriceBuy			= result.PriceBuy			+ character.Points.PriceBuy;	// 
	result.CostMaintenance	= result.CostMaintenance	+ character.Points.CostMaintenance;	// 
	return result;
};

SEntityFlags klib::calculateFinalFlags(const SCharacter& character)
{
	SEntityFlags result = {};
	const SEntityFlags	weaponFlags			= klib::getWeaponFlags		(character.CurrentEquip.Weapon);
	const SEntityFlags	accessoryFlags		= klib::getAccessoryFlags	(character.CurrentEquip.Accessory);
	const SEntityFlags	armorFlags			= klib::getArmorFlags		(character.CurrentEquip.Armor);
	const SEntityFlags	professionFlags		= klib::getProfessionFlags	(character.CurrentEquip.Profession);
	const SEntityFlags	vehicleFlags		= klib::getVehicleFlags		(character.CurrentEquip.Vehicle);
	const SEntityFlags	facilityFlags		= klib::getFacilityFlags	(character.CurrentEquip.Facility);
	const SEntityFlags&	bonusFlags			= character.ActiveBonus.Points.Flags;
	
	result	= bonusFlags | weaponFlags | accessoryFlags | armorFlags | vehicleFlags | professionFlags | facilityFlags | character.Flags;
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
