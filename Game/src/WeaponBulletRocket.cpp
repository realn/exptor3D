#include "WeaponBulletRocket.h"
#include "WeaponBulletExplosion.h"

#include "Level.h"
#include "Special.h"

/*====================
KLASA weBullRocked
Rakieta z wyrzutni
====================*/
CBullRocket::CBullRocket( CActor* owner, const float damage, const Vector3f& pos, const Vector3f& vector, const float speed ) :
	CProjectile(PROJECTILE_TYPE::ROCKET, owner, damage, pos, vector, speed )
{
	GfxLoaded = false;
	Angle = ::GetAngle( Pos, Pos + Vector );
	Sec = 0.0f;
	Radius = 0.3f;
	//glEnable( GL_LIGHT0 );
}

const bool	CBullRocket::LoadGraphic( CTexManager& texManager, CModelManager& modelManager )
{
	Model = modelManager.Get( "Missle.glm" );
	GfxLoaded = Model != nullptr;

	return true;
}

void CBullRocket::Render()
{
	glPushMatrix();
	glTranslatef( Pos.X, Pos.Y, -Pos.Z );
	glRotatef( Angle, 0.0f, 1.0f, 0.0f );

	glRotatef( 180.f, 1.0f, 0.0f ,0.0f );
#ifdef LIGHT_TEST
	float t[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_POSITION, t );
#endif
	Model->RenderObject( 0 );

	glPopMatrix();
}

void CBullRocket::Update( const float fTD )
{
	Pos = NextPos;
	NextPos = Pos + ( Veloc * fTD );

	Sec += fTD;
	if( Sec > 0.1f )
	{
		Vector3f Tail = Veloc.Normalize();
		Tail = Tail.Reverse();
		Sec = 0.0f;

		pGLevel->AddEntity( new CEffectSprite( Pos + Tail, 1.0f, 0.7f, 0.0f ) );
	}
}

void CBullRocket::OnDelete()
{
	pGLevel->AddEntity( new CBullExplosion( nullptr, this->Damage / 10.0f, Pos, 15.0f, 20.0f ) );
}

const bool	CBullRocket::OnCollision( const Vector3f& point, const Planef& plane )
{
	DeleteThis = true;
	return true;
}