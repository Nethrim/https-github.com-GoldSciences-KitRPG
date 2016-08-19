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
,	{	{	{	1,	0,	1	},	{	0,	0,	0	},	{	1,	1,	},	1	}, "Colonel %s"			}	// 5
,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	1,	},	1	}, "Lieutenant %s"		}	// 6
,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	1,	1,	},	1	}, "Captain %s"			}	// 7
,	{	{	{	1,	1,	1	},	{	1,	1,	0	},	{	1,	1,	},	1	}, "Major %s"			}	// 8
,	{	{	{	1,	1,	1	},	{	1,	1,	1	},	{	1,	1,	},	1	}, "Colonel %s"			}	// 9
,	{	{	{	2,	1,	1	},	{	1,	1,	1	},	{	1,	1,	},	1	}, "Zombie %s"			}	// 10
,	{	{	{	2,	2,	1	},	{	1,	1,	1	},	{	1,	1,	},	1	}, "Undead %s"			}	// 11
,	{	{	{	2,	2,	1	},	{	1,	1,	1	},	{	1,	2,	},	1	}, "Possessed %s"		}	// 12
,	{	{	{	2,	2,	1	},	{	1,	1,	1	},	{	2,	2,	},	1	}, "Champion %s"		}	// 13
,	{	{	{	2,	2,	2	},	{	1,	1,	1	},	{	2,	2,	},	1	}, "Elite %s"			}	// 14
,	{	{	{	2,	2,	2	},	{	1,	1,	1	},	{	2,	2,	},	2	}, "King %s"			}	// 15
,	{	{	{	2,	2,	2	},	{	1,	2,	1	},	{	2,	2,	},	2	}, "%s CEO"				}	// 16
,	{	{	{	2,	2,	2	},	{	2,	2,	1	},	{	2,	2,	},	2	}, "Demon %s"			}	// 17
,	{	{	{	2,	2,	2	},	{	2,	2,	2	},	{	2,	2,	},	2	}, "%s Mad Scientist"	}	// 18
,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	2,	2,	},	2	}, "Emperor %s"			}	// 19
,	{	{	{	3,	3,	2	},	{	2,	2,	2	},	{	2,	2,	},	2	}, "Demigod %s"			}	// 10
,	{	{	{	3,	3,	3	},	{	2,	2,	2	},	{	2,	2,	},	2	}, "Angel %s"			}	// 21
,	{	{	{	3,	3,	3	},	{	2,	2,	2	},	{	2,	3,	},	2	}, "%s God"				}	// 22
};

// Probably the base points should be 6 instead of five considering we now have more fields to place them in.
static const CProfession professionDefinitions[] =
{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	}, "Nobody"			}
,	{	{	{	2,	0,	1	},	{	0,	0,	0	},	{	1,	2,	},	0	}, "Swordman"		}
,	{	{	{	0,	1,	0	},	{	0,	1,	0	},	{	1,	3,	},	0	}, "Mage"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	3,	2,	},	1	}, "Thief"			}
,	{	{	{	1,	2,	0	},	{	1,	1,	0	},	{	1,	0,	},	0	}, "Acolyte"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	2,	},	2	}, "Merchant"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	0	}, "Archer"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	3,	3,	},	0	}, "Sniper"			}
,	{	{	{	2,	1,	0	},	{	0,	0,	0	},	{	1,	2,	},	0	}, "Driver"			}
,	{	{	{	1,	0,	1	},	{	0,	0,	0	},	{	2,	2,	},	0	}, "Raider"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	0	}, "Assault"		}
,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	}, "Heavy"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	4,	},	0	}, "Sorcerer"		}
,	{	{	{	1,	0,	0	},	{	2,	0,	0	},	{	2,	1,	},	0	}, "Medic"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	3,	2,	},	0	}, "Ninja"			}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	}, "Grenadier"		}
,	{	{	{	1,	0,	0	},	{	1,	1,	0	},	{	0,	2,	},	1	}, "Biochemist"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0	}, "Assassin"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	1,	3,	},	0	}, "Engineer"		}
,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	}, "Executioner"	}
,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	2,	2,	},	0	}, "Commando"		}
,	{	{	{	2,	1,	0	},	{	1,	0,	0	},	{	0,	2,	},	0	}, "Vampire"		}
,	{	{	{	2,	0,	0	},	{	1,	0,	0	},	{	0,	3,	},	0	}, "Valkyr"			}
,	{	{	{	2,	1,	0	},	{	2,	1,	0	},	{	0,	0,	},	0	}, "Trinity"		}
,	{	{	{	3,	2,	0	},	{	0,	0,	0	},	{	0,	2,	},	0	}, "Rhino"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	2,	},	0	}, "Mesa"			}
,	{	{	{	0,	1,	1	},	{	0,	1,	1	},	{	0,	2,	},	0	}, "Mag"			}
,	{	{	{	0,	3,	0	},	{	0,	1,	0	},	{	0,	1,	},	1	}, "Loki"			}
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
