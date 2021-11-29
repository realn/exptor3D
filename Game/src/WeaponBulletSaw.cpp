#include "WeaponBulletSaw.h"

/*====================
	KLASA CBullSaw
====================*/
CBullSaw::CBullSaw( CActor* owner, const float damage, const glm::vec3& pos ) :
	CProjectile(PROJECTILE_TYPE::MELEE, owner, damage, pos, glm::vec3(), 0.0f )
{
	Radius = 0.5f;
}

void CBullSaw::Update( const float fTD )
{
	DeleteThis = true;
}

float CBullSaw::DoTest( CDynamic* Dum, float Armor )
{
	if( Dum != NULL && Dum != Owner && Distance( Dum->NextPos, Pos ) < 5.0f  )
	{
		float ArmorMod = 1.0f - ( ( Armor * 0.5f ) / 100.0f );
		return Damage * ArmorMod;
	}
	return 0.0f;
}
