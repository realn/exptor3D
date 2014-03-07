#include "WeaponBulletRocket.h"
#include "WeaponBulletExplosion.h"

#include "Level.h"
#include "Special.h"
#include "WeaponBulletManager.h"

/*====================
KLASA weBullRocked
Rakieta z wyrzutni
====================*/
CBullRocket::CBullRocket( CActor* owner, const float damage, const Vector3f& pos, const Vector3f& vector, const float speed, CModelManager& modelManager ) :
	CBullet( owner, damage, pos, vector, speed ),
	TexManager( modelManager.GetTexMng() )
{
	Angle = ::GetAngle( Pos, Pos + Vector );
	Sec = 0.0f;
	Radius = 0.3f;
	//glEnable( GL_LIGHT0 );

	Model = modelManager.Get( "Missle.glm" );

	Type = BULLET_TYPE_ROCKET;
}

void CBullRocket::Render()
{
	glPushMatrix();
	glTranslatef( Pos.X, Pos.Y, Pos.Z );
	glRotatef( Angle, 0.0f, 1.0f, 0.0f );

	glRotatef( 0.f, 1.0f, 0.0f ,0.0f );
#ifdef LIGHT_TEST
	float t[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_POSITION, t );
#endif
	Model->CallObject( 0 );

	glPopMatrix();
}

void CBullRocket::Update( const float fTD )
{
	if( !TestCollBlock( this, pGLevel->GetBlock( this->GetBlockPos() ) ) )
	{
		Pos = NextPos;
		NextPos = Pos + ( Veloc * GUI.GetSpeed() * fTD );
		Sec += fTD * GUI.GetSpeed();
		if( Sec > 0.1f )
		{
			Vector3f Tail = Veloc.Normalize();
			Tail = Tail.Reverse();
			Sec = 0.0f;

			SEManager.AddEffect( new CEffectSprite( TexManager, Pos + Tail, 1.0f, 0.7f, 0.0f ) );
		}
		if( pGLevel->GetBlock( this->GetBlockPos() ) == NULL )
			DoDelete = true;
	}
	else DoDelete = true;

	if( DoDelete )
		OnDelete();
}

void CBullRocket::OnDelete()
{
	CanDelete = true;
	BManager.AddBullet( new CBullExplosion( nullptr, this->Damage / 10.0f, Pos, 15.0f, 20.0f ) );
}

