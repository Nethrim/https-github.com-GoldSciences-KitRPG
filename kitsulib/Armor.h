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
		uint32_t			Absorption;
		ARMOR_GRADE			ArmorGrade;
		SCharacterPoints	Points;
		std::string			Name;
	};

	struct CArmorModifier
	{
		uint32_t			Absorption;
		SCharacterPoints	Points;
		std::string			Name;
	};

static const CArmorModifier armorModifiers[] = 
{	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"%s"					}
,	{	0	,	{	{	0,	0,	0	},	{	2,	1,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"%s of Regeneration"	}
,	{	2	,	{	{	0,	0,	1	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_STUN		},	"Tough %s"				}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_BURN		},	"Fireproof %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_SHOCK		},	"Insulating %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_POISON		},	"Biohazard %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_FREEZING	},	"Warm %s"				}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_FROZEN		},	"Self-Heating %s"		}
,	{	1	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_REFLECT										,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Reflective %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	(DEFEND_EFFECT)(DEFEND_EFFECT_REFLECT|DEFEND_EFFECT_BLIND)	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_BLIND	,	STATUS_TYPE_BLIND		},	"Polarized %s"			}
,	{	0	,	{	{	0,	0,	0	},	{	1,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE											,	PASSIVE_EFFECT_SHIELD_REPAIR	,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Regenerative %s"		}
};

static const CArmor armorDefinitions[] = 	
{	{	0	,	ARMOR_GRADE_NONE	,	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Underwear"				}
,	{	1	,	ARMOR_GRADE_LIGHT	,	{	{	0,	0,	1	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_BLIND		},	"Robe"					}
,	{	2	,	ARMOR_GRADE_LIGHT	,	{	{	0,	0,	2	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_FREEZING	},	"Leather Armor"			}
,	{	3	,	ARMOR_GRADE_LIGHT	,	{	{	0,	0,	3	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Wooden Armor"			}
,	{	4	,	ARMOR_GRADE_MEDIUM	,	{	{	0,	0,	4	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Chainmail Armor"		}
,	{	5	,	ARMOR_GRADE_HEAVY	,	{	{	0,	0,	5	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_BLEEDING	},	"Plate Armor"			}
,	{	6	,	ARMOR_GRADE_MEDIUM	,	{	{	0,	0,	6	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Kevlar Armor"			}
,	{	7	,	ARMOR_GRADE_HEAVY	,	{	{	0,	0,	7	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Diamond Armor"			}
,	{	8	,	ARMOR_GRADE_MEDIUM	,	{	{	0,	0,	8	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Dragon Scale Armor"	}
,	{	9	,	ARMOR_GRADE_HEAVY	,	{	{	0,	0,	8	},	{	0,	1,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_BLIND	,	STATUS_TYPE_STUN		},	"Stardust Armor"		}
,	{	10	,	ARMOR_GRADE_HEAVY	,	{	{	0,	0,	10	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_SHIELD_REPAIR	,	STATUS_TYPE_SHOCK	,	STATUS_TYPE_NONE		},	"Mech Armor"			}
,	{	10	,	ARMOR_GRADE_MEDIUM	,	{	{	0,	0,	9	},	{	1,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE			,	PASSIVE_EFFECT_SHIELD_REPAIR	,	STATUS_TYPE_SHOCK	,	STATUS_TYPE_NONE		},	"Biomech Armor"			}
,	{	10	,	ARMOR_GRADE_LIGHT	,	{	{	0,	0,	9	},	{	0,	0,	0	},	{	1,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_IMPENETRABLE	,	PASSIVE_EFFECT_SHIELD_REPAIR	,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		},	"Force Field"			}
};

	static std::string getArmorName(const SArmor& armor)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, armorModifiers[armor.Modifier].Name.c_str(), armorDefinitions[armor.Index].Name.c_str());
		return formattedName;
	}

	static inline constexpr int32_t getArmorAbsorption(const SArmor& armor) {
		return	(int32_t)((armorDefinitions[armor.Index].Absorption + armorModifiers[armor.Modifier].Absorption) * armor.Level);
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
}

#endif // __ARMOR_H__928364982734698273462834__
