#include "GameBase.h"

#include <string>

#ifndef __WEAPON_H__928364890236498716349825347895236__
#define __WEAPON_H__928364890236498716349825347895236__

enum WEAPON_EFFECT
{	WEAPON_EFFECT_NONE
,	WEAPON_EFFECT_LEECH	= 0x01
,	WEAPON_EFFECT_STEAL	= 0x02
};

struct CWeapon
{
	SCharacterPoints	Points;
	WEAPON_EFFECT		Effect;
	STATUS_TYPE			Status;
	std::string			Name;
};

typedef CWeapon CWeaponModifier;

static const CWeaponModifier weaponModifiers[] = 
{	{	{	0,	0,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"%s"					}
,	{	{	1,	0,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"%s of Resistance"		}
,	{	{	0,	0,	0,		0,	1	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"%s of the Thief"		}
,	{	{	0,	0,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BURN	,	"%s of Fire"			}	
,	{	{	0,	0,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_POISON	,	"%s of Poison"			}
,	{	{	0,	1,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"%s of Acolyte"			}
,	{	{	2,	0,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"%s of Vitality"		}
,	{	{	0,	0,	0,		0,	2	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"%s of the Rogue"		}
,	{	{	0,	0,	0,		1,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BURN	,	"%s of Lava"			}	
,	{	{	0,	0,	0,		1,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_POISON	,	"%s of Venom"			}
,	{	{	0,	2,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"%s of the Priest"		}
,	{	{	0,	0,	0,		0,	0	},	WEAPON_EFFECT_STEAL	,	STATUS_TYPE_NONE	,	"Bonny and Clyde's %s"	}
,	{	{	3,	0,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"Soldier's %s"			}
,	{	{	0,	0,	0,		0,	3	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"Assassin's %s"			}
,	{	{	0,	0,	0,		3,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BURN	,	"Laser %s"				}	
,	{	{	0,	0,	0,		2,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_POISON	,	"Acid %s"				}
,	{	{	0,	3,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"Saint's %s"			}
,	{	{	4,	0,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"Executioner's %s"		}
,	{	{	0,	0,	0,		0,	4	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"Stalker's %s"			}
,	{	{	0,	0,	0,		2,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BURN	,	"Plasma %s"				}	
,	{	{	0,	0,	0,		3,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_POISON	,	"Radiactive %s"			}	
,	{	{	0,	4,	0,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE	,	"Angel's %s"			}
,	{	{	0,	0,	0,		0,	0	},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_NONE	,	"Vampire's %s"			}
};

static const CWeapon weaponDefinitions[] = 
{	{	{	0,	0,	10,		0,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE										,	"Glove"							}
,	{	{	0,	0,	0,		1,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Club"							}
,	{	{	0,	0,	10,		1,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND										,	"Knuckle"						}
,	{	{	0,	0,	0,		2,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Spiked Club"					}
,	{	{	0,	0,	10,		2,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Knife"							}
,	{	{	0,	0,	15,		2,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Claw"							}
,	{	{	0,	0,	5,		3,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Machete"						}
,	{	{	0,	0,	5,		3,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Mace"							}
,	{	{	0,	0,	5,		4,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Sword"							}
,	{	{	0,	0,	0,		5,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Axe"							}
,	{	{	0,	0,	-5,		6,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Spear"							}
,	{	{	0,	0,	-10,	7,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND										,	"Bow"							}
,	{	{	0,	0,	10,		8,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Pistol"						}
,	{	{	0,	0,	0,		9,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Machinegun"					}
,	{	{	0,	0,	-5,		10,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Shotgun"						}
,	{	{	0,	0,	30,		11,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Rocket Launcher"				}
,	{	{	0,	0,	15,		12,	0	},	WEAPON_EFFECT_NONE	,	(STATUS_TYPE)(STATUS_TYPE_BLIND|STATUS_TYPE_BLEEDING)	,	"Sniper Rifle"					}
,	{	{	10,	2,	0,		25,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Thor's Mace"					}
,	{	{	10,	2,	0,		25,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND										,	"Zeus' Staff"					}
,	{	{	10,	2,	0,		25,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Anhur's Spear"					}
,	{	{	20,	2,	10,		50,	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Death Scythe"					}
};

static std::string getWeaponName(const SWeapon& weapon)
{
	char formattedName[128] = {};
	sprintf_s(formattedName, weaponModifiers[weapon.Modifier].Name.c_str(), weaponDefinitions[weapon.Index].Name.c_str());
	return formattedName;
}

static inline constexpr WEAPON_EFFECT getWeaponEffect(const SWeapon& weapon) {
	return WEAPON_EFFECT(weaponDefinitions[weapon.Index].Effect | weaponModifiers[weapon.Modifier].Effect);
}

static inline constexpr STATUS_TYPE getWeaponStatus(const SWeapon& weapon) {
	return STATUS_TYPE(weaponDefinitions[weapon.Index].Status | weaponModifiers[weapon.Modifier].Status);
}

static SCharacterPoints getWeaponPoints(const SWeapon& weapon) {
	SCharacterPoints weaponPoints = {};
	weaponPoints.MaxHP	= weaponDefinitions[weapon.Index].Points.MaxHP	+	weaponModifiers[weapon.Modifier].Points.MaxHP;
	weaponPoints.Attack	= weaponDefinitions[weapon.Index].Points.Attack	+	weaponModifiers[weapon.Modifier].Points.Attack;
	weaponPoints.Hit	= weaponDefinitions[weapon.Index].Points.Hit	+	weaponModifiers[weapon.Modifier].Points.Hit;
	weaponPoints.HP		= weaponDefinitions[weapon.Index].Points.HP		+	weaponModifiers[weapon.Modifier].Points.HP;
	weaponPoints.Coins	= weaponDefinitions[weapon.Index].Points.Coins	+	weaponModifiers[weapon.Modifier].Points.Coins;
	return weaponPoints;
}

#endif // __WEAPON_H__928364890236498716349825347895236__