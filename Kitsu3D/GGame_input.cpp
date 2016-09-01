// File:GGame_input.cpp
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


//------------------------------------------------------------
// Read and process user input.
//------------------------------------------------------------
int32_t CGGame::ProcessUserInput( float fElapsedTime )
{
	__CGGAME_FUNCTION_HEADER( -1 );
	GPtrNCO(IGKeyboard) pKeyboard = m_pKeyboardManager;
	if( pKeyboard->IsKeyDown( GKEY_LALT ) && pKeyboard->IsKeyDown( GKEY_RETURN ) )
	{// ALT + Enter toggles fullscreen mode.
		if( m_pFramework->ToggleFullScreen() )
			return -1;
		return 0;
	}
	if( pKeyboard->IsKeyDown( GKEY_LCONTROL ) && pKeyboard->IsKeyDown( GKEY_Q ) )
	{// CTRL + Q Exits the application.
		ShutdownGame();
		return 0;
	}
	GPtrPOD(SCamera) pCameraData = m_Camera.GetCameraData();

	SVector3 vCam;
	float fScale = 3;
	if( pKeyboard->IsKeyDown( GKEY_LSHIFT ) || pKeyboard->IsKeyDown( GKEY_RSHIFT ) )
		fScale *= 5;

	if( pKeyboard->IsKeyDown( GKEY_W ) )
	{
		vCam	= *GVectorNormalizeInPlace( &pCameraData->Front );
		vCam.y	= 0;
		GVectorAddScaledInPlace( &pCameraData->Position, &vCam, fElapsedTime*fScale );
	}
	if( pKeyboard->IsKeyDown( GKEY_S ) )
	{
		vCam	= *GVectorNormalizeInPlace( &pCameraData->Front );
		vCam.y	= 0;
		GVectorSubstractScaledInPlace( &pCameraData->Position, &vCam, fElapsedTime*fScale );
	}
	if( pKeyboard->IsKeyDown( GKEY_D ) )
		GVectorAddScaledInPlace( &pCameraData->Position, GVectorNormalizeInPlace( &pCameraData->Right ),fElapsedTime*fScale );
	if( pKeyboard->IsKeyDown( GKEY_A ) )
		GVectorSubstractScaledInPlace( &pCameraData->Position, GVectorNormalizeInPlace( &pCameraData->Right ), fElapsedTime*fScale );
	//if( pKeyboard->IsKeyDown( GKEY_SPACE ) )
	//	pCameraData->Position.y += 1.0f*fElapsedTime*fScale;
	if( pKeyboard->IsKeyDown( GKEY_C) )
		pCameraData->Position.y -= 1.0f*fElapsedTime*fScale;

	uint32_t nBaseTileCount	= m_pTerrainEntity->GetBaseTileCount(),
		nTerrainWidth = m_pTerrainEntity->GetTerrainWidth(),
		nTerrainDepth = m_pTerrainEntity->GetTerrainDepth(),

		nBackTileIndex			= GetBackTileIndex2D( m_nSelectedTile, nTerrainWidth ), //, m_pTerrainEntity->GetTerrainWidth(), m_pTerrainEntity->GetTerrainDepth() ),
		nLeftTileIndex			= GetLeftTileIndex2D( m_nSelectedTile, nTerrainWidth ),		//, m_pTerrainEntity->GetTerrainDepth() ),
		nFrontTileIndex			= GetFrontTileIndex2D( m_nSelectedTile, nTerrainWidth, nTerrainDepth ),
		nRightTileIndex			= GetRightTileIndex2D( m_nSelectedTile, nTerrainWidth, nTerrainDepth ),

		nBackLeftTileIndex		= GetBackLeftTileIndex2D( m_nSelectedTile, nTerrainWidth ),	// m_pTerrainEntity->GetTerrainDepth() ),
		nBackRightTileIndex		= GetBackRightTileIndex2D( m_nSelectedTile, nTerrainWidth, nTerrainDepth ),
		nFrontLeftTileIndex		= GetFrontLeftTileIndex2D( m_nSelectedTile, nTerrainWidth ),	// m_pTerrainEntity->GetTerrainDepth() ),
		nFrontRightTileIndex	= GetFrontRightTileIndex2D( m_nSelectedTile, nTerrainWidth, nTerrainDepth );

	if( pKeyboard->IsKeyDown( GKEY_NUMADD ) || pKeyboard->IsKeyDown( GKEY_NUMSUBS ) )
	{
		GPtrPOD(STileMapping) SelectedTile
			, SelectedTileBackLeft, SelectedTileLeft, SelectedTileFrontLeft
			, SelectedTileBack, SelectedTileFront
			, SelectedTileBackRight, SelectedTileRight, SelectedTileFrontRight;

			SelectedTile			=	m_pTerrainEntity->GetTileMappingData( m_nSelectedTile ),
			SelectedTileBack		=	nBackTileIndex	== -1 ? 0 : m_pTerrainEntity->GetTileMappingData( nBackTileIndex ),
			SelectedTileLeft		=	nLeftTileIndex	== -1 ? 0 : m_pTerrainEntity->GetTileMappingData( nLeftTileIndex ),
			SelectedTileFront		=	nFrontTileIndex	== -1 ? 0 : m_pTerrainEntity->GetTileMappingData( nFrontTileIndex ),
			SelectedTileRight		=	nRightTileIndex	== -1 ? 0 : m_pTerrainEntity->GetTileMappingData( nRightTileIndex ),

			SelectedTileBackLeft	=	nBackLeftTileIndex		== -1 ? 0 : m_pTerrainEntity->GetTileMappingData( nBackLeftTileIndex ),
			SelectedTileBackRight	=	nBackRightTileIndex		== -1 ? 0 : m_pTerrainEntity->GetTileMappingData( nBackRightTileIndex ),
			SelectedTileFrontLeft	=	nFrontLeftTileIndex		== -1 ? 0 : m_pTerrainEntity->GetTileMappingData( nFrontLeftTileIndex ),
			SelectedTileFrontRight	=	nFrontRightTileIndex	== -1 ? 0 : m_pTerrainEntity->GetTileMappingData( nFrontRightTileIndex );

		GPtrNCO(IGVideoBuffer) pVertexBuffer	= m_pTerrainEntity->GetBuffer( 0 ).GetElementBuffer();
		GPtrNCO(SBuffer) VertexData = pVertexBuffer->GetBufferData();
		SVector3* pVertices				= (SVector3*)VertexData->pByteArray;

		float fHeights[4] = { 
			pVertices[SelectedTile->VerticesTop[0]].y, 
			pVertices[SelectedTile->VerticesTop[1]].y, 
			pVertices[SelectedTile->VerticesTop[2]].y, 
			pVertices[SelectedTile->VerticesTop[3]].y 
		};
		if( pKeyboard->IsKeyDown( GKEY_NUMADD ) )
		{
			for( uint32_t hidx = 0; hidx < 4; hidx++ )
				fHeights[hidx]	+= 0.1f*fScale;

		}
		else if( pKeyboard->IsKeyDown( GKEY_NUMSUBS ) )
		{
			for( uint32_t hidx = 0; hidx < 4; hidx++ )
				fHeights[hidx]	-= 0.1f*fScale;

		};
		setTileCornerHeights(	m_SelectedTileCorners, 
								SelectedTile.get_address(), 
								SelectedTileBackLeft.get_address(), 
								SelectedTileLeft.get_address(), 
								SelectedTileFrontLeft.get_address(), 
								SelectedTileBack.get_address(), 
								SelectedTileFront.get_address(), 
								SelectedTileBackRight.get_address(), 
								SelectedTileRight.get_address(), 
								SelectedTileFrontRight.get_address(), 
								fHeights,
								pVertices );
		VertexData.free();
		pVertexBuffer->Flush();
	}


	pCameraData->Up = SVector3( 0.0f, 1.0f, 0.0f );
	float fTileScale = m_pTerrainEntity->GetTileScale();
	GVectorCrossAndNormalize( &pCameraData->Right, &pCameraData->Up, GVectorNormalizeInPlace( &pCameraData->Front ) );


	uint32_t nIndexX = min( max( 0, (int)(pCameraData->Position.x/fTileScale) ), (int)m_pTerrainEntity->GetTerrainWidth()-1 ),
			nIndexZ = min( max( 0, -(int)(pCameraData->Position.z/fTileScale) ), (int)m_pTerrainEntity->GetTerrainDepth()-1 );
	GPtrPOD(STileGeometry) Tile = gacquire( m_pTerrainEntity->GetTileGeometryList()[nIndexZ][nIndexX] );

	double factorX = (pCameraData->Position.x/fTileScale - (int)(pCameraData->Position.x/fTileScale) )
		, factorZ = -( (pCameraData->Position.z/fTileScale - (int)(pCameraData->Position.z/fTileScale) ) );

	double	Height0 = ( Tile->fHeight[0]*(1.0-factorX)	*(1.0-factorZ)	),
			Height1 = ( Tile->fHeight[1]*(factorX)		*(1.0-factorZ)	),
			Height2 = ( Tile->fHeight[2]*(1.0-factorX)	*(factorZ)		),
			Height3 = ( Tile->fHeight[3]*(factorX)		*(factorZ)		);
	double averageHeight = (Height0+Height1+Height2+Height3)+1.8;

	if( (averageHeight - (double)pCameraData->Position.y) < -.05 )
		m_bFalling = true;

	if( !m_bFalling )
		pCameraData->Position.y = (float)averageHeight;
	else
	{
		m_verticalSpeed -= 9.8f*fElapsedTime*fTileScale;
		pCameraData->Position.y += m_verticalSpeed*fElapsedTime;
		if( pCameraData->Position.y < averageHeight )
		{
			pCameraData->Position.y = (float)averageHeight;
			m_verticalSpeed = 0;
			m_bFalling = false;
		}
	}
	return 0;
};


