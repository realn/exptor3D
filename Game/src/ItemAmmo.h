#pragma once

#include "Item.h"
#include "Weapon.h"

class CItemAmmo : 
	public CItem
{
private:
	const WEAPON_TYPE WeapType;
	const unsigned AmmoCount;

public:
	CItemAmmo( const WEAPON_TYPE weapType, const unsigned ammoCount );
	virtual ~CItemAmmo();

	const bool	LoadGraphic( gfx::TextureRepository & texManager, gfx::ModelManager& modelManager) override;

	const WEAPON_TYPE	GetWeaponType() const;
	const unsigned		GetAmmoCount() const;
};
