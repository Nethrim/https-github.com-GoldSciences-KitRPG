#pragma once

#include <string>
   
enum ITEM_TYPE
{	IT_UNKNOWN	= 0
,	IT_POTION
,	IT_GRENADE
};

enum PROPERTY_TYPE
{	PT_NONE	 = 0
,	PT_BLAST
,	PT_HEALTH
};

struct SItem 
{
	ITEM_TYPE		Type;
	PROPERTY_TYPE	Property;
	int				Grade;
	int				Price;
	std::string		Name;
};

#define MAX_ITEM_DESCRIPTIONS 6

static const SItem itemDescriptions[MAX_ITEM_DESCRIPTIONS+1] = // We add 1 for an extra element which is the invalid description. Useful for using as default for spotting bugs of items initialized incorrectly.
{	{IT_POTION,		PT_HEALTH	,	1,	50	, "Small HP Potion"					}
,	{IT_POTION,		PT_HEALTH	,	2,	100	, "HP Potion"						}
,	{IT_POTION,		PT_HEALTH	,	3,	150	, "Large HP Potion"					}
,	{IT_GRENADE,	PT_BLAST	,	1,	50	, "Small Grenade"					}
,	{IT_GRENADE,	PT_BLAST	,	2,	100	, "Grenade"							}
,	{IT_GRENADE,	PT_BLAST	,	3,	150	, "Large Grenade"					}
,	{IT_UNKNOWN,	PT_NONE		,	0,	0	, "This item doesn't exist. Bug?"	}	// the invalid description should be always in the last element
};