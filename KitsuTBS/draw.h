#define NOMINMAX

#include "Game.h"

#include <algorithm>

#ifndef __DRAW_H_2394623987462983746823749623__
#define __DRAW_H_2394623987462983746823749623__

template<size_t _Width, size_t _Height>
void drawFireBackground( STacticalDisplay<_Width, _Height>& display, double lastTimeSeconds )
{
	int32_t displayWidth	= (int32_t)display.Width;
	int32_t displayDepth	= (int32_t)display.Depth;

	for(int32_t x=0; x<displayWidth; ++x) 
		if(display.DisplayWeights	[displayDepth-1][x] == 0) 
		{
			if( rand()%2 )
			{
				display.Screen			[displayDepth-1][x] = '.';
				display.DisplayWeights	[displayDepth-1][x] = .0000001f;
				display.Speed			[displayDepth-1][x] = rand()*.033f;
				display.SpeedTarget		[displayDepth-1][x] = rand()*.033f;
			}
			else
			{
				display.Screen			[displayDepth-1][x] = ' ';
				display.DisplayWeights	[displayDepth-1][x] = 0;
				display.Speed			[displayDepth-1][x] = 0;
				display.SpeedTarget		[displayDepth-1][x] = 0;
			}
		}
	
	for(uint32_t z=1; z<display.Depth; ++z) 
		for(uint32_t x=0; x<display.Width; ++x) 
		{
			if(display.DisplayWeights[z][x] == 0)
				continue;

			display.DisplayWeights[z][x] += (float)(lastTimeSeconds*display.Speed[z][x]);

			if(display.Speed[z][x] < display.SpeedTarget[z][x])
				display.Speed	[z][x] += (float)((lastTimeSeconds+z*.001)*.01);//*(z/(float)TACTICAL_DISPLAY_DEPTH));
			else
				display.Speed	[z][x] -= (float)((lastTimeSeconds+z*.001)*.01);//*(z/(float)TACTICAL_DISPLAY_DEPTH));
				
			int randX = (rand()%2) ? rand()%5-2 : 0;
			if(display.DisplayWeights[z][x] > 1.0)
			{
				if(0 == z-1)
				{
					display.Screen			[z-1][std::max(std::min((int)x+randX, displayWidth-1), 0)]	= ' ';
					display.DisplayWeights	[z-1][std::max(std::min((int)x+randX, displayWidth-1), 0)]	= 0;
					display.Speed			[z-1][std::max(std::min((int)x+randX, displayWidth-1), 0)]	= 0; 
					display.SpeedTarget		[z-1][std::max(std::min((int)x+randX, displayWidth-1), 0)]	= 0;
				}
				else
				{
					display.Screen			[z-1][std::max(std::min((int)x+randX, displayWidth-1), 0)]	= display.Screen[z][x];
					display.DisplayWeights	[z-1][std::max(std::min((int)x+randX, displayWidth-1), 0)]	= 0.000001f;
					display.Speed			[z-1][std::max(std::min((int)x+randX, displayWidth-1), 0)]	= rand()*.0025f; 
					display.SpeedTarget		[z-1][std::max(std::min((int)x+randX, displayWidth-1), 0)]	= rand()*.0025f*(z/(float)displayDepth);
				}

				display.Screen			[z][x]		= ' ';
				display.DisplayWeights	[z][x]		= 0;
				display.Speed			[z][x]		= 0; 
				display.SpeedTarget		[z][x]		= 0;
			}
		}

#define TACTICAL_DISPLAY_YPOS 5
}

#endif // __DRAW_H_2394623987462983746823749623__
