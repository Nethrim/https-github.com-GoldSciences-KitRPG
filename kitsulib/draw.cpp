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

	for(int32_t z = 0, maxZ = (int32_t)std::min(_Depth, targetHeight-offsetY); z < maxZ; ++z) 
	{
		if( (offsetY+z) < 0 )
			continue;

		memcpy(&target[(offsetY+z)*rowPitch+offsetX], &source.Cells[z][0], actualWidth);
	}
};

template<typename _TCell, size_t _Width, size_t _Depth>
void drawDisplay(SGrid<_TCell, _Width, _Depth>& source, uint32_t offsetY, uint32_t offsetX ) {
	blitGrid(source, offsetY, offsetX, ktools::getASCIIBackBuffer(), ktools::getASCIIBackBufferWidth(), ktools::getASCIIBackBufferHeight());
};

void drawStateBackground( SGame& instanceGame )
{
	switch(instanceGame.State.State) {
	case	GAME_STATE_MENU_MAIN		:	drawIntro(instanceGame);																	; break;
	case	GAME_STATE_CREDITS			:	drawSnowBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds);	; break;
	case	GAME_STATE_MENU_SQUAD_SETUP	:	drawRainBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds);	; break;
	}
}

template <typename... _Args>
int32_t lineToScreen( int32_t offsetLine, int32_t offsetColumn, ktools::ALIGN align, const char* format, _Args... args ) {
	return printfToRect(ktools::getASCIIBackBuffer(), (size_t)ktools::getASCIIBackBufferWidth(), (size_t)ktools::getASCIIBackBufferHeight(), offsetLine, offsetColumn, align, format, args...);
}

