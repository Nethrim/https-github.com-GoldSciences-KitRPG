#include "EntityImpl.h"
#include "Misc.h"

#include <string>

#ifndef __ITEM_H__98214809271346928734293846__
#define __ITEM_H__98214809271346928734293846__

namespace klib
{
#pragma pack(push, 1)

	enum ITEM_TYPE : uint16_t
	{	ITEM_TYPE_UNKNOWN	= 0x00
	,	ITEM_TYPE_POTION	= 0x01
	,	ITEM_TYPE_GRENADE	= 0x02
	,	ITEM_TYPE_BOX		= 0x04
	};

	enum PROPERTY_TYPE : uint32_t
	{	PROPERTY_TYPE_NONE				= 0x000000
	,	PROPERTY_TYPE_HEALTH			= 0x000001
	,	PROPERTY_TYPE_MANA				= 0x000002
	,	PROPERTY_TYPE_SHIELD			= 0x000004
	,	PROPERTY_TYPE_DAMAGE			= 0x000008
	,	PROPERTY_TYPE_HIT				= 0x000010
	,	PROPERTY_TYPE_ATTACK_SPEED		= 0x000020
	,	PROPERTY_TYPE_MOVEMENT_SPEED	= 0x000040
	,	PROPERTY_TYPE_REFLEXES			= 0x000080
	,	PROPERTY_TYPE_ABSORPTION		= 0x000100
	,	PROPERTY_TYPE_HEALTH_DAMAGE		= 0x000200
	,	PROPERTY_TYPE_MANA_DAMAGE		= 0x000400
	,	PROPERTY_TYPE_SHIELD_DAMAGE		= 0x000800
	,	PROPERTY_TYPE_BLAST				= 0x001000
	,	PROPERTY_TYPE_PIERCING			= 0x002000
	,	PROPERTY_TYPE_FRAG				= 0x004000
	,	PROPERTY_TYPE_STUN				= 0x008000
	,	PROPERTY_TYPE_SMOKE				= 0x010000
	,	PROPERTY_TYPE_FIRE				= 0x020000
	,	PROPERTY_TYPE_POISON			= 0x040000
	,	PROPERTY_TYPE_COLD				= 0x080000
	,	PROPERTY_TYPE_FREEZE			= 0x100000
	,	PROPERTY_TYPE_SHOCK				= 0x200000
	,	PROPERTY_TYPE_EMP				= 0x400000
	,	PROPERTY_TYPE_SLEEP				= 0x800000
	};

	struct CItem 
	{
		ITEM_TYPE		Type;
		PROPERTY_TYPE	Property;
		int32_t			Price;
		std::string		Name;
	};

	struct CItemV2
	{
		ITEM_TYPE		Type;
		int32_t			Price;
		std::string		Name;
	};

