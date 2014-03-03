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


/*	KLASA BONUSÓW	*/
weBonus::weBonus()
{
	CanDelete = false;
	Model = NULL;
	type = BONUS_TYPE_UNKNOWN;
}

void weBonus::Render()
{
	glPushMatrix();
	glTranslatef( Pos.X, Pos.Y, Pos.Z );
	glRotatef( rot, 0.0f, 1.0f, 0.0f );
	Model->CallObject( 0 );
	glPopMatrix();
}

void weBonus::Update( const float fTD )
{
	rot += fTD * GUI.GetSpeed();
}

unsigned int weBonus::GetType()
{
	return type;
}

/*	BONUS - AMUNICJA	*/
void weAmmo::Init(unsigned int weaptype, unsigned int ammocount, std::string modelfile) 
{
	WeapType = weaptype;
	AmmoCount = ammocount;
	Model = GLMManager.Get( modelfile );
	type = BONUS_TYPE_AMMO;
}

unsigned int weAmmo::GetWeapType()
{
	return WeapType;
}

unsigned int weAmmo::GetAmmoCount()
{
	return AmmoCount;
}

/*	BONUS - ZDROWIE	*/
void weHealth::Init(float health, std::string modelfile)
{
	HealthAdd = health;
	Model = GLMManager.Get( modelfile );
	type = BONUS_TYPE_HEALTH;
}

float weHealth::GetHealth()
{
	return HealthAdd;
}

/*	BONUS - PANCERZ	*/
void weArmor::Init(float armor, std::string modelfile)
{
	ArmorAdd = armor;
	Model = GLMManager.Get( modelfile );
	type = BONUS_TYPE_ARMOR;
}

float weArmor::GetArmor()
{
	return ArmorAdd;
}

/*	MENAD¯ER BONUSÓW	*/
weBonusManager::weBonusManager()
{	}

weBonusManager::~weBonusManager()
{
	if( Count() != 0 )
		Clear();
}

void weBonusManager::AddBonus(weBonus *Bonus)
{
	List.push_back( Bonus );
}

weBonus* weBonusManager::GetBonus(unsigned int index)
{
	if( index >= Count() )
		return NULL;
	
	return List[index];
}

void weBonusManager::DeleteBonus(unsigned int index)
{
	if( index >= Count() )
		return;

	delete List[index];
	List.erase( List.begin() + index );
}

unsigned int weBonusManager::Count()
{
	return List.size();
}

void weBonusManager::Clear()
{
	if( Count() == 0 )
		return;

	int i;
	for( i = Count()-1; i >= 0; i-- )
	{
		DeleteBonus( i );
	}
}

void weBonusManager::Update( CPlayer *Player, const float fTD )
{
	if( Count() == 0 || Player == NULL )
		return;

	int i; weBonus* Bonus;
	for( i = Count()-1; i >= 0; i-- )
	{
		Bonus = GetBonus( i );

		if( mathDistSq( Bonus->Pos, Player->NextPos ) > POW( 90.0f ) )
			continue;

		Bonus->Update( fTD );
		Player->TestBonus( Bonus );

		if( Bonus->CanDelete )
			DeleteBonus( i );
	}
}

void weBonusManager::Render()
{
	unsigned int i;
	weBonus* Bonus;
	for( i = 0; i < List.size(); i++ )
	{
		Bonus = List[i];
		if( mathDistSq( Bonus->Pos, MainPlayer.NextPos ) > POW( 90.0f ) )
			continue;
		Bonus->Render();
	}
	if( GUI.GetReflection() )
	{
		glPushMatrix();
		glTranslatef( 0.0f, -10.0f, 0.0f );
		glScalef( 1.0f, -1.0f, 1.0f );
		glDisable( GL_CLIP_PLANE0 );
		glEnable( GL_CLIP_PLANE1 );
		glFrontFace( GL_CW );
		for( i = 0; i < List.size(); i++ )
		{
			Bonus = List[i];
			if( mathDistSq( Bonus->Pos, MainPlayer.NextPos ) > POW( 90.0f ) )
				continue;
			Bonus->Render();
		}
		glFrontFace( GL_CCW );
		glDisable( GL_CLIP_PLANE1 );
		glEnable( GL_CLIP_PLANE0 );
		glPopMatrix();
	}

}

