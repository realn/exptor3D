#include "GamePlayer.h"

#include "Weapon.h"
#include "gui.h"
#include "Level.h"
#include "ItemManager.h"
#include "ItemHealth.h"
#include "ItemAmmo.h"
#include "ItemArmor.h"

/*=========================	
	KLASA CPlayer


=========================*/
CPlayer::CPlayer()
{
	run = true;
	Angle = 0.0f;
	WalkStep = 2.5f;
	RunStep = 5.0f;
	Radius = 3.0f;
	Pos.Set( 5.0f, 0.0f, -5.0f );
	AIflags = AI_NO_AI;
	Type = GAME_THING_PLAYER;

	Armor = 0.0f;
	MaxArmor = 150.0f;
	CurrWeap = GAME_WEAP_PHAZER;

	Weapon[GAME_WEAP_SAW] = new weSaw();
	Weapon[GAME_WEAP_PISTOL] = new wePistol();
	Weapon[GAME_WEAP_MINIPZR] = new weMiniPhazer();
	Weapon[GAME_WEAP_MINIGUN] = new weMiniGun();
	Weapon[GAME_WEAP_ROCKETLUN] = new weRocketLuncher();
	Weapon[GAME_WEAP_PHAZER] = new wePhazer();
	Weapon[GAME_WEAP_ATOM_BOMB] = new weAtomBomb();
}

CPlayer::~CPlayer()
{
	delete Weapon[GAME_WEAP_SAW];
	delete Weapon[GAME_WEAP_PISTOL];
	delete Weapon[GAME_WEAP_MINIPZR];
	delete Weapon[GAME_WEAP_MINIGUN];
	delete Weapon[GAME_WEAP_ROCKETLUN];
	delete Weapon[GAME_WEAP_PHAZER];
	delete Weapon[GAME_WEAP_ATOM_BOMB];
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
	if( Weapon[CurrWeap]->GetHave() )
		Weapon[CurrWeap]->Render();
}

void CPlayer::Update( const float fTD )
{

}

void CPlayer::Update( bool* Keys, const float fTD )
{
	Actions = 0;
	if( Keys['W'] || Keys[VK_UP] )
	{
		Actions |= GAME_MOVE_FORWARD;
	}
	if( Keys['S'] || Keys[VK_DOWN] )
	{
		Actions |= GAME_MOVE_BACK;
	}
	if( Keys[VK_LEFT] )
	{
		ModAngle( -1.5f );
	}
	if( Keys[VK_RIGHT] )
	{
		ModAngle( 1.5f );
	}
	if( Keys['A'] )
	{
		Actions |= GAME_MOVE_STRAFE_L;
	}
	if( Keys['D'] )
	{
		Actions |= GAME_MOVE_STRAFE_R;
	}
	if( Keys[VK_LBUTTON] || Keys[VK_CONTROL] )
	{
		Actions |= GAME_DO_FIREWEAPON;
	}
	if( Keys['0'] )
	{
		SwichWeap( 0 );
		Keys['0'] = false;
	}
	if( Keys['1'] )
	{
		SwichWeap( 1 );
		Keys['1'] = false;
	}
	if( Keys['3'] )
	{
		SwichWeap( 3 );
		Keys['3'] = false;
	}
	if( Keys['4'] )
	{
		SwichWeap( 4 );
		Keys['4'] = false;
	}
	if( Keys['5'] )
	{
		SwichWeap( 5 );
		Keys['5'] = false;
	}
	if( Keys['7'] )
	{
		SwichWeap( 7 );
		Keys['7'] = false;
	}
	if( Keys['9'] )
	{
		SwichWeap( 9 );
		Keys['9'] = false;
	}
	if( run )
		Speed = RunStep;
	else Speed = WalkStep;

	Move( Actions, fTD );

	float an = Angle;
	switch( GUI.GetHandPos() )
	{
	case GAME_RIGHT_HAND :
		an += 50.0f;
		break;
	case GAME_LEFT_HAND :
		an -= 50.0f;
		break;
	case GAME_CENTER_HAND :
	default:
		an = 0.0f;			
	}
	Vector3f temp;
	temp.X = sinf( an * PIOVER180 ) * 1.2f;
	temp.Z = -cosf( an * PIOVER180 ) * 1.2f;
	temp.Y = -1.0f;

	GUI.PInfo.WeapName = "---";
	if( Weapon[CurrWeap]->GetHave() )
	{
		Weapon[CurrWeap]->Pos = Pos + temp;
		Weapon[CurrWeap]->Update( fTD );
		GUI.PInfo.AMMO = Weapon[CurrWeap]->GetAmmo();
		GUI.PInfo.CLIPS = Weapon[CurrWeap]->GetClip();
		GUI.PInfo.WeapName = Weapon[CurrWeap]->Name;
	}
	TestCollBlock( this, GLevel.GetBlock( this->GetBlockPos() ), true );

	ApplyNextPos();
}

