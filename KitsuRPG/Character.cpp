#include "Character.h"

void rest(SCharacterPoints& points)
{
	printf("\nYou decide to get some rest.\n");
	points.HP = points.MaxHP;
	printf("Your HP is: %u.\n", points.HP);
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

void showInventory(const SCharacter& adventurer)
{
	printf("\n-- Your inventory --\n");
	if(adventurer.Weapon)
		printf("-- You're carrying %s.\n", weaponDefinitions[adventurer.Weapon].Name.c_str());
	else
		printf("-- You're not carrying any weapons.\n");
	printf("-- You look at your wallet and count %u coins.\n", adventurer.Points.Coins);
	if(adventurer.Inventory.ItemCount) {
		printf("You look at the remaining supplies in your backpack...\n");
		for (unsigned int i = 0; i < adventurer.Inventory.ItemCount; i++)
			printf("%u: x%.2u %s.\n", i + 1, adventurer.Inventory.Slots[i].ItemCount, itemDescriptions[adventurer.Inventory.Slots[i].ItemIndex].Name.c_str());
		printf("\n");
	}

}

