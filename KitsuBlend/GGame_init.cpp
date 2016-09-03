#ifndef min
#define min( a, b ) ( ((a)>(b)) ? (b) : (a) )
#endif 
#ifndef max
#define max( a, b ) ( ((a)<(b)) ? (b) : (a) )
#endif 
//#define GPROFILE_ENABLED
/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GGame.h"
#include "GGame_function_macros.h"
#include "GCore_function_macros.h"
#include "IGFramework.h"

#include "GCore_printf.h"
#include "GDebugger_core.h"

#include "helper_macros.h"

GDEFINE_NCO_CUSTOMPAGE( god, CGGame, 16 );
__GDEFINE_MODULE_EXPORT_FUNCTIONS( god, CGGame );

god::CGGame::CGGame( void )
{
	GPROFILE_METHOD( "CGGame", "CGGame" );		 
	// -- Put your custom initialization code after this line
	//_CrtSetBreakAlloc( 550 );
	memset( &m_mapKeyboard, 0, sizeof(m_mapKeyboard) );
	memset( &m_SelectedPos, 0, sizeof(m_SelectedPos) );
	memset( &m_UnitCursor.Position, 0, sizeof(m_UnitCursor.Position) );
	memset( &m_UICursor.Position, 0, sizeof(m_UICursor.Position) );
	m_bSelectionActive	= false;
	m_UnitSelected		= -1;
	m_CurrentGameFrame.PlayerInfo[0].Color = GMAGENTA*.5f;
	m_CurrentGameFrame.PlayerInfo[1].Color = GORANGE*.5f;
	m_ActivePlayer		= &m_CurrentGameFrame.PlayerInfo[0];
}

god::CGGame::~CGGame( void )
{
	GPROFILE_METHOD( "CGGame", "~CGGame" );		
	// -- Put your custom destructor code after this line
}

god::error_t	god::CGGame::InitGame( IGFramework* pFramework )
{ 
	GPROFILE_METHOD( "CGGame", "InitGame" );	
	CGBaseGame::InitGame(pFramework);
	m_GameWorld.InitWorld( m_pVideoContext, &m_BodyIntegrator );

	// We initialize the game before initializing any asset so we make sure the game is not dependant on the graphical representation.
	KLIB_INIT_GAME(m_pGame);

	// Game scene
	if( InitChessMaterials()		) { error_printf( "InitMaterials()		" ); return -1; }
	if( InitChessShaders()			) { error_printf( "InitShaders()		" ); return -1; }
	if( InitChessMeshes()			) { error_printf( "InitMeshes()			" ); return -1; }
	if( InitChessLights()			) { error_printf( "InitLights()			" ); return -1; }
	if( InitChessCamera()			) { error_printf( "InitCamera()			" ); return -1; }
	if( InitChessPlayerUnits()		) { error_printf( "InitPlayerUnits()	" ); return -1; }
	if( InitChessUnitCursor()		) { error_printf( "InitUnitCursor()		" ); return -1; }
	if( InitChessMinimapSprite()	) { error_printf( "InitMinimapSprite()	" ); return -1; }
	
	// UI
	if( InitChessUICursor()			) { error_printf( "InitUnitCursor()		" ); return -1; }
	if( InitChessInput()			) { error_printf( "InitInput()			" ); return -1; }

	m_ActivePlayer = &m_CurrentGameFrame.PlayerInfo[0];
	ResetBoard();
	return 0; 
}

god::error_t	god::CGGame::ShutdownGame( void )
{ 
	GPROFILE_METHOD( "CGGame", "ShutdownGame" );
	// Put your custom shutdown code after this line
	safe_delete(m_pGame);

	// Put your custom shutdown code before this line
	return CGBaseGame::ShutdownGame();
}

