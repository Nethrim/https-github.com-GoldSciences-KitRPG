#include "EntityImpl.h"
#include "EntityHelper.h"

#ifndef __FACILITIES_H__986349827346982734__
#define __FACILITIES_H__986349827346982734__

namespace klib
{
#pragma pack(push, 1)
//	{SEntityPoints, SEntityFlags, Name}
static const CRecordFacility modifiersFacilities[] = 
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Small %s"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Large %s"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Advanced %s"	}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s Hub"		}
};	
	
// Currently the amount of points available for a single character is 14.	
//	{SEntityPoints, SEntityFlags, Name}
static const CRecordFacility definitionsFacilities[] =
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Dog House"			}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Training Room"		}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Foundry"			}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Warfare Labs"		}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Physics Labs"		}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Chemical Labs"		}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Mechanics Labs"	}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Airport"			}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Digital Labs"		}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Launch Base"		}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Quantum Labs"		}
,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Genetic Labs"		}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Shack"				}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Barn"				}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Wall"				}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Farm"				}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Town Hall"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Barracks"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Tower"				}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Store"				}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Castle"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Lumber Mill"		}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Shipyard"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Foundry"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Coal Mine"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Iron Refinery"		}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Steel Mill"		}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Paper Mill"		}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Oil Wells"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Oil Refinery"		}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Oil Rig"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Coal Power Plant"	}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Airbase"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Seaport"			}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Space Academy"		}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Lanch Platform"	}
//,	{{{10,0,10},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Landing Drone"		}
};

	static std::string getFacilityName(const SFacility& facility) {
		return getEntityName(facility, definitionsFacilities, modifiersFacilities);
	}

	static SEntityPoints getFacilityPoints(const SFacility& facility) {
		return getEntityPoints(facility, definitionsFacilities, modifiersFacilities);
	}

	static SEntityFlags getFacilityFlags(const SFacility& facility) {
		return  getEntityFlags(facility, definitionsFacilities, modifiersFacilities);
	}

#pragma pack(pop)
}

#endif __FACILITIES_H__986349827346982734__
