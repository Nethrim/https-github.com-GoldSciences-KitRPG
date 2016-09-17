#include "Menu.h"
#include "timer.h"
#include "input.h"
#include "color.h"

#include <Windows.h>

#ifndef __DRAWMENU_H__98237409236409872364987236498__
#define __DRAWMENU_H__98237409236409872364987236498__


struct SDrawMenuGlobals
{
	klib::STimer				Timer;
	klib::SAccumulator<double>	Accumulator;
};

extern SDrawMenuGlobals	drawMenu_globals;

#define MENU_ROFFSET 8

namespace klib
{
#pragma pack(push, 1)
	struct SDrawMenuLocalStatics
	{
		char		SlowMessage[128]	= {'_',};
		uint32_t	MenuItemAccum		= 0;
		uint32_t	CurrentPage			= 0;
	};
#pragma pack(pop)

	static void printMultipageHelp(char* targetASCII, uint16_t* targetAttributes, size_t targetWidth, size_t targetHeight, size_t currentPage, uint32_t pageCount, uint32_t posXOffset) {
		if(currentPage == 0)					lineToRect(targetASCII, targetWidth, targetHeight, (int32_t)targetHeight-MENU_ROFFSET+2, posXOffset, klib::CENTER, "Page down: Next page.");	
		else if(currentPage == (pageCount-1))	lineToRect(targetASCII, targetWidth, targetHeight, (int32_t)targetHeight-MENU_ROFFSET+2, posXOffset, klib::CENTER, "Page up: Previous page.");	
		else									lineToRect(targetASCII, targetWidth, targetHeight, (int32_t)targetHeight-MENU_ROFFSET+2, posXOffset, klib::CENTER, "Page up: Previous page. Page down: Next page");	
	}

	template <size_t _FormatLen>
	static void drawExitOption(char* targetASCII, uint16_t* targetAttributes, size_t targetWidth, size_t targetHeight, uint32_t posXOffset, klib::ALIGN align, uint32_t rowWidth, const char (&formatString)[_FormatLen], const std::string& exitText ) {

		int32_t offsetY = (int32_t)targetHeight-MENU_ROFFSET;
		int32_t actualOffsetX = printfToRect(targetASCII, targetWidth, targetHeight, offsetY, posXOffset, align, formatString, "0", exitText.c_str());	

		uint16_t colorBkg = COLOR_GREEN; 
		valueToRect(targetAttributes, targetWidth, targetHeight,  offsetY, actualOffsetX, LEFT, &colorBkg, 1, (int32_t)exitText.size()+3);
	}

