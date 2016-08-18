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
	uint32_t			Absorption;
	SCharacterPoints	Points;
	ARMOR_EFFECT		Effect;
	std::string			Name;
};

typedef CArmor CArmorModifier;

static const CArmorModifier armorModifiers[] = 
{	{	0,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"%s"					}
,	{	0,	{	{	0,	0,	0	},	{	2,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"%s of Regeneration"	}
,	{	1,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_REFLECT,	"Reflective %s"			}
,	{	0,	{	{	0,	0,	0	},	{	1,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_RECHARGE,	"Regenerative %s"		}
};

static const CArmor armorDefinitions[] = 	
{	{	0 ,	{	{	0,	0,	0 	},	{	0,	0,	0	},	{	2,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Underwear"				}
,	{	1 ,	{	{	0,	0,	1 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Robe"					}
,	{	2 ,	{	{	0,	0,	2 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Leather Armor"			}
,	{	3 ,	{	{	0,	0,	3 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Hard Leather Armor"	}
,	{	4 ,	{	{	0,	0,	4 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Wooden Armor"			}
,	{	5 ,	{	{	0,	0,	5 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Chainmail Armor"		}
,	{	6 ,	{	{	0,	0,	6 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Plate Armor"			}
,	{	7 ,	{	{	0,	0,	7 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Kevlar Armor"			}
,	{	8 ,	{	{	0,	0,	8 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Diamond Armor"			}
,	{	9 ,	{	{	0,	0,	9 	},	{	0,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Dragon Scale Armor"	}
,	{	10,	{	{	0,	0,	10	},	{	1,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_NONE,		"Stardust Armor"		}
,	{	10,	{	{	0,	0,	10	},	{	1,	0,	0	},	{	0,	0,	},	0	},	ARMOR_EFFECT_RECHARGE,	"Biomech Armor"			}
,	{	10,	{	{	0,	0,	10	},	{	0,	0,	0	},	{	0,	0,	},	0	},	(ARMOR_EFFECT)(ARMOR_EFFECT_RECHARGE | ARMOR_EFFECT_IMPENETRABLE),	"Force Field"	}
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
	return	(int32_t)(	armorDefinitions[armor.Index].Points.MaxLife.Shield		*	std::max(1.0000001, armor.Level*0.25	)	+	armorModifiers[armor.Modifier].Points.MaxLife.Shield		*	std::max(1.0000001, armor.Level*0.25	)	);
}

static inline constexpr int32_t getArmorAbsorption(const SArmor& armor) {
	return	(int32_t)(	armorDefinitions[armor.Index].Absorption * armor.Level + armorModifiers[armor.Modifier].Absorption * armor.Level);
}

static SCharacterPoints getArmorPoints(const SArmor& armor) {
	SCharacterPoints armorPoints = {};
	armorPoints.MaxLife.HP			= (int32_t)(	armorDefinitions[armor.Index].Points.MaxLife.HP			*	std::max(1.0000001, armor.Level*0.5		)	+	armorModifiers[armor.Modifier].Points.MaxLife.HP			*	std::max(1.0000001, armor.Level*0.5		)	);
	armorPoints.MaxLife.Mana		= (int32_t)(	armorDefinitions[armor.Index].Points.MaxLife.Mana		*	std::max(1.0000001, armor.Level*0.25	)	+	armorModifiers[armor.Modifier].Points.MaxLife.Mana			*	std::max(1.0000001, armor.Level*0.25	)	);
	armorPoints.MaxLife.Shield		= (int32_t)(	armorDefinitions[armor.Index].Points.MaxLife.Shield		*	std::max(1.0000001, armor.Level*0.25	)	+	armorModifiers[armor.Modifier].Points.MaxLife.Shield		*	std::max(1.0000001, armor.Level*0.25	)	);
	
	armorPoints.CurrentLife.HP		= (int32_t)(	armorDefinitions[armor.Index].Points.CurrentLife.HP		*	std::max(1.0000001, armor.Level*0.125	)	+	armorModifiers[armor.Modifier].Points.CurrentLife.HP		*	std::max(1.0000001, armor.Level*0.125	)	);
	armorPoints.CurrentLife.Mana	= (int32_t)(	armorDefinitions[armor.Index].Points.CurrentLife.Mana	*	std::max(1.0000001, armor.Level*0.125	)	+	armorModifiers[armor.Modifier].Points.CurrentLife.Mana		*	std::max(1.0000001, armor.Level*0.125	)	);
	armorPoints.CurrentLife.Shield	= (int32_t)(	armorDefinitions[armor.Index].Points.CurrentLife.Shield	*	std::max(1.0000001, armor.Level*0.25	)	+	armorModifiers[armor.Modifier].Points.CurrentLife.Shield	*	std::max(1.0000001, armor.Level*0.25	)	);
	
	armorPoints.Attack.Damage		= (int32_t)(	armorDefinitions[armor.Index].Points.Attack.Damage		*	std::max(1.0000001, armor.Level*0.1		)	+	armorModifiers[armor.Modifier].Points.Attack.Damage			*	std::max(1.0000001, armor.Level*0.1		)	);
	armorPoints.Attack.Hit			= (int32_t)(	armorDefinitions[armor.Index].Points.Attack.Hit			*	std::max(1.0000001, armor.Level*0.1		)	+	armorModifiers[armor.Modifier].Points.Attack.Hit			*	std::max(1.0000001, armor.Level*0.1		)	);

	armorPoints.Coins				= (int32_t)(	armorDefinitions[armor.Index].Points.Coins				*	std::max(1.0000001, armor.Level*0.1		)	+	armorModifiers[armor.Modifier].Points.Coins					*	std::max(1.0000001, armor.Level*0.1		)	);
	return armorPoints;
}

#endif // __ARMOR_H__928364982734698273462834__
