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
{	{	{	0,	0,	0,	0,	0	}, "Novice %s"			}
,	{	{	1,	0,	0,	0,	0	}, "Squaddie %s"		}
,	{	{	1,	0,	5,	0,	0	}, "Scholar %s"			}
,	{	{	1,	0,	5,	1,	0	}, "Corporal %s"		}
,	{	{	1,	0,	5,	1,	1	}, "Sergeant %s"		}
,	{	{	1,	0,	10,	1,	1	}, "Colonel %s"			}
,	{	{	2,	0,	10,	1,	1	}, "Lieutenant %s"		}
,	{	{	2,	0,	10,	2,	1	}, "Captain %s"			}
,	{	{	2,	0,	10,	2,	2	}, "Major %s"			}
,	{	{	2,	0,	10,	2,	2	}, "Colonel %s"			}
,	{	{	0,	0,	10,	3,	2	}, "Zombie %s"			}
,	{	{	1,	0,	10,	2,	2	}, "Undead %s"			}
,	{	{	2,	1,	10,	2,	2	}, "Possessed %s"		}
,	{	{	2,	1,	10,	3,	2	}, "Champion %s"		}
,	{	{	3,	1,	10,	3,	2	}, "King %s"			}
,	{	{	3,	1,	10,	3,	3	}, "%s CEO"				}
,	{	{	3,	2,	10,	3,	3	}, "%s Mad Scientist"	}
,	{	{	3,	2,	15,	3,	3	}, "Emperor %s"			}
,	{	{	4,	2,	15,	3,	3	}, "Demigod %s"			}
,	{	{	4,	2,	15,	4,	3	}, "Angel %s"			}
,	{	{	4,	3,	15,	4,	3	}, "%s God"				}
};

static const CProfession professionDefinitions[] =
{	{	{	0,	0,	0,	0,	0	}, "Civilian"		}
,	{	{	2,	0,	5,	2,	0	}, "Swordman"		}
,	{	{	0,	0,	5,	4,	0	}, "Mage"			}
,	{	{	0,	0,	10,	2,	1	}, "Thief"			}
,	{	{	2,	2,	5,	0,	0	}, "Acolyte"		}
,	{	{	1,	0,	0,	2,	2	}, "Merchant"		}
,	{	{	2,	0,	10,	1,	0	}, "Archer"			}
,	{	{	1,	0,	15,	1,	0	}, "Sniper"			}
,	{	{	1,	0,	0,	0,	0	}, "Driver"			}
,	{	{	1,	0,	5,	3,	0	}, "Assault"		}
,	{	{	2,	0,	0,	3,	0	}, "Heavy"			}
,	{	{	1,	0,	0,	4,	0	}, "Sorcerer"		}
,	{	{	1,	1,	10,	1,	0	}, "Medic"			}
,	{	{	1,	0,	15,	1,	0	}, "Ninja"			}
,	{	{	1,	0,	0,	4,	0	}, "Grenadier"		}
,	{	{	1,	1,	0,	3,	0	}, "Biochemist"		}
,	{	{	0,	0,	10,	3,	0	}, "Assassin"		}
,	{	{	2,	0,	5,	2,	0	}, "Engineer"		}
,	{	{	1,	0,	0,	4,	0	}, "Executioner"	}
,	{	{	2,	1,	0,	2,	0	}, "Vampire"		}
,	{	{	1,	0,	10,	2,	0	}, "Commando"		}
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
	professionPoints.MaxHP		= (int32_t)	(professionDefinitions[profession.Index].Points.MaxHP	*	std::max(1.000001, profession.Level/1.0)	+	professionModifiers[profession.Modifier].Points.MaxHP	);
	professionPoints.Attack		= (int32_t)	(professionDefinitions[profession.Index].Points.Attack	*	std::max(1.000001, profession.Level/2.0)	+	professionModifiers[profession.Modifier].Points.Attack	);
	professionPoints.Hit		= (int32_t)	(professionDefinitions[profession.Index].Points.Hit		*	std::max(1.000001, profession.Level/3.0)	+	professionModifiers[profession.Modifier].Points.Hit		);
	professionPoints.HP			= (int32_t)	(professionDefinitions[profession.Index].Points.HP		*	std::max(1.000001, profession.Level/5.0)	+	professionModifiers[profession.Modifier].Points.HP		);
	professionPoints.Coins		= (int32_t)	(professionDefinitions[profession.Index].Points.Coins	*	std::max(1.000001, profession.Level/4.0)	+	professionModifiers[profession.Modifier].Points.Coins	);
	return professionPoints;
}


#endif __PROFESSION_H__986349827346982734__