weBonusManager BonusMan;


/*	KLASA WEAPON
	Jest to klasa do dziedziczenia,
	zawiera wszystko co jest potrzebne
	do broni :)
*/

/*	KONSTRUKTOR	*/
weWeapon::weWeapon()
{
	Damage[0] = 0.0f;
	Damage[1] = 1.0f;
	type = 0;
	Ammo = 0;
	MaxAmmo = 0;
	AmmoPerClip = 0;
	inited = false;
	InHand = false;
	Rot = 0.0f;
	Shake[0] = 0.0f;
	Shake[1] = 0.0f;
	ShotTime = 0.0f;
	ShotPause = 0.0f;
	ReloadTime = 0.0f;
	hand = 0;
	fire = false;
	Model = NULL;
	Owner = NULL;
}

/*	DESTRUKTOR	*/
weWeapon::~weWeapon()
{
	Free();
}

/*	Jako, ¿e broñ na ziemi wygl¹da nudno
	to bêdziemy j¹ obracaæ. Efekt wygl¹da
	lepiej. :)
*/
void weWeapon::Rotate( const float fTD )
{
	Rot += fTD * 0.5f * GUI.GetSpeed();
	if( Rot >= 360.0f )
		Rot = 0.0f;
}

/*	Ta funkcja jest wywo³ywana, gdy
	jakaœ postaæ podniesie broñ.
*/
void weWeapon::PickUp( weWeapon* Weapon, CPlayer* Player )
{
	if( !InHand )
	{
		hand = Player->GetHand();
		InHand = true;
		Owner = Player;
		if( !inited )
			Init();
		GUI.SendMsg( "Podniosles: " + Weapon->Name, 4000, 10.0f, -1.0f, 1.5f, 1.5f, 1.0f, 0.5f, 0.5f );
	}
	else
	{
		if( Weapon->GetAllAmmo() == -1 )
			return;

		int PickAmmo = 0;
		if( this->Ammo + Weapon->GetAllAmmo() <= this->MaxAmmo )
		{
			PickAmmo = Weapon->GetAllAmmo();
			this->Ammo += Weapon->GetAllAmmo();
			Weapon->SetAmmo( 0 );
		}
		else
		{
			int temp = this->MaxAmmo - this->Ammo;
			Ammo += temp;
			PickAmmo = temp;
			Weapon->SetAmmo( Weapon->GetAllAmmo() - temp );
		}
		if( PickAmmo != 0 )
			GUI.SendMsg( "Podniosles " + guiIntToStr( PickAmmo ) + " amunicji.", 4000, 10.0f, -1.0f, 1.5f, 1.5f, 0.5f, 1.0f, 0.5f );		
	}
}

int weWeapon::GetAmmo()
{
	if( Ammo == -1 )
		return -1;
	if( AmmoPerClip != -1 )
		return ( Ammo % AmmoPerClip) + 1;
	else
		return Ammo;
}

int weWeapon::GetAllAmmo()
{
	return Ammo;
}

void weWeapon::SetAmmo( int ammo )
{
	Ammo = ammo;
}

bool weWeapon::ModAmmo( int ammo )
{
	if( !inited || Ammo == MaxAmmo || Ammo == -1 )
		return false;

	Ammo += ammo;
	if( Ammo > MaxAmmo )
		Ammo = MaxAmmo;

	return true;
}

int weWeapon::GetClip()
{
	if( AmmoPerClip == -1 )
		return -1;
	return ( Ammo / AmmoPerClip );
}

bool weWeapon::GetHave()
{
	return InHand;
}

unsigned int weWeapon::GetType()
{
	return type;
}

bool weWeapon::GetInited()
{
	return inited;
}

void weWeapon::Update( const float fTD )
{

}

void weWeapon::Render()
{

}

void weWeapon::Init()
{

}

void weWeapon::Shot()
{

}

void weWeapon::Free()
{
	if( !inited )
		return;

	Model = NULL;
}

