#ifndef min
#define min( a, b ) ( ((a)>(b)) ? (b) : (a) )
#endif 
#ifndef max
#define max( a, b ) ( ((a)<(b)) ? (b) : (a) )
#endif 
//#define GPROFILE_ENABLED
/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GGame.h"
#include "GGame_function_macros.h"
#include "IGFramework.h"

#ifdef WIN32
#include <Windows.h>
#include <crtdbg.h>
#endif

#include "GCore_printf.h"
#include "GDebugger_core.h"

using namespace god;
//
//bool PosInBoard( const SPointL& point )
//{
//	return point.x >= 0 && point.x < MAP_WIDTH && point.y >= 0 && point.y < MAP_DEPTH;
//};
template<typename _CellType, size_t _Width, size_t _Depth>
gepos2i	calculateAvailablePositionsPawn	( const SChessPiece* pawnPiece, uint32_t UnitX, uint32_t UnitZ, const _CellType (&unitMap)[_Depth][_Width])
{ 
	gepos2i	lstAvailablePositions;
	if( UnitZ >= (_Depth-1) || UnitZ <= 0 )	// pawns cannot move once reached the last row
		return lstAvailablePositions;
	id_t iPlayerTeam = pawnPiece->TeamID;
	SPointL newPos = { (int)UnitX, (int)(iPlayerTeam ? UnitZ-1 : UnitZ+1) };
	if( unitMap[newPos.y][newPos.x] == 0 )
	{
		lstAvailablePositions.push( newPos );
	}
	if( ( UnitZ == 1 && 0 == iPlayerTeam ) || ( UnitZ == (_Depth-2)/*6*/ && 1 == iPlayerTeam ) )
	{ // allow moving two places if in starting position
		newPos = { (int)UnitX, (int)(iPlayerTeam ? UnitZ-2 : UnitZ+2) };
		if( unitMap[newPos.y][newPos.x] == 0 )
		{
			lstAvailablePositions.push( newPos );
		}
	}

	newPos.x = UnitX+1; newPos.y = iPlayerTeam ? UnitZ-1 : UnitZ+1;
	if( UnitX < (_Width-1)
		&& unitMap[newPos.y][newPos.x]
		&& unitMap[newPos.y][newPos.x]->TeamID != iPlayerTeam
		)
	{
		lstAvailablePositions.push( newPos );
	}
	newPos.x = UnitX-1; newPos.y = iPlayerTeam ? UnitZ-1 : UnitZ+1;
	if( UnitX > 0 
		&& unitMap[newPos.y][newPos.x]
		&& unitMap[newPos.y][newPos.x]->TeamID != iPlayerTeam
		)
	{
		lstAvailablePositions.push( newPos );
	}
	return lstAvailablePositions; 
}

template<typename _CellType, size_t _Width, size_t _Depth>
gepos2i	calculateAvailablePositionsKnight	( const SChessPiece* pawnPiece, uint32_t UnitX, uint32_t UnitZ, const _CellType (&unitMap)[_Depth][_Width] )
{ 
	gepos2i	lstAvailablePositions; 
	id_t iPlayerTeam = pawnPiece->TeamID;
	SPointL deltas[8] = { 
		{	1,	2	}, 
		{	2,	1	}, 
		{	-1,	2	}, 
		{	2,	-1	}, 
		{	1,	-2	}, 
		{	-2,	1	}, 
		{	-1,	-2	}, 
		{	-2,	-1	}
	};
	for( uint32_t i=0; i<8; i++ )
	{
		SPointL newPos = { (int)(UnitX+deltas[i].x), (int)(UnitZ+deltas[i].y) };
		if( PosInBoard( newPos, unitMap ) &&
			(unitMap[newPos.y][newPos.x] == 0
			|| unitMap[newPos.y][newPos.x]->TeamID != iPlayerTeam)
			)
			lstAvailablePositions.push(newPos);
	}
	return lstAvailablePositions; 
}

