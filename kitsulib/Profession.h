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
{	{{{},{},{},0,{},{},																					{EQUIP_TECHNOLOGY_BASIC}	}, "Rookie %s"			}
,	{{{1},{},{},0,{},{},																				{EQUIP_TECHNOLOGY_BASIC}	}, "Squaddie %s"		}	// points: 1
,	{{{1},{},{1},0,{},{},																				{EQUIP_TECHNOLOGY_BASIC}	}, "Corporal %s"		}	// points: 2
,	{{{1,1},{},{1},0,{},{},																				{EQUIP_TECHNOLOGY_BASIC}	}, "Sergeant %s"		}	// points: 3
,	{{{1,1,1},{},{1},0,{},{},																			{EQUIP_TECHNOLOGY_BASIC}	}, "Lieutenant %s"		}	// points: 4
,	{{{1,1,1},{},{1},1,{},{},																			{EQUIP_TECHNOLOGY_BASIC}	}, "Captain %s"			}	// points: 5
,	{{{1,1,1},{},{1,1},1,{},{},																			{EQUIP_TECHNOLOGY_BASIC}	}, "Major %s"			}	// points: 6
,	{{{1,1,1},{0,1},{1,1,},1,{},{},																		{EQUIP_TECHNOLOGY_BASIC}	}, "Colonel %s"			}	// points: 7
,	{{{1,1,1},{0,1},{2,1,},1,{},{COMBAT_STATUS_NONE,COMBAT_STATUS_BLEEDING},							{EQUIP_TECHNOLOGY_BASIC}	}, "Zombie %s"			}	// points: 8
,	{{{1,2,1},{0,1},{2,1,},1,{},{COMBAT_STATUS_NONE,COMBAT_STATUS_BLIND},								{EQUIP_TECHNOLOGY_BASIC}	}, "Undead %s"			}	// points: 9
,	{{{2,2,1},{0,1},{2,1,},1,{},{COMBAT_STATUS_NONE,COMBAT_STATUS_STUN},								{EQUIP_TECHNOLOGY_BASIC}	}, "Possessed %s"		}	// points: 10
,	{{{2,2,1},{0,1},{2,2,},1,{ATTACK_EFFECT_NONE,DEFEND_EFFECT_NONE,PASSIVE_EFFECT_SHIELD_REPAIR},{},	{EQUIP_TECHNOLOGY_GENETIC}	}, "Mutant %s"			}	// points: 11
,	{{{2,2,2},{0,1},{2,2,},1,{},{},																		{EQUIP_TECHNOLOGY_BASIC}	}, "Champion %s"		}	// points: 12
,	{{{2,2,2},{1,1},{2,2,},1,{},{},																		{EQUIP_TECHNOLOGY_BASIC}	}, "Elite %s"			}	// points: 13
,	{{{2,2,2},{1,1},{2,2,},2,{},{},																		{EQUIP_TECHNOLOGY_BASIC}	}, "King %s"			}	// points: 14
,	{{{3,2,2},{1,1},{2,2,},2,{},{},																		{EQUIP_TECHNOLOGY_BASIC}	}, "%s CEO"				}	// points: 15
,	{{{3,2,2},{1,1,1},{2,2,},2,{},{},																	{EQUIP_TECHNOLOGY_MAGIC}	}, "Demon %s"			}	// points: 16
,	{{{3,2,2},{1,1,1},{2,3,},2,{},{},																	{EQUIP_TECHNOLOGY_BASIC}	}, "%s Mad Scientist"	}	// points: 17
,	{{{3,2,2},{1,1,1},{3,3,},2,{},{COMBAT_STATUS_NONE, COMBAT_STATUS_BLEEDING},							{EQUIP_TECHNOLOGY_DIGITAL}	}, "Robot %s"			}	// points: 18
,	{{{3,2,2},{1,2,1},{3,3,},2,{},{},																	{EQUIP_TECHNOLOGY_BASIC}	}, "Emperor %s"			}	// points: 19
,	{{{3,2,2},{2,2,1},{3,3,},2,{},{},																	{EQUIP_TECHNOLOGY_MAGIC}	}, "Demigod %s"			}	// points: 20
,	{{{3,2,2},{2,2,2},{3,3,},2,{},{},																	{EQUIP_TECHNOLOGY_MAGIC}	}, "Angel %s"			}	// points: 21
,	{{{3,2,3},{2,2,2},{3,3,},2,{},{},																	{EQUIP_TECHNOLOGY_MAGIC}	}, "%s God"				}	// points: 22
//,	{{{3,2,2},{2,2,2},{3,3,},2,{},{},																	{EQUIP_TECHNOLOGY_BASIC}	}, "%s God"				}	// points: 23
};

