#include "EntityImpl.h"
#include "EntityHelper.h"

#ifndef __ACCESSORY_H__928402836490238648723432__
#define __ACCESSORY_H__928402836490238648723432__

namespace klib
{
#pragma pack(push, 1)
//	{SEntityPoints, SEntityFlags, Name}
static const CRecordAccessory modifiersAccessory[] = 
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Cloth %s"			}
,	{{{0,0,0},{0,0,0}	,{1,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Wooden %s"			}
,	{{{0,0,0},{0,0,0}	,{1,0,{0,0,0},1,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Iron %s"			}
,	{{{0,0,0},{0,0,0}	,{1,0,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Gold %s"			}
,	{{{0,0,0},{0,0,0}	,{1,1,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Steel %s"			}
,	{{{0,0,0},{0,0,0}	,{2,1,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Glass %s"			}
,	{{{0,0,1},{0,0,0}	,{2,1,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Titanium %s"		}
,	{{{0,0,2},{0,0,0}	,{2,1,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Diamond %s"		}
,	{{{0,0,2},{0,0,0}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Laser %s"			}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Power %s"			}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of Ambition"	}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of Passion"		}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of Lust"		}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of Confidence"	}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of Trust"		}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of Anguish"		}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of Pain"		}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of Love"		}
,	{{{0,0,2},{0,0,1}	,{2,2,{0,0,0},1,{0,0,0}},1,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "%s of the Union"	}
};	
	
// Currently the amount of points available for a single character is 14.	
//	{SEntityPoints, SEntityFlags, Name}
static const CRecordAccessory definitionsAccessory[] =
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Bracelet"	}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Hairband"	}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Ring"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Gauntlet"	}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Monocle"	}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Scope"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Glasses"	}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}}, "Watch"		}
};

	static std::string getAccessoryName(const SAccessory& accessory) {
		return getEntityName(accessory, definitionsAccessory, modifiersAccessory);
	}

	static SEntityPoints getAccessoryPoints(const SAccessory& accessory) {
		return getEntityPoints(accessory, definitionsAccessory, modifiersAccessory);
	}

	static SEntityFlags getAccessoryFlags(const SAccessory& accessory) {
		return getEntityFlags(accessory, definitionsAccessory, modifiersAccessory);
	}

#pragma pack(pop)
}

#endif // __ACCESSORY_H__928402836490238648723432__