god::error_t	god::CGGame::InitChessCamera( void )
{ 
	GPROFILE_METHOD( "CGGame", "InitCamera" );
	// Set camera values
	GPtrPODX(SCamera) Camera(onCreateDefaultSCamera,0,0);
	Camera->Position	= SVector3( 6.25f, 4.75f, 6.25f );
	Camera->Front		= -SVector3( Camera->Position ).Normalize();
	GVectorCrossAndNormalize( &Camera->Right, &Camera->Up, &Camera->Front );	// Calculate the perpendicular vector from up and front
	Camera->FarPlane	= 10000.0f;
	Camera->NearPlane	= 0.1f;
	m_MainCamera.SetCameraData( Camera );
	m_MeshRenderer.SetCamera( &m_MainCamera );
	m_GameWorld.SetCamera( &m_MainCamera );
	m_GameWorld.SetViewport( &m_MainViewport );
	m_GameWorld.SetProjectionType( GPROJECTION_TYPE_PERSPECTIVE );

	m_MeshWorld.SetCamera( &m_MainCamera );
	m_MeshWorld.SetViewport( &m_MainViewport );
	m_MeshWorld.SetDisplayInfo( false );

	//m_MeshWorld.SetRotation(SVector3(0.0f, 0.1f, 0.0f));
	//m_MeshWorld.SetActive( true );
	m_BodyIntegrator.RegisterBody( &m_MeshWorld );

	return 0;
}

god::error_t	god::CGGame::InitChessLights( void )
{
	m_DirectionalLight.SetType( GLIGHT_TYPE_DIRECTIONAL );
	m_DirectionalLight.SetAmbient( GDARKGRAY/2.0f );
	m_DirectionalLight.SetDiffuse( GWHITE*.9f );
	m_DirectionalLight.SetSpecular( GWHITE*.9f );
	m_DirectionalLight.SetDirection( SVector3( 1.0f, 1.0f, 1.0f ).Normalize() );
	m_DirectionalLight.SetState( GLIGHT_STATE_ENABLED );
	
	m_GameWorld.RegisterLight( &m_DirectionalLight );
	m_GameWorld.SetDirectionalLight( &m_DirectionalLight );
	printInfoString( GPtrPOD(SLight)(m_DirectionalLight) );

	m_MeshWorld.RegisterLight( &m_DirectionalLight );
	m_MeshWorld.SetDirectionalLight( &m_DirectionalLight );
	return 0;
}

god::error_t	god::CGGame::InitChessMaterials( void )
{
	m_MaterialWhite->Ambient			= GWHITE;
	m_MaterialWhite->Diffuse			= GWHITE;
	m_MaterialWhite->Specular			= GWHITE;
	m_MaterialWhite->fTime				= 0.0f;
	m_MaterialWhite->Emissive			= GBLACK;
	m_MaterialWhite->SpecularPower		= 20.0f;

	m_MaterialWhite.clone(m_MaterialBlack);
	m_MaterialBlack->Ambient			= GDARKGRAY*.1f;
	m_MaterialBlack->Diffuse			= 0xAA202013;

	m_MaterialWhite.clone(m_UnitCursor.Invalid);
	m_UnitCursor.Invalid->Ambient			= GRED*.5f;
	m_UnitCursor.Invalid->Diffuse			= 0xAAFF0000;

	m_MaterialWhite.clone(m_UnitCursor.Valid);
	m_UnitCursor.Valid->Ambient				= GGREEN*.5f;
	m_UnitCursor.Valid->Diffuse				= 0xAA00FF00;

	m_MaterialWhite.clone(m_UnitCursor.Danger);
	m_UnitCursor.Danger->Ambient			= GYELLOW*.5f;
	m_UnitCursor.Danger->Diffuse			=  0xAA000000 | (0xFFFFFF & (uint32_t)GYELLOW);

	m_MaterialWhite.clone(m_UnitCursor.Empty);
	m_UnitCursor.Empty->Ambient				= GWHITE*.5f;
	m_UnitCursor.Empty->Diffuse				=  0xAAFFFFFF;

	m_MaterialWhite.clone(m_UnitCursor.Select);
	m_UnitCursor.Select->Ambient			= GCYAN*.5f;
	m_UnitCursor.Select->Diffuse			=  0xAA000000 | (0xFFFFFF & (uint32_t)GCYAN);

	m_MaterialWhite.clone(m_MaterialSelectedPiece);
	m_MaterialSelectedPiece->Ambient	= GCOLOR(0xFF0000FF)*.5f;
	m_MaterialSelectedPiece->Diffuse	= 0xAA0000FF;//GBLACK;

	return 0;
}

