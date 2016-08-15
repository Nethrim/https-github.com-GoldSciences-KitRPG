#include "GameBase.h"

#include <string>

#ifndef __ARMOR_H__928364982734698273462834__
#define __ARMOR_H__928364982734698273462834__

struct CArmor
{
	uint32_t			Shield;
	SCharacterPoints	Points;
	std::string			Name;
};

static const CArmor armorDefinitions[] = 
{	{	0,	{	0,	0,	0,		0,	0},	"None"									}
,	{	1,	{	0,	0,	0,		0,	0},	"Robe"									}
,	{	1,	{	0,	1,	0,		0,	0},	"Robe of Regeneration"					}
,	{	2,	{	0,	0,	0,		0,	0},	"Leather Armor"							}
,	{	2,	{	0,	0,	0,		0,	0},	"Leather Armor of Regeneration"			}
,	{	3,	{	0,	0,	0,		0,	0},	"Hard Leather Armor"					}
,	{	3,	{	0,	1,	0,		0,	0},	"Hard Leather Armor of Regeneration"	}
,	{	4,	{	0,	0,	0,		0,	0},	"Wooden Armor"							}
,	{	4,	{	0,	1,	0,		0,	0},	"Wooden Armor of Regeneration"			}
,	{	5,	{	0,	0,	0,		0,	0},	"Chainmail Armor"						}
,	{	5,	{	0,	1,	0,		0,	0},	"Chainmail Armor of Regeneration"		}
,	{	6,	{	0,	0,	0,		0,	0},	"Plate Armor"							}
,	{	6,	{	0,	1,	0,		0,	0},	"Plate Armor of Regeneration"			}
,	{	7,	{	0,	0,	0,		0,	0},	"Dragon Scale Armor"					}
,	{	7,	{	0,	1,	0,		0,	0},	"Dragon Scale Armor of Regeneration"	}
,	{	8,	{	0,	0,	0,		0,	0},	"Stardust Armor"						}
,	{	8,	{	0,	1,	0,		0,	0},	"Stardust Armor of Regeneration"		}
};

#endif // __ARMOR_H__928364982734698273462834__