template<typename _CellType, size_t _Width, size_t _Depth>
gepos2i	calculateAvailablePositionsBishop	( const SChessPiece* pawnPiece, uint32_t UnitX, uint32_t UnitZ, const _CellType (&unitMap)[_Depth][_Width] )
{ 
	gepos2i	lstAvailablePositions; 
	SPointL newPos = { (int)UnitX, (int)UnitZ };
	id_t iPlayerTeam = pawnPiece->TeamID;
	for( uint32_t z=UnitZ, x=UnitX; z>0 && x>0; z--, x-- )
	{
		newPos.y = z-1; newPos.x = x-1;
		if( 0 == unitMap[newPos.y][newPos.x] )
			lstAvailablePositions.push(newPos);
		else if( unitMap[newPos.y][newPos.x]->TeamID == iPlayerTeam )
			break;
		else 
		{
			lstAvailablePositions.push(newPos);
			break;
		}
	}
	for( uint32_t z=UnitZ+1, x=UnitX+1; z<_Depth && x<_Width; z++, x++ )
	{
		newPos.y = z;
		newPos.x = x;
		if( 0 == unitMap[newPos.y][newPos.x] )
			lstAvailablePositions.push(newPos);
		else if( unitMap[newPos.y][newPos.x]->TeamID == iPlayerTeam )
			break;
		else 
		{
			lstAvailablePositions.push(newPos);
			break;
		}
	}
	for( uint32_t z=UnitZ, x=UnitX+1; z>0 && x<_Width; x++, z-- )
	{
		newPos.x = x; newPos.y = z-1;
		if( 0 == unitMap[newPos.y][newPos.x] )
			lstAvailablePositions.push(newPos);
		else if( unitMap[newPos.y][newPos.x]->TeamID == iPlayerTeam )
			break;
		else 
		{
			lstAvailablePositions.push(newPos);
			break;
		}
	}
	for( uint32_t z=UnitZ+1, x=UnitX; x>0 && z<_Depth; x--, z++ )
	{
		newPos.x = x-1; newPos.y = z;
		if( 0 == unitMap[newPos.y][newPos.x])
			lstAvailablePositions.push(newPos);
		else if( unitMap[newPos.y][newPos.x]->TeamID == iPlayerTeam )
			break;
		else 
		{
			lstAvailablePositions.push(newPos);
			break;
		}
	}
	return lstAvailablePositions; 
}

template<typename _CellType, size_t _Width, size_t _Depth>
gepos2i	calculateAvailablePositionsRook	( const SChessPiece* pawnPiece, uint32_t UnitX, uint32_t UnitZ, const _CellType (&unitMap)[_Depth][_Width], bool bCastlingAvailable )
{ 
	gepos2i	lstAvailablePositions; 
	SPointL newPos = { (int)UnitX, (int)UnitZ };
	id_t iPlayerTeam = pawnPiece->TeamID;

#define LOOP_AND_PUSH(forClause, assignCoord)							\
	for forClause														\
	{																	\
		assignCoord;													\
		if( 0 == unitMap[newPos.y][newPos.x] )							\
			lstAvailablePositions.push(newPos);							\
		else if( unitMap[newPos.y][newPos.x]->TeamID == iPlayerTeam )	\
			break;														\
		else 															\
		{																\
			lstAvailablePositions.push(newPos);							\
			break;														\
		}																\
	}
	//LOOP_AND_PUSH( (uint32_t z=UnitZ; z>0; z--), newPos.y = z-1 );
	for (uint32_t z=UnitZ; z>0; z--)							
	{																	
		newPos.y = z-1;													
		if( 0 == unitMap[newPos.y][newPos.x] )							
			lstAvailablePositions.push(newPos);							
		else if( unitMap[newPos.y][newPos.x]->TeamID == iPlayerTeam )	
			break;														
		else 															
		{																
			lstAvailablePositions.push(newPos);							
			break;														
		}																
	}
	LOOP_AND_PUSH( (uint32_t z=UnitZ+1; z<_Depth; z++), newPos.y = z );
	newPos.y = UnitZ;
	LOOP_AND_PUSH( (uint32_t x=UnitX; x>0; x--), newPos.x = x-1 );
	LOOP_AND_PUSH( (uint32_t x=UnitX+1; x<_Width; x++), newPos.x = x );

	if( bCastlingAvailable )
	{
		if( UnitX == (_Width-1) )
		{
			newPos.x = UnitX-1; newPos.y = UnitZ;
			bool bAvailableTiles = true;
			while( !unitMap[newPos.y][newPos.x] || unitMap[newPos.y][newPos.x]->Class != KING)
			{
				if( unitMap[newPos.y][newPos.x] )
				{
					bAvailableTiles = false; 
					break;
				}
				newPos.x--;
			}
			if( bAvailableTiles )
			{
				newPos.x = UnitX - ((newPos.x == 4) ? 2 : 3); newPos.y = UnitZ;
				lstAvailablePositions.push(newPos);
			}
		}
		else if( UnitX == 0 )
		{
			newPos.x = UnitX+1; newPos.y = UnitZ;
			bool bAvailableTiles = true;
			while( !unitMap[newPos.y][newPos.x] || unitMap[newPos.y][newPos.x]->Class != KING)
			{
				if( unitMap[newPos.y][newPos.x] )
				{
					bAvailableTiles = false; 
					break;
				}
				newPos.x++;
			}
			if( bAvailableTiles )
			{
				newPos.x = UnitX + ((newPos.x == 3) ? 2 : 3); newPos.y = UnitZ;
				lstAvailablePositions.push(newPos);
			}
		}
	}

	return lstAvailablePositions; 
}