god::error_t	god::CGGame::InitChessShaders( void )
{
	GPtrNCO(IGVideoContext) ctx=GetVideoContext();
	error_t errMy=0;
	if (errMy = ctx->CreateProgramFromFile("cursorunits3d.fx", &m_UnitCursor.Shader))
	{
		error_printf("IGVideoContext::CreateProgramFromFile() returned 0x%X!", errMy );
		return -1;
	}
	
	if (errMy = ctx->CreateProgramFromFile("spritenofilter.fx", &m_SpriteShader))
	{
		error_printf("IGVideoContext::CreateProgramFromFile() returned 0x%X!", errMy );
		return -1;
	}

	if (errMy = ctx->CreateProgramFromFile("spritenofilter.fx", &m_ShaderCursorMinimap))
	{
		error_printf("IGVideoContext::CreateProgramFromFile() returned 0x%X!", errMy );
		return -1;
	}
	
	//if( errMy = ctx->CreateProgramFromFile( "GTerrainShader.fx", 0, &m_DefaultShader ) )
	//if( errMy = ctx->CreateProgramFromFile( "GTileSetShader.fx", 0, &m_DefaultShader ) )
	//if( errMy = ctx->CreateProgramFromFile( "texturedlighting.fx", 0, &m_DefaultShader ) )
	if( errMy = ctx->CreateProgramFromFile( "texturednolightnofilter.fx", &m_DefaultShader ) )
	//if (errMy = ctx->CreateProgramFromFile("terrain_texlight_nofilter.fx", 0, &m_DefaultShader))
	
	//if (errMy = ctx->CreateProgramFromFile("texlighting_nofilter.fx", 0, &m_DefaultShader))
	{
		error_printf("IGVideoContext::CreateProgramFromFile() returned 0x%X!", errMy );
		return -1;
	}
	return 0;
}

god::error_t	god::CGGame::InitChessMeshes( void )
{
	if( InitChessBoardMesh() )
	{
		error_printf("InitBoardMesh()");
		return -1;
	}
	if( InitChessUnitMeshes() )
	{
		error_printf("InitUnitMeshes()");
		return -1;
	}
	m_MeshWorld.UpdateWorld(0);
	return 0;
}

god::error_t	god::CGGame::InitChessUnitMeshes( void )
{
	GPtrNCO(IGVideoContext) pvc = GetVideoContext();
	error_t result = 0;
	CGEntity* pNewEntity=0;
	CGModelEntity* pModelEntity = 0;

	if( result = m_MeshWorld.InitWorld( GetVideoContext(), &m_BodyIntegrator ) )
	{
		error_printf("m_MeshHolder.InitWorld() returned 0x%X", result);
		return -1;
	}

	glabel filename;
	FILE* fp = 0;
	SVector3 Position = GVECTOR3_ZERO;
	SVector3 DeltaVector = SVector3(2.0f, 0.0f, 0.0f);
	SMatrix3x3 rot;
	rot.RotationY((float)(GMATH_PI/3));
	uint32_t i=0;
#define CREATE_NEW_MESH_ENTITY( ms3dFilename )												\
	m_EntityManager.CreateEntity( GENTITY_FAMILY_MODEL, &pNewEntity );							\
	pModelEntity = (CGModelEntity*)pNewEntity->GetInterfacePointer(GIID_CUSTOMINTERFACE);	\
	filename = ms3dFilename ;																\
	fp = 0; fopen_s( &fp, filename.c_str(), "rb" );											\
	debug_printf( "Opening ms3d file: %s", filename.c_str() );								\
	if( pModelEntity->BuildFromMS3DFile( gget(m_pVideoContext), filename ) )				\
	{																						\
		error_printf("Failed to open file '%s'", filename);									\
		return -1;																			\
	};																						\
	pModelEntity->SetPosition( Position );													\
	Position += DeltaVector;																\
	DeltaVector = DeltaVector*rot;															\
	pModelEntity->SetHidden( true, true );													\
	m_MeshWorld.DelegateEntity( pModelEntity );
	
	//CREATE_NEW_MESH_ENTITY( "pawn_only.ms3d" )		// thief
	CREATE_NEW_MESH_ENTITY( "my_dalek_v2.ms3d" )	// thief
	CREATE_NEW_MESH_ENTITY( "rook_only.ms3d" )		// archer
	CREATE_NEW_MESH_ENTITY( "knight_only.ms3d" )	// swordman
	CREATE_NEW_MESH_ENTITY( "bishop_only.ms3d" )	// aco
	CREATE_NEW_MESH_ENTITY( "queen_only.ms3d" )		// mage
	CREATE_NEW_MESH_ENTITY( "king_only.ms3d" )		// merch

	return 0;
}



