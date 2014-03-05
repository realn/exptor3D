#pragma once

#include "Item.h"
#include "Weapon.h"

class CItemWeapon :
	public CItem
{
private:
	const WEAPON_TYPE	WeaponType;
	const unsigned		AmmorCount;

public:
	CItemWeapon( const WEAPON_TYPE type, const unsigned ammo );

	const bool	LoadGraphic( CTexManager& texManager, GLModelManager& modelManager ) override;

	const WEAPON_TYPE	GetWeaponType() const;
	const unsigned		GetAmmoCount() const;
};