// File:GGame_tileset.cpp
//
// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
// 
//
#include "GGame.h"
#include "GGame_function_macros.h"
#include "GCore_nativetype.h"

#include <math.h>

//#define GPROFILE_ENABLED
#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "GMath_base.h"

using namespace god;

error_t CGGame::SetupTileSetTargetImages( IGVideoContext* in_pVideoManager, uint32_t nWidth, uint32_t nHeight )
{
	error_t result=0;
	GPtrNCO(IGVideoImage) pCommonImage=0;
	nWidth	*= 4;
	nHeight	*= 4;

	if( result = setupRenderTargetImage( in_pVideoManager, nWidth, nHeight, &pCommonImage ) )
	{
		error_printf("setupRenderTargetImage() returned 0x%X!", result);
		return -1;
	};
	m_TileSetRenderTarget = pCommonImage;
	return 0;
};

error_t CGGame::SetupTileSetViewports( uint32_t nWidth, uint32_t nHeight )
{
	error_t result=0;
	for( uint32_t iTileSet=0; iTileSet<MAX_TILESETS; iTileSet++ )
	{
		if( result = setupRenderTargetViewport( (iTileSet%4)*nWidth, (iTileSet/4)*nHeight, nWidth, nHeight, m_TileSetViewport[iTileSet] ) )
		{
			error_printf("setupRenderTargetViewport() returned 0x%X!", result);
			return -1;
		};

	}
	return 0;
};

error_t CGGame::SetupTileSetTargetSprites( uint32_t nWidth, uint32_t nHeight )
{
	error_t result=0;

	CGTexture tempTex;
	for( uint32_t iTileSet=0; iTileSet<MAX_TILESETS; iTileSet++ )
	{
		if( result = setupRenderTargetSprite( m_TileSetRenderTarget, m_ViewportShader, 5+(iTileSet%4)*(nWidth+5), 5+(iTileSet/4)*(nHeight+5), nWidth, nHeight, m_TileSetTargetSprite[iTileSet] ) )
		{
			error_printf("setupRenderTargetSprite() returned 0x%X!", result);
			return -1;
		};
		m_TileSetTargetSprite[iTileSet].UpdateAnimation( (float)iTileSet );
		m_SpriteRenderer.RegisterNode( &m_TileSetTargetSprite[iTileSet] );
	}
	return 0;
};

error_t CGGame::SetupTileSetRenderTargets( IGVideoContext* pManager, uint32_t nWidth, uint32_t nHeight )
{
	error_t result=0;
	//nWidth = min( max( 100, nWidth ), 200 );
	//nHeight = min( max( 200, nHeight ), 500 );
	if( result = SetupTileSetTargetImages( pManager, nWidth, nHeight ) )
	{
		error_printf("SetupTileSetTargetImages() returned 0x%X.", result);
		return -1;
	}
	if( result = SetupTileSetViewports( nWidth, nHeight ) )
	{
		error_printf("SetupTileSetViewports() returned 0x%X.", result);
		return -1;
	}
	if( result = SetupTileSetTargetSprites( nWidth, nHeight ) )
	{
		error_printf("SetupTileSetTargetSprites() returned 0x%X.", result);
		return -1;
	}

	// Set camera values
	GPtrPODX(SCamera) TileSetCamera(onCreateDefaultSCamera,0,0), 
		TileSetCamera2;
	TileSetCamera->Position	= SVector3( 1.25f, 1.85f, 1.25f );
	GVectorMultiplyInPlace( &TileSetCamera->Position,  1.5f );
	TileSetCamera->Front		= -SVector3(TileSetCamera->Position).Normalize();
	GVectorCrossAndNormalize( &TileSetCamera->Right, &TileSetCamera->Up, &TileSetCamera->Front );
	TileSetCamera->FarPlane		= 10.0f;
	TileSetCamera->NearPlane	= 0.001f;
	for( uint32_t i=0; i<MAX_TILESETS; i++ )
	{
		gclone( &TileSetCamera2, TileSetCamera );
		m_TileSetCamera[i].SetCameraData( TileSetCamera2 );
	}
	return 0;
};

error_t CGGame::SetupTileSetWorlds( void )
{
	error_t result=0;
	
	GPtrPOD(SMaterial) defMat(CGMaterial( GDARKGRAY, GWHITE, GGRAY, GBLACK, 20.0f ));
	for( uint32_t iTileSet=0; iTileSet<MAX_TILESETS; iTileSet++ )
	{
		if( result |= m_TileSetWorld[iTileSet].InitWorld( m_pVideoContext, &m_BodyIntegrator ) )
		{
			error_printf("CGGameWorld::InitWorld() returned 0x%X!", result);
		};
		m_TileSetWorld[iTileSet].SetCamera( &m_TileSetCamera[iTileSet] );
		m_TileSetWorld[iTileSet].SetViewport( &m_TileSetViewport[iTileSet] );
		m_TileSetWorld[iTileSet].SetDefaultMaterial( defMat );
	}
	return result;
}

