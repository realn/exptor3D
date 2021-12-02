#pragma once

#include "Weapon.h"

class CWeaponRocketLuncher : 
	public CWeapon
{
private:
	gfx::ModelManager&	ModelManager;

public:
	CWeaponRocketLuncher( gfx::ModelManager& modelManager );
	virtual ~CWeaponRocketLuncher();

	void	Render() override;

protected:
	void	OnFired() override;
};