god::error_t	god::CGGame::InitChessBoardMesh( void )
{
	m_BoardTextures[0] = "boardtiles.png";
	const uint32_t width = klib::STacticalTiles::Width, depth = klib::STacticalTiles::Depth;
	m_BoardTiles.resize( width, depth );
	m_BoardTileTextures.resize(3);
	GPtrPOD(STileTexture) ttex;
	ttex.create();
	ttex->u[0] = 0.0f; ttex->u[1] = 0.5f;ttex->u[2] = 0.0f;ttex->u[3] = 0.5f;
	ttex->v[0] = 0.0f; ttex->v[1] = 0.0f;ttex->v[2] = 1.0f;ttex->v[3] = 1.0f;
	ttex->nTextureIndex	= 0;
//	ttex->nShaderIndex	= 0;
	m_BoardTileTextures.set(ttex, 0);
	ttex.clone(ttex);
	ttex->u[0] = 0.5f; ttex->u[1] = 1.0f;ttex->u[2] = 0.5f;ttex->u[3] = 1.0f;
	ttex->v[0] = 0.0f; ttex->v[1] = 0.0f;ttex->v[2] = 1.0f;ttex->v[3] = 1.0f;
	ttex->nTextureIndex	= 0;
	//ttex->nTextureIndex	= 1;
	m_BoardTileTextures.set(ttex, 1);
	for( uint32_t z=0; z<m_BoardTiles.column_size(); z++ )
	{
		for( uint32_t x=0; x<width; x++ )
		{
			gcreate( &m_BoardTiles[z][x] );
			STileGeometry* data = gget( m_BoardTiles[z][x] );
			memset(data->fHeight, 0, sizeof(float)*4 );
			data->FrontTextureDataIndex = data->RightTextureDataIndex = -1;
			char index = (z%2) ? (1-x%2) : (x%2);
			data->TopTextureDataIndex	= index;
			data->Flags = -1;
		}
	}
	GPtrPOD(STiledTerrain) tt;
	tt->Width		= width;
	tt->Depth		= depth;
	tt->TileScale	= 1.0f;
	tt->Offset		= GVECTOR3_ZERO;

	CGEntity* newEntity = 0;
	m_EntityManager.CreateEntity( GENTITY_FAMILY_TERRAIN, &newEntity );
	CGTerrainEntity* BoardTerrain = (CGTerrainEntity*)newEntity->GetInterfacePointer(GIID_CUSTOMINTERFACE);
	BoardTerrain->SetProgram(m_DefaultShader);
	if( 0 > BoardTerrain->BuildTerrainFromTileElements( gget(m_pVideoContext), tt, &m_BoardTiles, &m_BoardTileTextures, 0, m_BoardTextures, true, true, true ) )
	{
		error_printf("failed to build board terrain!");
		delete( BoardTerrain );
		return -1;
	}
	BoardTerrain->InitEntity(&m_GameWorld);
	
	m_MeshWorld.DelegateEntity(BoardTerrain);
	return 0;
}

