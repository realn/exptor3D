#include "stdafx.h"
#include "WeaponRocketLuncher.h"
#include "WeaponBulletRocket.h"
#include "Level.h"

/*	ROCKET LUNCHER	*/
CWeaponRocketLuncher::CWeaponRocketLuncher( CModelManager& modelManager ) :
	CWeapon( WEAPON_TYPE::ROCKET_LUNCHER, "Wyrzutnia Rakiet" ),
	ModelManager( modelManager )
{
	DamageMin = 5.0f;
	DamageMax = 15.0f;
	ShotTime = 0.2f;
	ShotPause = 1.0f;

	MaxAmmo = 20;

	Pos.Set( 1.5f, -1.5f, 3.5f );

	Model = modelManager.Get( "rocketlun-model.glm" );
}

CWeaponRocketLuncher::~CWeaponRocketLuncher()
{
}


void CWeaponRocketLuncher::Render()
{
	if( !InHand )
		return;

	auto pos = GenPos() + GenShakePos();

	// Zachowujemy aktualn¹ Macierz
	glPushMatrix();

	glLoadIdentity();

	float Zcorr = 0.0f;
	switch (State)
	{
	case WEAPON_STATE::FIRING:
		Zcorr = (Time / ShotTime);
		break;
	case WEAPON_STATE::RECOIL:
		Zcorr = ((ShotPause - Time) / ShotPause);
		break;
	}

	glTranslatef( pos.X, pos.Y, -(pos.Z - Zcorr) );

	glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );
	Model->RenderObject( 0 );

	// Przywracamy Macierz
	glPopMatrix();
}

void	CWeaponRocketLuncher::OnFired()
{
	Vector3f fireCorr = CorrectByHandPos( Vector3f( -0.6f, 0.6f, 0.0f ) );
	auto pos = GenWorldPos( GenPos() + GenShakePos() + fireCorr );
	auto vector = pGLevel->GetCollisionManager().RayCast( Owner->Pos, Owner->Vector, 0.5f );
	vector = (vector - pos).Normalize();
	auto damage = GenDamage();

	CProjectile* pRocket = new CBullRocket( Owner, damage, pos, vector, 30.0f );

	pGLevel->AddEntity( pRocket );
}
