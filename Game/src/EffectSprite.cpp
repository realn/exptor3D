#include "stdafx.h"
#include "Effect.h"

CEffectSprite::CEffectSprite( const Vector3f& pos, const float R, const float G, const float B ) :
	Pos( pos ),
	Alpha( 1.0f ),
	Speed( 0.4f ),
	Texture( nullptr )
{
	GfxLoaded = false;
	C[0] = R;
	C[1] = G;
	C[2] = B;
}

CEffectSprite::~CEffectSprite()
{
}

const bool	CEffectSprite::IsTransparent() const
{
	return true;
}

const bool	CEffectSprite::LoadGraphic( CTexManager& texManager, CModelManager& modelManager )
{
	Texture = texManager.Get( "Part.tga" );
	GfxLoaded = Texture != nullptr;
	return GfxLoaded;
}

void CEffectSprite::Update( const float fTD )
{
	Alpha -= Speed * fTD;
	if( Alpha <= 0.0f )
		DeleteThis = true;
}

void CEffectSprite::Render()
{
	glPushMatrix();
	glDisable( GL_CULL_FACE );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable( GL_BLEND );

	glTranslatef( Pos.X, Pos.Y, -Pos.Z );
	glRotatef( -180.0f, 0.0f, 1.0f, 0.0f );

	Texture->Activate();
	glBegin( GL_TRIANGLE_STRIP );
	glColor4f( C[0], C[1], C[2], Alpha );
	glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( -0.8f, 0.8f, 0.0f );
	glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( -0.8f, -0.8f, 0.0f );
	glTexCoord2f( 0.0f, 1.0f );
	glVertex3f( 0.8f, 0.8f, 0.0f );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( 0.8f, -0.8f, 0.0f );
	glEnd();

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glDisable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glPopMatrix();
}
