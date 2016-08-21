#include "GameBase.h"

#include <string>
#include <algorithm>

#ifndef __WEAPON_H__928364890236498716349825347895236__
#define __WEAPON_H__928364890236498716349825347895236__

namespace klib
{
	struct CWeapon
	{
		SCharacterPoints	Points;
		std::string			Name;
	};

	typedef CWeapon CWeaponModifier;

static const CWeaponModifier modifiersWeapon[] = 
//		MaxHP MaxMana HP Mana	Hit Attack Shield Coins
{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"%s"					}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"%s of Resistance"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	1,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"%s of the Thief"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BURN		},	"%s of Fire"			}	
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_POISON	},	"%s of Poison"			}
,	{	{	{	0,	0,	0	},	{	1,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"%s of Acolyte"			}
//																																										}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"%s of Vitality"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	2,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"%s of the Rogue"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	1,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BURN		},	"%s of Lava"			}	
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	1,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_POISON	},	"%s of Venom"			}
,	{	{	{	0,	0,	0	},	{	2,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"%s of the Priest"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_STEAL	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"Bonny and Clyde's %s"	}
//																																										}
,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"Soldier's %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	3,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"Assassin's %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_STUN		},	"Sonic %s"				}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING	},	"Gauss %s"				}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BURN		},	"Laser %s"				}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_POISON	},	"Acid %s"				}
,	{	{	{	0,	0,	0	},	{	3,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"Saint's %s"			}
//																																										}
,	{	{	{	4,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"Executioner's %s"		}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	4,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"Stalker's %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BURN		},	"Plasma %s"				}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_POISON	},	"Radiactive %s"			}
,	{	{	{	0,	0,	0	},	{	4,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"Angel's %s"			}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	0,	0,	},	0,	ATTACK_EFFECT_LEECH	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE		},	"Vampire's %s"			}
//
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BURN		},	"Satellite Orbital %s"	}
};

#define DEATH_SCYTHE_STATUS_INFLICT ((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLEEDING|klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_POISON|klib::COMBAT_STATUS_FROZEN|klib::COMBAT_STATUS_PETRIFY))

static const CWeapon definitionsWeapon[] = 
// 6th grade	No weapon: five points of hit chance.
//		MaxHP MaxMana HP Mana	Hit Attack Shield Coins
{	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	5,	0,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_NONE											},	"Glove"						}
// 5th grade	five points																																													}
,	{	{	{	3,	0,	0	},	{	0,	0,	0	},	{	0,	2,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_STUN											},	"Club"						}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	1,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLIND											},	"Knuckle"					}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	4,	1,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Dagger"					}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Spiked Club"				}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Knife"						}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	3,	2,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Claw"						}
// 4th grade	six																																															}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	1,	3,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_STUN											},	"Mace"						}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	2,	4,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Machete"					}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	4,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Sword"						}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	4,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Axe"						}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	-1,	6,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Spear"						}
,	{	{	{	-1,	0,	0	},	{	0,	0,	0	},	{	0,	7,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Bow"						}
// 3rd grade	seven																																														}
,	{	{	{	0,	0,	0	},	{	0,	0,	0	},	{	1,	6,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Pistol"					}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	5,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Rifle"						}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	-2,	7,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Shotgun"					}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	0,	5,	},	1,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Machinegun"				}
,	{	{	{	-2,	0,	0	},	{	0,	0,	0	},	{	2,	7,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	(COMBAT_STATUS)(COMBAT_STATUS_BLIND|COMBAT_STATUS_BLEEDING)	},	"Sniper Rifle"				}
,	{	{	{	2,	0,	0	},	{	0,	0,	0	},	{	0,	6,	},	-1,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	(COMBAT_STATUS)(COMBAT_STATUS_STUN|COMBAT_STATUS_BURN)		},	"Rocket Launcher"			}
,	{	{	{	0,	-3,	0	},	{	0,	0,	0	},	{	6,	6,	},	-2,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	(COMBAT_STATUS)(COMBAT_STATUS_STUN|COMBAT_STATUS_BURN)		},	"Guided Rocket Launcher"	}
// 2nd grade	eight																																														}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	-1,	7,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_STUN											},	"Thor's Mace"				}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	-1,	7,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLIND											},	"Zeus' Staff"				}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	-1,	7,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	COMBAT_STATUS_BLEEDING										},	"Anhur's Spear"				}
,	{	{	{	1,	0,	0	},	{	1,	0,	0	},	{	-1,	7,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	DEATH_SCYTHE_STATUS_INFLICT									},	"Death Scythe"				}
// 1st grade	ten																																															}
,	{	{	{	1,	0,	0	},	{	0,	0,	0	},	{	1,	8,	},	0,	ATTACK_EFFECT_NONE	,	DEFEND_EFFECT_NONE	,	PASSIVE_EFFECT_NONE	,	(COMBAT_STATUS)(COMBAT_STATUS_BURN|COMBAT_STATUS_POISON)	},	"BFG-9000"					}
};

	static std::string getWeaponName(const SWeapon& weapon)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, modifiersWeapon[weapon.Modifier].Name.c_str(), definitionsWeapon[weapon.Index].Name.c_str());
		return formattedName;
	}

	static SCharacterPoints getWeaponPoints(const SWeapon& weapon) {
		SCharacterPointsMultipliers	multipliers;

		multipliers.MaxLife		= {.1, .1, .1};
		multipliers.CurrentLife	= {.1, .1, .1};
		multipliers.Attack		= {.25, .5};
		multipliers.Coins		= 0.125;

		multipliers				= multipliers*weapon.Level;

		return (definitionsWeapon[weapon.Index].Points + modifiersWeapon[weapon.Modifier].Points)*multipliers;
	}
}

#endif // __WEAPON_H__928364890236498716349825347895236__