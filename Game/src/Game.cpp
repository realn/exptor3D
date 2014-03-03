/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Game.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Game.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Game.h"

#include "GamePlayer.h"

CActorManager::CActorManager()
{
	
}

CActorManager::~CActorManager()
{
	Clear();
}

void CActorManager::AddThing( CActor* Thing )
{
	List.push_back( Thing );
}

void CActorManager::DeleteThing( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

CActor* CActorManager::GetThing( unsigned int index )
{
	if( index >= List.size() )
		return NULL;

	return List[index];
}

CEnemy* CActorManager::GetEnemyByID( std::string ID )
{
	CEnemy* Enemy;
	for( unsigned int i = 0; i < List.size(); i++ )
	{
		if( GetThing( i )->GetType() != GAME_THING_ENEMY )
			continue;

		Enemy = (CEnemy*)GetThing( i );
		if( Enemy->GetID() == ID )
			return Enemy;
	}
	return NULL;
}

Vector3f CActorManager::GetThingPos( unsigned int index )
{
	if( index >= List.size() )
		return Vector3f( 0.0f, 0.0f, 0.0f );

	return List[index]->NextPos;	
}

Vector3f CActorManager::GetThingBlockPos( unsigned int index )
{
	if( index >= List.size() )
		return Vector3f( 0.0f, 0.0f, 0.0f );

	return List[index]->GetBlockPos();	
}

gameBlockInfo* CActorManager::GetThingBlock( unsigned int index )
{
	if( index >= List.size() )
		return NULL;

	return GLevel.GetBlock( List[index]->GetBlockPos() );
}

void CActorManager::Update( const float fTD )
{
	CActor* Thing;
	this->all = List.size();
	this->dead = 0;
	this->life = 0;
	unsigned int i = 0;

	for( i = 0; i < List.size(); i++ )
	{
		Thing = GetThing( i );

		if( Thing->IsDead() )
		{
			this->dead++;
			continue;
		}

		if( mathDistSq( MainPlayer.Pos, Thing->Pos ) > POW(100.0f) )
			continue;

		Thing->ModHealth( -BManager.DoTest( Thing, Thing->GetArmor() ) );

		Thing->Update( fTD );

		if( Thing->GetType() != GAME_THING_PLAYER )
			this->life++;
		else this->all--;
	}
}

void CActorManager::Render()
{
	CActor* Thing;
	unsigned int i;

	for( i = 0; i < List.size(); i++ )
	{
		Thing = GetThing( i );

		if( mathDistSq( MainPlayer.Pos, Thing->Pos ) > POW(100.0f) )
			continue;

		Thing->Render();
	}
}

void CActorManager::ResetAll()
{
	unsigned int i;
	CActor* Thing;

	for( i = 0; i < List.size(); i++ )
	{
		Thing = GetThing( i );
		Thing->Reset();
	}
}

void CActorManager::ReCountStats()
{
	this->all = List.size();
	this->dead = 0;
	this->life = 0;
	CActor* Thing;
	unsigned int i;

	for( i = 0; i < List.size(); i++ )
	{
		Thing = GetThing( i );
		if( Thing->IsDead() )
			this->dead++;
		else if( Thing->GetType() != GAME_THING_PLAYER )
			this->life++;
		else this->all--;
	}
}

unsigned int CActorManager::Count()
{
	return List.size();
}

unsigned int CActorManager::GetAllCount()
{
	return all;
}

unsigned int CActorManager::GetLifeCount()
{
	return life;
}

unsigned int CActorManager::GetDeadCount()
{
	return dead;
}

void CActorManager::Clear()
{
	int i;

	for( i = List.size()-1; i >= 0; i-- )
	{
		if( GetThing( i )->GetType() == GAME_THING_PLAYER )
			continue;

		DeleteThing( i );
	}
}

CActorManager ThingManager;

/*=========================	
	KLASA gameWeaponManager


=========================*/
gameWeaponManager::gameWeaponManager()
{

}

gameWeaponManager::~gameWeaponManager()
{
	Clear();
}

void gameWeaponManager::Update( CPlayer* Players, int PlayerCount, const float fTD )
{
	unsigned int i;
	int j;
	for( i = 0; i < List.size(); i++ )
	{
		List[i]->Update( fTD );
	}
	for( i = 0; i < PlayerCount; i++ )
	{
		for( j = List.size()-1; j >= 0; j-- )
		{
			Players[i].TestWeapon( List[j] );
			if( List[j]->GetAllAmmo() == 0 )
			{
				DeleteWeapon( j );
			}
		}
	}
}

void gameWeaponManager::Render()
{
	unsigned int i;
	for( i = 0; i < List.size(); i++ )
	{
		List[i]->Render();
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
			List[i]->Render();
		}
		glFrontFace( GL_CCW );
		glDisable( GL_CLIP_PLANE1 );
		glEnable( GL_CLIP_PLANE0 );
		glPopMatrix();
	}
}

void gameWeaponManager::AddWeapon( weWeapon* weapon )
{
	if( weapon == NULL )
		return;

	if( !weapon->GetInited() )
		weapon->Init();

	List.push_back( weapon );
}

void gameWeaponManager::DeleteWeapon( unsigned int index )
{
	if( index >= List.size() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

weWeapon* gameWeaponManager::GetWeapon( unsigned int index )
{
	if( index >= List.size() )
		return NULL;

	return List[index];
}

void gameWeaponManager::Clear()
{
	for( unsigned int i = 0; i < List.size(); i++ )
	{
		delete List[i];
	}
	List.clear();
}

void gameWeaponManager::LoadFromLevel()
{
}

gameWeaponManager WManager;
