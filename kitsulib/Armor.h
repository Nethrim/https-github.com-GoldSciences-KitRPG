#include "EntityImpl.h"
#include "EntityHelper.h"

#ifndef __ARMOR_H__928364982734698273462834__
#define __ARMOR_H__928364982734698273462834__

namespace klib
{
#pragma pack(push, 1)

#define MECH_ARMOR_EFFECTS			{klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_NONE,			klib::PASSIVE_EFFECT_SHIELD_REPAIR}
#define REGENERATIVE_TECHNOLOGY		((klib::ENTITY_TECHNOLOGY)(klib::ENTITY_TECHNOLOGY_DIGITAL|klib::ENTITY_TECHNOLOGY_GENETIC))
#define POLARIZED_EFFECTS			{{klib::ATTACK_EFFECT_NONE, (klib::DEFEND_EFFECT)(klib::DEFEND_EFFECT_REFLECT|klib::DEFEND_EFFECT_BLIND)}
#define POLARIZED_STATUS			{klib::COMBAT_STATUS_BLIND, klib::COMBAT_STATUS_BLIND}	

	//	{SEntityPoints, SEntityFlags, Name}
static const CRecordArmor modifiersArmor[] = 
{	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}},	"%s"					}
,	{{{0,0,0},	{2,1,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}},	"%s of Regeneration"	}
,	{{{0,0,1},	{0,0,0}	,{0,0,{0,0,0},2,{0,0,0}}	,0,0	},{{}, {COMBAT_STATUS_NONE,COMBAT_STATUS_STUN		}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Tough %s"				}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},{{},{COMBAT_STATUS_NONE, COMBAT_STATUS_BURN		}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Fireproof %s"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},{{},{COMBAT_STATUS_NONE, COMBAT_STATUS_SHOCK		}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Insulating %s"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},{{},{COMBAT_STATUS_NONE, COMBAT_STATUS_POISON		}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Biohazard %s"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},{{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FREEZING	}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Warm %s"				}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},{{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FROZEN		}	,{ENTITY_TECHNOLOGY_DIGITAL	}},	"Self-Heating %s"		}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},1,{0,0,0}}	,0,0	},{{ATTACK_EFFECT_NONE, DEFEND_EFFECT_REFLECT},	{}		,{ENTITY_TECHNOLOGY_BASIC	}},	"Reflective %s"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},	POLARIZED_EFFECTS,	POLARIZED_STATUS				,{ENTITY_TECHNOLOGY_BASIC	}},	"Polarized %s"			}
,	{{{0,0,0},	{1,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	},{	MECH_ARMOR_EFFECTS, {}								,{REGENERATIVE_TECHNOLOGY	}},	"Regenerative %s"		}
};

#define MECH_ARMOR_TECHNOLOGY		((klib::ENTITY_TECHNOLOGY)(klib::ENTITY_TECHNOLOGY_DIGITAL|klib::ENTITY_TECHNOLOGY_MECHANIC))
#define BIOMECH_ARMOR_TECHNOLOGY	((klib::ENTITY_TECHNOLOGY)(klib::ENTITY_TECHNOLOGY_DIGITAL|klib::ENTITY_TECHNOLOGY_MECHANIC|klib::ENTITY_TECHNOLOGY_GENETIC))
#define FORCE_FIELD_EFFECTS			{klib::ATTACK_EFFECT_NONE,	klib::DEFEND_EFFECT_IMPENETRABLE,	klib::PASSIVE_EFFECT_SHIELD_REPAIR}

//	{SEntityPoints, SEntityFlags, Name}
static const CRecordArmor definitionsArmor[] = 	
{	{{{0,0,0},	{0,0,0}	,{2,0,{0,0,0},0	,{0,0,0}}	,0,0	},	{{},{}												,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Underwear"				}
,	{{{0,0,1},	{0,0,0}	,{0,0,{0,0,0},1	,{0,0,0}}	,0,0	},	{{},{}												,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Robe"					}
,	{{{0,0,2},	{0,0,0}	,{0,0,{0,0,0},2	,{0,0,0}}	,0,0	},	{{},{COMBAT_STATUS_NONE, COMBAT_STATUS_FREEZING}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Leather Armor"			}
,	{{{0,0,3},	{0,0,0}	,{0,0,{0,0,0},3	,{0,0,0}}	,0,0	},	{{},{}												,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Wooden Armor"			}
,	{{{0,0,4},	{0,0,0}	,{0,0,{0,0,0},4	,{0,0,0}}	,0,0	},	{{},{}												,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Chainmail Armor"		}
,	{{{0,0,5},	{0,0,0}	,{0,0,{0,0,0},5	,{0,0,0}}	,0,0	},	{{},{COMBAT_STATUS_NONE, COMBAT_STATUS_BLEEDING}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}},	"Plate Armor"			}
,	{{{0,0,6},	{0,0,0}	,{0,0,{0,0,0},6	,{0,0,0}}	,0,0	},	{{},{}												,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Kevlar Armor"			}
,	{{{0,0,7},	{0,0,0}	,{0,0,{0,0,0},7	,{0,0,0}}	,0,0	},	{{},{}												,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}},	"Diamond Armor"			}
,	{{{0,0,8},	{0,0,0}	,{0,0,{0,0,0},8	,{0,0,0}}	,0,0	},	{{},{}												,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Dragon Scale Armor"	}
,	{{{0,0,8},	{0,1,0}	,{0,0,{0,0,0},9	,{0,0,0}}	,0,0	},	{{},{COMBAT_STATUS_BLIND,COMBAT_STATUS_STUN}		,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}},	"Stardust Armor"		}
,	{{{0,0,10},	{0,0,0}	,{0,0,{0,0,0},10,{0,0,0}}	,0,0	},	{MECH_ARMOR_EFFECTS	,{COMBAT_STATUS_SHOCK}			,{MECH_ARMOR_TECHNOLOGY		, ENTITY_GRADE_HEAVY	}},	"Mech Armor"			}
,	{{{0,0,9},	{1,0,0}	,{0,0,{0,0,0},10,{0,0,0}}	,0,0	},	{MECH_ARMOR_EFFECTS	,{COMBAT_STATUS_SHOCK}			,{BIOMECH_ARMOR_TECHNOLOGY	, ENTITY_GRADE_MEDIUM	}},	"Biomech Armor"			}
,	{{{0,0,9},	{0,0,0}	,{1,0,{0,0,0},10,{0,0,0}}	,0,0	},	{FORCE_FIELD_EFFECTS,{}								,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_LIGHT	}},	"Force Field"			}
};

	static std::string getArmorName(const SArmor& armor) {
		return getEntityName(armor, definitionsArmor, modifiersArmor);
	}

	static SEntityPoints getArmorPoints(const SArmor& armor) {
		return getEntityPoints(armor, definitionsArmor, modifiersArmor);
	}

	static SEntityFlags getArmorFlags(const SArmor& armor) {
		return  getEntityFlags(armor, definitionsArmor, modifiersArmor);
	}

	static inline constexpr int32_t getArmorAbsorption(const SArmor& armor) {
		return	(int32_t)((definitionsArmor[armor.Index].Points.Attack.Absorption + modifiersArmor[armor.Modifier].Points.Attack.Absorption) * armor.Level);
	}

#pragma pack(pop)
}

#endif // __ARMOR_H__928364982734698273462834__
