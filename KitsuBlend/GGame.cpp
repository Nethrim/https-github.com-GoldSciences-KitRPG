#define NOMINMAX

/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GGame.h"
#include "GGame_function_macros.h"

#include <math.h>
#include <algorithm>

//#define GPROFILE_ENABLED
#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "draw.h"


using namespace god;

void generateUnitMinimap( const SGameFrame& currentGameFrame, uint32_t mapWidth, uint32_t mapDepth, id_t iPlayerTeam, GODS(SBuffer)* out_ImageBuffer );

error_t	CGGame::UpdateChessStuff( float fElapsedTime )
{ 

	const uint32_t width = klib::STacticalBoard::Width, depth = klib::STacticalBoard::Depth;
	for( uint32_t iPlayer=0; iPlayer < MAX_PLAYERS; iPlayer++ )
	{
		for( uint32_t iUnit=0; iUnit < klib::SGameSquad::Size; iUnit++ )
		{
			SChessPiece* pUnit = &m_CurrentGameFrame.PlayerInfo[iPlayer].Pieces[iUnit];
			SPointL pos = pUnit->Position;
			if( pUnit->pEntity )
			{
				if( iPlayer )
					pUnit->pEntity->SetPosition((float)pos.x+1, 0, -(float)(pos.y+1));
				else
					pUnit->pEntity->SetPosition((float)pos.x, 0, -(float)pos.y);
			}
		}
	}

	m_BodyIntegrator.UpdatePositions( fElapsedTime );
	m_BodyForceRegistry.UpdateGenerators( fElapsedTime );
	
	m_GameWorld.UpdateWorld(fElapsedTime);
	m_MeshWorld.UpdateWorld(fElapsedTime);
	CalculateAvailablePositions(); 
	generateUnitMinimap( m_CurrentGameFrame, width, depth, -1, &m_BufferUnitsCurrent );
	RefreshSpriteMinimapChess();

	m_UnitCursor.GlowAlphaTime += fElapsedTime*5;
	if( m_UnitCursor.GlowActive )
	{
		GCOLOR col(m_UnitCursor.Sprite.GetColor());
		float fw = sinf(m_UnitCursor.GlowAlphaTime)/(float)GMATH_PI2;
		col.a = std::min( 1.0f, fw/2+0.6f );
		m_UnitCursor.Sprite.SetColor( col );
	};

	return 0;
}

error_t	CGGame::Update( float fElapsedTime )
{ 
	__CGGAME_FUNCTION_HEADER( -1 );
	
	if(m_pGame) 
	{
		pollInput(m_pGame->FrameInput);
		drawAndPresentGame(*m_pGame);
		if(!m_pGame->bRunning)
			return ShutdownGame();
	}
	else 
		return 0;

	//

	UpdateChessStuff(fElapsedTime);

	return 0; 
}

