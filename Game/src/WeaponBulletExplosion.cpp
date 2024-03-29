#include "WeaponBulletExplosion.h"

#include "Special.h"

/*====================
	KLASA CBullExplosion
	eksplozja
====================*/
CBullExplosion::CBullExplosion( CActor* owner, const float damage, const glm::vec3& pos, const float radius, const float speed ) :
	CProjectile( PROJECTILE_TYPE::EXPLOSION, owner, damage, pos, glm::vec3(), 0.0f ),
	Speed( speed ),
	Power( radius ),
	thisPower( 0.0f )
{
	pGLevel->AddEntity( new CEffectExplosion( Pos, Power, speed ) );
}

float CBullExplosion::DoTest( CDynamic *Dum, float Armor )
{
	if( Dum == NULL || Dum == Owner )
		return 0.0f;

	float ArmorMod = 1.0f - ( ( Armor * 0.5f ) / 100.0f );
	float dist = Distance( Pos, Dum->NextPos );

	if( dist < Dum->Radius + this->thisPower )
	{
		if( dist < Dum->Radius + ( this->thisPower / 2.0f ) )
			ArmorMod *= 2.0f;

		return Damage * ArmorMod;
	}

	return 0.0f;
}

void CBullExplosion::Update( const float fTD )
{
	thisPower += Speed * fTD;

	if( thisPower >= Power )
	{
		DeleteThis = true;
	}
}

void CBullExplosion::Render()
{
//#ifdef LIGHT_TEST
//	float t[] = { Pos.X, Pos.Y, Pos.Z, 1.0f };
//	glLightfv( GL_LIGHT1, GL_POSITION, t );
//#endif
	// Nic...
}
