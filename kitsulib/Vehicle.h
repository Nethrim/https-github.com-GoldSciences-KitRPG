#include "GameBase.h"
#include <string>

#ifndef __VEHICLES_H__91236982714698236423423__
#define __VEHICLES_H__91236982714698236423423__

namespace klib
{
	struct CVehicle
	{
		SCharacterPoints	Points;
		std::string			Name;
	};

	typedef CVehicle CVehicleModifier;

//	{HP, Mana, Shield}, {HP, Mana, Shield}, {Hit, Damage, Speed, Absorption}, dropCoins, {Attack, Defend, Passive}, {Status Inflict, Status Immunity}, Name.
static const CVehicleModifier modifiersVehicle[] = 
{	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_BASIC, ENTITY_GRADE_LIGHT	}}},	"%s"					}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_BASIC, ENTITY_GRADE_LIGHT	}}},	"Personal %s"			}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_BASIC, ENTITY_GRADE_LIGHT	}}},	"Lightweight %s"		}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_BASIC, ENTITY_GRADE_LIGHT	}}},	"Cargo %s"				}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_BASIC, ENTITY_GRADE_LIGHT	}}},	"Raid %s"				}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_BASIC, ENTITY_GRADE_LIGHT	}}},	"Military %s"			}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_BASIC, ENTITY_GRADE_LIGHT	}}},	"Combat %s"				}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_BASIC, ENTITY_GRADE_LIGHT	}}},	"Armored %s"			}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_DIGITAL, ENTITY_GRADE_LIGHT	}}},	"Supersonic %s"			}
,	{{{{},{},{},0},{{},{},	{ENTITY_TECHNOLOGY_DIGITAL, ENTITY_GRADE_LIGHT	}}},	"Space %s"				}
};

//	{HP, Mana, Shield}, {HP, Mana, Shield}, {Hit, Damage, Speed, Absorption}, dropCoins, {Attack, Defend, Passive}, {Status Inflict, Status Immunity}, Name.
static const CVehicle definitionsVehicle[] = 	
{	{{{{0,0,0},	{},{1,0,0,0},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}}},	"Rollers"				}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}}},	"Ship"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}}},	"Carriage"				}
,	{{{{0,0,1},	{},{-1,0,0,0},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}}},	"Bike"					}
,	{{{{0,0,1},	{},{-2,0,0,0},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}}},	"Motorbike"				}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}}},	"Car"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}}},	"Van"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}}},	"Bus"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}}},	"4x4"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}}},	"Jeep"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}}},	"Truck"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}}},	"Train"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}}},	"Plane"					}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}}},	"Hovercraft"			}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_HEAVY	}}},	"Aircraft"				}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}}},	"Helicopter"			}
,	{{{{0,0,1},	{},{0,0,0,1},0	},	{{},{}	,	{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_MEDIUM	}}},	"Jet"					}
};

	static std::string getVehicleName(const SVehicle& armor)
	{
		char formattedName[128] = {};
		sprintf_s(formattedName, modifiersVehicle[armor.Modifier].Name.c_str(), definitionsVehicle[armor.Index].Name.c_str());
		return formattedName;
	}

	static inline constexpr int32_t getVehicleAbsorption(const SVehicle& armor) {
		return	(int32_t)((definitionsVehicle[armor.Index].Points.Points.Attack.Absorption + modifiersVehicle[armor.Modifier].Points.Points.Attack.Absorption) * armor.Level);
	}

	static SCharacterPoints getVehiclePoints(const SVehicle& armor) {
		SEntityPointsMultiplier multipliers;

		multipliers.LifeMax		= {.1,  .1, .25		};
		multipliers.LifeCurrent	= {.1,  .1, .1		};
		multipliers.Attack		= {.15, .1, .15, .1	};
		multipliers.Coins		= 0.125;

		multipliers				= multipliers*armor.Level;

		return (definitionsVehicle[armor.Index].Points + modifiersVehicle[armor.Modifier].Points)*multipliers;
	}}

#endif // __VEHICLES_H__91236982714698236423423__
