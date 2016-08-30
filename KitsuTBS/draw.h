#define NOMINMAX

#include "Game.h"

#include <algorithm>

#ifndef __DRAW_H_2394623987462983746823749623__
#define __DRAW_H_2394623987462983746823749623__

#define TACTICAL_DISPLAY_YPOS 5

template<size_t _Width, size_t _Height>
void drawSnowBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds )
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
				int32_t xpos = std::max(std::min((int)x+randX, displayWidth-1), 0);
				display.Screen			[z+1][xpos]	= display.Screen[z][x];
				display.DisplayWeights	[z+1][xpos]	= 0.0001f;
				display.Speed			[z+1][xpos]	= display.Speed[z][x];
				display.SpeedTarget		[z+1][xpos]	= (float)((rand()%5000))*0.001f;

				display.Screen			[z][x]		= ' ';
				display.DisplayWeights	[z][x]		= 0;
				display.Speed			[z][x]		= 0; 
				display.SpeedTarget		[z][x]		= 0;
			}
		}

}

template<size_t _Width, size_t _Height>
void drawFireBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds )
{
	int32_t displayWidth	= (int32_t)display.Width;
	int32_t displayDepth	= (int32_t)display.Depth;

	for(int32_t x=0; x<displayWidth; ++x) 
		if(display.DisplayWeights	[displayDepth-1][x] == 0) 
		{
			if( rand()%2 )
			{
				display.Screen			[displayDepth-1][x] = (rand()%2) ? '.' : (rand()%2) ? '|' : ',';
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
					if( '|' == display.Screen[z-1][xpos] && z < (display.Depth/5*4))
						display.Screen[z-1][xpos] = '.';
					else if((rand()%10) == 0)
						display.Screen[z-1][xpos] = ' ';

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
}


template<size_t _Width, size_t _Height>
void drawBubblesBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds )
{
	int32_t displayWidth	= (int32_t)display.Width;
	int32_t displayDepth	= (int32_t)display.Depth;

	for(int32_t x=0; x<displayWidth; ++x) 
		if(display.Screen	[displayDepth-1][x] == ' ') 
		{
			if( rand()%2 )
			{
				display.Screen			[displayDepth-1][x] = (rand()%2) ? 'o' : (rand()%2) ? '0' : (rand()%2) ? '.' : 'O';
				display.DisplayWeights	[displayDepth-1][x] = .000001f;
				display.Speed			[displayDepth-1][x] = rand()*.002f;
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

			display.Speed	[z][x] *= .999f;
			
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
					
					{
						display.Screen			[z-1][xpos]	= display.Screen[z][x];
						if( 15 == display.Screen[z-1][xpos] && z < (display.Depth/5*4))
							display.Screen[z-1][xpos] = 'O';
						else if( '0' == display.Screen[z-1][xpos] && z < (display.Depth/5*4))
							display.Screen[z-1][xpos] = 'O';
						else if( 'O' == display.Screen[z-1][xpos] && z < (display.Depth/3*2))
							display.Screen[z-1][xpos] = (rand()%2) ? 'o' : '\'';
						else if( 'o' == display.Screen[z-1][xpos] && z < (display.Depth/2))
							display.Screen[z-1][xpos] = '.';
						else if((rand()%10) == 0)
							display.Screen[z-1][xpos] = ' ';
						display.DisplayWeights	[z-1][xpos]	= 0.00001f;
						display.Speed			[z-1][xpos]	= display.Speed[z][x];
						display.SpeedTarget		[z-1][xpos]	= (float)((rand()%100))*(z*1.0f/display.Depth);
					}
				}

				display.Screen			[z][x]		= ' ';
				display.DisplayWeights	[z][x]		= 0;
				display.Speed			[z][x]		= 0; 
				display.SpeedTarget		[z][x]		= 0;
			}
		}
}




#endif // __DRAW_H_2394623987462983746823749623__
