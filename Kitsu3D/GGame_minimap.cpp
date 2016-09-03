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

error_t CGGame::SetupMinimap( IGVideoContext* in_pVideoManager, uint32_t nWidth, uint32_t nDepth, CGSpriteNode& out_SpriteMinimap )
{
	GPtrNCO(IGVideoImage) pnewImage=0;
	GPtrNCO(IGVideoProgram)	pnewShader=0;

	GPtrNCO(SBuffer) HeightmapBuffer;

	error_t result = 0;
	if (result = in_pVideoManager->CreateProgramFromFile("minimap.fx", &pnewShader))
	{
		error_printf("IGVideoContext::CreateProgramFromFile() returned 0x%X.", result);
		return -1;
	}

	if( result = buildTileHeightImage( m_pTerrainEntity->GetTileGeometryList()[0], 1, 10, m_pTerrainEntity->GetTerrainWidth(), m_pTerrainEntity->GetTerrainDepth(), &HeightmapBuffer ) )
	{
		error_printf("buildTileHeightImage() returned 0x%X!", result);
		return -1;
	}

	if( result = in_pVideoManager->CreateImageFromMemory( nWidth, nDepth, HeightmapBuffer, GRESMODE_DEVICE_READ, &pnewImage ) )
	{
		error_printf("IGVideoContext::CreateImageFromMemory() returned 0x%X.", result);
		return -1;
	}

	GPtrPODX(STexture) pTextureData(onCreateDefaultSTexture,0,0);
	pTextureData->ImageAlpha	= false;

	GPtrPOD(SSprite) pSpriteData=0;
	pSpriteData->Loop			= true;
	pSpriteData->Reverse		= false;
	pSpriteData->StepTime		= 0;
	pSpriteData->FrameColumns	= 1;
	pSpriteData->FrameCount		= 1;
	pSpriteData->FrameSize[0]	= nWidth;
	pSpriteData->FrameSize[1]	= nDepth;
	pSpriteData->FrameOffset	= 0;
	
	CGTexture TextureMinimap;
	TextureMinimap.SetTextureData( pTextureData );
	TextureMinimap.SetSpriteData( pSpriteData );
	TextureMinimap.SetImage( pnewImage );

	out_SpriteMinimap.SetTextureCount( 1 );
	out_SpriteMinimap.SetTexture( 0, TextureMinimap );
	out_SpriteMinimap.SetProgram( pnewShader );
	out_SpriteMinimap.SetColor( GWHITE );
	out_SpriteMinimap.SetAlphaTestEnabled( false );

	GPtrPOD(STransform) TransformData;
	TransformData->Matrix.Scale( nWidth*-2.0f, nDepth*-2.0f, 1.0f, true );
	TransformData->Matrix.SetTranslation( SVector3( (float)m_SpriteRenderer.GetViewport()->GetWidth(), nDepth*2.0f, 0.5f ) );
	TransformData->MatrixInverse.SetInverse( TransformData->Matrix );
	out_SpriteMinimap.SetGlobalTransform( TransformData );
	out_SpriteMinimap.UpdateAnimation(0);

	return 0;
}