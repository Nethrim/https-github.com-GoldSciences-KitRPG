#include "EntityImpl.h"
#include "EntityHelper.h"


#ifndef __STAGEPROP_H__98723409213640987236498273__
#define __STAGEPROP_H__98723409213640987236498273__

namespace klib
{
#pragma pack(push, 1)
static const CRecordStageProp modifiersStageProp	[]	= 
{	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"%s"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Reinforced %s"	}
};															
															
static const CRecordStageProp definitionsStageProp	[]	= 	
{	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Pot"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Lamp"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Mailbox"		}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Chest"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Semaphore"		}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Door"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Table"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Streetlight"	}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Chair"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Glass"			}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Screen"		}
,	{{{0,0,0},	{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}}	,0,0	, 1},{{},{}	,{ENTITY_TECHNOLOGY_BASIC	}},	"Wall"			}
};

	static std::string getStagePropName(const SStageProp& stageProp) {
		return getEntityName(stageProp, definitionsStageProp, modifiersStageProp);
	}

	static SEntityPoints getStagePropPoints(const SStageProp& stageProp) {
		return getEntityPoints(stageProp, definitionsStageProp, modifiersStageProp);
	}

	static SEntityFlags getStagePropFlags(const SStageProp& stageProp) {
		return  getEntityFlags(stageProp, definitionsStageProp, modifiersStageProp);
	}

#pragma pack(pop)
}


#endif // __STAGEPROP_H__98723409213640987236498273__
