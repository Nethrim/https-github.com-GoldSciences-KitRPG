#include "EntityImpl.h"
#include "EntityHelper.h"

#ifndef __VEHICLES_H__91236982714698236423423__
#define __VEHICLES_H__91236982714698236423423__

namespace klib
{
#pragma pack(push, 1)

//	{SEntityPoints, SEntityFlags, Name}
static const CRecordVehicle modifiersVehicle[] = 
{	{{{0,0,0},{0,0,0}	,{0,0,{0,0,0},0,{0,0,0}},0, 000,  0}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"%s"					}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,1,0},0,{0,0,0}},0, 100,  1}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Personal %s"			}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,2,0},0,{0,0,0}},0, 200,  2}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Scout %s"				}
,	{{{0,0,5},{0,0,0}	,{0,0,{0,0,0},5,{0,0,0}},2, 300,  3}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}},	"Cargo %s"				}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,3,0},0,{0,0,0}},1, 400,  4}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Raid %s"				}
,	{{{0,0,3},{0,0,1}	,{2,5,{0,4,0},4,{0,0,0}},0, 500,  5}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Combat %s"				}
//--------------------------------------- Upgrade	 	   
,	{{{0,0,5},{0,0,0}	,{1,4,{0,5,0},5,{0,0,0}},0, 600,  6}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Military %s"			}
,	{{{0,0,7},{0,0,1}	,{0,0,{0,0,0},7,{0,0,0}},1, 700,  7}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}},	"Armored %s"			}
,	{{{0,0,2},{0,0,0}	,{0,4,{0,5,2},2,{0,0,0}},0, 800,  8}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_LIGHT	}},	"Nuclear %s"			}
,	{{{0,1,0},{0,1,0}	,{0,4,{0,7,0},1,{1,0,0}},0, 900,  9}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_LIGHT	}},	"Supersonic %s"			}
// -------------------------------------- Update 2
,	{{{2,1,4},{2,2,2}	,{5,7,{0,9,0},4,{1,1,1}},2,1000, 10}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_LIGHT	}},	"Warp %s"				}
}; //											   
//												   
//	{SEntityPoints, SEntityFlags, Name}			   
// Not sure how to deal with the fucntions		   
static const CRecordVehicle definitionsVehicle[] = 
{	{{{0,0,0},{0,0,0}	,{0,0,{0,1,0},0,{0,0,0}},0,10	, 1}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Socks"					}
,	{{{0,0,1},{0,0,0}	,{0,0,{0,1,0},0,{0,0,0}},0,50	, 2}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Boat"					}
,	{{{0,0,1},{0,0,0}	,{0,1,{0,2,0},0,{0,0,0}},0,100	, 2}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Carriage"				}
,	{{{0,0,2},{0,0,0}	,{0,2,{0,1,0},0,{0,0,0}},0,150	, 3}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Ship"					}
,	{{{0,0,0},{0,0,0}	,{0,2,{0,2,0},0,{0,0,0}},0,200	, 2}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Bike"					}
//--------------------------------------- Upgrade		, 1
,	{{{0,0,0},{0,0,0}	,{0,3,{0,5,0},0,{0,0,0}},0,200	, 3}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"Motorbike"				}
,	{{{0,0,3},{0,0,0}	,{0,0,{0,5,0},0,{0,0,0}},0,300	, 3}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Car"					}
,	{{{0,0,4},{0,0,0}	,{0,0,{0,4,0},0,{0,0,0}},0,300	, 3}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Van"					}
,	{{{0,0,4},{0,0,0}	,{0,0,{0,3,0},0,{0,0,0}},0,400	, 4}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}},	"Bus"					}
,	{{{0,0,3},{0,0,0}	,{0,3,{0,4,0},0,{0,0,0}},0,400	, 4}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"4x4"					}
,	{{{0,0,3},{0,0,0}	,{0,0,{0,4,0},0,{0,0,0}},0,500	, 4}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Jeep"					}
,	{{{0,0,4},{0,0,0}	,{0,5,{0,3,0},0,{0,0,0}},0,500	, 4}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}},	"Truck"					}
,	{{{0,0,0},{0,0,0}	,{0,0,{0,3,0},0,{0,0,0}},0,600	, 4}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_LIGHT	}},	"ATP"					}
,	{{{0,0,6},{0,0,0}	,{0,5,{0,5,0},0,{0,0,0}},0,600	, 6}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_HEAVY	}},	"Train"					}
,	{{{0,0,5},{0,0,0}	,{0,3,{0,6,0},0,{0,0,0}},0,700	, 5}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Plane"					}
,	{{{0,0,1},{0,0,0}	,{0,3,{0,5,0},0,{0,0,0}},0,700	, 5}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Hovercraft"			}
,	{{{0,0,4},{0,0,0}	,{0,5,{0,7,0},0,{0,0,0}},0,800	, 6}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_HEAVY	}},	"Aircraft"				}
,	{{{0,0,6},{0,0,0}	,{0,4,{0,0,0},0,{0,0,0}},0,800	, 8}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_HEAVY	}},	"Aircraft Carrier"		}
,	{{{0,0,6},{0,0,0}	,{0,5,{0,1,0},0,{0,0,0}},0,900	, 7}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_HEAVY	}},	"Submarine"				}
,	{{{0,0,2},{0,0,0}	,{0,2,{0,4,0},0,{0,0,0}},0,900	, 6}	,{{},{}	,{ENTITY_TECHNOLOGY_BASIC	, ENTITY_GRADE_MEDIUM	}},	"Helicopter"			}
,	{{{0,0,4},{0,0,0}	,{0,4,{0,8,0},0,{0,0,0}},0,1000	, 7}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_MEDIUM	}},	"Jet"					}
,	{{{0,0,4},{0,0,0}	,{0,4,{0,4,0},0,{0,0,0}},0,700	, 6}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_HEAVY	}},	"Kurata"				}
// -------------------------------------- Update 2		, 1
,	{{{0,0,4},{0,0,0}	,{0,4,{0,8,0},0,{0,0,0}},0,1200	, 10}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_HEAVY	}},	"Spaceship"				}
,	{{{0,0,4},{0,0,0}	,{0,4,{0,8,0},0,{0,0,0}},0,1400	, 30}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_HEAVY	}},	"Mothership"			}
,	{{{0,0,4},{0,0,4}	,{0,4,{0,8,0},0,{0,0,0}},0,2000	, 90}	,{{},{}	,{ENTITY_TECHNOLOGY_DIGITAL	, ENTITY_GRADE_HEAVY	}},	"Death Planet"			}
};

	static std::string getVehicleName(const SVehicle& vehicle) {
		return getEntityName(vehicle, definitionsVehicle, modifiersVehicle);
	}

	static SEntityPoints getVehiclePoints(const SVehicle& vehicle) {
		return getEntityPoints(vehicle, definitionsVehicle, modifiersVehicle);
	}

	static SEntityFlags getVehicleFlags(const SVehicle& vehicle) {
		return  getEntityFlags(vehicle, definitionsVehicle, modifiersVehicle);
	}

#pragma pack(pop)
}

#endif // __VEHICLES_H__91236982714698236423423__
