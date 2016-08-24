#include "GameBase.h"
#include "Misc.h"

#include <string>

#ifndef __ITEM_H__98214809271346928734293846__
#define __ITEM_H__98214809271346928734293846__

namespace klib
{
	enum ITEM_TYPE : uint16_t
	{	ITEM_TYPE_UNKNOWN	= 0x00
	,	ITEM_TYPE_POTION	= 0x01
	,	ITEM_TYPE_GRENADE	= 0x02
	,	ITEM_TYPE_BOX		= 0x04
	};

	enum PROPERTY_TYPE : uint32_t
	{	PROPERTY_TYPE_NONE		= 0x00000
		// good properties
	,	PROPERTY_TYPE_HEALTH	= 0x00001
	,	PROPERTY_TYPE_MANA		= 0x00002
	,	PROPERTY_TYPE_SHIELD	= 0x00004
	,	PROPERTY_TYPE_STRENGTH	= 0x00008
	,	PROPERTY_TYPE_HIT		= 0x00010
		// bad properties
	,	PROPERTY_TYPE_BLAST		= 0x00020
	,	PROPERTY_TYPE_PIERCING	= 0x00040
	,	PROPERTY_TYPE_FRAG		= 0x00080
	,	PROPERTY_TYPE_STUN		= 0x00100
	,	PROPERTY_TYPE_SMOKE		= 0x00200
	,	PROPERTY_TYPE_FIRE		= 0x00400
	,	PROPERTY_TYPE_POISON	= 0x00800
	,	PROPERTY_TYPE_COLD		= 0x01000
	,	PROPERTY_TYPE_FREEZE	= 0x02000
	,	PROPERTY_TYPE_SHOCK		= 0x04000
	,	PROPERTY_TYPE_EMP		= 0x08000
	,	PROPERTY_TYPE_SLEEP		= 0x10000
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

	struct CItemModifier
	{
		SCharacterPoints	Points;
		int32_t				Price;
		std::string			Name;
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

static const CItemModifier itemModifiers[] = 
{	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	0	,	"%s"					}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	0	,	"%s	of Healing"			}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"%s	of Mana"			}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	10	,	"%s	of Shield"			}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	10	,	"%s	of Rejuvenation"	}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"%s	of Focus"			}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	10	,	"Strength %s"			}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	0	,	"Smoke %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	0	,	"Sleep %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"Stun %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"Blast %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"Fire %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"Needle %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"Poison %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"Cold %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"Freeze %s"				}
,	{{{0	,0	,0	},{0	,0	,0	},{0	,0	},0	,{}},	5	,	"Shock %s"				}
};

static const CItemV2 itemDefinitions[] = 
	// Type										Price		Name
{	{ITEM_TYPE_UNKNOWN							,	0	,	"Water"			}	// the invalid description should be always in the last element
,	{ITEM_TYPE_POTION							,	10	,	"Potion"		}
,	{ITEM_TYPE_GRENADE							,	20	,	"Grenade"		}
,	{(ITEM_TYPE)(ITEM_TYPE_BOX|ITEM_TYPE_POTION	),	100	,	"Potion Box"	}
,	{(ITEM_TYPE)(ITEM_TYPE_BOX|ITEM_TYPE_GRENADE),	200	,	"Grenade Box"	}
};

static const CItem itemDescriptions[] = 
	// Type					Property				Price	Name
{	{ITEM_TYPE_UNKNOWN	,	PROPERTY_TYPE_NONE											,	0	,	"Water"							}	// the invalid description should be always in the last element
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HEALTH										,	10	,	"Potion of Healing"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_MANA											,	10	,	"Potion of Mana"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_SHIELD										,	10	,	"Potion of Shield"				}
,	{ITEM_TYPE_POTION	,	(PROPERTY_TYPE)(PROPERTY_TYPE_MANA|PROPERTY_TYPE_HEALTH)	,	10	,	"Potion of Rejuvenation"		}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_HIT											,	10	,	"Potion of Focus"				}
,	{ITEM_TYPE_POTION	,	PROPERTY_TYPE_STRENGTH										,	10	,	"Potion of Strength"			}
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

	static std::string getItemName(const SItem& item)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, itemGrades[item.Level].Name.c_str(), itemDescriptions[item.Index].Name.c_str());
		return formattedName;
	}

	static inline int32_t getItemPrice(const SItem& item)
	{
		return itemDescriptions[item.Index].Price*item.Level;
	}
} // namespace

#endif // __ITEM_H__98214809271346928734293846__