/// This file is part of the Game Object Data System
/// Copyright © 2013-2016: Pablo Ariel Zorrilla Cepeda
#include "GBaseGame.h"
#include "GGameEntity.h"
#include "GChess_core.h"
#include "GGame_macros.h"


#include <math.h>

#ifndef __GGAME_H__9874538097590328479082734908234__
#define __GGAME_H__9874538097590328479082734908234__

#define	KEY_CREEP			0

#define	KEY_CURSOR_UP		(KEY_CREEP			+1)
#define	KEY_CURSOR_DOWN		(KEY_CURSOR_UP		+1)
#define	KEY_CURSOR_LEFT		(KEY_CURSOR_DOWN	+1)
#define	KEY_CURSOR_RIGHT	(KEY_CURSOR_LEFT	+1)
#define	KEY_CURSOR_ACTION	(KEY_CURSOR_RIGHT	+1)
#define	KEY_CURSOR_DELETE	(KEY_CURSOR_ACTION	+1)

#define	KEY_MOVE_FORWARD	(KEY_CURSOR_DELETE	+1)
#define	KEY_MOVE_BACK		(KEY_MOVE_FORWARD	+1)
#define	KEY_MOVE_LEFT		(KEY_MOVE_BACK		+1)
#define	KEY_MOVE_RIGHT		(KEY_MOVE_LEFT		+1)

#define	KEY_CAMERA_FORWARD	(KEY_MOVE_RIGHT		+1)
#define	KEY_CAMERA_BACK		(KEY_CAMERA_FORWARD	+1)
#define	KEY_CAMERA_LEFT		(KEY_CAMERA_BACK	+1)
#define	KEY_CAMERA_RIGHT	(KEY_CAMERA_LEFT	+1)

#define	KEY_NEXTUNIT		(KEY_CAMERA_RIGHT	+1)
#define	KEY_NEXTTEAM		(KEY_NEXTUNIT		+1)
#define	KEY_NEXTBASEUNIT	(KEY_NEXTTEAM		+1)
#define	KEY_NEWTEAM			(KEY_NEXTBASEUNIT	+1)
#define	KEY_DELTEAM			(KEY_NEWTEAM		+1)
#define	KEY_NEXTTILE		(KEY_DELTEAM		+1)
							 
#define	KEY_ACTION			(KEY_NEXTTILE		+1)
#define	KEY_CANCEL			(KEY_ACTION			+1)
#define	KEY_SHOOT			(KEY_CANCEL			+1)
#define	KEY_RUN				(KEY_SHOOT			+1)
#define	KEY_JUMP			(KEY_RUN			+1)	
#define	KEY_CROUCH			(KEY_JUMP			+1)

#define MAX_KEYS			(KEY_CROUCH			+1)


#define STATE_INITIALIZING	1
#define STATE_LOADING		2
#define STATE_PLAYING		3
#define STATE_SAVING		4

#define MINIMAP_SCALE 4

namespace god
{
	struct SCommonCursorStuff
	{
		GPtrPOD(SMaterial)				Valid;		// green
		GPtrPOD(SMaterial)				Invalid;	// red
		GPtrPOD(SMaterial)				Danger;		// yellow
		GPtrPOD(SMaterial)				Empty;		// white
		GPtrPOD(SMaterial)				Select;		// cyan
		GPtrNCO(IGVideoProgram)				Shader;		// loaded from spritecursorminimap.fx
		SPointL							Position;
		CGSpriteNode					Sprite;
	};

	struct SUnitCursorStuff : public SCommonCursorStuff
	{
		float							GlowAlphaTime;
		bool							GlowActive;
	};

	struct SUICursorStuff : public SCommonCursorStuff
	{
	};

	struct SWindow
	{
		CGSpriteNode					ClientArea;
		glabel							Title;
		GPtrNCO(IGFont)						Font;
		void							SetPosition( int32_t x, int32_t y )
		{
			Position.x = x; Position.y = y;
			UpdateTransform();
		};
		void							SetSize( int32_t x, int32_t y )
		{
			Size.x = x; Size.y = y;
			UpdateTransform();
		};
		const SPointL&					GetPosition( void ) const { return Position; };
		const SPointL&					GetSize( void ) const { return Size; };
		const bool						IsMouseOver( int32_t x, int32_t y ) const;
		const bool						IsMouseOver( SVector2& v ) const 
		{
			return IsMouseOver( lroundf(v.x), lroundf(v.y) );
		};
		const bool						IsMouseOver( SPointL& p ) const 
		{
			return IsMouseOver( p.x, p.y );
		};

	protected:
		SPointL							Position;
		SPointL							Size;

