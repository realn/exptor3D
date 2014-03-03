#include "GameActor.h"
#include "gui.h"
#include "Level.h"
#include "GamePlayer.h"

/*=========================	
	KLASA CActor


=========================*/
CActor::CActor() : 
	CEntity(2.0f),
	CActorStats(100.0f, 100.0f),
	Model(0),
	Enemy(0)
{
	Armor = 0.0f;

	AIflags = AI_MOVE_AROUND;
	Speed = 0.1f;
	RotSpeed = 1.0f;
	Angle = 180.0f;
	StartAngle = 180.0f;
	ToAngle = 180.0f;
	Pos.Set( 5.0f, 0.0f, -25.0f );
	Type = GAME_THING_OTHER;
	AIState = AI_STATE_NOTHING;
}

CActor::~CActor()
{
	Free();
	Model = NULL;
}

void CActor::Init()
{
}

Vector3f CActor::CreatePos( float ang )
{
	Vector3f zwrot;

	zwrot.X = sinf( ang * PIOVER180 );
	zwrot.Z = -cosf( ang * PIOVER180 );

	return zwrot;
}

void CActor::Move( unsigned int flags, const float fTD )
{
	Vector3f temp;

	if( flags & GAME_MOVE_FORWARD )
		temp += CreatePos( Angle );

	if( flags & GAME_MOVE_BACK )
		temp += CreatePos( Angle - 180 );

	if( flags & GAME_MOVE_STRAFE_L )
		temp += CreatePos( Angle - 90 );

	if( flags & GAME_MOVE_STRAFE_R )
		temp += CreatePos( Angle + 90 );

	if(temp.X == 0.0f && temp.Y == 0.0f && temp.Z == 0.0f)
		NextPos = Pos;
	else
	{
		temp.Normalize();
		NextPos = Pos + temp * Speed * GUI.GetSpeed() * fTD;
	}

	Vector = CreatePos( Angle );
	Vector.Normalize();
}

void CActor::Update( const float fTD )
{
	UpdateStats( fTD );

	if( State == ACTOR_STATE::DEAD )
		return;

	DoAI();

	Move( Actions, fTD );

	Actions = 0;

	TestCollBlock( this, GLevel.GetBlock( this->GetBlockPos() ), true );

	Pos = NextPos;
}

void CActor::Render()
{
	// Dla niezdefiniowanych obiektów to pole pozostaje puste
}

bool CActor::IsEnemyInFront()
{
	if( Enemy == NULL )
		return false;

	if( fabsf( ::GetAngle( Enemy->NextPos, Pos )- Angle )  < 3.0f )
		return true;

	return false;
}

void CActor::DoAI()
{
	if( this->AIflags == AI_NO_AI )
		return;

	if( this->AIflags & AI_PLAYER_ENEMY )
	{
		if( this->AIFindTarget() )
		{
			HasTarget = true;
			Target = Enemy->NextPos;

			if( mathDist( Pos, Target ) < 70.0f )
			{
				if( this->AIflags & AI_ATTACK_CLOSE )
				{
					if( mathDist( Pos, Target ) > 5.0f )
						this->AIWalk();
					else this->AIAttackTarget();
				}
				else if( this->AIflags & AI_ATTACK_FAR )
					this->AIAttackTarget();

				return;
			}
		}
	}

	if( this->AIflags & AI_MOVE_AROUND )
	{
		if( this->AIState == AI_STATE_STAND )
		{
			Target = this->AIFindWalkTarget();
			HasTarget = true;

			this->AIWalk();
		}
		else if( this->AIState == AI_STATE_WALK )
			this->AIWalk();
	}

	if( AIState == AI_STATE_NOTHING )
		AIStand();
}

bool CActor::AIFindTarget()
{
	if( this->AIflags & AI_PLAYER_ENEMY )
	{
		Enemy = &MainPlayer;

		if( !IsCollOnRay( Enemy->NextPos, Pos ) )
			return true;
		else 
		{
			if( AIState == AI_STATE_FIRE )
				AIState = AI_STATE_NOTHING;
			Enemy = NULL;
			return false;
		}
	}

	if( ThingManager.Count() == 0 )
		return false;

	unsigned int i = 0;
	for( i = 0; i < ThingManager.Count(); i++ )
	{
		Enemy = ThingManager.GetThing( i );

		if( !IsCollOnRay( Enemy->NextPos, Pos ) )
			return true;
	}

	Enemy = NULL;
	return false;
}