void CGGame::RefreshSpriteMinimapChess( void )
{
	GPtrNCO(IGVideoImage) pImage;
	const uint32_t width = klib::STacticalBoard::Width, depth = klib::STacticalBoard::Depth;
	if( m_pVideoContext->CreateImageFromMemory(width, depth, m_BufferUnitsCurrent, GRESMODE_DEVICE_READ, &pImage ) )
	{
		error_printf("Failed to create image from memory for unit map texture!");
		return;
	}
	CGTexture tex(m_SpriteMinimap.GetTexture(0));
	tex.SetImage( pImage );
	m_SpriteMinimap.SetTexture(0, tex);
	uint32_t nMapWidth=width*MINIMAP_SCALE, nMapHeight=depth*MINIMAP_SCALE;
	uint32_t nOffsetX=m_pVideoManager->GetWidth(0)-nMapWidth-4, nOffsetY=4;
	GPtrPOD(STransform) xform;
	xform->Matrix.Scale( nMapWidth*1.0f, nMapHeight*1.0f, 1.0f, true );
	xform->Matrix.SetTranslation( SVector3((float)nOffsetX, (float)nOffsetY, 0.1f), false );
	xform->MatrixInverse.SetInverse( xform->Matrix );
	//m_SpriteMinimap.SetLocalTransform( xform );
	m_SpriteMinimap.SetGlobalTransform( xform );

	xform.clone(xform);
	//
	uint32_t nWidth = 24/4, nHeight = 24/4;
	//uint32_t nWidth = 8, nHeight = 8;

	xform->Matrix.Scale( nWidth*1.0f, nHeight*1.0f, 1.0f, true );
	xform->Matrix.SetTranslation( SVector3( m_pVideoManager->GetWidth(0)-nMapWidth-8.0f+(MINIMAP_SCALE)*m_UnitCursor.Position.x +3, (MINIMAP_SCALE)*m_UnitCursor.Position.y*1.0f +3, 0.0f ), false );
	xform->MatrixInverse.SetInverse( xform->Matrix );
	//m_UnitCursor.Sprite.SetLocalTransform( xform );
	m_UnitCursor.Sprite.SetGlobalTransform( xform );	
}

void CGGame::UpdateImageUnitsNextFromCursor( void )
{

};


error_t	CGGame::PlaySounds( float fElapsedTime )
{ 
	__CGGAME_FUNCTION_HEADER( -1 );
	return 0; 
}

void* god::CGGame::GetInterfacePointer( GINTERFACEID_TYPE InterfaceID )
{
	switch( InterfaceID )
	{
	case GIID_CUSTOMINTERFACE:
		return this;
	default:
		return 0;
	}
};

void CGGame::ResetBoard( void )
{
	int32_t x, z;
	const uint32_t width = klib::STacticalBoard::Width, depth = klib::STacticalBoard::Depth;
	for( z=0; z<depth; z++ )
	{
		for( x=0; x<width; x++ )
		{
			m_CurrentGameFrame.Board.Units.Cells[z][x] = 0;
			m_CurrentGameFrame.Board.ChangedTimes.Cells[z][x] = 0;
		}
	}
	for( uint32_t iPlayer=0; iPlayer<MAX_PLAYERS; iPlayer++ )
	{
		m_CurrentGameFrame.PlayerInfo[iPlayer].bKingMoved = m_CurrentGameFrame.PlayerInfo[iPlayer].bRooksMoved[0] = m_CurrentGameFrame.PlayerInfo[iPlayer].bRooksMoved[1] = false;
		m_CurrentGameFrame.PlayerInfo[iPlayer].TeamID = iPlayer;
		SChessPiece* p = m_CurrentGameFrame.PlayerInfo[iPlayer].Pieces;
		for( uint32_t iPiece=0; iPiece<klib::SGameSquad::Size; iPiece++ )
		{
			x = 4;
			z = iPlayer ? (depth-1) : 0;
			switch( p[iPiece].Class )
			{
			case PAWN:		{ x = iPiece; z = iPlayer ? (depth-2) : 1; }	break;
			case ROOK:		{ x = (iPiece % 2)*7;  }				break;
			case KNIGHT:	{ x = 1+(iPiece % 2)*5;  }				break;
			case BISHOP:	{ x = 2+(iPiece % 2)*3;  }				break;
			case QUEEN:		{ x = 3;  }								break;
			default:		break;
			};
			p[iPiece].Position	= {x, z};
			p[iPiece].TeamID	= m_CurrentGameFrame.PlayerInfo[iPlayer].TeamID;
			p[iPiece].Alive		= true;
			GQuaternion q;
			if( iPlayer )
				q = GQuaternion(0.0f, -(float)(GMATH_PI/2), 0.0f, (float)(GMATH_PI/2) );
			else
			{
				q = GQuaternion(0.0f, (float)(GMATH_PI/2), 0.0f, (float)(GMATH_PI/2) );
			}
			q.Normalize();

			p[iPiece].pEntity->SetOrientation(q);
			p[iPiece].pEntity->SetEntityID( iPiece );
			p[iPiece].pEntity->SetMaterial( iPlayer ? m_MaterialBlack : m_MaterialWhite );
			m_CurrentGameFrame.Board.Units.Cells[z][x] = &p[iPiece];
		}
	}

	m_bSelectionActive = false;
	m_UnitSelected = -1;
};

