#include "GameBase.h"

#include <string>
#include <algorithm>

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
//		MaxHP MaxMana HP Mana	Hit Attack Shield Coins
{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"%s"					}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"%s of Resistance"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	1	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"%s of the Thief"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BURN		,	"%s of Fire"			}	
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_POISON		,	"%s of Poison"			}
,	{	{	{	0,	0,	0	},	{	1,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"%s of Acolyte"			}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"%s of Vitality"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	2	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"%s of the Rogue"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	1,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BURN		,	"%s of Lava"			}	
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	1,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_POISON		,	"%s of Venom"			}
,	{	{	{	0,	0,	0	},	{	2,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"%s of the Priest"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_STEAL	,	STATUS_TYPE_NONE		,	"Bonny and Clyde's %s"	}
,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Soldier's %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	3	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Assassin's %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN		,	"Sonic %s"				}	
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING	,	"Gauss %s"				}	
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BURN		,	"Laser %s"				}	
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_POISON		,	"Acid %s"				}
,	{	{	{	0,	0,	0	},	{	3,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Saint's %s"			}
,	{	{	{	4,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Executioner's %s"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	4	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Stalker's %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BURN		,	"Plasma %s"				}	
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_POISON		,	"Radiactive %s"			}	
,	{	{	{	0,	0,	0	},	{	4,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE		,	"Angel's %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0	},	WEAPON_EFFECT_LEECH	,	STATUS_TYPE_NONE		,	"Vampire's %s"			}
};

static const CWeapon weaponDefinitions[] = 
// 6th grade	No weapon: five points of hit chance.
//		MaxHP MaxMana HP Mana	Hit Attack Shield Coins
{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	5,	0,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_NONE										,	"Glove"						}
// 5th grade	five points
,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	2,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Club"						}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	1,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND										,	"Knuckle"					}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	1,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Dagger"					}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Spiked Club"				}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Knife"						}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	3,	2,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Claw"						}
// 4th grade	six
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Machete"					}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	1,	3,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Mace"						}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	4,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Sword"						}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Axe"						}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	-1,	6,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Spear"						}
,	{	{	{	-1,	0,	0	},	{	0,	0,	0	},	{	0,	7,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND										,	"Bow"						}
// 3rd grade	seven
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	1,	6,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Pistol"					}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	5,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Rifle"						}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	5,	},	1	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Machinegun"				}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	-2,	7,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Shotgun"					}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	6,	},	-2	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Rocket Launcher"			}
,	{	{	{	-2,	0,	0	},	{	0,	0,	0	},	{	6,	6,	},	-3	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Guided Rocket Launcher"	}
,	{	{	{	-2,	0,	0	},	{	0,	0,	0	},	{	2,	7,	},	0	},	WEAPON_EFFECT_NONE	,	(STATUS_TYPE)(STATUS_TYPE_BLIND|STATUS_TYPE_BLEEDING)	,	"Sniper Rifle"				}
// 2nd grade	eight
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	-1,	7,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_STUN										,	"Thor's Mace"				}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	-1,	7,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLIND										,	"Zeus' Staff"				}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	-1,	7,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Anhur's Spear"				}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	-1,	7,	},	0	},	WEAPON_EFFECT_NONE	,	STATUS_TYPE_BLEEDING									,	"Death Scythe"				}
// 1st grade	ten
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	8,	},	0	},	WEAPON_EFFECT_NONE	,	(STATUS_TYPE)(STATUS_TYPE_BURN|STATUS_TYPE_POISON)		,	"BFG-9000"					}
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
	SCharacterPointsMultipliers	multipliers;

	multipliers.MaxLife		= {.1, .1, .1};
	multipliers.CurrentLife	= {.1, .1, .1};
	multipliers.Attack		= {.25, .5};
	multipliers.Coins		= 0.125;

	return (weaponDefinitions[weapon.Index].Points + weaponModifiers[weapon.Modifier].Points)*multipliers;
}

#endif // __WEAPON_H__928364890236498716349825347895236__