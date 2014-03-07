#include "ItemAmmo.h"

/*	BONUS - AMUNICJA	*/
CItemAmmo::CItemAmmo( const WEAPON_TYPE weapType, const unsigned ammoCount ) :
	CItem(ITEM_TYPE::AMMO),
	WeapType( weapType ),
	AmmoCount( ammoCount )
{
}

CItemAmmo::~CItemAmmo()
{
}

const bool	CItemAmmo::LoadGraphic( CTexManager& texManager, CModelManager& modelManager )
{
	Model = modelManager.Get( "rocketlun-model.glm" );
	GfxLoaded = Model != nullptr;

	return GfxLoaded;
}

const WEAPON_TYPE CItemAmmo::GetWeaponType() const
{
	return WeapType;
}

const unsigned CItemAmmo::GetAmmoCount() const
{
	return AmmoCount;
}
