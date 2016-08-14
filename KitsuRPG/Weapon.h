#pragma once

#include <string>

enum STATUS_TYPE
{	STATUS_TYPE_NONE
,	STATUS_TYPE_BLEEDING
,	STATUS_TYPE_STUN
};

struct SCharacterPoints
{
	int	MaxHP	;
	int	HP		;
	int	Hit		;
	int	Attack	;
	int	Coins	;
};

struct SWeapon
{
	SCharacterPoints	Points;
	STATUS_TYPE			Status;
	std::string			Name;
};

static const SWeapon weaponDefinitions[] = 
{	{	{	0,	0,	 0,		0,	0},	STATUS_TYPE_NONE		,	"None"						}
,	{	{	0,	0,	 0,		0,	1},	STATUS_TYPE_NONE		,	"Glove of the Thief"		}
,	{	{	0,	1,	 0,		0,	0},	STATUS_TYPE_NONE		,	"Glove of Acolyte"			}
,	{	{	0,	0,	 0,		2,	0},	STATUS_TYPE_STUN		,	"Club"						}
,	{	{	0,	0,	 0,		2,	1},	STATUS_TYPE_STUN		,	"Club of the Thief"			}
,	{	{	0,	1,	 0,		2,	0},	STATUS_TYPE_STUN		,	"Club of Acolyte"			}
,	{	{	0,	0,	10,		2,	0},	STATUS_TYPE_NONE		,	"Knuckle"					}
,	{	{	0,	0,	10,		2,	1},	STATUS_TYPE_NONE		,	"Knuckle of the Thief"		}
,	{	{	0,	1,	10,		2,	0},	STATUS_TYPE_NONE		,	"Knuckle of Acolyte"		}
,	{	{	0,	0,	 0,		3,	0},	STATUS_TYPE_STUN		,	"Spiked Club"				}
,	{	{	0,	0,	 0,		3,	2},	STATUS_TYPE_STUN		,	"Spiked Club of the Rogue"	}
,	{	{	0,	2,	 0,		3,	0},	STATUS_TYPE_STUN		,	"Spiked Club of the Priest"	}
,	{	{	0,	0,	10,		3,	0},	STATUS_TYPE_BLEEDING	,	"Knife"						}
,	{	{	0,	0,	10,		3,	2},	STATUS_TYPE_BLEEDING	,	"Knife of the Rogue"		}
,	{	{	0,	2,	10,		3,	0},	STATUS_TYPE_BLEEDING	,	"Knife of the Priest"		}
,	{	{	0,	0,	15,		3,	0},	STATUS_TYPE_BLEEDING	,	"Claw"						}
,	{	{	0,	0,	15,		3,	2},	STATUS_TYPE_BLEEDING	,	"Claw of the Rogue"			}
,	{	{	0,	2,	15,		3,	0},	STATUS_TYPE_BLEEDING	,	"Claw of the Priest"		}
,	{	{	0,	0,	5,		4,	0},	STATUS_TYPE_BLEEDING	,	"Machete"					}
,	{	{	0,	0,	5,		4,	2},	STATUS_TYPE_BLEEDING	,	"Machete of the Rogue"		}
,	{	{	0,	2,	5,		4,	0},	STATUS_TYPE_BLEEDING	,	"Machete of the Priest"		}
,	{	{	0,	0,	5,		4,	0},	STATUS_TYPE_STUN		,	"Mace"						}
,	{	{	0,	0,	5,		4,	2},	STATUS_TYPE_STUN		,	"Mace of the Rogue"			}
,	{	{	0,	2,	5,		4,	0},	STATUS_TYPE_STUN		,	"Mace of the Priest"		}
,	{	{	0,	0,	5,		5,	0},	STATUS_TYPE_BLEEDING	,	"Sword"						}
,	{	{	0,	0,	5,		5,	3},	STATUS_TYPE_BLEEDING	,	"Assassin's Sword"			}
,	{	{	0,	3,	5,		5,	0},	STATUS_TYPE_BLEEDING	,	"Saint's Sword"				}
,	{	{	0,	0,	0,		6,	0},	STATUS_TYPE_BLEEDING	,	"Axe"						}
,	{	{	0,	0,	0,		6,	3},	STATUS_TYPE_BLEEDING	,	"Assassin's Axe"			}
,	{	{	0,	3,	0,		6,	0},	STATUS_TYPE_BLEEDING	,	"Saint's Axe"				}
,	{	{	0,	0,	-5,		7,	0},	STATUS_TYPE_NONE		,	"Spear"						}
,	{	{	0,	0,	-5,		7,	4},	STATUS_TYPE_NONE		,	"Executioner's Spear"		}
,	{	{	0,	4,	-5,		7,	0},	STATUS_TYPE_NONE		,	"Angel's Spear"				}
,	{	{	0,	0,	-10,	8,	0},	STATUS_TYPE_NONE		,	"Bow"						}
,	{	{	0,	0,	-10,	8,	4},	STATUS_TYPE_NONE		,	"Executioner's Bow"			}
,	{	{	0,	4,	-10,	8,	0},	STATUS_TYPE_NONE		,	"Angel's Bow"				}
};