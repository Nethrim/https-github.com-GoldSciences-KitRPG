#pragma once

#include <string>
   
enum ITEM_TYPE : unsigned int
{	ITEM_TYPE_UNKNOWN	= 0
,	ITEM_TYPE_POTION
,	ITEM_TYPE_GRENADE
};

enum PROPERTY_TYPE : unsigned int
{	PROPERTY_TYPE_NONE	 = 0
,	PROPERTY_TYPE_BLAST
,	PROPERTY_TYPE_PIERCING
,	PROPERTY_TYPE_HEALTH
,	PROPERTY_TYPE_STRENGTH
,	PROPERTY_TYPE_HIT
};

struct SItem 
{
	ITEM_TYPE		Type;
	PROPERTY_TYPE	Property;
	int				Grade;
	int				Price;
	std::string		Name;
};

static const SItem itemDescriptions[] = 
	// Type					Property				Grade	Price	Name
{	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	1,	10	,	"Small HP Potion"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	2,	20	,	"HP Potion"						}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	3,	30	,	"Large HP Potion"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	1,	10	,	"Small Strength Potion"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	2,	20	,	"Strength Potion"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	3,	30	,	"Large Strength Potion"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	1,	10	,	"Small Focus Potion"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	2,	20	,	"Focus Potion"					}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	3,	30	,	"Large Focus Potion"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	1,	10	,	"Small Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	2,	20	,	"Grenade"						}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	3,	30	,	"Large Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	1,	10	,	"Small Needle Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	2,	20	,	"Needle Grenade"				}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	3,	30	,	"Large Needle Grenade"			}
,	{ITEM_TYPE_UNKNOWN	,	PROPERTY_TYPE_NONE		,	0,	0	,	"This item doesn't exist. Bug?"	}	// the invalid description should be always in the last element
};

template<size_t _Size>
static inline constexpr size_t getDescriptionCount(const SItem (&descriptionArray)[_Size]) { return size(descriptionArray)-1; }