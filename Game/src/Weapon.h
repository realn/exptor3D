#pragma once

#include "3dMath.h"
#include "gfx_Model.h"
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
	elementy wsp�lne dla
	wszystkich klas.
*/
class CWeapon
{
protected:	
	// Typ Broni ( patrz -> definicje sta�ych )
	const WEAPON_TYPE Type;
	WEAPON_STATE	State;
	WEAPON_HAND		Hand;

	glm::vec3	Pos;

	// To jest stopie� dr�enia w r�kach broni
	float		Shake;
	glm::vec3	ShakeRadius;
	glm::vec3	ShakeSpeed;

	/*	Okre�la ile zadaje bro� obra�e�.
	*/
	float DamageMin;
	float DamageMax;

	std::string Name;

	// Lista wy�wietlania, gdy bro� le�y na ziemi.
	gfx::Model	*Model;

	CPlayer* Owner;

	//	Amunicja og�lna ( -1 dla niesko�czonej liczby naboi )
	int Ammo;
	//	Maksymalna amunicja
	unsigned MaxAmmo;

	// D�ugo�� w czasie jednego strza�u
	float ShotTime;
	// Pauza mi�dzy strza�ami
	float ShotPause;

	float Time;

	// Czy bro� jest w r�kach?
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
	const glm::vec3	GenPos() const;
	const glm::vec3	GenWorldPos( const glm::vec3 pos ) const;
	const glm::vec3	GenShakePos() const;
	const glm::vec3	CorrectByHandPos( const glm::vec3& pos ) const;

	virtual void	OnShot();
	virtual void	OnFired();
};

extern const WEAPON_TYPE	ParseWeapon( const std::string& str );

///*	KLASA BRONI -> KLASA PI�Y	*/
//class weSaw : public CWeapon
//{
//private:
//public:
//	weSaw();
//
//	// Funckja inicjuj�ca
//	void Init( gfx::ModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//
//	// Funkcja wywo�ywana podczas strza�u
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
//	void Init( gfx::ModelManager& modelManager );
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
//	void Init( gfx::ModelManager& modelManager );
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
//	void Init( gfx::ModelManager& modelManager );
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
//	void Init( gfx::ModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//	void Shot();
//};

///*	KLASA BRONI -> KLASA BOMBY ATOMOWEJ	*/
//class weATOM_BOMBb : public CWeapon
//{
//private:
//	gfx::ModelManager* ModelManager;
//	float Time;
//	bool armed;
//	bool puted;
//	glm::vec3 PutPos;
//
//public:
//	weATOM_BOMBb();
//
//	void Init( gfx::ModelManager& modelManager );
//	void Update( const float fTD ) override;
//	void Render();
//	void Shot();
//};