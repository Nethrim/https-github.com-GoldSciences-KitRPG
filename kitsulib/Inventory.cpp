//#include "Inventory.h"
//#include "Item.h"
//#include "Misc.h"

//void klib::removeItem( SCharacterInventory& inventory, size_t indexInventory, const std::string& ownerName)
//{
//	const std::string itemName = getItemName(inventory.Slots[indexInventory].Item);
//	if( --inventory.Slots[indexInventory].Count )
//		printf("\n%s has %u %s left.\n", ownerName.c_str(), inventory.Slots[indexInventory].Count, itemName.c_str());
//	else 
//	{
//		inventory.Slots[indexInventory] = inventory.Slots[--inventory.ItemCount];
//		printf("\n%s ran out of %s.\n", ownerName.c_str(), itemName.c_str());
//	}
//}
//
//bool klib::addItem(SCharacterInventory& inventory, const SItem& item)
//{
//	// look in the inventory for the name so we just increment the counter instead of adding the item
//	std::string itemName = getItemName(item);
//	for(int i=0, count = inventory.ItemCount; i<count; i++) {
//		std::string itemNameOther = getItemName(inventory.Slots[i].Item);
//		if(itemName == itemNameOther) {
//			inventory.Slots[i].Count++;
//			return true;
//		}
//	}
//
//	if(inventory.ItemCount >= size(inventory.Slots))
//		return false;
//	else
//	{
//		// If we didn't return yet it means that the item was not found and we need to add it to the inventory.
//		inventory.Slots[inventory.ItemCount].Item	= item;
//		inventory.Slots[inventory.ItemCount].Count	= 1;
//		inventory.ItemCount++;
//		return true;
//	}
//}

