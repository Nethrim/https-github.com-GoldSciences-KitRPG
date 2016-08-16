#include "Character.h"

#include "Weapon.h"
#include "Item.h"
#include "Armor.h"

#include "Misc.h"

#include <algorithm>

void rest(CCharacter& character)
{
	SCharacterPoints finalPoints = calculateFinalPoints(character);
	character.Points.HP = finalPoints.MaxHP;
	printf("\nYou decide to get some rest.\n");
	printf("Your HP is: %u.\n", character.Points.HP);
}

bool addItem(SCharacterInventory& inventory, uint32_t itemIndex)
{
	// look in the inventory for the name so we just increment the counter instead of adding the item
	for(int i=0, count = inventory.ItemCount; i<count; i++) {
		if(itemDescriptions[itemIndex].Name == itemDescriptions[inventory.Slots[i].ItemIndex].Name) {
			inventory.Slots[i].ItemCount++;
			return true;
		}
	}

	if(inventory.ItemCount >= size(inventory.Slots))
		return false;
	else
	{
		// If we didn't return yet it means that the item was not found and we need to add it to the inventory.
		inventory.Slots[inventory.ItemCount].ItemIndex	= itemIndex;
		inventory.Slots[inventory.ItemCount].ItemCount	= 1;
		inventory.ItemCount++;
		return true;
	}
}

SCharacterPoints calculateFinalPoints(const CCharacter& character)
{
	SCharacterPoints result;
	const CWeapon&			weaponDefinition	= weaponDefinitions	[character.Weapon];
	const CWeaponModifier&	weaponModifier		= weaponModifiers	[character.WeaponModifier];
	const CArmor&			armorDefinition		= armorDefinitions	[character.Armor];
	const CArmor&			armorModifier		= armorModifiers	[character.ArmorModifier];

	result.MaxHP	= character.Points.MaxHP	+	character.CombatBonus.Points.MaxHP	+	weaponDefinition.Points.MaxHP	+	weaponModifier.Points.MaxHP		+	armorDefinition.Points.MaxHP	+	armorModifier.Points.MaxHP	;
	result.HP		=								character.CombatBonus.Points.HP		+	weaponDefinition.Points.HP		+	weaponModifier.Points.HP		+	armorDefinition.Points.HP		+	armorModifier.Points.HP		;
	result.Hit		= character.Points.Hit		+	character.CombatBonus.Points.Hit	+	weaponDefinition.Points.Hit		+	weaponModifier.Points.Hit		+	armorDefinition.Points.Hit		+	armorModifier.Points.Hit	;
	result.Attack	= character.Points.Attack	+	character.CombatBonus.Points.Attack	+	weaponDefinition.Points.Attack	+	weaponModifier.Points.Attack	+	armorDefinition.Points.Attack	+	armorModifier.Points.Attack	;
	result.Coins	=								character.CombatBonus.Points.Coins	+	weaponDefinition.Points.Coins	+	weaponModifier.Points.Coins		+	armorDefinition.Points.Coins	+	armorModifier.Points.Coins	;

	return result;
};

bool addStatus(SCombatStatus& characterStatus, STATUS_TYPE statusType, uint32_t turnCount)
{
	for(int i=0, count=MAX_STATUS_COUNT; i<count; ++i)
	{
		STATUS_TYPE bitStatus =  (STATUS_TYPE)(1<<i);
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

void showInventory(const SCharacter& adventurer)
{
	printf("\n-- Your inventory --\n");
	if(adventurer.Weapon)
		printf("-- You're carrying %s.\n", getWeaponName(adventurer.Weapon, adventurer.WeaponModifier).c_str());
	else
		printf("-- You're not carrying any weapons.\n");

	if(adventurer.Armor)
		printf("-- You're wearing %s.\n", getArmorName(adventurer.Armor, adventurer.ArmorModifier).c_str());
	else
		printf("-- You're not wearing any armor.\n");

	printf("-- You look at your wallet and count %u coins.\n", adventurer.Points.Coins);
	if(adventurer.Inventory.ItemCount) {
		printf("You look at the remaining supplies in your backpack...\n");
		for (unsigned int i = 0; i < adventurer.Inventory.ItemCount; i++)
			printf("%u: x%.2u %s.\n", i + 1, adventurer.Inventory.Slots[i].ItemCount, itemDescriptions[adventurer.Inventory.Slots[i].ItemIndex].Name.c_str());
		printf("\n");
	}

}

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