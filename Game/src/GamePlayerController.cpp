#include <CBSDL/Consts.h>

#include "GamePlayerController.h"
#include "EventInput.h"

enum MOVE_INDEX
{
	MOVE_FORWARD = 0,
	MOVE_BACKWARD = 1,
	MOVE_STRAFE_LEFT = 2,
	MOVE_STRAFE_RIGHT = 3,
};

CLocalPlayerController::CLocalPlayerController( CPlayer& player ) :
	Player( player ),
	FireWeapon( false ),
	Angle( 0.0f ),
	Run( false ),
	Weapon( 0 )
{
	memset( Move, 0, sizeof(bool) * 4 );
}

CLocalPlayerController::~CLocalPlayerController()
{

}

void	CLocalPlayerController::ProcessEvent( const CEvent& event )
{
	CEventKey	keyEvent;
	CEventMouse	mouseEvent;

	switch (static_cast<EVENT_INPUT_TYPE>(event.Type))
	{
	case EVENT_INPUT_TYPE::KEYDOWN:
		memcpy( &keyEvent, &event, sizeof(CEventKey) );
		ProcessKey( keyEvent.Key, true );
		break;

	case EVENT_INPUT_TYPE::KEYUP:
		memcpy( &keyEvent, &event, sizeof(CEventKey) );
		ProcessKey( keyEvent.Key, false );
		break;

	case EVENT_INPUT_TYPE::MOUSEMOVEDIF:
		memcpy( &mouseEvent, &event, sizeof(CEventMouse) );
		Angle += (float)mouseEvent.X * 0.1f;
		break;

	default:
		break;
	}
}

void	CLocalPlayerController::Update()
{
	if( Move[MOVE_FORWARD] )
		Player.DoAction( GAME_ACTION::MOVE_FORWARD );
	if( Move[MOVE_BACKWARD] )
		Player.DoAction( GAME_ACTION::MOVE_BACK );
	if( Move[MOVE_STRAFE_LEFT] )
		Player.DoAction( GAME_ACTION::MOVE_STRAFE_LEFT );
	if( Move[MOVE_STRAFE_RIGHT] )
		Player.DoAction( GAME_ACTION::MOVE_STRAFE_RIGHT );
	if( FireWeapon )
		Player.DoAction( GAME_ACTION::DO_ATTACK );

	Player.SetAngle( Angle );
	Player.SwichWeap( Weapon );
	Player.SetMoveSpeed( Run );
}

void	CLocalPlayerController::ProcessKey( const unsigned key, const bool down )
{
	switch (key)
	{
	case 'W':
		Move[MOVE_FORWARD] = down;
		break;

	case 'S':
		Move[MOVE_BACKWARD] = down;
		break;

	case 'A':
		Move[MOVE_STRAFE_LEFT] = down;
		break;

	case 'D':
		Move[MOVE_STRAFE_RIGHT] = down;
		break;

	//case VK_LCONTROL:
	//case VK_LBUTTON:
	//	FireWeapon = down;
	//	break;

	case '1':	Weapon = 0;	break;
	case '2':	Weapon = 1;	break;
	case '3':	Weapon = 2;	break;
	case '4':	Weapon = 3;	break;
	case '5':	Weapon = 4;	break;
	case '6':	Weapon = 5;	break;
	case '7':	Weapon = 6;	break;
	case '8':	Weapon = 7;	break;
	case '9':	Weapon = 8;	break;

	//case VK_SHIFT:
	//	Run = down;
	//	break;

	default:
		break;
	}
}