Vector3f CActor::AIFindWalkTarget()
{
	gameBlockInfo* Block = GLevel.GetBlock( this->GetBlockPos() );
	if( Block == NULL )
		return Vector3f( 0.0f ,0.0f, 0.0f );
	
	bool waysok[4] = { false, false, false, false };
	int waycount = 0;
	if( !( Block->walls & LEV_WALL_LEFT ) )
	{
		waycount++;
		waysok[0] = true;
	}
	if( !( Block->walls & LEV_WALL_RIGHT ) )
	{
		waycount++;
		waysok[1] = true;
	}
	if( !( Block->walls & LEV_WALL_TOP ) )
	{
		waycount++;
		waysok[2] = true;
	}
	if( !( Block->walls & LEV_WALL_BOTTOM ) )
	{
		waycount++;
		waysok[3] = true;
	}

	if( waycount == 0 )
		return Vector3f( 0.0f ,0.0f, 0.0f );

	Vector3f BPos = this->GetBlockPos();
	do
	{
		int way = rand() % 4;
		if( !waysok[way] )
			continue;

		switch( way )
		{
		case 0 :
			return Vector3f( ( BPos.X - 1.0f ) * 10.0f + 5.0f, 0.0f, ( BPos.Z ) * -10.0f - 5.0f );
		case 1 :
			return Vector3f( ( BPos.X + 1.0f ) * 10.0f + 5.0f, 0.0f, ( BPos.Z ) * -10.0f - 5.0f );
		case 2 :
			return Vector3f( ( BPos.X ) * 10.0f + 5.0f, 0.0f, ( BPos.Z + 1.0f ) * -10.0f - 5.0f );
		case 3 :
			return Vector3f( ( BPos.X ) * 10.0f + 5.0f, 0.0f, ( BPos.Z - 1.0f ) * -10.0f - 5.0f );		
		}
	}
	while( true );
}

void CActor::AIWalk()
{
	if( !HasTarget )
	{
		AIState = AI_STATE_NOTHING;
		return;
	}

	if( IsCollOnRay( Target, NextPos ) )
	{
		AIStand();
		return;
	}

	AIState = AI_STATE_WALK;

	float Ang = SwapAngle( ::GetAngle( Target, NextPos ) );

	this->GoToAngle( Ang );

	if( mathDist( Pos, Target ) < 0.5f )
	{
		AIState = AI_STATE_NOTHING;
		return;
	}

	if( fabsf( Ang - Angle ) < 1.0f )
	{
		Actions |= GAME_MOVE_FORWARD;
	}

}

void CActor::AIStand()
{
	AIState = AI_STATE_STAND;
}

void CActor::AIAttackTarget()
{
	if( IsEnemyInFront() )
	{
		AIState = AI_STATE_FIRE;
		Fire( Target );
	}
	else
	{
		if( Enemy != NULL )
		{
			GoToAngle( ::GetAngle( Enemy->NextPos, Pos ) );
		}
	}
}

void CActor::Fire( Vector3f Target )
{
	// Dla niezdefiniwanych obiektów to pole jest puste
}

float CActor::GetAng()
{
	return Angle;
}

void CActor::ModAngle( float mod )
{
	Angle += mod;
	Angle = SwapAngle( Angle );
}

void CActor::SetAngle( float set )
{
	Angle = set;
	Angle = SwapAngle( Angle );
}

void CActor::GoToAngle( float ang )
{
	ang = SwapAngle( ang );
	if( ang > Angle )
	{
		if( ang - Angle < 180.0f )
		{
			ModAngle( RotSpeed * GUI.GetSpeed() );
		}
		else
		{
			ModAngle( -RotSpeed * GUI.GetSpeed() );
		}
	}
	else if( ang < Angle )
	{
		if( Angle - ang < 180.0f )
		{
			ModAngle( -RotSpeed * GUI.GetSpeed() );
		}
		else
		{
			ModAngle( RotSpeed * GUI.GetSpeed() );
		}
	}
}

void CActor::SetStartAngle( float set )
{
	StartAngle = set;
}

float CActor::GetStartAngle()
{
	return StartAngle;
}

void CActor::SetStartPos( Vector3f set )
{
	StartPos = set;
}

Vector3f CActor::GetStartPos()
{
	return StartPos;
}

unsigned int CActor::GetType()
{
	return Type;
}

void CActor::Reset()
{
	CActorStats::Reset();
	Pos = StartPos;
	NextPos = StartPos;
	Angle = StartAngle;
	Armor = 0.0f;
	Enemy = NULL;
	HasTarget = false;
}

void CActor::Free()
{

}
