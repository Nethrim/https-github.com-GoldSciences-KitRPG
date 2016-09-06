//#define GPROFILE_ENABLED
/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GEntity.h"
#include "Game.h"

typedef god::gelementaryex2_old<god::SPointL, god::GDATA_TYPE_INT32_2, god::GUSAGE_POSITION> gepos2i;

enum ENTITY_TYPE : uint8_t
{	BOARD	
,	PAWN	
,	ROOK	
,	KNIGHT	
,	BISHOP	
,	QUEEN	
,	KING	
,	CURSOR	
};

typedef uint8_t PIECECLASS_TYPE;
static const PIECECLASS_TYPE	PIECECLASS_BOARD	= 0;
static const PIECECLASS_TYPE	PIECECLASS_PAWN		= 1;
static const PIECECLASS_TYPE	PIECECLASS_ROOK		= 2;
static const PIECECLASS_TYPE	PIECECLASS_KNIGHT	= 3;
static const PIECECLASS_TYPE	PIECECLASS_BISHOP	= 4;
static const PIECECLASS_TYPE	PIECECLASS_QUEEN	= 5;
static const PIECECLASS_TYPE	PIECECLASS_KING		= 6;
static const PIECECLASS_TYPE	PIECECLASS_CURSOR	= 7;

#define MAX_PLAYERS 2
//   A R G B 
// 0xFFFF0000	red
// 0xFF00FF00	green
// 0xFF0000FF	blue
// 0xFFFFFF00	yellow
// 0xFFFF00FF	magenta
// 0xFF00FFFF	cyan

struct SChessPiece
{
	PIECECLASS_TYPE		Class;	// type of chess piece
	god::id_t			TeamID;	// player 0 or 1
	uint32_t			MoveCount;	// the amount of movements done by a piece
	uint32_t			TotalTilesWalked;	// the amount of tiles a piece has moved
	uint32_t			Alive;		// tells if this piece is still playing the game
	god::SPointL		Position;	// the position in the board.
	god::CGEntity*		pEntity;	// pointer to engine object that holds display and animation data of the entity
};

struct SChessBoard
{
	klib::SGrid<SChessPiece*,	klib::STacticalBoard::Width, klib::STacticalBoard::Depth>	Units			= {};
	klib::SGrid<uint32_t,		klib::STacticalBoard::Width, klib::STacticalBoard::Depth>	ChangedTimes	= {};
};

struct SPlayer
{
	bool			bRooksMoved[2];
	bool			bKingMoved;
	SChessPiece		Pieces[klib::SGameSquad::Size];
	god::id_t		TeamID;
	god::GCOLOR		Color;
	god::gelementaryex2_old<god::SPointL, god::GDATA_TYPE_INT32_2, god::GUSAGE_POSITION>	AvailablePositions[klib::SGameSquad::Size];	// hold for each piece, the list of valid positions to move to
};

struct SGameFrame
{
	SChessBoard														Board;
	SPlayer															PlayerInfo[MAX_PLAYERS];
};

template<typename _CellType, size_t _Width, size_t _Depth>
static inline bool PosInBoard( const god::SPointL& point, const _CellType (&board)[_Depth][_Width] )
{
	return point.x >= 0 && point.x < _Width && point.y >= 0 && point.y < _Depth;
};

god::error_t calculateAvailablePositionsForFrame( SGameFrame* gameFrame );
bool isThreatenedPosition( god::id_t ThreatenedTeamID, const SPlayer* FuturePlayers, const god::SPointL& Position );