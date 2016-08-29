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
				display.Screen			[displayDepth-1][x] = (rand()%2) ? '.' : (rand()%2) ? '`' : ',';
				display.DisplayWeights	[displayDepth-1][x] = .0000001f;
				display.Speed			[displayDepth-1][x] = rand()*.001f;
				display.SpeedTarget		[displayDepth-1][x] = rand()*.0009f;
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
			if(display.Screen[z][x] == ' ')
				continue;

			display.DisplayWeights[z][x] += (float)(lastTimeSeconds*display.Speed[z][x]);

			if(display.Speed[z][x] < display.SpeedTarget[z][x])
				display.Speed	[z][x] += (float)((display.Speed[z][x]*lastTimeSeconds))*(display.Depth-z);
			else
				display.Speed	[z][x] -= (float)((display.Speed[z][x]*lastTimeSeconds))*(display.Depth-z);
			
			int randX = (rand()%2) ? rand()%5-2 : 0;
			if(display.DisplayWeights[z][x] > 1.0)
			{
				if(1 == z)
				{
					display.Screen			[0][x]	= ' ';
					display.DisplayWeights	[0][x]	= 0;
					display.Speed			[0][x]	= 0; 
					display.SpeedTarget		[0][x]	= 0;
				}
				else
				{
					int32_t xpos = std::max(std::min((int)x+randX, displayWidth-1), 0);
					display.Screen			[z-1][xpos]	= display.Screen[z][x];
					display.DisplayWeights	[z-1][xpos]	= 0.00001f;
					display.Speed			[z-1][xpos]	= display.Speed[z][x];
					display.SpeedTarget		[z-1][xpos]	= (float)((rand()%1000))*0.05f*(z*2/(float)display.Depth);
				}

				display.Screen			[z][x]		= ' ';
				display.DisplayWeights	[z][x]		= 0;
				display.Speed			[z][x]		= 0; 
				display.SpeedTarget		[z][x]		= 0;
			}
		}

#define TACTICAL_DISPLAY_YPOS 5
}



template<size_t _Width, size_t _Height>
void drawSnowBackground( STacticalDisplay<_Width, _Height>& display, double lastTimeSeconds )
{
	int32_t displayWidth	= (int32_t)display.Width;
	int32_t displayDepth	= (int32_t)display.Depth;

	for(int32_t x=0; x<displayWidth; ++x) 
		if(display.DisplayWeights[0][x] == 0) 
		{
			if( 0 == (rand()%500))
			{
				display.Screen			[0][x] = (rand()%2) ? '.' : (rand()%2) ? 15 : ',';	
				display.DisplayWeights	[0][x] = .00001f;
				display.Speed			[0][x] = rand()*.001f;
				display.SpeedTarget		[0][x] = rand()*.001f;
			}
			else
			{
				display.Screen			[0][x] = ' ';
				display.DisplayWeights	[0][x] = 0;
				display.Speed			[0][x] = 0;
				display.SpeedTarget		[0][x] = 0;
			}
		}
	
	for(uint32_t z=0; z<display.Depth-2; ++z) 
		for(uint32_t x=0; x<display.Width; ++x) 
		{
			if(display.DisplayWeights[z][x] == 0)
				continue;

			display.DisplayWeights[z][x] += (float)(lastTimeSeconds*display.Speed[z][x]);

			if(display.Speed[z][x] < display.SpeedTarget[z][x])
				display.Speed	[z][x] += (float)((display.Speed[z][x]*lastTimeSeconds*lastTimeSeconds))*.1f;
			else
				display.Speed	[z][x] -= (float)((display.Speed[z][x]*lastTimeSeconds*lastTimeSeconds))*.1f;
				
			int randX = (rand()%2) ? rand()%3-1 : 0;
			if(display.DisplayWeights[z][x] > 1.0)
			{
				if(z == (display.Depth-2))
				{
					display.Screen			[display.Depth-1][x]	= '_';
					display.DisplayWeights	[display.Depth-1][x]	= 0;
					display.Speed			[display.Depth-1][x]	= 0; 
					display.SpeedTarget		[display.Depth-1][x]	= 0;
				}
				else
				{
					int32_t xpos = std::max(std::min((int)x+randX, displayWidth-1), 0);
					display.Screen			[z+1][xpos]	= display.Screen[z][x];
					display.DisplayWeights	[z+1][xpos]	= 0.0001f;
					display.Speed			[z+1][xpos]	= display.Speed[z][x];
					display.SpeedTarget		[z+1][xpos]	= (float)((rand()%100))*0.001f;
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
