#pragma once

#include "Character.h"
#include "Item.h"

struct SInventorySlot
{
	SItem		Description;
	int			Count;
};

#define MAX_INVENTORY_SLOTS 3

// This class has the same properties as the base CCharacter and also adds the inventory which is unnecessary for other character types.
class CPlayer : public CCharacter
{
public:
	using CCharacter::CCharacter;	// This makes our CPlayer class to expose all the CCharacter constructors.

	int itemCount;
	SInventorySlot	inventory[MAX_INVENTORY_SLOTS];
};