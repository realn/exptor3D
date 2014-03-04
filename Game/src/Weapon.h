#pragma once

#include "3dMath.h"
#include "GameEntity.h"
#include "glm.h"
#include "GamePlayer.h"

#define	GAME_WEAP_SAW		0
#define	GAME_WEAP_PISTOL	1
#define	GAME_WEAP_MINIPZR	3
#define	GAME_WEAP_MINIGUN	4
#define	GAME_WEAP_ROCKETLUN	5
#define	GAME_WEAP_PICKABOO	6
#define	GAME_WEAP_PHAZER	7
#define	GAME_WEAP_MINE		8
#define	GAME_WEAP_ATOM_BOMB	9



/*	KLASA BRONI
	To jest tylko klasa do 
	dziedziczenia i zawiera
	elementy wspólne dla
	wszystkich klas.
*/
class weWeapon : public CDynamic
{
	/*	¯eby by³ dostêp do tych zmiennych
		w klasach dziedzicz¹czych, umieœci³em
		je w grupie protected.
	*/
protected:	
	// To jest stopieñ obrotu, kiedy broñ le¿y na ziemi
	float Rot;
	// To jest stopieñ dr¿enia w rêkach broni
	float Shake[2];
	// Typ Broni ( patrz -> definicje sta³ych )
	unsigned int type;

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
	weWeapon();
	~weWeapon();

	// Funkcja podnoszenia broni
	void PickUp( weWeapon* Weapon, CPlayer* Player );

	virtual void Init();
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
	unsigned int GetType();
	bool GetInited();
};

/*	KLASA BRONI -> KLASA PI£Y	*/
class weSaw : public weWeapon
{
private:
public:
	weSaw();
	// Funckja inicjuj¹ca
	void Init();
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
	// Inicjalizacja
	void Init();
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
	// Inicjalizacja
	void Init();
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
	// Inicjalizacja
	void Init();
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
	void Init();
	void Update( const float fTD ) override;
	void Render();
	void Shot();
};

class weRocketLuncher : public weWeapon
{
private:
	float Time;
	bool back;
	float BackA;
public:
	void Init();
	void Update( const float fTD ) override;
	void Render();
	void Shot();
};

/*	KLASA BRONI -> KLASA BOMBY ATOMOWEJ	*/
class weAtomBomb : public weWeapon
{
private:
	float Time;
	bool armed;
	bool puted;
	Vector3f PutPos;
public:
	void Init();
	void Update( const float fTD ) override;
	void Render();
	void Shot();
};

/*	KLASA gameWeaponManager
	S³u¿y do zarz¹dzania broni¹ le¿¹c¹ na ziemi
	na poziomie. z tej klasy g³ównie korzysta gracz.
	( i nikt poza nim :) )
*/
class gameWeaponManager
{
private:
	std::vector <weWeapon*> List;
public:
	gameWeaponManager();
	~gameWeaponManager();
	void Update( CPlayer* Players, int PlayerCount, const float fTD );
	void Render();

	void AddWeapon( weWeapon* weapon );
	void DeleteWeapon( unsigned int index );
	weWeapon* GetWeapon( unsigned int index );
	void Clear();
	void LoadFromLevel();
};

extern gameWeaponManager WManager;
