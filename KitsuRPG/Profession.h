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
{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	}, "Rookie %s"			}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	}, "Squaddie %s"		}	// points: 1
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	0,	},	0	}, "Corporal %s"		}	// points: 2
,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	1,	0,	},	0	}, "Sergeant %s"		}	// points: 3
,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	0,	},	0	}, "Lieutenant %s"		}	// points: 4
,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	0,	},	1	}, "Captain %s"			}	// points: 5
,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	1,	},	1	}, "Major %s"			}	// points: 6
,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	1,	1,	},	1	}, "Colonel %s"			}	// points: 7
,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	}, "Zombie %s"			}	// points: 8
,	{	{	{	1,	2,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	}, "Undead %s"			}	// points: 9
,	{	{	{	2,	2,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	}, "Possessed %s"		}	// points: 10
,	{	{	{	2,	2,	1	},	{	0,	1,	0	},	{	2,	2,	},	1	}, "Mutant %s"			}	// points: 11
,	{	{	{	2,	2,	2	},	{	0,	1,	0	},	{	2,	2,	},	1	}, "Champion %s"		}	// points: 12
,	{	{	{	2,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	1	}, "Elite %s"			}	// points: 13
,	{	{	{	2,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	2	}, "King %s"			}	// points: 14
,	{	{	{	3,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	2	}, "%s CEO"				}	// points: 15
,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	2,	2,	},	2	}, "Demon %s"			}	// points: 16
,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	2,	3,	},	2	}, "%s Mad Scientist"	}	// points: 18
,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	3,	3,	},	2	}, "Robot %s"			}	// points: 17
,	{	{	{	3,	2,	2	},	{	1,	2,	1	},	{	3,	3,	},	2	}, "Emperor %s"			}	// points: 19
,	{	{	{	3,	2,	2	},	{	2,	2,	1	},	{	3,	3,	},	2	}, "Demigod %s"			}	// points: 10
,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	3,	3,	},	2	}, "Angel %s"			}	// points: 11
,	{	{	{	3,	2,	3	},	{	2,	2,	2	},	{	3,	3,	},	2	}, "%s God"				}	// points: 22
//,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	3,	3,	},	2	}, "%s God"				}	// points: 23
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
,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	}, "Grenadier"		}
,	{	{	{	1,	0,	0	},	{	1,	1,	0	},	{	0,	2,	},	1	}, "Biochemist"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0	}, "Assassin"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	1,	3,	},	0	}, "Engineer"		}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	}, "Executioner"	}
,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	2,	2,	},	0	}, "Commando"		}
,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	1,	3,	},	0	}, "Sectoid"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	2	},	{	2,	2,	},	0	}, "Android"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0	}, "Drone"			}
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

	multipliers				= multipliers*profession.Level;

	return (professionDefinitions[profession.Index].Points + professionModifiers[profession.Modifier].Points)*multipliers;
}


#endif __PROFESSION_H__986349827346982734__
