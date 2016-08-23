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
{	{{{0,0,0},{0,0,0},{0,0},0,{},{}								, EQUIP_TECHNOLOGY_BASIC												},	"%s"					}
,	{{{1,0,0},{0,0,0},{0,0},0,{},{}								, EQUIP_TECHNOLOGY_BASIC												},	"%s of Resistance"		}
,	{{{0,0,0},{0,0,0},{0,0},1,{},{}								, EQUIP_TECHNOLOGY_BASIC												},	"%s of the Thief"		}
,	{{{0,0,0},{0,0,0},{0,0},0,{},{	COMBAT_STATUS_BURN		}	, EQUIP_TECHNOLOGY_BASIC												},	"%s of Fire"			}	
,	{{{0,0,0},{0,0,0},{0,0},0,{},{	COMBAT_STATUS_POISON	}	, EQUIP_TECHNOLOGY_BASIC												},	"%s of Poison"			}
,	{{{0,0,0},{1,0,0},{0,0},0,{},{}								, EQUIP_TECHNOLOGY_BASIC												},	"%s of Acolyte"			}
//
,	{{{2,0,0},{0,0,0},{0,0},0,{},{}								, EQUIP_TECHNOLOGY_BASIC												},	"%s of Vitality"		}
,	{{{0,0,0},{0,0,0},{0,0},2,{},{}								, EQUIP_TECHNOLOGY_BASIC												},	"%s of the Rogue"		}
,	{{{0,0,0},{0,0,0},{0,1},0,{},{	COMBAT_STATUS_BURN		}	, EQUIP_TECHNOLOGY_BASIC												},	"%s of Lava"			}	
,	{{{0,0,0},{0,0,0},{0,1},0,{},{	COMBAT_STATUS_POISON	}	, EQUIP_TECHNOLOGY_BASIC												},	"%s of Venom"			}
,	{{{0,0,0},{2,0,0},{0,0},0,{},{}								, EQUIP_TECHNOLOGY_BASIC												},	"%s of the Priest"		}
,	{{{0,0,0},{0,0,0},{0,0},0,{ATTACK_EFFECT_STEAL	},{}		, EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_MECHANIC)							},	"Bonny and Clyde's %s"	}
//
,	{{{3,0,0},{0,0,0},{0,0},0,{},{}	, EQUIP_TECHNOLOGY_BASIC																			},	"Soldier's %s"			}
,	{{{0,0,0},{0,0,0},{0,0},3,{},{}	, EQUIP_TECHNOLOGY_BASIC																			},	"Assassin's %s"			}
,	{{{0,0,0},{0,0,0},{0,3},0,{},{COMBAT_STATUS_STUN		}	, EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_MECHANIC)	},	"Sonic %s"				}
,	{{{0,0,0},{0,0,0},{0,3},0,{},{COMBAT_STATUS_BLEEDING	}	, EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_MECHANIC)	},	"Gauss %s"				}
,	{{{0,0,0},{0,0,0},{0,3},0,{},{COMBAT_STATUS_BURN		}	, EQUIP_TECHNOLOGY_DIGITAL												},	"Laser %s"				}
,	{{{0,0,0},{0,0,0},{0,3},0,{},{COMBAT_STATUS_POISON		}	, EQUIP_TECHNOLOGY_BASIC												},	"Acid %s"				}
,	{{{0,0,0},{3,0,0},{0,0},0,{},{}	, EQUIP_TECHNOLOGY_BASIC																			},	"Saint's %s"			}
//
,	{{{4,0,0},{0,0,0},{0,0},0,{},{}	, EQUIP_TECHNOLOGY_BASIC																			},	"Executioner's %s"		}
,	{{{0,0,0},{0,0,0},{0,0},4,{},{}	, EQUIP_TECHNOLOGY_BASIC																			},	"Stalker's %s"			}
,	{{{0,0,0},{0,0,0},{0,3},0,{},{COMBAT_STATUS_BURN}			, EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_DIGITAL)							},	"Plasma %s"				}
,	{{{0,0,0},{0,0,0},{0,3},0,{},{(COMBAT_STATUS)(COMBAT_STATUS_BURN|COMBAT_STATUS_POISON)}, EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_DIGITAL)},	"Radiactive %s"			}
,	{{{0,0,0},{4,0,0},{0,0},0,{},{}	, EQUIP_TECHNOLOGY_BASIC																			},	"Angel's %s"			}
,	{{{0,0,0},{0,4,0},{0,0},0,{},{}	, EQUIP_TECHNOLOGY_BASIC																			},	"Wizard's %s"			}
,	{{{0,0,0},{0,0,0},{0,0},0,{	ATTACK_EFFECT_LEECH_HEALTH	},{COMBAT_STATUS_BLEEDING}, EQUIP_TECHNOLOGY_BASIC							},	"Vampire's %s"			}
//
,	{{{0,0,0},{0,0,0},{4,3,},0	,{},{	COMBAT_STATUS_BURN		}	, EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_DIGITAL)						},	"Satellite Orbital %s"	}
};

