#include "Game.h"

CActorStats::CActorStats() :
	Health(100.0f),
	MaxHealth(100.0f),
	Armor(100.0f),
	MaxArmor(100.0f),
	State(ACTOR_STATE::ALIVE),
	TimeUntilDead(3.0f),
	DyingTime(0.0f)
{

}

CActorStats::CActorStats(const float health, const float armor) :
	Health(health),
	MaxHealth(health),
	Armor(armor),
	MaxArmor(armor),
	State(ACTOR_STATE::ALIVE),
	TimeUntilDead(3.0f),
	DyingTime(0.0f)
{

}
	
void	CActorStats::UpdateStats(const float fTD)
{
	if(State == ACTOR_STATE::ALIVE)
	{
		if(Health <= 0.0f)
		{
			State = ACTOR_STATE::DYING;
			OnDie();
		}
	}
	if(State == ACTOR_STATE::DYING)
	{
		DyingTime += fTD;
		if(DyingTime >= TimeUntilDead)
		{
			State = ACTOR_STATE::DEAD;
			OnDead();
		}
	}
}

const bool CActorStats::IsDying() const
{
	return State == ACTOR_STATE::DYING;
}

const bool CActorStats::IsDead() const
{
	return State == ACTOR_STATE::DEAD;
}

void CActorStats::Reset()
{
	State = ACTOR_STATE::ALIVE;
	Health = MaxHealth;
	Armor = MaxArmor;
}


const ACTOR_STATE	CActorStats::GetState() const
{
	return State;
}

const float CActorStats::GetHealth() const
{
	return Health;
}

void CActorStats::ModHealth( const float mod )
{
	Health += mod;
	if( Health > MaxHealth )
		Health = MaxHealth;
	if( Health < 0.0f )
		Health = 0.0f;
}

void CActorStats::SetHealth( const float set )
{
	Health = set;
}

const float CActorStats::GetArmor() const
{
	return Armor;
}

void CActorStats::ModArmor( const float mod )
{
	Armor += mod;
	if( Armor > MaxArmor )
		Armor = MaxArmor;
	if( Armor < 0.0f )
		Armor = 0.0f;
}

void CActorStats::SetArmor( const float set )
{
	Armor = set;
}
