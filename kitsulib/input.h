#include <vector>

#ifndef __INPUT_H__9263487236498723649213640918273098__
#define __INPUT_H__9263487236498723649213640918273098__

namespace klib
{
	struct IInputHandler
	{
		virtual ~IInputHandler(){};

		virtual void	OnKeyUp				(uint8_t key) {};
		virtual void	OnKeyDown			(uint8_t key) {};
		virtual void	OnMouseButtonUp		(uint8_t key) {};
		virtual void	OnMouseButtonDown	(uint8_t key) {};
		virtual void	OnMouseMove			(int32_t x, int32_t y, int32_t z) {};
	};


	struct SInput
	{
		uint8_t Keys[256];
		uint8_t PreviousKeys[256];

		uint8_t MouseButtons[8];
		uint8_t PreviousMouseButtons[8];

		uint32_t MouseX;
		uint32_t MouseY;
	};

	void pollInput(SInput& input);
	
} // namespace
#endif // __INPUT_H__9263487236498723649213640918273098__