		void UpdateTransform( void )
		{
			int TitleHeight = (Font ? Font->GetHeight() : 0)+4;
			GPtrPOD(STransform) xform = ClientArea.GetGlobalTransform();
			xform.clone(xform);
			xform->Matrix.Scale( SVector3( (float)Size.x, (float)Size.y, 1 ), true );
			xform->Matrix.SetTranslation( SVector3( (float)Position.x, (float)Position.y+TitleHeight, 0 ) );
			ClientArea.SetGlobalTransform( xform );
		}
	};

	/// CGGame will be the custom application
	class CGGame : public CGBaseGame, public IGMouseHandler, public IGKeyboardHandler
	{
	public:
		CGGame( void );
		virtual ~CGGame( void );
		//---------------- Custom game methods

		//---------------- IGGame methods
		__GDECLARE_IGGAME_INHERITED_MEMBERS();

		//---------------- Custom game methods
		virtual void						OnKeyUp( unsigned char key );
		virtual void						OnKeyDown( unsigned char key );
		virtual void						OnMouseMove( int32_t x, int32_t y, int32_t z );
		virtual void						OnMouseButtonUp( uint8_t nButton );
		virtual void						OnMouseButtonDown( uint8_t nButton );


		error_t								UpdateChessStuff		(float fElapsedTime);
		error_t								UpdateGGUIStuff			(float fElapsedTime);
		error_t								UpdateLabyrinthStuff	(float fElapsedTime);

	protected:
		CGGameEntityManager					m_EntityManager;
		CGGameWorld							m_GameWorld;
		//CGGameWorld							m_UnusedPieces;
		CGGameWorld							m_MeshWorld;

		SGameFrame							m_CurrentGameFrame;
		SGameFrame							m_FutureGameFrameFromCursor;

		SPlayer*							m_ActivePlayer;
		uint32_t							m_UnitSelected;

		GGrid2DPOD(STileGeometry)			m_BoardTiles;
		GListPOD(STileTexture)				m_BoardTileTextures;
		glabel								m_BoardTextures[1];
		SPointL								m_SelectedPos;
		bool								m_bSelectionActive;
		GPtrNCO(IGVideoProgram)				m_DefaultShader;
		CGLight								m_DirectionalLight;
		// input
		uint8_t								m_mapKeyboard[MAX_KEYS];
		CGRenderNode						m_SelectedPieceModels[6];

		SUnitCursorStuff					m_UnitCursor;
		SUICursorStuff						m_UICursor;

		GPtrPOD(SMaterial)					m_MaterialWhite;
		GPtrPOD(SMaterial)					m_MaterialBlack;

		CGSpriteNode						m_SpriteMinimap;
		CGSpriteNode						m_SpriteMinimapNextFromCursor;
		GPtrNCO(IGVideoProgram)				m_SpriteShader;
		GPtrNCO(IGVideoProgram)				m_ShaderCursorMinimap;

		GPtrPOD(SMaterial)					m_MaterialSelectedPiece;	// blue
		GPtrPOD(SMaterial)					m_MaterialSelectedOrig;

		GPtrNCO(SBuffer)					m_BufferUnitsCurrent;
		//GPtrNCO(SBuffer)					m_BufferUnitsNextFromCursor;
		//GPtrNCO(SBuffer)					m_BufferTiles;

		error_t								InitChessMaterials( void );
		error_t								InitChessShaders( void );

		// Cursors
		error_t								InitChessUICursor( void );
		error_t								InitChessUnitCursor( void );
		error_t								InitChessUnitCursorMesh( void );
		error_t								InitChessUnitCursorSprite( void );

		// Scene
		error_t								InitChessMeshes( void );
		error_t								InitChessBoardMesh( void );
		error_t								InitChessUnitMeshes( void );
		error_t								InitChessLights( void );
		error_t								InitChessCamera( void );
		error_t								InitChessPlayerUnits( void );

		error_t								InitChessMinimapSprite( void );

		error_t								InitChessInput( void );

		error_t								InitChess( void );

		error_t								MoveUnit( SPlayer* pPlayer, uint32_t iPiece, const SPointL& TargetPosition );
		error_t								MoveUnitCursor( SPointL& TargetPos );
		error_t								SelectUnit( void );
		error_t								DeselectUnit( void );

		void								ResetBoard( void );
		error_t								CalculateAvailablePositions( void );

		void								NextUnit( bool bReverse );
		void								RefreshSpriteMinimapChess( void );
		void								UpdateImageUnitsNextFromCursor( void );

		klib::SGame*						m_pGame;
	};
	GDECLARE_NCO(CGGame);
};

#endif // __GGAME_H__9874538097590328479082734908234__