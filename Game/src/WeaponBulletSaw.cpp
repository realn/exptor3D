#include "stdafx.h"
#include "WeaponBulletSaw.h"

/*====================
	KLASA CBullSaw
====================*/
CBullSaw::CBullSaw( CActor* owner, const float damage, const Vector3f& pos ) :
	CProjectile(PROJECTILE_TYPE::MELEE, owner, damage, pos, Vector3f(), 0.0f )
{
	Radius = 0.5f;
}

void CBullSaw::Update( const float fTD )
{
	DeleteThis = true;
}

float CBullSaw::DoTest( CDynamic* Dum, float Armor )
{
	if( Dum != NULL && Dum != Owner && DistanceSq( Dum->NextPos, Pos ) < POW( 5.0f )  )
	{
		float ArmorMod = 1.0f - ( ( Armor * 0.5f ) / 100.0f );
		return Damage * ArmorMod;
	}
	return 0.0f;
}
