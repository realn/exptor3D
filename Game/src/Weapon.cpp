/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Weapon.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Weapon.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Weapon.h"

#include "GamePlayer.h"
#include "Level.h"
#include "Special.h"

/*	KLASA WEAPON
	Jest to klasa do dziedziczenia,
	zawiera wszystko co jest potrzebne
	do broni :)
*/

/*	KONSTRUKTOR	*/
CWeapon::CWeapon( const WEAPON_TYPE type, const std::string& name ) :
	Type( type ),
	Name( name ),
	State( WEAPON_STATE::UNKNOWN ),
	Hand( WEAPON_HAND::RIGHT ),
	Shake( 0.0f ),
	ShakeSpeed( 10.0f, 5.0f, 0.0f ),
	ShakeRadius( 0.1f, 0.1f, 0.0f ),
	Model( nullptr ),
	Owner( nullptr ),
	DamageMax( 1.0f ),
	DamageMin( 0.0f ),
	Time( 0.0f ),
	Ammo( 0 ),
	MaxAmmo( 0 ),
	ShotTime( 0.0f ),
	ShotPause( 0.0f ),
	InHand( false )
{
}

/*	DESTRUKTOR	*/
CWeapon::~CWeapon()
{
}

const std::string	CWeapon::GetName() const
{
	return Name;
}

const WEAPON_TYPE	CWeapon::GetType() const
{
	return Type;
}

const WEAPON_STATE	CWeapon::GetState() const
{
	return State;
}

/*	Ta funkcja jest wywo³ywana, gdy
	jakaœ postaæ podniesie broñ.
*/
void CWeapon::PickUp( CPlayer& Player, const unsigned ammo )
{
	if( !InHand )
	{
		Hand = Player.GetHand();
		InHand = true;
		Owner = &Player;

		SetAmmo( ammo );
		GUI.SendMsg( "Podniosles: " + Name, 4000, 10.0f, -1.0f, 1.5f, 1.5f, 1.0f, 0.5f, 0.5f );
		State = WEAPON_STATE::READY;
	}
	else
	{
		ModAmmo( ammo );
		GUI.SendMsg( "Podniosles " + IntToStr( ammo ) + " amunicji.", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 0.5f, 1.0f, 0.5f );		
	}
}

const int CWeapon::GetAmmo() const
{
	return Ammo;
}

const unsigned	CWeapon::GetMaxAmmo() const
{
	return MaxAmmo;
}

void CWeapon::SetAmmo( int ammo )
{
	Ammo = ammo;
}

bool CWeapon::ModAmmo( int ammo )
{
	if( Ammo == MaxAmmo || Ammo == -1 )
		return false;

	Ammo += ammo;
	if( Ammo > MaxAmmo )
		Ammo = MaxAmmo;

	return true;
}

bool CWeapon::GetHave()
{
	return InHand;
}

void CWeapon::Update( const float fTD )
{
	if( State == WEAPON_STATE::UNKNOWN )
		return;

	switch (State)
	{
	case WEAPON_STATE::UNKNOWN:
		break;

	case WEAPON_STATE::READY:
		Shake = SwapAngle( Shake + fTD );
		break;

	case WEAPON_STATE::FIRING:
		Time += fTD;
		if( Time > ShotTime )
		{
			OnFired();
			State = WEAPON_STATE::RECOIL;
			Time = 0.0f;
		}
		break;

	case WEAPON_STATE::RECOIL:
		Time += fTD;
		if( Time > ShotPause )
		{
			State = WEAPON_STATE::READY;
			Time = 0.0f;
		}
		break;

	default:
		break;
	}
}

void CWeapon::Render()
{

}

void CWeapon::Shot()
{
	if( State == WEAPON_STATE::READY )
		State = WEAPON_STATE::FIRING;
}

const float	CWeapon::GenDamage() const
{
	if(DamageMax == DamageMin)
		return DamageMax;
	return DamageMin + ((float)rand() / (float)RAND_MAX) * (DamageMax - DamageMin);
}

const Vector3f	CWeapon::GenPos() const
{
	switch (Hand)
	{
	case WEAPON_HAND::CENTER:
		return Vector3f( 0.0f, Pos.Y, Pos.Z );
	case WEAPON_HAND::LEFT:
		return Vector3f( -Pos.X, Pos.Y, Pos.Z );
	default:
	case WEAPON_HAND::RIGHT:
		return Vector3f( Pos.X, Pos.Y, Pos.Z );
	}
}