	struct CItemGrade
	{
		std::string		Name;
	};

static const CItemGrade itemGrades[] = 
{	{	"%s (Prop)"		}
,	{	"%s (Small)"	}
,	{	"%s (Regular)"	}
,	{	"%s (Large)"	}
//,	{	"%s (Huge)"		}
};

static const CRecordItem	itemModifiers[] = 
{	{	{{},{},{},0,0	},{},	"%s"					}
,	{	{{},{},{},0,0	},{},	"%s	of Healing"			}
,	{	{{},{},{},0,5	},{},	"%s	of Mana"			}
,	{	{{},{},{},0,5	},{},	"%s	of Shield"			}
,	{	{{},{},{},0,15	},{},	"%s	of Rejuvenation"	}
,	{	{{},{},{},0,5	},{},	"%s	of Focus"			}
,	{	{{},{},{},0,10	},{},	"Strength %s"			}
,	{	{{},{},{},0,10	},{},	"Speed %s"				}
,	{	{{},{},{},0,10	},{},	"Sprint %s"				}
,	{	{{},{},{},0,10	},{},	"%s of Reflexes"		}
,	{	{{},{},{},0,10	},{},	"%s of Shielding"		}
,	{	{{},{},{},0,10	},{},	"Epidemic %s"			}
,	{	{{},{},{},0,10	},{},	"%s of Damnation"		}
,	{	{{},{},{},0,10	},{},	"%s of Corrosion"		}
,	{	{{},{},{},0,0	},{},	"Smoke %s"				}
,	{	{{},{},{},0,0	},{},	"Sleep %s"				}
,	{	{{},{},{},0,5	},{},	"Stun %s"				}
,	{	{{},{},{},0,5	},{},	"Blast %s"				}
,	{	{{},{},{},0,5	},{},	"Fire %s"				}
,	{	{{},{},{},0,5	},{},	"Needle %s"				}
,	{	{{},{},{},0,5	},{},	"Poison %s"				}
,	{	{{},{},{},0,5	},{},	"Cold %s"				}
,	{	{{},{},{},0,5	},{},	"Freeze %s"				}
,	{	{{},{},{},0,5	},{},	"Shock %s"				}
,	{	{{},{},{},0,5	},{},	"EMP %s"				}
};

static const CItemV2 itemDefinitions[] = 
	// Type										Price		Name
{	{ITEM_TYPE_UNKNOWN							,	0	,	"Advice"			}	// the invalid description should be always in the last element
,	{ITEM_TYPE_POTION							,	10	,	"Potion"			}
,	{ITEM_TYPE_GRENADE							,	20	,	"Grenade"			}
//,	{(ITEM_TYPE)(ITEM_TYPE_BOX|ITEM_TYPE_POTION	),	100	,	"Potion Box"		}
//,	{(ITEM_TYPE)(ITEM_TYPE_BOX|ITEM_TYPE_GRENADE),	200	,	"Grenade Box"		}
};

static const CItem itemDescriptions[] = 
	// Type					Property				Price	Name
{	{ITEM_TYPE_UNKNOWN	,	PROPERTY_TYPE_NONE											,	0	,	"Water"							}	// the invalid description should be always in the last element
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH										,	10	,	"Potion of Healing"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_MANA											,	10	,	"Potion of Mana"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_SHIELD										,	10	,	"Potion of Shield"				}
,	{ITEM_TYPE_POTION	,	(PROPERTY_TYPE)(PROPERTY_TYPE_MANA|PROPERTY_TYPE_HEALTH)	,	10	,	"Potion of Rejuvenation"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT											,	10	,	"Potion of Focus"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_DAMAGE										,	10	,	"Potion of Strength"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_ATTACK_SPEED									,	10	,	"Potion of Speed"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_MOVEMENT_SPEED								,	10	,	"Potion of Sprint"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_REFLEXES										,	10	,	"Potion of Reflexes"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_ABSORPTION									,	10	,	"Potion of Shielding"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH_DAMAGE									,	10	,	"Epidemic Potion"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_MANA_DAMAGE									,	10	,	"Potion of Damnation"			}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_SHIELD_DAMAGE									,	10	,	"Potion of Corrosion"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_SMOKE											,	10	,	"Smoke Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_SLEEP											,	10	,	"Sleep Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_STUN											,	10	,	"Stun Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_BLAST											,	10	,	"Blast Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_FIRE											,	10	,	"Fire Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_PIERCING										,	10	,	"Needle Grenade"				}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_FRAG											,	10	,	"Fragmentary Grenade"			}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_POISON										,	10	,	"Poison Grenade"				}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_SHOCK											,	10	,	"Shock Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_COLD											,	10	,	"Cold Grenade"					}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_FREEZE										,	10	,	"Freeze Grenade"				}
,	{ITEM_TYPE_GRENADE	,	PROPERTY_TYPE_EMP											,	10	,	"EMP Grenade"					}
};

	static inline constexpr int32_t getFinalItemCount() {
		return (int32_t)((size(itemDescriptions)-1)*size(itemGrades));
	}

	static std::string getItemName(const SItem& item) {
		char formattedName[128] = {};
		sprintf_s(formattedName, itemGrades[item.Level].Name.c_str(), itemDescriptions[item.Definition].Name.c_str());
		return formattedName;
	}

	static inline int32_t getItemPrice(const SItem& item, bool bSellPrice=false) {
		return (int32_t)( bSellPrice ? (itemDescriptions[item.Definition].Price*item.Level)*.5 : itemDescriptions[item.Definition].Price*item.Level );
	}

#pragma pack(pop)
} // namespace

#endif // __ITEM_H__98214809271346928734293846__