void CGGame::NextUnit( bool bReverse )
{
	bool bValidUnitAvailable = false;
	int32_t nFirstValidUnitAvailable = -1;

	for( uint32_t i=0; i<klib::SGameSquad::Size; i++ )
		if( m_ActivePlayer->Pieces[i].Alive )
		{
			bValidUnitAvailable = true;
			nFirstValidUnitAvailable = i;
			break;
		}

	if( !bValidUnitAvailable )
		return;

	uint32_t UnitSelected = m_UnitSelected;
	DeselectUnit();
	m_UnitSelected = UnitSelected;
	if( m_UnitSelected != -1 )
	{
		if( bReverse )
		{
			if( m_UnitSelected > 0 )
				m_UnitSelected--;
			else
				m_UnitSelected = 15;
			while( !m_ActivePlayer->Pieces[m_UnitSelected].Alive )
				NextUnit( bReverse );
		}
		else
		{
			if( m_UnitSelected < 15 ) 
				m_UnitSelected++;
			else
				m_UnitSelected = 0;
			while( !m_ActivePlayer->Pieces[m_UnitSelected].Alive )
				NextUnit( bReverse );
		}
	}
	else 
		m_UnitSelected = nFirstValidUnitAvailable;

	CGEntity* pEntity = m_ActivePlayer->Pieces[m_UnitSelected].pEntity;
	m_UnitCursor.Position = m_ActivePlayer->Pieces[m_UnitSelected].Position;
	m_SelectedPos = m_UnitCursor.Position;
	SVector3 vCam = SVector3( 4.0f, m_MainCamera.GetPosition().y, 4.0f )+SVector3((float)m_UnitCursor.Position.x, 0, (float)(m_UnitCursor.Position.y));
	m_MainCamera.SetPosition( vCam );
	vCam = SVector3( pEntity->GetPosition() )-vCam;
	vCam.Normalize();
	m_MainCamera.SetFront( vCam );
	m_MainCamera.SetRight( SVector3(GVECTOR3_UP).Cross(vCam).Normalize() );

	m_MaterialSelectedOrig = pEntity->GetMaterial();
	pEntity->SetMaterial(m_MaterialSelectedPiece);

	m_bSelectionActive = true;
};

error_t CGGame::SelectUnit( void )
{
	if( m_CurrentGameFrame.Board.Units.Cells[m_UnitCursor.Position.y][m_UnitCursor.Position.x] &&
		m_CurrentGameFrame.Board.Units.Cells[m_UnitCursor.Position.y][m_UnitCursor.Position.x]->TeamID == m_ActivePlayer->TeamID )
	{
		DeselectUnit();
		m_SelectedPos = m_UnitCursor.Position;
		m_bSelectionActive = true;
		m_UnitSelected = m_CurrentGameFrame.Board.Units.Cells[m_UnitCursor.Position.y][m_UnitCursor.Position.x]->pEntity->GetEntityID();
		CGEntity* pEntity;
		if( pEntity = m_CurrentGameFrame.Board.Units.Cells[m_SelectedPos.y][m_SelectedPos.x]->pEntity )
		{
			m_MaterialSelectedOrig = pEntity->GetMaterial();
			pEntity->SetMaterial( m_MaterialSelectedPiece );
		}
	}
	return 0;
}

