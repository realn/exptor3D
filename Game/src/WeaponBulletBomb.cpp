#include "WeaponBulletBomb.h"
#include "WeaponBulletExplosion.h"
#include "Level.h"

/*====================
	KLASA CBullBomb
	Jest to bomba
====================*/
CBullBomb::CBullBomb( CActor* owner, const float damage, const glm::vec3& pos, const float time, CModelManager& modelManager ) :
	CProjectile( PROJECTILE_TYPE::BOMB, owner, damage, pos, glm::vec3(), 0.0f )
{
	BoomTime = time;
	ThisTime = 0.0f;
	Model = modelManager.Get( "bomb-model.glm" );
}

float CBullBomb::DoTest(CDynamic *Dum, float Armor )
{
	// Nic...
	return 0.0f;
}

void CBullBomb::Update( const float fTD )
{
	ThisTime += fTD;
	if( ThisTime >= BoomTime )
	{
		DeleteThis = true;
		CBullExplosion* bull = new CBullExplosion( nullptr, this->Damage, Pos, 60.0f, 10.0f );
		pGLevel->AddEntity( bull );
	}
}

void CBullBomb::Render()
{
	glPushMatrix();

	glTranslatef( Pos.x, Pos.y, Pos.z );
	Model->RenderObject( 0 );

	glPopMatrix();
}
