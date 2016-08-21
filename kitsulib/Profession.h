#include "GameBase.h"

#include <string>
#include <algorithm>

#ifndef __PROFESSION_H__986349827346982734__
#define __PROFESSION_H__986349827346982734__

namespace klib
{

	struct CProfession
	{
		SCharacterPoints	Points;
		std::string			Name;
	};

	typedef CProfession CProfessionModifier;

	static const CProfessionModifier modifiersProfession[] = 
	{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Rookie %s"			}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Squaddie %s"		}	// points: 1
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Corporal %s"		}	// points: 2
	,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	1,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Sergeant %s"		}	// points: 3
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Lieutenant %s"		}	// points: 4
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	0,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Captain %s"			}	// points: 5
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Major %s"			}	// points: 6
	,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	1,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Colonel %s"			}	// points: 7
	,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_BLEEDING	}, "Zombie %s"			}	// points: 8
	,	{	{	{	1,	2,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_BLIND		}, "Undead %s"			}	// points: 9
	,	{	{	{	2,	2,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_STUN		}, "Possessed %s"		}	// points: 10
	,	{	{	{	2,	2,	1	},	{	0,	1,	0	},	{	2,	2,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Mutant %s"			}	// points: 11
	,	{	{	{	2,	2,	2	},	{	0,	1,	0	},	{	2,	2,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Champion %s"		}	// points: 12
	,	{	{	{	2,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Elite %s"			}	// points: 13
	,	{	{	{	2,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "King %s"			}	// points: 14
	,	{	{	{	3,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "%s CEO"				}	// points: 15
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	2,	2,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Demon %s"			}	// points: 16
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	2,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "%s Mad Scientist"	}	// points: 18
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_BLEEDING	}, "Robot %s"			}	// points: 17
	,	{	{	{	3,	2,	2	},	{	1,	2,	1	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Emperor %s"			}	// points: 19
	,	{	{	{	3,	2,	2	},	{	2,	2,	1	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Demigod %s"			}	// points: 10
	,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Angel %s"			}	// points: 11
	,	{	{	{	3,	2,	3	},	{	2,	2,	2	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "%s God"				}	// points: 22
	//,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "%s God"				}	// points: 23
	};

	// Probably the base points should be 6 instead of five considering we now have more fields to place them in.	
	static const CProfession definitionsProfession[] =
	{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Nobody"			}
	,	{	{	{	2,	0,	1	},	{	0,	0,	0	},	{	1,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Swordman"		}
	,	{	{	{	0,	1,	0	},	{	0,	1,	0	},	{	1,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Mage"			}
	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	3,	2,	},	1	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Thief"			}
	,	{	{	{	1,	2,	0	},	{	1,	1,	0	},	{	1,	0,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Acolyte"		}
	,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	2,	},	2	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Merchant"		}
	,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Archer"			}
	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	3,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Sniper"			}
	,	{	{	{	2,	1,	0	},	{	0,	0,	0	},	{	1,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Driver"			}
	,	{	{	{	1,	0,	1	},	{	0,	0,	0	},	{	2,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Raider"			}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Assault"		}
	,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Heavy"			}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Sorcerer"		}
	,	{	{	{	1,	0,	0	},	{	2,	0,	0	},	{	2,	1,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Medic"			}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	3,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Ninja"			}
	,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Grenadier"		}
	,	{	{	{	1,	0,	0	},	{	1,	1,	0	},	{	0,	2,	},	1	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Biochemist"		}
	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Assassin"		}
	,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	1,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Engineer"		}
	,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Executioner"	}
	,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	2,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Commando"		}
	,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	1,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Sectoid"		}
	,	{	{	{	0,	0,	0	},	{	0,	0,	2	},	{	2,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Android"		}
	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_BURN	,	STATUS_TYPE_BLEEDING	}, "Drone"			}
	,	{	{	{	2,	1,	0	},	{	1,	0,	0	},	{	0,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Vampire"		}
	,	{	{	{	2,	0,	0	},	{	1,	0,	0	},	{	0,	3,	},	0	,	ATTACK_EFFECT_LEECH,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Valkyr"			}
	,	{	{	{	2,	1,	0	},	{	2,	1,	0	},	{	0,	0,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Trinity"		}
	,	{	{	{	3,	2,	0	},	{	0,	0,	0	},	{	0,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Rhino"			}
	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Mesa"			}
	,	{	{	{	0,	1,	1	},	{	0,	1,	1	},	{	0,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_NONE	,	STATUS_TYPE_NONE		}, "Mag"			}
	,	{	{	{	0,	3,	0	},	{	0,	1,	0	},	{	0,	1,	},	1	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	STATUS_TYPE_BLIND	,	STATUS_TYPE_NONE		}, "Loki"			}
	};

	static std::string getProfessionName(const SProfession& profession)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, modifiersProfession[profession.Modifier].Name.c_str(), definitionsProfession[profession.Index].Name.c_str());
		return formattedName;
	}

	static SCharacterPoints getProfessionPoints(const SProfession& profession) {
		SCharacterPointsMultipliers	multipliers;

		multipliers.MaxLife		= {.5, .5, .25};
		multipliers.CurrentLife	= {.1, .1, .1};
		multipliers.Attack		= {.1, .1};
		multipliers.Coins		= 0.125;

		multipliers				= multipliers*profession.Level;

		return (definitionsProfession[profession.Index].Points + modifiersProfession[profession.Modifier].Points)*multipliers;
	}
}

#endif __PROFESSION_H__986349827346982734__
