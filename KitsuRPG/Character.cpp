#include "Character.h"

void rest(SCharacterPoints& points)
{
	printf("\nYou decide to get some rest.\n");
	points.HP = points.MaxHP;
	printf("Your HP is: %u.\n", points.HP);
}

bool addItem(CCharacter& adventurer, const SItem& itemDescription)
{
	// look in the inventory for the name so we just increment the counter instead of adding the item
	for(int i=0, count = adventurer.ItemCount; i<count; i++) {
		if(itemDescription.Name == adventurer.Inventory[i].Description.Name) {
			adventurer.Inventory[i].Count++;
			return true;
		}
	}

	if(adventurer.ItemCount >= size(adventurer.Inventory))
		return false;
	else
	{
		// If we didn't return yet it means that the item was not found and we need to add it to the inventory.
		adventurer.Inventory[adventurer.ItemCount].Description	= itemDescription;
		adventurer.Inventory[adventurer.ItemCount].Count		= 1;
		adventurer.ItemCount++;
		return true;
	}
}

void showInventory(const CCharacter& adventurer)
{
	printf("\n-- Your inventory --\n\n");
	printf("-- You look at your wallet and count %u coins.\n\n", adventurer.Points.Coins);
	if(adventurer.ItemCount) {
		printf("You look at the remaining supplies in your backpack...\n");
		for (unsigned int i = 0; i < adventurer.ItemCount; i++)
			printf("%u: x%.2u %s.\n", i + 1, adventurer.Inventory[i].Count, adventurer.Inventory[i].Description.Name.c_str());
		printf("\n");
	}
}

