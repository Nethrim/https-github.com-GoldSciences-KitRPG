
#ifndef min
#define min( a, b ) ( ((a)>(b)) ? (b) : (a) )
#endif 
#ifndef max
#define max( a, b ) ( ((a)<(b)) ? (b) : (a) )
#endif 
/// This file is part of the Game Object Data System
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GGame.h"
#include "GGame_function_macros.h"

//#define GPROFILE_ENABLED
#include "GCore_printf.h"
#include "GDebugger_core.h"


using namespace god;

error_t	CGGame::ProcessUserInput( float fElapsedTime )
{ 
	__CGGAME_FUNCTION_HEADER( -1 );
	GPtrPOD(SCamera) pCameraData = m_MainCamera.GetCameraData();

	GPtrNCO(IGKeyboard) pKeyboard = GetKeyboardManager();
	SVector3 vCam;
	float fScale = 3;
	if( pKeyboard->IsKeyDown( GKEY_LSHIFT ) || pKeyboard->IsKeyDown( GKEY_RSHIFT ) )
		fScale *= 5;

	if( pKeyboard->IsKeyDown( GKEY_W ) )
	{
		vCam	= *GVectorNormalizeInPlace( &pCameraData->Front );
		vCam.y	= 0;
		GVectorAddScaledInPlace( &pCameraData->Position, &vCam, fElapsedTime*fScale );
	}
	if( pKeyboard->IsKeyDown( GKEY_S ) )
	{
		vCam	= *GVectorNormalizeInPlace( &pCameraData->Front );
		vCam.y	= 0;
		GVectorSubstractScaledInPlace( &pCameraData->Position, &vCam, fElapsedTime*fScale );
	}
	if( pKeyboard->IsKeyDown( GKEY_D ) )
		GVectorAddScaledInPlace( &pCameraData->Position, GVectorNormalizeInPlace( &pCameraData->Right ),fElapsedTime*fScale );
	if( pKeyboard->IsKeyDown( GKEY_A ) )
		GVectorSubstractScaledInPlace( &pCameraData->Position, GVectorNormalizeInPlace( &pCameraData->Right ), fElapsedTime*fScale );
	if( pKeyboard->IsKeyDown( GKEY_SPACE ) )
		pCameraData->Position.y += 1.0f*fElapsedTime*fScale;
	if( pKeyboard->IsKeyDown( GKEY_C ) )
		pCameraData->Position.y -= 1.0f*fElapsedTime*fScale;

	return 0; 
}

void CGGame::OnKeyUp( unsigned char key )
{
	switch( key )
	{
	case GKEY_N:
		ResetBoard();
	}
};

void CGGame::OnKeyDown( unsigned char key )
{
	SPointL cursorDelta = {0,0};
	int32_t camx = 0, camz = 0;
	int32_t x = 0, z = 0;

	uint32_t keyIndex = -1;
	for( uint32_t iKey=0; iKey < MAX_KEYS; iKey++ )
	{
		if( key == m_mapKeyboard[iKey] )
		{
			keyIndex = iKey;
			break;
		}
	}

	GPtrNCO(IGKeyboard)keyb = m_pKeyboardManager;
	switch(keyIndex)
	{
	case KEY_CURSOR_RIGHT	: cursorDelta.x+=1; break;
	case KEY_CURSOR_LEFT	: cursorDelta.x-=1; break;
	case KEY_CURSOR_UP		: cursorDelta.y+=1; break;
	case KEY_CURSOR_DOWN	: cursorDelta.y-=1; break;
	case KEY_CURSOR_ACTION	: break;
	case KEY_CURSOR_DELETE	: break;
		 
	case KEY_MOVE_FORWARD	: x+=1; break;
	case KEY_MOVE_BACK		: x-=1; break;
	case KEY_MOVE_LEFT		: z-=1; break;
	case KEY_MOVE_RIGHT		: z+=1; break;
		 
	case KEY_CAMERA_FORWARD	: camx+=1; break;
	case KEY_CAMERA_BACK	: camx-=1; break;
	case KEY_CAMERA_LEFT	: camz-=1; break;
	case KEY_CAMERA_RIGHT	: camz+=1; break;
		 
	case KEY_ACTION			: break;
	case KEY_CANCEL			: 
		DeselectUnit();
		break;
	case KEY_SHOOT			: break;
	case KEY_RUN			: break;
	case KEY_JUMP			: 
		if( m_bSelectionActive )
			MoveUnit( m_ActivePlayer, m_UnitSelected, m_UnitCursor.Position );
		else
			SelectUnit();
		break;
	case KEY_CROUCH			: break;

	case KEY_NEXTUNIT		: 
		if( keyb->IsKeyDown(GKEY_LSHIFT) || keyb->IsKeyDown(GKEY_RSHIFT) )
			NextUnit( true );
		else
			NextUnit( false );
		break;

	case KEY_NEXTBASEUNIT		: 
		break;
	case KEY_NEXTTEAM		: break;
	case KEY_NEXTTILE		: break;
	default:
		break;
	};
	MoveUnitCursor(cursorDelta); 
};

void CGGame::OnMouseMove( int32_t x, int32_t y, int32_t z )
{
	GPROFILE_METHOD( "CGGame", "OnMouseMove" );
	GPtrPOD(SCamera) pCameraData = m_GameWorld.GetCamera()->GetCameraData();

	float fScale = 1.0f/150.0f, fScaleZ = 0.01f;
	GPtrNCO(IGKeyboard) pKeyboard = m_pKeyboardManager;
	if( pKeyboard->IsKeyDown( GKEY_LSHIFT ) || pKeyboard->IsKeyDown( GKEY_RSHIFT ) )
		fScaleZ *= 10.0f;

	float fRotationX = y*fScale, fRotationY = x*fScale;

	SMatrix3 mX, mY;
	mY.RotationY( fRotationY );
	mX.RotationArbitraryAxis( pCameraData->Right, fRotationX );
	GVectorNormalizeInPlace( &pCameraData->Front );
	GMatrixTransformInPlace( &pCameraData->Front, &mX );
	GMatrixTransformInPlace( &pCameraData->Front, &mY );
//		GMatrixTransformInPlace( &pCameraData->Right, &mX );
	GMatrixTransformInPlace( &pCameraData->Right, &mY );
	GMatrixTransformInPlace( &pCameraData->Up, &mX );
	GMatrixTransformInPlace( &pCameraData->Up, &mY );

	GVectorAddScaledInPlace( &pCameraData->Position, GVectorNormalizeInPlace( &pCameraData->Front ), ((float)z)*fScaleZ );

	pCameraData->Up = GVECTOR3_UP;
	GVectorCrossAndNormalize( &pCameraData->Right, &pCameraData->Up, &pCameraData->Front );
};

void CGGame::OnMouseButtonUp( uint8_t nButton )
{
};

void CGGame::OnMouseButtonDown( uint8_t nButton )
{
};