	template <size_t _ArraySize, typename _ReturnType>
	_ReturnType drawMenu(char* targetASCII, uint16_t* targetAttributes, size_t targetWidth, size_t targetHeight, size_t optionCount, const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ArraySize], const klib::SInput& frameInput, const _ReturnType& exitValue, const _ReturnType& noActionValue=-1, uint32_t rowWidth=20, bool disableEscKeyClose=false, const std::string& exitText="Exit this menu")
	{
		drawMenu_globals.Timer.Frame();

		optionCount = (optionCount < _ArraySize) ? optionCount : _ArraySize; // Fix optionCount to the maximum size of the array if optionCount is higher than the allowed size.

		int32_t lineOffset = (int32_t)(targetHeight-MENU_ROFFSET-3-std::min((int32_t)optionCount, 9));
		std::string clearString(rowWidth<<1, ' ');
		for(int32_t i=-1, count = (int32_t)targetHeight-lineOffset; i<count; ++i)
			klib::printfToRect(targetASCII, targetWidth, targetHeight, lineOffset+i, 0, klib::CENTER, "%s", clearString.c_str()); // clear all lines where we're going to draw

		
		const bool multipage = optionCount > 9;
		const uint32_t pageCount = (uint32_t)((multipage == false) ? 1 : optionCount/9 + ((optionCount%9)?1:0));

		static SDrawMenuLocalStatics localPersistentState;
		if( localPersistentState.CurrentPage >= pageCount )
			localPersistentState.CurrentPage = 0;

		const std::string textToPrint = "-- " + title + " --";
		rowWidth = std::max((uint32_t)textToPrint.size(), std::max((uint32_t)exitText.size()+3, rowWidth));
		const bool bDonePrinting = getMessageSlow(localPersistentState.SlowMessage, textToPrint, drawMenu_globals.Timer.LastTimeSeconds*4);
		int32_t actualOffsetX = klib::lineToRect(targetASCII, targetWidth, targetHeight, lineOffset, 0, klib::CENTER, localPersistentState.SlowMessage);		//"-- %s --", title.c_str() );	// Print menu title
		for(uint32_t i=0; i<rowWidth+1; i++)
			targetAttributes[lineOffset*targetWidth+actualOffsetX+i] = COLOR_GREEN;

		if( !bDonePrinting )
			return noActionValue;
		else if(drawMenu_globals.Accumulator.Value < 0.575)
			drawMenu_globals.Accumulator.Value = 0.575;

		lineOffset += 2;

		// Print menu options
		uint32_t numberCharsAvailable = rowWidth-4;	// 4 is for "%2.2s: "
		char formatString[24] = {};
		char numberKey[4] = {};
		sprintf_s(formatString, "%%2.2s: %%-%u.%us", numberCharsAvailable, numberCharsAvailable);

		// Draw options
		const uint32_t posXOffset = 0;
		uint32_t actualOptionCount = std::min(9U, (uint32_t)(optionCount-(localPersistentState.CurrentPage*9)));
		const size_t itemOffset = localPersistentState.CurrentPage*9;
		for(size_t i=0, count = (localPersistentState.MenuItemAccum < actualOptionCount) ? localPersistentState.MenuItemAccum : actualOptionCount; i<count; i++) 
		{
			sprintf_s(numberKey, "%u", (uint32_t)(i+1));
			actualOffsetX = klib::printfToRect(targetASCII, targetWidth, targetHeight, lineOffset, posXOffset, klib::CENTER, formatString, numberKey, menuItems[itemOffset+i].Text.c_str());
			for(uint32_t i=0; i<rowWidth+1; i++)
				targetAttributes[lineOffset*targetWidth+actualOffsetX+i] = COLOR_YELLOW;

			lineOffset++;
		}

		// Print Exit option at the end.
		if(localPersistentState.MenuItemAccum > actualOptionCount)
			drawExitOption(targetASCII, targetAttributes, targetWidth, targetHeight, posXOffset, klib::CENTER, rowWidth, formatString, exitText);

		// Print page control help if multipage.
		if(multipage) 
			printMultipageHelp(targetASCII, targetAttributes, targetWidth, targetHeight, localPersistentState.CurrentPage, pageCount, posXOffset);
		
		_ReturnType resultVal = noActionValue;
		bool bResetMenuStuff = false;
		if( drawMenu_globals.Accumulator.Accumulate(drawMenu_globals.Timer.LastTimeSeconds) )
		{
			// Don't process keys until the menu has finished displaying
			if(localPersistentState.MenuItemAccum <= actualOptionCount) 
			{
				drawMenu_globals.Accumulator.Value = 0.575;
				localPersistentState.MenuItemAccum++;
			}
			// Process page change keys first.
			else if(localPersistentState.CurrentPage < (pageCount-1) && frameInput.Keys[VK_NEXT]) 
			{
				bResetMenuStuff = true;
				localPersistentState.CurrentPage++;
			}
			else if(localPersistentState.CurrentPage > 0 && frameInput.Keys[VK_PRIOR]) 
			{
				bResetMenuStuff = true;
				localPersistentState.CurrentPage--;
			}
			else if(frameInput.Keys['0'] || frameInput.Keys[VK_NUMPAD0] || (frameInput.Keys[VK_ESCAPE] && !disableEscKeyClose)) 
			{
				bResetMenuStuff = true;

				localPersistentState.CurrentPage = 0;
				resultVal = exitValue;
			}
			else 
			{
				for(uint32_t i=0, count = (uint32_t)actualOptionCount; i < count; i++) 
					if(frameInput.Keys['1'+i] || frameInput.Keys[VK_NUMPAD1+i]) 
					{
						bResetMenuStuff = true;
						resultVal = menuItems[i+itemOffset].ReturnValue;
						break;
					}
			}
		}

		if(bResetMenuStuff)
		{
			localPersistentState.MenuItemAccum = actualOptionCount>>1;
			drawMenu_globals.Accumulator.Value = 0;
			resetCursorString(localPersistentState.SlowMessage);
		}

		return resultVal;
	}

	template <typename _CellType, size_t _Width, size_t _Depth, size_t _ItemCount, typename _ReturnType>
	inline _ReturnType drawMenu(SGrid<_CellType, _Width, _Depth>& display, uint16_t* targetAttributes, size_t optionCount, const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ItemCount], const klib::SInput& frameInput, _ReturnType exitValue, _ReturnType noActionValue = -1, uint32_t rowWidth=20, bool disableEscapeKey=false, const std::string& exitText="Exit this menu") {
		return drawMenu(&display.Cells[0][0], targetAttributes, _Width, _Depth, optionCount, title, menuItems, frameInput, exitValue, noActionValue, rowWidth, disableEscapeKey, exitText);
	}
	
	template <typename _CellType, size_t _Width, size_t _Depth, size_t _ItemCount, typename _ReturnType>
	inline _ReturnType drawMenu(SGrid<_CellType, _Width, _Depth>& display, uint16_t* targetAttributes, const std::string& title, const klib::SMenuItem<_ReturnType>(&menuItems)[_ItemCount], const klib::SInput& frameInput, _ReturnType exitValue, _ReturnType noActionValue = -1, uint32_t rowWidth=20, bool disableEscapeKey=false, const std::string& exitText="Exit this menu") {
		return drawMenu(&display.Cells[0][0], targetAttributes, _Width, _Depth, _ItemCount, title, menuItems, frameInput, exitValue, noActionValue, rowWidth, disableEscapeKey, exitText);
	}

	template <typename _ReturnType, size_t _ItemCount>
	struct SMenu
	{
		const std::string					Title;
		const uint32_t						RowWidth;
		const _ReturnType					ValueExit;
		const bool							bDisableEscapeKey;
		const std::string					TextExit;
		klib::SMenuItem<_ReturnType>		Items[_ItemCount];

		SMenu(const klib::SMenuItem<_ReturnType> (&options)[_ItemCount], _ReturnType exitValue, const std::string& title, size_t rowWidth=24, bool disableEscapeKey=false, const std::string& exitText="Exit this menu")
			:Title				(title)
			,RowWidth			((uint32_t)rowWidth)
			,ValueExit			(exitValue)
			,bDisableEscapeKey	(disableEscapeKey)
			,TextExit			(exitText)
			//,Items				()
		{
			for(uint32_t i=0; i<_ItemCount; ++i)
				Items[i] = options[i];
		};
	};

	template <typename _CellType, size_t _Width, size_t _Depth, size_t _ItemCount, typename _ReturnType>
	_ReturnType drawMenu(SGrid<_CellType, _Width, _Depth>& display, uint16_t* targetAttributes, const SMenu<_ReturnType, _ItemCount>& menuInstance, const klib::SInput& frameInput, _ReturnType noActionValue = -1) {
		return drawMenu(&display.Cells[0][0], targetAttributes, _Width, _Depth, (uint32_t)_ItemCount, menuInstance.Title, menuInstance.Items, frameInput, menuInstance.ValueExit, noActionValue, menuInstance.RowWidth, menuInstance.bDisableEscapeKey, menuInstance.TextExit);
	}

	template <typename _CellType, size_t _Width, size_t _Depth, size_t _ItemCount, typename _ReturnType>
	_ReturnType drawMenu(SGrid<_CellType, _Width, _Depth>& display, uint16_t* targetAttributes, const SMenu<_ReturnType, _ItemCount>& menuInstance, uint32_t optionCount, const klib::SInput& frameInput, _ReturnType noActionValue = -1) {
		return drawMenu(&display.Cells[0][0], targetAttributes, _Width, _Depth, optionCount, menuInstance.Title, menuInstance.Items, frameInput, menuInstance.ValueExit, noActionValue, menuInstance.RowWidth, menuInstance.bDisableEscapeKey, menuInstance.TextExit);
	}
} // namespace

#endif // __DRAWMENU_H__98237409236409872364987236498__
