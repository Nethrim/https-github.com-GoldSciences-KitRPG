#include "draw.h"

#include "credits.h"

using namespace klib;

void drawIntro(SGame& instanceGame);

template<typename _TCell, size_t _Width, size_t _Depth>
void drawDisplay(SGrid<_TCell, _Width, _Depth>& source, uint32_t offsetY, uint32_t offsetX )
{
	int32_t	  bbWidth	= getASCIIBackBufferWidth()
			, bbHeight	= getASCIIBackBufferHeight();
	for(uint32_t z = 0, maxZ=_Depth; z<maxZ; ++z) {
		memcpy(&getASCIIBackBuffer()[(offsetY+z)*bbWidth+offsetX], &source.Cells[z][0], std::min(_Width, (size_t)bbWidth-offsetX) );
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

	drawDisplay(instanceGame.GlobalDisplay.Screen, 0, 0);
	//drawDisplay(instanceGame.MenuDisplay, 0);
	switch(instanceGame.State.State) { 
	case GAME_STATE_CREDITS:
		drawCredits(instanceGame.FrameTimer.LastTimeSeconds, namesSpecialThanks, instanceGame.State);
	case GAME_STATE_WELCOME_COMMANDER: 
	case GAME_STATE_MENU_SQUAD_SETUP: 
		break;
	default:
		drawDisplay(instanceGame.PostEffectDisplay.Screen, 5, instanceGame.GlobalDisplay.Screen.Width/2-instanceGame.PostEffectDisplay.Width/2);
	}


	memcpy(getASCIIColorBackBuffer(), &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], instanceGame.GlobalDisplay.TextAttributes.Width*instanceGame.GlobalDisplay.TextAttributes.Depth*sizeof(uint16_t));

	uint32_t y=0;
	uint32_t  bbWidth	= getASCIIBackBufferWidth()
			, bbHeight	= getASCIIBackBufferHeight();

	switch(instanceGame.State.State) { 
	case GAME_STATE_CREDITS:
	case GAME_STATE_WELCOME_COMMANDER: 
	case GAME_STATE_MENU_SQUAD_SETUP: 
		break;
	default:
		for(y=0; y<instanceGame.PostEffectDisplay.TextAttributes.Depth; ++y)
			memcpy(&getASCIIColorBackBuffer()[(TACTICAL_DISPLAY_YPOS+y)*bbWidth+(bbWidth/2-instanceGame.PostEffectDisplay.TextAttributes.Width/2)], &instanceGame.PostEffectDisplay.TextAttributes.Cells[y][0], instanceGame.PostEffectDisplay.TextAttributes.Width*sizeof(uint16_t));
	}

	// Frame timer
	frameMeasure.Frame();
	instanceGame.FrameTimer.Frame();

	// Print user error messages
	lineToScreen(bbHeight-3, 1, CENTER, "%s", instanceGame.UserMessage.c_str());
	lineToScreen(bbHeight-2, 1, CENTER, "%s", instanceGame.UserError.c_str());
	for(uint32_t i=0, count = instanceGame.UserMessage.size()+1; i<count; i++)	getASCIIColorBackBuffer()[(bbHeight-3)*bbWidth+bbWidth/2-instanceGame.UserMessage	.size()/2+i] = COLOR_GREEN;
	for(uint32_t i=0, count = instanceGame.UserError.size()+1; i<count; i++)	getASCIIColorBackBuffer()[(bbHeight-2)*bbWidth+bbWidth/2-instanceGame.UserError		.size()/2+i] = COLOR_RED;

	// Print some debugging information 
	lineToScreen(1, 1, LEFT, "Frame time: %.5f seconds.", instanceGame.FrameTimer.LastTimeSeconds);
	lineToScreen(2, 1, LEFT, "Frames last second: %f.", instanceGame.FrameTimer.FramesLastSecond);
	lineToScreen(getASCIIBackBufferHeight()-2, 1, RIGHT, "%s.", instanceGame.StateMessage.c_str());
	lineToScreen(getASCIIBackBufferHeight()-2, 1, LEFT, "sizeof(SGame): %u.", sizeof(SGame));
	for(uint32_t i=0, count = 32U; i<count; i++) {
		getASCIIColorBackBuffer()[1*bbWidth+i] = COLOR_GREEN;
		getASCIIColorBackBuffer()[2*bbWidth+i] = COLOR_CYAN;
		getASCIIColorBackBuffer()[(bbHeight-2)*bbWidth+i] = COLOR_DARKMAGENTA;
		getASCIIColorBackBuffer()[(bbHeight-2)*bbWidth+(bbWidth-1-i)] = COLOR_DARKYELLOW;
	}

	lineToScreen(instanceGame.FrameInput.MouseY, instanceGame.FrameInput.MouseX, LEFT, "\x8");
	getASCIIColorBackBuffer()[instanceGame.FrameInput.MouseY*bbWidth+instanceGame.FrameInput.MouseX] = COLOR_MAGENTA;
	//lineToScreen(instanceGame.FrameInput.MouseY, instanceGame.FrameInput.MouseX, LEFT, "\x21");
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
	const std::string textToPrint = "Welcome back commander " + instanceGame.Player.Name + ".";
	int32_t lineOffset		= (instanceGame.GlobalDisplay.Screen.Depth/2-1);
	int32_t columnOffset	= instanceGame.GlobalDisplay.Screen.Width/2-(int32_t)textToPrint.size()/2;

	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
	columnOffset = lineToGrid(instanceGame.GlobalDisplay.Screen, lineOffset, columnOffset, klib::LEFT, "%s", instanceGame.SlowMessage);
	for(size_t i=0, charCount = textToPrint.size()+2; i<charCount; i++)
		instanceGame.GlobalDisplay.TextAttributes.Cells[lineOffset][columnOffset+i] = COLOR_GREEN;
	if ( bDonePrinting ) {
		static SMenu<SGameState, size(optionsControlCenter)> menuControlCenter(optionsControlCenter, {GAME_STATE_MENU_MAIN}, "Options", 28);
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuControlCenter, instanceGame.FrameInput, returnValue);
	}
	return returnValue;
};
