#pragma once

#include "Character.h"
#include "Item.h"

struct SInventorySlot
{
	SItem		Description;
	int			Count;
};

#define MAX_INVENTORY_SLOTS 10

// This class has the same properties as the base CCharacter and also adds the inventory which is unnecessary for other character types.
class CPlayer : public CCharacter
{
public:
	using CCharacter::CCharacter;	// This makes our CPlayer class to expose all the CCharacter constructors.

	int itemCount;
	SInventorySlot	inventory[MAX_INVENTORY_SLOTS];
};

template<size_t _Size>
static inline constexpr int getInventorySize(const SInventorySlot (&inventoryArray)[_Size]) {
	return ((int)_Size);
}