const Vector3f	CWeapon::GenWorldPos( const Vector3f pos ) const
{
	Vector3f result;

	float fx = cosf( Owner->GetAngle() * PIOVER180 );
	float fy = sinf( Owner->GetAngle() * PIOVER180 );

	result.X = fx * pos.X - fy * pos.Z;
	result.Y = pos.Y;
	result.Z = fy * pos.X + fx * pos.Z;

	return Owner->Pos + result;
}

const Vector3f	CWeapon::GenShakePos() const
{
	Vector3f pos;

	pos.X = sinf( Shake * PIOVER180 * ShakeSpeed.X ) * ShakeRadius.X;
	pos.Y = cosf( Shake * PIOVER180 * ShakeSpeed.Y ) * ShakeRadius.Y;
	pos.Z = sinf( Shake * PIOVER180 * ShakeSpeed.Z ) * cosf( Shake * PIOVER180 * ShakeSpeed.Z ) * ShakeRadius.Z;

	return pos;
}

void	CWeapon::OnShot()
{

}

void	CWeapon::OnFired()
{

}

const WEAPON_TYPE	ParseWeapon( const std::string& str )
{
	if( str == "SAW" )			return WEAPON_TYPE::SAW;
	if( str == "PISTOL" )		return WEAPON_TYPE::PISTOL;
	if( str == "MINIPHAZER" )	return WEAPON_TYPE::MINIPHAZER;
	if( str == "MINIGUN" )		return WEAPON_TYPE::MINIGUN;
	if( str == "ROCKET_LUNCHER" )	return WEAPON_TYPE::ROCKET_LUNCHER;
	if( str == "PICK_A_BOO" )	return WEAPON_TYPE::PICK_A_BOO;
	if( str == "PHAZER" )		return WEAPON_TYPE::PHAZER;
	if( str == "MINE" )			return WEAPON_TYPE::MINE;
	if( str == "ATOM_BOMB" )	return WEAPON_TYPE::ATOM_BOMB;

	return WEAPON_TYPE::UNKNOWN;
}


