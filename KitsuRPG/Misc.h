#pragma once

#include <string>
#include <iostream>

template <typename _T, size_t _Size> 
size_t size(const _T (&staticArray)[_Size]) { return _Size; }

static int64_t getNumericInput()
{
	// Get user input
	std::string userChoice;
	getline(std::cin, userChoice);
	
	// Convert the input string to a menuitem index.
	int64_t selectedOption = -1;	// this is the bits of 0 negated which results in 0xFFFFFFFF.
	try { selectedOption = std::stoll(userChoice); }	// stoul converts an std::string to an unsigned long
	catch(std::invalid_argument) {}	// this try/catch bullshit is required because std::stoi() throws exceptions if the input can't be converted to a number.

	return selectedOption;
}