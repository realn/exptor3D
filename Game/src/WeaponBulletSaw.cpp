#include "WeaponBulletSaw.h"

/*====================
	KLASA CBullSaw
====================*/
CBullSaw::CBullSaw( CActor* owner, const float damage, const Vector3f& pos ) :
	CBullet( owner, damage, pos, Vector3f(), 0.0f )
{
	Radius = 0.5f;
	Type = BULLET_TYPE_SAW;
}

void CBullSaw::Update( const float fTD )
{
	CanDelete = true;
}

float CBullSaw::DoTest( CDynamic* Dum, float Armor )
{
	if( Dum != NULL && Dum != Owner && mathDistSq( Dum->NextPos, Pos ) < POW( 5.0f )  )
	{
		float ArmorMod = 1.0f - ( ( Armor * 0.5f ) / 100.0f );
		CanDelete = true;
		return Damage * ArmorMod;
	}
	return 0.0f;
}
