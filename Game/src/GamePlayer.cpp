#include "GamePlayer.h"

#include "Weapon.h"
#include "WeaponRocketLuncher.h"

#include "gui.h"
#include "Level.h"

#include "ItemHealth.h"
#include "ItemAmmo.h"
#include "ItemArmor.h"
#include "ItemWeapon.h"

/*=========================	
KLASA CPlayer


=========================*/
CPlayer::CPlayer( CModelManager& modelManager ) :
	CActor( ACTOR_TYPE::ACTOR_PLAYER ),
	ModelManager( modelManager ),
	Hand( WEAPON_HAND::RIGHT )
{

	run = true;
	Angle = 0.0f;
	WalkStep = 5.0f;
	RunStep = 10.0f;
	Radius = 4.0f;
	Pos.Set( 5.0f, 0.0f, -5.0f );

	Armor = 0.0f;
	MaxArmor = 150.0f;
	CurrWeap = 0;

	for( unsigned i = 0; i < WEAPON_COUNT; i++ )
		Weapon[i] = nullptr;

	Weapon[(unsigned)WEAPON_TYPE::ROCKET_LUNCHER] = new CWeaponRocketLuncher( modelManager );
	//Weapon[(unsigned)WEAPON_TYPE::SAW] = new weSaw();
	//Weapon[(unsigned)WEAPON_TYPE::PISTOL] = new wePistol();
	//Weapon[(unsigned)WEAPON_TYPE::MINIPHAZER] = new weMiniPhazer();
	//Weapon[(unsigned)WEAPON_TYPE::MINIGUN] = new weMiniGun();
	//Weapon[(unsigned)WEAPON_TYPE::PHAZER] = new wePhazer();
	//Weapon[(unsigned)WEAPON_TYPE::ATOM_BOMB] = new weATOM_BOMBb();
}

CPlayer::~CPlayer()
{
	for( unsigned i = 0 ; i < WEAPON_COUNT; i++ )
	{
		if( Weapon[i] != nullptr )
		{
			delete Weapon[i];
			Weapon[i] = nullptr;
		}
	}
}

void	CPlayer::OnDie()
{
}

void	CPlayer::OnDead()
{
}

void CPlayer::ApplyNextPos()
{
	Pos = NextPos;
}

void CPlayer::Render()
{
	if( Weapon[CurrWeap] != nullptr && Weapon[CurrWeap]->GetHave() )
		Weapon[CurrWeap]->Render();
}

void CPlayer::Update( const float fTD )
{
	CActor::Update( fTD );

	GUI.PInfo.WeapName = "---";
	if( Weapon[CurrWeap] != nullptr && Weapon[CurrWeap]->GetHave() )
	{
		Weapon[CurrWeap]->Update( fTD );

		GUI.PInfo.AMMO = Weapon[CurrWeap]->GetAmmo();
		GUI.PInfo.CLIPS = 0;
		GUI.PInfo.WeapName = Weapon[CurrWeap]->GetName();
	}
}

void CPlayer::ParseKeys( const bool* Keys )
{
	//if( Keys['W'] || Keys[VK_UP] )
	//	DoAction( GAME_ACTION::MOVE_FORWARD );
	//if( Keys['S'] || Keys[VK_DOWN] )
	//	DoAction( GAME_ACTION::MOVE_BACK );
	//if( Keys[VK_LEFT] )
	//	ModAngle( -1.5f );
	//if( Keys[VK_RIGHT] )
	//	ModAngle( 1.5f );
	//if( Keys['A'] )
	//	DoAction( GAME_ACTION::MOVE_STRAFE_LEFT );
	//if( Keys['D'] )
	//	DoAction( GAME_ACTION::MOVE_STRAFE_RIGHT );
	//if( Keys[VK_LBUTTON] || Keys[VK_CONTROL] )
	//	DoAction( GAME_ACTION::DO_ATTACK );
	if( Keys['0'] )
		SwichWeap( 0 );
	if( Keys['1'] )
		SwichWeap( 1 );
	if( Keys['3'] )
		SwichWeap( 3 );
	if( Keys['4'] )
		SwichWeap( 4 );
	if( Keys['5'] )
		SwichWeap( 5 );
	if( Keys['7'] )
		SwichWeap( 7 );
	if( Keys['9'] )
		SwichWeap( 9 );

	if( run )
		Speed = RunStep;
	else Speed = WalkStep;
}

const WEAPON_HAND CPlayer::GetHand() const
{
	return Hand;
}

