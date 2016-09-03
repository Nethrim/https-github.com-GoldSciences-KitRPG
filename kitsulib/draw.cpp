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

	clearASCIIBackBuffer( ' ' );

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

	frameMeasure.Frame();
	instanceGame.FrameTimer.Frame();
	lineToScreen(1, 1, LEFT, "Frame time: %.5f seconds.", instanceGame.FrameTimer.LastTimeSeconds);
	lineToScreen(2, 1, LEFT, "Frames last second: %f.", instanceGame.FrameTimer.FramesLastSecond);
	lineToScreen(getASCIIBackBufferHeight()-2, 1, RIGHT, "%s.", instanceGame.UserMessage.c_str());
	lineToScreen(getASCIIBackBufferHeight()-2, 1, LEFT, "sizeof(SGame): %u.", sizeof(SGame));
	
	presentASCIIBackBuffer();
}; 	// 


void drawIntro( SGame& instanceGame )
{
	drawFireBackground(instanceGame.PostEffectDisplay, instanceGame.FrameTimer.LastTimeSeconds);

	int32_t displayWidth	= (int32_t)instanceGame.PostEffectDisplay.Width;
	int32_t displayDepth	= (int32_t)instanceGame.PostEffectDisplay.Depth;

	static std::string words[] = {"Vulgar", "Display", "of", "Power"};
	for( uint32_t i=0; i<klib::size(words); ++i)
		lineToGrid(instanceGame.PostEffectDisplay.Screen, (int32_t)((displayDepth/2)-(klib::size(words)/2)+i*2), 0, CENTER, "%s", words[i].c_str());
};


SGameState drawWelcome(SGame& instanceGame, const SGameState& returnValue)
{
	const std::string textToPrint = "Welcome back commander " + instanceGame.PlayerName + ".";
	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
	lineToGrid(instanceGame.GlobalDisplay.Screen, instanceGame.GlobalDisplay.Screen.Depth/2-1, instanceGame.GlobalDisplay.Screen.Width/2-(int32_t)textToPrint.size()/2, klib::LEFT, "%s", instanceGame.SlowMessage);
	if ( bDonePrinting ) {
		static SMenu<SGameState, size(optionsControlCenter)> menuControlCenter(optionsControlCenter, {GAME_STATE_MENU_MAIN}, "Options", 28);
		return drawMenu(instanceGame.GlobalDisplay.Screen, menuControlCenter, instanceGame.FrameInput, returnValue);
	}
	return returnValue;
};
