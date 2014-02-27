#ifndef _WEAPON_H_
#define _WEAPON_H_

#include	"defines.h"
#include	"gui.h"
#include	"Render.h"
#include	"Texture.h"
#include	"glm.h"
#include	"3dMath.h"
#include	"Special.h"
#include	"Game.h"

/*	KLASY POCISK�W	*/
class weBullet : public Dummy
{
protected:
	unsigned int Type;
	Vector3f Veloc;
	bool DoDelete;
public:
	float Damage;
	gameThing* Owner;

	weBullet();

	virtual void Init( Vector3f pos, Vector3f veloc, float speed );

	virtual void DoEngine();
	virtual float DoTest( Dummy* Dum, float Armor = 0.0f );
	virtual void DoDraw();

	virtual void OnDelete();

	bool CanDelete;
	bool Visible;
};

class weBullSaw : public weBullet
{
private:
public:
	void Init( Vector3f pos, Vector3f veloc, float speed );
	void DoEngine();
	float DoTest( Dummy* Dum, float Armor = 0.0f );
};
// Promie�
class weBullRay : public weBullet
{
private:
	Vector3f Temp;
	float Time;
public:
	void Init( Vector3f pos, Vector3f veloc, float speed );
	
	void DoEngine();
	float DoTest( Dummy* Dum, float Armor = 0.0f );
	void DoDraw();
};

// Rakieta
class weBullRocket : public weBullet
{
private:
	GLModel* Model;
	float Angle;
	float Sec;
public:
	void Init( Vector3f pos, Vector3f veloc, float speed );

	void DoEngine();
	void DoDraw();
	void OnDelete();
};

// Wybuch
class weBullExplode : public weBullet
{
private:
	float thisPower;
	float Step;
public:
	float Power;

	void Init( Vector3f pos, Vector3f veloc, float speed );

	void DoEngine();
	float DoTest( Dummy* Dum, float Armor = 0.0f );
	void DoDraw();
};

// Bomba
class weBullBomb : public weBullet
{
private:
	float BoomTime;
	float ThisTime;
	GLModel* Model;
public:
	void Init( Vector3f pos, Vector3f veloc, float speed );
	void DoEngine();
	float DoTest( Dummy* Dum, float Armor = 0.0f );
	void DoDraw();
};
// Menager pocisk�w
class weBulletManager
{
private:
	std::vector<weBullet*> List;
public:
	void AddBullet( weBullet* bullet );
	void DeleteBullet( unsigned int index );
	weBullet* GetBullet( unsigned int index );

	float DoTest( Dummy* Dum, float Armor = 0.0f );
	void DoEngine();
	void DoDraw();

	void Clear();
};


extern weBulletManager BManager;

class weBonus : public Dummy
{
protected:
	float rot;
	unsigned int type;
	GLModel* Model;
public:
	weBonus();
	bool CanDelete;
	virtual void DoDraw();
	virtual void DoEngine();
	unsigned int GetType();
};

class weAmmo : public weBonus
{
private:
	unsigned int WeapType;
	unsigned int AmmoCount;
public:
	void Init( unsigned int weaptype, unsigned int ammocount, std::string modelfile );
	unsigned int GetWeapType();
	unsigned int GetAmmoCount();
};

class weHealth : public weBonus
{
private:
	float HealthAdd;
public:
	void Init( float health, std::string modelfile );
	float GetHealth();
};

class weArmor : public weBonus
{
private:
	float ArmorAdd;
public:
	void Init( float armor, std::string modelfile );
	float GetArmor();
};

class weBonusManager
{
private:
	std::vector<weBonus*> List;
public:
	weBonusManager();
	~weBonusManager();

	void AddBonus( weBonus* Bonus );
	weBonus* GetBonus( unsigned int index );
	void DeleteBonus( unsigned int index );

	void DoEngine( gamePlayer* Player );
	void DoDraw();

	unsigned int Count();
	void Clear();
};

extern weBonusManager BonusMan;

/*	KLASA BRONI
	To jest tylko klasa do 
	dziedziczenia i zawiera
	elementy wsp�lne dla
	wszystkich klas.
*/
class weWeapon : public Dummy
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

	gamePlayer* Owner;

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
	void PickUp( weWeapon* Weapon, gamePlayer* Player );

	virtual void Init();
	virtual void DoEngine();
	virtual void DoDraw();
	virtual void Shot();
	virtual void Free();

	// Funkcja obracaj�ca bro� ( animacja raczej wsp�lna dla wszystkich broni );
	void Rotate();

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
	void DoEngine();
	void DoDraw();

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
	void DoEngine();
	void DoDraw();

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
	void DoEngine();
	void DoDraw();
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
	void DoEngine();
	void DoDraw();
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
	void DoEngine();
	void DoDraw();
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
	void DoEngine();
	void DoDraw();
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
	void DoEngine();
	void DoDraw();
	void Shot();
};

#endif