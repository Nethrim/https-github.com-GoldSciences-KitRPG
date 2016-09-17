//#define GPROFILE_ENABLED
#include "GGame.h"
#include "GGame_function_macros.h"
#include "GCore_function_macros.h"

#include "draw.h"
#include "helper_macros.h"

#include <math.h>
#ifdef WIN32
#include <crtdbg.h>
#include <strsafe.h>
#endif
#include "GMath_base.h"

#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "GCore_nativetype.h"

using namespace god;
	
GDEFINE_NCO_CUSTOMPAGE_NONS(CGGame, 16);
__GDEFINE_MODULE_EXPORT_FUNCTIONS_NONS( CGGame );

CGGame::CGGame( void )
{
	//_CrtSetBreakAlloc( 220 );
	m_bDisplayFrameRate	= true;
	m_bFalling			= false;
	m_nSelectedTile		= 0;
	m_SelectedTileCorners = GTILEBORDER_LEFT | GTILEBORDER_FRONT; // GTILEBORDER_BACK| GTILEBORDER_RIGHT;

	setValue(m_nTileSetSelectedTile			, 0U);
	setValue(m_TileSetSelectedTileCorners	, (GTILE_BORDER)0);
	setValue(m_lstTileSetEntity				, (CGTerrainEntity*)nullptr);
	m_nTileSetSelected		= 0;

	m_nViewportWidth	= 128;
	m_nViewportHeight	= 128;
	m_verticalSpeed		= 0;
};

CGGame::~CGGame( void )
{};

error_t CGGame::ShutdownGame( void ) {

	safe_delete(m_pGame);

	return CGBaseGame::ShutdownGame();
}