error_t CGGame::SetupTileSetTiles( void )
{
	GPtrPOD(STiledTerrain) commonTileSetTerrain;
	commonTileSetTerrain->Width		= 3;
	commonTileSetTerrain->Depth		= 3;
	commonTileSetTerrain->TileScale	= 1.0f;
	commonTileSetTerrain->Offset.Zero();
#define	MAX_TILEAREA 9

	gelabel lstTextureNames;
	lstTextureNames.push("TileGrass0.png");
	lstTextureNames.push("TileBricks0.png");
	lstTextureNames.push("TileStone0.png");

	GGrid2DPOD(STileGeometry) TileSetGeometry( 3, 3 );
	CGTerrainEntity* pnewTerrain;
	for( uint32_t iTileSet=0; iTileSet<MAX_TILESETS; iTileSet++ )
	{
		TileSetGeometry.release_all();
		for( uint32_t iTileIndex=0; iTileIndex<MAX_TILEAREA; iTileIndex++ )
		{
			GODS(STileGeometry) temp = (g_TileSetIndices3x3[iTileSet][iTileIndex] == 0xFF) ? 0 : m_lstTileGeometryDataTpl[g_TileSetIndices3x3[iTileSet][iTileIndex]];
			if( 0 != temp )
			{
				if( 0 != TileSetGeometry.begin()[iTileIndex] )
					throw("");
				gclone(&TileSetGeometry.begin()[iTileIndex], temp);
				memset(&gget(TileSetGeometry.begin()[iTileIndex])->fHeight[0], 0, sizeof(float)*4);
			}
			else
				TileSetGeometry.begin()[iTileIndex] = 0;
		};
		GODS(STileGeometry) temp1 = 0, temp2 = 0,
							temp3 = 0, temp4 = 0;

		switch(iTileSet)
		{
		case 0:
			break;
		case 1:
			temp1 = TileSetGeometry.begin()[4];
			break;
		case 2:
			temp2 = TileSetGeometry.begin()[5];
			break;
		case 3:
			temp3 = TileSetGeometry.begin()[7];
			break;
		case 4:
			temp4 = TileSetGeometry.begin()[8];
			break;
		case 5:
			temp1 = TileSetGeometry.begin()[4]; 
			temp2 = TileSetGeometry.begin()[5];
			break;
		case 6:
			temp1 = TileSetGeometry.begin()[4];
			temp3 = TileSetGeometry.begin()[7];
			break;
		case 7:
			temp1 = TileSetGeometry.begin()[4];
			temp4 = TileSetGeometry.begin()[8];
			break;
		case 8:
			temp2 = TileSetGeometry.begin()[5];
			temp3 = TileSetGeometry.begin()[7];
			break;
		case 9:
			temp2 = TileSetGeometry.begin()[5];
			temp4 = TileSetGeometry.begin()[8];
			break;
		case 10:
			temp3 = TileSetGeometry.begin()[7];	
			temp4 = TileSetGeometry.begin()[8];
			break;
		case 11:
			temp1 = TileSetGeometry.begin()[4];	
			temp2 = TileSetGeometry.begin()[5];
			temp3 = TileSetGeometry.begin()[7];
			break;
		case 12:
			temp1 = TileSetGeometry.begin()[4];	
			temp2 = TileSetGeometry.begin()[5];
			temp4 = TileSetGeometry.begin()[8];
			break;
		case 13:
			temp1 = TileSetGeometry.begin()[4];
			temp3 = TileSetGeometry.begin()[7];	
			temp4 = TileSetGeometry.begin()[8];
			break;
		case 14:
			temp2 = TileSetGeometry.begin()[5]; 
			temp3 = TileSetGeometry.begin()[7];	
			temp4 = TileSetGeometry.begin()[8];
			break;
		case 15:
			temp1 = TileSetGeometry.begin()[4];	
			temp2 = TileSetGeometry.begin()[5];
			temp3 = TileSetGeometry.begin()[7];	
			temp4 = TileSetGeometry.begin()[8];
			break;
		default:
			error_printf("Index out of boundaries!");
			break;
		};

		if( temp1 ) setValue( gget( temp1 )->fHeight, 2.0f );
		if( temp2 ) setValue( gget( temp2 )->fHeight, 2.0f );
		if( temp3 ) setValue( gget( temp3 )->fHeight, 2.0f );
		if( temp4 ) setValue( gget( temp4 )->fHeight, 2.0f );
		
		pnewTerrain = new CGTerrainEntity( &m_World, GENTITY_FAMILY_BASE );
		pnewTerrain->InitEntity( 0, &m_TileSetWorld[iTileSet] );
		pnewTerrain->SetIdentityBody();
		GListPOD(STileTexture) lTextureDataTpl;
		lTextureDataTpl.create( m_lstTileTextureDataTpl, MAX_TILETEMPLATES );
		
		if( 0 > pnewTerrain->BuildTerrainFromTileElements(gget(m_pVideoContext), commonTileSetTerrain, &TileSetGeometry, &lTextureDataTpl, 0, lstTextureNames, true, true, true) )
		{
			error_printf("BuildTerrainFromTileElements() failed!");
			delete(pnewTerrain);
			continue;
		};

		uint32_t nSubsetCount = pnewTerrain->GetEntitySubsetCount();
		for( uint32_t iSubset=0; iSubset<nSubsetCount; iSubset++ )
			pnewTerrain->GetEntitySubsetList()[iSubset]->SetProgram(m_TileSetShader);
		m_lstTileSetEntity[iTileSet] = pnewTerrain;
		
		m_TileSetWorld[iTileSet].DelegateEntity(pnewTerrain);
	}
	return 0;
}

