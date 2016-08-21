#include "GameBase.h"

#include <cstring>
#include <string>
#include <algorithm>

#ifndef __ARMOR_H__928364982734698273462834__
#define __ARMOR_H__928364982734698273462834__

namespace klib
{
	enum ARMOR_GRADE
	{	ARMOR_GRADE_NONE	= 0x00
	,	ARMOR_GRADE_LIGHT	= 0x01
	,	ARMOR_GRADE_MEDIUM	= 0x01
	,	ARMOR_GRADE_HEAVY	= 0x01
	};

	struct CArmor
	{
		int32_t			Absorption;
		ARMOR_GRADE			ArmorGrade;
		SCharacterPoints	Points;
		std::string			Name;
	};

	struct CArmorModifier
	{
		int32_t			Absorption;
		SCharacterPoints	Points;
		std::string			Name;
	};

static const CArmorModifier modifiersArmor[] = 
{	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"%s"					}
,	{	0	,	{	{	0,	0,	0	},	{	2,	1,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"%s of Regeneration"	}
,	{	2	,	{	{	0,	0,	1	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_STUN		},	"Tough %s"				}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_BURN		},	"Fireproof %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_SHOCK		},	"Insulating %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_POISON	},	"Biohazard %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_FREEZING	},	"Warm %s"				}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_FROZEN	},	"Self-Heating %s"		}
,	{	1	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_REFLECT										,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Reflective %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	(DEFEND_EFFECT)(DEFEND_EFFECT_REFLECT|DEFEND_EFFECT_BLIND)	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_BLIND	,	COMBAT_STATUS_BLIND		},	"Polarized %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	1,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_SHIELD_REPAIR	,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Regenerative %s"		}
};

static const CArmor definitionsArmor[] = 	
{	{	0	,	ARMOR_GRADE_NONE	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Underwear"				}
,	{	1	,	ARMOR_GRADE_LIGHT	,	{	{	0,	0,	1	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_BLIND		},	"Robe"					}
,	{	2	,	ARMOR_GRADE_LIGHT	,	{	{	0,	0,	2	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_FREEZING	},	"Leather Armor"			}
,	{	3	,	ARMOR_GRADE_LIGHT	,	{	{	0,	0,	3	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Wooden Armor"			}
,	{	4	,	ARMOR_GRADE_MEDIUM	,	{	{	0,	0,	4	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Chainmail Armor"		}
,	{	5	,	ARMOR_GRADE_HEAVY	,	{	{	0,	0,	5	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_BLEEDING	},	"Plate Armor"			}
,	{	6	,	ARMOR_GRADE_MEDIUM	,	{	{	0,	0,	6	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Kevlar Armor"			}
,	{	7	,	ARMOR_GRADE_HEAVY	,	{	{	0,	0,	7	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Diamond Armor"			}
,	{	8	,	ARMOR_GRADE_MEDIUM	,	{	{	0,	0,	8	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Dragon Scale Armor"	}
,	{	9	,	ARMOR_GRADE_HEAVY	,	{	{	0,	0,	8	},	{	0,	1,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_BLIND	,	COMBAT_STATUS_STUN		},	"Stardust Armor"		}
,	{	10	,	ARMOR_GRADE_HEAVY	,	{	{	0,	0,	10	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_SHIELD_REPAIR	,	COMBAT_STATUS_SHOCK	,	COMBAT_STATUS_NONE		},	"Mech Armor"			}
,	{	10	,	ARMOR_GRADE_MEDIUM	,	{	{	0,	0,	9	},	{	1,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_SHIELD_REPAIR	,	COMBAT_STATUS_SHOCK	,	COMBAT_STATUS_NONE		},	"Biomech Armor"			}
,	{	10	,	ARMOR_GRADE_LIGHT	,	{	{	0,	0,	9	},	{	0,	0,	0	},	{	1,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_IMPENETRABLE	,	PASSIVE_EFFECT_SHIELD_REPAIR	,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		},	"Force Field"			}
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

		multipliers.MaxLife		= {.125, .125, .5};
		multipliers.CurrentLife	= {.1, .1, .1};
		multipliers.Attack		= {.1, .1};
		multipliers.Coins		= 0.125;

		multipliers				= multipliers*armor.Level;

		return (definitionsArmor[armor.Index].Points + modifiersArmor[armor.Modifier].Points)*multipliers;
	}
}

#endif // __ARMOR_H__928364982734698273462834__
