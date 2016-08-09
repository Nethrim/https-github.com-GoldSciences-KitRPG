#pragma once
#include <string>
#include <iostream>
#include <algorithm>

class SMenuItem
{
public:
	int			ReturnValue;
	std::string	Text;
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
		std::string userChoice;
		getline(std::cin, userChoice);
	
		// Convert the input string to a menuitem index.
		unsigned int selectedOption = ~0U;	// this is the bits of 0 negated which results in 0xFFFFFFFF.
		try { selectedOption = std::stoul(userChoice)-1; }	// stoul converts an std::string to an unsigned long
		catch(std::invalid_argument) {}	// this try/catch bullshit is required because std::stoi() throws exceptions if the input can't be converted to a number.

		if(selectedOption >= optionCount)	// We only accept from 0 to 
			printf("Invalid answer. Answer again...\n");
		else
		{
			printf("\n");
			return menuItems[selectedOption].ReturnValue;
		}
	}
}

template <size_t _Size>
int displayMenu(const std::string& title, const SMenuItem (&menuItems)[_Size]) {
	return displayMenu(_Size, title, menuItems);
}

