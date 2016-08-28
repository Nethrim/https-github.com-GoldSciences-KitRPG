#ifndef __INPUT_H__9263487236498723649213640918273098__
#define __INPUT_H__9263487236498723649213640918273098__


struct SInput
{
	char Keys[256];
	char PreviousKeys[256];

	char MouseButtons[8];
	char PreviousMouseButtons[8];
};

void pollInput(SInput& input);


#endif // __INPUT_H__9263487236498723649213640918273098__
