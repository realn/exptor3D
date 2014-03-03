#pragma once

#include "3dMath.h"
#include "glm.h"
#include "GameEntity.h"

#define GAME_THING_PLAYER	0
#define GAME_THING_ENEMY	1
#define GAME_THING_OTHER	2

#define GAME_MOVE_FORWARD	1
#define GAME_MOVE_BACK		2
#define GAME_MOVE_STRAFE_R	4
#define	GAME_MOVE_STRAFE_L	8
#define GAME_DO_FIREWEAPON	16
#define GAME_DO_ATTACK		16

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
	public IEntity,
	public CDynamic,
	public CActorStats
{
protected:
	float StartAngle;
	float Speed;
	float RotSpeed;
	float ToAngle;

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
	CActor();
	~CActor();

	virtual void Init();

	virtual void Update( const float fTD ) override;
	virtual void Render() override;

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
