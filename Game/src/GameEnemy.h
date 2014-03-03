#pragma once

#include "GameActor.h"

/*	KLASA PRZECIWNIKÓW
	Powsta³a na bazie CActor i zadaniem
	tego typu postaci jest walka. Posiada
	odpowiednie wartoœci by precyzyjnie umieœciæ
	broñ oraz swoje imie i nazwe identyfikacyjn¹.
*/
class CEnemy : 
	public CActor
{
private:
	std::string		file;
	std::string		name;
	std::string		ID;
	unsigned int	WeapType;
	unsigned int	RotObj;
	float	WeapR;
	float	WeapY;
	float	WeapAngle;
	float	FirePause;
	float	FireTime;
	bool loaded;

	const std::string GetStr( std::fstream& fileStream );
public:
	CEnemy()
	{	Type = GAME_THING_ENEMY; file = "-";	};

	const bool LoadEnemy( const std::string filename );

	void Render();
	void Fire( Vector3f FireTarget );

	void OnDie() override;
	void OnDead() override;
	std::string GetID();
};