#include "WeaponBulletSaw.h"
#include "WeaponBulletManager.h"

/*	PI£A :D
	Teraz zaczynaj¹ siê klasy broni.
	Ka¿da broñ ma odzieln¹ klas¹, by
	mia³a w³asny wygl¹d, w³aœciwoœci
	i animacje. Na pierwszy ogieñ idzie
	pi³a tarczowa ;D
*/

weSaw::weSaw()
{
}

/*	Metoda inicjuje pi³e, czyli
	Przypisuje wartoœci i tworzy jej
	wygl¹d. Mog³em zrobiæ jakiœ loader
	plików, ale to by siê zesz³o zbyt
	d³ugo :P 
*/
void weSaw::Init()
{
	if( inited ) 
		return;

	type = GAME_WEAP_SAW;
	/*	Najpierw inicjujemy zmienne.
		Te bêd¹ odpowiedzialne za
		animacje obracaj¹cej siê tarczy
		i przysuwania podczas ataku
	*/
	ShotTime = 0.0f;
	ShotPause = 0.1f;
	ReloadTime = 0.0f;
	/*	Teraz ustawiamy amunicje.
		Jako, ¿e broñ nie posiada
		amunicji jako takiej, to
		ustawiamy na -1 (nieskoñczone)
	*/
	Ammo = -1;
	MaxAmmo = -1;
	AmmoPerClip = -1;

	Damage[0] = 0.1f;
	Damage[1] = 0.5f;

	Name = "SAW";

	Model = GLMManager.Get( "Data/saw-model.glm" );

	inited = true;
}

/*	METODA ENGINE'A
	By zachowaæ jak¹œ dynamike
	doda³em tu parê bajerów: ruszaj¹ca
	siê broñ, przysuwanie przy strzale,
	powolny rozruch tarczy, itp. Lepiej
	to wygl¹da ni¿ statyczny model i s³owo
	"O, trafi³em..." ;)
*/
void weSaw::Update( const float fTD )
{
	//	Funkcja obracaj¹ca, dzia³a bez wzglêdu, czy broñ jest na ziemi, czy nie
	Rotate( fTD );

	// Sprawdzamy, czy broñ jest podnisiona
	if( !InHand ) 
		return;

	// Je¿eli broñ jest w rêku, to uruchamiamy dodatkowy kod.

	/*	Gdy strzelamy, broñ siê przysuwa do przeciwnika,
		a ostrze wiruje coraz szybciej. Przy okazji rêka siê
		nie trzêsie ( jak to dziwnie brzmi :P )
	*/
	if( fire ) 
	{
		if( ShotPause < 20.0f )
			ShotPause += 0.4f * fTD;
		if( ReloadTime > -1.0f )
			ReloadTime -= 0.2f * fTD;

		Shake[0] = 0.0f;
		Shake[1] = 0.0f;
		CBullSaw* bull = new CBullSaw( Owner, 0.5f, Owner->Pos );
		BManager.AddBullet( bull );
		fire = false;
	}
	/*	Gdy jednak nic siê niedzieje, ostrze zwalnia, a broñ
		wraca na miejsce. Przy okazji brón wykonuje delikatne
		"chwianie" siê powoduj¹c z³udzenie trzymaj¹cej jej rêki.
		*/
	else 
	{
		if( ShotPause > 0.3f )
			ShotPause -= 0.1f * fTD;
		if( ReloadTime < 0.0f )
			ReloadTime += 0.05f * fTD;

		Shake[0] = ( sinf( Rot / 10.0f ) / 20.0f );
		Shake[1] = ( cosf( Rot / 10.0f ) / 20.0f );
	}
	// Obracamy ostrze i gdy wykona³o pe³ny obrót, zerujemy
	ShotTime += ShotPause * GUI.GetSpeed() * fTD;
	if( ShotTime >= 360.0f )
		ShotTime = 0.0f;
}

