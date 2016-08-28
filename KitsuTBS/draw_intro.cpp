#define NOMINMAX

#include "Game.h"

#include <algorithm>

void drawIntro( SGame& instanceGame )
{
	for(uint32_t x=0; x<TACTICAL_DISPLAY_WIDTH; ++x) 
		if(instanceGame.TacticalMap.DisplayWeights	[TACTICAL_DISPLAY_DEPTH-1][x] == 0) 
		{
			instanceGame.TacticalMap.Screen			[0][x] = ' ';
			if( rand()%2 )
			{
				instanceGame.TacticalMap.Screen			[TACTICAL_DISPLAY_DEPTH-1][x] = '.';
				instanceGame.TacticalMap.DisplayWeights	[TACTICAL_DISPLAY_DEPTH-1][x] = .0000001f;
				instanceGame.TacticalMap.Speed			[TACTICAL_DISPLAY_DEPTH-1][x] = rand()*.033f;
				instanceGame.TacticalMap.SpeedTarget	[TACTICAL_DISPLAY_DEPTH-1][x] = rand()*.033f;
			}
			else
			{
				instanceGame.TacticalMap.Screen			[TACTICAL_DISPLAY_DEPTH-1][x] = ' ';
				instanceGame.TacticalMap.DisplayWeights	[TACTICAL_DISPLAY_DEPTH-1][x] = 0;
				instanceGame.TacticalMap.Speed			[TACTICAL_DISPLAY_DEPTH-1][x] = 0;
				instanceGame.TacticalMap.SpeedTarget	[TACTICAL_DISPLAY_DEPTH-1][x] = 0;
			}
		}
	
	for(uint32_t z=1; z<TACTICAL_DISPLAY_DEPTH; ++z) 
		for(uint32_t x=0; x<TACTICAL_DISPLAY_WIDTH; ++x) 
		{
			if(instanceGame.TacticalMap.DisplayWeights[z][x] == 0)
				continue;

			instanceGame.TacticalMap.DisplayWeights[z][x] += (float)(instanceGame.FrameTimer.LastTimeSeconds*instanceGame.TacticalMap.Speed[z][x]);

			if(instanceGame.TacticalMap.Speed[z][x] < instanceGame.TacticalMap.SpeedTarget[z][x])
			{
				instanceGame.TacticalMap.Speed	[z][x] += (float)((instanceGame.FrameTimer.LastTimeSeconds+z*.001)*.01);//*(z/(float)TACTICAL_DISPLAY_DEPTH));
			}
			else
			{
				instanceGame.TacticalMap.Speed	[z][x] -= (float)((instanceGame.FrameTimer.LastTimeSeconds+z*.001)*.01);//*(z/(float)TACTICAL_DISPLAY_DEPTH));
			}
				
			int randX = (rand()%2) ? rand()%3-1 : 0;
			if(instanceGame.TacticalMap.DisplayWeights[z][x] > 1.0)
			{
				instanceGame.TacticalMap.Screen			[z-1][std::max(std::min((int)x+randX, TACTICAL_DISPLAY_WIDTH-1), 0)]	= instanceGame.TacticalMap.Screen[z][x];
				instanceGame.TacticalMap.DisplayWeights	[z-1][std::max(std::min((int)x+randX, TACTICAL_DISPLAY_WIDTH-1), 0)]	= 0.000001f;
				instanceGame.TacticalMap.Speed			[z-1][std::max(std::min((int)x+randX, TACTICAL_DISPLAY_WIDTH-1), 0)]	= rand()*.0025f; 
				instanceGame.TacticalMap.SpeedTarget	[z-1][std::max(std::min((int)x+randX, TACTICAL_DISPLAY_WIDTH-1), 0)]	= rand()*.0025f*(z/(float)TACTICAL_DISPLAY_DEPTH);

				instanceGame.TacticalMap.Screen			[z][x]		= ' ';
				instanceGame.TacticalMap.DisplayWeights	[z][x]		= 0;
				instanceGame.TacticalMap.Speed			[z][x]		= 0; 
				instanceGame.TacticalMap.SpeedTarget	[z][x]		= 0;
			}
		}


	memcpy(&instanceGame.TacticalMap.Screen[TACTICAL_DISPLAY_DEPTH/2-2]	[TACTICAL_DISPLAY_WIDTH/2-strlen("Un"			)/2-1], "Un"		, strlen("Un"			));
	memcpy(&instanceGame.TacticalMap.Screen[TACTICAL_DISPLAY_DEPTH/2]	[TACTICAL_DISPLAY_WIDTH/2-strlen("Juego"		)/2-1], "Juego" 	, strlen("Juego"		));
	memcpy(&instanceGame.TacticalMap.Screen[TACTICAL_DISPLAY_DEPTH/2+2]	[TACTICAL_DISPLAY_WIDTH/2-strlen("Increible"	)/2-1], "Increible"	, strlen("Increible"	));

	//memcpy(&instanceGame.TacticalMap.Tiles[TACTICAL_DISPLAY_DEPTH/2-2]	[TACTICAL_DISPLAY_WIDTH/2-strlen("New"		)/2-1], "New"	, strlen("New"		));
	//memcpy(&instanceGame.TacticalMap.Tiles[TACTICAL_DISPLAY_DEPTH/2]	[TACTICAL_DISPLAY_WIDTH/2-strlen("World"	)/2-1], "World"	, strlen("World"	));
	//memcpy(&instanceGame.TacticalMap.Tiles[TACTICAL_DISPLAY_DEPTH/2+2]	[TACTICAL_DISPLAY_WIDTH/2-strlen("Owner"	)/2-1], "Owner"	, strlen("Owner"	));
};