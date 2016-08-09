#include "Player.h"

void addItem(CPlayer& adventurer, const SItem& itemDescription)
{
	// look in the inventory for the name so we just increment the counter instead of adding the item
	for(int i=0, count = adventurer.itemCount; i<count; i++) {
		if(itemDescription.Name == adventurer.inventory[i].Description.Name) {
			adventurer.inventory[i].Count++;
			return;
		}
	}

	// If we didn't return yet it means that the item was not found and we need to add it to the inventory.
	adventurer.inventory[adventurer.itemCount].Description	= itemDescription;
	adventurer.inventory[adventurer.itemCount].Count		= 1;
	adventurer.itemCount++;
}

void showInventory(const CPlayer& adventurer)
{
	printf("\n-- Your inventory --\n\n");
	printf("-- You look at your wallet and count %u coins.\n\n", adventurer.Points.Coins);
	if(adventurer.itemCount) {
		printf("You look at the remaining supplies in your backpack...\n");
		for (unsigned int i = 0; i < adventurer.itemCount; i++)
			printf("%u: x%.2u %s.\n", i + 1, adventurer.inventory[i].Count, adventurer.inventory[i].Description.Name.c_str());
		printf("\n");
	}
}
