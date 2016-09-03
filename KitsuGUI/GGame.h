/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GBaseGame.h"
#include "GGame_macros.h"
#include "GGUI.h"

#include "draw.h"

#ifndef __GGAME_H
#define __GGAME_H

namespace god
{

	/// CGGame will be the custom application
	class CGGame : public CGBaseGame
	{
	public:
		__GDECLARE_IGGAME_INHERITED_MEMBERS();
		CGGame( void );
		virtual ~CGGame( void );

		//---------------- Declare methods here
		virtual error_t			DoNothing( void );

	protected:
		virtual error_t			InitCursorSprite( void );
		virtual error_t			InitGUISprite( void );
	private:
		CGUIGPU					m_MainGUI;

		GPtrNCO(IGFont)			m_Font;

		klib::SGame*			m_pGame;

		GPtrNCO(SBuffer) 		m_oldGUIScreen;
		bool					m_bDisplayFrameRate;
	};
	GDECLARE_NCO(CGGame);

	//error_t setupRenderTargetImage( GODS(IGVideoContext) pManager, unsigned int nWidth, unsigned int nHeight, GODS(IGVideoImage)* out_pImage );
	//error_t setupRenderTargetViewport( uint32_t nOffsetX, uint32_t nOffsetY, unsigned int nWidth, unsigned int nHeight, CGViewport& out_Viewport );
	//error_t setupRenderTargetSprite( GODS(IGVideoImage) pImage, GODS(IGVideoProgram) pProgram, uint32_t nOffsetX, uint32_t nOffsetY, unsigned int nWidth, unsigned int nHeight, class god::CGSpriteNode& out_SpriteNode );

};

#endif // __GGAME_H