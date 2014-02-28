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
#ifndef _GAME_H_
#define _GAME_H_

#include "defines.h"
#include "Render.h"
#include "Texture.h"
#include "3dMath.h"
#include "Level.h"
#include "Weapon.h"
#include "glm.h"
#include <vector>

/*	KLASA gameThing
	Jest to klasa wyj�ciowa dla wi�kszo�ci
	obiekt�w "�ywych" -  w tym tak�e dla gracza.
	Posiada odpowiednie metody, by odpowiednio
	zarz�dza� dan� postaci�, oraz gotowe algorytmy
	sztucznej inteligencji.
*/
class gameThing : public Dummy
{
protected:
	GLModelManager& mdlManager;
	gameLevel&		level;

	float Health;
	float MaxHealth;
	float Armor;
	float MaxArmor;
	float Angle;
	float StartAngle;
	float Speed;
	float RotSpeed;
	float DyingTime;
	float ThisDTime;
	bool Dead;
	bool Dying;
	bool HasTarget;
	unsigned int AIflags;
	unsigned int AIState;
	unsigned int Actions;
	unsigned int Type;

	GLModel*	Model;

	Vector3f MoveVector;
	Vector3f StartPos;
	Vector3f Target;

	gameThing* Enemy;

	virtual Vector3f CreatePos( float ang );

	virtual bool AIFindTarget();
	virtual Vector3f AIFindWalkTarget();
	virtual void AIStand();
	virtual void AIWalk();
	virtual void AIAttackTarget();
	virtual bool IsEnemyInFront();
public:
	float ToAngle;
	gameThing(GLModelManager& mdlManager, gameLevel& level);
	~gameThing();

	virtual void Init();

	virtual void DoEngine();
	virtual void DoDraw();

	virtual void OnDie();
	virtual void OnDead();

	virtual void Move( unsigned int flags );
	virtual void DoAI();

	bool IsDead();
	bool IsDying();
	virtual void Reset();

	float GetHealth();
	virtual void ModHealth( float mod );
	virtual void SetHealth( float set );

	float GetArmor();
	virtual void ModArmor( float mod );
	virtual void SetArmor( float set );

	virtual void Fire( Vector3f FireTarget );

	float GetAng();
	void ModAngle( float mod );
	void SetAngle( float set );
	void GoToAngle( float ang );

	void SetStartAngle( float set );
	float GetStartAngle();

	void SetStartPos( Vector3f set );
	Vector3f GetStartPos();
	
	unsigned int GetType();
	virtual void Free();
};


/*	KLASA POSTACI
	Postacie gry maj� swoj� binarn�
	posta� w taki spos�b. Dzi�ki niej
	mo�na �atwo sterowa� i wp�ywa� na ich
	zachowanie.
*/
class gamePlayer : public gameThing
{
private:
	// Posta� biega?
	bool run;

	// "Krok" o jaki porusza si� posta� chodz�c
	float WalkStep;

	// ... i biegaj�c
	float RunStep;
	
	// Aktualna Bro�
	unsigned int CurrWeap;
	unsigned int Hand;
public:
	weWeapon* Weapon[10];

	gamePlayer(GLModelManager& mdlManager);
	~gamePlayer();

	void DoDraw();
	void DoEngine();
	void DoEngine( bool* Keys );
	void SwichWeap( unsigned int index );

	unsigned int GetHand();

	void Move( unsigned int flags );
	void TestWeapon( weWeapon* Weap );
	void TestBonus( weBonus* Bonus );
	void ApplyNextPos();

	void ModHealth( float mod );
	void Reset();
};

/*	KLASA PRZECIWNIK�W
	Powsta�a na bazie gameThing i zadaniem
	tego typu postaci jest walka. Posiada
	odpowiednie warto�ci by precyzyjnie umie�ci�
	bro� oraz swoje imie i nazwe identyfikacyjn�.
*/
class gameEnemy : public gameThing
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

	std::string GetStr( FILE* fp );
public:
	gameEnemy(GLModelManager& mdlManager) :
		gameThing(mdlManager)
	{	Type = GAME_THING_ENEMY; file = "-";	};

	bool LoadEnemy( std::string filename );

	void DoDraw();
	void Fire( Vector3f FireTarget );

	void OnDie();
	void OnDead();
	std::string GetID();
};

/*	KLASA gameStatObj
*/
class gameStatObj : public gameThing
{
private:
	std::string		file;

	std::string GetStr( FILE* fp );
public:
	gameStatObj(GLModelManager& mdlManager) :
		gameThing(mdlManager)
	{	file = "-";	};

	void DoEngine();
	void DoDraw();

	bool LoadObj( std::string filename );
};


/*	KLASA gameThingManager
	Jest to klasa zarz�daj�ca wszystkimi
	obiektami na scenie ( poza broni�, amunicj� i bonusami )
*/
class gameThingManager
{
private:
	std::vector<gameThing*> List;
	unsigned int dead;
	unsigned int life;
	unsigned int all;
public:
	gameThingManager();
	~gameThingManager();

	void DoEngine();
	void DoDraw();

	void ReCountStats();
	void ResetAll();

	void AddThing( gameThing* Thing );
	void DeleteThing( unsigned int index );
	gameThing* GetThing( unsigned int index );
	gameEnemy* GetEnemyByID( std::string ID );
	Vector3f GetThingPos( unsigned int index );
	Vector3f GetThingBlockPos( unsigned int index );
	gameBlockInfo* GetThingBlock( unsigned int index );

	unsigned int Count();
	void Clear();

	unsigned int GetDeadCount();
	unsigned int GetLifeCount();
	unsigned int GetAllCount();
};

/*	KLASA gameWeaponManager
	S�u�y do zarz�dzania broni� le��c� na ziemi
	na poziomie. z tej klasy g��wnie korzysta gracz.
	( i nikt poza nim :) )
*/
class gameWeaponManager
{
private:
	std::vector <weWeapon*> List;
public:
	gameWeaponManager();
	~gameWeaponManager();
	void DoEngine( gamePlayer* Players, int PlayerCount );
	void DoDraw();

	void AddWeapon( weWeapon* weapon );
	void DeleteWeapon( unsigned int index );
	weWeapon* GetWeapon( unsigned int index );
	void Clear();
	void LoadFromLevel();
};

/*
class gameStatObj : public Dummy
{
private:
	float rot;
	GLModel* Model;
public:
	gameStatObj();
	~gameStatObj();

	void SetRot( float set );
	float GetRot();

	void LoadFromFile( std::string filename );
	void DoDraw();
};
*/
extern gameThingManager ThingManager;
extern gameWeaponManager WManager;

#endif