god::error_t god::CGGame::InitChessPlayerUnits()
{
	CGEntity* newEntity;
	GPtrPOD(SMaterial) pieceMaterial = m_MaterialWhite;
	gepointer<CGEntity> pieceModels = m_MeshWorld.GetChildList();
	for( uint32_t iPlayer=0; iPlayer<MAX_PLAYERS; iPlayer++ )
	{
		m_CurrentGameFrame.PlayerInfo[iPlayer].TeamID = iPlayer;
		if( iPlayer > 0 )
			pieceMaterial = m_MaterialBlack;

		SChessPiece* p = m_CurrentGameFrame.PlayerInfo[iPlayer].Pieces;
		GPtrPODX(SEntity) edata(onCreateDefaultSEntity,0,0);
		edata.create();
		GPtrNCO(IGVideoProgram) shader;
		for( uint32_t iPiece=0; iPiece < klib::SGameSquad::Size; iPiece++ )
		{
			PIECECLASS_TYPE pieceClass = KING;
			if( iPiece < 8 )
				pieceClass = PAWN;
			else if( iPiece < 10 )
				pieceClass = ROOK;
			else if( iPiece < 12 )
				pieceClass = KNIGHT;
			else if( iPiece < 14 )
				pieceClass = BISHOP;
			else if( iPiece == 14 )
				pieceClass = QUEEN;

			m_EntityManager.CreateEntity( GENTITY_FAMILY_BASE, &newEntity );

			newEntity->SetBufferCount( pieceModels[pieceClass]->GetBufferCount() );
			for( uint32_t iBuffer = 0; iBuffer < newEntity->GetBufferCount(); iBuffer++ )
				newEntity->SetBuffer( iBuffer, pieceModels[pieceClass]->GetBuffer(iBuffer) );

			for( uint32_t iChild=0; iChild < pieceModels[pieceClass]->GetChildList().size(); iChild++ )
			{
				for( uint32_t iChild2=0; iChild2< pieceModels[pieceClass]->GetChildList()[iChild]->GetChildList().size(); iChild2++ )
				{
					CGEntity* pChildEntity = pieceModels[pieceClass]->GetChildList()[iChild]->GetChildList()[iChild2];
					newEntity->SetTextureCount( pChildEntity->GetTextureCount() );
					for( uint32_t iTexture = 0; iTexture < newEntity->GetTextureCount(); iTexture++ )
						newEntity->SetTexture( iTexture, pChildEntity->GetTexture(iTexture) );
					newEntity->SetMaterial( pChildEntity->GetMaterial() );
					newEntity->SetRenderStateData( pChildEntity->GetRenderStateData() );
					shader = pChildEntity->GetProgram();
				}
			}
			
			newEntity->SetProgram( shader );
			newEntity->SetMaterial( pieceMaterial );
			newEntity->AcquireResources(gget(m_pVideoContext));

			p[iPiece].pEntity	= newEntity;
			p[iPiece].Class		= pieceClass;
			p[iPiece].TeamID	= iPlayer;

			newEntity->InitEntity( edata, &m_GameWorld );
			newEntity->SetEntityID( iPiece );
			m_GameWorld.DelegateEntity( newEntity );
		}
	}
	return 0;
};

