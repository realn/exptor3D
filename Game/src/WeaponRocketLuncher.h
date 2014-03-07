#pragma once

#include "Weapon.h"

class CWeaponRocketLuncher : 
	public CWeapon
{
private:
	CModelManager&	ModelManager;

public:
	CWeaponRocketLuncher( CModelManager& modelManager );
	virtual ~CWeaponRocketLuncher();

	void	Render() override;

protected:
	void	OnFired() override;
};
