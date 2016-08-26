#include "EntityImpl.h"
#include "EntityHelper.h"

#ifndef __WEAPON_H__928364890236498716349825347895236__
#define __WEAPON_H__928364890236498716349825347895236__

namespace klib
{
#pragma pack(push, 1)

#define TECHNOLOGY_DIGITAL_MECHANIC		((klib::ENTITY_TECHNOLOGY)(klib::ENTITY_TECHNOLOGY_DIGITAL|klib::ENTITY_TECHNOLOGY_MECHANIC))
#define RADIACTIVE_STATUS_INFLICT		((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_POISON))
#define POLISHED_STATUS_INFLICT			((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLIND|klib::COMBAT_STATUS_BLEEDING))
#define VAMPIRE_STATUS_INFLICT			((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_SLEEP|klib::COMBAT_STATUS_BLEEDING))
#define VAMPIRE_FLAGS					{klib::ATTACK_EFFECT_LEECH_HEALTH},{VAMPIRE_STATUS_INFLICT}, {klib::ENTITY_TECHNOLOGY_BASIC}
#define THERMAL_STATUS					{klib::COMBAT_STATUS_NONE, klib::COMBAT_STATUS_FREEZING}	

//	{SEntityPoints, SEntityFlags, Name}
static const CRecordWeapon modifiersWeapon[] = 
//
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"%s"					}
,	{{{1,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of Resistance"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},1,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of the Thief"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{	POLISHED_STATUS_INFLICT	}	, {ENTITY_TECHNOLOGY_BASIC				}},	"Polished %s"			}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{	COMBAT_STATUS_BURN		}	, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of Fire"			}	
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{	COMBAT_STATUS_POISON	}	, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of Poison"			}
,	{{{0,0,0},{1,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of Acolyte"			}
//					0,																	  
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},	THERMAL_STATUS				, {ENTITY_TECHNOLOGY_BASIC				}},	"Thermal %s"			}
,	{{{2,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of Vitality"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},2,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of the Rogue"		}
,	{{{0,0,0},{0,0,0}	,{0,1,{0,0,0},0,{0,0,0}},0,0}	,{{},{	COMBAT_STATUS_BURN		}	, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of Lava"			}	
,	{{{0,0,0},{0,0,0}	,{0,1,{0,0,0},0,{0,0,0}},0,0}	,{{},{	COMBAT_STATUS_POISON	}	, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of Venom"			}
,	{{{0,0,0},{2,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"%s of the Priest"		}
,	{{{0,0,0},{0,0,0}	,{1,1,{0,0,0},0,{0,0,0}},0,0}	,{{ATTACK_EFFECT_STEAL	},{}		, {ENTITY_TECHNOLOGY_MECHANIC			}},	"Bonny and Clyde's %s"	}
//			0,		0,																	  
,	{{{3,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC	 			}},	"Soldier's %s"			}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},3,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC	 			}},	"Assassin's %s"			}
,	{{{0,0,0},{0,0,0}	,{0,2,{0,0,0},0,{0,0,0}},0,0}	,{{},{COMBAT_STATUS_STUN		}	, {TECHNOLOGY_DIGITAL_MECHANIC			}},	"Sonic %s"				}
,	{{{0,0,0},{0,0,0}	,{0,2,{0,0,0},0,{0,0,0}},0,0}	,{{},{COMBAT_STATUS_BLEEDING	}	, {TECHNOLOGY_DIGITAL_MECHANIC			}},	"Gauss %s"				}
,	{{{0,0,0},{0,0,0}	,{0,2,{0,0,0},0,{0,0,0}},0,0}	,{{},{COMBAT_STATUS_BURN		}	, {ENTITY_TECHNOLOGY_DIGITAL			}},	"Laser %s"				}
,	{{{0,0,0},{0,0,0}	,{0,2,{0,0,0},0,{0,0,0}},0,0}	,{{},{COMBAT_STATUS_POISON		}	, {ENTITY_TECHNOLOGY_BASIC				}},	"Acid %s"				}
,	{{{0,0,0},{3,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"Saint's %s"			}
//		  ,0	  ,0
,	{{{4,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"Executioner's %s"		}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},4,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"Stalker's %s"			}
,	{{{0,0,0},{0,0,0}	,{0,3,{0,0,0},0,{0,0,0}},0,0}	,{{},{COMBAT_STATUS_BURN}			, {ENTITY_TECHNOLOGY_DIGITAL			}},	"Plasma %s"				}
,	{{{0,0,0},{0,0,0}	,{0,3,{0,0,0},0,{0,0,0}},0,0}	,{{},{RADIACTIVE_STATUS_INFLICT}	, {ENTITY_TECHNOLOGY_DIGITAL			}},	"Radiactive %s"			}
,	{{{0,0,0},{4,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"Angel's %s"			}
,	{{{0,0,0},{0,4,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{{},{}								, {ENTITY_TECHNOLOGY_BASIC				}},	"Wizard's %s"			}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0,0}	,{VAMPIRE_FLAGS																},	"Vampire's %s"			}
//
,	{{{0,0,0},{0,0,0}	,{4,3,{0,0,0},0,{0,0,0}},0,0},	{	{},{	COMBAT_STATUS_BURN	}	, {ENTITY_TECHNOLOGY_DIGITAL			}},	"Satellite Orbital %s"	}
};

#define DEATH_SCYTHE_STATUS_INFLICT		((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLEEDING|klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_POISON|klib::COMBAT_STATUS_FROZEN|klib::COMBAT_STATUS_PETRIFY))
#define ROCKET_LAUNCHER_STATUS_INFLICT	((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_STUN|klib::COMBAT_STATUS_BURN))
#define SNIPER_RIFLE_STATUS_INFLICT		((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BLIND|klib::COMBAT_STATUS_BLEEDING))
#define BFG9000_STATUS_INFLICT			((klib::COMBAT_STATUS)(klib::COMBAT_STATUS_BURN|klib::COMBAT_STATUS_POISON))

//	{SEntityPoints, SEntityFlags, Name}
static const CRecordWeapon definitionsWeapon[] = 
// 6th grade	No weapon: five points of hit chance.
{	{{{0,0,0},{0,0,0}	,{5,0, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_NONE				},	{ENTITY_TECHNOLOGY_BASIC		}},	"Glove"						}
// 5th grade five points
,	{{{0,0,0},{0,0,0}	,{3,2, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Rod"						}
,	{{{3,0,0},{0,0,0}	,{0,2, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_STUN				},	{ENTITY_TECHNOLOGY_BASIC		}},	"Club"						}
,	{{{0,0,0},{0,0,0}	,{4,1, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Dagger"					}
,	{{{0,0,0},{0,0,0}	,{4,1, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_SLEEP				},	{ENTITY_TECHNOLOGY_BASIC		}},	"Knuckle"					}
,	{{{1,0,0},{0,0,0}	,{1,3, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Spiked Club"				}
,	{{{0,0,0},{0,0,0}	,{2,3, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Knife"						}
,	{{{0,0,0},{0,0,0}	,{3,2, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Claw"						}
// 4th grade six	
,	{{{0,0,0},{0,0,0}	,{3,2, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLIND				},	{ENTITY_TECHNOLOGY_BASIC		}},	"Wand"						}
,	{{{2,0,0},{0,0,0}	,{1,3, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_STUN				},	{ENTITY_TECHNOLOGY_BASIC		}},	"Mace"						}
,	{{{0,0,0},{0,0,0}	,{2,4, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Machete"					}
,	{{{0,0,0},{0,0,0}	,{3,2, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLIND				},	{ENTITY_TECHNOLOGY_BASIC		}},	"Staff"						}
,	{{{1,0,0},{0,0,0}	,{1,4, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Sword"						}
,	{{{2,0,0},{0,0,0}	,{0,4, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Axe"						}
,	{{{1,0,0},{0,0,0}	,{-1,6,{0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Spear"						}
,	{{{-1,0,0},{0,0,0}	,{0,7, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Bow"						}
// 3rd grade seven		
,	{{{0,0,0},{0,0,0}	,{2,5, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_MECHANIC		}},	"Crossbow"					}
,	{{{0,0,0},{0,0,0}	,{1,6, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_MECHANIC		}},	"Pistol"					}
,	{{{1,0,0},{0,0,0}	,{1,5, {0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_MECHANIC		}},	"Rifle"						}
,	{{{2,0,0},{0,0,0}	,{-2,7,{0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_MECHANIC		}},	"Shotgun"					}
,	{{{1,0,0},{0,0,0}	,{0,5, {0,0,0},0,{0,0,0}},1,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_MECHANIC		}},	"Machinegun"				}
,	{{{-2,0,0},{0,0,0}	,{2,7, {0,0,0},0,{0,0,0}},0,0	},{{},{SNIPER_RIFLE_STATUS_INFLICT		},	{ENTITY_TECHNOLOGY_MECHANIC		}},	"Sniper Rifle"				}
,	{{{2,0,0},{0,0,0}	,{0,6, {0,0,0},0,{0,0,0}},-1,0	},{{},{ROCKET_LAUNCHER_STATUS_INFLICT	},	{ENTITY_TECHNOLOGY_MECHANIC		}},	"Rocket Launcher"			}
,	{{{0,-3,0},{0,0,0}	,{6,6, {0,0,0},0,{0,0,0}},-2,0	},{{},{ROCKET_LAUNCHER_STATUS_INFLICT	},	{TECHNOLOGY_DIGITAL_MECHANIC	}},	"Guided Rocket Launcher"	}
// 2nd grade eight		,
,	{{{1,0,0},{1,0,0}	,{-1,7,{0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_STUN				},	{ENTITY_TECHNOLOGY_BASIC		}},	"Thor's Mace"				}
,	{{{1,0,0},{1,0,0}	,{-1,7,{0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLIND				},	{ENTITY_TECHNOLOGY_BASIC		}},	"Zeus' Staff"				}
,	{{{1,0,0},{1,0,0}	,{-1,7,{0,0,0},0,{0,0,0}},0,0	},{{},{COMBAT_STATUS_BLEEDING			},	{ENTITY_TECHNOLOGY_BASIC		}},	"Anhur's Spear"				}
,	{{{1,0,0},{1,0,0}	,{-1,7,{0,0,0},0,{0,0,0}},0,0	},{{},{DEATH_SCYTHE_STATUS_INFLICT		},	{ENTITY_TECHNOLOGY_BASIC		}},	"Death Scythe"				}
// 1st grade ten		,
,	{{{1,0,0},{0,0,0}	,{1,8, {0,0,0},0,{0,0,0}},0,0	},{{},{BFG9000_STATUS_INFLICT			},	{ENTITY_TECHNOLOGY_DIGITAL		}},	"BFG-9000"					}
};

	static std::string getWeaponName(const SWeapon& weapon) {
		return getEntityName(weapon, definitionsWeapon, modifiersWeapon);
	}

	static SEntityPoints getWeaponPoints(const SWeapon& weapon) {
			return getEntityPoints(weapon, definitionsWeapon, modifiersWeapon, weapon.getMultipliers());
	}

	static SEntityFlags getWeaponFlags(const SWeapon& weapon) {
		return  getEntityFlags(weapon, definitionsWeapon, modifiersWeapon);
	}

#pragma pack(pop)
}

#endif // __WEAPON_H__928364890236498716349825347895236__