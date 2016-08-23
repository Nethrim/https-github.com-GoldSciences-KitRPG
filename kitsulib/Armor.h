#include "GameBase.h"

#include <cstring>
#include <string>
#include <algorithm>

#ifndef __ARMOR_H__928364982734698273462834__
#define __ARMOR_H__928364982734698273462834__

namespace klib
{


	struct CArmor
	{
		SCharacterPoints	Points;
		std::string			Name;
	};

	typedef CArmor CArmorModifier;

//	{HP, Mana, Shield}, {HP, Mana, Shield}, {Hit, Damage, Speed, Absorption}, dropCoins, {Attack, Defend, Passive}, {Status Inflict, Status Immunity}, Name.
static const CArmorModifier modifiersArmor[] = 
{	{ {{},{}		,{},0,{},{}																				,	{EQUIP_TECHNOLOGY_BASIC		}},	"%s"					}
,	{ {{},{2,1}		,{},0,{},{}																				,	{EQUIP_TECHNOLOGY_BASIC		}},	"%s of Regeneration"	}
,	{ {{0,0,1},{}	,{0,0,0,2},0,{}, {COMBAT_STATUS_NONE,COMBAT_STATUS_STUN	}								,	{EQUIP_TECHNOLOGY_BASIC		}},	"Tough %s"				}
,	{ {{},{}		,{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_BURN		}								,	{EQUIP_TECHNOLOGY_BASIC		}},	"Fireproof %s"			}
,	{ {{},{}		,{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_SHOCK	}									,	{EQUIP_TECHNOLOGY_BASIC		}},	"Insulating %s"			}
,	{ {{},{}		,{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_POISON	}									,	{EQUIP_TECHNOLOGY_BASIC		}},	"Biohazard %s"			}
,	{ {{},{}		,{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FREEZING	}								,	{EQUIP_TECHNOLOGY_BASIC		}},	"Warm %s"				}
,	{ {{},{}		,{},0,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FROZEN	}									,	{EQUIP_TECHNOLOGY_DIGITAL	}},	"Self-Heating %s"		}
,	{ {{},{}		,{0,0,0,1},0,{ATTACK_EFFECT_NONE, DEFEND_EFFECT_REFLECT},	{}							,	{EQUIP_TECHNOLOGY_BASIC		}},	"Reflective %s"			}
,	{ {{},{}		,{},0,{ATTACK_EFFECT_NONE, (DEFEND_EFFECT)(DEFEND_EFFECT_REFLECT|DEFEND_EFFECT_BLIND)}	,	{COMBAT_STATUS_BLIND, COMBAT_STATUS_BLIND}, { EQUIP_TECHNOLOGY_BASIC	}},	"Polarized %s"			}
,	{ {{},{1}		,{},0,{ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_SHIELD_REPAIR}, {}, {(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_GENETIC)		}},	"Regenerative %s"		}
};

//	{HP, Mana, Shield}, {HP, Mana, Shield}, {Hit, Damage, Speed, Absorption}, dropCoins, {Attack, Defend, Passive}, {Status Inflict, Status Immunity}, Name.
static const CArmor definitionsArmor[] = 	
{	{ 	{{}		,	{}		,{2,0,0,0},0,	{},{}											,	{EQUIP_TECHNOLOGY_BASIC, GRADE_LIGHT	}},	"Underwear"				}
,	{ 	{{0,0,1},	{}		,{0,0,0,1},0,	{},{}											,	{EQUIP_TECHNOLOGY_BASIC, GRADE_LIGHT	}},	"Robe"					}
,	{ 	{{0,0,2},	{}		,{0,0,0,2},0,	{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FREEZING}	,	{EQUIP_TECHNOLOGY_BASIC, GRADE_LIGHT	}},	"Leather Armor"			}
,	{ 	{{0,0,3},	{}		,{0,0,0,3},0,	{},{}											,	{EQUIP_TECHNOLOGY_BASIC, GRADE_MEDIUM	}},	"Wooden Armor"			}
,	{ 	{{0,0,4},	{}		,{0,0,0,4},0,	{},{}											,	{EQUIP_TECHNOLOGY_BASIC, GRADE_MEDIUM	}},	"Chainmail Armor"		}
,	{ 	{{0,0,5},	{}		,{0,0,0,5},0,	{},{COMBAT_STATUS_NONE, COMBAT_STATUS_BLEEDING}	,	{EQUIP_TECHNOLOGY_BASIC, GRADE_HEAVY	}},	"Plate Armor"			}
,	{ 	{{0,0,6},	{}		,{0,0,0,6},0,	{},{}											,	{EQUIP_TECHNOLOGY_BASIC, GRADE_MEDIUM	}},	"Kevlar Armor"			}
,	{ 	{{0,0,7},	{}		,{0,0,0,7},0,	{},{}											,	{EQUIP_TECHNOLOGY_BASIC, GRADE_HEAVY	}},	"Diamond Armor"			}
,	{ 	{{0,0,8},	{}		,{0,0,0,8},0,	{},{}											,	{EQUIP_TECHNOLOGY_BASIC, GRADE_MEDIUM	}},	"Dragon Scale Armor"	}
,	{ 	{{0,0,8},	{0,1}	,{0,0,0,9},0,	{},{COMBAT_STATUS_BLIND,COMBAT_STATUS_STUN}		,	{EQUIP_TECHNOLOGY_BASIC, GRADE_HEAVY	}},	"Stardust Armor"		}
,	{ 	{{0,0,10},	{}		,{0,0,0,10},0,	{ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_SHIELD_REPAIR},{COMBAT_STATUS_SHOCK},	{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_MECHANIC)								, GRADE_HEAVY	}},	"Mech Armor"	}
,	{ 	{{0,0,9},	{1}		,{0,0,0,10},0,	{ATTACK_EFFECT_NONE, DEFEND_EFFECT_NONE, PASSIVE_EFFECT_SHIELD_REPAIR},{COMBAT_STATUS_SHOCK},	{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_MECHANIC|EQUIP_TECHNOLOGY_GENETIC)	, GRADE_MEDIUM	}},	"Biomech Armor"	}
,	{ 	{{0,0,9},	{}		,{1,0,0,10},0,	{ATTACK_EFFECT_NONE,DEFEND_EFFECT_IMPENETRABLE,PASSIVE_EFFECT_SHIELD_REPAIR}, {},				{			EQUIP_TECHNOLOGY_DIGITAL																, GRADE_LIGHT	}},	"Force Field"}
};

	static std::string getArmorName(const SArmor& armor)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, modifiersArmor[armor.Modifier].Name.c_str(), definitionsArmor[armor.Index].Name.c_str());
		return formattedName;
	}

	static inline constexpr int32_t getArmorAbsorption(const SArmor& armor) {
		return	(int32_t)((definitionsArmor[armor.Index].Points.Attack.Absorption + modifiersArmor[armor.Modifier].Points.Attack.Absorption) * armor.Level);
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