/*	METODA RYSUJ¥CA
	Tu nastêpuje wyœwietlenie na ekranie. Nic wielkiego :P
*/
void weSaw::Render()
{
	// Zachowujemy aktualn¹ Macierz
	glPushMatrix();
	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
	if( !InHand )
	{
		glTranslatef( Pos.X, 0.0f, Pos.Z );

		glTranslatef( 0.0f, -3.0f, 0.0f );
		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
		Model->CallObject( 0 );
	}
	else
	{
		glLoadIdentity();

		switch( hand )
		{
		case GAME_RIGHT_HAND :
			glTranslatef( 1.5f, -1.5f, -5.0f );
			break;
		case GAME_LEFT_HAND :
			glTranslatef( -1.5f, -1.5f, -5.0f );
			break;
		case GAME_CENTER_HAND :
			glTranslatef( 0.0f, -1.5f, -5.0f );
			break;
		default:
			glTranslatef( 1.5f, -1.5f, -5.0f );
			break;
		}

		glTranslatef( Shake[0], Shake[1], ReloadTime * GUI.GetSpeed() );

		Model->CallObject( 1 );		

		glTranslatef( 0.0f, 0.0f, -2.0f );
		glRotatef( ShotTime, 0.0f, 1.0f, 0.0f );

		Model->CallObject( 2 );	
	}

	// Przywracamy Macierz
	glPopMatrix();
}

void weSaw::Shot()
{
	if( !inited )
		return;
	fire = true;
}

/*	PISTOLET	*/
void wePistol::Init()
{
	if( inited ) 
		return;

	type = GAME_WEAP_PISTOL;
	Reloading = false;
	back = false;
	BackA = 0.0f;

	ShotTime = 30.0f;
	ShotPause = 40.0f;
	ReloadTime = 80.0f;

	Ammo = 50;
	AmmoPerClip = -1;
	MaxAmmo = 999;
	CurrClip = -1;

	Damage[0] = 2.0f;
	Damage[1] = 3.0f;

	Name = "Pistol";

	Time = 0.0f;

	Model = GLMManager.Get( "Data/pistol-model.glm" );

	inited = true;
}

void wePistol::Update( const float fTD )
{
	Rotate( fTD );

	if( !InHand )
		return;

	if( back )
	{
		Time += 1.0f * GUI.GetSpeed() * fTD;
		BackA += 0.7f * GUI.GetSpeed() * fTD;
		if( Time >= ShotPause )
		{
			back = false;
			Time = 0.0f;
			BackA = 0.0f;
		}
	}

	if( fire ) 
	{
		if( !back )
		{
			back = true;
			Time = 0.0f;
			Ammo--;
			CurrClip--;

			Vector3f temp;
			temp = RayCast( Owner->NextPos, Owner->Vector, 2.0f, &GLevel );
			temp = temp - Pos;
			temp.Normalize();
			CBullet* Bull = new CBullet( Owner, Damage[0], Pos, temp, 10.0f );
			BManager.AddBullet( Bull );
		}
		Shake[0] = 0.0f;
		Shake[1] = 0.0f;
		fire = false;
	}
}

void wePistol::Render()
{
	// Zachowujemy aktualn¹ Macierz
	glPushMatrix();
	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
	if( !InHand )
	{
		glTranslatef( Pos.X, 0.0f, Pos.Z );

		glTranslatef( 0.0f, -3.0f, 0.0f );
		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
		Model->CallObject( 0 );
	}
	else
	{
		glLoadIdentity();

		switch( hand )
		{
		case GAME_RIGHT_HAND :
			glTranslatef( 1.5f, -1.5f, -5.0f );
			break;
		case GAME_LEFT_HAND :
			glTranslatef( -1.5f, -1.5f, -5.0f );
			break;
		case GAME_CENTER_HAND :
			glTranslatef( 0.0f, -1.5f, -5.0f );
			break;
		default:
			glTranslatef( 1.5f, -1.5f, -5.0f );
			break;
		}

		if( back )
			glRotatef( BackA, 1.0f, 0.0f, 0.0f );

		if( Reloading && !back)
		{
			glTranslatef( 0.0f, -Time / 50.0f, 0.0f );
			glRotatef( Time, 0.0f, 1.0f, 0.0f );
		}

		glTranslatef( Shake[0], Shake[1], 0.0f );

		Model->CallObject( 0 );		
	}

	// Przywracamy Macierz
	glPopMatrix();
}