error_t CGGame::DeselectUnit( void )
{
	if( m_UnitSelected != -1 )
	{
		if( m_CurrentGameFrame.Board.Units.Cells[m_SelectedPos.y][m_SelectedPos.x] )
		{
			CGEntity* pEntity;
			if( pEntity = m_CurrentGameFrame.Board.Units.Cells[m_SelectedPos.y][m_SelectedPos.x]->pEntity )
			{
				pEntity->SetMaterial( m_MaterialSelectedOrig );
			}

			m_bSelectionActive = false;
			m_UnitSelected = -1;
			SPointL p ={0,0};
			MoveUnitCursor(p);
		}
	}
	return 0;
};

bool moveUnit( SPlayer* pPlayer, uint32_t iPiece, SChessBoard* Board, const SPointL& TargetPos )
{
	SChessPiece* currentUnit = &pPlayer->Pieces[iPiece];
	const SPointL& sourcePos = currentUnit->Position;
	switch( pPlayer->Pieces[iPiece].Class )
	{
	case KING:
		//if( IsThreatenedPosition( pPlayer->TeamID, 0, TargetPos ) )
		//	return false;
	case ROOK:
	case PAWN:
	case KNIGHT:
	case BISHOP:
	case QUEEN:
	default:
		break;
	};
	if( -1 != pPlayer->AvailablePositions[iPiece].find( TargetPos ) )
	{
		SChessPiece* EnemyUnit = Board->Units.Cells[TargetPos.y][TargetPos.x];
		if( EnemyUnit 
			&& EnemyUnit->TeamID != pPlayer->TeamID
			) // can only kill valid enemy units
		{
			EnemyUnit->Position = {0, -1};
			EnemyUnit->Alive	= false;
		}
		Board->Units.Cells[currentUnit->Position.y][currentUnit->Position.x] = 0;
		Board->Units.Cells[TargetPos.y][TargetPos.x] = currentUnit;
		currentUnit->Position = TargetPos;
		currentUnit->MoveCount++;
		currentUnit->TotalTilesWalked += abs(sourcePos.y-TargetPos.y) + abs(sourcePos.x-TargetPos.x);
		Board->ChangedTimes.Cells[sourcePos.y][sourcePos.x]	++;
		Board->ChangedTimes.Cells[TargetPos.y][TargetPos.x]	++;
		return true;
	};
	return false;
};

