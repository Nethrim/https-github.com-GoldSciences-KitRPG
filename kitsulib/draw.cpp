#include "draw.h"

#include "credits.h"

using namespace klib;

void drawIntro(SGame& instanceGame);

template<typename _TCell, size_t _Width, size_t _Depth>
void drawDisplay(SGrid<_TCell, _Width, _Depth>& source, uint32_t offset)	
{
	for(uint32_t z=0, maxZ=std::min(source.Depth, getASCIIBackBufferHeight()-1-offset); z<maxZ; ++z) {
		char row[_Width+1] = {};
		memcpy(row, source.Cells[z], std::min((uint32_t)_Width, getASCIIBackBufferWidth()-1));
		lineToScreen(offset, 0, CENTER, row);
		++offset;
	}
};

void drawStateBackground( SGame& instanceGame )
{
	switch(instanceGame.State.State) {
	case	GAME_STATE_MENU_MAIN		:	drawIntro(instanceGame);																	; break;
	case	GAME_STATE_CREDITS			:	drawSnowBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds);	; break;
	case	GAME_STATE_MENU_SQUAD_SETUP	:	drawRainBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds);	; break;
	}
}

void klib::drawAndPresentGame( SGame& instanceGame )
{
	static STimer frameMeasure;

	clearASCIIBackBuffer(' ', COLOR_WHITE);

	drawStateBackground(instanceGame);
	showMenu(instanceGame);

	drawDisplay(instanceGame.GlobalDisplay.Screen, 0);
	//drawDisplay(instanceGame.MenuDisplay, 0);
	switch(instanceGame.State.State) { 
	case GAME_STATE_CREDITS:
		drawCredits(instanceGame.FrameTimer.LastTimeSeconds, namesSpecialThanks, instanceGame.State);
	case GAME_STATE_WELCOME_COMMANDER: 
	case GAME_STATE_MENU_SQUAD_SETUP: 
		break;
	default:
		drawDisplay(instanceGame.PostEffectDisplay.Screen, 5);
	}


	memcpy(getASCIIColorBackBuffer(), &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], instanceGame.GlobalDisplay.TextAttributes.Width*instanceGame.GlobalDisplay.TextAttributes.Depth*sizeof(uint16_t));

	uint32_t y=0;
	switch(instanceGame.State.State) { 
	case GAME_STATE_CREDITS:
	case GAME_STATE_WELCOME_COMMANDER: 
	case GAME_STATE_MENU_SQUAD_SETUP: 
		break;
	default:
		for(y=0; y<instanceGame.PostEffectDisplay.TextAttributes.Depth; ++y)
			memcpy(&getASCIIColorBackBuffer()[(TACTICAL_DISPLAY_YPOS+y)*getASCIIBackBufferWidth()+(getASCIIBackBufferWidth()/2-instanceGame.PostEffectDisplay.TextAttributes.Width/2)], &instanceGame.PostEffectDisplay.TextAttributes.Cells[y][0], instanceGame.PostEffectDisplay.TextAttributes.Width*sizeof(uint16_t));
	}
		
	lineToScreen(instanceGame.FrameInput.MouseY, instanceGame.FrameInput.MouseX, LEFT, "\x8");
	//lineToScreen(instanceGame.FrameInput.MouseY, instanceGame.FrameInput.MouseX, LEFT, "\x21");

	frameMeasure.Frame();
	instanceGame.FrameTimer.Frame();
	lineToScreen(1, 1, LEFT, "Frame time: %.5f seconds.", instanceGame.FrameTimer.LastTimeSeconds);
	lineToScreen(2, 1, LEFT, "Frames last second: %f.", instanceGame.FrameTimer.FramesLastSecond);
	lineToScreen(getASCIIBackBufferHeight()-2, 1, RIGHT, "%s.", instanceGame.UserMessage.c_str());
	lineToScreen(getASCIIBackBufferHeight()-2, 1, LEFT, "sizeof(SGame): %u.", sizeof(SGame));
	for(uint32_t i=0; i<36; i++) {
		getASCIIColorBackBuffer()[1*getASCIIBackBufferWidth()+i] = COLOR_GREEN;
		getASCIIColorBackBuffer()[2*getASCIIBackBufferWidth()+i] = COLOR_CYAN;
		getASCIIColorBackBuffer()[(getASCIIBackBufferHeight()-2)*getASCIIBackBufferWidth()+i] = COLOR_DARKMAGENTA;
		getASCIIColorBackBuffer()[(getASCIIBackBufferHeight()-2)*getASCIIBackBufferWidth()+(getASCIIBackBufferWidth()-1-i)] = COLOR_DARKYELLOW;
	}

	presentASCIIBackBuffer();
}; 	// 


void drawIntro( SGame& instanceGame )
{
	drawFireBackground(instanceGame.PostEffectDisplay, instanceGame.FrameTimer.LastTimeSeconds);

	int32_t displayWidth	= (int32_t)instanceGame.PostEffectDisplay.Width;
	int32_t displayDepth	= (int32_t)instanceGame.PostEffectDisplay.Depth;

	static std::string words[] = {"Vulgar", "Display", "of", "Power"};
	for( uint32_t i=0; i<klib::size(words); ++i) {
		uint32_t offsetY = (uint32_t)((displayDepth/2)-(klib::size(words)/2)+i*2);
		uint32_t offsetX = lineToGrid(instanceGame.PostEffectDisplay.Screen, offsetY, 0, CENTER, "%s", words[i].c_str());
		for( size_t j=0, wordLen = words[i].size(); j<wordLen; j++ )
			instanceGame.PostEffectDisplay.TextAttributes.Cells[offsetY][offsetX+j] = COLOR_ORANGE;
	}
};


SGameState drawWelcome(SGame& instanceGame, const SGameState& returnValue)
{
	const std::string textToPrint = "Welcome back commander " + instanceGame.PlayerName + ".";
	int32_t lineOffset		= (instanceGame.GlobalDisplay.Screen.Depth/2-1);
	int32_t columnOffset	= instanceGame.GlobalDisplay.Screen.Width/2-(int32_t)textToPrint.size()/2;
	for(size_t i=0, charCount = textToPrint.size()+1; i<charCount; i++)
		instanceGame.GlobalDisplay.TextAttributes.Cells[lineOffset][columnOffset+i] = COLOR_DARKGREEN;

	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
	lineToGrid(instanceGame.GlobalDisplay.Screen, lineOffset, columnOffset, klib::LEFT, "%s", instanceGame.SlowMessage);
	if ( bDonePrinting ) {
		static SMenu<SGameState, size(optionsControlCenter)> menuControlCenter(optionsControlCenter, {GAME_STATE_MENU_MAIN}, "Options", 28);
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuControlCenter, instanceGame.FrameInput, returnValue);
	}
	return returnValue;
};
