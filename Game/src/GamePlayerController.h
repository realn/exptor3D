#pragma once

#include <event_Mapper.h>
#include <event_Observer.h>

#include "GameController.h"
#include "GamePlayer.h"

class CLocalPlayerController :
	public event::IObserver,
	public IController
{
private:
	CPlayer&	Player;
	bool		Move[4];
	bool		Run;
	bool		FireWeapon;
	float		Angle;
	unsigned	Weapon;
	event::Mapper mapper;

public:
	CLocalPlayerController( CPlayer& player );
	virtual ~CLocalPlayerController();

	void	processEvent(const event::Event& event) override;
	void	Update() override;
};