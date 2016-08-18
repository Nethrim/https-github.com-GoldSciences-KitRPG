#include <string>

#include "Menu.h"

#ifndef __ITEM_H__98214809271346928734293846__
#define __ITEM_H__98214809271346928734293846__

enum ITEM_TYPE : uint16_t
{	ITEM_TYPE_UNKNOWN	= 0x00
,	ITEM_TYPE_POTION	= 0x01
,	ITEM_TYPE_GRENADE	= 0x02
,	ITEM_TYPE_PACK		= 0x04
};

enum PROPERTY_TYPE : uint16_t
{	PROPERTY_TYPE_NONE		= 0x000
,	PROPERTY_TYPE_BLAST		= 0x001
,	PROPERTY_TYPE_PIERCING	= 0x002
,	PROPERTY_TYPE_STUN		= 0x004
,	PROPERTY_TYPE_SMOKE		= 0x008
,	PROPERTY_TYPE_FIRE		= 0x010
,	PROPERTY_TYPE_POISON	= 0x020
,	PROPERTY_TYPE_FREEZE	= 0x040
,	PROPERTY_TYPE_HEALTH	= 0x080
,	PROPERTY_TYPE_STRENGTH	= 0x100
,	PROPERTY_TYPE_HIT		= 0x200
};

struct CItem 
{
	ITEM_TYPE		Type;
	PROPERTY_TYPE	Property;
	int32_t			Price;
	std::string		Name;
};

struct CItemModifier
{
	std::string Name;
};

static const CItemModifier itemModifiers[] = 
{	{	"Prop %s"		}
,	{	"Small %s"		}
,	{	"Regular %s"	}
,	{	"Large %s"		}
,	{	"Huge %s"		}
,	{	"Pack %s"		}
};

static const CItem itemDefinitions[] = 
	// Type					Property				Price	Name
{	{ITEM_TYPE_UNKNOWN	,	PROPERTY_TYPE_NONE		,	0	,	"This item doesn't exist. Bug?"	}	// the invalid description should be always in the last element
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	10	,	"Potion of Healing"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	10	,	"Potion of Focus"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	10	,	"Potion of Strength"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_SMOKE		,	10	,	"Smoke Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_STUN		,	10	,	"Stun Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	10	,	"Grenade"						}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_FIRE		,	10	,	"Fire Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	10	,	"Needle Grenade"				}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_POISON	,	10	,	"Poison Grenade"				}
};


static std::string getItemName(const SItem& item)
{
	char formattedName[128] = {};
	sprintf_s(formattedName, itemModifiers[item.Modifier].Name.c_str(), itemDefinitions[item.Index].Name.c_str());
	return formattedName;
}

static int32_t getItemPrice(const SItem& item)
{
	return itemDefinitions[item.Index].Price*item.Modifier;
}

static void removeItem( SCharacterInventory& inventory, size_t indexInventory, const std::string ownerName)
{
	const std::string itemName = getItemName(inventory.Slots[indexInventory].Item);
	if( --inventory.Slots[indexInventory].ItemCount )
		printf("\n%s has %u %s left.\n", ownerName.c_str(), inventory.Slots[indexInventory].ItemCount, itemName.c_str());
	else 
	{
		inventory.Slots[indexInventory] = inventory.Slots[--inventory.ItemCount];
		printf("\n%s ran out of %s.\n", ownerName.c_str(), itemName.c_str());
	}
}
#endif // __ITEM_H__98214809271346928734293846__