//------------------------------------------------------------
// make some initial configuration for the GUI and everything
// else you may need at start
//------------------------------------------------------------
error_t CGGame::InitGame( IGFramework* pFramework )
{
	GPROFILE_METHOD( "CGGame", "InitGame" );
	error_t result = CGBaseGame::InitGame(pFramework);
	if( 0 > result ) {
		error_printf("CGBaseGame::InitGame() returned 0x%X!", result);
		return -1;
	}

	// We initialize the game before initializing any asset so we make sure the game is not dependant on the graphical representation.
	KLIB_INIT_GAME(m_pGame);

#ifdef CreateFont
#undef  CreateFont
#endif
	GPtrNCO(IGVideoContext) pManager = m_pVideoContext;
#define FW_NORMAL           400
	if (result = pManager->CreateFont(14, 0, FW_NORMAL, "Arial", false, &m_Font)) {
		error_printf("IGVideoContext::CreateFont() returned 0x%X!", result );
		return -1;
	}

	if (result = pManager->CreateProgramFromFile("GTileSetShader.fx", &m_TileSetShader)) {
		error_printf("IGVideoContext::CreateProgramFromFile() returned 0x%X!", result );
		return -1;
	}	

	if (result = pManager->CreateProgramFromFile("sprite.fx", &m_ViewportShader)) {
		error_printf("IGVideoContext::CreateProgramFromFile() returned 0x%X!", result );
		return -1;
	}

	m_MinimapShader = m_ViewportShader;

	m_pKeyboardManager->AddHandler( this );
	m_pMouseManager->AddHandler( this );

	// 
	if( result = setupTileTemplates( m_lstTileGeometryDataTpl, m_lstTileTextureDataTpl ) ) {
		error_printf("setupTileTemplates() returned 0x%X!", result);
		return -1;
	};

	// ----------------------- terrain load
	if( result = LoadTerrainBitmaps() ) {
		error_printf("LoadTerrainBitmaps() FAILED!");
		return -1;
	};

	// - Create tile list

	uint32_t gridWidth = m_pGame->TacticalInfo.Board.Width, gridDepth = m_pGame->TacticalInfo.Board.Depth;

	GGrid2DPOD(STileGeometry) MazeGeometryData(gridWidth, gridDepth);//(m_TerrainBitmaps[0]->nColumnCount, m_TerrainBitmaps[0]->nElementCount/m_TerrainBitmaps[0]->nColumnCount);
	//if( 0 > buildGeometryMapFromImage( m_TerrainBitmaps[0].get_address(), 0.05f, MazeGeometryData[0] ) ){
	GPNCO(god, SBuffer) tacticalMap;
	gcreateBuffer(GDATA_TYPE_UINT8_4, GUSAGE_TEXEL, gridWidth*gridDepth, gridWidth, &tacticalMap);

	const klib::STopologyHeight* topologyCells = &m_pGame->TacticalInfo.Board.Terrain.Topology.Cells[0][0];
	for(uint32_t i=0, count=gridWidth*gridDepth; i<count; ++i)
		((GCOLOR32*)tacticalMap->pByteArray)[i] = GCOLOR32(topologyCells[i].Sharp*8, 0, topologyCells[i].Smooth*5, 0xFFU);

	if( 0 > buildGeometryMapFromImage( tacticalMap.get_address(), 0.05f, MazeGeometryData[0] ) ){
		error_printf("buildGeometryMapFromImage() FAILED!");
		return -1;
	};

	CGTerrainEntity* pnewTerrain = new CGTerrainEntity( &m_World, GENTITY_FAMILY_TERRAIN );

	GListPOD(STileTexture) lTextureDataTpl( m_lstTileTextureDataTpl, MAX_TILETEMPLATES );
		
	GPtrPOD(STiledTerrain) dataTerrain;
	dataTerrain->Width		= MazeGeometryData.row_size();
	dataTerrain->Depth		= MazeGeometryData.column_size();
	dataTerrain->TileScale	= .5f;
	dataTerrain->Offset		= GVECTOR3_ZERO;

	gelabel lstTextureNames;
	lstTextureNames.push("TileGrass0.png"); 
	lstTextureNames.push("TileBricks0.png"); 
	lstTextureNames.push("TileStone0.png");

	// -
	if (0 > pnewTerrain->BuildTerrainFromTileElements(gget(m_pVideoContext), dataTerrain, &MazeGeometryData, &lTextureDataTpl, 0, lstTextureNames, true, true, true)){
		error_printf("BuildTerrainFromTileElements() failed!");
		delete( pnewTerrain );
		return -1;
	};

	m_pTerrainEntity = pnewTerrain;
	uint32_t nSubsetCount = pnewTerrain->GetEntitySubsetCount();
	for( uint32_t iSubset=0; iSubset<nSubsetCount; iSubset++ )
		pnewTerrain->GetEntitySubsetList()[iSubset]->SetProgram( m_TileSetShader );

	pnewTerrain->InitEntity( 0, &m_World );

	if( result = m_World.InitWorld( pManager, &m_BodyIntegrator ) ) {
		error_printf("InitWorld() returned 0x%X", result);
		return -1;
	}
	m_World.SetTerrainShader( m_TileSetShader );

	m_World.DelegateEntity( pnewTerrain );
	CGModelEntity* newModelEntity = new CGModelEntity(&m_World);
	const glabel filename = "thug jump.ms3d";
	if( newModelEntity->BuildFromMS3DFile(gget(GetVideoContext()), filename) ) {
		error_printf("Failed to load ms3d file: %s", filename);
		return -1;
	}
	newModelEntity->SetScale(0.02f, 0.02f, 0.02f);
	m_World.DelegateEntity(newModelEntity);

	if( result = SetupTileSetWorlds() ) {
		error_printf("SetupTileSetWorlds() returned 0x%X!", result);
		return -1;
	}; 

	if( result = SetupTileSetTiles() ) {
		error_printf("SetupTileSetTiles() returned 0x%X!", result);
		return -1;
	}; 

	if( result = SetupTileSetRenderTargets( gget(pManager), m_nViewportWidth, m_nViewportHeight ) ) {
		error_printf("SetupTileSetRenderTargets() returned 0x%X!", result);
		return -1;
	}; 

	if( result = SetupTileSetLights() ) {
		error_printf("SetupTileSetLights() returned 0x%X!", result);
		return -1;
	}; 

	if( result = SetupMinimap( gget(pManager), pnewTerrain->GetTerrainWidth(), pnewTerrain->GetTerrainDepth(), m_SpriteMinimap ) ) {
		error_printf("SetupMinimap() returned 0x%X!", result);
		return -1;
	}; 
	m_SpriteRenderer.RegisterNode( &m_SpriteMinimap );

	// Set camera values
	GPtrPODX(SCamera) TerrainCamera(onCreateDefaultSCamera,0,0);
	TerrainCamera->Position		= SVector3( 2.25f, 3.75f, 2.25f );
	TerrainCamera->Front		= -SVector3( TerrainCamera->Position ).Normalize();
	GVectorCrossAndNormalize( &TerrainCamera->Right, &TerrainCamera->Up, &TerrainCamera->Front );	// Calculate the perpendicular vector from up and front
	TerrainCamera->FarPlane		= 10000.0f;
	TerrainCamera->NearPlane	= 0.1f;
	m_Camera.SetCameraData( TerrainCamera );
	m_MeshRenderer.SetCamera( &m_Camera );
	m_World.SetCamera( &m_Camera );
	m_World.SetProjectionType( GPROJECTION_TYPE_PERSPECTIVE );

	m_DirectionalLight.SetType( GLIGHT_TYPE_DIRECTIONAL );
	m_DirectionalLight.SetAmbient( GDARKGRAY/2.0f );
	m_DirectionalLight.SetDiffuse( GGRAY );
	m_DirectionalLight.SetSpecular( GWHITE*.5f );
	m_DirectionalLight.SetDirection( SVector3( 1.0f, 1.0f, 1.0f ).Normalize() );
	m_World.SetDirectionalLight( &m_DirectionalLight );
	m_World.RegisterLight( &m_DirectionalLight );
	printInfoString( GPtrPOD(SLight)(m_DirectionalLight) );
	m_World.SetViewport( m_MeshRenderer.GetViewport() );

	GPtrPODX(SRenderState) pRenderStateData(onCreateDefaultSRenderState,0,0);
	pRenderStateData.create();
	m_World.m_DefaultRenderState = ( pRenderStateData );

	GPtrPOD(SMaterial) defMat(CGMaterial( GDARKGRAY, GWHITE, GGRAY, GBLACK, 20.0f ));
	m_World.SetDefaultMaterial(defMat);

	m_SpriteRenderer.SetDefaultCamera(0);

	return 0;
};
