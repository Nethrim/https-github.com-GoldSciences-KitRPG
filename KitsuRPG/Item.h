#pragma once

#include <string>

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
{	{ITEM_TYPE_UNKNOWN	,	PROPERTY_TYPE_NONE		,	0,	0	,	"This item doesn't exist. Bug?"	}	// the invalid description should be always in the last element
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	1,	10	,	"Small Potion of Healing"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	2,	20	,	"Potion of Healing"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH	,	3,	30	,	"Large Potion of Healing"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	1,	10	,	"Small Potion of Strength"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	2,	20	,	"Potion of Strength"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH	,	3,	30	,	"Large Potion of Strength"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	1,	10	,	"Small Potion of Focus"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	2,	20	,	"Potion of Focus"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT		,	3,	30	,	"Large Potion of Focus"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	1,	10	,	"Small Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	2,	20	,	"Grenade"						}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST		,	3,	30	,	"Large Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	1,	10	,	"Small Needle Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	2,	20	,	"Needle Grenade"				}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING	,	3,	30	,	"Large Needle Grenade"			}
};

#endif // __ITEM_H__98214809271346928734293846__