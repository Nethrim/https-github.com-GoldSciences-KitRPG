// File:GEditor.h
//
// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
// 
//
#ifndef min
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#endif
#include "GBaseGame.h"
#include "GGame_macros.h"


#include "GGameEntity.h"
#include "draw.h"

#ifndef __GGAME_H__
#define __GGAME_H__

using namespace god;

class CGGame : public CGBaseGame, public IGMouseHandler, public IGKeyboardHandler
{
	__GDECLARE_IGGAME_INHERITED_MEMBERS();

public:
	CGGame( void );
	virtual ~CGGame( void );

	virtual void					OnKeyUp( unsigned char key );
	virtual void					OnKeyDown( unsigned char key );
	virtual void					OnMouseMove( int32_t x, int32_t y, int32_t z );
	virtual void					OnMouseButtonUp( uint8_t nButton );
	virtual void					OnMouseButtonDown( uint8_t nButton );
private:
	GTILE_BORDER					m_SelectedTileCorners;
	int32_t							m_nSelectedTile;
	CGGameWorld						m_World;
	CGTerrainEntity*				m_pTerrainEntity;
	CGModelEntity*					m_pPlayerEntity;

	klib::SGame*					m_pGame;

	//virtual error_t				SetupLighting( void );
	GPtrNCO(IGFont)					m_Font;

	virtual error_t					SetupMinimap( IGVideoContext* in_pVideoManager, uint32_t nWidth, uint32_t nHeight, CGSpriteNode& out_SpriteMinimap );
	CGSpriteNode					m_SpriteMinimap;
	
	virtual error_t					SetupTileSetTiles( void );
	virtual error_t					SetupTileSetWorlds( void );
	virtual error_t					SetupTileSetRenderTargets( IGVideoContext* in_pVideoManager, uint32_t nWidth, uint32_t nHeight );
	virtual error_t					SetupTileSetViewports( uint32_t nWidth, uint32_t nHeight );
	virtual error_t					SetupTileSetTargetSprites( uint32_t nWidth, uint32_t nHeight );
	virtual error_t					SetupTileSetTargetImages( IGVideoContext* in_pVideoManager, uint32_t nWidth, uint32_t nHeight );
	virtual error_t					SetupTileSetLights( void );
	virtual error_t					LoadTerrainBitmaps( void );
#define MAX_TILESETS 4
	CGGameWorld						m_TileSetWorld					[MAX_TILESETS];
	CGTerrainEntity*				m_lstTileSetEntity				[MAX_TILESETS];
	CGSpriteNode					m_TileSetTargetSprite			[MAX_TILESETS];
	CGViewport						m_TileSetViewport				[MAX_TILESETS];
	CGCamera						m_TileSetCamera					[MAX_TILESETS];
	CGLight							m_TileSetDirectionalLight		[MAX_TILESETS];
	CGLight							m_TileSetPointLight				[MAX_TILESETS];
	CGLight							m_TileSetSpotLight				[MAX_TILESETS];
	uint32_t						m_nTileSetSelectedTile			[MAX_TILESETS];
	GTILE_BORDER					m_TileSetSelectedTileCorners	[MAX_TILESETS];
	GPtrNCO(IGVideoImage)			m_TileSetRenderTarget;
	uint32_t						m_nTileSetSelected;

	// Let's store the shaders somewhere just in case we want them later.
	GPtrNCO(IGVideoProgram)			m_MinimapShader;
	GPtrNCO(IGVideoProgram)			m_ViewportShader;
	GPtrNCO(IGVideoProgram)			m_TileSetShader;
	GPtrNCO(IGVideoProgram)			m_TerrainDirectional;
	GPtrNCO(IGVideoProgram)			m_TerrainPhong;

	GPtrNCO(SBuffer) 				m_TerrainBitmaps[GREGION_TERRAINCOUNT];

#define MAX_TILETEMPLATES 7
	GPtrPOD(STileGeometry)			m_lstTileGeometryDataTpl		[MAX_TILETEMPLATES];
	GPtrPOD(STileTexture)			m_lstTileTextureDataTpl			[MAX_TILETEMPLATES];
	GPtrPOD(STileMapping)			m_lstTileMappingDataTpl			[MAX_TILETEMPLATES];
	GPtrPOD(STileNavigability)		m_lstTileNavigabilityDataTpl	[MAX_TILETEMPLATES];
	GPtrPOD(STileBrightness)		m_lstTileBrightnessDataTpl		[MAX_TILETEMPLATES];

