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
_ReturnType drawMenu(size_t optionCount, const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ArraySize], SInput& frameInput, _ReturnType exitValue, const std::string& exitText="Exit this menu", bool disableEscKeyClose=false)
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

	return (_ReturnType)~(_ReturnType)0;
}

template <size_t _ArraySize, typename _ReturnType>
_ReturnType drawMenu(const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ArraySize], SInput& frameInput, _ReturnType exitValue, const std::string& exitText="Exit this menu", bool disableEscapeKey=false) {
	return drawMenu(_ArraySize, title, menuItems, frameInput, exitValue, exitText, disableEscapeKey);
}

enum GAME_STATE
{	GAME_MENU_MAIN							
,	GAME_MENU_CONTROL_CENTER				
,	GAME_MENU_SQUAD_SETUP_MAIN				
,	GAME_MENU_CONSTRUCTION_MAIN				
,	GAME_MENU_EQUIP_MAIN					
,	GAME_MENU_EQUIP_CHARACTER				
,	GAME_MENU_EQUIP_WEAPON					
,	GAME_MENU_EQUIP_ACCESSORY				
,	GAME_MENU_EQUIP_ARMOR					
,	GAME_MENU_EQUIP_VEHICLE					
,	GAME_MENU_EQUIP_FACILITY				
,	GAME_MENU_ACTION_MAIN					
,	GAME_MENU_SENSE_MAIN					
,	GAME_MENU_RESEARCH						
,	GAME_MENU_RESEARCH_CHARACTER			
,	GAME_MENU_RESEARCH_WEAPON				
,	GAME_MENU_RESEARCH_ACCESSORY			
,	GAME_MENU_RESEARCH_ARMOR				
,	GAME_MENU_RESEARCH_VEHICLE				
,	GAME_MENU_RESEARCH_FACILITY				
,	GAME_MENU_RESEARCH_MODIFIER_CHARACTER	
,	GAME_MENU_RESEARCH_MODIFIER_WEAPON		
,	GAME_MENU_RESEARCH_MODIFIER_ACCESSORY	
,	GAME_MENU_RESEARCH_MODIFIER_ARMOR		
,	GAME_MENU_RESEARCH_MODIFIER_VEHICLE		
,	GAME_MENU_RESEARCH_MODIFIER_FACILITY	
,	GAME_MENU_INSPECT_MAIN					
,	GAME_MENU_BUY_MAIN						
,	GAME_MENU_BUY_ITEM						
//,	GAME_MENU_BUY_EQUIP_MAIN				
,	GAME_MENU_BUY_EQUIP_CHARACTER			
,	GAME_MENU_BUY_EQUIP_WEAPON				
,	GAME_MENU_BUY_EQUIP_ACCESSORY			
,	GAME_MENU_BUY_EQUIP_ARMOR				
,	GAME_MENU_BUY_EQUIP_VEHICLE				
,	GAME_MENU_BUY_EQUIP_FACILITY			
,	GAME_MENU_SELL_MAIN						
,	GAME_MENU_SELL_ITEM						
//,	GAME_MENU_SELL_EQUIP_MAIN				
,	GAME_MENU_SELL_EQUIP_CHARACTER			
,	GAME_MENU_SELL_EQUIP_WEAPON				
,	GAME_MENU_SELL_EQUIP_ACCESSORY			
,	GAME_MENU_SELL_EQUIP_ARMOR				
,	GAME_MENU_SELL_EQUIP_VEHICLE			
,	GAME_MENU_SELL_EQUIP_FACILITY			
,	GAME_MENU_USE_ITEM						
,	GAME_MENU_USE_SKILL						
,	GAME_MENU_OPTIONS						
,	GAME_MENU_OPTIONS_SCREEN				
,	GAME_MENU_OPTIONS_HOTKEYS				
,	GAME_INSPECT_CHARACTER					
,	GAME_INSPECT_WEAPON						
,	GAME_INSPECT_ACCESSORY					
,	GAME_INSPECT_ARMOR						
,	GAME_INSPECT_VEHICLE					
,	GAME_INSPECT_FACILITY					
,	GAME_INSPECT_ITEMS						
,	GAME_SENSE_CHARACTER					
,	GAME_SENSE_WEAPON						
,	GAME_SENSE_ACCESSORY					
,	GAME_SENSE_ARMOR						
,	GAME_SENSE_VEHICLE						
,	GAME_SENSE_FACILITY						
,	GAME_SENSE_ITEMS						
,	GAME_START_MISSION						
,	GAME_CREDITS							
,	GAME_EXIT								
};

