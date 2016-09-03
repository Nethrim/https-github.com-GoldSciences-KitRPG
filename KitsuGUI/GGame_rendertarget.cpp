// File:GGame_rendertarget.cpp
//
// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
// 
//
#include "GGame.h"
#include "GGame_function_macros.h"
#include <math.h>

//#define GPROFILE_ENABLED
#include "GCore_printf.h"
#include "GDebugger_core.h"

using namespace god;
//
//error_t god::setupRenderTargetImage( GODS(IGVideoContext) pManager, unsigned int nWidth, unsigned int nHeight, GODS(IGVideoImage)* out_pImage )
//{
//	GPtrNCO(SBuffer) newRenderTargetData=0;
//	gcreateBuffer( GDATA_TYPE_UINT8_4, GUSAGE_RENDERTARGET, nWidth*nHeight, nWidth, &newRenderTargetData );
//	GPtrNCO(IGVideoImage) pNewImage;
//	error_t result=0;
//	if( result = pManager->CreateImageFromMemory( nWidth, nHeight, newRenderTargetData, GRESMODE_DEVICE_READ | GRESMODE_DEVICE_WRITE, &pNewImage ) )
//	{
//		error_printf( "CreateImageFromMemory() returned 0x%X!", result );
//		return -1;
//	}
//	GPtrNCO(IGVideoImage) garbage = *out_pImage;
//	*out_pImage = pNewImage.acquire();
//	return 0;
//}
//
//error_t god::setupRenderTargetViewport( uint32_t nOffsetX, uint32_t nOffsetY, unsigned int nWidth, unsigned int nHeight, god::CGViewport& out_pImage )
//{
//	GPtrPOD(SViewport) newRenderTargetData;
//	newRenderTargetData->PositionX	= nOffsetX;
//	newRenderTargetData->PositionY	= nOffsetY;
//	newRenderTargetData->Width		= nWidth;
//	newRenderTargetData->Height		= nHeight;
//	out_pImage.SetViewportData( newRenderTargetData );
//	return 0;
//}
//
//error_t god::setupRenderTargetSprite( GODS(IGVideoImage) pImage, GODS(IGVideoProgram) pProgram, uint32_t nOffsetX, uint32_t nOffsetY, unsigned int nWidth, unsigned int nHeight, god::CGSpriteNode& out_pSpriteRenderTarget )
//{
//	GPtrPOD(STexture) TextureData;
//	TextureData->ImageOffset.Zero();
//	TextureData->ImageUsage		= GIMAGE_USAGE_COLOR;
//	TextureData->ImageAlpha		= true;
//	TextureData->WrapMode		= GWRAP_MODE_DEFAULT;
//
//	GPtrPOD(SSprite) SpriteData;
//	SpriteData->Loop			= true;
//	SpriteData->Reverse			= false;
//	SpriteData->StepTime		= 0;
//	SpriteData->FrameColumns	= 4;
//	SpriteData->FrameCount		= 16;
//	SpriteData->FrameSize[0]	= nWidth;
//	SpriteData->FrameSize[1]	= nHeight;
//	SpriteData->FrameOffset		= 0;
//
//	CGTexture TextureRenderTarget;
//	TextureRenderTarget.SetImage( pImage );
//	TextureRenderTarget.SetTextureData( TextureData );
//	TextureRenderTarget.SetSpriteData( SpriteData );
//
//	GPtrPOD(STransform) TransformData;
//	TransformData->Matrix.Scale( nWidth*1.0f, nHeight*1.0f, 1.0f, true );
//	TransformData->Matrix.SetTranslation( SVector3((float)nOffsetX, (float)nOffsetY, 0), false );
//	TransformData->MatrixInverse.SetInverse( TransformData->Matrix );
//
//	out_pSpriteRenderTarget.SetLocalTransform( TransformData );
//	out_pSpriteRenderTarget.SetGlobalTransform( TransformData );
//
//	out_pSpriteRenderTarget.SetProgram( pProgram );
//	out_pSpriteRenderTarget.SetTextureCount( 1 );
//	out_pSpriteRenderTarget.SetTexture( 0, TextureRenderTarget );
//	out_pSpriteRenderTarget.SetColor( GWHITE );
//	out_pSpriteRenderTarget.SetAlphaTestEnabled( true );
//	out_pSpriteRenderTarget.UpdateAnimation(0);
//
//	return 0;
//}