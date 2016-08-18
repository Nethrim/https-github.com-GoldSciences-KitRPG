#include "GameBase.h"

#include <cstring>
#include <string>
#include <algorithm>

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
{	{	0,	0,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"%s"					}
,	{	0,	0,	{	0,	0,	2,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"%s of Regeneration"	}
,	{	0,	1,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_REFLECT,	"Reflective %s"			}
,	{	0,	0,	{	0,	0,	1,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_RECHARGE,	"Regenerative %s"		}
};

static const CArmor armorDefinitions[] = 	
{	{	0,	0,	{	0,	0,	0,	0,		2,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Underwear"				}
,	{	1,	1,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Robe"					}
,	{	2,	2,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Leather Armor"			}
,	{	3,	3,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Hard Leather Armor"	}
,	{	4,	4,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Wooden Armor"			}
,	{	5,	5,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Chainmail Armor"		}
,	{	6,	6,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Plate Armor"			}
,	{	7,	7,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Kevlar Armor"			}
,	{	8,	8,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Diamond Armor"			}
,	{	9,	9,	{	0,	0,	0,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Dragon Scale Armor"	}
,	{	10,	10,	{	0,	0,	1,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_NONE,		"Stardust Armor"		}
,	{	10,	10,	{	0,	0,	1,	0,		0,	0,	0,	0	},	ARMOR_EFFECT_RECHARGE,	"Biomech Armor"			}
,	{	10,	10,	{	0,	0,	0,	0,		0,	0,	0,	0	},	(ARMOR_EFFECT)(ARMOR_EFFECT_RECHARGE | ARMOR_EFFECT_IMPENETRABLE),	"Force Field"	}
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
	return	(int32_t)(	armorDefinitions[armor.Index].Shield *	std::max(1.0000001, armor.Level*0.5)	+ armorModifiers[armor.Modifier].Shield *	std::max(1.0000001, armor.Level*0.5)	);
}

static inline constexpr int32_t getArmorAbsorption(const SArmor& armor) {
	return	(int32_t)(	armorDefinitions[armor.Index].Absorption * armor.Level + armorModifiers[armor.Modifier].Absorption * armor.Level);
}

static SCharacterPoints getArmorPoints(const SArmor& armor) {
	SCharacterPoints armorPoints = {};
	armorPoints.MaxHP	= (int32_t)(	armorDefinitions[armor.Index].Points.MaxHP	*	std::max(1.0000001, armor.Level*1.0)	+	armorModifiers[armor.Modifier].Points.MaxHP		*	std::max(1.0000001, armor.Level*1.0)	);
	armorPoints.Attack	= (int32_t)(	armorDefinitions[armor.Index].Points.Attack	*	std::max(1.0000001, armor.Level*0.34)	+	armorModifiers[armor.Modifier].Points.Attack	*	std::max(1.0000001, armor.Level*0.34)	);
	armorPoints.Hit		= (int32_t)(	armorDefinitions[armor.Index].Points.Hit	*	std::max(1.0000001, armor.Level*0.5)	+	armorModifiers[armor.Modifier].Points.Hit		*	std::max(1.0000001, armor.Level*0.5)	);
	armorPoints.HP		= (int32_t)(	armorDefinitions[armor.Index].Points.HP		*	std::max(1.0000001, armor.Level*0.2)	+	armorModifiers[armor.Modifier].Points.HP		*	std::max(1.0000001, armor.Level*0.2)	);
	armorPoints.Shield	= (int32_t)(	armorDefinitions[armor.Index].Points.Shield	*	std::max(1.0000001, armor.Level*0.25)	+	armorModifiers[armor.Modifier].Points.Shield	*	std::max(1.0000001, armor.Level*0.25)	);
	armorPoints.Coins	= (int32_t)(	armorDefinitions[armor.Index].Points.Coins	*	std::max(1.0000001, armor.Level*0.25)	+	armorModifiers[armor.Modifier].Points.Coins		*	std::max(1.0000001, armor.Level*0.25)	);
	return armorPoints;
}

#endif // __ARMOR_H__928364982734698273462834__