void	CPlayer::Move(unsigned uFlags, const float fTD)
{
	if( uFlags & GAME_DO_FIREWEAPON )
		this->Weapon[this->CurrWeap]->Shot();

	CActor::Move(uFlags, fTD);
}

unsigned int CPlayer::GetHand()
{
	return Hand;
}

void CPlayer::TestWeapon( weWeapon* Weap )
{
	if( mathDistSq( Pos, Weap->Pos ) <= POW(Radius + Weap->Radius) )
	{
		if( !Weapon[Weap->GetType()]->GetInited() )
			this->SwichWeap( Weap->GetType() );

		Weapon[Weap->GetType()]->PickUp( Weap, this );
	}
}

void CPlayer::TestBonus( CItem* Bonus )
{
	if( mathDistSq( Bonus->Pos, this->NextPos ) < POW( Radius ) )
	{
		switch( Bonus->GetType() )
		{
		case BONUS_TYPE_AMMO :
			if( Weapon[((CItemAmmo*)Bonus)->GetWeapType()]->ModAmmo( ((CItemAmmo*)Bonus)->GetAmmoCount() ) )
			{
				GUI.SendMsg( "Podniosles: " + guiIntToStr( ((CItemAmmo*)Bonus)->GetAmmoCount() ) + " Amunicji", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 0.5f, 0.5f, 0.5f );
				Bonus->CanDelete = true;
			}
			break;
		case BONUS_TYPE_HEALTH :
			if( Health != MaxHealth )
			{
				GUI.SendMsg( "Podniosles: " + guiFloatToStr( ((CItemHealth*)Bonus)->GetHealth() ) + " Zdrowia", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 1.0f, 0.3f, 0.3f );
				ModHealth( ((CItemHealth*)Bonus)->GetHealth() );
				Bonus->CanDelete = true;
			}
			break;
		case BONUS_TYPE_ARMOR :
			if( Armor != MaxArmor )
			{
				GUI.SendMsg( "Podniosles: " + guiFloatToStr( ((CItemArmor*)Bonus)->GetArmor() ) + " Pancerza", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 1.0f, 0.3f, 0.3f );
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
	for( int i = 0; i < 10; i++ )
	{
		delete Weapon[i];
	}
	Weapon[GAME_WEAP_SAW] = new weSaw();
	Weapon[GAME_WEAP_PISTOL] = new wePistol();
	Weapon[GAME_WEAP_MINIPZR] = new weMiniPhazer();
	Weapon[GAME_WEAP_MINIGUN] = new weMiniGun();
	Weapon[GAME_WEAP_ROCKETLUN] = new weRocketLuncher();
	Weapon[GAME_WEAP_PHAZER] = new wePhazer();
	Weapon[GAME_WEAP_ATOM_BOMB] = new weAtomBomb();
	GUI.PInfo.FRAGS = 0;
}

CPlayer MainPlayer;
