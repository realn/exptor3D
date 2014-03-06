#include "GameEnemy.h"

#include "Weapon.h"
#include "Log.h"
#include "gui.h"

#include "WeaponBulletManager.h"
#include "WeaponBulletRay.h"
#include "WeaponBulletRocket.h"

#include "Special.h"

///*=========================
//	KLASA CEnemy
//
//
//=========================*/
//const std::string CEnemy::GetStr( std::fstream& fileStream )
//{
//	const unsigned BUFFER_SIZE = 256;
//	char buf[BUFFER_SIZE + 1];
//	memset(buf, 0, sizeof(char) * (BUFFER_SIZE + 1));
//
//	fileStream.getline(buf, BUFFER_SIZE);
//
//	return  buf;
//}
//
//const bool CEnemy::LoadEnemy( const std::string& filename )
//{
//	// Sprawdzamy, czy argument nie jest pusty
//	if( filename.empty() )
//	{
//		Log.Error( "ENEMY( " + file + " ): Ci¹g znaków jest pusty" );
//		return false;
//	}
//
//	// Otwieramy plik
//	std::fstream fileStream(filename, std::ios::in);
//
//	// zmienna pomocnicza
//	std::string str;
//
//	// Sprawdzamy czy otwarcie siê uda³o
//	if( !fileStream )
//	{
//		Log.Error( "ENEMY( " + file + " ): Niew³aœciwa œcie¿ka, lub plik nie istnieje" );
//		return false;
//	}
//
//	if( loaded )
//	{
//		Log.Log( "ENEMY( " + file + " ): Prze³adowanie na: " + filename );
//		Model = NULL;
//	}
//
//	// Przypisujemy nazwê pliku
//	file = filename;
//
//	// przypisujemy identyfikator
//	str = GetStr( fileStream );
//	ID = str;
//
//	// przypisujemy nazwê wroga
//	str = GetStr( fileStream );
//	name = str;
//
//	// przypisujemy, która czêœæ modelu ma siê obracaæ
//	str = GetStr( fileStream );
//	RotObj = atoi( str.c_str() );
//
//	// przypisujemy promieñ kolizyjny
//	str = GetStr( fileStream );
//	Radius = atof( str.c_str() );
//	
//	// przypisujemy typ broni
//	str = GetStr( fileStream );
//	WeapType = atoi( str.c_str() );
//
//	// przypisujemy korekcje pozycji broni
//	str = GetStr( fileStream );
//	WeapR = atof( str.c_str() );
//	str = GetStr( fileStream );
//	WeapY = atof( str.c_str() );
//	str = GetStr( fileStream );
//	WeapAngle = atof( str.c_str() );
//
//	// przypisujemy statystyki
//	str = GetStr( fileStream );
//	Health = atof( str.c_str() );
//	str = GetStr( fileStream );
//	Armor = atof( str.c_str() );
//
//	// przypisujemy AI
//	str = GetStr( fileStream );
//	AIflags = atoi( str.c_str() );
//
//	// przypisujemy model
//	str = GetStr( fileStream );
//	Model = ModelManager.Get( str );
//
//	str = GetStr( fileStream );
//	FireTime = FirePause = atof( str.c_str() );
//
//	loaded = true;
//
//	return true;
//}
//
//void CEnemy::Render()
//{
//	if( IsDead() )
//		return;
//
//	unsigned int i;
//	glPushMatrix();
//	if( IsDying() )
//	{
//		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//		glEnable( GL_BLEND );
//		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f - ( DyingTime * ( 1.0f / TimeUntilDead ) ) );
//	}
//
//	glTranslatef( Pos.X, Pos.Y, Pos.Z );
//
//	for( i = 0; i < Model->GetObjCount(); i++ )
//	{
//		if( i == RotObj )
//		{
//			glPushMatrix();
//			glRotatef( Angle, 0.0f, 1.0f, 0.0f );
//		}
//
//		Model->CallObject( i );
//
//		if( i == RotObj )
//			glPopMatrix();
//	}
//
//	if( GUI.GetReflection() )
//	{
//		glPushMatrix();
//		glTranslatef( 0.0f, -10.0f, 0.0f );
//		glScalef( 1.0f, -1.0f, 1.0f );
//		glDisable( GL_CLIP_PLANE0 );
//		glEnable( GL_CLIP_PLANE1 );
//
//		for( i = 0; i < Model->GetObjCount(); i++ )
//		{
//			if( i == RotObj )
//			{
//				glPushMatrix();
//				glRotatef( Angle, 0.0f, 1.0f, 0.0f );
//			}
//
//			Model->CallObject( i );
//
//			if( i == RotObj )
//				glPopMatrix();
//		}
//
//		glDisable( GL_CLIP_PLANE1 );
//		glEnable( GL_CLIP_PLANE0 );
//		glPopMatrix();
//	}
//	if( IsDying() )
//	{
//		glDisable( GL_BLEND );
//		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
//	}
//
//
//	glPopMatrix();
//
//	if( !GUI.GetShowAITarget() )
//		return;
//
//	GLUquadric* obj = gluNewQuadric();
//	glPushMatrix();
//	glTranslatef( Target.X, Target.Y, Target.Z );
//
//	gluSphere( obj, 0.5f, 32, 32 );
//
//	glPopMatrix();
//	gluDeleteQuadric( obj );
//}
//
//void CEnemy::Fire( Vector3f FireTarget )
//{
//	if( IsDying() )
//		return;
//
//	FireTime += 1.0f * GUI.GetSpeed();
//
//	if( FireTime < FirePause )
//		return;
//	else FireTime = 0.0f;
//
//	Vector3f temp = Pos;
//	Vector3f add;
//	Vector3f FireVeloc;
//
//	add.X = sinf( ( Angle + WeapAngle ) * PIOVER180 ) * WeapR;
//	add.Z = -cosf( ( Angle + WeapAngle ) * PIOVER180 ) * WeapR;
//	add.Y = WeapY;
//
//	temp += add;
//
//	FireVeloc = ( FireTarget - temp ).Normalize();
//
//	switch( WeapType )
//	{
//	case WEAPON_TYPE::PISTOL :
//		{
//			CBullet* Bull = new CBullet( this, 1.0f, Pos, FireVeloc, 12.0f );
//			BManager.AddBullet( Bull );
//			return;
//		}
//	case WEAPON_TYPE::MINIPHAZER :
//	case WEAPON_TYPE::PHAZER :
//		{
//			CBullet* Bull = new CBullRay( this, 50.0f, temp, FireVeloc );
//			BManager.AddBullet( Bull );
//			return;
//		}
//	case WEAPON_TYPE::ROCKET_LUNCHER :
//		{
//			CBullet* Bull = new CBullRocket( this, 10.0f, temp, FireVeloc, 2.0f, ModelManager );
//			BManager.AddBullet( Bull );
//			return;
//		}
//	}
//}
//
//void CEnemy::OnDie()
//{
//	SEManager.AddEffect( new CEffectExplosion( Pos, 5.0f, 0.1f ) );
//}
//
//void CEnemy::OnDead()
//{
//	GUI.PInfo.FRAGS++;
//}
//
//std::string CEnemy::GetID()
//{
//	return ID;
//}