god::error_t	god::CGGame::InitChessInput( void )
{
	// setup defaults
	m_mapKeyboard[KEY_CURSOR_UP		]	= GKEY_UP;		// GKEY_NUMPAD8;
	m_mapKeyboard[KEY_CURSOR_DOWN	]	= GKEY_DOWN;	// GKEY_NUMPAD2;
	m_mapKeyboard[KEY_CURSOR_LEFT	]	= GKEY_LEFT;	// GKEY_NUMPAD4;
	m_mapKeyboard[KEY_CURSOR_RIGHT	]	= GKEY_RIGHT;	// GKEY_NUMPAD6;
	m_mapKeyboard[KEY_CURSOR_ACTION	]	= GKEY_NUMPAD0;
	m_mapKeyboard[KEY_CURSOR_DELETE	]	= GKEY_NUMDECIMAL;
	
	m_mapKeyboard[KEY_MOVE_BACK		]	= GKEY_W;
	m_mapKeyboard[KEY_MOVE_FORWARD	]	= GKEY_A;
	m_mapKeyboard[KEY_MOVE_LEFT		]	= GKEY_S;
	m_mapKeyboard[KEY_MOVE_RIGHT	]	= GKEY_D;
		
	m_mapKeyboard[KEY_ACTION		]	= GKEY_E;
	m_mapKeyboard[KEY_CANCEL		]	= GKEY_ESCAPE;
	m_mapKeyboard[KEY_SHOOT			]	= GKEY_RETURN;
	m_mapKeyboard[KEY_RUN			]	= GKEY_LSHIFT;
	m_mapKeyboard[KEY_JUMP			]	= GKEY_SPACE;
	m_mapKeyboard[KEY_CROUCH		]	= GKEY_C;

	m_mapKeyboard[KEY_NEXTBASEUNIT	]	= GKEY_F3;
	m_mapKeyboard[KEY_NEXTUNIT		]	= GKEY_TAB;
	m_mapKeyboard[KEY_NEXTTEAM		]	= GKEY_F5;
	m_mapKeyboard[KEY_NEXTTILE		]	= GKEY_F6;

	m_pKeyboardManager->AddHandler( this );
	m_pMouseManager->AddHandler( this );

	return 0;
};

god::error_t	god::CGGame::InitChessUnitCursor( void )
{
	if( InitChessUnitCursorMesh() )
		return -1;
	if( InitChessUnitCursorSprite() )
		return -1;
	return 0;
};

god::error_t	god::CGGame::InitChessUnitCursorMesh( void )
{
	CGEntity* newEntity;
	if( 0 > m_EntityManager.CreateEntity( GENTITY_FAMILY_BASE, &newEntity ) )
	{
		error_printf("CreateEntity FAILED!");
		return -1;
	}
	newEntity->SetProgram( m_UnitCursor.Shader );
	newEntity->BuildBox( gget(m_pVideoContext), 1.0f, 2.0f, 1.0f );
	GPtrPODX(SEntity) asd(onCreateDefaultSEntity,0,0);
	asd->Visible = true;
	newEntity->InitEntity(asd, &m_GameWorld);
	newEntity->SetMaterial(m_UnitCursor.Invalid);
	newEntity->SetPosition( 0, 0, -2 );

	m_MeshWorld.DelegateEntity( newEntity );

	return 0;
}

god::error_t	god::CGGame::InitChessMinimapSprite( void )
{
	const uint32_t	width = klib::STacticalTiles::Width,
					depth = klib::STacticalTiles::Depth;

	GPtrPODX(SSprite) SpriteData(onCreateDefaultSSprite,0,0);
	SpriteData->FrameColumns	= 1;
	SpriteData->FrameCount		= 1;
	SpriteData->FrameSize[0]	= width;
	SpriteData->FrameSize[1]	= depth;
	GPtrPODX(STexture) texData(onCreateDefaultSTexture,0,0);
	texData->ImageAlpha		= true;
	GPtrPOD(SRenderState) rsData;
	rsData->fTime		= 0.0f;
	rsData->AlphaBlend	= false;
	rsData->AlphaTest	= false;
	rsData->AlphaValue	= 1.0f;
	rsData->ApplyLights	= GLIGHT_TYPE_NONE;
	rsData->CullMode	= GCULL_MODE_CCW;
	rsData->FillMode	= GFILL_MODE_SOLID;

	uint32_t nWidth=width*MINIMAP_SCALE, nHeight=depth*MINIMAP_SCALE;
	uint32_t nOffsetX=m_pVideoManager->GetWidth(0)-(nWidth)-4, nOffsetY=4;
	GPtrPOD(STransform) xform;
	xform->Matrix.Scale( nWidth*1.0f, nHeight*1.0f, 1.0f, true );
	xform->Matrix.SetTranslation( SVector3((float)nOffsetX, (float)nOffsetY, 0), false );
	xform->MatrixInverse.SetInverse( xform->Matrix );
	//m_SpriteMinimap.SetLocalTransform( xform );
	m_SpriteMinimap.SetGlobalTransform( xform );

	CGTexture tex;
	tex.SetTextureData( texData );
	tex.SetSpriteData( SpriteData );
	m_SpriteMinimap.SetTextureCount( 1 );
	m_SpriteMinimap.SetTexture( 0, tex );

	GPtrNCO(IGVideoProgram) pProgram(m_SpriteShader);
	m_SpriteMinimap.SetProgram( pProgram );
	m_SpriteMinimap.SetColor( GWHITE );
	m_SpriteMinimap.SetAlphaTestEnabled( true );
	m_SpriteMinimap.UpdateAnimation(0);
	m_SpriteRenderer.RegisterNode( &m_SpriteMinimap );

	return 0;
}