template<typename _CellType, size_t _Width, size_t _Depth>
gepos2i	calculateAvailablePositionsQueen	( const SChessPiece* pawnPiece, uint32_t UnitX, uint32_t UnitZ, const _CellType (&unitMap)[_Depth][_Width] )
{ 
	// Rook
	gepos2i	lstAvailablePositions = calculateAvailablePositionsRook( pawnPiece, UnitX, UnitZ, unitMap, false ); 
	// +Bishop
	lstAvailablePositions.join( lstAvailablePositions, calculateAvailablePositionsBishop( pawnPiece, UnitX, UnitZ, unitMap ) );

	return lstAvailablePositions; 
}

template<typename _CellType, size_t _Width, size_t _Depth>
gepos2i	calculateAvailablePositionsKing	( const SChessPiece* pawnPiece, uint32_t UnitX, uint32_t UnitZ, const _CellType (&unitMap)[_Depth][_Width], bool bCastlingAvailableRook0, bool bCastlingAvailableRook1 )
{ 
	gepos2i	lstAvailablePositions; 
	id_t iPlayerTeam = pawnPiece->TeamID;
	SPointL newPos;
	SPointL deltas[8] = { 
		{	0,	1	}, 
		{	1,	0	}, 
		{	1,	1	}, 
		{	0,	-1	}, 
		{	-1,	0	}, 
		{	-1,	-1	}, 
		{	-1,	1	}, 
		{	1,	-1	}
	};
	for( uint32_t i=0; i<8; i++ ) // for 8 deltas
	{
		newPos.x = UnitX+deltas[i].x; newPos.y = UnitZ+deltas[i].y;
		if( PosInBoard(newPos, unitMap) && 
			( !unitMap[newPos.y][newPos.x]
			|| unitMap[newPos.y][newPos.x]->TeamID != iPlayerTeam ) )
			lstAvailablePositions.push(newPos);
	}

	if( bCastlingAvailableRook0 )
	{
		newPos.x = UnitX-1; newPos.y = UnitZ;
		bool bAvailableTiles = true;
		while(newPos.x > 0)
		{
			if( unitMap[newPos.y][newPos.x] )
			{
				bAvailableTiles = false; 
				break;
			}
			newPos.x--;
		}
		if( bAvailableTiles )
		{
			newPos.x = UnitX-2; newPos.y = UnitZ;
			lstAvailablePositions.push(newPos);
		}
	}
	if( bCastlingAvailableRook1 )
	{
		newPos.x = UnitX+1; newPos.y = UnitZ;
		bool bAvailableTiles = true;
		while(newPos.x < (_Width-1))
		{
			if( unitMap[newPos.y][newPos.x] )
			{
				bAvailableTiles = false; 
				break;
			}
			newPos.x++;
		}
		if( bAvailableTiles )
		{
			newPos.x = UnitX+2; newPos.y = UnitZ;
			lstAvailablePositions.push(newPos);
		}
	}

	return lstAvailablePositions; 
}

