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
	Angle( 0.0f )
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

	switch (event.Type)
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
}

void	CLocalPlayerController::ProcessKey( const unsigned key, const bool down )
{
	switch (key)
	{
	case 'W':
	case VK_UP:
		Move[MOVE_FORWARD] = down;
		break;

	case 'S':
	case VK_DOWN:
		Move[MOVE_BACKWARD] = down;
		break;

	case 'A':
	case VK_LEFT:
		Move[MOVE_STRAFE_LEFT] = down;
		break;

	case 'D':
	case VK_RIGHT:
		Move[MOVE_STRAFE_RIGHT] = down;
		break;

	case VK_LBUTTON:
		FireWeapon = down;
		break;

	default:
		break;
	}
}