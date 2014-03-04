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
	elementy wsp�lne dla
	wszystkich klas.
*/
class weWeapon : public CDynamic
{
	/*	�eby by� dost�p do tych zmiennych
		w klasach dziedzicz�czych, umie�ci�em
		je w grupie protected.
	*/
protected:	
	// To jest stopie� obrotu, kiedy bro� le�y na ziemi
	float Rot;
	// To jest stopie� dr�enia w r�kach broni
	float Shake[2];
	// Typ Broni ( patrz -> definicje sta�ych )
	unsigned int type;

	/*	Okre�la ile zadaje bro� obra�e�.
		Damage[0] - minimum
		Damage[1] - maximum
	*/
	float Damage[2];

	// Lista wy�wietlania, gdy bro� le�y na ziemi.
	GLModel	*Model;

	CPlayer* Owner;

	//	Amunicja og�lna ( -1 dla niesko�czonej liczby naboi )
	int Ammo;
	//	Maksymalna amunicja
	int MaxAmmo;
	//	Amunicja na jeden magazynek ( -1, gdy bro� nie ma magazynk�w )
	int AmmoPerClip;
	//	Amunicja w aktualnym magazynku
	int CurrClip;

	// D�ugo�� jednego strza�u
	float ShotTime;
	// Pauza mi�dzy strza�ami
	float ShotPause;
	// Czas potrzebny na prze�adowanie magazynku
	float ReloadTime;

	// Czy bro� zosta�a zainicjowana
	bool inited;
	// Czy bro� jest w r�kach?
	bool InHand;
	// W Kt�rej r�ce trzyma bro�?
	unsigned int hand;
	// Czy bro� aktualnie strzela
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

	// Funkcja obracaj�ca bro� ( animacja raczej wsp�lna dla wszystkich broni );
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

/*	KLASA BRONI -> KLASA PI�Y	*/
class weSaw : public weWeapon
{
private:
public:
	weSaw();
	// Funckja inicjuj�ca
	void Init();
	void Update( const float fTD ) override;
	void Render();

	// Funkcja wywo�ywana podczas strza�u
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
	S�u�y do zarz�dzania broni� le��c� na ziemi
	na poziomie. z tej klasy g��wnie korzysta gracz.
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
