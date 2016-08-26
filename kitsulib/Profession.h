#include "EntityImpl.h"
#include "EntityHelper.h"

#ifndef __PROFESSION_H__986349827346982734__
#define __PROFESSION_H__986349827346982734__

namespace klib
{
#pragma pack(push, 1)

#define DRONE_EFFECTS			{klib::ATTACK_EFFECT_NONE,klib::DEFEND_EFFECT_NONE,klib::PASSIVE_EFFECT_SHIELD_REPAIR}
#define MUTANT_EFFECTS DRONE_EFFECTS

//	{SEntityPoints, SEntityFlags, Name}
static const CRecordProfession modifiersProfession[] = 
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Rookie %s"			}
,	{{{1,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Squaddie %s"		}	// points: 1
,	{{{1,0,0},{0,0,0}	,{1,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Corporal %s"		}	// points: 2
,	{{{1,1,0},{0,0,0}	,{1,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Sergeant %s"		}	// points: 3
,	{{{1,1,1},{0,0,0}	,{1,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Lieutenant %s"		}	// points: 4
,	{{{1,1,1},{0,0,0}	,{1,0,{0,0,0},0,{0,0,0}},1,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Captain %s"		}	// points: 5
,	{{{1,1,1},{0,0,0}	,{1,1,{0,0,0},0,{0,0,0}},1,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Major %s"			}	// points: 6
,	{{{1,1,1},{0,1,0}	,{1,1,{0,0,0},0,{0,0,0}},1,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Colonel %s"		}	// points: 7
,	{{{1,1,1},{0,1,0}	,{2,1,{0,0,0},0,{0,0,0}},1,0}	,{{},{COMBAT_STATUS_NONE,COMBAT_STATUS_POISON}		,{ENTITY_TECHNOLOGY_BASIC	}}, "Zombie %s"			}	// points: 8
,	{{{1,2,1},{0,1,0}	,{2,1,{0,0,0},0,{0,0,0}},1,0}	,{{},{COMBAT_STATUS_NONE,COMBAT_STATUS_BLIND}		,{ENTITY_TECHNOLOGY_BASIC	}}, "Undead %s"			}	// points: 9
,	{{{2,2,1},{0,1,0}	,{2,1,{0,0,0},0,{0,0,0}},1,0}	,{{},{COMBAT_STATUS_NONE,COMBAT_STATUS_STUN}		,{ENTITY_TECHNOLOGY_BASIC	}}, "Possessed %s"		}	// points: 10
,	{{{2,2,1},{0,1,0}	,{2,2,{0,0,0},0,{0,0,0}},1,0}	,{MUTANT_EFFECTS ,{}								,{ENTITY_TECHNOLOGY_GENETIC	}}, "Mutant %s"			}	// points: 11
,	{{{2,2,2},{0,1,0}	,{2,2,{0,0,0},0,{0,0,0}},1,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Champion %s"		}	// points: 12
,	{{{2,2,2},{1,1,0}	,{2,2,{0,0,0},0,{0,0,0}},1,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Elite %s"			}	// points: 13
,	{{{2,2,2},{1,1,0}	,{2,2,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "King %s"			}	// points: 14
,	{{{3,2,2},{1,1,0}	,{2,2,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "%s CEO"			}	// points: 15
,	{{{3,2,2},{1,1,1}	,{2,2,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_MAGIC	}}, "Demon %s"			}	// points: 16
,	{{{3,2,2},{1,1,1}	,{2,3,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "%s Mad Scientist"	}	// points: 17
,	{{{3,2,2},{1,1,1}	,{3,3,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "Emperor %s"		}	// points: 19
,	{{{3,2,2},{1,1,2}	,{3,3,{0,0,0},0,{0,0,0}},2,0}	,{{},{COMBAT_STATUS_NONE, COMBAT_STATUS_BLEEDING}	,{ENTITY_TECHNOLOGY_DIGITAL	}}, "Robot %s"			}	// points: 18
,	{{{3,2,2},{1,2,2}	,{3,3,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_MAGIC	}}, "Demigod %s"		}	// points: 20
,	{{{3,2,2},{2,2,2}	,{3,3,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_MAGIC	}}, "Angel %s"			}	// points: 21
,	{{{3,2,3},{2,2,2}	,{3,3,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_MAGIC	}}, "%s God"			}	// points: 22
//,	{{{3,2,2},{2,2,2}	,{3,3,{0,0,0},0,{0,0,0}},2,0}	,{{},{}												,{ENTITY_TECHNOLOGY_BASIC	}}, "%s God"			}	// points: 23
};	
	

#define DRONE_COMBAT_STATUS		{klib::COMBAT_STATUS_BURN,klib::COMBAT_STATUS_BLEEDING}
#define VAMPIRE_EFFECTS			{klib::ATTACK_EFFECT_LEECH_HEALTH,klib::DEFEND_EFFECT_NONE,klib::PASSIVE_EFFECT_MANA_REGEN}
#define ANDROID_TECHNOLOGY		(klib::ENTITY_TECHNOLOGY)(klib::ENTITY_TECHNOLOGY_GENETIC|klib::ENTITY_TECHNOLOGY_DIGITAL)

// Currently the amount of points available for a single character is 14.	
//	{SEntityPoints, SEntityFlags, Name}
static const CRecordProfession definitionsProfession[] =
{	{{{1,1,1},{1,1,1}	,{1,1,{1,1,1},1,{1,1,1}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "John Doe"		}
,	{{{4,0,4},{0,0,0}	,{0,4,{0,0,0},3,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Heavy"			}
,	{{{1,4,0},{1,3,1}	,{2,0,{2,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Support"		}
,	{{{2,0,1},{0,0,0}	,{2,5,{2,0,0},2,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Assault"		}
,	{{{0,1,0},{0,0,0}	,{7,6,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Sniper"		}
,	{{{2,0,1},{0,0,0}	,{5,1,{4,0,0},0,{0,0,0}},2,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Thief"			}
,	{{{0,5,0},{2,5,0}	,{0,0,{2,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Acolyte"		}
,	{{{1,0,1},{0,0,0}	,{4,4,{4,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Archer"		}
,	{{{4,0,2},{0,0,0}	,{2,4,{0,0,0},2,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Swordman"		}
,	{{{2,0,1},{0,0,0}	,{2,4,{0,0,0},2,{0,0,0}},3,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Merchant"		}
,	{{{0,3,0},{0,3,0}	,{3,5,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Mage"			}
,	{{{0,2,0},{1,4,0}	,{1,6,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Sorcerer"		}
,	{{{0,2,0},{0,0,0}	,{4,4,{4,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Ninja"			}
,	{{{2,0,2},{1,0,0}	,{1,6,{0,0,0},2,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Grenadier"		}
,	{{{2,1,0},{1,1,0}	,{0,6,{0,0,0},3,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Biochemist"	}
,	{{{2,0,1},{0,0,0}	,{3,5,{3,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Assassin"		}
,	{{{2,0,1},{0,0,0}	,{4,6,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Executioner"	}
,	{{{1,0,1},{0,0,0}	,{5,5,{2,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC								}}, "Commando"		}
,	{{{0,0,1},{1,0,1}	,{5,5,{1,0,0},0,{0,0,0}},0,0}	,{DRONE_EFFECTS, {},{ENTITY_TECHNOLOGY_BASIC					}}, "Engineer"		}
,	{{{1,0,1},{0,0,1}	,{6,4,{1,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ANDROID_TECHNOLOGY									}}, "Android"		}
,	{{{2,0,2},{0,0,0}	,{4,5,{1,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL								}}, "Cyborg"		}
,	{{{0,0,0},{0,0,1}	,{5,5,{3,0,0},0,{0,0,0}},0,0}	,{DRONE_EFFECTS, DRONE_COMBAT_STATUS,{ENTITY_TECHNOLOGY_DIGITAL	}}, "Drone"			}
,	{{{3,2,0},{1,1,0}	,{1,5,{1,0,0},0,{0,0,0}},0,0}	,{VAMPIRE_EFFECTS,{COMBAT_STATUS_BLEEDING						}}, "Vampire"		}
};

	static std::string getProfessionName(const SProfession& profession) {
		return getEntityName(profession, definitionsProfession, modifiersProfession);
	}

	static SEntityPoints getProfessionPoints(const SProfession& profession) {
		return getEntityPoints(profession, definitionsProfession, modifiersProfession, profession.getMultipliers());
	}

	static SEntityFlags getProfessionFlags(const SProfession& profession) {
		return  getEntityFlags(profession, definitionsProfession, modifiersProfession);
	}

#pragma pack(pop)
}

#endif __PROFESSION_H__986349827346982734__
