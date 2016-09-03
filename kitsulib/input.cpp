#include "input.h"
#include "Misc.h"

#include <memory.h>
#include <cstdint>
#include <Windows.h>

void klib::pollInput(SInput& input)
{
	static const size_t keyCount = klib::size(input.Keys);
	memcpy(input.PreviousKeys, input.Keys, keyCount);
	for(uint32_t i=0; i<keyCount; i++)
		input.Keys[i] = GetAsyncKeyState(i) ? 1 : 0;

	static const size_t buttonCount = klib::size(input.MouseButtons);
	memcpy(input.PreviousMouseButtons, input.MouseButtons, buttonCount);
	//for(uint32_t i=0; i<8; i++)
	//	input.MouseButtons[i] = 
};

