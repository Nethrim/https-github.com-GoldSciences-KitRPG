#pragma once

#include <string>
   
enum ITEM_TYPE : unsigned int
{	IT_UNKNOWN	= 0
,	IT_POTION
,	IT_GRENADE
};

enum PROPERTY_TYPE : unsigned int
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

static const SItem itemDescriptions[] = 
	// Type			Property	Grade	Price	Name
{	{IT_POTION	,	PT_HEALTH	,	1,	50	,	"Small HP Potion"				}
,	{IT_POTION	,	PT_HEALTH	,	2,	100	,	"HP Potion"						}
,	{IT_POTION	,	PT_HEALTH	,	3,	150	,	"Large HP Potion"				}
,	{IT_GRENADE	,	PT_BLAST	,	1,	50	,	"Small Grenade"					}
,	{IT_GRENADE	,	PT_BLAST	,	2,	100	,	"Grenade"						}
,	{IT_GRENADE	,	PT_BLAST	,	3,	150	,	"Large Grenade"					}
,	{IT_UNKNOWN	,	PT_NONE		,	0,	0	,	"This item doesn't exist. Bug?"	}	// the invalid description should be always in the last element
};

template<size_t _Size>
static inline constexpr int getDescriptionCount(const SItem (&descriptionArray)[_Size]) {
	return ((int)_Size)-1;
}