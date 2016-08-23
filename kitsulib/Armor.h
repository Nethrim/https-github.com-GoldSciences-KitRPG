#include "GameBase.h"

#include <cstring>
#include <string>
#include <algorithm>

#ifndef __ARMOR_H__928364982734698273462834__
#define __ARMOR_H__928364982734698273462834__

namespace klib
{
	struct CArmorModifier
	{
		int32_t				Absorption;
		SCharacterPoints	Points;
		std::string			Name;
	};

static const CArmorModifier modifiersArmor[] = 
{	{ 0,{{},{0,0},{},0,{},{}																			,	EQUIP_TECHNOLOGY_BASIC		},	"%s"					}
,	{ 0,{{},{2,1},{},0,{},{}																			,	EQUIP_TECHNOLOGY_BASIC		},	"%s of Regeneration"	}
,	{ 2,{{0,0,1},{0,0},{},0,{}, {COMBAT_STATUS_NONE,COMBAT_STATUS_STUN	}								,	EQUIP_TECHNOLOGY_BASIC		},	"Tough %s"				}
,	{ 0,{{},{0,0},{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_BURN		}								,	EQUIP_TECHNOLOGY_BASIC		},	"Fireproof %s"			}
,	{ 0,{{},{0,0},{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_SHOCK		}								,	EQUIP_TECHNOLOGY_BASIC		},	"Insulating %s"			}
,	{ 0,{{},{0,0},{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_POISON		}								,	EQUIP_TECHNOLOGY_BASIC		},	"Biohazard %s"			}
,	{ 0,{{},{0,0},{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FREEZING	}								,	EQUIP_TECHNOLOGY_BASIC		},	"Warm %s"				}
,	{ 0,{{},{0,0},{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FROZEN		}								,	EQUIP_TECHNOLOGY_DIGITAL	},	"Self-Heating %s"		}
,	{ 1,{{},{0,0},{},0,{ATTACK_EFFECT_NONE, DEFEND_EFFECT_REFLECT},	{}									,	EQUIP_TECHNOLOGY_BASIC		},	"Reflective %s"			}
,	{ 0,{{},{0,0},{},0,{ATTACK_EFFECT_NONE, (DEFEND_EFFECT)(DEFEND_EFFECT_REFLECT|DEFEND_EFFECT_BLIND)	},	{COMBAT_STATUS_BLIND, COMBAT_STATUS_BLIND}, EQUIP_TECHNOLOGY_BASIC	},	"Polarized %s"			}
,	{ 0,{{},{1,0},{},0,{ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_SHIELD_REPAIR}, {}, (EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_GENETIC)		},	"Regenerative %s"		}
};

	enum GRADE
	{	GRADE_NONE	= 0x00
	,	GRADE_LIGHT	= 0x01
	,	GRADE_MEDIUM	= 0x01
	,	GRADE_HEAVY	= 0x01
	};

	struct CArmor
	{
		int32_t				Absorption;
		GRADE			ArmorGrade;
		SCharacterPoints	Points;
		std::string			Name;
	};

static const CArmor definitionsArmor[] = 	
{	{ 0		, GRADE_NONE	,{{},		{},		{2},0,	{},{}											,	EQUIP_TECHNOLOGY_BASIC	},	"Underwear"				}
,	{ 1		, GRADE_LIGHT	,{{0,0,1},	{}		,{},0,	{},{}											,	EQUIP_TECHNOLOGY_BASIC	},	"Robe"					}
,	{ 2		, GRADE_LIGHT	,{{0,0,2},	{}		,{},0,	{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FREEZING}	,	EQUIP_TECHNOLOGY_BASIC	},	"Leather Armor"			}
,	{ 3		, GRADE_LIGHT	,{{0,0,3},	{}		,{},0,	{},{}											,	EQUIP_TECHNOLOGY_BASIC	},	"Wooden Armor"			}
,	{ 4		, GRADE_MEDIUM	,{{0,0,4},	{}		,{},0,	{},{}											,	EQUIP_TECHNOLOGY_BASIC	},	"Chainmail Armor"		}
,	{ 5		, GRADE_HEAVY	,{{0,0,5},	{}		,{},0,	{},{COMBAT_STATUS_NONE, COMBAT_STATUS_BLEEDING}	,	EQUIP_TECHNOLOGY_BASIC	},	"Plate Armor"			}
,	{ 6		, GRADE_MEDIUM	,{{0,0,6},	{}		,{},0,	{},{}											,	EQUIP_TECHNOLOGY_BASIC	},	"Kevlar Armor"			}
,	{ 7		, GRADE_HEAVY	,{{0,0,7},	{}		,{},0,	{},{}											,	EQUIP_TECHNOLOGY_BASIC	},	"Diamond Armor"			}
,	{ 8		, GRADE_MEDIUM	,{{0,0,8},	{}		,{},0,	{},{}											,	EQUIP_TECHNOLOGY_BASIC	},	"Dragon Scale Armor"	}
,	{ 9		, GRADE_HEAVY	,{{0,0,8},	{0,1}	,{},0,	{},{COMBAT_STATUS_BLIND,COMBAT_STATUS_STUN}		,	EQUIP_TECHNOLOGY_BASIC	},	"Stardust Armor"		}
,	{ 10	, GRADE_HEAVY	,{{0,0,10},	{}		,{},0,	{ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_SHIELD_REPAIR},{COMBAT_STATUS_SHOCK},(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_MECHANIC)								},	"Mech Armor"	}
,	{ 10	, GRADE_MEDIUM	,{{0,0,9},	{1}		,{},0,	{ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_SHIELD_REPAIR},{COMBAT_STATUS_SHOCK},(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_MECHANIC|EQUIP_TECHNOLOGY_GENETIC)	},	"Biomech Armor"	}
,	{ 10	, GRADE_LIGHT	,{{0,0,9},	{}		,{1},0,	{ATTACK_EFFECT_NONE,DEFEND_EFFECT_IMPENETRABLE,PASSIVE_EFFECT_SHIELD_REPAIR}, {},	EQUIP_TECHNOLOGY_DIGITAL	},	"Force Field"}
};

	static std::string getArmorName(const SArmor& armor)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, modifiersArmor[armor.Modifier].Name.c_str(), definitionsArmor[armor.Index].Name.c_str());
		return formattedName;
	}

	static inline constexpr int32_t getArmorAbsorption(const SArmor& armor) {
		return	(int32_t)((definitionsArmor[armor.Index].Absorption + modifiersArmor[armor.Modifier].Absorption) * armor.Level);
	}

	static SCharacterPoints getArmorPoints(const SArmor& armor) {
		SCharacterPointsMultipliers multipliers;

		multipliers.MaxLife		= {.1, .1, .25};
		multipliers.CurrentLife	= {.1, .1, .1};
		multipliers.Attack		= {.1, .1};
		multipliers.Coins		= 0.125;

		multipliers				= multipliers*armor.Level;

		return (definitionsArmor[armor.Index].Points + modifiersArmor[armor.Modifier].Points)*multipliers;
	}
}

#endif // __ARMOR_H__928364982734698273462834__
