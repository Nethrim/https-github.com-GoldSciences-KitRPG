#include <cstdint>

#include <string>

#ifndef __INVENTORY_H__9283409263409236409182312__
#define __INVENTORY_H__9283409263409236409182312__


namespace klib
{
	struct SItem
	{
		int32_t Index;
		int32_t Modifier;
	};

	struct SInventorySlot
	{
		SItem		Item;
		uint32_t	ItemCount;
	};

#define MAX_INVENTORY_SLOTS 128
	struct SCharacterInventory
	{
		uint32_t			ItemCount					= 0;
		SInventorySlot		Slots[MAX_INVENTORY_SLOTS]	= {};
	};

	bool addItem(SCharacterInventory& inventory, const SItem& item);
	void removeItem(SCharacterInventory& inventory, size_t indexInventory, const std::string& ownerName);
}
#endif // __INVENTORY_H__9283409263409236409182312__