template<typename _CellType, size_t _Width, size_t _Depth>
error_t calculateAvailablePositionsForPlayer( const SPlayer* playerInfo, const klib::SGrid<_CellType, _Width, _Depth>& unitMap, gepos2i* out_AvailablePositions )
{ 
	const uint32_t agentCount = klib::SGameSquad::Size;
	for( uint32_t iUnit=0; iUnit<agentCount; iUnit++ )
	{
		const SChessPiece* playerUnit = &playerInfo->Pieces[iUnit];
		if( !playerUnit->Alive )
		{
			out_AvailablePositions[iUnit].clear();
			continue;
		}
		const uint32_t x=playerUnit->Position.x, z=playerUnit->Position.y;
		gepos2i	lstAvailablePositions;
		switch( playerUnit->Class )
		{
		case PAWN:		lstAvailablePositions = calculateAvailablePositionsPawn		(playerUnit, x, z, unitMap.Cells); break;
		case KNIGHT:	lstAvailablePositions = calculateAvailablePositionsKnight	(playerUnit, x, z, unitMap.Cells); break;
		case BISHOP:	lstAvailablePositions = calculateAvailablePositionsBishop	(playerUnit, x, z, unitMap.Cells); break;
		case QUEEN:		lstAvailablePositions = calculateAvailablePositionsQueen	(playerUnit, x, z, unitMap.Cells); break;
		case ROOK:		lstAvailablePositions = calculateAvailablePositionsRook		(playerUnit, x, z, unitMap.Cells, !playerInfo->bRooksMoved[(iUnit % 2) ? 1 : 0] && !playerInfo->bKingMoved ); break;
		case KING:		lstAvailablePositions = calculateAvailablePositionsKing		(playerUnit, x, z, unitMap.Cells, !playerInfo->bRooksMoved[0] && !playerInfo->bKingMoved, !playerInfo->bRooksMoved[1] && !playerInfo->bKingMoved); break;
		default:
			break;
		}
		out_AvailablePositions[iUnit] = lstAvailablePositions;
	}
	return 0; 
};

error_t calculateAvailablePositionsForFrame( SGameFrame* gameFrame )
{
	for( uint32_t iPlayer=0; iPlayer<MAX_PLAYERS; iPlayer++ )
		calculateAvailablePositionsForPlayer( &gameFrame->PlayerInfo[iPlayer], gameFrame->Board.Units, gameFrame->PlayerInfo[iPlayer].AvailablePositions ); 
	return 0;
}

bool isThreatenedPosition( id_t ThreatenedTeamID, const SPlayer* FuturePlayers, const SPointL& Position )
{
	for( uint32_t iPlayer=0; iPlayer<MAX_PLAYERS; iPlayer++ )
	{
		if( iPlayer == ThreatenedTeamID )
			continue;
		for( uint32_t iPiece =0; iPiece<klib::SGameSquad::Size; iPiece++ )
		{
			if( !FuturePlayers[iPlayer].Pieces[iPiece].Alive )
				continue;
			if( -1 != FuturePlayers[iPlayer].AvailablePositions[iPiece].find( Position ) )
			{
				int unitX = (int)FuturePlayers[iPlayer].Pieces[iPiece].Position.x;
				if( FuturePlayers[iPlayer].Pieces[iPiece].Class == PAWN 
					&& labs(unitX-Position.x) != 1 )
					continue;
				return true;
			}
		}
	}
	return false;
};
