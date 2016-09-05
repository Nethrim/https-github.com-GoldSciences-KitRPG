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

	INPUT_RECORD recordIn	= {};
	DWORD NumRead			= 0;

	static HANDLE handleIn = GetStdHandle(STD_INPUT_HANDLE);

	DWORD	eventCount=0;
	GetNumberOfConsoleInputEvents (handleIn, &eventCount);
	if( 0 == eventCount )
		return;

	ReadConsoleInput(handleIn, &recordIn, 1, &NumRead);

	switch (recordIn.EventType)
	{
	//case KEY_EVENT:
	//			// Do stuff
	//	break;

	case MOUSE_EVENT:
		input.MouseX = recordIn.Event.MouseEvent.dwMousePosition.X;
		input.MouseY = recordIn.Event.MouseEvent.dwMousePosition.Y;
		input.MouseButtons[0] = (recordIn.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED	);
		input.MouseButtons[1] = (recordIn.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED	);
		input.MouseButtons[2] = (recordIn.Event.MouseEvent.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED	);
		input.MouseButtons[3] = (recordIn.Event.MouseEvent.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED	);
		input.MouseButtons[4] = (recordIn.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED		);
		break;
	}
	for(uint32_t i=0; i<klib::size(input.MouseButtons); ++i)
		if( input.MouseButtons[i] != input.PreviousMouseButtons[i] )
			Beep(220+220*i, 35);



	//for(uint32_t i=0; i<8; i++)
	//	input.MouseButtons[i] = 
};

