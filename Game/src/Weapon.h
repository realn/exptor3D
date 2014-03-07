#pragma once

#include "3dMath.h"
#include "glm.h"
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

/*	KLASA BRONI
	To jest tylko klasa do 
	dziedziczenia i zawiera
	elementy wspólne dla
	wszystkich klas.
*/
class weWeapon : 
	public CDynamic
{
	/*	¯eby by³ dostêp do tych zmiennych
		w klasach dziedzicz¹czych, umieœci³em
		je w grupie protected.
	*/
protected:	
	// Typ Broni ( patrz -> definicje sta³ych )
	const WEAPON_TYPE Type;

	// To jest stopieñ obrotu, kiedy broñ le¿y na ziemi
	float Rot;
	// To jest stopieñ dr¿enia w rêkach broni
	float Shake[2];


	/*	Okreœla ile zadaje broñ obra¿eñ.
		Damage[0] - minimum
		Damage[1] - maximum
	*/
	float Damage[2];

	// Lista wyœwietlania, gdy broñ le¿y na ziemi.
	GLModel	*Model;

	CPlayer* Owner;

	//	Amunicja ogólna ( -1 dla nieskoñczonej liczby naboi )
	int Ammo;
	//	Maksymalna amunicja
	int MaxAmmo;
	//	Amunicja na jeden magazynek ( -1, gdy broñ nie ma magazynków )
	int AmmoPerClip;
	//	Amunicja w aktualnym magazynku
	int CurrClip;

	// D³ugoœæ jednego strza³u
	float ShotTime;
	// Pauza miêdzy strza³ami
	float ShotPause;
	// Czas potrzebny na prze³adowanie magazynku
	float ReloadTime;

	// Czy broñ zosta³a zainicjowana
	bool inited;
	// Czy broñ jest w rêkach?
	bool InHand;
	// W Której rêce trzyma broñ?
	unsigned int hand;
	// Czy broñ aktualnie strzela
	bool fire;

public:
	std::string Name;
	// Konstruktor i destruktor
	weWeapon(const WEAPON_TYPE type);
	virtual ~weWeapon();

	const WEAPON_TYPE GetType() const;

	// Funkcja podnoszenia broni
	void	PickUp( CPlayer& Player, const unsigned ammo, CModelManager& modelManager );

	virtual void Init( CModelManager& modelManager );
	virtual void Update( const float fTD );
	virtual void Render();
	virtual void Shot();
	virtual void Free();

	// Funkcja obracaj¹ca broñ ( animacja raczej wspólna dla wszystkich broni );
	void Rotate( const float fTD );

	int GetAmmo();
	int GetAllAmmo();
	void SetAmmo( int ammo );
	bool ModAmmo( int ammo );
	int GetClip();
	bool GetHave();
	bool GetInited();
};

extern const WEAPON_TYPE	ParseWeapon( const std::string& str );

/*	KLASA BRONI -> KLASA PI£Y	*/
class weSaw : public weWeapon
{
private:
public:
	weSaw();

	// Funckja inicjuj¹ca
	void Init( CModelManager& modelManager );
	void Update( const float fTD ) override;
	void Render();

	// Funkcja wywo³ywana podczas strza³u
	void Shot();
};

/*	KLASA BRONI -> KLASA PISTOLETU	*/
class wePistol : public weWeapon
{
private:
	float	Time;
	bool	Reloading;
	bool	back;
	float	BackA;

public:
	wePistol();

	// Inicjalizacja
	void Init( CModelManager& modelManager );
	void Update( const float fTD ) override;
	void Render();

	void Shot();
};

/*	KLASA BRONI -> KLASA PHAZER'A	*/
class weMiniPhazer : public weWeapon
{
private:
	// Tekstury
	float Time;
	bool back;
	float BackA;

public:
	weMiniPhazer();

	// Inicjalizacja
	void Init( CModelManager& modelManager );
	void Update( const float fTD ) override;
	void Render();
	void Shot();
};

/*	KLASA BRONI -> KLASA PHAZER'A	*/
class wePhazer : public weWeapon
{
private:
	// Tekstury
	float Time;
	bool back;
	float BackA;
public:
	wePhazer();

	// Inicjalizacja
	void Init( CModelManager& modelManager );
	void Update( const float fTD ) override;
	void Render();
	void Shot();
};

/*	KLASA BRONI -> KLASA MINIGUN'A	*/
class weMiniGun : public weWeapon
{
private:
	float Rot2;
	bool back;
	float BackA;
	float Time;
public:
	weMiniGun();

	void Init( CModelManager& modelManager );
	void Update( const float fTD ) override;
	void Render();
	void Shot();
};

class weROCKET_LUNCHERcher : public weWeapon
{
private:
	CModelManager* ModelManager;
	float Time;
	bool back;
	float BackA;

public:
	weROCKET_LUNCHERcher();

	void Init( CModelManager& modelManager );
	void Update( const float fTD ) override;
	void Render();
	void Shot();
};

/*	KLASA BRONI -> KLASA BOMBY ATOMOWEJ	*/
class weATOM_BOMBb : public weWeapon
{
private:
	CModelManager* ModelManager;
	float Time;
	bool armed;
	bool puted;
	Vector3f PutPos;

public:
	weATOM_BOMBb();

	void Init( CModelManager& modelManager );
	void Update( const float fTD ) override;
	void Render();
	void Shot();
};