error_t CGGame::MoveUnitCursor( SPointL& cursorDeltas )
{
	//if( 0 == cursorDeltas.x && 0 == cursorDeltas.y )
	//	return 0;
	const uint32_t width = klib::STacticalBoard::Width, depth = klib::STacticalBoard::Depth;
	if( cursorDeltas.x ) m_UnitCursor.Position.x = std::min(std::max(0, m_UnitCursor.Position.x + cursorDeltas.x), (int32_t)width-1);
	if( cursorDeltas.y ) m_UnitCursor.Position.y = std::min(std::max(0, m_UnitCursor.Position.y + cursorDeltas.y), (int32_t)depth-1);
	if( m_UnitSelected != -1 )
	{
		m_FutureGameFrameFromCursor = m_CurrentGameFrame;
		memset( &m_FutureGameFrameFromCursor.Board.Units.Cells[0][0], 0, sizeof(m_FutureGameFrameFromCursor.Board.Units) );
		for( uint32_t iPlayer=0; iPlayer<MAX_PLAYERS; iPlayer++ )
		{
			for( uint32_t iPiece=0; iPiece<klib::SGameSquad::Size; iPiece++ )
			{
				SChessPiece* pPiece = &m_FutureGameFrameFromCursor.PlayerInfo[iPlayer].Pieces[iPiece];
				if( !pPiece->Alive )
					continue;
				SPointL& pos = pPiece->Position;
				m_FutureGameFrameFromCursor.Board.Units.Cells[pos.y][pos.x] = pPiece;
			}
		}
		moveUnit( &m_FutureGameFrameFromCursor.PlayerInfo[m_ActivePlayer->TeamID], m_UnitSelected, &m_FutureGameFrameFromCursor.Board, m_UnitCursor.Position );
		calculateAvailablePositionsForFrame( &m_FutureGameFrameFromCursor );
	}
	CGEntity* pCursorEntity = m_MeshWorld.GetChildList()[CURSOR];
	pCursorEntity->SetPosition( (float)m_UnitCursor.Position.x, 0, -(float)(m_UnitCursor.Position.y+1) );
	bool isValidPosition = false;

	if( m_CurrentGameFrame.Board.Units.Cells[m_UnitCursor.Position.y][m_UnitCursor.Position.x] 
		&& m_CurrentGameFrame.Board.Units.Cells[m_UnitCursor.Position.y][m_UnitCursor.Position.x]->TeamID == m_ActivePlayer->TeamID )
	{
		m_UnitCursor.GlowActive		= true;
		m_UnitCursor.GlowAlphaTime	= 0.0f;
		m_UnitCursor.Sprite.SetColor( GCYAN );
		pCursorEntity->SetMaterial(m_UnitCursor.Select);
	}
	else if( m_UnitSelected != -1 )
	{
		isValidPosition = -1 != m_ActivePlayer->AvailablePositions[m_UnitSelected].find(m_UnitCursor.Position);
		if( !isValidPosition )
		{
			pCursorEntity->SetMaterial(m_UnitCursor.Invalid);
			m_UnitCursor.Sprite.SetColor( GRED );
			m_UnitCursor.GlowActive = false;
		}
		else
		{
			pCursorEntity->SetMaterial(m_UnitCursor.Valid);
			m_UnitCursor.Sprite.SetColor( GGREEN );
			m_UnitCursor.GlowAlphaTime	= 0.0f;
			m_UnitCursor.GlowActive		= true;
		}
	}
	else
	{
		pCursorEntity->SetMaterial(m_UnitCursor.Empty);
		m_UnitCursor.Sprite.SetColor( GWHITE );
		m_UnitCursor.GlowActive = false;
	}


	return 0;
}

error_t CGGame::MoveUnit( SPlayer* pPlayer, uint32_t iPiece, const SPointL& TargetPos )
{
	uint32_t iUnitSelected=m_UnitSelected;
	if( m_CurrentGameFrame.Board.Units.Cells[TargetPos.y][TargetPos.x] && 
		m_CurrentGameFrame.Board.Units.Cells[TargetPos.y][TargetPos.x]->TeamID == pPlayer->TeamID )
	{
		DeselectUnit();
		SelectUnit();
		return false;
	}

	if( moveUnit( pPlayer, iPiece, &m_CurrentGameFrame.Board, TargetPos ) )
	{
		m_SelectedPos = TargetPos;
		DeselectUnit();
		return true;
	}
	return false;
}

error_t CGGame::CalculateAvailablePositions( void )
{
	return calculateAvailablePositionsForFrame( &m_CurrentGameFrame );
}

