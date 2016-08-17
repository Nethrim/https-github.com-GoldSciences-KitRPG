#include "GameBase.h"

#include <cstring>
#include <string>

#ifndef __ARMOR_H__928364982734698273462834__
#define __ARMOR_H__928364982734698273462834__


enum ARMOR_EFFECT
{	ARMOR_EFFECT_NONE			= 0
,	ARMOR_EFFECT_REFLECT		= 0x01
,	ARMOR_EFFECT_RECHARGE		= 0x02
,	ARMOR_EFFECT_IMPENETRABLE	= 0x04
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
,	{	0,	0, {	0,	1,	0,		0,	0},	ARMOR_EFFECT_RECHARGE,	"Regenerative %s"		}
};

static const CArmor armorDefinitions[] = 	
{	{	0,	0, {	0,	0,	10,		0,	0},	ARMOR_EFFECT_NONE,		"Underwear"				}
,	{	1,	10, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Robe"					}
,	{	2,	20, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Leather Armor"			}
,	{	3,	30, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Hard Leather Armor"	}
,	{	4,	40, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Wooden Armor"			}
,	{	5,	50, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Chainmail Armor"		}
,	{	6,	60, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Plate Armor"			}
,	{	7,	70, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Diamond Armor"			}
,	{	8,	80, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Dragon Scale Armor"	}
,	{	9,	90, {	0,	0,	0,		0,	0},	ARMOR_EFFECT_NONE,		"Stardust Armor"		}
,	{	10,	100, {	0,	0,	0,		0,	0},	(ARMOR_EFFECT)(ARMOR_EFFECT_RECHARGE | ARMOR_EFFECT_IMPENETRABLE),	"Force Field"	}
};

static std::string getArmorName(const SArmor& armor)
{
	char formattedName[128] = {};
	sprintf_s(formattedName, armorModifiers[armor.Modifier].Name.c_str(), armorDefinitions[armor.Index].Name.c_str());
	return formattedName;
}

static inline constexpr ARMOR_EFFECT getArmorEffect(const SArmor& armor) {
	return ARMOR_EFFECT(armorDefinitions[armor.Index].Effect | armorModifiers[armor.Modifier].Effect);
}

static inline constexpr int32_t getArmorShield(const SArmor& armor) {
	return armorDefinitions[armor.Index].Shield + armorModifiers[armor.Modifier].Shield;
}

static inline constexpr int32_t getArmorAbsorption(const SArmor& armor) {
	return armorDefinitions[armor.Index].Absorption + armorModifiers[armor.Modifier].Absorption;
}

static SCharacterPoints getArmorPoints(const SArmor& armor) {
	SCharacterPoints armorPoints = {};
	armorPoints.MaxHP	= armorDefinitions[armor.Index].Points.MaxHP	+	armorModifiers[armor.Modifier].Points.MaxHP;
	armorPoints.Attack	= armorDefinitions[armor.Index].Points.Attack	+	armorModifiers[armor.Modifier].Points.Attack;
	armorPoints.Hit		= armorDefinitions[armor.Index].Points.Hit		+	armorModifiers[armor.Modifier].Points.Hit;
	armorPoints.HP		= armorDefinitions[armor.Index].Points.HP		+	armorModifiers[armor.Modifier].Points.HP;
	armorPoints.Coins	= armorDefinitions[armor.Index].Points.Coins	+	armorModifiers[armor.Modifier].Points.Coins;
	return armorPoints;
}

#endif // __ARMOR_H__928364982734698273462834__
