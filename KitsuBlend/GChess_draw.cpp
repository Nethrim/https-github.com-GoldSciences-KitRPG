
#ifndef min
#define min( a, b ) ( ((a)>(b)) ? (b) : (a) )
#endif 
#ifndef max
#define max( a, b ) ( ((a)<(b)) ? (b) : (a) )
#endif 
/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GGame.h"
#include "GGame_function_macros.h"

//#define GPROFILE_ENABLED
#include "GCore_printf.h"
#include "GDebugger_core.h"


using namespace god;


int32_t	CGGame::Draw( float fElapsedTime )
{ 
	__CGGAME_FUNCTION_HEADER( -1 );

	error_t result=0;
	//__CGGAME_RENDERER_DRAWNODES_CALL( Mesh );
	m_pVideoManager->SetBackfaceCulling(GCULL_MODE_CCW);
	if( m_GameWorld.RenderWorld(fElapsedTime) )
	{
		error_printf( "m_GameWorld.RenderWorld() failed!" );
	}
	m_pVideoManager->SetBackfaceCulling(GCULL_MODE_NONE);
	if( m_MeshWorld.RenderWorld(fElapsedTime) )
	{
		error_printf( "m_MeshWorld.RenderWorld() failed!" );
	}
	__CGGAME_RENDERER_DRAWNODES_CALL( Sprite );

	GPtrNCO(IGFont) m_Font = m_MeshRenderer.GetInfoFont();

	GPtrPOD(SRectangle) rc;
	rc->vOffset = GVECTOR2_ZERO;
	rc->vSize	= GVECTOR2_ONE;
	char buffer[1024];
	sprintf_s(buffer, 
		"m_UnitSelected = %i\n"
		"m_bSelectionActive = %i\n"
		"m_CursorPos = x=%i, y=%i\n"
		"m_SelectedPos = x=%i, y=%i\n"
		"Is position available = %i\n"
		"Is position threatened = %i"
		, m_UnitSelected
		, m_bSelectionActive
		, m_UnitCursor.Position.x, m_UnitCursor.Position.y
		, m_SelectedPos.x, m_SelectedPos.y
		, (m_UnitSelected == -1) ? 0 : ( (-1 != m_ActivePlayer->AvailablePositions[m_UnitSelected].find(m_UnitCursor.Position)) ? 1 : 0 )
		, (m_UnitSelected == -1) ? 0 : (-1 != m_ActivePlayer->AvailablePositions[m_UnitSelected].find(m_UnitCursor.Position)) && isThreatenedPosition( m_ActivePlayer->TeamID, m_FutureGameFrameFromCursor.PlayerInfo, m_UnitCursor.Position )
	);
	m_Font->Draw( buffer, -1, rc, true, 0, 0xFFFFFFFF );

	return result; 
}
