#include "WeaponBullet.h"

#include "Level.h"
#include "Special.h"

/*====================
	KLASA CBullet
	zwyk³y pocisk - 
	podstawa do 
	reszty pocisków.
====================*/
CBullet::CBullet(CActor* owner, const float damage, const Vector3f& pos, const Vector3f& vector, const float speed ) :
	CDynamic(0.1f),
	Owner(owner),
	Damage(damage),
	CanDelete( false ),
	DoDelete( false ),
	Visible( true ),
	obj( nullptr )
{
	Pos = pos;
	Vector = vector;
	Veloc = Vector * speed;
	NextPos = Pos;

	Type = BULLET_TYPE_NORMAL;
	obj = gluNewQuadric();
	gluQuadricTexture( obj, GL_FALSE );
	gluQuadricNormals( obj, GL_TRUE );
}

CBullet::~CBullet()
{
	if( obj != nullptr )
	{
		gluDeleteQuadric( obj );
		obj = nullptr;
	}
}

void CBullet::Update( const float fTD )
{
	if( !TestCollBlock( this, pGLevel->GetBlock( this->GetBlockPos() ) ) )
	{
		Pos = NextPos;
		NextPos = Pos + ( Veloc * GUI.GetSpeed() * fTD );
		if( pGLevel->GetBlock( this->GetBlockPos() ) == NULL )
			DoDelete = true;
	}
	else DoDelete = true;

	if( DoDelete )
		OnDelete();
}

float CBullet::DoTest( CDynamic* Dum, float Armor )
{
	if( Dum != NULL && Dum != Owner && TestCollDum( Dum, this ) )
	{
		float ArmorMod = 1.0f - ( ( Armor * 0.5f ) / 100.0f );
		DoDelete = true;
		return Damage * ArmorMod;
	}
	return 0.0f;
}

void CBullet::Render()
{
	glPushMatrix();
	glTranslatef( Pos.X, Pos.Y, Pos.Z );

	glDisable( GL_TEXTURE_2D );

	glColor3f( 0.5f, 0.5f, 0.5f );
	gluSphere( obj, 0.1, 8, 8 );

	glEnable( GL_TEXTURE_2D );

	glPopMatrix();
}

void CBullet::OnDelete()
{
	SEManager.AddEffect( new CEffectSprite( SEManager.GetTexMng(), NextPos, 0.5f, 0.5f, 0.5f ) );
	CanDelete = true;
}
