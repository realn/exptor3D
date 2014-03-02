/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Game.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klas obiektów poruszaj¹cych siê 
		po poziomie ( w tym gracza ) oraz obiektów do
		zarz¹dzania gr¹.

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
#include <vector>

enum class ACTOR_STATE
{
	ALIVE = 0,
	DISABLED,
	DYING,
	DEAD,
};

class CActorStats
{
protected:
	float Health;
	float MaxHealth;
	float Armor;
	float MaxArmor;
	ACTOR_STATE	State;
	float TimeUntilDead;
	float DyingTime;

public:
	CActorStats();
	CActorStats(const float health, const float armor);
	virtual ~CActorStats(){}

	virtual void OnDie() = 0;
	virtual void OnDead() = 0;

	void	UpdateStats(const float fTD);

	const bool IsDead() const;
	const bool IsDying() const;
	virtual void Reset();
	const ACTOR_STATE	GetState() const;

	const float GetHealth() const;
	virtual void ModHealth( const float mod );
	virtual void SetHealth( const float set );

	const float GetArmor() const;
	virtual void ModArmor( const float mod );
	virtual void SetArmor( const float set );
};

class IUpdateable
{
public:
	virtual void Update(const float fTD) = 0;
};

class IRenderable
{
public:
	virtual void Render() = 0;
};

/*	KLASA CActor
	Jest to klasa wyjœciowa dla wiêkszoœci
	obiektów "¿ywych" -  w tym tak¿e dla gracza.
	Posiada odpowiednie metody, by odpowiednio
	zarz¹dzaæ dan¹ postaci¹, oraz gotowe algorytmy
	sztucznej inteligencji.
*/
class CActor : 
	public CEntity,
	public CActorStats,
	public IUpdateable
{
protected:
	float StartAngle;
	float Speed;
	float RotSpeed;
	bool HasTarget;
	unsigned int AIflags;
	unsigned int AIState;
	unsigned int Actions;
	unsigned int Type;

	GLModel*	Model;

	Vector3f StartPos;
	Vector3f Target;

	CActor* Enemy;

	virtual Vector3f CreatePos( float ang );

	virtual bool AIFindTarget();
	virtual Vector3f AIFindWalkTarget();
	virtual void AIStand();
	virtual void AIWalk();
	virtual void AIAttackTarget();
	virtual bool IsEnemyInFront();
public:
	float ToAngle;
	CActor();
	~CActor();

	virtual void Init();

	virtual void Update( const float fTD ) override;
	virtual void Render();

	virtual void Reset() override;

	virtual void Move( unsigned int flags, const float fTD );
	virtual void DoAI();

	virtual void Fire( Vector3f FireTarget );

	float GetAng();
	void ModAngle( float mod );
	void SetAngle( const float set ) override;
	void GoToAngle( float ang );

	void SetStartAngle( float set );
	float GetStartAngle();

	void SetStartPos( Vector3f set );
	Vector3f GetStartPos();
	
	unsigned int GetType();
	virtual void Free();
};


/*	KLASA POSTACI
	Postacie gry maj¹ swoj¹ binarn¹
	postaæ w taki sposób. Dziêki niej
	mo¿na ³atwo sterowaæ i wp³ywaæ na ich
	zachowanie.
*/
class CPlayer : 
	public CActor
{
private:
	// Postaæ biega?
	bool run;

	// "Krok" o jaki porusza siê postaæ chodz¹c
	float WalkStep;

	// ... i biegaj¹c
	float RunStep;
	
	// Aktualna Broñ
	unsigned int CurrWeap;
	unsigned int Hand;
public:
	weWeapon* Weapon[10];

	CPlayer();
	~CPlayer();

	void	OnDie() override;
	void	OnDead() override;

	void Render();
	void Update( const float fTD ) override;
	void Update( bool* Keys, const float fTD );

	void Move(unsigned uFlags, const float fTD ) override;

	void SwichWeap( unsigned int index );

	unsigned int GetHand();

	void TestWeapon( weWeapon* Weap );
	void TestBonus( weBonus* Bonus );
	void ApplyNextPos();

	void ModHealth( const float mod ) override;
	void Reset() override;
};

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

/*	KLASA gameStatObj
*/
class gameStatObj : 
	public CObject
{
private:
	GLModel*	Model;
	std::string	file;

	std::string GetStr( FILE* fp );
public:
	gameStatObj()
	{	file = "-";	};

	void Update( const float fTD );
	void Render();

	bool LoadObj( std::string filename );
};


/*	KLASA CActorManager
	Jest to klasa zarz¹daj¹ca wszystkimi
	obiektami na scenie ( poza broni¹, amunicj¹ i bonusami )
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

/*	KLASA gameWeaponManager
	S³u¿y do zarz¹dzania broni¹ le¿¹c¹ na ziemi
	na poziomie. z tej klasy g³ównie korzysta gracz.
	( i nikt poza nim :) )
*/
class gameWeaponManager
{
private:
	std::vector <weWeapon*> List;
public:
	gameWeaponManager();
	~gameWeaponManager();
	void Update( CPlayer* Players, int PlayerCount );
	void Render();

	void AddWeapon( weWeapon* weapon );
	void DeleteWeapon( unsigned int index );
	weWeapon* GetWeapon( unsigned int index );
	void Clear();
	void LoadFromLevel();
};

/*
class gameStatObj : public CEntity
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
	void Render();
};
*/
extern CActorManager ThingManager;
extern gameWeaponManager WManager;
extern CPlayer MainPlayer;

#endif