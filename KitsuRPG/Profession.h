#include "GameBase.h"

#include <string>
#include <algorithm>

#ifndef __PROFESSION_H__986349827346982734__
#define __PROFESSION_H__986349827346982734__

struct CProfession
{
	SCharacterPoints	Points;
	std::string			Name;
};

typedef CProfession CProfessionModifier;

static const CProfessionModifier professionModifiers[] = 
{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	}, "Novice %s"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	}, "Squaddie %s"		}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	0,	},	0	}, "Scholar %s"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	1,	},	0	}, "Corporal %s"		}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	1,	},	1	}, "Sergeant %s"		}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	2,	1,	},	1	}, "Colonel %s"			}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	1,	},	1	}, "Lieutenant %s"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	1	}, "Captain %s"			}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	2	}, "Major %s"			}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	2	}, "Colonel %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	2	}, "Zombie %s"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	2	}, "Undead %s"			}
,	{	{	{	2,	0,	0	},	{	1,	0,	0	},	{	2,	2,	},	2	}, "Possessed %s"		}
,	{	{	{	2,	0,	0	},	{	1,	0,	0	},	{	2,	3,	},	2	}, "Champion %s"		}
,	{	{	{	3,	0,	0	},	{	1,	0,	0	},	{	2,	3,	},	2	}, "Elite %s"			}
,	{	{	{	3,	0,	0	},	{	1,	0,	0	},	{	2,	3,	},	3	}, "King %s"			}
,	{	{	{	3,	0,	0	},	{	1,	0,	0	},	{	3,	3,	},	3	}, "%s CEO"				}
,	{	{	{	3,	0,	0	},	{	1,	0,	0	},	{	3,	4,	},	3	}, "%s Mad Scientist"	}
,	{	{	{	4,	0,	0	},	{	1,	0,	0	},	{	3,	4,	},	3	}, "Emperor %s"			}
,	{	{	{	4,	0,	0	},	{	2,	0,	0	},	{	3,	4,	},	3	}, "Demigod %s"			}
,	{	{	{	4,	0,	0	},	{	3,	0,	0	},	{	3,	4,	},	3	}, "Angel %s"			}
,	{	{	{	4,	0,	0	},	{	3,	0,	0	},	{	4,	4,	},	3	}, "%s God"				}
};

static const CProfession professionDefinitions[] =
{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	}, "Civilian"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	1,	2,	},	0	}, "Swordman"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	1,	4,	},	0	}, "Mage"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	1	}, "Thief"			}
,	{	{	{	2,	0,	0	},	{	2,	0,	0	},	{	1,	0,	},	0	}, "Acolyte"		}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	2,	},	2	}, "Merchant"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	1,	},	0	}, "Archer"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	3,	1,	},	0	}, "Sniper"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	}, "Driver"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	3,	},	0	}, "Assault"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	}, "Heavy"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	}, "Sorcerer"		}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	2,	1,	},	0	}, "Medic"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	3,	1,	},	0	}, "Ninja"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	}, "Grenadier"		}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	0,	2,	},	1	}, "Biochemist"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	0	}, "Assassin"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	1,	2,	},	0	}, "Engineer"		}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	}, "Executioner"	}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	0	}, "Commando"		}
,	{	{	{	2,	0,	0	},	{	1,	0,	0	},	{	0,	2,	},	0	}, "Vampire"		}
,	{	{	{	2,	0,	0	},	{	1,	0,	0	},	{	-1,	3,	},	0	}, "Valkyr"			}
,	{	{	{	3,	0,	0	},	{	3,	0,	0	},	{	-1,	0,	},	0	}, "Trinity"		}
,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	2,	},	0	}, "Rhino"			}
,	{	{	{	-1,	0,	0	},	{	0,	0,	0	},	{	3,	3,	},	0	}, "Mesa"			}
};

