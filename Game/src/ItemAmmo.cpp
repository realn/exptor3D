#include "ItemAmmo.h"

/*	BONUS - AMUNICJA	*/
CItemAmmo::CItemAmmo( const unsigned weapType, const unsigned ammoCount, GLModel* model ) :
	CItem(ITEM_TYPE::AMMO),
	WeapType( weapType ),
	AmmoCount( ammoCount )
{
	Model = model;
}

const unsigned CItemAmmo::GetWeapType() const
{
	return WeapType;
}

const unsigned CItemAmmo::GetAmmoCount() const
{
	return AmmoCount;
}