#define DEATH_SCYTHE_STATUS_INFLICT ((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLEEDING|klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_POISON|klib::COMBAT_STATUS_FROZEN|klib::COMBAT_STATUS_PETRIFY))

static const CWeapon definitionsWeapon[] = 
// 6th grade	No weapon: five points of hit chance.
//		MaxHP MaxMana HP Mana	Hit Attack Shield Coins
{	{{{0,0,0},{0,0,0},{5,0},0,{},{COMBAT_STATUS_NONE			},	EQUIP_TECHNOLOGY_BASIC		},	"Glove"						}
// 5th grade five points
,	{{{3},{},{0,2},0,{},{COMBAT_STATUS_STUN			},	EQUIP_TECHNOLOGY_BASIC		},	"Club"						}
,	{{{},{},{4,1},0,{},{COMBAT_STATUS_BLIND			},	EQUIP_TECHNOLOGY_BASIC		},	"Knuckle"					}
,	{{{},{},{4,1},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Dagger"					}
,	{{{1},{},{1,3},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Spiked Club"				}
,	{{{},{},{2,3},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Knife"						}
,	{{{},{},{3,2},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Claw"						}
// 4th grade six	
,	{{{2},{0,0,0},{1,3},0,{},{COMBAT_STATUS_STUN			},	EQUIP_TECHNOLOGY_BASIC		},	"Mace"						}
,	{{{},{},{2,4},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Machete"					}
,	{{{1,0,0},{0,0,0},{1,4},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Sword"						}
,	{{{2,0,0},{0,0,0},{0,4},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Axe"						}
,	{{{1,0,0},{0,0,0},{-1,6},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Spear"						}
,	{{{-1,0,0},{0,0,0},{0,7},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Bow"						}
// 3rd grade seven
,	{{{},{},{2,5},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_MECHANIC)},	"Crossbow"					}
,	{{{},{},{1,6},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_MECHANIC)},	"Pistol"					}
,	{{{1},{},{1,5},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_MECHANIC)},	"Rifle"						}
,	{{{2},{},{-2,7},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_MECHANIC)},	"Shotgun"					}
,	{{{1},{},{0,5},1,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_MECHANIC)},	"Machinegun"				}
,	{{{-2},{},{2,7},0,{},{(COMBAT_STATUS)(COMBAT_STATUS_BLIND|COMBAT_STATUS_BLEEDING)	},	EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_MECHANIC)								},	"Sniper Rifle"				}
,	{{{2},{},{0,6},-1,{},{(COMBAT_STATUS)(COMBAT_STATUS_STUN|COMBAT_STATUS_BURN)		},	EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_MECHANIC)								},	"Rocket Launcher"			}
,	{{{0,-30},{},{6,6},-2,{},{(COMBAT_STATUS)(COMBAT_STATUS_STUN|COMBAT_STATUS_BURN)		},	EQUIP_TECHNOLOGY(EQUIP_TECHNOLOGY_DIGITAL|EQUIP_TECHNOLOGY_MECHANIC)	},	"Guided Rocket Launcher"	}
// 2nd grade eight
,	{{{1},{1},{-1,7},0,{},{COMBAT_STATUS_STUN			},	EQUIP_TECHNOLOGY_BASIC		},	"Thor's Mace"	}
,	{{{1},{1},{-1,7},0,{},{COMBAT_STATUS_BLIND			},	EQUIP_TECHNOLOGY_BASIC		},	"Zeus' Staff"	}
,	{{{1},{1},{-1,7},0,{},{COMBAT_STATUS_BLEEDING		},	EQUIP_TECHNOLOGY_BASIC		},	"Anhur's Spear"	}
,	{{{1},{1},{-1,7},0,{},{DEATH_SCYTHE_STATUS_INFLICT	},	EQUIP_TECHNOLOGY_BASIC		},	"Death Scythe"	}
// 1st grade ten
,	{{{1,0,0},{0,0,0},{1,8},0,{},{(COMBAT_STATUS)(COMBAT_STATUS_BURN|COMBAT_STATUS_POISON)},EQUIP_TECHNOLOGY_DIGITAL},	"BFG-9000"	}
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