error_t	CGGame::SetupTileSetLights( void )
{
	GPtrPODX(SLight) viewportDirectionalLight(onCreateDefaultSLight,0,0),
		viewportPointLight(onCreateDefaultSLight,0,0),
		viewportSpotLight(onCreateDefaultSLight,0,0); 

	viewportDirectionalLight->Ambient	= GGRAY*.5f;
	viewportDirectionalLight->Diffuse	= GGRAY*.5f;
	viewportDirectionalLight->Specular	= GGRAY;
	viewportDirectionalLight->Range		= GINFINITY;
	viewportDirectionalLight->Type		= GLIGHT_TYPE_DIRECTIONAL;
	viewportDirectionalLight->State		= GLIGHT_STATE_ENABLED;
	viewportDirectionalLight->Direction	= SVector3( 2.0f, 3.6f, -0.0f );
	viewportDirectionalLight->Direction.Normalize();

	viewportPointLight->Ambient		= GGRAY*.05f;
	viewportPointLight->Diffuse		= GGRAY;
	viewportPointLight->Specular	= GGRAY;
	viewportPointLight->Range		= GINFINITY;
	viewportPointLight->Type		= GLIGHT_TYPE_POINT;
	viewportPointLight->State		= GLIGHT_STATE_ENABLED;
	viewportPointLight->Position	= SVector3( -5.0f, 5.0f, -5.0f ).Normalize();
	viewportPointLight->Attenuation	= SVector3( 1.0f, 0.1f, 0.001f );

	viewportSpotLight->Ambient		= GWHITE*.05f;
	viewportSpotLight->Diffuse		= GWHITE;
	viewportSpotLight->Specular		= GGRAY;
	viewportSpotLight->Range		= GINFINITY;
	viewportSpotLight->Type			= GLIGHT_TYPE_SPOT;
	viewportSpotLight->State		= GLIGHT_STATE_ENABLED;
	viewportSpotLight->Position		= SVector3( -12.0f, 24.0f, -12.0f );
	viewportSpotLight->Direction	= -viewportSpotLight->Position;
	viewportSpotLight->Direction.Normalize();
	viewportSpotLight->Attenuation	= SVector3( 1.0f, 0.1f, 0.01f );
	viewportSpotLight->SpotPower	= 20.0f;
	for( uint32_t iTileSet=0; iTileSet<MAX_TILESETS; iTileSet++ )
	{
		m_TileSetDirectionalLight[iTileSet].SetLightData( viewportDirectionalLight );
		//m_TileSetWorld[iTileSet].RegisterLight( &m_TileSetDirectionalLight[iTileSet] );
		m_TileSetWorld[iTileSet].RegisterLight( &m_DirectionalLight );
		m_TileSetPointLight[iTileSet].SetLightData( viewportPointLight );
		m_TileSetWorld[iTileSet].RegisterLight( &m_TileSetPointLight[iTileSet] );
		m_TileSetSpotLight[iTileSet].SetLightData( viewportSpotLight );
		m_TileSetWorld[iTileSet].RegisterLight( &m_TileSetSpotLight[iTileSet] );
	}

	return 0;
};

error_t CGGame::LoadTerrainBitmaps( void )
{
#ifndef MAX_PATH 
#define MAX_PATH 512
#endif
	char filename[MAX_PATH];
	FILE* fp = 0;
	for( uint32_t i=0; i < 1; i++ )
	{
		fp = 0;
		sprintf_s( filename, "Maze%u.bmp", i );
		if( 0 != fopen_s( &fp, filename, "rb" ) )
		{
			error_printf("Failed to open bitmap file!");
			return -1;
		};
	
		if( LoadBMP( &m_TerrainBitmaps[i], fp ) )
		{
			error_printf("Failed to load bitmap!");
			fclose( fp );
			return -1;
		};
		fclose( fp );
	}
	return 0;
}