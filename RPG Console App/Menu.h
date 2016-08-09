#pragma once

#include "Misc.h"

#include <algorithm>

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
		for(int i=0; i<optionCount; i++)
			printf("%u: %s.\n", i+1, menuItems[i].Text.c_str());	
		
		// Get user input
		// Convert the input string to a menuitem index.
		const unsigned int selectedIndex = getNumericInput()-1;	// 

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

