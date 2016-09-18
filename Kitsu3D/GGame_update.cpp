// File:GGame_update.cpp
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
#include "GMath_base.h"


//#define GPROFILE_ENABLED
#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "draw.h"

#include "helper_macros.h"

#include <chrono>
#include <thread>

//------------------------------------------------------------
// Update game stuff.
//------------------------------------------------------------
int32_t CGGame::Update( float fElapsedTime )
{
	__CGGAME_FUNCTION_HEADER( -1 );

	if(m_pGame)
	{
		KLIB_UPDATE_GAME(m_pGame);
		if(!m_pGame->bRunning)
			return ShutdownGame();
	}



	const klib::SGameState& newState = m_pGame->State, &prevState = m_pGame->PreviousState;
	if(newState.State == klib::GAME_STATE_START_MISSION && prevState.State != klib::GAME_STATE_START_MISSION )
	{
		// Add here the geometry generation for the generated map
		debug_printf("%s", "Something fishy is going on.");
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	m_BodyForceRegistry.UpdateGenerators( fElapsedTime );
	m_BodyIntegrator.UpdatePositions( fElapsedTime );
	m_World.UpdateWorld( fElapsedTime );

	for( uint32_t i=0; i<MAX_TILESETS; i++ )
		m_TileSetWorld[i].UpdateWorld( fElapsedTime );

	GPtrPODX(STransform) TransformData(onCreateDefaultSTransform,0,0,m_SpriteMinimap.GetGlobalTransform());
	TransformData->Matrix._41 = (float)m_pVideoManager->GetWidth(0);//--+pTransformData->mWorld._11;
	TransformData->Matrix._42 = (float)0-TransformData->Matrix._22;
	TransformData->MatrixInverse.SetInverse( TransformData->Matrix );
	//TransformData->mWorldInverseTranspose.SetTranspose( pTransformData->mWorldInverse );
	//m_SpriteMinimap.SetLocalTransform( TransformData );
	m_SpriteMinimap.SetGlobalTransform( TransformData );

	static const float fltScale = .1f;
	SVector3 lightDir = m_DirectionalLight.GetDirection();
	SMatrix4x4 rot;
	rot.RotationZ(((float)GMATH_PI2)*fElapsedTime*fltScale);
	lightDir = rot.Transform( lightDir );
	m_DirectionalLight.SetDirection( lightDir );

	static float fLightTime = 0;
	fLightTime += fElapsedTime/4;
	GCOLOR Ambient = m_DirectionalLight.GetAmbient();
	Ambient.b = min( max( .05f, 1.0f-lightDir.y ), 0.4f );
	m_DirectionalLight.SetAmbient(Ambient);

	uint32_t nWidth = m_pTerrainEntity->GetTerrainWidth(), 
		nDepth = m_pTerrainEntity->GetTerrainDepth();
	float fTileScale = m_pTerrainEntity->GetTileScale();

	SMatrix4 trans;
	float fRotY = ((float)GMATH_PI2)*min( 50, fElapsedTime/2 );
	trans.SetTranslation( -(SVector3(nWidth/2.0f, 0, nDepth/-2.0f)*fTileScale), true );
	rot.Rotation( SVector3( 0, fRotY/10, 0 ) );
	rot = trans*rot;
	trans.SetTranslation( (SVector3(nWidth/2.0f, 0, nDepth/-2.0f)*fTileScale), true );
	rot = rot*trans;

	GPtrPODX(SCamera) Camera(onCreateDefaultSCamera,0,0), TileSetCamera;
	if( m_World.GetCamera() )
		Camera = m_World.GetCamera()->GetCameraData();
	if( false )
	{	// The following code makes the camera rotate around the center
		Camera->Position	= rot.Transform( Camera->Position );
		GVectorNormalizeInPlace( &(Camera->Front = SVector3(nWidth/2.0f, 0, nDepth/-2.0f)*fTileScale-Camera->Position ) );
		float fDirScale=1;
		while( Camera->Position.x < 0 )
			Camera->Position.x += Camera->Front.x*fDirScale;
		while( Camera->Position.z > 0 )
			Camera->Position.z += Camera->Front.z*fDirScale;
		while( Camera->Position.x > ((nWidth*fTileScale)/1.25) )
		{
			if( Camera->Front.x > 0 )
				Camera->Position.x -= Camera->Front.x*fDirScale;
			else
				Camera->Position.x += Camera->Front.x*fDirScale;
		}
	}
	uint32_t nTileSetWidth = 3,
		nTileSetDepth = 3;
	float fTileSetTileScale		= 1.0f;
	gclone( &TileSetCamera, Camera );
	TileSetCamera->Position	= SVector3( TileSetCamera->Position ) - SVector3( nWidth*fTileScale/2.0f, 0, -(nDepth*fTileScale/2.0f) );
	GVectorNormalizeInPlace( &TileSetCamera->Position );
	GVectorMultiplyInPlace( &TileSetCamera->Position, 5.0f );
	TileSetCamera->Position.y	= max( 2.5f, TileSetCamera->Position.y );
	GVectorAddInPlace( &TileSetCamera->Position, &SVector3( 2.0f, 0, -2.0f ) );
	TileSetCamera->Front		=  SVector3( 2.0f, 0.75f, -2.0f ) //(SVector3(nTileSetWidth/2.0f, 0.75, nTileSetDepth/-2.0f)+SVector3(0.5f, 0.0f, -0.5f))
		* fTileSetTileScale
		- TileSetCamera->Position;
	GVectorNormalizeInPlace( &TileSetCamera->Front );
	{ // fix up and right vectors
		TileSetCamera->Up	= GVECTOR3_UP;
		GVectorCrossAndNormalize( &TileSetCamera->Right, &TileSetCamera->Up, &TileSetCamera->Front );
	}
	for( uint32_t iTileSet=0; iTileSet<MAX_TILESETS; iTileSet++ )
	{
		m_TileSetCamera[iTileSet].SetCameraData( TileSetCamera );
	};



	return 0;
};


//------------------------------------------------------------
// Play sounds.
//------------------------------------------------------------
int32_t CGGame::PlaySounds( float fElapsedTime )
{
	__CGGAME_FUNCTION_HEADER( -1 );
	return 0;
};
//
//----------- End of main loop function calls ----------------
void* CGGame::GetInterfacePointer( GINTERFACEID_TYPE InterfaceID )
{
	switch( InterfaceID )
	{
	case GIID_CUSTOMINTERFACE:
		return this;
	default:
		return 0;
	}
};

