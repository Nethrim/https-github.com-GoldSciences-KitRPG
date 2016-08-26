#include <string>
#include <iostream>

#ifndef __MISC_H__982143092634987160987328097642398__
#define __MISC_H__982143092634987160987328097642398__

namespace klib
{
#pragma pack(push, 1)

	template <typename _T, size_t _Size> 
	inline constexpr size_t size(const _T (&staticArray)[_Size]) { return _Size; }

	// returns -1 if the user entered an invalid numeric input
	static int64_t getNumericInput()
	{
		// Get user input
		std::string userChoice;
		getline(std::cin, userChoice);
	
		// Convert the input string to a menuitem index.
		int64_t selectedOption = -1;
		try { selectedOption = std::stoll(userChoice); }
		catch(std::invalid_argument) {}	// this try/catch bullshit is required because std::stoi() throws exceptions if the input can't be converted to a number.

		return selectedOption;
	}

#pragma pack(pop)
}

#endif // __MISC_H__982143092634987160987328097642398__