void CPlayer::TestWeapon( CWeapon* Weap )
{
	//if( DistanceSq( Pos, Weap->Pos ) <= POW(Radius + Weap->Radius) )
	//{
	//	if( !Weapon[(unsigned)Weap->GetType()]->GetInited() )
	//		this->SwichWeap( (unsigned)Weap->GetType() );

	//	Weapon[(unsigned)Weap->GetType()]->PickUp( Weap, this, *ModelManager );
	//}
}

void CPlayer::TestBonus( CItem* Bonus )
{
	if( DistanceSq( Bonus->Pos, this->NextPos ) < POW( Radius ) )
	{
		switch( Bonus->GetType() )
		{
		case ITEM_TYPE::AMMO :
			if( Weapon[(unsigned)((CItemAmmo*)Bonus)->GetWeaponType()]->ModAmmo( ((CItemAmmo*)Bonus)->GetAmmoCount() ) )
			{
				GUI.SendMsg( "Podniosles: " + IntToStr( ((CItemAmmo*)Bonus)->GetAmmoCount() ) + " Amunicji", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 0.5f, 0.5f, 0.5f );
				Bonus->CanDelete = true;
			}
			break;
		case ITEM_TYPE::HEALTH :
			if( Health != MaxHealth )
			{
				GUI.SendMsg( "Podniosles: " + FloatToStr( ((CItemHealth*)Bonus)->GetHealth() ) + " Zdrowia", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 1.0f, 0.3f, 0.3f );
				ModHealth( ((CItemHealth*)Bonus)->GetHealth() );
				Bonus->CanDelete = true;
			}
			break;
		case ITEM_TYPE::ARMOR :
			if( Armor != MaxArmor )
			{
				GUI.SendMsg( "Podniosles: " + FloatToStr( ((CItemArmor*)Bonus)->GetArmor() ) + " Pancerza", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 1.0f, 0.3f, 0.3f );
				ModArmor( ((CItemArmor*)Bonus)->GetArmor() );
				Bonus->CanDelete = true;
			}
			break;
		}
	}
}
void CPlayer::SwichWeap( unsigned int index )
{
	CurrWeap = index;
}

void CPlayer::ModHealth( const float mod )
{
	if( mod < 0.0f )
	{
		GUI.ActiveFScrColor( 1.0f, 0.0f, 0.0f, 0.25f );
		ModArmor( mod * 1.5f );
	}
	else if( mod > 0.0f )
	{
		GUI.ActiveFScrColor( 1.0f, 1.0f, 0.0f );
	}	
	CActor::ModHealth( mod );
}

void CPlayer::Reset()
{
	CActor::Reset();
	for( unsigned i = 0; i < WEAPON_COUNT; i++ )
	{
		if( Weapon[i] != nullptr )
		{
			delete Weapon[i];
			Weapon[i] = nullptr;
		}
	}
	//Weapon[(unsigned)WEAPON_TYPE::SAW] = new weSaw();
	//Weapon[(unsigned)WEAPON_TYPE::PISTOL] = new wePistol();
	//Weapon[(unsigned)WEAPON_TYPE::MINIPHAZER] = new weMiniPhazer();
	//Weapon[(unsigned)WEAPON_TYPE::MINIGUN] = new weMiniGun();
	Weapon[(unsigned)WEAPON_TYPE::ROCKET_LUNCHER] = new CWeaponRocketLuncher( ModelManager );
	//Weapon[(unsigned)WEAPON_TYPE::PHAZER] = new wePhazer();
	//Weapon[(unsigned)WEAPON_TYPE::ATOM_BOMB] = new weATOM_BOMBb();
	GUI.PInfo.FRAGS = 0;
}

const bool	CPlayer::OnCollision( CObject* pObject )
{
	CItem* pItem = dynamic_cast<CItem*>(pObject);
	if( pItem != nullptr )
	{
		switch (pItem->GetType())
		{
		case ITEM_TYPE::WEAPON:
			{
				CItemWeapon* pWeap = dynamic_cast<CItemWeapon*>(pItem);
				if(pWeap != nullptr && Weapon[(unsigned)pWeap->GetWeaponType()] != nullptr)
				{
					Weapon[(unsigned)pWeap->GetWeaponType()]->PickUp( *this, pWeap->GetAmmoCount() );
					pWeap->SetActive( false );
					return false;
				}
			}
			break;

		default:
			return true;
		}
	}

	return true;
}

const bool	CPlayer::OnCollision( const Vector3f& point, const Planef& plane )
{
	return true;
}

void	CPlayer::SolveActions( const float fTD )
{
	if( (Actions & (unsigned)GAME_ACTION::DO_ATTACK) && Weapon[CurrWeap] != nullptr )
		Weapon[CurrWeap]->Shot();

	CActor::SolveActions( fTD );
}