void wePistol::Shot()
{
	if( back )
		return;

	if( Ammo > 0 || Ammo == -1 )
		fire = true;
}


#include "WeaponBulletRay.h"

/*	MINIPHAZER	*/
void weMiniPhazer::Init()
{
	if( inited ) 
		return;

	type = GAME_WEAP_MINIPZR;
	back = false;
	BackA = 0.0f;

	ShotTime = 5.0f;
	ShotPause = 60.0f;
	ReloadTime = 0.0f;

	Ammo = 20;
	AmmoPerClip = -1;
	MaxAmmo = 500;
	CurrClip = -1;

	Damage[0] = 40.0f;
	Damage[1] = 60.0f;

	Name = "MiniPhazer";

	Time = 0.0f;

	Model = GLMManager.Get( "Data/miniphazer-model.glm" );

	inited = true;
}

void weMiniPhazer::Update( const float fTD )
{
	Rotate( fTD );

	if( InHand )
	{
		Shake[0] = ( sin( Rot / 20.0f ) / 10.0f );
		Shake[1] = ( cos( Rot / 10.0f ) / 20.0f );
		if( fire )
		{
			if( !back )
			{
				Shake[0] = 0.0f;
				Shake[1] = 0.0f;
				
				Vector3f temp;
				temp = RayCast( Owner->Pos, Owner->Vector, 0.5f, &GLevel );
				temp = temp - Pos;
				temp.Normalize();

				float damage = this->Damage[0] + float( rand() % int( this->Damage[1] - this->Damage[0]) );

				CBullet* Bull = new CBullRay( Owner, damage, Pos, temp );
				BManager.AddBullet( Bull );

				back = true;
				Ammo--;
			}
			else
			{
				Time += fTD * GUI.GetSpeed();
				if( Time >= ShotPause * 0.2f )
				{
					BackA -= ( 1.0f / ( ShotPause * 0.8f )) * GUI.GetSpeed() * fTD;
					if( Time >= ShotPause )
					{
						BackA = 0.0f;
						Time = 0.0f;
						fire = false;
						back = false;
					}
				}
				else
				{
					BackA += ( 1.0f / ( ShotPause * 0.2f ) ) * GUI.GetSpeed() * fTD;
				}
			}
		}

	}
}

void weMiniPhazer::Render()
{
	// Zachowujemy aktualn¹ Macierz
	glPushMatrix();
	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
	if( !InHand )
	{
		glTranslatef( Pos.X, 0.0f, Pos.Z );

		glTranslatef( 0.0f, -3.0f, 0.0f );
		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
		Model->CallObject( 0 );
	}
	else
	{
		glLoadIdentity();

		switch( hand )
		{
		case GAME_RIGHT_HAND :
			glTranslatef( 1.5f, -1.5f, -4.5f );
			break;
		case GAME_LEFT_HAND :
			glTranslatef( -1.5f, -1.5f, -4.5f );
			break;
		case GAME_CENTER_HAND :
			glTranslatef( 0.0f, -1.5f, -4.5f );
			break;
		default:
			glTranslatef( 1.5f, -1.5f, -4.5f );
			break;
		}

		if( back )
			glTranslatef( 0.0f, 0.0f, BackA );

		glTranslatef( Shake[0], Shake[1], 0.0f );

		Model->CallObject( 0 );
	}

	// Przywracamy Macierz
	glPopMatrix();
}

void weMiniPhazer::Shot()
{
	if( Ammo == 0 )
		return;
	if( !back )
		fire = true;
}



/*	PHAZER	*/
void wePhazer::Init()
{
	if( inited ) 
		return;

	type = GAME_WEAP_PHAZER;
	back = false;
	BackA = 0.0f;

	ShotTime = 5.0f;
	ShotPause = 60.0f;
	ReloadTime = 0.0f;

	Ammo = 5;
	AmmoPerClip = -1;
	MaxAmmo = 50;
	CurrClip = -1;

	Damage[0] = 80.0f;
	Damage[1] = 120.0f;

	Name = "Phazer";

	Time = 0.0f;

	Model = GLMManager.Get( "Data/phazer-model.glm" );

	inited = true;
}

