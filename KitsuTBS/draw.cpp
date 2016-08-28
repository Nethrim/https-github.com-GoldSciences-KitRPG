#include "Game.h"

void drawIntro(SGame& instanceGame);

GAME_STATE drawCredits()
{
	static double offset = (double)game::getASCIIBackBufferHeight();// (game::getASCIIBackBufferHeight()>>1)-4;
	int32_t curLine;
	curLine = (int32_t)(offset+1); if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight()) game::lineToScreen(curLine, 0, game::CENTER, "%-34.34s: %13.13s", "Programming"						, "Kitsu."		);
	curLine = (int32_t)(offset+2); if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight()) game::lineToScreen(curLine, 0, game::CENTER, "%-34.34s: %13.13s", "Everything else"					, "Kitsu."		);
	curLine = (int32_t)(offset+3); if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight()) game::lineToScreen(curLine, 0, game::CENTER, "%-34.34s: %13.13s", "Loosely based on homework code by"	, "Nethrim."	);
	curLine = (int32_t)(offset+6); if(curLine >= 0 && curLine < (double)game::getASCIIBackBufferHeight()) game::lineToScreen(curLine, 0, game::CENTER, "Press ESC to go back.");
	
	offset -= 0.01;

	if( offset <= -5 )
		offset = game::getASCIIBackBufferHeight()+5;
	
	return GAME_CREDITS;
}

void drawStage(SGame& instancedGame)	{

	int32_t offset = 5;
	for(uint32_t z=0, maxZ=TACTICAL_DISPLAY_DEPTH; z<maxZ; ++z) {
		char row[TACTICAL_DISPLAY_WIDTH+1] = {};
		memcpy(row, instancedGame.TacticalMap.Screen[z], TACTICAL_DISPLAY_WIDTH);
		game::lineToScreen(offset++, 0, game::CENTER, "%s", row	);
	}
};

void drawState( SGame& instanceGame )
{
	if(instanceGame.CurrentMenu == GAME_MENU_MAIN)
		drawIntro(instanceGame);
	
	if(instanceGame.CurrentMenu == GAME_CREDITS)
		drawCredits();
	else
		drawStage(instanceGame);
}