void CGGame::OnKeyUp( unsigned char key )
{
	int32_t nIndex=m_nSelectedTile;
	uint32_t nTerrainWidth = m_pTerrainEntity->GetTerrainWidth(),
		nTerrainDepth = m_pTerrainEntity->GetTerrainDepth();
	switch( key )
	{
	case GKEY_LEFT:
		nIndex	= GetBackTileIndex2D( m_nSelectedTile, nTerrainWidth );
		break;
	case GKEY_RIGHT:
		nIndex	= GetFrontTileIndex2D( m_nSelectedTile, nTerrainWidth, nTerrainDepth );
		break;
	case GKEY_UP:
		nIndex	= GetLeftTileIndex2D( m_nSelectedTile, nTerrainWidth );
		break;
	case GKEY_DOWN:
		nIndex	= GetRightTileIndex2D( m_nSelectedTile, nTerrainWidth, nTerrainDepth );
		break;
	case GKEY_SPACE:
		//if( !m_bFalling )
		//{
			m_bFalling = true;
			m_verticalSpeed = 2.5f;
		//}
		break;
	case GKEY_TAB:
		if( m_pKeyboardManager->IsKeyDown(GKEY_LSHIFT) || m_pKeyboardManager->IsKeyDown(GKEY_RSHIFT))
			m_SelectedTileCorners--;
		else
			m_SelectedTileCorners++;
	}
	if( nIndex != -1 && nIndex < (int32_t)m_pTerrainEntity->GetBaseTileCount() )
		m_nSelectedTile = nIndex;
};

