/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Game.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klas obiekt�w poruszaj�cych si� 
		po poziomie ( w tym gracza ) oraz obiekt�w do
		zarz�dzania gr�.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include "Texture.h"
#include "3dMath.h"
#include "Level.h"
#include "Weapon.h"
#include <vector>

#include "GameActor.h"
#include "GameEnemy.h"

class gameBlockInfo;

/*	KLASA CActorManager
	Jest to klasa zarz�daj�ca wszystkimi
	obiektami na scenie ( poza broni�, amunicj� i bonusami )
*/
class CActorManager
{
private:
	std::vector<CActor*> List;
	unsigned int dead;
	unsigned int life;
	unsigned int all;
public:
	CActorManager();
	~CActorManager();

	void Update( const float fTD );
	void Render();

	void ReCountStats();
	void ResetAll();

	void AddThing( CActor* Thing );
	void DeleteThing( unsigned int index );
	CActor* GetThing( unsigned int index );
	CEnemy* GetEnemyByID( std::string ID );
	Vector3f GetThingPos( unsigned int index );
	Vector3f GetThingBlockPos( unsigned int index );
	gameBlockInfo* GetThingBlock( unsigned int index );

	unsigned int Count();
	void Clear();

	unsigned int GetDeadCount();
	unsigned int GetLifeCount();
	unsigned int GetAllCount();
};

extern CActorManager ThingManager;
