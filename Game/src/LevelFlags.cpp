#include "stdafx.h"
#include "Level.h"
#include "GamePlayer.h"
#include "Weapon.h"

/*	KLASA gameWLFlags
S�u�y do sprawdzania warunk�w wygranej jak i przegranej.
w klasie level'u wyst�puj� dwa takei obiekty.
*/
gameWLFlags::gameWLFlags() :
	flags( 0 ),
	BlockID( 0 ),
	Block( nullptr ),
	//Enemy( nullptr ),
	WeapID( 0 )
{
	Color[0] = 1.0f;
	Color[1] = 1.0f;
	Color[2] = 1.0f;
}

// Sprawdzamy, czy wszyscy nie �yj� ( poza graczem (
bool gameWLFlags::IsAllDead()
{
	//if( ThingManager.GetAllCount() == 0 )
	//	return false;

	//if( ThingManager.GetLifeCount() == 0 )
	//	return true;
	//else return false;

	return false;
}

// Sprawdzamy, czy gracz jest w odpowiednim bloku
bool gameWLFlags::IsInBlock()
{
	//if( pGLevel->GetBlock( MainPlayer.GetBlockPos() ) == Block )
	//	return true;
	//else return false;

	return false;
}

// Sprawdzamy, czy gracz znalaz� odpowiedni� bro�
bool gameWLFlags::GotWeap()
{
	return false;//return MainPlayer.Weapon[WeapID]->GetHave();	
}

// Sprawdzamy, czy gracz nie �yje
bool gameWLFlags::IsSelfDead()
{
	return false;//return MainPlayer.IsDead();
}

// Sprawdzamy, czy okre�lony stw�r nie �yje
bool gameWLFlags::IsThingDead()
{
	//if( Enemy == NULL )
	//	return false;

	//return Enemy->IsDead();
	return false;
}

// Inicjujemy flagi ( znajdujemy potwory, itp ).
void gameWLFlags::VerifyFlags()
{
	if( flags & WLFLAG_GO_TO_BLOCK )
	{
		Block = pGLevel->GetBlock( BlockID );
	}
	if( flags & WLFLAG_THING_DEAD )
	{
		//Enemy = ThingManager.GetEnemyByID( EnemyID );
	}
}

// Metoda sprawdza, czy WSZYSTKIE warunki s� prawdziwe
bool gameWLFlags::CheckAllFlags()
{
	bool status = true;
	if( flags == 0 )
		status = false;

	if( ( flags & WLFLAG_ALL_ENEM_DEAD ) )
		if( !IsAllDead() )
			status = false;
	if( ( flags & WLFLAG_GO_TO_BLOCK ) )
		if( !IsInBlock() )
			status = false;
	if( ( flags & WLFLAG_GET_WEAP ) )
		if( !GotWeap() )
			status = false;
	if( ( flags & WLFLAG_SELF_DEAD ) )
		if( !IsSelfDead() )
			status = false;
	if( ( flags & WLFLAG_THING_DEAD ) )
		if( !IsThingDead() )
			status = false;
	return status;
}

// Metoda sprawdza, czy chocia� jeden warunek jest prawdziwy
bool gameWLFlags::CheckOneFlag()
{
	bool status = false;
	if( ( flags & WLFLAG_ALL_ENEM_DEAD ) )
		if( IsAllDead() )
			status = true;
	if( ( flags & WLFLAG_GO_TO_BLOCK ) )
		if( IsInBlock() )
			status = true;
	if( ( flags & WLFLAG_GET_WEAP ) )
		if( GotWeap() )
			status = true;
	if( ( flags & WLFLAG_SELF_DEAD ) )
		if( IsSelfDead() )
			status = true;
	if( ( flags & WLFLAG_THING_DEAD ) )
		if( IsThingDead() )
			status = true;
	return status;
}

