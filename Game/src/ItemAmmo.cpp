#include "ItemAmmo.h"

/*	BONUS - AMUNICJA	*/
CItemAmmo::CItemAmmo( const unsigned weapType, const unsigned ammoCount, GLModel* model ) :
	WeapType( weapType ),
	AmmoCount( ammoCount )
{
	Model = model;
	type = BONUS_TYPE_AMMO;
}

const unsigned CItemAmmo::GetWeapType() const
{
	return WeapType;
}

const unsigned CItemAmmo::GetAmmoCount() const
{
	return AmmoCount;
}