void wePhazer::Update( const float fTD )
{
	Rotate( fTD );

	if( InHand )
	{
		Shake[0] = ( sin( Rot / 20.0f ) / 10.0f );
		Shake[1] = ( cos( Rot / 10.0f ) / 20.0f );
		if( fire )
		{
			if( !back )
			{
				Shake[0] = 0.0f;
				Shake[1] = 0.0f;
				Vector3f temp;
				temp = RayCast( Owner->Pos, Owner->Vector, 0.5f, &GLevel );
				temp = temp - Pos;
				temp.Normalize();

				float damage = this->Damage[0] + float( rand() % int( this->Damage[1] - this->Damage[0]) );

				CBullet* Bull = new CBullRay( Owner, damage, Pos, temp );
				BManager.AddBullet( Bull );

				back = true;
				Ammo--;
			}
			else
			{
				Time += fTD * GUI.GetSpeed();
				if( Time >= ShotPause * 0.2f )
				{
					BackA -= ( 2.0f / ( ShotPause * 0.8f )) * GUI.GetSpeed() * fTD;
					if( Time >= ShotPause )
					{
						BackA = 0.0f;
						Time = 0.0f;
						fire = false;
						back = false;
					}
				}
				else
				{
					BackA += ( 2.0f / ( ShotPause * 0.2f ) ) * GUI.GetSpeed() * fTD;
				}
			}
		}

	}
}

void wePhazer::Render()
{
	// Zachowujemy aktualn¹ Macierz
	glPushMatrix();
	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
	if( !InHand )
	{
		glTranslatef( Pos.X, 0.0f, Pos.Z );

		glTranslatef( 0.0f, -3.0f, 0.0f );
		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
		Model->CallObject( 0 );
	}
	else
	{
		glLoadIdentity();

		switch( hand )
		{
		case GAME_RIGHT_HAND :
			glTranslatef( 1.5f, -1.5f, -3.5f );
			break;
		case GAME_LEFT_HAND :
			glTranslatef( -1.5f, -1.5f, -3.5f );
			break;
		case GAME_CENTER_HAND :
			glTranslatef( 0.0f, -1.5f, -3.5f );
			break;
		default:
			glTranslatef( 1.5f, -1.5f, -3.5f );
			break;
		}

		if( back )
			glTranslatef( 0.0f, 0.0f, BackA );

		glTranslatef( Shake[0], Shake[1], 0.0f );

		Model->CallObject( 0 );
	}

	// Przywracamy Macierz
	glPopMatrix();
}

void wePhazer::Shot()
{
	if( Ammo == 0 )
		return;
	if( !back )
		fire = true;
}

/*	MINIGUN	*/
void weMiniGun::Init()
{
	if( inited ) 
		return;

	type = GAME_WEAP_MINIGUN;
	Rot2 = 0.0f;

	ShotTime = 0.0001f;
	ShotPause = 1.0f;
	ReloadTime = 0.0f;

	Ammo = 1000;
	AmmoPerClip = -1;
	MaxAmmo = 6000;
	CurrClip = -1;

	Damage[0] = 0.5f;
	Damage[1] = 1.0f;

	Name = "MiniGun";

	back = false;
	BackA = 0.0f;
	Time = 0.0f;

	Model = GLMManager.Get( "Data/minigun-model.glm" );

	inited = true;
}

