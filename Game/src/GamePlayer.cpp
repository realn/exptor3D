#include "GamePlayer.h"

#include "Weapon.h"
#include "WeaponRocketLuncher.h"

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
	Hand( WEAPON_HAND::RIGHT ),
	Run( false )
{
	Angle = 0.0f;
	WalkStep = 5.0f;
	RunStep = 10.0f;

	Speed = WalkStep;

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

void CPlayer::Render()
{
	if( Weapon[CurrWeap] != nullptr && Weapon[CurrWeap]->GetHave() )
		Weapon[CurrWeap]->Render();
}

void CPlayer::Update( const float fTD )
{
	if( Run )
		Speed = RunStep;
	else
		Speed = WalkStep;

	CActor::Update( fTD );

	if( Weapon[CurrWeap] != nullptr && Weapon[CurrWeap]->GetHave() )
		Weapon[CurrWeap]->Update( fTD );
}

const WEAPON_HAND CPlayer::GetHand() const
{
	return Hand;
}

void	CPlayer::SetMoveSpeed( const bool run )
{
	Run = run;
}

const bool	CPlayer::ProcessItem( CItem* pItem )
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

void CPlayer::SwichWeap( unsigned int index )
{
	CurrWeap = index;
}

void CPlayer::ModHealth( const float mod )
{
	if( mod < 0.0f )
	{
		//GUI.ActiveFScrColor( 1.0f, 0.0f, 0.0f, 0.25f );
		ModArmor( mod * 1.5f );
	}
	else if( mod > 0.0f )
	{
		//GUI.ActiveFScrColor( 1.0f, 1.0f, 0.0f );
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
}

const bool	CPlayer::OnCollision( CObject* pObject )
{
	CItem* pItem = dynamic_cast<CItem*>(pObject);
	if( pItem != nullptr )
		return ProcessItem( pItem );

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