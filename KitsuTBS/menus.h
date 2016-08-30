#include "Menu.h"
#include "input.h"
#include "timer.h"
#include "asciiscreen.h"

#include <Windows.h>

#ifndef __MENUS_H__9237409126340927634987234__
#define __MENUS_H__9237409126340927634987234__

#pragma pack(push, 1)

static double lastKeyPress = 0.5;
static STimer timer;

#define MENU_ROFFSET 6

template <size_t _ArraySize, typename _ReturnType>
_ReturnType drawMenu(size_t optionCount, const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ArraySize], SInput& frameInput, _ReturnType exitValue, _ReturnType noActionValue = -1, bool disableEscKeyClose=false, const std::string& exitText="Exit this menu")
{
	optionCount = (optionCount < _ArraySize) ? optionCount : _ArraySize; // Fix optionCount to the maximum size of the array if optionCount is higher than the allowed size.

	int32_t lineOffset = (int32_t)(game::getASCIIBackBufferHeight()-MENU_ROFFSET-optionCount);
	game::lineToScreen(lineOffset++,	0, game::CENTER, "-- %s --", title.c_str() );	// Print menu title
	game::lineToScreen(lineOffset++,	0, game::CENTER, "Make your selection:" );

	lineOffset += 1;
	// Print menu options
	char numberKey[4] = {};
	for(size_t i=0; i<optionCount; i++) {
		sprintf_s(numberKey, "%u", (uint32_t)(i+1));
		game::lineToScreen(lineOffset++, 10, game::CENTER, "%2.2s: %-38.38s", numberKey, menuItems[i].Text.c_str());	
	}
	sprintf_s(numberKey, "%u", (uint32_t)(optionCount+1));
	game::lineToScreen(lineOffset++, 10, game::CENTER, "%2.2s: %-38.38s", numberKey, exitText.c_str());	
		
	timer.Frame();
	lastKeyPress += timer.LastTimeSeconds;
	if( lastKeyPress > 0.6 )
	{
		if(frameInput.Keys['1'+optionCount] || frameInput.Keys[VK_NUMPAD1+optionCount] || (frameInput.Keys[VK_ESCAPE] && !disableEscKeyClose)) {
			lastKeyPress = 0;
			return exitValue;
		}

		for(uint32_t i=0, count = (uint32_t)optionCount; i < count; i++) 
			if(frameInput.Keys['1'+i]) {
				lastKeyPress = 0;
				return menuItems[i].ReturnValue;
			}

		for(uint32_t i=0, count = (uint32_t)optionCount; i < count; i++) 
			if(frameInput.Keys[VK_NUMPAD1+i]) {
				lastKeyPress = 0;
				return menuItems[i].ReturnValue;
			}
	}

	return noActionValue;
}

template <size_t _ArraySize, typename _ReturnType>
_ReturnType drawMenu(const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ArraySize], SInput& frameInput, _ReturnType exitValue, _ReturnType noActionValue = -1, bool disableEscapeKey=false, const std::string& exitText="Exit this menu") {
	return drawMenu(_ArraySize, title, menuItems, frameInput, exitValue, noActionValue, disableEscapeKey, exitText);
}

enum GAME_SUBSTATE : uint8_t
{	GAME_SUBSTATE_MAIN
,	GAME_SUBSTATE_CHARACTER
,	GAME_SUBSTATE_PROFESSION
,	GAME_SUBSTATE_WEAPON
,	GAME_SUBSTATE_ARMOR
,	GAME_SUBSTATE_ACCESSORY
,	GAME_SUBSTATE_VEHICLE
,	GAME_SUBSTATE_FACILITY
,	GAME_SUBSTATE_STAGEPROP
,	GAME_SUBSTATE_ITEM
,	GAME_SUBSTATE_SKILL
};

enum GAME_STATE_EX : uint8_t
{	GAME_STATE_MENU_MAIN							
,	GAME_STATE_MENU_CONTROL_CENTER				
,	GAME_STATE_MENU_SQUAD_SETUP
,	GAME_STATE_MENU_RESEARCH						
,	GAME_STATE_MENU_EQUIPMENT
,	GAME_STATE_MENU_INSPECT
,	GAME_STATE_MENU_SENSE
,	GAME_STATE_MENU_ACTION
,	GAME_STATE_MENU_BUY
,	GAME_STATE_MENU_SELL
,	GAME_STATE_MENU_USE
,	GAME_STATE_MENU_OPTIONS						
,	GAME_STATE_MENU_OPTIONS_SCREEN				
,	GAME_STATE_MENU_OPTIONS_HOTKEYS				
,	GAME_STATE_START_MISSION						
,	GAME_STATE_WELCOME_COMMANDER					
,	GAME_STATE_CREDITS							
,	GAME_STATE_EXIT				
};