void weMiniGun::Update( const float fTD )
{
	Rotate( fTD );

	if( InHand )
	{
		if( fire )
		{
			if( Rot2 < 15.0f )
			{
				Rot2 += GUI.GetSpeed() * fTD;
			}
			else Rot2 = 15.0f;

			if( BackA == 1.0f && Rot2 == 15.0f )
			{
				Time += GUI.GetSpeed() * fTD;
			}

			if( Time >= ShotPause )
			{
				Time = 0.0f;
				Shake[0] = 0.0f;
				Shake[1] = 0.0f;
				Vector3f temp;
				temp = RayCast( Owner->Pos, Owner->Vector, 6.0f, &GLevel );
				temp = temp - Pos;
				temp.Normalize();

				CBullet* Bull = new CBullet( Owner, Damage[0] * 0.1f, Pos, temp, 10.0f ); 
				Bull->Visible = false;
				
				BManager.AddBullet( Bull );
				Ammo -= 10;
			}
			if( BackA < 1.0f )
			{
				BackA += ( 0.5f / 14.0f ) * GUI.GetSpeed() * fTD;
				back = true;
			}
			else BackA = 1.0f;
			fire = false;
		}
		else
		{
			Shake[0] = ( sin( Rot / 20.0f ) / 10.0f );
			Shake[1] = ( cos( Rot / 10.0f ) / 20.0f );
			if( Rot2 > 0.0f )
			{
				Rot2 -= GUI.GetSpeed() * fTD;
			}
			else Rot2 = 0.0f;

			if( BackA > 0.0f )
			{
				BackA -= ( 0.5f / 14.0f ) * GUI.GetSpeed() * fTD;
				back = false;
			}
			else BackA = 0.0f;

		}
	}
}

void weMiniGun::Render()
{
	glPushMatrix();
	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
	if( !InHand )
	{
		glTranslatef( Pos.X, 0.0f, Pos.Z );

		glTranslatef( 0.0f, -3.0f, 0.0f );
		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
		Model->CallObject( 0 );
	}
	else
	{
		glLoadIdentity();

		switch( hand )
		{
		case GAME_RIGHT_HAND :
			glTranslatef( 1.5f, -1.5f, -3.5f );
			break;
		case GAME_LEFT_HAND :
			glTranslatef( -1.5f, -1.5f, -3.5f );
			break;
		case GAME_CENTER_HAND :
			glTranslatef( 0.0f, -1.5f, -3.5f );
			break;
		default:
			glTranslatef( 1.5f, -1.5f, -3.5f );
			break;
		}

		glTranslatef( Shake[0], Shake[1], BackA );

		Model->CallObject( 1 );
		glRotatef( Rot * Rot2, 0.0f, 0.0f, 1.0f );
		Model->CallObject( 2 );
	}

	glPopMatrix();
}

void weMiniGun::Shot()
{
	if( Ammo < 0 && Ammo != -1 )
		Ammo = 0;

	if( Ammo == 0 )
		return;

	fire = true;
}


#include "WeaponBulletRocket.h"

/*	ROCKET LUNCHER	*/
void weRocketLuncher::Init()
{
	if( inited ) 
		return;

	type = GAME_WEAP_ROCKETLUN;
	back = false;
	BackA = 0.0f;

	ShotTime = 8.0f;
	ShotPause = 120.0f;
	ReloadTime = 0.0f;

	Ammo = 20;
	AmmoPerClip = -1;
	MaxAmmo = 100;
	CurrClip = -1;

	Damage[0] = 5.0f;
	Damage[1] = 15.0f;

	Name = "Wyrzutnia Rakiet";

	Time = 0.0f;

	Model = GLMManager.Get( "Data/rocketlun-model.glm" );

	inited = true;
}

void weRocketLuncher::Update( const float fTD )
{
	Rotate( fTD );

	if( InHand )
	{
		Shake[0] = ( sin( Rot / 20.0f ) / 10.0f );
		Shake[1] = ( cos( Rot / 10.0f ) / 20.0f );
		if( fire )
		{
			if( !back )
			{
				Shake[0] = 0.0f;
				Shake[1] = 0.0f;

				Vector3f temp;
				temp = RayCast( Owner->Pos, Owner->Vector, 0.5f, &GLevel );
				temp = temp - Pos;
				temp.Normalize();

				float damage = this->Damage[0] + float( rand() % int( this->Damage[1] - this->Damage[0]) );

				CBullet* Bull = new CBullRocket( Owner, damage, Pos, temp, 2.0f );
				BManager.AddBullet( Bull );

				back = true;
				Ammo--;
			}
			else
			{
				Time += GUI.GetSpeed() * fTD;
				if( Time >= ShotPause * 0.2f )
				{
					BackA -= ( 2.0f / ( ShotPause * 0.8f )) * GUI.GetSpeed() * fTD;
					if( Time >= ShotPause )
					{
						BackA = 0.0f;
						Time = 0.0f;
						fire = false;
						back = false;
					}
				}
				else
				{
					BackA += ( 2.0f / ( ShotPause * 0.2f ) ) * GUI.GetSpeed() * fTD;
				}
			}
		}

	}
}

