#pragma once

#include "GameActor.h"
#include "ModelManager.h"

/*	KLASA PRZECIWNIKÓW
	Powstała na bazie CActor i zadaniem
	tego typu postaci jest walka. Posiada
	odpowiednie wartości by precyzyjnie umieścić
	broń oraz swoje imie i nazwe identyfikacyjną.
*/
//class CEnemy : 
//	public CActor
//{
//private:
//	CModelManager& ModelManager;
//
//	std::string		file;
//	std::string		name;
//	std::string		ID;
//	unsigned int	WeapType;
//	unsigned int	RotObj;
//	float	WeapR;
//	float	WeapY;
//	float	WeapAngle;
//	float	FirePause;
//	float	FireTime;
//	bool loaded;
//
//	const std::string GetStr( std::fstream& fileStream );
//public:
//	CEnemy( CModelManager& modelManager ) :
//		ModelManager( modelManager )
//	{	Type = GAME_THING_ENEMY; file = "-";	};
//
//	const bool LoadEnemy( const std::string& filename );
//
//	void Render();
//	void Fire( Vector3f FireTarget );
//
//	void OnDie() override;
//	void OnDead() override;
//	std::string GetID();
//};
//
