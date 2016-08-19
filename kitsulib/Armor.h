#include "GameBase.h"

#include <cstring>
#include <string>
#include <algorithm>

#ifndef __ARMOR_H__928364982734698273462834__
#define __ARMOR_H__928364982734698273462834__


struct CArmor
{
	uint32_t			Absorption;
	SCharacterPoints	Points;
	std::string			Name;
};

typedef CArmor CArmorModifier;

static const CArmorModifier armorModifiers[] = 
{	{	0,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"%s"					}
,	{	0,	{	{	0,	0,	0	},	{	2,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"%s of Regeneration"	}
,	{	1,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_REFLECT		,	PASSIVE_EFFECT_NONE		},	"Reflective %s"			}
,	{	0,	{	{	0,	0,	0	},	{	1,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_RECHARGE },	"Regenerative %s"		}
};											
											
static const CArmor armorDefinitions[] = 	
{	{	0 ,	{	{	0,	0,	0 	},	{	0,	0,	0	},	{	2,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Underwear"				}
,	{	1 ,	{	{	0,	0,	1 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Robe"					}
,	{	2 ,	{	{	0,	0,	2 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Leather Armor"			}
,	{	3 ,	{	{	0,	0,	3 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Hard Leather Armor"	}
,	{	4 ,	{	{	0,	0,	4 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Wooden Armor"			}
,	{	5 ,	{	{	0,	0,	5 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Chainmail Armor"		}
,	{	6 ,	{	{	0,	0,	6 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Plate Armor"			}
,	{	7 ,	{	{	0,	0,	7 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Kevlar Armor"			}
,	{	8 ,	{	{	0,	0,	8 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Diamond Armor"			}
,	{	9 ,	{	{	0,	0,	9 	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Dragon Scale Armor"	}
,	{	10,	{	{	0,	0,	10	},	{	1,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE		},	"Stardust Armor"		}
,	{	10,	{	{	0,	0,	10	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_RECHARGE	},	"Mech Armor"			}
,	{	10,	{	{	0,	0,	10	},	{	1,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_RECHARGE	},	"Biomech Armor"			}
,	{	10,	{	{	0,	0,	10	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_IMPENETRABLE	,	PASSIVE_EFFECT_RECHARGE	},	"Force Field"			}
};

static std::string getArmorName(const SArmor& armor)
{
	char formattedName[128] = {};
	sprintf_s(formattedName, armorModifiers[armor.Modifier].Name.c_str(), armorDefinitions[armor.Index].Name.c_str());
	return formattedName;
}

static inline constexpr int32_t getArmorAbsorption(const SArmor& armor) {
	return	(int32_t)(	armorDefinitions[armor.Index].Absorption * armor.Level + armorModifiers[armor.Modifier].Absorption * armor.Level);
}

static SCharacterPoints getArmorPoints(const SArmor& armor) {
	SCharacterPointsMultipliers multipliers;

	multipliers.MaxLife		= {.125, .125, .5};
	multipliers.CurrentLife	= {.1, .1, .1};
	multipliers.Attack		= {.1, .1};
	multipliers.Coins		= 0.125;

	multipliers				= multipliers*armor.Level;

	return (armorDefinitions[armor.Index].Points + armorModifiers[armor.Modifier].Points)*multipliers;
}

#endif // __ARMOR_H__928364982734698273462834__
