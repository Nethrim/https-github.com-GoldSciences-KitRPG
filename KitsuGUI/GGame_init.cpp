//#define GPROFILE_ENABLED
/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GGame.h"
#include "GGame_function_macros.h"
#include "GCore_function_macros.h"
#include "IGFramework.h"

#include "helper_macros.h"

#if defined(WIN32) || defined(_WIN32)
//#include <crtdbg.h>
#endif

#include "GCore_printf.h"
#include "GDebugger_core.h"

GDEFINE_NCO_CUSTOMPAGE( god, CGGame, 16 );
__GDEFINE_MODULE_EXPORT_FUNCTIONS( god, CGGame );

god::CGGame::CGGame( void )
{
	GPROFILE_METHOD( "CGGame", "CGGame" );		 
	// -- Put your custom initialization code after this line
	//_CrtSetBreakAlloc( 7539 );

}

god::CGGame::~CGGame( void )
{
	GPROFILE_METHOD( "CGGame", "~CGGame" );		
	// -- Put your custom destructor code after this line

}

god::error_t god::CGGame::InitGame( IGFramework* pFramework )
{ 
	GPROFILE_METHOD( "CGGame", "InitGame" );	

	// We initialize the game before initializing any asset so we make sure the game is not dependant on the graphical representation.
	KLIB_INIT_GAME(m_pGame);

#ifdef CreateFont
#undef CreateFont
#endif
	glabel name = "Arial";
#define FW_NORMAL           400

	reterr_msg_if(0 > CGBaseGame::InitGame(pFramework), "CGBaseGame::InitGame(pFramework) failed.");
	reterr_msg_if(0 > m_MainGUI.InitGUI(m_pVideoManager, m_MainViewport->Width/2, m_MainViewport->Height/2), "InitGUI failed.");
	reterr_msg_if(0 > InitCursorSprite(), "InitCursorSprite failed.");
	reterr_msg_if(0 > InitGUISprite(), "InitGUISprite failed.");
	reterr_msg_if(0 > m_pVideoContext->CreateFont(14, 0, FW_NORMAL, name, false, &m_Font), "Failed to create font! Font name: '%s'", name);

	m_pMouseManager->SetImmediate(true);
	m_pMouseManager->SetForeground(true);
	m_pMouseManager->SetExclusive(true);
	m_pMouseManager->AddHandler( &this->m_MainGUI );
	m_pKeyboardManager->AddHandler( &this->m_MainGUI );
	m_pVideoManager->RegisterHandler( &this->m_MainGUI );

	return 0; 
}

god::error_t god::CGGame::InitGUISprite( void )
{
	error_t result = 0;
	GPtrNCO(IGVideoContext) pManager = GetVideoContext();

#ifdef CreateFont
#undef CreateFont
#endif
	glabel name = "Arial";
#define FW_NORMAL           400
	reterr_msg_if(0 > pManager->CreateFont(14, 0, FW_NORMAL, name, false, &m_MainGUI.m_GUIControl.Resources->m_Font), "Failed to create font! Font name: '%s'", name);

	name = "spritenofilter.fx";
	GPtrNCO(IGVideoProgram) newShader;
	reterr_msg_if(0 > pManager->CreateProgramFromFile(name, &newShader), "Failed to create sprite shader! File name: '%s'", name);

	//m_MainGUI.m_GUIResources.m_SpriteShader = newShader;
	m_MainGUI.m_GUIControl.Resources->m_SpriteNodeNormal.SetProgram(newShader);

	int32_t 
		width	= m_MainGUI.m_ScreenSizeScaled.Width, 
		height	= m_MainGUI.m_ScreenSizeScaled.Height;
	CGTexture tex = m_MainGUI.m_GUIControl.Resources->m_SpriteNodeNormal.GetTexture(0);

	GPtrNCO(IGVideoImage) newImage;
	reterr_msg_if(m_MainGUI.m_ScreenBackBuffer && (result = pManager->CreateImageFromMemory(width, height, m_MainGUI.m_ScreenBackBuffer, GRESMODE_DEVICE_READ, &newImage)), 
		"CreateImageFromMemory() returned 0x%X.", result);

	tex.SetImage(newImage);
	m_MainGUI.m_GUIControl.Resources->m_SpriteNodeNormal.SetTexture(0, tex);
	//m_MainGUI.ResetBackBufferSprite();
	m_SpriteRenderer.RegisterNode( &m_MainGUI.m_GUIControl.Resources->m_SpriteNodeNormal );

	GPtrPOD(STransform) TransformData;
	TransformData->Matrix.Scale( (float)width, (float)height, 1.0f, true );
	TransformData->Matrix.SetTranslation( SVector3( ((float)m_MainViewport->Width-width), ((float)0), 0.01f ) );
	TransformData->MatrixInverse.SetInverse( TransformData->Matrix );
	//m_MainGUI.m_GUIControl.Resources.m_SpriteNodeNormal.SetLocalTransform( TransformData );
	m_MainGUI.m_GUIControl.Resources->m_SpriteNodeNormal.SetGlobalTransform( TransformData );

	return 0;
}


god::error_t god::CGGame::InitCursorSprite( void )
{
	error_t result = 0;
	GPtrNCO(IGVideoContext) pManager = GetVideoContext();

//	GetMouseManager()->SetExclusive( true );
#ifdef CreateFont
#undef CreateFont
#endif
	glabel name = "Arial";
#define FW_NORMAL           400
	if (pManager->CreateFont(14, 0, FW_NORMAL, name, false, &m_MainGUI.m_CursorControl.Resources->m_Font))
	{
		error_printf("Failed to create font!");
		return -1;
	}
	name = "sprite.fx";
	GPtrNCO(IGVideoProgram) newShader;
	if (pManager->CreateProgramFromFile(1, name, &newShader))
	{
		error_printf("Failed to load sprite shader!");
		return -1;
	}
	//m_MainGUI.m_CursorResources.m_SpriteShader = newShader;
	m_MainGUI.m_CursorControl.Resources->m_SpriteNodeNormal.SetProgram(newShader);
	name = "saucer1.png";
	CGTexture newTex = m_MainGUI.m_CursorControl.Resources->m_SpriteNodeNormal.GetTexture(0);
	GPtrNCO(IGVideoImage) newImage;
	if (pManager->CreateImageFromFile(1, 0xFFFF00FF, name, 0, &newImage))
	{
		error_printf("Failed to load sprite image!");
		return -1;
	}
	newTex.SetImage( newImage );
	m_MainGUI.m_CursorControl.Resources->m_SpriteNodeNormal.SetTexture(0, newTex);
	m_SpriteRenderer.RegisterNode( &m_MainGUI.m_CursorControl.Resources->m_SpriteNodeNormal );

	return 0;
};

god::error_t god::CGGame::ShutdownGame( void )
{ 
	GPROFILE_METHOD( "CGGame", "ShutdownGame" );

	// Put your custom shutdown code after this line
	safe_delete(m_pGame);

	return CGBaseGame::ShutdownGame();
}