god::error_t	god::CGGame::InitChessUnitCursorSprite( void )
{
	const uint32_t width = klib::STacticalTiles::Width, depth = klib::STacticalTiles::Depth;

	GPtrPODX(SSprite) SpriteData(onCreateDefaultSSprite,0,0);
	SpriteData->FrameColumns	= 1;
	SpriteData->FrameCount		= 1;
	SpriteData->FrameSize[0]	= 24;
	SpriteData->FrameSize[1]	= 24;

	GPtrPODX(STexture) texData(onCreateDefaultSTexture,0,0);
	texData->ImageAlpha		= true;

	GPtrPOD(SRenderState) rsData;
	rsData->fTime		= 0.0f;
	rsData->AlphaBlend	= false;
	rsData->AlphaTest	= false;
	rsData->AlphaValue	= 1.0f;
	rsData->ApplyLights	= GLIGHT_TYPE_NONE;
	rsData->CullMode	= GCULL_MODE_CCW;
	rsData->FillMode	= GFILL_MODE_SOLID;

	GPtrNCO(IGVideoProgram) pProgram(m_SpriteShader);

	//uint32_t nWidth=24, nHeight=24, nOffsetX=m_pVideoManager->GetWidth(0)-128-8, nOffsetY=0;
	uint32_t nWidth=MINIMAP_SCALE+6, nHeight=MINIMAP_SCALE+6;
	uint32_t nOffsetX=m_pVideoManager->GetWidth(0)-(depth*MINIMAP_SCALE)-8, nOffsetY=0;
	GPtrPOD(STransform) xform;
	xform->Matrix.Scale( nWidth*1.0f, nHeight*1.0f, 1.0f, true );
	xform->Matrix.SetTranslation( SVector3((float)nOffsetX, (float)nOffsetY, 0), false );
	xform->MatrixInverse.SetInverse( xform->Matrix );
	//m_UnitCursor.Sprite.SetLocalTransform( xform );
	m_UnitCursor.Sprite.SetGlobalTransform( xform );

	GPtrNCO(IGVideoImage) pImage;
	if( m_pVideoContext->CreateImageFromFile( "CursorTileMinimap.png", &pImage ) ) 
	{
		error_printf("%s", "CursorTileMinimap.png");
		return -1;
	}
	CGTexture tex;
	tex.SetTextureData( texData );
	SpriteData.clone(SpriteData);
	tex.SetSpriteData( SpriteData );
	tex.SetImage( pImage );
	m_UnitCursor.Sprite.SetTextureCount( 1 );
	m_UnitCursor.Sprite.SetTexture( 0, tex );
	m_UnitCursor.Sprite.SetColor( GCYAN );
	m_UnitCursor.Sprite.SetProgram( pProgram );
	m_UnitCursor.Sprite.SetAlphaTestEnabled( true );
	m_UnitCursor.Sprite.UpdateAnimation(0);

	m_UnitCursor.GlowActive = true;
	m_UnitCursor.GlowAlphaTime = 0;

	m_SpriteRenderer.RegisterNode( &m_UnitCursor.Sprite );
	return 0;
}

god::error_t god::CGGame::InitChessUICursor( void )
{


	return 0;
}