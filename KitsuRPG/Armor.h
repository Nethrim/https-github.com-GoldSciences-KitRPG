#include "GameBase.h"

#include <cstring>
#include <string>

#ifndef __ARMOR_H__928364982734698273462834__
#define __ARMOR_H__928364982734698273462834__


enum ARMOR_EFFECT
{	ARMOR_EFFECT_NONE
,	ARMOR_EFFECT_REFLECT	= 0x01
};


struct CArmor
{
	uint32_t			Shield;
	uint32_t			Absorption;
	SCharacterPoints	Points;
	ARMOR_EFFECT		Effect;
	std::string			Name;
};

typedef CArmor CArmorModifier;

static const CArmorModifier armorModifiers[] = 
{	{	0,	0, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"%s"					}
,	{	0,	0, {	0,	1,	0,		0,	0},	ARMOR_EFFECT_NONE,		"%s of Regeneration"	}
,	{	0,	0, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_REFLECT,	"Reflective %s"			}
};

static const CArmor armorDefinitions[] = 	
{	{	0,	0, {	0,	0,	10,		0,	0},	ARMOR_EFFECT_NONE, "Underwear"				}
,	{	1,	10, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Robe"					}
,	{	2,	20, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Leather Armor"			}
,	{	3,	30, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Hard Leather Armor"		}
,	{	4,	40, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Wooden Armor"			}
,	{	5,	50, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Chainmail Armor"		}
,	{	6,	60, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Plate Armor"			}
,	{	7,	70, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Diamond Armor"			}
,	{	8,	80, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Dragon Scale Armor"		}
,	{	9,	90, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Stardust Armor"			}
,	{	10,	100, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE, "Force Field"			}
};

static std::string getArmorName(uint32_t armorDefinition, uint32_t armorModifier)
{
	char formattedName[128] = {};
	sprintf_s(formattedName, armorModifiers[armorModifier].Name.c_str(), armorDefinitions[armorDefinition].Name.c_str());
	return formattedName;
}

#endif // __ARMOR_H__928364982734698273462834__