struct SGameState
{
	GAME_STATE_EX State;
	GAME_SUBSTATE Substate;
	GAME_STATE_EX PreviousState;
	GAME_SUBSTATE PreviousSubState;
};

// 1
static const klib::SMenuItem<SGameState> optionsResearch[] =
{	{ {GAME_STATE_MENU_RESEARCH, GAME_SUBSTATE_WEAPON		}, "Research new weapons"		}
,	{ {GAME_STATE_MENU_RESEARCH, GAME_SUBSTATE_ACCESSORY	}, "Research new accessories"	}
,	{ {GAME_STATE_MENU_RESEARCH, GAME_SUBSTATE_ARMOR		}, "Research new armors"		}
,	{ {GAME_STATE_MENU_RESEARCH, GAME_SUBSTATE_CHARACTER	}, "Learn new professions"		}
,	{ {GAME_STATE_MENU_RESEARCH, GAME_SUBSTATE_VEHICLE		}, "Research new vehicles"		}
,	{ {GAME_STATE_MENU_RESEARCH, GAME_SUBSTATE_FACILITY		}, "Research new buildings"		}
};

// 2
static const klib::SMenuItem<SGameState> optionsInspect[] =
{	{ {	GAME_STATE_MENU_INSPECT, GAME_SUBSTATE_WEAPON		}, "Inspect equipped weapon"	}
,	{ {	GAME_STATE_MENU_INSPECT, GAME_SUBSTATE_CHARACTER	}, "Inspect equipped accessory"	}
,	{ {	GAME_STATE_MENU_INSPECT, GAME_SUBSTATE_ARMOR		}, "Inspect equipped armor"		}
,	{ {	GAME_STATE_MENU_INSPECT, GAME_SUBSTATE_ACCESSORY	}, "Inspect active agent"		}
,	{ {	GAME_STATE_MENU_INSPECT, GAME_SUBSTATE_VEHICLE		}, "Inspect current vehicle"	}
,	{ {	GAME_STATE_MENU_INSPECT, GAME_SUBSTATE_FACILITY		}, "Inspect current facility"	}
,	{ {	GAME_STATE_MENU_INSPECT, GAME_SUBSTATE_ITEM			}, "Show inventory"				}
};	

// 3
static const klib::SMenuItem<SGameState> optionsSense[] =
{	{ {	GAME_STATE_MENU_SENSE, GAME_SUBSTATE_WEAPON			}, "Inspect enemy weapon"		}
,	{ {	GAME_STATE_MENU_SENSE, GAME_SUBSTATE_ACCESSORY		}, "Inspect enemy accessory"	}
,	{ {	GAME_STATE_MENU_SENSE, GAME_SUBSTATE_ARMOR			}, "Inspect enemy armor"		}
,	{ {	GAME_STATE_MENU_SENSE, GAME_SUBSTATE_CHARACTER		}, "Inspect enemy agent"		}
,	{ {	GAME_STATE_MENU_SENSE, GAME_SUBSTATE_VEHICLE		}, "Inspect enemy vehicle"		}
,	{ {	GAME_STATE_MENU_SENSE, GAME_SUBSTATE_FACILITY		}, "Inspect enemy building"		}
,	{ {	GAME_STATE_MENU_SENSE, GAME_SUBSTATE_ITEM			}, "Peek enemy inventory"		}
};

// 4
static const klib::SMenuItem<SGameState> optionsEquip[] =
{	{ {	GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_WEAPON		}, "Equip weapon"				}
,	{ {	GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_ACCESSORY	}, "Equip accessorie"			}
,	{ {	GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_ARMOR		}, "Equip armor"				}
,	{ {	GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_CHARACTER	}, "Hire agent"					}
,	{ {	GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_VEHICLE	}, "Equip vehicle"				}
,	{ {	GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_FACILITY	}, "Build facility"				}
,	{ {	GAME_STATE_MENU_EQUIPMENT, GAME_SUBSTATE_ITEM		}, "Equip items"				}
};

