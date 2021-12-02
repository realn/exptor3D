#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	Pos = glm::vec3( 1.5f, -1.5f, 3.5f );

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

	auto mat = glm::mat4(1.0f);
	

	mat = glm::translate(mat, glm::vec3(pos.x, pos.y, -(pos.z - Zcorr)));
	mat = glm::rotate(mat, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glLoadMatrixf(glm::value_ptr(mat));

	Model->render( 0 );

	// Przywracamy Macierz
	glPopMatrix();
}

void	CWeaponRocketLuncher::OnFired()
{
	auto fireCorr = CorrectByHandPos( glm::vec3( -0.6f, 0.6f, 0.0f ) );
	auto pos = GenWorldPos( GenPos() + GenShakePos() + fireCorr );
	auto vector = pGLevel->GetCollisionManager().RayCast( Owner->Pos, Owner->Vector, 0.5f );
	vector = glm::normalize(vector - pos);
	auto damage = GenDamage();

	CProjectile* pRocket = new CBullRocket( Owner, damage, pos, vector, 30.0f );

	pGLevel->AddEntity( pRocket );
}
