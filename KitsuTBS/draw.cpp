#include "draw.h"

void drawIntro(SGame& instanceGame);
static const std::string namesProgramming[] =
{	"Kitsu"											
};

static const std::string namesEverythingElse[] =
{	"Kitsu"											
};

static const std::string namesSpecialThanks[] =
{	"Nethrim"												
,	"Gorby"													
,	"Andrew"												
,	"James"													
,	"Suelika"												
,	"Paul"													
,	"Bill"													
,	"Robert"												
,	"Kirk"													
,	"Pia"													
,	"Stuart"												
,	"Brent"													
,	"Riva"													
,	"Serguei"												
,	"David"													
,	"Nhi"													
,	"Andy"													
,	"Esteban"												
,	"Chi-Wei"												
,	"Matt"													
,	"Michael"												
,	"Henry"													
,	"Gabriela"												
,	"Marc"											
,	"Terran"												
,	"Adan"													
,	"Ted"													
,	"Fredrik"												
,	"Jose"													
,	"Jose"													
,	"Zach"													
,	"Cara"													
,	"Nicolas"												
,	"Martin"												
,	"Mauro"													
,	"Luis"													
,	"Carlos"												
,	"... and also to those which I'm going to add later to this list!"	
};

GAME_STATE drawCredits(double lastFrameTime)
{
	static double offset = (double)game::getASCIIBackBufferHeight();// (game::getASCIIBackBufferHeight()>>1)-4;
	int32_t curLine = (int32_t)offset;
	static int32_t maxDifference = curLine;
	int32_t curDifference = curLine;
	curLine = (int32_t)(curLine += 1);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "-- Loosely based on homework code and idea by --"	);
	curLine = (int32_t)(curLine += 2);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "Nethrim"											);
	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "-- Programming --"								);
	curLine = (int32_t)(curLine += 2);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "Kitsu"											);
	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "-- Beta Testing --"								);
	curLine = (int32_t)(curLine += 2);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "Gorby"											);
	curLine = (int32_t)(curLine += 2);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "Nethrim"											);
	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "-- Everything else --"							);
	curLine = (int32_t)(curLine += 2);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "Kitsu"											);
	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "-- Special Thanks to --"							);
	for(uint32_t i=0; i<klib::size(namesSpecialThanks); ++i) {
		curLine = (int32_t)(curLine += 2);	
		if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight()) 
			game::lineToScreen(curLine, 0, game::CENTER, "%s", namesSpecialThanks[i].c_str());
	}

	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "Have a nice weekend!"								); else if(curLine < -10) game::lineToScreen( game::getASCIIBackBufferHeight()+curLine+maxDifference, 0, game::CENTER, "Press ESC to go back."							);
	curLine = (int32_t)(curLine += 4);	if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight())	game::lineToScreen(curLine, 0, game::CENTER, "Press ESC to go back."							); else if(curLine < -10) game::lineToScreen( game::getASCIIBackBufferHeight()+curLine+maxDifference, 0, game::CENTER, "Press ESC to go back."							);
	
	maxDifference = std::max(curLine - curDifference, maxDifference);

	offset -= lastFrameTime*10.0;

	if( offset <= -maxDifference )
		offset += game::getASCIIBackBufferHeight()+maxDifference;

	return GAME_CREDITS;
}

template<size_t _Width, size_t _Depth>
void drawGlobalDisplay(STacticalDisplay<_Width, _Depth>& source, uint32_t offset)	
{
	for(uint32_t z=0, maxZ=std::min(source.Depth, game::getASCIIBackBufferHeight()-1-offset); z<maxZ; ++z) {
		char row[_Width+1] = {};
		memcpy(row, source.Screen[z], std::min(source.Width, game::getASCIIBackBufferWidth()-1));
		game::lineToScreen(offset, 0, game::CENTER, row);
		++offset;
	}
};

void drawIntro( SGame& instanceGame )
{
	drawFireBackground(instanceGame.TacticalDisplay, instanceGame.FrameTimer.LastTimeSeconds);

	int32_t displayWidth	= (int32_t)instanceGame.TacticalDisplay.Width;
	int32_t displayDepth	= (int32_t)instanceGame.TacticalDisplay.Depth;

	memcpy(&instanceGame.TacticalDisplay.Screen[displayDepth	/2-2]	[displayWidth/2-strlen("Un"			)/2-1], "Un"		, strlen("Un"			));
	memcpy(&instanceGame.TacticalDisplay.Screen[displayDepth	/2]		[displayWidth/2-strlen("Juego"		)/2-1], "Juego" 	, strlen("Juego"		));
	memcpy(&instanceGame.TacticalDisplay.Screen[displayDepth	/2+2]	[displayWidth/2-strlen("Increible"	)/2-1], "Increible"	, strlen("Increible"	));
};

void drawState( SGame& instanceGame )
{
	if(instanceGame.CurrentMenu == GAME_MENU_MAIN)
		drawIntro(instanceGame);
	
	if(instanceGame.CurrentMenu == GAME_CREDITS)
	{
		drawFireBackground(instanceGame.GlobalDisplay, instanceGame.FrameTimer.LastTimeSeconds);
		drawGlobalDisplay(instanceGame.GlobalDisplay, 0);
		drawCredits(instanceGame.FrameTimer.LastTimeSeconds);
	}
	else
		drawGlobalDisplay(instanceGame.TacticalDisplay, 5);
}