// Probably the base points should be 8 instead of six considering it's difficult to advance in the game with so little points.	
static const CProfession definitionsProfession[] =
{	{{{1,1,1},{1,1,1},{1,1},1,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Nobody"				}
,	{{{3},{},{0,3},0,{},{}			,{EQUIP_TECHNOLOGY_BASIC}}	, "Heavy"				}
,	{{{2},{2},{3,1},0,{},{}			,{EQUIP_TECHNOLOGY_BASIC}}	, "Support"				}
,	{{{1},{},{2,3},0,{},{}			,{EQUIP_TECHNOLOGY_BASIC}}	, "Assault"				}
,	{{{},{},{4,4},0,{},{}			,{EQUIP_TECHNOLOGY_BASIC}}	, "Sniper"				}
,	{{{2,0,1},{1,0},{1,3},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Swordman"			}
,	{{{0,2,0},{0,1},{1,4},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Mage"				}
,	{{{1,0,0},{1,0},{3,2},1,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Thief"				}
,	{{{2,2,0},{1,2},{1,0},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Acolyte"				}
,	{{{3,0,0},{0,0},{0,3},2,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Merchant"			}
,	{{{2,0,0},{0,0},{3,3},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Archer"				}
,	{{{3,1,1},{0,0},{1,2},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Driver"				}
,	{{{1,0,2},{0,0},{2,3},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Raider"				}
,	{{{1,1,0},{0,1},{1,4},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Sorcerer"			}
,	{{{1,0,0},{0,0},{4,3},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Ninja"				}
,	{{{2,0,1},{0,0},{1,4},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Grenadier"			}
,	{{{1,0,0},{1,1},{0,4},1,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Biochemist"			}
,	{{{2,0,0},{0,0},{2,4},0,{},{}	,{EQUIP_TECHNOLOGY_BASIC}}	, "Assassin"			}
//,	{{{},{},{2,4},0,{},{}}, "Tanya"		}
//,	{{{},{},{2,4},0,{},{}}, "Natasha"	}
//,	{{{},{},{2,4},0,{},{}}, "Yuriko"	}
,	{{{1,1,1},{},{3,2},0,{},{}		,{EQUIP_TECHNOLOGY_BASIC}}		, "Imperial Soldier"	}
,	{{{1,1,1},{},{3,2},0,{},{}		,{EQUIP_TECHNOLOGY_BASIC}}		, "Conscript"			}
,	{{{1,1,1},{},{3,2},0,{},{}		,{EQUIP_TECHNOLOGY_BASIC}}		, "Peacekeeper"			}
,	{{{1,0,2},{},{2,3},0,{},{}		,{EQUIP_TECHNOLOGY_BASIC}}		, "Flak Trooper"		}
,	{{{1,0,2},{},{2,3},0,{},{}		,{EQUIP_TECHNOLOGY_BASIC}}		, "Javelin Soldier"		}
,	{{{1,0,2},{},{2,3},0,{},{}		,{EQUIP_TECHNOLOGY_BASIC}}		, "Tank Buster"			}
,	{{{2,0,1},{},{2,3},0,{ATTACK_EFFECT_NONE,DEFEND_EFFECT_NONE,PASSIVE_EFFECT_SHIELD_REPAIR}, {},{EQUIP_TECHNOLOGY_BASIC}	}													, "Engineer"	}
,	{{{3,0,0},{},{1,4},0,{},{}		,{EQUIP_TECHNOLOGY_BASIC}}																													, "Executioner"	}
,	{{{1,0,1},{},{3,3},0,{},{}		,{EQUIP_TECHNOLOGY_BASIC}}																													, "Commando"	}
,	{{{0,2,0},{0,1,0},{2,3},0,{}, {},{EQUIP_TECHNOLOGY_GENETIC}}																												, "Sectoid"		}
,	{{{0,0,1},{0,0,1},{3,3},0,{}, {},{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_GENETIC|EQUIP_TECHNOLOGY_DIGITAL)}}																	, "Android"		}
,	{{{1,0,1},{1,0,1},{2,2},0,{}, {},{EQUIP_TECHNOLOGY_DIGITAL}}																												, "Cyborg"		}
,	{{{0,0,0},{0,0,1},{3,4},0,{ATTACK_EFFECT_NONE,DEFEND_EFFECT_NONE,PASSIVE_EFFECT_SHIELD_REPAIR}, { COMBAT_STATUS_BURN,COMBAT_STATUS_BLEEDING},{EQUIP_TECHNOLOGY_DIGITAL}}	, "Drone"		}
,	{{{2,1,0},{1,1,0},{0,3},0,{ATTACK_EFFECT_LEECH_HEALTH,DEFEND_EFFECT_NONE,PASSIVE_EFFECT_MANA_REGEN},{}}													, "Vampire"		}
,	{{{4,0,0},{},{0,4},0,{ATTACK_EFFECT_LEECH_HEALTH},{},{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_BASIC|EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_GENETIC)}}	, "Valkyr"		}
,	{{{2,2,0},{2,2,0},{0,0},0,{ATTACK_EFFECT_NONE,DEFEND_EFFECT_NONE,PASSIVE_EFFECT_SHIELD_REPAIR},{},{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_BASIC|EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_GENETIC)}} , "Trinity"	}
,	{{{3,2,1},{},{0,2},0,{},{}							,{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_BASIC|EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_GENETIC)}}	, "Rhino"	}
,	{{{0,1,0},{},{3,4},0,{},{}							,{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_BASIC|EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_GENETIC)}}	, "Mesa"	}
,	{{{0,1,2},{0,1,2},{0,1},1,{},{}						,{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_BASIC|EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_GENETIC)}}	, "Mag"		}
,	{{{0,2,0},{0,1,0},{2,2},1,{},{COMBAT_STATUS_BLIND}	,{(EQUIP_TECHNOLOGY)(EQUIP_TECHNOLOGY_BASIC|EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_GENETIC)}}	, "Loki"	}
};

	static std::string getProfessionName(const SProfession& profession)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, modifiersProfession[profession.Modifier].Name.c_str(), definitionsProfession[profession.Index].Name.c_str());
		return formattedName;
	}

	static SCharacterPoints getProfessionPoints(const SProfession& profession) {
		SCharacterPointsMultipliers	multipliers;

		multipliers.MaxLife		= {.5, .125, .1};
		multipliers.CurrentLife	= {.2, .1, .1};
		multipliers.Attack		= {.1, .1};
		multipliers.Coins		= 0.125;

		multipliers				= multipliers*profession.Level;

		return (definitionsProfession[profession.Index].Points + modifiersProfession[profession.Modifier].Points)*multipliers;
	}
}

#endif __PROFESSION_H__986349827346982734__
