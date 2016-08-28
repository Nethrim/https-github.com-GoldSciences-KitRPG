#include "input.h"

#include <memory.h>
#include <cstdint>
#include <Windows.h>

void pollInput(SInput& input)
{
	Sleep(1);
	memcpy(input.PreviousKeys, input.Keys, 256);
	for(uint32_t i=0; i<256; i++)
		input.Keys[i] = GetAsyncKeyState(i) ? 1 : 0;

	memcpy(input.PreviousMouseButtons, input.MouseButtons, 8);
	//for(uint32_t i=0; i<8; i++)
	//	input.MouseButtons[i] = 
};