// 5
static const klib::SMenuItem<SGameState> optionsConfig[] =
{	{ {GAME_STATE_MENU_OPTIONS_SCREEN	}, "Screen size"									}
,	{ {GAME_STATE_MENU_OPTIONS_HOTKEYS	}, "Keyboard configuration"							}
};

// 6
static const klib::SMenuItem<int32_t> optionsAreYouSure[]
{	{ 1, "Yes"	}
,	{ 0, "No"	}
};

// 7
static const klib::SMenuItem<SGameState> optionsMain[] = 
{	{ {	GAME_STATE_WELCOME_COMMANDER	}, "Start new game"									}
,	{ {	GAME_STATE_MENU_OPTIONS			}, "Options"										}
,	{ {	GAME_STATE_CREDITS				}, "Credits"										}
};

// 8
static const klib::SMenuItem<SGameState> optionsControlCenter[] = 
{	{	{ GAME_STATE_START_MISSION		}, "Start new Mission"								}
,	{	{ GAME_STATE_MENU_SQUAD_SETUP	}, "Set up Squad"									}
,	{	{ GAME_STATE_MENU_RESEARCH		}, "Visit Labs"										}
,	{	{ GAME_STATE_MENU_BUY			}, "Buy items and/or equipment"						}
,	{	{ GAME_STATE_MENU_SELL			}, "Sell items and/or equipment"					}
};

// 9
static const klib::SMenuItem<SGameState> optionsBuy[] =
{	{ {	GAME_STATE_MENU_BUY, GAME_SUBSTATE_WEAPON		}	, "Buy weapons"					}
,	{ {	GAME_STATE_MENU_BUY, GAME_SUBSTATE_ACCESSORY	}	, "Buy accessories"				}
,	{ {	GAME_STATE_MENU_BUY, GAME_SUBSTATE_ARMOR		}	, "Buy armors"					}
,	{ {	GAME_STATE_MENU_BUY, GAME_SUBSTATE_CHARACTER	}	, "Hire agents"					}
,	{ {	GAME_STATE_MENU_BUY, GAME_SUBSTATE_VEHICLE		}	, "Buy vehicles"				}
,	{ {	GAME_STATE_MENU_BUY, GAME_SUBSTATE_FACILITY		}	, "Build facilities"			}
,	{ {	GAME_STATE_MENU_BUY, GAME_SUBSTATE_ITEM			}	, "Buy items"					}
};

// 10
static const klib::SMenuItem<SGameState> optionsSell[] =
{	{ {	GAME_STATE_MENU_SELL, GAME_SUBSTATE_WEAPON		}	, "Sell weapons"				}
,	{ {	GAME_STATE_MENU_SELL, GAME_SUBSTATE_ACCESSORY	}	, "Sell accessories"			}
,	{ {	GAME_STATE_MENU_SELL, GAME_SUBSTATE_ARMOR		}	, "Sell armors"					}
,	{ {	GAME_STATE_MENU_SELL, GAME_SUBSTATE_CHARACTER	}	, "Sack agents"					}
,	{ {	GAME_STATE_MENU_SELL, GAME_SUBSTATE_VEHICLE		}	, "Buy vehicles"				}
,	{ {	GAME_STATE_MENU_SELL, GAME_SUBSTATE_FACILITY	}	, "Dismantle facilities"		}
,	{ {	GAME_STATE_MENU_SELL, GAME_SUBSTATE_ITEM		}	, "Sell items"					}
};


enum TURN_ACTION
{	TURN_ACTION_ATTACK
,	TURN_ACTION_INVENTORY
,	TURN_ACTION_SKILL
,	TURN_ACTION_SENSE
,	TURN_ACTION_RUN
,	TURN_ACTION_CANCEL
,	TURN_ACTION_CONTINUE
};

// 11
static const klib::SMenuItem<TURN_ACTION> optionsCombatTurn[] =
{ { TURN_ACTION_ATTACK		, "Attack"			}
, { TURN_ACTION_INVENTORY	, "Inventory"		}
, { TURN_ACTION_SKILL		, "Skills"			}
, { TURN_ACTION_SENSE		, "Sense"			}
, { TURN_ACTION_CANCEL		, "Cancel turn"		}
, { TURN_ACTION_RUN			, "Run"				}
};

#pragma pack(pop)


#endif // __MENUS_H__9237409126340927634987234__