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
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	}, "Squaddie %s"		}	// 1
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	0,	},	0	}, "Scholar %s"			}	// 2
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	1,	},	0	}, "Corporal %s"		}	// 3
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	1,	},	1	}, "Sergeant %s"		}	// 4
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	2,	1,	},	1	}, "Colonel %s"			}	// 5
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	1,	},	1	}, "Lieutenant %s"		}	// 6
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	1	}, "Captain %s"			}	// 7
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	2	}, "Major %s"			}	// 8
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	2	}, "Colonel %s"			}	// 9
,	{	{	{	0,	2,	0	},	{	0,	0,	0	},	{	2,	4,	},	2	}, "Zombie %s"			}	// 10
,	{	{	{	1,	2,	0	},	{	0,	0,	0	},	{	2,	4,	},	2	}, "Undead %s"			}	// 11
,	{	{	{	1,	2,	0	},	{	1,	0,	0	},	{	2,	5,	},	1	}, "Possessed %s"		}	// 12
,	{	{	{	2,	1,	0	},	{	1,	0,	0	},	{	2,	5,	},	2	}, "Champion %s"		}	// 13
,	{	{	{	3,	1,	0	},	{	1,	0,	0	},	{	2,	5,	},	2	}, "Elite %s"			}	// 14
,	{	{	{	3,	1,	0	},	{	1,	0,	0	},	{	2,	5,	},	3	}, "King %s"			}	// 15
,	{	{	{	3,	1,	0	},	{	1,	0,	0	},	{	3,	4,	},	4	}, "%s CEO"				}	// 16
,	{	{	{	3,	1,	0	},	{	1,	1,	0	},	{	3,	4,	},	4	}, "%s Mad Scientist"	}	// 17
,	{	{	{	4,	1,	0	},	{	1,	1,	0	},	{	3,	4,	},	4	}, "Emperor %s"			}	// 18
,	{	{	{	4,	2,	0	},	{	1,	1,	0	},	{	3,	4,	},	4	}, "Demigod %s"			}	// 19
,	{	{	{	4,	2,	0	},	{	2,	1,	0	},	{	3,	4,	},	4	}, "Angel %s"			}	// 20
,	{	{	{	4,	2,	0	},	{	2,	1,	0	},	{	4,	4,	},	4	}, "%s God"				}	// 21
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

static SCharacterPoints getProfessionPoints(const SProfession& profession) {
	SCharacterPointsMultipliers	multipliers;

	multipliers.MaxLife		= {.5, .5, .25};
	multipliers.CurrentLife	= {.1, .1, .1};
	multipliers.Attack		= {.1, .1};
	multipliers.Coins		= 0.125;

	return (professionDefinitions[profession.Index].Points + professionModifiers[profession.Modifier].Points)*multipliers;
}


#endif __PROFESSION_H__986349827346982734__
