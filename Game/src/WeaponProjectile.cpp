#include "WeaponProjectile.h"

#include "Level.h"
#include "Special.h"

/*====================
	KLASA CProjectile
	zwyk³y pocisk - 
	podstawa do 
	reszty pocisków.
====================*/
CProjectile::CProjectile(const PROJECTILE_TYPE type, CActor* owner, const float damage, const Vector3f& origin, const Vector3f& vector, const float speed ) :
	Type( type ),
	CDynamic(0.1f),
	Owner(owner),
	Damage(damage),
	obj( nullptr )
{
	Pos = origin;
	Vector = vector;
	Veloc = Vector * speed;
	NextPos = Pos;

	obj = gluNewQuadric();
	gluQuadricTexture( obj, GL_FALSE );
	gluQuadricNormals( obj, GL_TRUE );
}

CProjectile::~CProjectile()
{
	if( obj != nullptr )
	{
		gluDeleteQuadric( obj );
		obj = nullptr;
	}
}

void CProjectile::Update( const float fTD )
{
	//if( !TestCollBlock( this, pGLevel->GetBlock( this->GetBlockPos() ) ) )
	//{
	//	Pos = NextPos;
	//	NextPos = Pos + ( Veloc * GUI.GetSpeed() * fTD );
	//	if( pGLevel->GetBlock( this->GetBlockPos() ) == NULL )
	//		DeleteThis = true;
	//}
}

float CProjectile::DoTest( CDynamic* Dum, float Armor )
{
	//if( Dum != NULL && Dum != Owner && TestCollDum( Dum, this ) )
	//{
	//	float ArmorMod = 1.0f - ( ( Armor * 0.5f ) / 100.0f );
	//	DeleteThis = true;
	//	return Damage * ArmorMod;
	//}
	return 0.0f;
}

void CProjectile::Render()
{
	glPushMatrix();
	glTranslatef( Pos.X, Pos.Y, Pos.Z );

	glDisable( GL_TEXTURE_2D );

	glColor3f( 0.5f, 0.5f, 0.5f );
	gluSphere( obj, 0.1, 8, 8 );

	glEnable( GL_TEXTURE_2D );

	glPopMatrix();
}

void CProjectile::OnDelete()
{
	//SEManager.AddEffect( new CEffectSprite( SEManager.GetTexMng(), NextPos, 0.5f, 0.5f, 0.5f ) );
}