void klib::drawAndPresentGame( SGame& instanceGame )
{
	static ktools::STimer frameMeasure;

	ktools::clearASCIIBackBuffer(' ', COLOR_WHITE);

	drawStateBackground(instanceGame);
	showMenu(instanceGame);

	drawDisplay(instanceGame.GlobalDisplay.Screen, 0, 0);
	uint32_t  bbWidth	= ktools::getASCIIBackBufferWidth()
			, bbHeight	= ktools::getASCIIBackBufferHeight();
	//drawDisplay(instanceGame.MenuDisplay, 0);
	switch(instanceGame.State.State) { 
	case GAME_STATE_MENU_ACTION:
	case GAME_STATE_START_MISSION:
		drawDisplay(instanceGame.TacticalDisplay.Screen, TACTICAL_DISPLAY_POSY, (instanceGame.GlobalDisplay.Screen.Width>>1)-(instanceGame.TacticalDisplay.Width>>1));
		break;
	case GAME_STATE_CREDITS:
		drawCredits(ktools::getASCIIBackBuffer(), bbWidth, bbHeight, instanceGame.FrameTimer.LastTimeSeconds, namesSpecialThanks, instanceGame.State);
	case GAME_STATE_WELCOME_COMMANDER: 
	case GAME_STATE_MENU_SQUAD_SETUP: 
	case GAME_STATE_MENU_EQUIPMENT: 
		break;
	default:
		drawDisplay(instanceGame.PostEffectDisplay.Screen, TACTICAL_DISPLAY_POSY, (instanceGame.GlobalDisplay.Screen.Width>>1)-(instanceGame.PostEffectDisplay.Width>>1));
	}

	memcpy(ktools::getASCIIColorBackBuffer(), &instanceGame.GlobalDisplay.TextAttributes.Cells[0][0], instanceGame.GlobalDisplay.TextAttributes.Width*instanceGame.GlobalDisplay.TextAttributes.Depth*sizeof(uint16_t));
	uint32_t y=0;

	switch(instanceGame.State.State) { 
	case GAME_STATE_MENU_ACTION:
	case GAME_STATE_START_MISSION:
		for(y=0; y<instanceGame.PostEffectDisplay.TextAttributes.Depth; ++y)
			memcpy(&ktools::getASCIIColorBackBuffer()[(TACTICAL_DISPLAY_POSY+y)*bbWidth+((bbWidth>>1)-(instanceGame.TacticalDisplay.TextAttributes.Width>>1))], &instanceGame.TacticalDisplay.TextAttributes.Cells[y][0], instanceGame.TacticalDisplay.TextAttributes.Width*sizeof(uint16_t));
		break;
	case GAME_STATE_CREDITS:
	case GAME_STATE_WELCOME_COMMANDER: 
	case GAME_STATE_MENU_SQUAD_SETUP: 
	case GAME_STATE_MENU_EQUIPMENT: 
		break;
	default:
		for(y=0; y<instanceGame.PostEffectDisplay.TextAttributes.Depth; ++y)
			memcpy(&ktools::getASCIIColorBackBuffer()[(TACTICAL_DISPLAY_POSY+y)*bbWidth+((bbWidth>>1)-(instanceGame.PostEffectDisplay.TextAttributes.Width>>1))], &instanceGame.PostEffectDisplay.TextAttributes.Cells[y][0], instanceGame.PostEffectDisplay.TextAttributes.Width*sizeof(uint16_t));
	}

	// Frame timer
	frameMeasure.Frame();
	instanceGame.FrameTimer.Frame();

	int32_t actualOffsetX;
	uint16_t color;
	// Print some debugging information 
	actualOffsetX = lineToScreen(bbHeight-4, 1, ktools::LEFT, "Mouse: %i, %i.", instanceGame.FrameInput.MouseX, instanceGame.FrameInput.MouseY);	ktools::valueToRect(ktools::getASCIIColorBackBuffer(), bbWidth, bbHeight, bbHeight-4, actualOffsetX, ktools::LEFT, &(color = COLOR_DARKGREEN	), 1, 20);
	actualOffsetX = lineToScreen(bbHeight-3, 1, ktools::LEFT, "Frame time: %.5f seconds.", instanceGame.FrameTimer.LastTimeSeconds);				ktools::valueToRect(ktools::getASCIIColorBackBuffer(), bbWidth, bbHeight, bbHeight-3, actualOffsetX, ktools::LEFT, &(color = COLOR_GREEN		), 1, 32);
	actualOffsetX = lineToScreen(bbHeight-2, 1, ktools::LEFT, "Frames last second: %f.", instanceGame.FrameTimer.FramesLastSecond);					ktools::valueToRect(ktools::getASCIIColorBackBuffer(), bbWidth, bbHeight, bbHeight-2, actualOffsetX, ktools::LEFT, &(color = COLOR_CYAN			), 1, 32);
	time_t curTimeWithUnreliableSize = 0;
	{
		god::CGLock thelock(instanceGame.ServerTimeMutex);
		curTimeWithUnreliableSize = instanceGame.ServerTime;
	}
	
	char send_buffer[128] = {};
	ctime_s(send_buffer, sizeof(send_buffer), &curTimeWithUnreliableSize);
	std::string serverTime = std::string("Server time: ") + send_buffer;
	serverTime = serverTime.substr(0, serverTime .size()-2);
	actualOffsetX = lineToScreen(bbHeight-3, 1, ktools::RIGHT, "%s.", serverTime.c_str());	
	ktools::valueToRect(ktools::getASCIIColorBackBuffer(), bbWidth, bbHeight, bbHeight-3, actualOffsetX, ktools::LEFT, &(color = COLOR_CYAN), 1, (int32_t)serverTime .size());
	actualOffsetX = lineToScreen(bbHeight-2, 1, ktools::RIGHT, "%s.", instanceGame.StateMessage.c_str()); ktools::valueToRect(ktools::getASCIIColorBackBuffer(), bbWidth, bbHeight, bbHeight-2, actualOffsetX, ktools::LEFT, &(color = COLOR_DARKYELLOW	), 1, (int32_t)instanceGame.StateMessage.size());

	// Print user error messages and draw cursor.
	if(instanceGame.State.State != GAME_STATE_CREDITS) {
		uint16_t color = COLOR_YELLOW;
		lineToScreen(bbHeight-4, 1, ktools::RIGHT, "Funds: %i.", instanceGame.Players[PLAYER_USER].Money);
		ktools::valueToRect(ktools::getASCIIColorBackBuffer(), bbWidth, bbHeight, bbHeight-4, 21, ktools::RIGHT, &color, 1, 20);

		lineToScreen(bbHeight-3, 0, ktools::CENTER, "%s", instanceGame.UserMessage.c_str());
		ktools::valueToRect(ktools::getASCIIColorBackBuffer(), bbWidth, bbHeight, bbHeight-3, -((int32_t)instanceGame.UserMessage.size()/2), ktools::CENTER, &(color = COLOR_GREEN), 1, (int32_t)instanceGame.UserMessage.size());

		lineToScreen(bbHeight-2, 0, ktools::CENTER, "%s", instanceGame.UserError.c_str());
		ktools::valueToRect(ktools::getASCIIColorBackBuffer(), bbWidth, bbHeight, bbHeight-2, -((int32_t)instanceGame.UserError.size()/2), ktools::CENTER, &(color = COLOR_RED), 1, (int32_t)instanceGame.UserError.size());

		// Draw cursor
		int32_t mouseX = instanceGame.FrameInput.MouseX, mouseY = instanceGame.FrameInput.MouseY;
		ktools::getASCIIColorBackBuffer()[mouseY*bbWidth+mouseX] = 
			( ((ktools::getASCIIColorBackBuffer()[mouseY*bbWidth+mouseX] & 0xF0) >> 4)
			| ((ktools::getASCIIColorBackBuffer()[mouseY*bbWidth+mouseX] & 0x0F) << 4)
			);
	}
	//lineToScreen(instanceGame.FrameInput.MouseY, instanceGame.FrameInput.MouseX, LEFT, "\x21");
	ktools::presentASCIIBackBuffer();
}; 	// 


