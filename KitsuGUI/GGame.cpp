/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GGame.h"
#include "GGame_function_macros.h"

#include "helper_macros.h"

//#define GPROFILE_ENABLED
#include "GCore_printf.h"
#include "GDebugger_core.h"

//
#include <math.h>

//using namespace god;

int32_t	god::CGGame::ProcessUserInput( float fElapsedTime )
{ 
	__CGGAME_FUNCTION_HEADER( -1 );

	return 0; 
}

int32_t	god::CGGame::Update( float fElapsedTime )
{ 
	__CGGAME_FUNCTION_HEADER( -1 );

	if(m_pGame)
	{
		KLIB_UPDATE_GAME(m_pGame);
		if(!m_pGame->bRunning)
			return ShutdownGame();
	}
	else 
		return 0;

	error_t result=0;

	// should be handled by CGUI method
	m_MainGUI.m_CursorControl.Resources->m_SpriteNodeNormal.UpdateAnimation(fElapsedTime);

	m_BodyIntegrator.UpdatePositions( fElapsedTime );
	m_BodyForceRegistry.UpdateGenerators( fElapsedTime );
	
	reterr_msg_if( 0 > (result = m_MainGUI.RedrawGUI()), "RedrawGUI Failed!");

	if( m_MainGUI.m_ScreenBackBuffer != m_oldGUIScreen )
	{
		GPtrNCO(IGVideoImage) newImage;
		CGTexture tex = m_MainGUI.m_GUIControl.Resources->m_SpriteNodeNormal.GetTexture(0);
		if( result = GetVideoContext()->CreateImageFromMemory(m_MainGUI.m_ScreenSizeScaled.Width, m_MainGUI.m_ScreenSizeScaled.Height, m_MainGUI.m_ScreenBackBuffer, GRESMODE_DEVICE_READ, &newImage) )
		{
			error_printf("IGVideoContext::CreateImageFromMemory() returned 0x%X.", result);
			return -1;
		}

		tex.SetImage(newImage);
		//m_MainGUI.m_GUIResources.m_Texture.SetImage( m_MainGUI.m_GUIResources.m_Image );
		m_MainGUI.m_GUIControl.Resources->m_SpriteNodeNormal.SetTexture(0,tex);
		//m_MainGUI.ResetBackBufferSprite();
		m_oldGUIScreen = m_MainGUI.m_ScreenBackBuffer;

		int32_t 
			width	= m_MainGUI.m_ScreenSize.Width, 
			height	= m_MainGUI.m_ScreenSize.Height;
		
		GPtrPOD(STransform) TransformData;
		TransformData.create();
		TransformData->Matrix.Scale( (float)width, (float)height, 1.0f, true );
		TransformData->Matrix.SetTranslation( SVector3( ((float)m_MainViewport->Width-width), ((float)0), 0.001f ) );
		TransformData->MatrixInverse.SetInverse( TransformData->Matrix );
		//m_MainGUI.m_GUIControl.Resources.m_SpriteNodeNormal.SetLocalTransform( TransformData );
		m_MainGUI.m_GUIControl.Resources->m_SpriteNodeNormal.SetGlobalTransform( TransformData );	
		
	}

	return 0; 
}

int32_t	god::CGGame::Draw( float fElapsedTime )
{ 
	__CGGAME_FUNCTION_HEADER( -1 );

	error_t result=0;

	__CGGAME_RENDERER_DRAWNODES_CALL( Mesh );
	__CGGAME_RENDERER_DRAWNODES_CALL( Sprite );

	return result; 
}

int32_t	god::CGGame::PlaySounds( float fElapsedTime )
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

int32_t	god::CGGame::DoNothing( void )
{ 
	__CGGAME_FUNCTION_HEADER( -1 );

	return 0; 
}
