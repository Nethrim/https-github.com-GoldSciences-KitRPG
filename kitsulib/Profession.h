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
	{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0, {}	,{},																					}, "Rookie %s"			}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0, {}	,{},																					}, "Squaddie %s"		}	// points: 1
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	0,	},	0, {}	,{},																					}, "Corporal %s"		}	// points: 2
	,	{	{	{	1,	1,	0	},	{	0,	0,	0	},	{	1,	0,	},	0, {}	,{},																					}, "Sergeant %s"		}	// points: 3
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	0,	},	0, {}	,{},																					}, "Lieutenant %s"		}	// points: 4
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	0,	},	1, {}	,{},																					}, "Captain %s"			}	// points: 5
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	1,	1,	},	1, {}	,{},																					}, "Major %s"			}	// points: 6
	,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	1,	1,	},	1, {}	,{},																					}, "Colonel %s"			}	// points: 7
	,	{	{	{	1,	1,	1	},	{	0,	1,	0	},	{	2,	1,	},	1, {}	,{ COMBAT_STATUS_NONE	, COMBAT_STATUS_BLEEDING	},									}, "Zombie %s"			}	// points: 8
	,	{	{	{	1,	2,	1	},	{	0,	1,	0	},	{	2,	1,	},	1, {}	,{ COMBAT_STATUS_NONE	, COMBAT_STATUS_BLIND		},									}, "Undead %s"			}	// points: 9
	,	{	{	{	2,	2,	1	},	{	0,	1,	0	},	{	2,	1,	},	1, {}	,{ COMBAT_STATUS_NONE	, COMBAT_STATUS_STUN		},									}, "Possessed %s"		}	// points: 10
	,	{	{	{	2,	2,	1	},	{	0,	1,	0	},	{	2,	2,	},	1, {	ATTACK_EFFECT_NONE,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	},	{},		}, "Mutant %s"			}	// points: 11
	,	{	{	{	2,	2,	2	},	{	0,	1,	0	},	{	2,	2,	},	1, {}	,{},																					}, "Champion %s"		}	// points: 12
	,	{	{	{	2,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	1, {}	,{},																					}, "Elite %s"			}	// points: 13
	,	{	{	{	2,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	2, {}	,{},																					}, "King %s"			}	// points: 14
	,	{	{	{	3,	2,	2	},	{	1,	1,	0	},	{	2,	2,	},	2, {}	,{},																					}, "%s CEO"				}	// points: 15
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	2,	2,	},	2, {}	,{},																					}, "Demon %s"			}	// points: 16
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	2,	3,	},	2, {}	,{},																					}, "%s Mad Scientist"	}	// points: 17
	,	{	{	{	3,	2,	2	},	{	1,	1,	1	},	{	3,	3,	},	2, {}	,{ COMBAT_STATUS_NONE	, COMBAT_STATUS_BLEEDING	}									}, "Robot %s"			}	// points: 18
	,	{	{	{	3,	2,	2	},	{	1,	2,	1	},	{	3,	3,	},	2, {}	,{},																					}, "Emperor %s"			}	// points: 19
	,	{	{	{	3,	2,	2	},	{	2,	2,	1	},	{	3,	3,	},	2, {}	,{},																					}, "Demigod %s"			}	// points: 20
	,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	3,	3,	},	2, {}	,{},																					}, "Angel %s"			}	// points: 21
	,	{	{	{	3,	2,	3	},	{	2,	2,	2	},	{	3,	3,	},	2, {}	,{},																					}, "%s God"				}	// points: 22
	//,	{	{	{	3,	2,	2	},	{	2,	2,	2	},	{	3,	3,	},	2, {}	,{},																					}, "%s God"				}	// points: 23
	};

	// Probably the base points should be 8 instead of six considering it's difficult to advance in the game with so little points.	
	static const CProfession definitionsProfession[] =
	{	{	{	{	1,	1,	1	},	{	1,	1,	1	},	{	1,	1,	},	1, {},	{}	}, "Nobody"				}
	,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0, {},	{}	}, "Heavy"				}
	,	{	{	{	2,	0,	0	},	{	2,	0,	0	},	{	3,	1,	},	0, {},	{}	}, "Support"			}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	0, {},	{}	}, "Assault"			}
	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	4,	},	0, {},	{}	}, "Sniper"				}
	,	{	{	{	2,	0,	1	},	{	1,	0,	0	},	{	1,	3,	},	0, {},	{}	}, "Swordman"			}
	,	{	{	{	0,	2,	0	},	{	0,	1,	0	},	{	1,	4,	},	0, {},	{}	}, "Mage"				}
	,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	3,	2,	},	1, {},	{}	}, "Thief"				}
	,	{	{	{	2,	2,	0	},	{	1,	2,	0	},	{	1,	0,	},	0, {},	{}	}, "Acolyte"			}
	,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	2, {},	{}	}, "Merchant"			}
	,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	3,	3,	},	0, {},	{}	}, "Archer"				}
	,	{	{	{	3,	1,	1	},	{	0,	0,	0	},	{	1,	2,	},	0, {},	{}	}, "Driver"				}
	,	{	{	{	1,	0,	2	},	{	0,	0,	0	},	{	2,	3,	},	0, {},	{}	}, "Raider"				}
	,	{	{	{	1,	1,	0	},	{	0,	1,	0	},	{	1,	4,	},	0, {},	{}	}, "Sorcerer"			}
	,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	4,	3,	},	0, {},	{}	}, "Ninja"				}
	,	{	{	{	2,	0,	1	},	{	0,	0,	0	},	{	1,	4,	},	0, {},	{}	}, "Grenadier"			}
	,	{	{	{	1,	0,	0	},	{	1,	1,	0	},	{	0,	4,	},	1, {},	{}	}, "Biochemist"			}
	,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0, {},	{}	}, "Assassin"			}
