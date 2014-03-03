#include "WeaponBulletBomb.h"
#include "WeaponBulletExplosion.h"

#include "gui.h"
#include "WeaponBulletManager.h"

/*====================
	KLASA CBullBomb
	Jest to bomba
====================*/
CBullBomb::CBullBomb( CActor* owner, const float damage, const Vector3f& pos, const float time ) :
	CBullet( owner, damage, pos, Vector3f(), 0.0f )
{
	BoomTime = time;
	ThisTime = 0.0f;
	Model = GLMManager.Get( "Data/bomb-model.glm" );
	Type = BULLET_TYPE_BOMB;
}

float CBullBomb::DoTest(CDynamic *Dum, float Armor )
{
	// Nic...
	return 0.0f;
}

void CBullBomb::Update( const float fTD )
{
	ThisTime += fTD * GUI.GetSpeed();
	if( ThisTime >= BoomTime )
	{
		CanDelete = true;
		CBullExplosion* bull = new CBullExplosion( nullptr, this->Damage, Pos, 60.0f, 10.0f );
		BManager.AddBullet( bull );
	}
}

void CBullBomb::Render()
{
	glPushMatrix();

	glTranslatef( Pos.X, Pos.Y, Pos.Z );
	Model->CallObject( 0 );

	glPopMatrix();
}