//#include "WeaponBulletSaw.h"
//#include "WeaponBulletManager.h"
//
///*	PI£A :D
//	Teraz zaczynaj¹ siê klasy broni.
//	Ka¿da broñ ma odzieln¹ klas¹, by
//	mia³a w³asny wygl¹d, w³aœciwoœci
//	i animacje. Na pierwszy ogieñ idzie
//	pi³a tarczowa ;D
//*/
//
//weSaw::weSaw() :
//	CWeapon(WEAPON_TYPE::SAW)
//{
//}
//
///*	Metoda inicjuje pi³e, czyli
//	Przypisuje wartoœci i tworzy jej
//	wygl¹d. Mog³em zrobiæ jakiœ loader
//	plików, ale to by siê zesz³o zbyt
//	d³ugo :P 
//*/
//void weSaw::Init( CModelManager& modelManager )
//{
//	if( inited ) 
//		return;
//
//	/*	Najpierw inicjujemy zmienne.
//		Te bêd¹ odpowiedzialne za
//		animacje obracaj¹cej siê tarczy
//		i przysuwania podczas ataku
//	*/
//	ShotTime = 0.0f;
//	ShotPause = 0.1f;
//	ReloadTime = 0.0f;
//	/*	Teraz ustawiamy amunicje.
//		Jako, ¿e broñ nie posiada
//		amunicji jako takiej, to
//		ustawiamy na -1 (nieskoñczone)
//	*/
//	Ammo = -1;
//	MaxAmmo = -1;
//	AmmoPerClip = -1;
//
//	Damage[0] = 0.1f;
//	Damage[1] = 0.5f;
//
//	Name = "SAW";
//
//	Model = modelManager.Get( "saw-model.glm" );
//
//	inited = true;
//}
//
///*	METODA ENGINE'A
//	By zachowaæ jak¹œ dynamike
//	doda³em tu parê bajerów: ruszaj¹ca
//	siê broñ, przysuwanie przy strzale,
//	powolny rozruch tarczy, itp. Lepiej
//	to wygl¹da ni¿ statyczny model i s³owo
//	"O, trafi³em..." ;)
//*/
//void weSaw::Update( const float fTD )
//{
//	// Sprawdzamy, czy broñ jest podnisiona
//	if( !InHand ) 
//		return;
//
//	// Je¿eli broñ jest w rêku, to uruchamiamy dodatkowy kod.
//
//	/*	Gdy strzelamy, broñ siê przysuwa do przeciwnika,
//		a ostrze wiruje coraz szybciej. Przy okazji rêka siê
//		nie trzêsie ( jak to dziwnie brzmi :P )
//	*/
//	if( fire ) 
//	{
//		if( ShotPause < 20.0f )
//			ShotPause += 0.4f * fTD;
//		if( ReloadTime > -1.0f )
//			ReloadTime -= 0.2f * fTD;
//
//		Shake[0] = 0.0f;
//		Shake[1] = 0.0f;
//		CBullSaw* bull = new CBullSaw( Owner, 0.5f, Owner->Pos );
//		BManager.AddBullet( bull );
//		fire = false;
//	}
//	/*	Gdy jednak nic siê niedzieje, ostrze zwalnia, a broñ
//		wraca na miejsce. Przy okazji brón wykonuje delikatne
//		"chwianie" siê powoduj¹c z³udzenie trzymaj¹cej jej rêki.
//		*/
//	else 
//	{
//		if( ShotPause > 0.3f )
//			ShotPause -= 0.1f * fTD;
//		if( ReloadTime < 0.0f )
//			ReloadTime += 0.05f * fTD;
//
//		Shake[0] = ( sinf( Rot / 10.0f ) / 20.0f );
//		Shake[1] = ( cosf( Rot / 10.0f ) / 20.0f );
//	}
//	// Obracamy ostrze i gdy wykona³o pe³ny obrót, zerujemy
//	ShotTime += ShotPause * GUI.GetSpeed() * fTD;
//	if( ShotTime >= 360.0f )
//		ShotTime = 0.0f;
//}
//
///*	METODA RYSUJ¥CA
//	Tu nastêpuje wyœwietlenie na ekranie. Nic wielkiego :P
//*/
//void weSaw::Render()
//{
//	// Zachowujemy aktualn¹ Macierz
//	glPushMatrix();
//	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
//	if( !InHand )
//	{
//		glTranslatef( Pos.X, 0.0f, Pos.Z );
//
//		glTranslatef( 0.0f, -3.0f, 0.0f );
//		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
//		Model->CallObject( 0 );
//	}
//	else
//	{
//		glLoadIdentity();
//
//		switch( hand )
//		{
//		case GAME_RIGHT_HAND :
//			glTranslatef( 1.5f, -1.5f, -5.0f );
//			break;
//		case GAME_LEFT_HAND :
//			glTranslatef( -1.5f, -1.5f, -5.0f );
//			break;
//		case GAME_CENTER_HAND :
//			glTranslatef( 0.0f, -1.5f, -5.0f );
//			break;
//		default:
//			glTranslatef( 1.5f, -1.5f, -5.0f );
//			break;
//		}
//
//		glTranslatef( Shake[0], Shake[1], ReloadTime * GUI.GetSpeed() );
//
//		Model->CallObject( 1 );		
//
//		glTranslatef( 0.0f, 0.0f, -2.0f );
//		glRotatef( ShotTime, 0.0f, 1.0f, 0.0f );
//
//		Model->CallObject( 2 );	
//	}
//
//	// Przywracamy Macierz
//	glPopMatrix();
//}
//
//void weSaw::Shot()
//{
//	if( !inited )
//		return;
//	fire = true;
//}
//
///*	PISTOLET	*/
//wePistol::wePistol() :
//	CWeapon(WEAPON_TYPE::PISTOL)
//{
//}
//
//void wePistol::Init( CModelManager& modelManager )
//{
//	if( inited ) 
//		return;
//
//	Reloading = false;
//	back = false;
//	BackA = 0.0f;
//
//	ShotTime = 30.0f;
//	ShotPause = 40.0f;
//	ReloadTime = 80.0f;
//
//	Ammo = 50;
//	AmmoPerClip = -1;
//	MaxAmmo = 999;
//	CurrClip = -1;
//
//	Damage[0] = 2.0f;
//	Damage[1] = 3.0f;
//
//	Name = "Pistol";
//
//	Time = 0.0f;
//
//	Model = modelManager.Get( "pistol-model.glm" );
//
//	inited = true;
//}
//
//void wePistol::Update( const float fTD )
//{
//	Rotate( fTD );
//
//	if( !InHand )
//		return;
//
//	if( back )
//	{
//		Time += 1.0f * GUI.GetSpeed() * fTD;
//		BackA += 0.7f * GUI.GetSpeed() * fTD;
//		if( Time >= ShotPause )
//		{
//			back = false;
//			Time = 0.0f;
//			BackA = 0.0f;
//		}
//	}
//
//	if( fire ) 
//	{
//		if( !back )
//		{
//			back = true;
//			Time = 0.0f;
//			Ammo--;
//			CurrClip--;
//
//			Vector3f temp;
//			temp = RayCast( Owner->NextPos, Owner->Vector, 2.0f, *pGLevel );
//			temp = (temp - Pos).Normalize();
//			CBullet* Bull = new CBullet( Owner, Damage[0], Pos, temp, 10.0f );
//			BManager.AddBullet( Bull );
//		}
//		Shake[0] = 0.0f;
//		Shake[1] = 0.0f;
//		fire = false;
//	}
//}
//
//void wePistol::Render()
//{
//	// Zachowujemy aktualn¹ Macierz
//	glPushMatrix();
//	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
//	if( !InHand )
//	{
//		glTranslatef( Pos.X, 0.0f, Pos.Z );
//
//		glTranslatef( 0.0f, -3.0f, 0.0f );
//		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
//		Model->CallObject( 0 );
//	}
//	else
//	{
//		glLoadIdentity();
//
//		switch( hand )
//		{
//		case GAME_RIGHT_HAND :
//			glTranslatef( 1.5f, -1.5f, -5.0f );
//			break;
//		case GAME_LEFT_HAND :
//			glTranslatef( -1.5f, -1.5f, -5.0f );
//			break;
//		case GAME_CENTER_HAND :
//			glTranslatef( 0.0f, -1.5f, -5.0f );
//			break;
//		default:
//			glTranslatef( 1.5f, -1.5f, -5.0f );
//			break;
//		}
//
//		if( back )
//			glRotatef( BackA, 1.0f, 0.0f, 0.0f );
//
//		if( Reloading && !back)
//		{
//			glTranslatef( 0.0f, -Time / 50.0f, 0.0f );
//			glRotatef( Time, 0.0f, 1.0f, 0.0f );
//		}
//
//		glTranslatef( Shake[0], Shake[1], 0.0f );
//
//		Model->CallObject( 0 );		
//	}
//
//	// Przywracamy Macierz
//	glPopMatrix();
//}
//
//void wePistol::Shot()
//{
//	if( back )
//		return;
//
//	if( Ammo > 0 || Ammo == -1 )
//		fire = true;
//}
//
//
//#include "WeaponBulletRay.h"
//
///*	MINIPHAZER	*/
//weMiniPhazer::weMiniPhazer() :
//	CWeapon(WEAPON_TYPE::MINIPHAZER)
//{
//}
//
//void weMiniPhazer::Init( CModelManager& modelManager )
//{
//	if( inited ) 
//		return;
//
//	back = false;
//	BackA = 0.0f;
//
//	ShotTime = 5.0f;
//	ShotPause = 60.0f;
//	ReloadTime = 0.0f;
//
//	Ammo = 20;
//	AmmoPerClip = -1;
//	MaxAmmo = 500;
//	CurrClip = -1;
//
//	Damage[0] = 40.0f;
//	Damage[1] = 60.0f;
//
//	Name = "MiniPhazer";
//
//	Time = 0.0f;
//
//	Model = modelManager.Get( "miniphazer-model.glm" );
//
//	inited = true;
//}
//
//void weMiniPhazer::Update( const float fTD )
//{
//	Rotate( fTD );
//
//	if( InHand )
//	{
//		Shake[0] = ( sin( Rot / 20.0f ) / 10.0f );
//		Shake[1] = ( cos( Rot / 10.0f ) / 20.0f );
//		if( fire )
//		{
//			if( !back )
//			{
//				Shake[0] = 0.0f;
//				Shake[1] = 0.0f;
//				
//				Vector3f temp;
//				temp = RayCast( Owner->Pos, Owner->Vector, 0.5f, *pGLevel );
//				temp = (temp - Pos).Normalize();
//
//				float damage = this->Damage[0] + float( rand() % int( this->Damage[1] - this->Damage[0]) );
//
//				CBullet* Bull = new CBullRay( Owner, damage, Pos, temp );
//				BManager.AddBullet( Bull );
//
//				back = true;
//				Ammo--;
//			}
//			else
//			{
//				Time += fTD * GUI.GetSpeed();
//				if( Time >= ShotPause * 0.2f )
//				{
//					BackA -= ( 1.0f / ( ShotPause * 0.8f )) * GUI.GetSpeed() * fTD;
//					if( Time >= ShotPause )
//					{
//						BackA = 0.0f;
//						Time = 0.0f;
//						fire = false;
//						back = false;
//					}
//				}
//				else
//				{
//					BackA += ( 1.0f / ( ShotPause * 0.2f ) ) * GUI.GetSpeed() * fTD;
//				}
//			}
//		}
//
//	}
//}
//
//void weMiniPhazer::Render()
//{
//	// Zachowujemy aktualn¹ Macierz
//	glPushMatrix();
//	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
//	if( !InHand )
//	{
//		glTranslatef( Pos.X, 0.0f, Pos.Z );
//
//		glTranslatef( 0.0f, -3.0f, 0.0f );
//		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
//		Model->CallObject( 0 );
//	}
//	else
//	{
//		glLoadIdentity();
//
//		switch( hand )
//		{
//		case GAME_RIGHT_HAND :
//			glTranslatef( 1.5f, -1.5f, -4.5f );
//			break;
//		case GAME_LEFT_HAND :
//			glTranslatef( -1.5f, -1.5f, -4.5f );
//			break;
//		case GAME_CENTER_HAND :
//			glTranslatef( 0.0f, -1.5f, -4.5f );
//			break;
//		default:
//			glTranslatef( 1.5f, -1.5f, -4.5f );
//			break;
//		}
//
//		if( back )
//			glTranslatef( 0.0f, 0.0f, BackA );
//
//		glTranslatef( Shake[0], Shake[1], 0.0f );
//
//		Model->CallObject( 0 );
//	}
//
//	// Przywracamy Macierz
//	glPopMatrix();
//}
//
//void weMiniPhazer::Shot()
//{
//	if( Ammo == 0 )
//		return;
//	if( !back )
//		fire = true;
//}
//
//
//
///*	PHAZER	*/
//wePhazer::wePhazer() :
//	CWeapon(WEAPON_TYPE::PHAZER)
//{
//}
//
//void wePhazer::Init( CModelManager& modelManager )
//{
//	if( inited ) 
//		return;
//
//	back = false;
//	BackA = 0.0f;
//
//	ShotTime = 5.0f;
//	ShotPause = 60.0f;
//	ReloadTime = 0.0f;
//
//	Ammo = 5;
//	AmmoPerClip = -1;
//	MaxAmmo = 50;
//	CurrClip = -1;
//
//	Damage[0] = 80.0f;
//	Damage[1] = 120.0f;
//
//	Name = "Phazer";
//
//	Time = 0.0f;
//
//	Model = modelManager.Get( "phazer-model.glm" );
//
//	inited = true;
//}
//
//void wePhazer::Update( const float fTD )
//{
//	Rotate( fTD );
//
//	if( InHand )
//	{
//		Shake[0] = ( sin( Rot / 20.0f ) / 10.0f );
//		Shake[1] = ( cos( Rot / 10.0f ) / 20.0f );
//		if( fire )
//		{
//			if( !back )
//			{
//				Shake[0] = 0.0f;
//				Shake[1] = 0.0f;
//				Vector3f temp;
//				temp = RayCast( Owner->Pos, Owner->Vector, 0.5f, *pGLevel );
//				temp = (temp - Pos).Normalize();
//
//				float damage = this->Damage[0] + float( rand() % int( this->Damage[1] - this->Damage[0]) );
//
//				CBullet* Bull = new CBullRay( Owner, damage, Pos, temp );
//				BManager.AddBullet( Bull );
//
//				back = true;
//				Ammo--;
//			}
//			else
//			{
//				Time += fTD * GUI.GetSpeed();
//				if( Time >= ShotPause * 0.2f )
//				{
//					BackA -= ( 2.0f / ( ShotPause * 0.8f )) * GUI.GetSpeed() * fTD;
//					if( Time >= ShotPause )
//					{
//						BackA = 0.0f;
//						Time = 0.0f;
//						fire = false;
//						back = false;
//					}
//				}
//				else
//				{
//					BackA += ( 2.0f / ( ShotPause * 0.2f ) ) * GUI.GetSpeed() * fTD;
//				}
//			}
//		}
//
//	}
//}
//
//void wePhazer::Render()
//{
//	// Zachowujemy aktualn¹ Macierz
//	glPushMatrix();
//	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
//	if( !InHand )
//	{
//		glTranslatef( Pos.X, 0.0f, Pos.Z );
//
//		glTranslatef( 0.0f, -3.0f, 0.0f );
//		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
//		Model->CallObject( 0 );
//	}
//	else
//	{
//		glLoadIdentity();
//
//		switch( hand )
//		{
//		case GAME_RIGHT_HAND :
//			glTranslatef( 1.5f, -1.5f, -3.5f );
//			break;
//		case GAME_LEFT_HAND :
//			glTranslatef( -1.5f, -1.5f, -3.5f );
//			break;
//		case GAME_CENTER_HAND :
//			glTranslatef( 0.0f, -1.5f, -3.5f );
//			break;
//		default:
//			glTranslatef( 1.5f, -1.5f, -3.5f );
//			break;
//		}
//
//		if( back )
//			glTranslatef( 0.0f, 0.0f, BackA );
//
//		glTranslatef( Shake[0], Shake[1], 0.0f );
//
//		Model->CallObject( 0 );
//	}
//
//	// Przywracamy Macierz
//	glPopMatrix();
//}
//
//void wePhazer::Shot()
//{
//	if( Ammo == 0 )
//		return;
//	if( !back )
//		fire = true;
//}
//
///*	MINIGUN	*/
//weMiniGun::weMiniGun() :
//	CWeapon(WEAPON_TYPE::MINIGUN)
//{
//}
//
//void weMiniGun::Init( CModelManager& modelManager )
//{
//	if( inited ) 
//		return;
//
//	Rot2 = 0.0f;
//
//	ShotTime = 0.0001f;
//	ShotPause = 1.0f;
//	ReloadTime = 0.0f;
//
//	Ammo = 1000;
//	AmmoPerClip = -1;
//	MaxAmmo = 6000;
//	CurrClip = -1;
//
//	Damage[0] = 0.5f;
//	Damage[1] = 1.0f;
//
//	Name = "MiniGun";
//
//	back = false;
//	BackA = 0.0f;
//	Time = 0.0f;
//
//	Model = modelManager.Get( "minigun-model.glm" );
//
//	inited = true;
//}
//
//void weMiniGun::Update( const float fTD )
//{
//	Rotate( fTD );
//
//	if( InHand )
//	{
//		if( fire )
//		{
//			if( Rot2 < 15.0f )
//			{
//				Rot2 += GUI.GetSpeed() * fTD;
//			}
//			else Rot2 = 15.0f;
//
//			if( BackA == 1.0f && Rot2 == 15.0f )
//			{
//				Time += GUI.GetSpeed() * fTD;
//			}
//
//			if( Time >= ShotPause )
//			{
//				Time = 0.0f;
//				Shake[0] = 0.0f;
//				Shake[1] = 0.0f;
//				Vector3f temp;
//				temp = RayCast( Owner->Pos, Owner->Vector, 6.0f, *pGLevel );
//				temp = (temp - Pos).Normalize();
//
//				CBullet* Bull = new CBullet( Owner, Damage[0] * 0.1f, Pos, temp, 10.0f ); 
//				Bull->Visible = false;
//				
//				BManager.AddBullet( Bull );
//				Ammo -= 10;
//			}
//			if( BackA < 1.0f )
//			{
//				BackA += ( 0.5f / 14.0f ) * GUI.GetSpeed() * fTD;
//				back = true;
//			}
//			else BackA = 1.0f;
//			fire = false;
//		}
//		else
//		{
//			Shake[0] = ( sin( Rot / 20.0f ) / 10.0f );
//			Shake[1] = ( cos( Rot / 10.0f ) / 20.0f );
//			if( Rot2 > 0.0f )
//			{
//				Rot2 -= GUI.GetSpeed() * fTD;
//			}
//			else Rot2 = 0.0f;
//
//			if( BackA > 0.0f )
//			{
//				BackA -= ( 0.5f / 14.0f ) * GUI.GetSpeed() * fTD;
//				back = false;
//			}
//			else BackA = 0.0f;
//
//		}
//	}
//}
//
//void weMiniGun::Render()
//{
//	glPushMatrix();
//	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
//	if( !InHand )
//	{
//		glTranslatef( Pos.X, 0.0f, Pos.Z );
//
//		glTranslatef( 0.0f, -3.0f, 0.0f );
//		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
//		Model->CallObject( 0 );
//	}
//	else
//	{
//		glLoadIdentity();
//
//		switch( hand )
//		{
//		case GAME_RIGHT_HAND :
//			glTranslatef( 1.5f, -1.5f, -3.5f );
//			break;
//		case GAME_LEFT_HAND :
//			glTranslatef( -1.5f, -1.5f, -3.5f );
//			break;
//		case GAME_CENTER_HAND :
//			glTranslatef( 0.0f, -1.5f, -3.5f );
//			break;
//		default:
//			glTranslatef( 1.5f, -1.5f, -3.5f );
//			break;
//		}
//
//		glTranslatef( Shake[0], Shake[1], BackA );
//
//		Model->CallObject( 1 );
//		glRotatef( Rot * Rot2, 0.0f, 0.0f, 1.0f );
//		Model->CallObject( 2 );
//	}
//
//	glPopMatrix();
//}
//
//void weMiniGun::Shot()
//{
//	if( Ammo < 0 && Ammo != -1 )
//		Ammo = 0;
//
//	if( Ammo == 0 )
//		return;
//
//	fire = true;
//}
//
//
//
//
//#include "WeaponBulletBomb.h"
//
///*	ATOM BOMB	*/
//weATOM_BOMBb::weATOM_BOMBb() :
//	CWeapon(WEAPON_TYPE::ATOM_BOMB)
//{
//}
//
//void weATOM_BOMBb::Init( CModelManager& modelManager )
//{
//	if( inited )
//		return;
//
//	ShotTime = 5.0f;
//	ShotPause = 10.0f;
//	ReloadTime = 0.0f;
//
//	Ammo = 1;
//	AmmoPerClip = -1;
//	MaxAmmo = 99;
//	CurrClip = -1;
//
//	puted = false;
//
//	Name = "NUKE";
//
//	ModelManager = &modelManager;
//	Model = modelManager.Get( "ATOM_BOMBb-model.glm" );
//
//	inited = true;
//}
//
//void weATOM_BOMBb::Update( const float fTD )
//{
//	Rotate( fTD );
//
//	if( InHand )
//	{
//		Shake[0] = sin( Rot / 20.0f ) / 10.0f;
//		Shake[1] = cos( Rot / 10.0f ) / 20.0f;
//		if( fire )
//		{
//			fire = false;
//			if( puted )
//				return;
//
//			if( Ammo == 0 )
//				return;
//
//			CBullBomb* Bull = new CBullBomb( Owner, 10.0f, Pos, 5.0f, *ModelManager );
//			BManager.AddBullet( Bull );
//
//			Ammo--;
//			puted = true;
//			return;
//		}
//		puted = false;
//	}
//}
//
//void weATOM_BOMBb::Render()
//{
//	// Zachowujemy aktualn¹ Macierz
//	glPushMatrix();
//	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
//	if( !InHand )
//	{
//		glTranslatef( Pos.X, 0.0f, Pos.Z );
//
//		glTranslatef( 0.0f, -3.0f, 0.0f );
//		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
//		Model->CallObject( 0 );
//	}
//	else if( !puted && Ammo != 0 )
//	{
//		glLoadIdentity();
//
//		switch( hand )
//		{
//		case GAME_RIGHT_HAND :
//			glTranslatef( 1.5f, -1.5f, -3.5f );
//			break;
//		case GAME_LEFT_HAND :
//			glTranslatef( -1.5f, -1.5f, -3.5f );
//			break;
//		case GAME_CENTER_HAND :
//			glTranslatef( 0.0f, -1.5f, -3.5f );
//			break;
//		default:
//			glTranslatef( 1.5f, -1.5f, -3.5f );
//			break;
//		}
//
//		glTranslatef( Shake[0], Shake[1], 0.0f );
//
//		Model->CallObject( 0 );
//	}
//
//	// Przywracamy Macierz
//	glPopMatrix();
//}
//
//void weATOM_BOMBb::Shot()
//{
//	if( Ammo == 0 )
//		return;
//
//	fire = true;
//}