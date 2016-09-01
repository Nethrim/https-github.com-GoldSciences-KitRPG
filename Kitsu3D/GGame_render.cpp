// File:GGame.cpp
//
// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
// 
//
#include "GGame.h"
#include "GGame_function_macros.h"
#include <math.h>
#ifdef WIN32
#include <crtdbg.h>
#include <strsafe.h>
#endif

//#define GPROFILE_ENABLED
#include "GCore_printf.h"
#include "GDebugger_core.h"

using namespace god;

void CalcFrameRate( wchar_t* buffer, uint32_t size, float32_t fElapsedTime, uint32_t Width, uint32_t Height );

//----------- Main loop function calls --------------
// Draw everything.
int32_t CGGame::Draw( float fElapsedTime )
{
	__CGGAME_FUNCTION_HEADER( -1 );
	//------------------------------------------------------------
	// Do your drawing here
	//------------------------------------------------------------
	error_t result=0;
	GPtrNCO(IGVideoDevice) pDevice = m_pVideoManager;
	SMatrix4 rot, trans;
	//uint32_t nTerrainWidth = m_pTerrainEntity->GetTerrainWidth(),
	//	nTerrainDepth = m_pTerrainEntity->GetTerrainDepth();
	pDevice->EndRender();
	pDevice->SetTargetImage( m_TileSetRenderTarget, 0 );
	pDevice->SetClearColorValue( GCOLOR( 0xCDCDCDCD ) );	// for testing
	pDevice->Clear(true, true, true);
	pDevice->EnableTargetAreaTest();
	//pDevice->SetFillMode( GFILL_MODE_WIREFRAME );
	for( uint32_t iTileSet=0; iTileSet<MAX_TILESETS; iTileSet++ )
	{
		//m_World.SetViewport( &m_TileSetViewport[iTileSet] );
		//m_World.SetCamera( &m_TileSetCamera[iTileSet] );
		pDevice->SetTargetArea( (iTileSet%4)*m_nViewportWidth, (iTileSet/4)*m_nViewportHeight, m_nViewportWidth, m_nViewportHeight );
		//pDevice->SetViewportArea( (iTileSet%4)*m_nViewportWidth, (iTileSet/4)*m_nViewportHeight, m_nViewportWidth, m_nViewportHeight );
		pDevice->StartRender();
		//if( result = m_World.RenderWorld(fElapsedTime) )
		if( result = m_TileSetWorld[iTileSet].RenderWorld(fElapsedTime) )
		{
			error_printf( "CGGameWorld::RenderWorld(%f) returned 0x%X!", fElapsedTime, result );
			return -1;
		}
		pDevice->EndRender();
	}
	pDevice->DisableTargetAreaTest();
	
	//pDevice->SetFillMode( GFILL_MODE_SOLID );
	//
	//m_World.SetViewport( m_MeshRenderer.GetViewport() );
	//m_World.SetCamera( &m_Camera );
	pDevice->SetTargetImage( 0, 0 );
	pDevice->SetClearColorValue( GCOLOR( 0xFF1212FF ) );	// for testing
	pDevice->Clear(true, true, true);
	//pDevice->SetBackfaceCulling( GCULL_MODE_CCW );
	pDevice->StartRender();
	if( result = m_World.RenderWorld(fElapsedTime) )
	{
		error_printf( "CGGameWorld::RenderWorld(%f) returned 0x%X!", fElapsedTime, result );
		return -1;
	}
	
	//pDevice->SetBackfaceCulling( GCULL_MODE_NONE );
	if( result = m_SpriteRenderer.DrawNodes(fElapsedTime) )
	{
		error_printf( "CGSpriteRenderer::DrawNodes(%f) returned 0x%X!", fElapsedTime, result );
		return -1;
	}

	GPtrPOD(SRectangle) TargetRect;
	TargetRect->vOffset	= GVECTOR2_ZERO;
	TargetRect->vSize	= GVECTOR2_ONE;
	char buffer[1024], format[1024] = "Selected tile corners: %s, %s, %s, %s.\n", *pTmpBack = "", *pTmpLeft = "", *pTmpFront = "", *pTmpRight = "", 
		*pTextBack="Back", *pTextLeft="Left", *pTextFront="Front", *pTextRight="Right";
	
	
	if( m_SelectedTileCorners & GTILEBORDER_BACK )
	{
		pTmpBack	= pTextBack;
	}
	if( m_SelectedTileCorners & GTILEBORDER_LEFT )
	{
		pTmpLeft	= pTextLeft;
	}
	if( m_SelectedTileCorners & GTILEBORDER_FRONT )
	{
		pTmpFront	= pTextFront;
	}
	if( m_SelectedTileCorners & GTILEBORDER_RIGHT )
	{
		pTmpRight	= pTextRight;
	}

	if( 0 == (m_SelectedTileCorners & (GTILEBORDER_BACK | GTILEBORDER_FRONT)) )
	{
		pTmpBack	= pTextBack;
		pTmpFront	= pTextFront;
	}

	if( 0 == (m_SelectedTileCorners & (GTILEBORDER_LEFT | GTILEBORDER_RIGHT)) )
	{
		pTmpLeft	= pTextLeft;
		pTmpRight	= pTextRight;
	}

	StringCchPrintfA( buffer, 1024, format, pTmpBack, pTmpLeft, pTmpFront, pTmpRight );

#define DT_LEFT                     0x00000000
#define DT_BOTTOM                   0x00000008
	m_Font->Draw( buffer, -1, TargetRect, true, DT_BOTTOM | DT_LEFT );
	return 0;
};





void CalcFrameRate( wchar_t* buffer, uint32_t size, float fElapsedTime, uint32_t Width, uint32_t Height )
{
	// with this we Draw some info
	static float fTime			= 0.0f;
	static float fAverageTime	= 0.0f;
	static unsigned int nFPS		= 0; // frames per second
	static unsigned int nFrameCount	= 0; 

	fTime += fElapsedTime;
	nFrameCount++;
	fAverageTime = gmax( fAverageTime, fElapsedTime );

	if( fTime >= 1.0f )
	{
		nFPS = nFrameCount;
		fTime = 0.0f;
		nFrameCount = 0;
		fAverageTime = fElapsedTime;
	}

	StringCchPrintfW(	buffer, size, 
						L"Current frame time: %f\n"
						L"Average frame time: %f\n"
						L"Frames per second: %u\n"
						L"Screen resolution: %u x %u", 
						fElapsedTime*1000, fAverageTime*1000, nFPS, Width, Height
					);
}
