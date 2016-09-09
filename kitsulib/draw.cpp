#include "draw.h"

#include "credits.h"

using namespace klib;

void drawIntro(SGame& instanceGame);

// Currently what this function is lacking is the ability to receive negative offsets.
template<typename _TCell, size_t _Width, size_t _Depth>
void blitGrid(SGrid<_TCell, _Width, _Depth>& source, int32_t offsetY, uint32_t offsetX, _TCell* target, size_t targetWidth, size_t targetHeight, int32_t rowPitch=-1)
{
	size_t actualWidth = std::min(_Width, std::max((size_t)0, targetWidth-offsetX));
	if(rowPitch < 0)
		rowPitch = (int32_t)targetWidth;

	for(int32_t z = 0, maxZ = (int32_t)std::min(_Depth-1, targetHeight-offsetY); z < maxZ; ++z) 
	{
		if( (offsetY+z) < 0 )
			continue;

		memcpy(&target[(offsetY+z)*rowPitch+offsetX], &source.Cells[z][0], actualWidth);
	}
};

template<typename _TCell, size_t _Width, size_t _Depth>
void drawDisplay(SGrid<_TCell, _Width, _Depth>& source, uint32_t offsetY, uint32_t offsetX ) {
	blitGrid(source, offsetY, offsetX, getASCIIBackBuffer(), getASCIIBackBufferWidth(), getASCIIBackBufferHeight());
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
	case GAME_STATE_MENU_EQUIPMENT: 
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
	case GAME_STATE_MENU_EQUIPMENT: 
		break;
	default:
		for(y=0; y<instanceGame.PostEffectDisplay.TextAttributes.Depth; ++y)
			memcpy(&getASCIIColorBackBuffer()[(TACTICAL_DISPLAY_YPOS+y)*bbWidth+(bbWidth/2-instanceGame.PostEffectDisplay.TextAttributes.Width/2)], &instanceGame.PostEffectDisplay.TextAttributes.Cells[y][0], instanceGame.PostEffectDisplay.TextAttributes.Width*sizeof(uint16_t));
	}

	// Frame timer
	frameMeasure.Frame();
	instanceGame.FrameTimer.Frame();

	size_t	offset;

	// Print some debugging information 
	lineToScreen(1, 1, LEFT, "Frame time: %.5f seconds.", instanceGame.FrameTimer.LastTimeSeconds);
	lineToScreen(2, 1, LEFT, "Frames last second: %f.", instanceGame.FrameTimer.FramesLastSecond);
	lineToScreen(bbHeight-2, 1, RIGHT, "%s.", instanceGame.StateMessage.c_str());
	lineToScreen(bbHeight-3, 1, LEFT, "sizeof(SGame): %u.", sizeof(SGame));
	lineToScreen(bbHeight-2, 1, LEFT, "sizeof(CCharacter): %u.", sizeof(CCharacter));
	for(uint32_t i=0, count = 32U; i<count; i++) {
		offset = 1*bbWidth;	getASCIIColorBackBuffer()[offset+i] = COLOR_GREEN;
		offset = 2*bbWidth;	getASCIIColorBackBuffer()[offset+i] = COLOR_CYAN;
		offset = (bbHeight-2)*bbWidth;	getASCIIColorBackBuffer()[offset+(bbWidth-1-i)] = COLOR_DARKYELLOW;
		offset = (bbHeight-3)*bbWidth;	getASCIIColorBackBuffer()[offset+i] = COLOR_DARKGREEN;
		offset = (bbHeight-2)*bbWidth;	getASCIIColorBackBuffer()[offset+i] = COLOR_DARKMAGENTA;
	}

	// Print user error messages and draw cursor.
	if(instanceGame.State.State != GAME_STATE_CREDITS) {
		lineToScreen(bbHeight-3, 1, CENTER, "%s", instanceGame.UserMessage.c_str());
		lineToScreen(bbHeight-2, 1, CENTER, "%s", instanceGame.UserError.c_str());
		offset = (bbHeight-3)*bbWidth+bbWidth/2-instanceGame.UserMessage	.size()/2;	for(size_t i=0, count = instanceGame.UserMessage.size()+1; i<count; i++)	getASCIIColorBackBuffer()[offset+i] = COLOR_GREEN;
		offset = (bbHeight-2)*bbWidth+bbWidth/2-instanceGame.UserError		.size()/2;	for(size_t i=0, count = instanceGame.UserError	.size()+1; i<count; i++)	getASCIIColorBackBuffer()[offset+i] = COLOR_RED;
		// Draw cursor
		lineToScreen(instanceGame.FrameInput.MouseY, instanceGame.FrameInput.MouseX, LEFT, "\x8");
		getASCIIColorBackBuffer()[instanceGame.FrameInput.MouseY*bbWidth+instanceGame.FrameInput.MouseX] = COLOR_MAGENTA;
	}

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
		uint32_t offsetX = printfToGrid(instanceGame.PostEffectDisplay.Screen, offsetY, 0, CENTER, "%s", words[i].c_str());
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
	columnOffset = printfToGrid(instanceGame.GlobalDisplay.Screen, lineOffset, columnOffset, klib::LEFT, "%s", instanceGame.SlowMessage);
	for(size_t i=0, charCount = textToPrint.size()+2; i<charCount; i++) instanceGame.GlobalDisplay.TextAttributes.Cells[lineOffset][columnOffset+i] = COLOR_GREEN;
	if ( bDonePrinting ) {
		static SMenu<SGameState, size(optionsControlCenter)> menuControlCenter(optionsControlCenter, {GAME_STATE_MENU_MAIN}, "Options", 28);
		return drawMenu(instanceGame.GlobalDisplay.Screen, &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], menuControlCenter, instanceGame.FrameInput, returnValue);
	}
	return returnValue;
};
