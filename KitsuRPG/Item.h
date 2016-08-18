#include <string>

#include "Menu.h"

#ifndef __ITEM_H__98214809271346928734293846__
#define __ITEM_H__98214809271346928734293846__

enum ITEM_TYPE : unsigned int
{	ITEM_TYPE_UNKNOWN	= 0
,	ITEM_TYPE_POTION
,	ITEM_TYPE_GRENADE
};

enum PROPERTY_TYPE : unsigned int
{	PROPERTY_TYPE_NONE	 = 0
,	PROPERTY_TYPE_BLAST
,	PROPERTY_TYPE_PIERCING
,	PROPERTY_TYPE_STUN
,	PROPERTY_TYPE_SMOKE
,	PROPERTY_TYPE_FIRE
,	PROPERTY_TYPE_POISON
,	PROPERTY_TYPE_FREEZE
,	PROPERTY_TYPE_HEALTH
,	PROPERTY_TYPE_STRENGTH
,	PROPERTY_TYPE_HIT
};

struct CItem 
{
	ITEM_TYPE		Type;
	PROPERTY_TYPE	Property;
	int32_t			Grade;
	int32_t			Price;
	std::string		Name;
};

struct SItem
{
	int32_t Index;
	int32_t Grade;
};

static const CItem itemDescriptions[] = 
	// Type					Property				Grade	Price	Name
{	{ITEM_TYPE_UNKNOWN	,	PROPERTY_TYPE_NONE		,	0,	0	,	"This item doesn't exist. Bug?"	}	// the invalid description should be always in the last element
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	1,	10	,	"Small Potion of Healing"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	2,	20	,	"Potion of Healing"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	3,	30	,	"Large Potion of Healing"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	1,	10	,	"Small Potion of Focus"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	2,	20	,	"Potion of Focus"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	3,	30	,	"Large Potion of Focus"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	1,	10	,	"Small Potion of Strength"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	2,	20	,	"Potion of Strength"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	3,	30	,	"Large Potion of Strength"		}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_SMOKE		,	1,	10	,	"Small Smoke Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_SMOKE		,	2,	20	,	"Smoke Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_SMOKE		,	3,	30	,	"Large Smoke Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_STUN		,	1,	10	,	"Small Stun Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_STUN		,	2,	20	,	"Stun Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_STUN		,	3,	30	,	"Large Stun Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	1,	10	,	"Small Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	2,	20	,	"Grenade"						}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	3,	30	,	"Large Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_FIRE		,	1,	10	,	"Small Fire Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_FIRE		,	2,	20	,	"Fire Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_FIRE		,	3,	30	,	"Large Fire Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	1,	10	,	"Small Needle Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	2,	20	,	"Needle Grenade"				}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	3,	30	,	"Large Needle Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_POISON	,	1,	10	,	"Small Poison Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_POISON	,	2,	20	,	"Poison Grenade"				}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_POISON	,	3,	30	,	"Large Poison Grenade"			}
};

template<size_t _Size>
static int initializeItemMenu(SMenuItem<uint32_t>(&menuItems)[_Size])
{
	char itemOption[128] = {};
	static const size_t descriptionCount = size(itemDescriptions);
	for(uint32_t i=0, count = descriptionCount-1; i<count; ++i)
	{
		sprintf_s(itemOption, "- $%.2u Coins - %s", itemDescriptions[i+1].Price, itemDescriptions[i+1].Name.c_str());
		menuItems[i].ReturnValue	= i+1;
		menuItems[i].Text			= itemOption;
	}
	menuItems[descriptionCount-1].ReturnValue	= descriptionCount;
	menuItems[descriptionCount-1].Text		= "Leave the bar";
	return 0;
};

static void removeItem( SCharacterInventory& inventory, size_t indexInventory, const std::string ownerName)
{
	const std::string& itemName = itemDescriptions[inventory.Slots[indexInventory].ItemIndex].Name;
	if( --inventory.Slots[indexInventory].ItemCount )
		printf("\n%s has %u %s left.\n", ownerName.c_str(), inventory.Slots[indexInventory].ItemCount, itemName.c_str());
	else 
	{
		inventory.Slots[indexInventory] = inventory.Slots[--inventory.ItemCount];
		printf("\n%s ran out of %s.\n", ownerName.c_str(), itemName.c_str());
	}
}
#endif // __ITEM_H__98214809271346928734293846__