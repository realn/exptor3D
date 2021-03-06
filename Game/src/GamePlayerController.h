#pragma once

#include "EventManager.h"
#include "GameController.h"
#include "GamePlayer.h"

class CLocalPlayerController :
	public IEventObserver,
	public IController
{
private:
	CPlayer&	Player;
	bool		Move[4];
	bool		Run;
	bool		FireWeapon;
	float		Angle;
	unsigned	Weapon;

public:
	CLocalPlayerController( CPlayer& player );
	virtual ~CLocalPlayerController();

	void	ProcessEvent( const CEvent& event ) override;
	void	Update() override;

private:
	void	ProcessKey( const unsigned key, const bool down );
};