static std::string getProfessionName(const SProfession& profession)
{
	char formattedName[128] = {};
	sprintf_s(formattedName, professionModifiers[profession.Modifier].Name.c_str(), professionDefinitions[profession.Index].Name.c_str());
	return formattedName;
}

//static inline constexpr ARMOR_EFFECT getArmorEffect(const SArmor& profession) {
//	return ARMOR_EFFECT(professionDefinitions[profession.Index].Effect | professionModifiers[profession.Modifier].Effect);
//}

//static inline constexpr int32_t getArmorShield(const SArmor& profession) {
//	return professionDefinitions[profession.Index].Shield + professionModifiers[profession.Modifier].Shield;
//}

//static inline constexpr int32_t getArmorAbsorption(const SArmor& profession) {
//	return professionDefinitions[profession.Index].Absorption + professionModifiers[profession.Modifier].Absorption;
//}

static SCharacterPoints getProfessionPoints(const SProfession& profession) {
	SCharacterPoints professionPoints = {};
	professionPoints.MaxLife.HP			= (int32_t)(	professionDefinitions[profession.Index].Points.MaxLife.HP			*	std::max(1.0000001, profession.Level*0.25	)	+	professionModifiers[profession.Modifier].Points.MaxLife.HP			*	std::max(1.0000001, profession.Level*0.25	)	);
	professionPoints.MaxLife.Mana		= (int32_t)(	professionDefinitions[profession.Index].Points.MaxLife.Mana			*	std::max(1.0000001, profession.Level*0.25	)	+	professionModifiers[profession.Modifier].Points.MaxLife.Mana		*	std::max(1.0000001, profession.Level*0.25	)	);
	professionPoints.MaxLife.Shield		= (int32_t)(	professionDefinitions[profession.Index].Points.MaxLife.Shield		*	std::max(1.0000001, profession.Level*0.25	)	+	professionModifiers[profession.Modifier].Points.MaxLife.Shield		*	std::max(1.0000001, profession.Level*0.25	)	);
	
	professionPoints.CurrentLife.HP		= (int32_t)(	professionDefinitions[profession.Index].Points.CurrentLife.HP		*	std::max(1.0000001, profession.Level*0.125	)	+	professionModifiers[profession.Modifier].Points.CurrentLife.HP		*	std::max(1.0000001, profession.Level*0.125	)	);
	professionPoints.CurrentLife.Mana	= (int32_t)(	professionDefinitions[profession.Index].Points.CurrentLife.Mana		*	std::max(1.0000001, profession.Level*0.125	)	+	professionModifiers[profession.Modifier].Points.CurrentLife.Mana	*	std::max(1.0000001, profession.Level*0.125	)	);
	professionPoints.CurrentLife.Shield	= (int32_t)(	professionDefinitions[profession.Index].Points.CurrentLife.Shield	*	std::max(1.0000001, profession.Level*0.25	)	+	professionModifiers[profession.Modifier].Points.CurrentLife.Shield	*	std::max(1.0000001, profession.Level*0.25	)	);
	
	professionPoints.Attack.Damage		= (int32_t)(	professionDefinitions[profession.Index].Points.Attack.Damage		*	std::max(1.0000001, profession.Level*0.2	)	+	professionModifiers[profession.Modifier].Points.Attack.Damage		*	std::max(1.0000001, profession.Level*0.2	)	);
	professionPoints.Attack.Hit			= (int32_t)(	professionDefinitions[profession.Index].Points.Attack.Hit			*	std::max(1.0000001, profession.Level*0.2	)	+	professionModifiers[profession.Modifier].Points.Attack.Hit			*	std::max(1.0000001, profession.Level*0.2	)	);

	professionPoints.Coins				= (int32_t)(	professionDefinitions[profession.Index].Points.Coins				*	std::max(1.0000001, profession.Level*0.2	)	+	professionModifiers[profession.Modifier].Points.Coins				*	std::max(1.0000001, profession.Level*0.2	)	);
	return professionPoints;
}


#endif __PROFESSION_H__986349827346982734__