//	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	6,	},	0, {},	{}	}, "Tanya"				}
//	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	6,	},	0, {},	{}	}, "Natasha"			}
//	,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	6,	},	0, {},	{}	}, "Yuriko"				}
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	3,	2,	},	0, {},	{}	}, "Imperial Soldier"	}
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	3,	2,	},	0, {},	{}	}, "Conscript"			}
	,	{	{	{	1,	1,	1	},	{	0,	0,	0	},	{	3,	2,	},	0, {},	{}	}, "Peacekeeper"		}
	,	{	{	{	1,	0,	2	},	{	0,	0,	0	},	{	2,	3,	},	0, {},	{}	}, "Flak Trooper"		}
	,	{	{	{	1,	0,	2	},	{	0,	0,	0	},	{	2,	3,	},	0, {},	{}	}, "Javelin Soldier"	}
	,	{	{	{	1,	0,	2	},	{	0,	0,	0	},	{	2,	3,	},	0, {},	{}	}, "Tank Buster"		}
	,	{	{	{	2,	0,	1	},	{	0,	0,	0	},	{	2,	3,	},	0, {	ATTACK_EFFECT_NONE			,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	}	,	{}												}, "Engineer"		}
	,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	1,	4,	},	0, {	ATTACK_EFFECT_NONE			,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				}	,	{}												}, "Executioner"	}
	,	{	{	{	1,	0,	1	},	{	0,	0,	0	},	{	3,	3,	},	0, {	ATTACK_EFFECT_NONE			,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				}	,	{}												}, "Commando"		}
	,	{	{	{	0,	2,	0	},	{	0,	1,	0	},	{	2,	3,	},	0, {	ATTACK_EFFECT_NONE			,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				}	,	{}												}, "Sectoid"		}
	,	{	{	{	0,	0,	1	},	{	0,	0,	1	},	{	3,	3,	},	0, {	ATTACK_EFFECT_NONE			,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				}	,	{}												}, "Android"		}
	,	{	{	{	0,	0,	0	},	{	0,	0,	1	},	{	3,	4,	},	0, {	ATTACK_EFFECT_NONE			,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	}	,	{ COMBAT_STATUS_BURN,COMBAT_STATUS_BLEEDING},		}, "Drone"			}
	,	{	{	{	2,	1,	0	},	{	1,	1,	0	},	{	0,	3,	},	0, {	ATTACK_EFFECT_LEECH_HEALTH	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_MANA_REGEN		}	,	{}												}, "Vampire"		}
	,	{	{	{	4,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0, {	ATTACK_EFFECT_LEECH_HEALTH	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE				}	,	{}												}, "Valkyr"			}
	,	{	{	{	2,	2,	0	},	{	2,	2,	0	},	{	0,	0,	},	0, {	ATTACK_EFFECT_NONE			,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_SHIELD_REPAIR	}	,	{}												}, "Trinity"		}
	,	{	{	{	3,	2,	1	},	{	0,	0,	0	},	{	0,	2,	},	0, {},	{}	}, "Rhino"				}
	,	{	{	{	0,	1,	0	},	{	0,	0,	0	},	{	3,	4,	},	0, {},	{}	}, "Mesa"				}
	,	{	{	{	0,	1,	2	},	{	0,	1,	2	},	{	0,	1,	},	1, {},	{}	}, "Mag"				}
	,	{	{	{	0,	2,	0	},	{	0,	1,	0	},	{	2,	2,	},	1, {},	{ COMBAT_STATUS_BLIND	},	}, "Loki"			}
	};

	static std::string getProfessionName(const SProfession& profession)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, modifiersProfession[profession.Modifier].Name.c_str(), definitionsProfession[profession.Index].Name.c_str());
		return formattedName;
	}

	static SCharacterPoints getProfessionPoints(const SProfession& profession) {
		SCharacterPointsMultipliers	multipliers;

		multipliers.MaxLife		= {.5, .125, .125};
		multipliers.CurrentLife	= {.2, .1, .1};
		multipliers.Attack		= {.1, .1};
		multipliers.Coins		= 0.125;

		multipliers				= multipliers*profession.Level;

		return (definitionsProfession[profession.Index].Points + modifiersProfession[profession.Modifier].Points)*multipliers;
	}
}

#endif __PROFESSION_H__986349827346982734__
