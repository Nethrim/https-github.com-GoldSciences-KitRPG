#include "EntityImpl.h"
#include "EntityHelper.h"

#ifndef __FACILITIES_H__986349827346982734__
#define __FACILITIES_H__986349827346982734__

namespace klib
{
#pragma pack(push, 1)

#define DRONE_EFFECTS			{klib::ATTACK_EFFECT_NONE,klib::DEFEND_EFFECT_NONE,klib::PASSIVE_EFFECT_SHIELD_REPAIR}
#define MUTANT_EFFECTS DRONE_EFFECTS

//	{SEntityPoints, SEntityFlags, Name}
static const CRecordFacility modifiersFacilities[] = 
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Small %s"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s"			}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Advanced %s"	}
};	
	

#define DRONE_COMBAT_STATUS		{klib::COMBAT_STATUS_BURN,klib::COMBAT_STATUS_BLEEDING}
#define VAMPIRE_EFFECTS			{klib::ATTACK_EFFECT_LEECH_HEALTH,klib::DEFEND_EFFECT_NONE,klib::PASSIVE_EFFECT_MANA_REGEN}
#define ANDROID_TECHNOLOGY		(klib::ENTITY_TECHNOLOGY)(klib::ENTITY_TECHNOLOGY_GENETIC|klib::ENTITY_TECHNOLOGY_DIGITAL)

// Currently the amount of points available for a single character is 14.	
//	{SEntityPoints, SEntityFlags, Name}
static const CRecordFacility definitionsFacilities[] =
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Sheet"				}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Training Room"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Foundry"			}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Physics Labs"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Chemical Labs"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Mechanics Labs"	}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Quantum Labs"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Genetic Labs"		}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Shack"				}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Barn"				}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Wall"				}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Farm"				}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Town Hall"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Barracks"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Tower"				}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Store"				}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Castle"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Lumber Mill"		}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Shipyard"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Foundry"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Coal Mine"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Iron Refinery"		}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Steel Mill"		}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Paper Mill"		}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Oil Wells"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Oil Refinery"		}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Oil Rig"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Coal Power Plant"	}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Airbase"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Seaport"			}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Space Academy"		}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Lanch Platform"	}
//,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Landing Drone"		}
};

	static std::string getFacilityName(const SFacility& facilities) {
		return getEntityName(facilities, definitionsFacilities, modifiersFacilities);
	}

	static SEntityPoints getFacilityPoints(const SFacility& facilities) {
		return getEntityPoints(facilities, definitionsFacilities, modifiersFacilities, facilities.getMultipliers());
	}

	static SEntityFlags getFacilityFlags(const SFacility& facilities) {
		return  getEntityFlags(facilities, definitionsFacilities, modifiersFacilities);
	}

#pragma pack(pop)
}

#endif __FACILITIES_H__986349827346982734__