void generateUnitMinimap( const SGameFrame& currentGameFrame, uint32_t mapWidth, uint32_t mapDepth, id_t iPlayerTeam, uint32_t* out_pixels );
void generateUnitMinimap( const SGameFrame& currentGameFrame, uint32_t mapWidth, uint32_t mapDepth, id_t iPlayerTeam, GODS(SBuffer)* out_ImageBuffer )
{
	if( 0 == out_ImageBuffer )
	{
		error_printf("Won't create an image if the output address is NULL.");
		return;
	}
	GPtrNCO(SBuffer) newImage;
	uint32_t mapSize = mapWidth*mapDepth;
	error_t errMy = gcreateBuffer( GDATA_TYPE_UINT8_4, GUSAGE_TEXEL, mapSize, &newImage );
	if( 0 > errMy )
	{
		error_printf("Failed to create buffer for updating unit map texture! Out of memory?");
		return;
	};
	memset( newImage->pByteArray, 0, sizeof(uint32_t)*mapSize );
	generateUnitMinimap( currentGameFrame, mapWidth, mapDepth, iPlayerTeam, (uint32_t*)newImage->pByteArray );
	//for( uint32_t z=0; z<mapDepth; z++ )
	//{
	//	for( uint32_t x=0; x<mapWidth; x++ )
	//	{
	//		GCOLOR unitColor = {};
	//		if( currentGameFrame.Board.Units[z][x] 
	//			&& (-1 == iPlayerTeam) || currentGameFrame.Board.Units[z][x]->TeamID == iPlayerTeam )
	//		{
	//			switch( currentGameFrame.Board.Units[z][x]->Class )
	//			{
	//			case PAWN:		unitColor = GRED;		break;
	//			case ROOK:		unitColor = GGREEN;		break;
	//			case KNIGHT:	unitColor = GBLUE;		break;
	//			case BISHOP:	unitColor = GYELLOW;	break;
	//			case QUEEN:		unitColor = GMAGENTA;	break;
	//			case KING:		unitColor = GCYAN;		break;
	//			default:		
	//				warning_printf( "unknown piece class: 0x%X at location (x=%u, z=%u)", currentGameFrame.Board.Units[z][x]->Class, x, z );
	//				unitColor = GORANGE;				
	//				break;
	//			}
	//			//if( currentGameFrame.Board.Units[z][x]->TeamID != 0 )
	//			//	unitColor*=.5f;
	//			unitColor *= (float)(1/currentGameFrame.Board.Units[z][x]->TeamID);
	//		}
	//		else
	//			unitColor = 0x00000000U; // GBLACK;
	//
	//		uint32_t index = z*mapWidth+x;
	//		((uint32_t*)newImage->pByteArray)[index] = unitColor;
	//	}
	//}
	gswap(out_ImageBuffer, newImage);
};

void generateUnitMinimap( const SGameFrame& currentGameFrame, uint32_t mapWidth, uint32_t mapDepth, id_t iPlayerTeam, unsigned int* out_pixels )
{
	if( 0 == out_pixels )
	{
		error_printf("Won't draw in buffer if the output array is NULL.");
		return;
	}
	for( uint32_t z=0; z<mapDepth; z++ )
	{
		for( uint32_t x=0; x<mapWidth; x++ )
		{
			GCOLOR unitColor = {};
			if( currentGameFrame.Board.Units.Cells[z][x] 
				&& (-1 == iPlayerTeam || currentGameFrame.Board.Units.Cells[z][x]->TeamID == iPlayerTeam)
			)
			{
				switch( currentGameFrame.Board.Units.Cells[z][x]->Class )
				{
				case PAWN:		unitColor = GRED;		break;
				case ROOK:		unitColor = GGREEN;		break;
				case KNIGHT:	unitColor = GBLUE;		break;
				case BISHOP:	unitColor = GYELLOW;	break;
				case QUEEN:		unitColor = GMAGENTA;	break;
				case KING:		unitColor = GCYAN;		break;
				default:		
					warning_printf( "unknown piece class: 0x%X at location (x=%u, z=%u)", currentGameFrame.Board.Units.Cells[z][x]->Class, x, z );
					unitColor = GORANGE;				
					break;
				}
				//if( currentGameFrame.Board.Units[z][x]->TeamID != 0 )
				//	unitColor*=.5f;
				//if( currentGameFrame.Board.Units[z][x]->TeamID )
					unitColor *= 1.0f/(currentGameFrame.Board.Units.Cells[z][x]->TeamID+1);
				uint32_t index = z*mapWidth+x;
				out_pixels[index] = unitColor;
				//if( -1 != iPlayerTeam )
					out_pixels[index] = currentGameFrame.PlayerInfo[currentGameFrame.Board.Units.Cells[z][x]->TeamID].Color;
			}
		}
	}
};