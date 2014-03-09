#pragma once

#include "3dMath.h"
#include "Model.h"
#include "ModelManager.h"
#include "GamePlayer.h"

enum class WEAPON_TYPE
{
	UNKNOWN			= -1,
	SAW				= 0,
	PISTOL			= 1,
	MINIPHAZER		= 3,
	MINIGUN			= 4,
	ROCKET_LUNCHER	= 5,
	PICK_A_BOO		= 6,
	PHAZER			= 7,
	MINE			= 8,
	ATOM_BOMB		= 9,
};

enum class WEAPON_STATE
{
	UNKNOWN		= -1,
	READY		= 0,
	FIRING		= 1,
	RECOIL		= 2,
};

enum class WEAPON_HAND
{
	CENTER	= 0,
	LEFT	= 1,
	RIGHT	= 2,
};

/*	KLASA BRONI
	To jest tylko klasa do 
	dziedziczenia i zawiera
	elementy wspólne dla
	wszystkich klas.
*/
class CWeapon
{
protected:	
	// Typ Broni ( patrz -> definicje sta³ych )
	const WEAPON_TYPE Type;
	WEAPON_STATE	State;
	WEAPON_HAND		Hand;

	Vector3f	Pos;

	// To jest stopieñ dr¿enia w rêkach broni
	float		Shake;
	Vector3f	ShakeRadius;
	Vector3f	ShakeSpeed;

	/*	Okreœla ile zadaje broñ obra¿eñ.
	*/
	float DamageMin;
	float DamageMax;

	std::string Name;

	// Lista wyœwietlania, gdy broñ le¿y na ziemi.
	CModel	*Model;

	CPlayer* Owner;

	//	Amunicja ogólna ( -1 dla nieskoñczonej liczby naboi )
	int Ammo;
	//	Maksymalna amunicja
	unsigned MaxAmmo;

	// D³ugoœæ w czasie jednego strza³u
	float ShotTime;
	// Pauza miêdzy strza³ami
	float ShotPause;

	float Time;

	// Czy broñ jest w rêkach?
	bool InHand;

public:
	CWeapon( const WEAPON_TYPE type, const std::string& name );
	virtual ~CWeapon();

	const std::string	GetName() const;
	const WEAPON_TYPE	GetType() const;
	const WEAPON_STATE	GetState() const;

	const unsigned	GetMaxAmmo() const;
	const int		GetAmmo() const;

	// Funkcja podnoszenia broni
	void	PickUp( CPlayer& Player, const unsigned ammo );

	virtual void Update( const float fTD );
	virtual void Render();
	virtual void Shot();

	void SetAmmo( int ammo );
	bool ModAmmo( int ammo );

	bool GetHave();

protected:
	const float		GenDamage() const;
	const Vector3f	GenPos() const;
	const Vector3f	GenWorldPos( const Vector3f pos ) const;
	const Vector3f	GenShakePos() const;
	const Vector3f	CorrectByHandPos( const Vector3f& pos ) const;

	virtual void	OnShot();
	virtual void	OnFired();
};

extern const WEAPON_TYPE	ParseWeapon( const std::string& str );

///*	KLASA BRONI -> KLASA PI£Y	*/
//class weSaw : public CWeapon
//{
//private:
//public:
//	weSaw();
//
//	// Funckja inicjuj¹ca
//	void Init( CModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//
//	// Funkcja wywo³ywana podczas strza³u
//	void Shot();
//};
//
///*	KLASA BRONI -> KLASA PISTOLETU	*/
//class wePistol : public CWeapon
//{
//private:
//	float	Time;
//	bool	Reloading;
//	bool	back;
//	float	BackA;
//
//public:
//	wePistol();
//
//	// Inicjalizacja
//	void Init( CModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//
//	void Shot();
//};
//
///*	KLASA BRONI -> KLASA PHAZER'A	*/
//class weMiniPhazer : public CWeapon
//{
//private:
//	// Tekstury
//	float Time;
//	bool back;
//	float BackA;
//
//public:
//	weMiniPhazer();
//
//	// Inicjalizacja
//	void Init( CModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//	void Shot();
//};
//
///*	KLASA BRONI -> KLASA PHAZER'A	*/
//class wePhazer : public CWeapon
//{
//private:
//	// Tekstury
//	float Time;
//	bool back;
//	float BackA;
//public:
//	wePhazer();
//
//	// Inicjalizacja
//	void Init( CModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//	void Shot();
//};
//
///*	KLASA BRONI -> KLASA MINIGUN'A	*/
//class weMiniGun : public CWeapon
//{
//private:
//	float Rot2;
//	bool back;
//	float BackA;
//	float Time;
//public:
//	weMiniGun();
//
//	void Init( CModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//	void Shot();
//};

///*	KLASA BRONI -> KLASA BOMBY ATOMOWEJ	*/
//class weATOM_BOMBb : public CWeapon
//{
//private:
//	CModelManager* ModelManager;
//	float Time;
//	bool armed;
//	bool puted;
//	Vector3f PutPos;
//
//public:
//	weATOM_BOMBb();
//
//	void Init( CModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//	void Shot();
//};