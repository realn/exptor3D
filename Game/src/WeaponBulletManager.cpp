#include "WeaponBulletManager.h"

#include "gui.h"

CBulletManager BManager;

/*====================
	KLASA weBullManager
	Zarz¹dza wszystkimi pociskami
====================*/
void CBulletManager::AddBullet( CBullet *bullet )
{
	List.push_back( bullet );
}

void CBulletManager::DeleteBullet( unsigned int index )
{
	delete List[index];
	List.erase( List.begin() + index );
}

CBullet* CBulletManager::GetBullet( unsigned int index )
{
	return List[index];
}

float CBulletManager::DoTest( CDynamic* Dum, float Armor )
{
	float Damage = 0.0f;
	CBullet* bull;
	for( int i = List.size()-1; i >= 0; i-- )
	{
		bull = List[i];

		Damage += bull->DoTest( Dum, Armor );

		if( bull->CanDelete )
			DeleteBullet( i );
	}
	return Damage;
}

void CBulletManager::Update( const float fTD )
{
	CBullet* bull;
	for( int i = List.size()-1; i >= 0; i-- )
	{
		bull = List[i];
		bull->Update( fTD );

		if( bull->CanDelete )
			DeleteBullet( i );
	}
}

void CBulletManager::Render()
{
	unsigned int i;
	CBullet* bull;
	for( i = 0; i < List.size(); i++ )
	{
		bull = List[i];
		if( !bull->Visible )
			continue;
		bull->Render();
	}
	if( GUI.GetReflection() )
	{
		glPushMatrix();
		glTranslatef( 0.0f, -10.0f, 0.0f );
		glScalef( 1.0f, -1.0f, 1.0f );
		glDisable( GL_CLIP_PLANE0 );
		glEnable( GL_CLIP_PLANE1 );
		glFrontFace( GL_CW );
		for( i = 0; i < List.size(); i++ )
		{
			bull = List[i];
			if( !bull->Visible )
				continue;
			bull->Render();
		}
		glFrontFace( GL_CCW );
		glDisable( GL_CLIP_PLANE1 );
		glEnable( GL_CLIP_PLANE0 );
		glPopMatrix();
	}
}

void CBulletManager::Clear()
{
	if( List.size() == 0 )
		return;

	for( int i = List.size()-1; i >= 0; i-- )
		DeleteBullet( i );
}

