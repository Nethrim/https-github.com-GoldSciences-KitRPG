#define NOMINMAX

#include "Game.h"
#include "noise.h"

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
					display.Screen			.Cells[0][x] = (noise1D((uint32_t)(lastTimeSeconds*10000+x), disturbance) > 0.0) ? '.' : (noise1D((uint32_t)(lastTimeSeconds*10000-x*x), disturbance) > 0.0) ? 15 : ',';	
					display.DisplayWeights	.Cells[0][x] = .00001f;
					display.Speed			.Cells[0][x] = rand()*.001f;
					display.SpeedTarget		.Cells[0][x] = rand()*.001f;
					display.TextAttributes	.Cells[0][x] = (noise1D((uint32_t)(lastTimeSeconds*10000-x), disturbance) > 0.0) ? COLOR_CYAN:COLOR_WHITE;
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
			}

		for(uint32_t z=0; z<display.Depth-2; ++z) 
			for(uint32_t x=0; x<display.Width; ++x) 
			{
				if(display.DisplayWeights.Cells[z][x] == 0)
					continue;

				if(display.DisplayWeights.Cells[z][x] > 1.0)
				{
					int randX = (rand()%2) ? rand()%(1+disturbance*2)-disturbance : 0;
					int32_t xpos = std::max(std::min((int)x+randX, displayWidth-1), 0);
					display.Screen			.Cells[z+1][xpos]	= display.Screen.Cells			[z][x];
					display.Speed			.Cells[z+1][xpos]	= display.Speed.Cells			[z][x];
					display.TextAttributes	.Cells[z+1][xpos]	= (noise1D((uint32_t)(lastTimeSeconds*10000+x), disturbance) > 0.0) ? COLOR_CYAN:COLOR_WHITE;
					//display.TextAttributes	.Cells[z+1][xpos]	= display.TextAttributes.Cells	[z][x];
					display.DisplayWeights	.Cells[z+1][xpos]	= 0.0001f;
					display.SpeedTarget		.Cells[z+1][xpos]	= (float)((rand()%5000))*0.001f+0.001f;

					display.Screen			.Cells[z][x]	= ' ';
					display.DisplayWeights	.Cells[z][x]	= 0;
					display.Speed			.Cells[z][x]	= 0; 
					display.SpeedTarget		.Cells[z][x]	= 0;
					display.TextAttributes	.Cells[z][x]	= COLOR_WHITE;
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
		uint32_t seed		= (uint32_t)(disturbance+lastTimeSeconds*100000*(1+(rand()%100)));
		uint32_t randBase	= (uint32_t)(lastTimeSeconds*(disturbance+654)*100000			);
		for(int32_t x=0; x<displayWidth; ++x) 
			//if(	display.Screen[displayDepth-1][x] != '.' 
			// && display.Screen[displayDepth-1][x] != '|' 
			// && display.Screen[displayDepth-1][x] != ','
			//) 
			if(display.DisplayWeights.Cells[firstRow][x] == 0)
			{
				if( 0 == (rand()%4) )
				{
					display.Screen			.Cells[firstRow][x] =  (noise1D(randBase+x, seed+1203) > 0.0) ? '.' :  (noise1D(randBase+1+x*x, seed+1235) > 0.0) ? '|' : ',';
					display.DisplayWeights	.Cells[firstRow][x] = .00001f;
					display.Speed			.Cells[firstRow][x] = rand()*.001f+0.001f;
					display.SpeedTarget		.Cells[firstRow][x] = rand()*.0009f+0.001f;
					display.TextAttributes	.Cells[firstRow][x] = bReverse ? ((noise1D(randBase+321+x, seed+91423) > 0.0)?COLOR_CYAN:COLOR_BLUE) :  (noise1D(randBase+32+x, seed<<1) > 0.0) ? COLOR_RED : (noise1D(randBase+987429654+x, seed+98234) > 0.0) ? COLOR_ORANGE : COLOR_DARKYELLOW;
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
			}

		for(uint32_t z = bReverse ? 0 : 1, maxZ = bReverse ? display.Depth-1 : display.Depth; z < maxZ; z ++) 
			for(uint32_t x=0; x<display.Width; ++x) 
			{
				if(display.Screen.Cells[z][x] == ' ')
					continue;
			
				if(display.DisplayWeights.Cells[z][x] > 1.0) 
				{
					int randX = ((noise1D(randBase+x+z*display.Width), seed+544) > 0.0) ? rand()%(1+disturbance*2)-disturbance : 0;
					if((bReverse ? display.Width-2 : 1) == z)
					{
						display.Screen			.Cells[lastRow][x]	= ' ';
						display.DisplayWeights	.Cells[lastRow][x]	= 0;
						display.Speed			.Cells[lastRow][x]	= 0; 
						display.SpeedTarget		.Cells[lastRow][x]	= 0;
						//display.TextAttributes	.Cells[lastRow][x]	= COLOR_WHITE;
					}
					else
					{
						int32_t xpos = std::min(std::max(0, (int)x+randX), displayWidth-1);
						int32_t zpos = bReverse ? z+1 : z-1;
	
						if((rand()%disappearChanceDivisor) == 0) {
							display.Screen			.Cells[zpos][xpos] = ' ';
							display.DisplayWeights	.Cells[zpos][xpos] = 0;
							//display.TextAttributes	.Cells[zpos][xpos] = 1;
						}
						else 
						{ 
							if(('|' == display.Screen	.Cells[z][x]) && z < (display.Depth/5*4)) {
								display.Screen			.Cells[zpos][xpos] = '.';
								display.TextAttributes	.Cells[zpos][xpos] = ((bReverse) || (noiseNormal(x, seed<<2) < 0.0)) ? COLOR_GRAY : COLOR_YELLOW; 
							}
							else if( bReverse && z > (display.Depth/5)) {
								display.Screen			.Cells[zpos][xpos] = '|';
								display.TextAttributes	.Cells[zpos][xpos] = COLOR_CYAN;
							}
							else {
								display.Screen			.Cells[zpos][xpos]	= display.Screen.Cells[z][x];
								display.TextAttributes	.Cells[zpos][xpos]	= display.TextAttributes.Cells[z][x];
							}


							display.DisplayWeights	.Cells[zpos][xpos]	= 0.00001f;
							display.Speed			.Cells[zpos][xpos]	= display.Speed.Cells[z][x];
							display.SpeedTarget		.Cells[zpos][xpos]	= (float)noiseNormal(x, seed)*50.0f;
							if(bDontSlowdown)
								display.SpeedTarget	.Cells[zpos][xpos]	*= ((bReverse ? display.Depth-z : z )*2/(float)display.Depth);
							display.SpeedTarget		.Cells[zpos][xpos]	+= 0.001f;
						}
					}

					display.Screen				.Cells[z][x]	= ' ';
					display.DisplayWeights		.Cells[z][x]	= 0;
					display.Speed				.Cells[z][x]	= 0; 
					display.SpeedTarget			.Cells[z][x]	= 0;
					display.TextAttributes		.Cells[z][x]	= COLOR_WHITE;
				}
			}
	}

	template<size_t _Width, size_t _Height>
	void drawBubblesBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds, uint32_t disturbance=2 )
	{
		int32_t displayWidth	= (int32_t)display.Width;
		int32_t displayDepth	= (int32_t)display.Depth;

		uint32_t seed		= (uint32_t)(disturbance+lastTimeSeconds*100000*(1+(rand()%100)));
		uint32_t randBase	= (uint32_t)(lastTimeSeconds*(disturbance+654)*100000			);
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
					display.Screen			.Cells[displayDepth-1][x] = (noise1D(randBase+x, seed+1203) > 0.0) ? 'o' : (noise1D(randBase+561+x, seed+2135) > 0.0) ? '0' : (noise1D(randBase+x+6, seed+103) > 0.0) ? '.' : 'O';
					display.DisplayWeights	.Cells[displayDepth-1][x] = .000001f;
					display.Speed			.Cells[displayDepth-1][x] = rand()*.001f+0.001f;
					display.SpeedTarget		.Cells[displayDepth-1][x] = rand()*.0025f+0.001f;
					display.TextAttributes	.Cells[displayDepth-1][x] = (rand() % 2)?COLOR_GREEN:COLOR_DARKGREEN;
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
			}

		for(uint32_t z=1; z<display.Depth; ++z) 
			for(uint32_t x=0; x<display.Width; ++x) 
			{
				if(display.Screen.Cells[z][x] == ' ')
					continue;

				if(display.DisplayWeights.Cells[z][x] > 1.0)
				{
					int randX = (rand()%2) ? rand()%(1+disturbance*2)-disturbance : 0;
					if(1 == z)
					{
						display.Screen			.Cells[0][x]	= ' ';
						display.DisplayWeights	.Cells[0][x]	= 0;
						display.Speed			.Cells[0][x]	= 0; 
						display.SpeedTarget		.Cells[0][x]	= 0;
						display.TextAttributes	.Cells[0][x]	= COLOR_WHITE;
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
								 if( '0' == display.Screen.Cells[z][x] && z < (display.Depth/5*4))
									display.Screen.Cells[z-1][xpos] = 'O';
							else if( 'O' == display.Screen.Cells[z][x] && z < (display.Depth/3*2))
									display.Screen.Cells[z-1][xpos] = (noise1D(randBase+x, seed+12345) > 0.0) ? 'o' : '\'';
							else if( 'o' == display.Screen.Cells[z][x] && z < (display.Depth>>1))
									display.Screen.Cells[z-1][xpos] = '.';
							else
									display.Screen.Cells			[z-1][xpos]	= display.Screen.Cells[z][x];

							display.TextAttributes	.Cells[z-1][xpos]	= (noise1D(randBase+x+x, seed+41203) > 0.0) ? COLOR_DARKGREEN : COLOR_GREEN;
							display.DisplayWeights	.Cells[z-1][xpos]	= 0.00001f;
							display.Speed			.Cells[z-1][xpos]	= display.Speed.Cells[z][x];
							display.SpeedTarget		.Cells[z-1][xpos]	= (float)noiseNormal(x, seed) * 20.0f * (z*1.0f/display.Depth)+0.001f;
						}
					}

					display.Screen			.Cells[z][x]		= ' ';
					display.DisplayWeights	.Cells[z][x]		= 0;
					display.Speed			.Cells[z][x]		= 0; 
					display.SpeedTarget		.Cells[z][x]		= 0;
					display.TextAttributes	.Cells[z][x]		= COLOR_WHITE;
				}
			}
	}

	template<size_t _Width, size_t _Height>
	void drawRainBackground( SWeightedDisplay<_Width, _Height>& display, double lastTimeSeconds )
	{
		for(uint32_t i=0; i<_Width; ++i)
			if(rand()%2)
				display.Screen.Cells[_Height-1][i] = (rand()%2) ? '.' : '|';
		return drawFireBackground( display, lastTimeSeconds*1.5, 0, 20, true, false );
	}

	template<typename _TCell, size_t _LineCount>
	SGameState drawCredits(_TCell* display, size_t width, size_t depth, double lastFrameTime, const std::string (&namesCredits)[_LineCount], const SGameState& returnValue)
	{
		static double offset = (double)depth;
		int32_t curLine = (int32_t)offset;
		static int32_t maxDifference = curLine;
		int32_t curDifference = curLine;
		double bbHeight = (double)depth;

		for(uint32_t i=0; i < klib::size(namesCredits) && curLine < bbHeight; ++i)
			if((curLine+=2) >= 0)
				printfToRect(display, width, depth, curLine, 0, CENTER, "%s", namesCredits[i].c_str());

		maxDifference = std::max(curLine - curDifference, maxDifference);

		offset -= lastFrameTime*6.0;

		if( offset <= -maxDifference )
			offset += depth+maxDifference;

		return returnValue;
	}

	template<typename _TCell, size_t _Width, size_t _Depth, size_t _LineCount>
	SGameState drawCredits(SGrid<_TCell, _Width, _Depth>& display, double lastFrameTime, const std::string (&namesCredits)[_LineCount], const SGameState& returnValue) {
		return drawCredits(&display.Cells[0][0], _Width, _Depth, lastFrameTime, namesCredits, returnValue);
	}

	void drawAndPresentGame( SGame& instanceGame );

} // namespace

#define TACTICAL_DISPLAY_POSY 5


#endif // __DRAW_H_2394623987462983746823749623__
