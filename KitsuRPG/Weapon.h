#include "GameBase.h"

#include <string>

#ifndef __WEAPON_H__928364890236498716349825347895236__
#define __WEAPON_H__928364890236498716349825347895236__

enum WEAPON_EFFECT
{	WEAPON_EFFECT_NONE
,	WEAPON_EFFECT_LEECH
};

struct CWeapon
{
	SCharacterPoints	Points;
	WEAPON_EFFECT		Effect;
	STATUS_TYPE			Status;
	std::string			Name;
};

static const CWeapon weaponDefinitions[] = 
{	{	{	0,	0,	10,		0,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"None"						}
,	{	{	1,	0,	10,		0,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Glove of Resistance"		}
,	{	{	0,	0,	10,		0,	1},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Glove of the Thief"		}
,	{	{	0,	1,	10,		0,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Glove of Acolyte"			}
,	{	{	0,	0,	10,		0,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_NONE		,	"Vampire's Glove"			}
,	{	{	0,	0,	0,		1,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Club"						}
,	{	{	1,	0,	0,		1,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Club of Resistance"		}
,	{	{	0,	0,	0,		1,	1},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Club of the Thief"			}
,	{	{	0,	1,	0,		1,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Club of Acolyte"			}
,	{	{	0,	0,	0,		1,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_STUN		,	"Vampire's Club"			}
,	{	{	0,	0,	10,		1,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Knuckle"					}
,	{	{	1,	0,	10,		1,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Knuckle of Resistance"		}
,	{	{	0,	0,	10,		1,	1},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Knuckle of the Thief"		}
,	{	{	0,	1,	10,		1,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Knuckle of Acolyte"		}
,	{	{	0,	0,	10,		1,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLIND		,	"Vampire's Knuckle"			}
,	{	{	0,	0,	0,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Spiked Club"				}
,	{	{	2,	0,	0,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Spiked Club of Vitality"	}
,	{	{	0,	0,	0,		2,	2},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Spiked Club of the Rogue"	}
,	{	{	0,	2,	0,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Spiked Club of the Priest"	}
,	{	{	0,	0,	0,		2,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLEEDING	,	"Vampire's Spiked Club"		}
,	{	{	0,	0,	10,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Knife"						}
,	{	{	2,	0,	10,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Knife of Vitality"			}
,	{	{	0,	0,	10,		2,	2},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Knife of the Rogue"		}
,	{	{	0,	2,	10,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Knife of the Priest"		}
,	{	{	0,	0,	10,		2,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLEEDING	,	"Vampire's Knife"			}
,	{	{	0,	0,	15,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Claw"						}
,	{	{	2,	0,	15,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Claw of Vitality"			}
,	{	{	0,	0,	15,		2,	2},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Claw of the Rogue"			}
,	{	{	0,	2,	15,		2,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Claw of the Priest"		}
,	{	{	0,	0,	15,		2,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLEEDING	,	"Vampire's Claw"			}
,	{	{	0,	0,	5,		3,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Machete"					}
,	{	{	2,	0,	5,		3,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Machete of Vitality"		}
,	{	{	0,	0,	5,		3,	2},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Machete of the Rogue"		}
,	{	{	0,	2,	5,		3,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Machete of the Priest"		}
,	{	{	0,	0,	5,		3,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLEEDING	,	"Vampire's Machete"			}
,	{	{	0,	0,	5,		3,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Mace"						}
,	{	{	2,	0,	5,		3,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Mace of Vitality"			}
,	{	{	0,	0,	5,		3,	2},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Mace of the Rogue"			}
,	{	{	0,	2,	5,		3,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Mace of the Priest"		}
,	{	{	0,	0,	5,		3,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_STUN		,	"Vampire's Mace"			}
,	{	{	0,	0,	5,		4,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Sword"						}
,	{	{	3,	0,	5,		4,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Soldier's Sword"			}
,	{	{	0,	0,	5,		4,	3},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Assassin's Sword"			}
,	{	{	0,	3,	5,		4,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Saint's Sword"				}
,	{	{	0,	0,	5,		4,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLEEDING	,	"Vampire's Sword"			}
,	{	{	0,	0,	0,		5,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Axe"						}
,	{	{	3,	0,	0,		5,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Soldier's Axe"				}
,	{	{	0,	0,	0,		5,	3},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Assassin's Axe"			}
,	{	{	0,	3,	0,		5,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Saint's Axe"				}
,	{	{	0,	0,	0,		5,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLEEDING	,	"Vampire's Axe"				}
,	{	{	0,	0,	-5,		6,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Spear"						}
,	{	{	4,	0,	-5,		6,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Executioner's Spear"		}
,	{	{	0,	0,	-5,		6,	4},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Stalker's Spear"			}
,	{	{	0,	4,	-5,		6,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Angel's Spear"				}
,	{	{	0,	0,	-5,		6,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLEEDING	,	"Vampire's Spear"			}
,	{	{	0,	0,	-10,	7,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Bow"						}
,	{	{	4,	0,	-10,	7,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Executioner's Bow"			}
,	{	{	0,	0,	-10,	7,	4},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Stalker's Bow"				}
,	{	{	0,	4,	-10,	7,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Angel's Bow"				}
,	{	{	0,	0,	-10,	7,	0},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_BLIND		,	"Vampire's Bow"				}
,	{	{	10,	2,	0,		8,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Thor's Mace"				}
,	{	{	10,	2,	0,		8,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND		,	"Zeus' Staff"				}
,	{	{	10,	2,	0,		8,	0},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Anhur's Spear"				}
};

#endif // __WEAPON_H__928364890236498716349825347895236__