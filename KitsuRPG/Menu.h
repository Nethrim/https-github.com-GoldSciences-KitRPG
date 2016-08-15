#include "Misc.h"

#include <algorithm>

#ifndef __MENU_H__926349827164392740982169862598423__
#define __MENU_H__926349827164392740982169862598423__

class SMenuItem
{
public:
	unsigned int	ReturnValue;
	std::string		Text;
};

template <size_t _ArraySize>
int displayMenu(size_t optionCount, const std::string& title, const SMenuItem (&menuItems)[_ArraySize])
{
	optionCount = (_ArraySize > optionCount) ? optionCount : _ArraySize; // Fix optionCount to the maximum size of the array if optionCount is higher than the allowed size.

	while (true)	// Query for user input until a valid selection is made
	{
		printf(	"\n-- %s.\n", title.c_str() );	// Print menu title
		printf(	"Make your selection:\n" );
		
		// Print menu options
		for(size_t i=0; i<optionCount; i++)
			printf("%u: %s.\n", i+1, menuItems[i].Text.c_str());	
		
		// Get user input
		// Convert the input string to a menuitem index.
		const uint32_t selectedIndex = (uint32_t)(getNumericInput()-1);	// 

		if(selectedIndex >= optionCount)	// We only accept from 0 to optionCount
			printf("Invalid answer. Answer again...\n");
		else
		{
			printf("\n");
			return menuItems[selectedIndex].ReturnValue;
		}
	}
}

template <size_t _Size>
int displayMenu(const std::string& title, const SMenuItem (&menuItems)[_Size]) {
	return displayMenu(_Size, title, menuItems);
}

#endif // __MENU_H__926349827164392740982169862598423__