void weRocketLuncher::Render()
{
	// Zachowujemy aktualn¹ Macierz
	glPushMatrix();
	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
	if( !InHand )
	{
		glTranslatef( Pos.X, 0.0f, Pos.Z );

		glTranslatef( 0.0f, -3.0f, 0.0f );
		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
		Model->CallObject( 0 );
	}
	else
	{
		glLoadIdentity();

		switch( hand )
		{
		case GAME_RIGHT_HAND :
			glTranslatef( 1.5f, -1.5f, -3.5f );
			break;
		case GAME_LEFT_HAND :
			glTranslatef( -1.5f, -1.5f, -3.5f );
			break;
		case GAME_CENTER_HAND :
			glTranslatef( 0.0f, -1.5f, -3.5f );
			break;
		default:
			glTranslatef( 1.5f, -1.5f, -3.5f );
			break;
		}

		if( back )
			glTranslatef( 0.0f, 0.0f, BackA );

		glTranslatef( Shake[0], Shake[1], 0.0f );

		glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );
		Model->CallObject( 0 );
	}

	// Przywracamy Macierz
	glPopMatrix();
}

void weRocketLuncher::Shot()
{
	if( Ammo == 0 )
		return;
	if( !back )
		fire = true;
}


#include "WeaponBulletBomb.h"

/*	ATOM BOMB	*/
void weAtomBomb::Init()
{
	if( inited )
		return;

	type = GAME_WEAP_ATOM_BOMB;

	ShotTime = 5.0f;
	ShotPause = 10.0f;
	ReloadTime = 0.0f;

	Ammo = 1;
	AmmoPerClip = -1;
	MaxAmmo = 99;
	CurrClip = -1;

	puted = false;

	Name = "NUKE";

	Model = GLMManager.Get( "Data/atombomb-model.glm" );

	inited = true;
}

void weAtomBomb::Update( const float fTD )
{
	Rotate( fTD );

	if( InHand )
	{
		Shake[0] = sin( Rot / 20.0f ) / 10.0f;
		Shake[1] = cos( Rot / 10.0f ) / 20.0f;
		if( fire )
		{
			fire = false;
			if( puted )
				return;

			if( Ammo == 0 )
				return;

			CBullBomb* Bull = new CBullBomb( Owner, 10.0f, Pos, 5.0f );
			BManager.AddBullet( Bull );

			Ammo--;
			puted = true;
			return;
		}
		puted = false;
	}
}

void weAtomBomb::Render()
{
	// Zachowujemy aktualn¹ Macierz
	glPushMatrix();
	// Rysujemy broñ, w zale¿noœci, czy ona jest na ziemi, czy nie
	if( !InHand )
	{
		glTranslatef( Pos.X, 0.0f, Pos.Z );

		glTranslatef( 0.0f, -3.0f, 0.0f );
		glRotatef( Rot, 0.0f, 1.0f, 0.0f );
		Model->CallObject( 0 );
	}
	else if( !puted && Ammo != 0 )
	{
		glLoadIdentity();

		switch( hand )
		{
		case GAME_RIGHT_HAND :
			glTranslatef( 1.5f, -1.5f, -3.5f );
			break;
		case GAME_LEFT_HAND :
			glTranslatef( -1.5f, -1.5f, -3.5f );
			break;
		case GAME_CENTER_HAND :
			glTranslatef( 0.0f, -1.5f, -3.5f );
			break;
		default:
			glTranslatef( 1.5f, -1.5f, -3.5f );
			break;
		}

		glTranslatef( Shake[0], Shake[1], 0.0f );

		Model->CallObject( 0 );
	}

	// Przywracamy Macierz
	glPopMatrix();
}

void weAtomBomb::Shot()
{
	if( Ammo == 0 )
		return;

	fire = true;
}
