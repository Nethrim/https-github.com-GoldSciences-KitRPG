#pragma once

#include <string>
#include <iostream>

static unsigned int getNumericInput()
{
	// Get user input
	std::string userChoice;
	getline(std::cin, userChoice);
	
	// Convert the input string to a menuitem index.
	unsigned int selectedOption = ~0U;	// this is the bits of 0 negated which results in 0xFFFFFFFF.
	try { selectedOption = std::stoul(userChoice); }	// stoul converts an std::string to an unsigned long
	catch(std::invalid_argument) {}	// this try/catch bullshit is required because std::stoi() throws exceptions if the input can't be converted to a number.

	return selectedOption;
}