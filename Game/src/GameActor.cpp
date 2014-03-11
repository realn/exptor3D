#include "GameActor.h"
#include "Level.h"
#include "GamePlayer.h"

/*=========================	
	KLASA CActor


=========================*/
CActor::CActor( const ACTOR_TYPE type ) : 
	CDynamic(2.0f),
	CActorStats(100.0f, 100.0f),
	Type( type ),
	Actions( 0 )
{
	Armor = 0.0f;
	
	Speed = 0.1f;
	Angle = 180.0f;
	StartAngle = 180.0f;
	Pos.Set( 5.0f, 0.0f, -25.0f );
}

CActor::~CActor()
{
}

const ACTOR_TYPE CActor::GetType() const
{
	return Type;
}

void	CActor::DoAction( const GAME_ACTION action )
{
	Actions |= (unsigned)action;
}

void	CActor::DoRotate( const float angle )
{
	Angle = ::SwapAngle( angle );
}

void CActor::Update( const float fTD )
{

	UpdateStats( fTD );

	if( State == ACTOR_STATE::DEAD )
		return;

	Pos = NextPos;

	SolveActions( fTD );

	if( Pos != NextPos )
	{
		Log.Log( " DIFF2 " );
	}
}

/*
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

			if( Distance( Pos, Target ) < 70.0f )
			{
				if( this->AIflags & AI_ATTACK_CLOSE )
				{
					if( Distance( Pos, Target ) > 5.0f )
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
	CLvlBlock* Block = pGLevel->GetBlock( this->GetBlockPos() );
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

	if( Distance( Pos, Target ) < 0.5f )
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
*/

const float CActor::GetAngle() const
{
	return Angle;
}

void CActor::ModAngle( const float mod )
{
	Angle = SwapAngle( Angle + mod );
}

void CActor::SetAngle( const float set )
{
	Angle = SwapAngle( set );
}

void CActor::SetStartAngle( const float set )
{
	StartAngle = SwapAngle( set );
}

const float CActor::GetStartAngle() const
{
	return StartAngle;
}

void CActor::SetStartPos( const Vector3f& set )
{
	StartPos = set;
}

const Vector3f CActor::GetStartPos() const
{
	return StartPos;
}

void CActor::Reset()
{
	CActorStats::Reset();

	Pos = StartPos;
	NextPos = StartPos;
	Angle = StartAngle;
	Armor = 0.0f;
}

void	CActor::SolveActions( const float fTD )
{
	Vector3f temp;

	if( Actions & (unsigned)GAME_ACTION::MOVE_FORWARD )
		temp += MakeVectorXZ( Angle );

	if( Actions & (unsigned)GAME_ACTION::MOVE_BACK )
		temp += -MakeVectorXZ( Angle );

	if( Actions & (unsigned)GAME_ACTION::MOVE_STRAFE_LEFT )
		temp += MakeVectorXZ( Angle - 90 );

	if( Actions & (unsigned)GAME_ACTION::MOVE_STRAFE_RIGHT )
		temp += MakeVectorXZ( Angle + 90 );

	if( temp.LengthSq() == 0.0f )
		NextPos = Pos;
	else
		NextPos = Pos + temp.Normalize() * Speed * fTD;

	Vector = MakeVectorXZ( Angle );

	Actions = 0;
}