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
	SCharacterPoints weaponPoints = {};
	weaponPoints.MaxLife.HP			= (int32_t)(	weaponDefinitions[weapon.Index].Points.MaxLife.HP			*	std::max(1.0000001, weapon.Level*0.1	)	+	weaponModifiers[weapon.Modifier].Points.MaxLife.HP			*	std::max(1.0000001, weapon.Level*0.1	)	);
	weaponPoints.MaxLife.Mana		= (int32_t)(	weaponDefinitions[weapon.Index].Points.MaxLife.Mana			*	std::max(1.0000001, weapon.Level*0.1	)	+	weaponModifiers[weapon.Modifier].Points.MaxLife.Mana		*	std::max(1.0000001, weapon.Level*0.1	)	);
	weaponPoints.MaxLife.Shield		= (int32_t)(	weaponDefinitions[weapon.Index].Points.MaxLife.Shield		*	std::max(1.0000001, weapon.Level*0.1	)	+	weaponModifiers[weapon.Modifier].Points.MaxLife.Shield		*	std::max(1.0000001, weapon.Level*0.1	)	);
	
	weaponPoints.CurrentLife.HP		= (int32_t)(	weaponDefinitions[weapon.Index].Points.CurrentLife.HP		*	std::max(1.0000001, weapon.Level*0.1	)	+	weaponModifiers[weapon.Modifier].Points.CurrentLife.HP		*	std::max(1.0000001, weapon.Level*0.1	)	);
	weaponPoints.CurrentLife.Mana	= (int32_t)(	weaponDefinitions[weapon.Index].Points.CurrentLife.Mana		*	std::max(1.0000001, weapon.Level*0.1	)	+	weaponModifiers[weapon.Modifier].Points.CurrentLife.Mana	*	std::max(1.0000001, weapon.Level*0.1	)	);
	weaponPoints.CurrentLife.Shield	= (int32_t)(	weaponDefinitions[weapon.Index].Points.CurrentLife.Shield	*	std::max(1.0000001, weapon.Level*0.1	)	+	weaponModifiers[weapon.Modifier].Points.CurrentLife.Shield	*	std::max(1.0000001, weapon.Level*0.1	)	);
	
	weaponPoints.Attack.Damage		= (int32_t)(	weaponDefinitions[weapon.Index].Points.Attack.Damage		*	std::max(1.0000001, weapon.Level*0.5	)	+	weaponModifiers[weapon.Modifier].Points.Attack.Damage		*	std::max(1.0000001, weapon.Level*0.5	)	);
	weaponPoints.Attack.Hit			= (int32_t)(	weaponDefinitions[weapon.Index].Points.Attack.Hit			*	std::max(1.0000001, weapon.Level*0.25	)	+	weaponModifiers[weapon.Modifier].Points.Attack.Hit			*	std::max(1.0000001, weapon.Level*0.25	)	);

	weaponPoints.Coins				= (int32_t)(	weaponDefinitions[weapon.Index].Points.Coins				*	std::max(1.0000001, weapon.Level*0.125	)	+	weaponModifiers[weapon.Modifier].Points.Coins				*	std::max(1.0000001, weapon.Level*0.125	)	);
	return weaponPoints;
}

#endif // __WEAPON_H__928364890236498716349825347895236__