#include "Effect.h"

CEffectExplosion::CEffectExplosion( const Vector3f& pos, const float radius, const float speed ) :
	Pos( pos ),
	MaxRadius( radius ),
	Radius( 0.0f ),
	Speed( speed ),
	Alpha( 1.0f ),
	Obj( nullptr )
{
	Obj = gluNewQuadric();
}

CEffectExplosion::~CEffectExplosion()
{
	if( Obj != nullptr )
	{
		gluDeleteQuadric( Obj );
		Obj = nullptr;
	}
}

const bool	CEffectExplosion::IsTransparent() const
{
	return true;
}

void CEffectExplosion::Update( const float fTD )
{
	if( Radius < MaxRadius )
	{
		Radius += Speed * GUI.GetSpeed() * fTD;

		Alpha = 1.0f - ( Radius / MaxRadius );
	}
	else DeleteThis = true;
}

void CEffectExplosion::Render()
{
	glPushMatrix();

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	glTranslatef( Pos.X, Pos.Y, -Pos.Z );

	glColor4f( 1.0f, 0.0f, 0.0f, Alpha );
	gluSphere( Obj, Radius / 2.0f, 16, 16 );

	glColor4f( 1.0f, 1.0f, 0.2f, Alpha );
	gluSphere( Obj, Radius, 16, 16 );

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glDisable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );

	glPopMatrix();
}