// 1
static const klib::SMenuItem<GAME_STATE> optionsResearch[] =
{	{ GAME_MENU_RESEARCH_WEAPON				, "Research new weapons"					}
,	{ GAME_MENU_RESEARCH_MODIFIER_WEAPON	, "Research new sciences"					}
,	{ GAME_MENU_RESEARCH_ACCESSORY			, "Research new accessories"				}
,	{ GAME_MENU_RESEARCH_MODIFIER_ACCESSORY	, "Research new techniques"					}
,	{ GAME_MENU_RESEARCH_ARMOR				, "Research new armors"						}
,	{ GAME_MENU_RESEARCH_MODIFIER_ARMOR		, "Research new technologies"				}
,	{ GAME_MENU_RESEARCH_CHARACTER			, "Learn new professions"					}
,	{ GAME_MENU_RESEARCH_MODIFIER_CHARACTER	, "Train for new ranks"						}
,	{ GAME_MENU_RESEARCH_VEHICLE			, "Research new vehicles"					}
,	{ GAME_MENU_RESEARCH_MODIFIER_VEHICLE	, "Research new advancements"				}
,	{ GAME_MENU_RESEARCH_FACILITY			, "Research new buildings"					}
,	{ GAME_MENU_RESEARCH_MODIFIER_FACILITY	, "Research new architectures"				}
//,	{ GAME_MENU_CONTROL_CENTER				, "Back to Control Center"					}
};

// 2
static const klib::SMenuItem<GAME_STATE> optionsInspect[] =
{	{ GAME_INSPECT_WEAPON					, "Inspect equipped weapon"					}
,	{ GAME_INSPECT_CHARACTER				, "Inspect equipped accessory"				}
,	{ GAME_INSPECT_ARMOR					, "Inspect equipped armor"					}
,	{ GAME_INSPECT_ACCESSORY				, "Inspect active agent"					}
,	{ GAME_INSPECT_VEHICLE					, "Inspect current vehicle"					}
,	{ GAME_INSPECT_FACILITY					, "Inspect current facility"				}
,	{ GAME_INSPECT_ITEMS					, "Show inventory"							}
//,	{ GAME_MENU_CONTROL_CENTER				, "Back to main menu"						}
};	

// 3
static const klib::SMenuItem<GAME_STATE> optionsSense[] =
{	{ GAME_SENSE_WEAPON						, "Inspect enemy weapon"					}
,	{ GAME_SENSE_ACCESSORY					, "Inspect enemy accessory"					}
,	{ GAME_SENSE_ARMOR						, "Inspect enemy armor"						}
,	{ GAME_SENSE_CHARACTER					, "Inspect enemy agent"						}
,	{ GAME_SENSE_VEHICLE					, "Inspect enemy vehicle"					}
,	{ GAME_SENSE_FACILITY					, "Inspect enemy building"					}
,	{ GAME_SENSE_ITEMS						, "Peek enemy inventory"					}
//,	{ GAME_MENU_ACTION_MAIN					, "Back to action menu"						}
};

// 4
static const klib::SMenuItem<GAME_STATE> optionsEquip[] =
{	{ GAME_MENU_EQUIP_WEAPON				, "Equip weapon"							}
,	{ GAME_MENU_EQUIP_ACCESSORY				, "Equip accessory"							}
,	{ GAME_MENU_EQUIP_ARMOR					, "Equip armor"								}
,	{ GAME_MENU_EQUIP_VEHICLE				, "Select vehicle"							}
,	{ GAME_MENU_EQUIP_FACILITY				, "Assign building"							}
//,	{ GAME_MENU_CONTROL_CENTER				, "Back to main menu"						}
};

