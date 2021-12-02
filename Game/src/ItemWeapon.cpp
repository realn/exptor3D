#include "ItemWeapon.h"

CItemWeapon::CItemWeapon( const WEAPON_TYPE type, const unsigned ammo ) :
	CItem( ITEM_TYPE::WEAPON ),
	WeaponType( type ),
	AmmorCount( ammo )
{

}

CItemWeapon::~CItemWeapon()
{
}

const bool	CItemWeapon::LoadGraphic( gfx::TextureRepository& texManager, CModelManager& modelManager )
{
	switch (WeaponType)
	{
	case WEAPON_TYPE::ROCKET_LUNCHER:
		Model = modelManager.Get( "rocketlun-model.glm" );
		break;

	default:
		Model = nullptr;
		break;
	}

	GfxLoaded = Model != nullptr;

	return GfxLoaded;
}

const WEAPON_TYPE	CItemWeapon::GetWeaponType() const
{
	return WeaponType;
}

const unsigned	CItemWeapon::GetAmmoCount() const
{
	return AmmorCount;
}