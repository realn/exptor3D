#pragma once

#include "3dMath.h"
#include "Updateable.h"
#include "SceneEntity.h"

enum class ACTOR_TYPE
{
	ACTOR_UNKNOWN	= 0,
	ACTOR_PLAYER	= 1,
	ACTOR_ENEMY		= 2,
};

enum class GAME_ACTION
{
	MOVE_FORWARD		= 0x01,
	MOVE_BACK			= 0x02,
	MOVE_STRAFE_LEFT	= 0x04,
	MOVE_STRAFE_RIGHT	= 0x08,
	DO_ATTACK			= 0x10,
};

#define	AI_NO_AI			0
#define AI_SIMPLE_AI		1
#define AI_PLAYER_ENEMY		2
#define AI_PLAYER_HELP		4
#define	AI_ATTACK_FAR		8
#define	AI_ATTACK_CLOSE		16
#define	AI_MOVE_AROUND		32
#define	AI_STAY_ON_ATTACK	64
#define	AI_MOVE_ON_ATTACK	128
#define AI_SEARCH_ENEMY		256
#define	AI_FIRE_AT_WILL		512
#define	AI_FIRE_IF_ATTACKED	1024
#define AI_TAKE_ITEMS		2048
#define	AI_DONT_TAKE_ITEMS	4096
#define	AI_ALLWAYS_WALK		8192
#define	AI_ALLWAYS_RUN		16384

#define AI_STATE_NOTHING	0
#define AI_STATE_STAND		1
#define AI_STATE_WALK		2
#define	AI_STATE_FIRE		3
#define	AI_STATE_WALKFIRE	4
#define	AI_STATE_STANDFIRE	5

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

/*	KLASA CActor
	Jest to klasa wyjœciowa dla wiêkszoœci
	obiektów "¿ywych" -  w tym tak¿e dla gracza.
	Posiada odpowiednie metody, by odpowiednio
	zarz¹dzaæ dan¹ postaci¹, oraz gotowe algorytmy
	sztucznej inteligencji.
*/
class CActor : 
	public CDynamic,
	public CActorStats,
	public IUpdateable,
	public ISceneEntity
{
protected:
	const ACTOR_TYPE	Type;

	float StartAngle;
	float Speed;

	unsigned int Actions;

	glm::vec3 StartPos;

	//virtual bool AIFindTarget();
	//virtual glm::vec3 AIFindWalkTarget();
	//virtual void AIStand();
	//virtual void AIWalk();
	//virtual void AIAttackTarget();
	//virtual bool IsEnemyInFront();
public:
	CActor( const ACTOR_TYPE type );
	virtual ~CActor();

	const ACTOR_TYPE GetType() const;

	virtual	void	DoAction( const GAME_ACTION action );
	virtual void	DoRotate( const float angle );

	virtual void Update( const float fTD ) override;

	virtual void Reset() override;

	const float GetAngle() const;
	void ModAngle( float mod );
	void SetAngle( const float set ) override;

	void SetStartAngle( const float set );
	const float GetStartAngle() const;

	void SetStartPos( const glm::vec3& set );
	const glm::vec3 GetStartPos() const;
	
protected:
	virtual void	SolveActions( const float fTD );
};