// 5
static const klib::SMenuItem<GAME_STATE> optionsConfig[] =
{	{ GAME_MENU_OPTIONS_SCREEN				, "Screen size"								}
,	{ GAME_MENU_OPTIONS_HOTKEYS				, "Keyboard configuration"					}
//,	{ GAME_MENU_MAIN						, "Back to main menu"						}
};

// 6
static const klib::SMenuItem<int32_t> optionsAreYouSure[]
{	{ 1, "Yes"	}
,	{ 0, "No"	}
};

// 7
static const klib::SMenuItem<GAME_STATE> optionsMain[] = 
{	{ GAME_MENU_CONTROL_CENTER				, "Start new game"							}
,	{ GAME_MENU_OPTIONS						, "Options"									}
,	{ GAME_CREDITS							, "Credits"									}
//,	{ GAME_EXIT								, "Exit Game"								}
};

// 8
static const klib::SMenuItem<GAME_STATE> optionsControlCenter[] = 
{	{ GAME_START_MISSION					, "Start new Mission"						}
,	{ GAME_MENU_SQUAD_SETUP_MAIN			, "Set up Squad"							}
,	{ GAME_MENU_RESEARCH					, "Visit Labs"								}
//,	{ GAME_MENU_CONSTRUCTION_MAIN			, "Visit Factory"							}
,	{ GAME_MENU_BUY_MAIN					, "Buy items and/or equipment"				}
,	{ GAME_MENU_SELL_MAIN					, "Sell items and/or equipment"				}
//,	{ GAME_MENU_MAIN						, "Quit game"								}
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

// 9
static const klib::SMenuItem<TURN_ACTION> optionsCombatTurn[] =
{ { TURN_ACTION_ATTACK						, "Attack"									}
, { TURN_ACTION_INVENTORY					, "Inventory"								}
, { TURN_ACTION_SKILL						, "Skills"									}
, { TURN_ACTION_SENSE						, "Sense"									}
, { TURN_ACTION_CANCEL						, "Cancel turn"								}
, { TURN_ACTION_RUN							, "Run"										}
};

// 10
static const klib::SMenuItem<GAME_STATE> optionsBuy[] =
{ { GAME_MENU_BUY_ITEM						, "Buy Items"								}
, { GAME_MENU_BUY_EQUIP_CHARACTER			, "Hire Agents"								}
, { GAME_MENU_BUY_EQUIP_WEAPON				, "Buy Weapons"								}
, { GAME_MENU_BUY_EQUIP_ACCESSORY			, "Buy Accessories"							}
, { GAME_MENU_BUY_EQUIP_ARMOR				, "Buy Armors"								}
, { GAME_MENU_BUY_EQUIP_VEHICLE				, "Buy Vehicles"							}
, { GAME_MENU_BUY_EQUIP_FACILITY			, "Build Facilities"						}
//, { GAME_MENU_CONTROL_CENTER				, "Back to Control Center"					}
};

// 11
static const klib::SMenuItem<GAME_STATE> optionsSell[] =
{ { GAME_MENU_SELL_ITEM						, "Sell Items"								}
, { GAME_MENU_SELL_EQUIP_CHARACTER			, "Sack Agents"								}
, { GAME_MENU_SELL_EQUIP_WEAPON				, "Sell Weapons"							}
, { GAME_MENU_SELL_EQUIP_ACCESSORY			, "Sell Accessories"						}
, { GAME_MENU_SELL_EQUIP_ARMOR				, "Sell Armors"								}
, { GAME_MENU_SELL_EQUIP_VEHICLE			, "Sell Vehicles"							}
, { GAME_MENU_SELL_EQUIP_FACILITY			, "Dismantle Facilities"					}
//, { GAME_MENU_CONTROL_CENTER				, "Back to Control Center"					}
};

#pragma pack(pop)


#endif // __MENUS_H__9237409126340927634987234__