void CGGame::OnKeyDown( unsigned char key )
{

};

void CGGame::OnMouseMove( int32_t x, int32_t y, int32_t z )
{
	GPROFILE_METHOD( "CGGame", "OnMouseMove" );
	GPtrPOD(SCamera) pCameraData = m_Camera.GetCameraData();

	float fScale = 1.0f/150.0f, fScaleZ = 0.01f;
	GPtrNCO(IGKeyboard) pKeyboard = m_pKeyboardManager;
	if( pKeyboard->IsKeyDown( GKEY_LSHIFT ) || pKeyboard->IsKeyDown( GKEY_RSHIFT ) )
		fScaleZ *= 10.0f;

	float fRotationX = y*fScale, fRotationY = x*fScale;

	SMatrix3 mX, mY;
	mY.RotationY( fRotationY );
	mX.RotationArbitraryAxis( pCameraData->Right, fRotationX );
	GVectorNormalizeInPlace( &pCameraData->Front );
	GMatrixTransformInPlace( &pCameraData->Front, &mX );
	GMatrixTransformInPlace( &pCameraData->Front, &mY );
//		GMatrixTransformInPlace( &pCameraData->Right, &mX );
	GMatrixTransformInPlace( &pCameraData->Right, &mY );
	GMatrixTransformInPlace( &pCameraData->Up, &mX );
	GMatrixTransformInPlace( &pCameraData->Up, &mY );

	GVectorAddScaledInPlace( &pCameraData->Position, GVectorNormalizeInPlace( &pCameraData->Front ), ((float)z)*fScaleZ );

	pCameraData->Up = GVECTOR3_UP;
	GVectorCrossAndNormalize( &pCameraData->Right, &pCameraData->Up, &pCameraData->Front );
};

void CGGame::OnMouseButtonUp( uint8_t nButton )
{
};
void CGGame::OnMouseButtonDown( uint8_t nButton )
{
};