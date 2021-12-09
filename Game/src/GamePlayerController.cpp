#include <CBSDL/Consts.h>

#include "GamePlayerController.h"

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

	mapper.addState(L"player_forward", [&](const event::EventState& s) { Move[MOVE_FORWARD] = s.getState(); });
	mapper.addState(L"player_backward", [&](const event::EventState& s) { Move[MOVE_BACKWARD] = s.getState(); });
	mapper.addState(L"player_left", [&](const event::EventState& s) { Move[MOVE_STRAFE_LEFT] = s.getState(); });
	mapper.addState(L"player_right", [&](const event::EventState& s) { Move[MOVE_STRAFE_RIGHT] = s.getState(); });
	mapper.addState(L"player_attack", [&](const event::EventState& s) { FireWeapon = s.getState(); });
}

CLocalPlayerController::~CLocalPlayerController() = default;

void	CLocalPlayerController::processEvent( const event::Event& event )
{
	mapper.executeEvent(event);
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