#include "align.h"
#include "text.h"

#ifndef __TILE_H__973098721983761298736129873691872361__
#define __TILE_H__973098721983761298736129873691872361__

namespace klib
{
#pragma pack(push, 1)

	template<typename _T, size_t _Width, size_t _Depth>
	struct SGrid
	{
		_T						Cells[_Depth][_Width] = {};

		static const uint32_t	Width = (uint32_t)_Width;
		static const uint32_t	Depth = (uint32_t)_Depth;
	};

	struct SCharacterTile {
		int8_t	SquadIndex = -1;
		int8_t	AgentIndex = -1;
	};

	struct STopologyTile {
		int8_t	HeightSmooth;
		int8_t	HeightSharp	;
	};

#pragma pack(pop)

	template <size_t _Width, size_t _Depth> 
	struct STerrainTiles
	{
		SGrid<STopologyTile	, _Width, _Depth>	Topology;
		SGrid<int8_t		, _Width, _Depth>	Occlusion;	//	This value defines wether the terrain blocks the sight up to a given height.
	};

	template <size_t _Width, size_t _Depth> 
	struct SEntityTiles
	{
		SGrid<SCharacterTile, _Width, _Depth>	AgentsPlayer;
		SGrid<SCharacterTile, _Width, _Depth>	AgentsEnemy;
		SGrid<uint16_t		, _Width, _Depth>	ItemsPlayer;
		SGrid<uint16_t		, _Width, _Depth>	ItemsEnemy;
	};

	template <size_t _Width, size_t _Depth> 
	struct SGameTiles 
	{
		STerrainTiles<_Width, _Depth>	Terrain;
		SEntityTiles<_Width, _Depth>	Entities;

		static const uint32_t Width = (uint32_t)_Width;
		static const uint32_t Depth = (uint32_t)_Depth;
	};

	template<typename _CellType, size_t _Width, size_t _Height>
	void fillCellsFromNoise( SGrid<_CellType, _Width, _Height>& grid, const _CellType& value, int32_t seed, const _CellType& clearValue = ' ', int32_t diceFaces=10 ) {
		_CellType* cells = &grid.Cells[0][0];
		for(uint32_t i=0, count=_Width*_Height;  i<count; ++i) {
			double noise = noise1D(i+1, seed) * .5 + .5;
			int32_t dice = int32_t(noise * diceFaces);
			if(0 == dice)
				cells[i] = value;
		}
	}

	template<typename _CellType, size_t _Width, size_t _Height>
	void clearGrid( SGrid<_CellType, _Width, _Height>& grid, const _CellType& clearValue = ' ') {
		_CellType* cells = &grid.Cells[0][0];
		for(uint32_t i=0, count=_Width*_Height; i<count; ++i )
			cells[i] = clearValue;
	}

	template<typename _CellType, size_t _Width, size_t _Height>
	void drawGridBorders( SGrid<_CellType, _Width, _Height>& display, const _CellType& value) {
		for(uint32_t z=0, maxZ=_Height; z<maxZ; ++z){
			display.Cells[z][0] = value; 
			display.Cells[z][_Width-1]  = value; 
		}
		for(uint32_t x=0, maxX=_Width ; x<maxX; ++x){ 
			display.Cells[0][x] = value; 
			display.Cells[_Height-1][x] = value; 
		}
	}


	template<typename _CellType, size_t _Width, size_t _Height, typename... _Args>
	void lineToGrid( SGrid<_CellType, _Width, _Height>& display, int32_t offsetLine, int32_t offsetColumn, ALIGN align, const char* format, _Args... args ) {
		lineToRect(&display.Cells[0][0], _Width, _Height, offsetLine, offsetColumn, align, format, args... );
	}

} // namespace

#endif // __TILE_H__973098721983761298736129873691872361__