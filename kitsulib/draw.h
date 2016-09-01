#define NOMINMAX

#include "Game.h"

#include <algorithm>

#ifndef __DRAW_H_2394623987462983746823749623__
#define __DRAW_H_2394623987462983746823749623__

namespace klib
{

	template<size_t _Width, size_t _Height>
	void drawSnowBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds, uint32_t disturbance = 1 )
	{
		int32_t displayWidth	= (int32_t)display.Width;
		int32_t displayDepth	= (int32_t)display.Depth;

		for(int32_t x=0; x<displayWidth; ++x) 
			if(display.DisplayWeights.Cells[0][x] == 0) 
			{
				if( 0 == (rand()%200) && x % 2)
				{
					display.Screen			.Cells[0][x] = (rand()%2) ? '.' : (rand()%2) ? 15 : ',';	
					display.DisplayWeights	.Cells[0][x] = .00001f;
					display.Speed			.Cells[0][x] = rand()*.001f;
					display.SpeedTarget		.Cells[0][x] = rand()*.001f;
				}
			}
	
		for(uint32_t z=0; z<display.Depth-2; ++z) 
			for(uint32_t x=0; x<display.Width; ++x) 
			{
				if(display.DisplayWeights.Cells[z][x] == 0)
					continue;

				display.DisplayWeights.Cells[z][x] += (float)(lastTimeSeconds*display.Speed.Cells[z][x]);

				if(display.Speed.Cells[z][x] < display.SpeedTarget.Cells[z][x])
					display.Speed.Cells	[z][x] += (float)((display.Speed.Cells[z][x]*lastTimeSeconds*lastTimeSeconds));//*.1f;
				else
					display.Speed.Cells	[z][x] -= (float)((display.Speed.Cells[z][x]*lastTimeSeconds*lastTimeSeconds));//*.1f;
				
				int randX = (rand()%2) ? rand()%(1+disturbance*2)-disturbance : 0;
				if(display.DisplayWeights.Cells[z][x] > 1.0)
				{
					int32_t xpos = std::max(std::min((int)x+randX, displayWidth-1), 0);
					display.Screen			.Cells[z+1][xpos]	= display.Screen.Cells[z][x];
					display.DisplayWeights	.Cells[z+1][xpos]	= 0.0001f;
					display.Speed			.Cells[z+1][xpos]	= display.Speed.Cells[z][x];
					display.SpeedTarget		.Cells[z+1][xpos]	= (float)((rand()%5000))*0.001f+0.001f;

					display.Screen			.Cells[z][x]		= ' ';
					display.DisplayWeights	.Cells[z][x]		= 0;
					display.Speed			.Cells[z][x]		= 0; 
					display.SpeedTarget		.Cells[z][x]		= 0;
				}
			}

	}

	template<size_t _Width, size_t _Height>
	void drawFireBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds, uint32_t disturbance = 2, uint32_t disappearChanceDivisor=10, bool bReverse=false, bool bDontSlowdown=true )
	{
		int32_t displayWidth	= (int32_t)display.Width;
		int32_t displayDepth	= (int32_t)display.Depth;

		uint32_t firstRow	= bReverse ? 0 : displayDepth - 1;
		uint32_t lastRow	= bReverse ? displayDepth - 1 : 0;
		for(int32_t x=0; x<displayWidth; ++x) 
			//if(	display.Screen[displayDepth-1][x] != '.' 
			// && display.Screen[displayDepth-1][x] != '|' 
			// && display.Screen[displayDepth-1][x] != ','
			//) 
			if(display.DisplayWeights.Cells[firstRow][x] == 0)
			{
				if( 0 == (rand()%4) )
				{
					display.Screen			.Cells[firstRow][x] = (rand()%2) ? '.' : (rand()%2) ? '|' : ',';
					display.DisplayWeights	.Cells[firstRow][x] = .0000001f;
					display.Speed			.Cells[firstRow][x] = rand()*.001f+0.001f;
					display.SpeedTarget		.Cells[firstRow][x] = rand()*.0009f+0.001f;
				}
			}
	
		for(uint32_t z = bReverse ? 0 : 1, maxZ = bReverse ? display.Depth-1 : display.Depth; z < maxZ; z ++) 
			for(uint32_t x=0; x<display.Width; ++x) 
			{
				if(display.Screen.Cells[z][x] == ' ')
					continue;

				display.DisplayWeights.Cells[z][x] += (float)(lastTimeSeconds*display.Speed.Cells[z][x]);

				if(display.Speed.Cells[z][x] < display.SpeedTarget.Cells[z][x])
					display.Speed.Cells[z][x] += (float)(display.Speed.Cells[z][x]*lastTimeSeconds);
				else
					display.Speed.Cells[z][x] -= (float)(display.Speed.Cells[z][x]*lastTimeSeconds);
			
				int randX = (rand()%2) ? rand()%(1+disturbance*2)-disturbance : 0;
				if(display.DisplayWeights.Cells[z][x] > 1.0) 
				{
					if((bReverse ? display.Width-2 : 1) == z)
					{
						display.Screen			.Cells[lastRow][x]	= ' ';
						display.DisplayWeights	.Cells[lastRow][x]	= 0;
						display.Speed			.Cells[lastRow][x]	= 0; 
						display.SpeedTarget		.Cells[lastRow][x]	= 0;
					}
					else
					{
						int32_t xpos = std::max(std::min((int)x+randX, displayWidth-1), 0);
						int32_t zpos = bReverse ? z+1 : z-1;
						display.Screen.Cells			[zpos][xpos]	= display.Screen.Cells[z][x];

						if((rand()%disappearChanceDivisor) == 0) {
							display.Screen.Cells		[zpos][xpos] = ' ';
							display.DisplayWeights.Cells[zpos][xpos] = 0;
						}
						else { 
							if( '|' == display.Screen.Cells	[zpos][xpos] && z < (display.Depth/5*4))
								display.Screen.Cells		[zpos][xpos] = '.';

							display.DisplayWeights	.Cells[zpos][xpos]	= 0.00001f;
							display.Speed			.Cells[zpos][xpos]	= display.Speed.Cells[z][x];
							display.SpeedTarget		.Cells[zpos][xpos]	= (float)((rand()%1000))*0.05f;
							if(bDontSlowdown)
								display.SpeedTarget		.Cells[zpos][xpos]	*= ((bReverse ? display.Depth-z : z )*2/(float)display.Depth);
						}


						display.SpeedTarget		.Cells[zpos][xpos]	+= 0.001f;
					}

					display.Screen				.Cells[z][x]	= ' ';
					display.DisplayWeights		.Cells[z][x]	= 0;
					display.Speed				.Cells[z][x]	= 0; 
					display.SpeedTarget			.Cells[z][x]	= 0;
				}
			}
	}

	template<size_t _Width, size_t _Height>
	void drawBubblesBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds, uint32_t disturbance=2 )
	{
		int32_t displayWidth	= (int32_t)display.Width;
		int32_t displayDepth	= (int32_t)display.Depth;

		for(int32_t x=0; x<displayWidth; ++x) 
			//if(display.DisplayWeights[displayDepth-1][x] == 0) 
			if(	display.Screen.Cells[displayDepth-1][x] != '0' &&
				display.Screen.Cells[displayDepth-1][x] != 'o' &&
				display.Screen.Cells[displayDepth-1][x] != '.' &&
				display.Screen.Cells[displayDepth-1][x] != 'O'
			) 
			{
				if( rand()%2 )
				{
					display.Screen			.Cells[displayDepth-1][x] = (rand()%2) ? 'o' : (rand()%2) ? '0' : (rand()%2) ? '.' : 'O';
					display.DisplayWeights	.Cells[displayDepth-1][x] = .000001f;
					display.Speed			.Cells[displayDepth-1][x] = rand()*.001f+0.001f;
					display.SpeedTarget		.Cells[displayDepth-1][x] = rand()*.0025f+0.001f;
				}
			}
	
		for(uint32_t z=1; z<display.Depth; ++z) 
			for(uint32_t x=0; x<display.Width; ++x) 
			{
				if(display.Screen.Cells[z][x] == ' ')
					continue;

				display.DisplayWeights.Cells[z][x] += (float)(lastTimeSeconds*display.Speed.Cells[z][x]);

				if(display.Speed.Cells[z][x] < display.SpeedTarget.Cells[z][x])
					display.Speed.Cells	[z][x] += (float)((display.Speed.Cells[z][x]*lastTimeSeconds));	
				else																					
					display.Speed.Cells	[z][x] -= (float)((display.Speed.Cells[z][x]*lastTimeSeconds));	

				display.Speed.Cells[z][x] *= .999f;
			
				int randX = (rand()%2) ? rand()%(1+disturbance*2)-disturbance : 0;
				if(display.DisplayWeights.Cells[z][x] > 1.0)
				{
					if(1 == z)
					{
						display.Screen			.Cells[0][x]	= ' ';
						display.DisplayWeights	.Cells[0][x]	= 0;
						display.Speed			.Cells[0][x]	= 0; 
						display.SpeedTarget		.Cells[0][x]	= 0;
					}
					else
					{
						int32_t xpos = std::max(std::min((int)x+randX, displayWidth-1), 0);
					
						if((rand()%10) == 0) 
						{
							display.Screen.Cells[z-1][xpos]			= ' ';
							display.DisplayWeights.Cells[z-1][xpos]	= 0;
						}
						else
						{
							display.Screen.Cells			[z-1][xpos]	= display.Screen.Cells[z][x];
							if( '0' == display.Screen.Cells[z-1][xpos] && z < (display.Depth/5*4))
								display.Screen.Cells[z-1][xpos] = 'O';
							else if( 'O' == display.Screen.Cells[z-1][xpos] && z < (display.Depth/3*2))
								display.Screen.Cells[z-1][xpos] = (rand()%2) ? 'o' : '\'';
							else if( 'o' == display.Screen.Cells[z-1][xpos] && z < (display.Depth/2))
								display.Screen.Cells[z-1][xpos] = '.';

							display.DisplayWeights	.Cells[z-1][xpos]	= 0.00001f;
							display.Speed			.Cells[z-1][xpos]	= display.Speed.Cells[z][x];
							display.SpeedTarget		.Cells[z-1][xpos]	= (float)((rand()%100))*(z*1.0f/display.Depth)*.2f+0.001f;
						}
					}

					display.Screen			.Cells[z][x]		= ' ';
					display.DisplayWeights	.Cells[z][x]		= 0;
					display.Speed			.Cells[z][x]		= 0; 
					display.SpeedTarget		.Cells[z][x]		= 0;
				}
			}
	}



	template<size_t _Width, size_t _Height>
	void drawRainBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds )
	{
		for(uint32_t i=0; i<_Width; ++i)
			if(rand()%2)
				display.Screen.Cells[_Height-1][i] = (rand()%2) ? '.' : '|';
		return drawFireBackground( display, lastTimeSeconds*1.5, 0, 10, true, false );
	}

	void drawAndPresentGame( SGame& instanceGame );

	// returns true if done printing all the text.
	template <size_t _Size>
	bool drawMessageSlow(char (&message)[_Size], const std::string& textToPrint, double lastFrameSeconds)
	{
		bool bDonePrinting = getMessageSlow(message, textToPrint, lastFrameSeconds);

		klib::lineToScreen(klib::getASCIIBackBufferHeight()/2-1, klib::getASCIIBackBufferWidth()/2-textToPrint.size()/2, klib::LEFT, message);
		return bDonePrinting;
	};
} // namespace

#define TACTICAL_DISPLAY_YPOS 5


#endif // __DRAW_H_2394623987462983746823749623__
