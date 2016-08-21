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
	{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Rookie %s"			}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Squaddie %s"		}	// points: 1
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Corporal %s"		}	// points: 2
	,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	1,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Sergeant %s"		}	// points: 3
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	0,	},	0	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Lieutenant %s"		}	// points: 4
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	0,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Captain %s"			}	// points: 5
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Major %s"			}	// points: 6
	,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	1,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Colonel %s"			}	// points: 7
	,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_BLEEDING	}, "Zombie %s"			}	// points: 8
	,	{	{	{	1,	2,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_BLIND		}, "Undead %s"			}	// points: 9
	,	{	{	{	2,	2,	1	},	{	0,	1,	0	},	{	2,	1,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_STUN		}, "Possessed %s"		}	// points: 10
	,	{	{	{	2,	2,	1	},	{	0,	1,	0	},	{	2,	2,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Mutant %s"			}	// points: 11
	,	{	{	{	2,	2,	2	},	{	0,	1,	0	},	{	2,	2,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Champion %s"		}	// points: 12
	,	{	{	{	2,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	1	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Elite %s"			}	// points: 13
	,	{	{	{	2,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "King %s"			}	// points: 14
	,	{	{	{	3,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "%s CEO"				}	// points: 15
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	2,	2,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Demon %s"			}	// points: 16
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	2,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "%s Mad Scientist"	}	// points: 18
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_BLEEDING	}, "Robot %s"			}	// points: 17
	,	{	{	{	3,	2,	2	},	{	1,	2,	1	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Emperor %s"			}	// points: 19
	,	{	{	{	3,	2,	2	},	{	2,	2,	1	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Demigod %s"			}	// points: 10
	,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Angel %s"			}	// points: 11
	,	{	{	{	3,	2,	3	},	{	2,	2,	2	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "%s God"				}	// points: 22
	//,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	3,	3,	},	2	,	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "%s God"				}	// points: 23
	};

	// Probably the base points should be 8 instead of six considering it's difficult to advance in the game with so little points.	
	static const CProfession definitionsProfession[] =
	{	{	{	{	1,	1,	1	},	{	1,	1,	1	},	{	1,	1,	},	1	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Nobody"			}
	,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Heavy"			}
	,	{	{	{	2,	0,	0	},	{	2,	0,	0	},	{	3,	1,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Support"		}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Assault"		}
	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Sniper"			}
	,	{	{	{	2,	0,	1	},	{	1,	0,	0	},	{	1,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Swordman"		}
	,	{	{	{	0,	2,	0	},	{	0,	1,	0	},	{	1,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Mage"			}
	,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	3,	2,	},	1	,	ATTACK_EFFECT_STEAL,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Thief"			}
	,	{	{	{	2,	2,	0	},	{	1,	2,	0	},	{	1,	0,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Acolyte"		}
	,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	2	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Merchant"		}
	,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	3,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Archer"			}
	,	{	{	{	3,	1,	1	},	{	0,	0,	0	},	{	1,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Driver"			}
	,	{	{	{	1,	0,	2	},	{	0,	0,	0	},	{	2,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Raider"			}
	,	{	{	{	1,	1,	0	},	{	0,	1,	0	},	{	1,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Sorcerer"		}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	4,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Ninja"			}
	,	{	{	{	2,	0,	1	},	{	0,	0,	0	},	{	1,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Grenadier"		}
	,	{	{	{	1,	0,	0	},	{	1,	1,	0	},	{	0,	4,	},	1	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Biochemist"		}
	,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Assassin"		}
	,	{	{	{	2,	0,	1	},	{	0,	0,	0	},	{	2,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Engineer"		}
	,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	1,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Executioner"	}
	,	{	{	{	1,	0,	1	},	{	0,	0,	0	},	{	3,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Commando"		}
	,	{	{	{	0,	2,	0	},	{	0,	1,	0	},	{	2,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Sectoid"		}
	,	{	{	{	0,	0,	1	},	{	0,	0,	1	},	{	3,	3,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Android"		}
	,	{	{	{	0,	0,	0	},	{	0,	0,	1	},	{	3,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	,	COMBAT_STATUS_BURN	,	COMBAT_STATUS_BLEEDING	}, "Drone"			}
	,	{	{	{	2,	1,	0	},	{	1,	1,	0	},	{	0,	3,	},	0	,	ATTACK_EFFECT_LEECH,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_MANA_REGEN		,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Vampire"		}
	,	{	{	{	4,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	,	ATTACK_EFFECT_LEECH,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Valkyr"			}
	,	{	{	{	2,	2,	0	},	{	2,	2,	0	},	{	0,	0,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Trinity"		}
	,	{	{	{	3,	2,	1	},	{	0,	0,	0	},	{	0,	2,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Rhino"			}
	,	{	{	{	0,	1,	0	},	{	0,	0,	0	},	{	3,	4,	},	0	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Mesa"			}
	,	{	{	{	0,	1,	2	},	{	0,	1,	2	},	{	0,	1,	},	1	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_NONE	,	COMBAT_STATUS_NONE		}, "Mag"			}
	,	{	{	{	0,	2,	0	},	{	0,	1,	0	},	{	2,	2,	},	1	,	ATTACK_EFFECT_NONE,		DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				,	COMBAT_STATUS_BLIND	,	COMBAT_STATUS_NONE		}, "Loki"			}
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