	CGCamera						m_Camera;
	CGLight							m_DirectionalLight;
	CGLight							m_PointLight;
	CGLight							m_SpotLight;

	// camera effects
	bool							m_bFalling;
	float							m_verticalSpeed;

	bool							m_bDisplayFrameRate;

	int32_t							m_nViewportWidth, m_nViewportHeight;
};
GDECLARE_NCO(CGGame);

namespace god
{
	error_t setupRenderTargetImage( IGVideoContext* pManager, unsigned int nWidth, unsigned int nHeight, GODS(IGVideoImage)* out_pImage );
	error_t setupRenderTargetViewport( uint32_t nOffsetX, uint32_t nOffsetY, unsigned int nWidth, unsigned int nHeight, CGViewport& out_Viewport );
	error_t setupRenderTargetSprite( GODS(IGVideoImage) pImage, GODS(IGVideoProgram) pProgram, uint32_t nOffsetX, uint32_t nOffsetY, unsigned int nWidth, unsigned int nHeight, class god::CGSpriteNode& out_SpriteNode );


	static const uint8_t g_TileSetIndices2x2[16][9] = 
	{
		// Tileset #0
		{	
			 0, 0,
			 0, 0
		},
		// Tset #1
		{
			 6, 0,
			 0, 0
		},
		// Tset #2
		{
			 2, 6,
			 0, 0
		},
		// Tset #3
		{
			 4, 0,
			 6, 0
		},
		// Tset #4
		{
			 0, 4,
			 2, 6
		},
		// Tset #5
		{
			4, 6,
			 0, 0
		},
		// Tset #6
		{
			 2, 0,
			 6, 0
		},
		// Tset #7
		{
			 6, 4,
			 2, 6
		},
		// Tset #8
		{
			 6, 6,
			 6, 0
		},
		// Tset #9
		{
			 2, 2,
			 2, 6
		},
		// Tset #10
		{
			 4, 4,
			 4, 6
		},
		// Tset #11
		{
			0, 6,
			6, 0
		},
		// Tset #12
		{
			 4, 2,
			 2, 6
		},
		// Tset #13
		{
			 2, 4,
			 4, 6
		},
		// Tset #14
		{
			 6, 2,
			 4, 6
		},
		// Tset #15
		{
			 0, 2,
			 4, 6
		}
	};

	static const int8_t g_TileSetIndices3x3[16][9] = 
	{
		// Tileset #0
		{	-1, -1, -1,
			-1, 0, 0,
			-1, 0, 0
		},
		// Tileset #1
		{	-1, 3, -1,
			 1, 6, 0,
			-1, 0, 0
		},
		// Tileset #2
		{	-1, -1, 3,
			-1, 2, 6,
			-1, 0, 0
		},
		// Tileset #3
		{	-1, -1, -1,
			-1, 4, 0,
			 1, 6, 0
		},
		// Tileset #4
		{	-1, -1, -1,
			-1, 0, 4,
			-1, 2, 6
		},
		// Tileset #5
		{	-1, 3, 3,
			1, 4, 6,
			-1, 0, 0
		},
		// Tileset #6
		{	-1, 3, -1,
			 1, 2, 0,
			 1, 6, 0
		},
		// Tileset #7
		{	-1, 3, -1,
			 1, 6, 4,
			-1, 2, 6
		},
		// Tileset #8
		{	-1, -1, 3,
			-1, 6, 6,
			 1, 6, 0
		},
		// Tileset #9
		{	-1, -1, 3,
			-1, 2, 2,
			-1, 2, 6
		},
		// Tileset #10
		{	-1, -1, -1,
			-1, 4, 4,
			 1, 4, 6
		},
		// Tileset #11
		{	-1, 3, 3,
			1, 0, 6,
			1, 6, 0
		},
		// Tileset #12
		{	-1, 3, 3,
			 1, 4, 2,
			-1, 2, 6
		},
		// Tileset #13
		{	-1, 3, -1,
			 1, 2, 4,
			 1, 4, 6
		},
		// Tileset #14
		{	-1, -1, 3,
			-1, 6, 2,
			 1, 4, 6
		},
		// Tileset #15
		{	-1, 3, 3,
			 1, 0, 2,
			 1, 4, 6
		}
	};

};



#endif // __GGAME_H