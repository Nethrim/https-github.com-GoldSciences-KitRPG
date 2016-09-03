#include "Menu.h"
#include "input.h"
#include "timer.h"
#include "asciiscreen.h"
#include "Misc.h"
#include "Tile.h"

#include <Windows.h>

#ifndef __MENUS_H__9237409126340927634987234__
#define __MENUS_H__9237409126340927634987234__

#pragma pack(push, 1)

#define MENU_ROFFSET 6

extern klib::STimer drawMenu_timer;
extern klib::SAccumulator<double> drawMenu_accumulator;

namespace klib
{

	template <size_t _ArraySize, typename _ReturnType>
	_ReturnType drawMenu(char* targetASCII, size_t targetWidth, size_t targetHeight, size_t optionCount, const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ArraySize], const klib::SInput& frameInput, _ReturnType exitValue, _ReturnType noActionValue=-1, uint32_t rowWidth=20, bool disableEscKeyClose=false, const std::string& exitText="Exit this menu")
	{
		drawMenu_timer.Frame();

		optionCount = (optionCount < _ArraySize) ? optionCount : _ArraySize; // Fix optionCount to the maximum size of the array if optionCount is higher than the allowed size.

		int32_t lineOffset = (int32_t)(targetHeight-MENU_ROFFSET-3-std::min((int32_t)optionCount, 9));
		std::string clearString(128, ' ');
		for(int32_t i=0, count = (int32_t)targetHeight-lineOffset; i<count; ++i)
			klib::lineToRect(targetASCII, targetWidth, targetHeight, lineOffset+i, 0, klib::CENTER, "%s", clearString.c_str());		//"-- %s --", title.c_str() );	// Print menu title

		const std::string textToPrint = "-- " + title + " --";
		static char slowMessage[128] = {'_',};
		static size_t menuItemAccum = 0;
		static size_t currentPage = 0;
		bool multipage = optionCount > 9;
		const uint32_t pageCount = (uint32_t)((multipage == false) ? 1 : 1+optionCount/9);

		if( currentPage >= pageCount )
			currentPage = 0;

		bool bDonePrinting = getMessageSlow(slowMessage, textToPrint, drawMenu_timer.LastTimeSeconds*2);
		klib::lineToRect(targetASCII, targetWidth, targetHeight, lineOffset++, 0, klib::CENTER, slowMessage);		//"-- %s --", title.c_str() );	// Print menu title
		if( !bDonePrinting )
			return noActionValue;
		else if(drawMenu_accumulator.Value < 0.575)
			drawMenu_accumulator.Value = 0.575;

		lineOffset += 1;
		// Print menu options
		char numberKey[4] = {};
		uint32_t numberCharsAvailable = rowWidth-4;	// 4 is for "%2.2s: "
		char formatString[24] = {};
		sprintf_s(formatString, "%%2.2s: %%-%u.%us", numberCharsAvailable, numberCharsAvailable);

		const uint32_t posXOffset = 0;
		uint32_t actualOptionCount = std::min(9U, (uint32_t)(optionCount-(currentPage*9)));
		const size_t itemOffset = currentPage*9;
		for(size_t i=0, count = (menuItemAccum < actualOptionCount) ? menuItemAccum : actualOptionCount; i<count; i++) {
			sprintf_s(numberKey, "%u", (uint32_t)(i+1));
			klib::lineToRect(targetASCII, targetWidth, targetHeight, lineOffset++, posXOffset, klib::CENTER, formatString, numberKey, menuItems[itemOffset+i].Text.c_str());	
		}
		if(menuItemAccum > actualOptionCount) {
			sprintf_s(numberKey, "%s", "0");
			klib::lineToRect(targetASCII, targetWidth, targetHeight, (int32_t)targetHeight-MENU_ROFFSET, posXOffset, klib::CENTER, formatString, numberKey, exitText.c_str());	
		}
		if(multipage)
		{
			if(currentPage == 0)
				klib::lineToRect(targetASCII, targetWidth, targetHeight, (int32_t)targetHeight-MENU_ROFFSET+2, posXOffset, klib::CENTER, "Page down: Next page.");	
			else if(currentPage == (pageCount-1))
				klib::lineToRect(targetASCII, targetWidth, targetHeight, (int32_t)targetHeight-MENU_ROFFSET+2, posXOffset, klib::CENTER, "Page up: Previous page.");	
			else
				klib::lineToRect(targetASCII, targetWidth, targetHeight, (int32_t)targetHeight-MENU_ROFFSET+2, posXOffset, klib::CENTER, "Page up: Previous page. Page down: Next page");	
		}
		
		_ReturnType& resultVal = noActionValue;
		bool bResetMenuStuff = false;
		if( drawMenu_accumulator.Accumulate(drawMenu_timer.LastTimeSeconds) )
		{
			// Don't process keys until the menu has finished displaying
			if(menuItemAccum <= actualOptionCount) {
				drawMenu_accumulator.Value = 0.575;
				menuItemAccum++;
			}
			// Process page change keys first.
			else if(currentPage < (pageCount-1) && frameInput.Keys[VK_NEXT]) {
				bResetMenuStuff = true;
				currentPage++;
			}
			else if(currentPage > 0 && frameInput.Keys[VK_PRIOR]) {
				bResetMenuStuff = true;
				currentPage--;
			}
			else if(frameInput.Keys['0'] || frameInput.Keys[VK_NUMPAD0] || (frameInput.Keys[VK_ESCAPE] && !disableEscKeyClose))  {
				bResetMenuStuff = true;
				resultVal = exitValue;
			}
			else 
				for(uint32_t i=0, count = (uint32_t)actualOptionCount; i < count; i++) 
					if(frameInput.Keys['1'+i] || frameInput.Keys[VK_NUMPAD1+i]) 
					{
						bResetMenuStuff = true;
						resultVal = menuItems[i+itemOffset].ReturnValue;
						break;
					}
		}

		if(bResetMenuStuff)
		{
			menuItemAccum = 0;
			drawMenu_accumulator.Value = 0;
			resetCursorString(slowMessage);
		}

		return resultVal;
	}

	template <typename _CellType, size_t _Width, size_t _Depth, size_t _ItemCount, typename _ReturnType>
	inline _ReturnType drawMenu(SGrid<_CellType, _Width, _Depth>& display, size_t optionCount, const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ItemCount], const klib::SInput& frameInput, _ReturnType exitValue, _ReturnType noActionValue = -1, uint32_t rowWidth=20, bool disableEscapeKey=false, const std::string& exitText="Exit this menu") {
		return drawMenu(&display.Cells[0][0], _Width, _Depth, optionCount, title, menuItems, frameInput, exitValue, noActionValue, rowWidth, disableEscapeKey, exitText);
	}
	
	template <typename _CellType, size_t _Width, size_t _Depth, size_t _ItemCount, typename _ReturnType>
	inline _ReturnType drawMenu(SGrid<_CellType, _Width, _Depth>& display, const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ItemCount], const klib::SInput& frameInput, _ReturnType exitValue, _ReturnType noActionValue = -1, uint32_t rowWidth=20, bool disableEscapeKey=false, const std::string& exitText="Exit this menu") {
		return drawMenu(&display.Cells[0][0], _Width, _Depth, _ItemCount, title, menuItems, frameInput, exitValue, noActionValue, rowWidth, disableEscapeKey, exitText);
	}

	template <typename _ReturnType, size_t _ItemCount>
	struct SMenu
	{
		const std::string					Title;
		const uint32_t						RowWidth;
		const bool							bDisableEscapeKey;
		const _ReturnType					ValueExit;
		const std::string					TextExit;
		klib::SMenuItem<_ReturnType>		Items[_ItemCount];

		SMenu(const klib::SMenuItem<_ReturnType> (&options)[_ItemCount], _ReturnType exitValue, const std::string& title, size_t rowWidth=24, bool disableEscapeKey=false, const std::string& exitText="Exit this menu")
			:Title				(title)
			,RowWidth			((uint32_t)rowWidth)
			//,Items				()
			,bDisableEscapeKey	(disableEscapeKey)
			,ValueExit			(exitValue)
			,TextExit			(exitText)
		{
			for(uint32_t i=0; i<_ItemCount; ++i)
				Items[i] = options[i];
		};
	};

	template <typename _CellType, size_t _Width, size_t _Depth, size_t _ItemCount, typename _ReturnType>
	_ReturnType drawMenu(SGrid<_CellType, _Width, _Depth>& display, const SMenu<_ReturnType, _ItemCount>& menuInstance, const klib::SInput& frameInput, _ReturnType noActionValue = -1) {
		return drawMenu(&display.Cells[0][0], _Width, _Depth, (uint32_t)_ItemCount, menuInstance.Title, menuInstance.Items, frameInput, menuInstance.ValueExit, noActionValue, menuInstance.RowWidth, menuInstance.bDisableEscapeKey, menuInstance.TextExit);
	}

	template <typename _CellType, size_t _Width, size_t _Depth, size_t _ItemCount, typename _ReturnType>
	_ReturnType drawMenu(SGrid<_CellType, _Width, _Depth>& display, const SMenu<_ReturnType, _ItemCount>& menuInstance, uint32_t optionCount, const klib::SInput& frameInput, _ReturnType noActionValue = -1) {
		return drawMenu(&display.Cells[0][0], _Width, _Depth, optionCount, menuInstance.Title, menuInstance.Items, frameInput, menuInstance.ValueExit, noActionValue, menuInstance.RowWidth, menuInstance.bDisableEscapeKey, menuInstance.TextExit);
	}


	// We can use 3 bits for the substate modifier and 5 bits for the substate itself
	enum GAME_SUBSTATE_MODIFIER : uint8_t
	{	GAME_SUBSTATE_MODIFIER_MAIN			= 0
	,	GAME_SUBSTATE_MODIFIER_SMALL		= 1
	,	GAME_SUBSTATE_MODIFIER_REGULAR		= 2
	,	GAME_SUBSTATE_MODIFIER_LARGE		= 3
	,	GAME_SUBSTATE_MODIFIER_HUGE			= 4
	,	GAME_SUBSTATE_MODIFIER_PRIMARY		= 5
	,	GAME_SUBSTATE_MODIFIER_SECONDARY	= 6
	};

	enum GAME_SUBSTATE : uint8_t
	{	GAME_SUBSTATE_MAIN			= 0
	,	GAME_SUBSTATE_CHARACTER		= 1
	,	GAME_SUBSTATE_PROFESSION	= 2
	,	GAME_SUBSTATE_WEAPON		= 3
	,	GAME_SUBSTATE_ARMOR			= 4
	,	GAME_SUBSTATE_ACCESSORY		= 5
	,	GAME_SUBSTATE_VEHICLE		= 6
	,	GAME_SUBSTATE_FACILITY		= 7
	,	GAME_SUBSTATE_STAGEPROP		= 8
	,	GAME_SUBSTATE_ITEM			= 9
	,	GAME_SUBSTATE_SKILL			= 10
	,	GAME_SUBSTATE_RESET			= 11
	,	GAME_SUBSTATE_SCREEN		= 12
	,	GAME_SUBSTATE_HOTKEY		= 13
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
	,	GAME_STATE_START_MISSION
	,	GAME_STATE_WELCOME_COMMANDER
	,	GAME_STATE_CREDITS
	,	GAME_STATE_EXIT
	};

	struct SGameState
	{
		GAME_STATE_EX State;
		GAME_SUBSTATE Substate;
	};

	// 1
	static const klib::SMenuItem<SGameState> optionsMain[] = 
	{	{ { GAME_STATE_WELCOME_COMMANDER	, GAME_SUBSTATE_RESET		}, "Start new game"					}
	,	{ { GAME_STATE_MENU_OPTIONS			, GAME_SUBSTATE_MAIN		}, "Options"						}
	,	{ { GAME_STATE_CREDITS				, GAME_SUBSTATE_MAIN		}, "Credits"						}
	};

	// 2
	static const klib::SMenuItem<SGameState> optionsMainInGame[] = 
	{	{ { GAME_STATE_WELCOME_COMMANDER	, GAME_SUBSTATE_MAIN		}, "Continue game"					}
	,	{ { GAME_STATE_WELCOME_COMMANDER	, GAME_SUBSTATE_RESET		}, "Start new game"					}
	,	{ { GAME_STATE_MENU_OPTIONS			, GAME_SUBSTATE_MAIN		}, "Options"						}
	,	{ { GAME_STATE_CREDITS				, GAME_SUBSTATE_MAIN		}, "Credits"						}
	};

	// 3
	static const klib::SMenuItem<SGameState> optionsConfig[] =
	{	{ { GAME_STATE_MENU_OPTIONS			, GAME_SUBSTATE_SCREEN		}, "Screen size"					}
	,	{ { GAME_STATE_MENU_OPTIONS			, GAME_SUBSTATE_HOTKEY		}, "Keyboard configuration"			}
	};

	// 4
	static const klib::SMenuItem<SGameState> optionsResearch[] =
	{	{ { GAME_STATE_MENU_RESEARCH		, GAME_SUBSTATE_WEAPON		}, "Research new weapons"			}
	,	{ { GAME_STATE_MENU_RESEARCH		, GAME_SUBSTATE_ACCESSORY	}, "Research new accessories"		}
	,	{ { GAME_STATE_MENU_RESEARCH		, GAME_SUBSTATE_ARMOR		}, "Research new armors"			}
	,	{ { GAME_STATE_MENU_RESEARCH		, GAME_SUBSTATE_PROFESSION	}, "Learn new jobs"					}
	,	{ { GAME_STATE_MENU_RESEARCH		, GAME_SUBSTATE_VEHICLE		}, "Research new vehicles"			}
	,	{ { GAME_STATE_MENU_RESEARCH		, GAME_SUBSTATE_FACILITY	}, "Research new buildings"			}
	};

	// 5
	static const klib::SMenuItem<SGameState> optionsInspect[] =
	{	{ { GAME_STATE_MENU_INSPECT			, GAME_SUBSTATE_WEAPON		}, "Inspect weapon"					}
	,	{ { GAME_STATE_MENU_INSPECT			, GAME_SUBSTATE_ACCESSORY	}, "Inspect accessory"				}
	,	{ { GAME_STATE_MENU_INSPECT			, GAME_SUBSTATE_ARMOR		}, "Inspect armor"					}
	,	{ { GAME_STATE_MENU_INSPECT			, GAME_SUBSTATE_PROFESSION	}, "Inspect job license"			}
	,	{ { GAME_STATE_MENU_INSPECT			, GAME_SUBSTATE_VEHICLE		}, "Inspect vehicle"				}
	,	{ { GAME_STATE_MENU_INSPECT			, GAME_SUBSTATE_FACILITY	}, "Inspect building"				}
	,	{ { GAME_STATE_MENU_INSPECT			, GAME_SUBSTATE_CHARACTER	}, "Inspect general information"	}
	,	{ { GAME_STATE_MENU_INSPECT			, GAME_SUBSTATE_ITEM		}, "View items"						}
	};

	// 6
	static const klib::SMenuItem<SGameState> optionsSense[] =
	{	{ { GAME_STATE_MENU_SENSE			, GAME_SUBSTATE_WEAPON		}, "Sense weapon"					}
	,	{ { GAME_STATE_MENU_SENSE			, GAME_SUBSTATE_ACCESSORY	}, "Sense accessory"				}
	,	{ { GAME_STATE_MENU_SENSE			, GAME_SUBSTATE_ARMOR		}, "Sense armor"					}
	,	{ { GAME_STATE_MENU_SENSE			, GAME_SUBSTATE_PROFESSION	}, "Sense job license"				}
	,	{ { GAME_STATE_MENU_SENSE			, GAME_SUBSTATE_VEHICLE		}, "Sense vehicle"					}
	,	{ { GAME_STATE_MENU_SENSE			, GAME_SUBSTATE_FACILITY	}, "Sense building"					}
	,	{ { GAME_STATE_MENU_SENSE			, GAME_SUBSTATE_CHARACTER	}, "Sense general information"		}
	,	{ { GAME_STATE_MENU_SENSE			, GAME_SUBSTATE_ITEM		}, "Peek items"						}
	};

	// 7
	static const klib::SMenuItem<SGameState> optionsEquip[] =
	{	{ { GAME_STATE_MENU_EQUIPMENT		, GAME_SUBSTATE_WEAPON		}, "Equip weapon"					}
	,	{ { GAME_STATE_MENU_EQUIPMENT		, GAME_SUBSTATE_ACCESSORY	}, "Equip accessories"				}
	,	{ { GAME_STATE_MENU_EQUIPMENT		, GAME_SUBSTATE_ARMOR		}, "Equip armor"					}
	,	{ { GAME_STATE_MENU_EQUIPMENT		, GAME_SUBSTATE_PROFESSION	}, "Assign task"					}
	,	{ { GAME_STATE_MENU_EQUIPMENT		, GAME_SUBSTATE_VEHICLE		}, "Equip vehicle"					}
	,	{ { GAME_STATE_MENU_EQUIPMENT		, GAME_SUBSTATE_FACILITY	}, "Assign facility"				}
	,	{ { GAME_STATE_MENU_EQUIPMENT		, GAME_SUBSTATE_ITEM		}, "Equip items"					}
	,	{ { GAME_STATE_MENU_EQUIPMENT		, GAME_SUBSTATE_CHARACTER	}, "Assign another agent"			}
	};

	// 8
	static const klib::SMenuItem<SGameState> optionsBuy[] =
	{	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_WEAPON		}, "Buy weapons"					}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_ACCESSORY	}, "Buy accessories"				}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_ARMOR		}, "Buy armors"						}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_PROFESSION	}, "Buy job license"				}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_VEHICLE		}, "Buy vehicles"					}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_FACILITY	}, "Build facilities"				}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_ITEM		}, "Buy items"						}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_CHARACTER	}, "Hire agents"					}
	};

	static const klib::SMenuItem<SGameState> optionsBuySize[] =
	{	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_ITEM		}, "Small"							}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_ITEM		}, "Regular"						}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_ITEM		}, "Large"							}
	,	{ { GAME_STATE_MENU_BUY				, GAME_SUBSTATE_ITEM		}, "Huge"							}
	};

	// 9
	static const klib::SMenuItem<SGameState> optionsSell[] =	
	{	{ { GAME_STATE_MENU_SELL			, GAME_SUBSTATE_WEAPON		}, "Sell weapons"					}
	,	{ { GAME_STATE_MENU_SELL			, GAME_SUBSTATE_ACCESSORY	}, "Sell accessories"				}
	,	{ { GAME_STATE_MENU_SELL			, GAME_SUBSTATE_ARMOR		}, "Sell armors"					}
	,	{ { GAME_STATE_MENU_SELL			, GAME_SUBSTATE_PROFESSION	}, "Sell job license"				}
	,	{ { GAME_STATE_MENU_SELL			, GAME_SUBSTATE_VEHICLE		}, "Sell vehicles"					}
	,	{ { GAME_STATE_MENU_SELL			, GAME_SUBSTATE_FACILITY	}, "Dismantle facilities"			}
	,	{ { GAME_STATE_MENU_SELL			, GAME_SUBSTATE_ITEM		}, "Sell items"						}
	,	{ { GAME_STATE_MENU_SELL			, GAME_SUBSTATE_CHARACTER	}, "Sack agents"					}
	};

	// 10
	static const klib::SMenuItem<SGameState> optionsControlCenter[] = 
	{	{ { GAME_STATE_START_MISSION		}, "Start new Mission"			}
	,	{ { GAME_STATE_MENU_SQUAD_SETUP		}, "Set up Squad"				}
	,	{ { GAME_STATE_MENU_RESEARCH		}, "Visit Labs"					}
	,	{ { GAME_STATE_MENU_BUY				}, "Buy items and equipment"	}
	,	{ { GAME_STATE_MENU_SELL			}, "Sell items and equipment"	}
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

	// 12
	static const klib::SMenuItem<int32_t> optionsAreYouSure[]
	{	{ 1, "Yes"	}
	,	{ 0, "No"	}
	};
}

#pragma pack(pop)


#endif // __MENUS_H__9237409126340927634987234__