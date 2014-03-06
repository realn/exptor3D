#include "WeaponBulletExplosion.h"

#include "gui.h"
#include "Special.h"

/*====================
	KLASA CBullExplosion
	eksplozja
====================*/
CBullExplosion::CBullExplosion( CActor* owner, const float damage, const Vector3f& pos, const float radius, const float speed ) :
	CBullet( owner, damage, pos, Vector3f(), 0.0f ),
	Speed( speed ),
	Power( radius ),
	thisPower( 0.0f )
{
	SEManager.AddEffect( new CEffectExplosion( Pos, Power, speed ) );
}

float CBullExplosion::DoTest( CDynamic *Dum, float Armor )
{
	if( Dum == NULL || Dum == Owner )
		return 0.0f;

	float ArmorMod = 1.0f - ( ( Armor * 0.5f ) / 100.0f );
	float dist = mathDist( Pos, Dum->NextPos );

	if( dist < Dum->Radius + this->thisPower )
	{
		if( dist < Dum->Radius + ( this->thisPower / 2.0f ) )
			ArmorMod *= 2.0f;

		return Damage * ArmorMod * GUI.GetSpeed();
	}

	return 0.0f;
}

void CBullExplosion::Update( const float fTD )
{
	thisPower += Speed * fTD;

	if( thisPower >= Power )
	{
		CanDelete = true;
	}
}

void CBullExplosion::Render()
{
#ifdef LIGHT_TEST
	float t[] = { Pos.X, Pos.Y, Pos.Z, 1.0f };
	glLightfv( GL_LIGHT1, GL_POSITION, t );
#endif
	// Nic...
}
