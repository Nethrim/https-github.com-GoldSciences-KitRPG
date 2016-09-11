#include "misc.h"
#include "text.h"
#include "noise.h"

#ifndef __TILE_H__973098721983761298736129873691872361__
#define __TILE_H__973098721983761298736129873691872361__

namespace klib
{
#pragma pack(push, 1)
	struct SCharacterTile {
		int8_t		PlayerIndex	;
		int8_t		SquadIndex	;
		int8_t		AgentIndex	;
	};

	struct STopologyHeight {
		int8_t		Smooth	;
		int8_t		Sharp	;
	};

	// The difference between SItemTile and SEntity is that SEntity member values must be always valid whereas these can be -1.
	// Setting IndexDefinition to -1 will effectively disable the tile whereas setting only Modifier or Level to -1 must default to 0 when converting to an SEntity.
	struct SPropTile {
		int16_t		Definition	;
		int16_t		Modifier	;
		int16_t		Level		;
		int16_t		Owner		;
	};

	template<typename _T, size_t _Width, size_t _Depth>
	struct SGrid
	{
		typedef _T	TCell;

		TCell		Cells[_Depth][_Width] = {};

		static const uint32_t	Width = (uint32_t)_Width;
		static const uint32_t	Depth = (uint32_t)_Depth;
	};
#pragma pack(pop)

	template <size_t _Width, size_t _Depth> 
	struct STerrainTiles
	{
		SGrid<STopologyHeight	, _Width, _Depth>	Topology	;
		SGrid<int8_t			, _Width, _Depth>	Occlusion	;	//	This value defines wether the terrain blocks the sight up to a given height.
		void Clear() {
			clearGrid(Topology	, STopologyHeight({0, 0}));
			clearGrid(Occlusion	, (int8_t)0);
		}
	};

	template <size_t _Width, size_t _Depth> 
	struct SEntityTiles
	{
		SGrid<SCharacterTile, _Width, _Depth>		Agents;	
		SGrid<SPropTile		, _Width, _Depth>		Props;	
		SGrid<int32_t		, _Width, _Depth>		Coins;	

		void Clear() {
			clearGrid(	Agents	, {-1, -1, -1} );
			clearGrid(	Props	, {-1, -1, -1, -1} );
			clearGrid(	Coins	, 0 );
		}
	};

	template <size_t _Width, size_t _Depth> 
	struct SGameTiles 
	{
		STerrainTiles<_Width, _Depth>				Terrain;
		SEntityTiles<_Width, _Depth>				Entities;

		void Clear() {
			Entities.Clear();
			Terrain.Clear();
		}

		static const uint32_t Width = (uint32_t)_Width;
		static const uint32_t Depth = (uint32_t)_Depth;
	};

	template<typename _CellType, size_t _Width, size_t _Height>
	void fillCellsFromNoise( SGrid<_CellType, _Width, _Height>& grid, const _CellType& value, int32_t seed, const _CellType& clearValue = ' ', int32_t diceFaces=10 ) {
		_CellType* cells = &grid.Cells[0][0];
		for(uint32_t i=0, count=_Width*_Height;  i<count; ++i) {
			double noise = noiseNormal(i+1, seed);
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

	template<typename _TCell, size_t _Width, size_t _Height, typename... _Args>
	int32_t valueToGrid( SGrid<_TCell, _Width, _Height>& grid, int32_t offsetLine, int32_t offsetColumn, ALIGN align, const _TCell* values, int32_t valueCount, int32_t repeat=0 ) {
		return valueToRect( &grid.Cells[0][0], _Width, _Height, offsetLine, offsetColumn, align, values, valueCount, repeat );
	}

	template<size_t _Width, size_t _Height>
	int32_t lineToGrid( SGrid<char, _Width, _Height>& display, int32_t offsetLine, int32_t offsetColumn, ALIGN align, const char* text, uint32_t charCount = 0xFFFFFFFF ) {
		return lineToRect(&display.Cells[0][0], _Width, _Height, offsetLine, offsetColumn, align, text, charCount );
	}

	template<typename _CellType, size_t _Width, size_t _Height, typename... _Args>
	int32_t printfToGrid( SGrid<_CellType, _Width, _Height>& display, int32_t offsetLine, int32_t offsetColumn, ALIGN align, const char* format, _Args... args ) {
		return printfToRect(&display.Cells[0][0], _Width, _Height, offsetLine, offsetColumn, align, format, args... );
	}

} // namespace

#endif // __TILE_H__973098721983761298736129873691872361__