void drawIntro( SGame& instanceGame )
{
	drawFireBackground(instanceGame.PostEffectDisplay, instanceGame.FrameTimer.LastTimeSeconds);

	int32_t displayWidth	= (int32_t)instanceGame.PostEffectDisplay.Width;
	int32_t displayDepth	= (int32_t)instanceGame.PostEffectDisplay.Depth;

	static std::string words[] = {"Vulgar", "Display", "of", "Power"};
	for( uint32_t i=0; i<ktools::size(words); ++i) {
		uint32_t offsetY = (uint32_t)((displayDepth>>1)-(ktools::size(words)>>1)+i*2);
		uint32_t offsetX = printfToGrid(instanceGame.PostEffectDisplay.Screen, offsetY, 0, ktools::CENTER, "%s", words[i].c_str());
		for( size_t j=0, wordLen = words[i].size(); j<wordLen; j++ )
			instanceGame.PostEffectDisplay.TextAttributes.Cells[offsetY][offsetX+j] = COLOR_ORANGE;
	}
};


SGameState drawWelcome(SGame& instanceGame, const SGameState& returnValue)
{
	const std::string textToPrint = "Welcome back commander " + std::string(instanceGame.Players[PLAYER_USER].Name) + ".";
	SGlobalDisplay& display = instanceGame.GlobalDisplay;
	int32_t lineOffset		= (display.Screen.Depth>>1)-1;
	int32_t columnOffset	=  display.Screen.Width/2-(int32_t)textToPrint.size()/2;

	bool bDonePrinting = getMessageSlow(instanceGame.SlowMessage, textToPrint, instanceGame.FrameTimer.LastTimeSeconds);
	columnOffset = printfToGrid(display.Screen, lineOffset, columnOffset, ktools::LEFT, "%s", instanceGame.SlowMessage);
	for(size_t i=0, charCount = textToPrint.size()+2; i<charCount; i++) display.TextAttributes.Cells[lineOffset][columnOffset+i] = COLOR_GREEN;
	if ( bDonePrinting ) {
		static SMenu<SGameState, ktools::size(optionsControlCenter)> menuControlCenter(optionsControlCenter, {GAME_STATE_MENU_MAIN}, "Control Center", 28);
		return drawMenu(display.Screen, &display.TextAttributes.Cells[0][0], menuControlCenter, instanceGame.FrameInput, returnValue);
	}
	return returnValue;
};
