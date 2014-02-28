/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Level.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Definicja klas i struktur do zarz�dzania poziomem
		gry. Zawiera tak�e funkcje odpowiedzialne za fizyke
		oraz logike gry.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "defines.h"
#include "Log.h"
#include "gui.h"
#include "Render.h"	// patrz-> nag��wek Render.h i plik Render.cpp
#include "3dMath.h"	// patrz-> nag��wek 3dMath.h i plik 3dMath.cpp
#include "Game.h"
#include <string>	// Nag��wek od klasy �a�cucha znak�w
#include <stdio.h>	// Nag��wek od operacji we/wy.

#include "glm.h"
#include "Texture.h"

/*	DEFINICJE MAKROWE
	Te synonimy s� dla u�atwienia.
	Dzi�ki nim poprawia si� czytelno��
	kodu.
*/
#define	LEV_WALL_TOP	1
#define	LEV_WALL_BOTTOM	2
#define	LEV_WALL_LEFT	4
#define	LEV_WALL_RIGHT	8

#define	WLFLAG_ALL_ENEM_DEAD	1
#define WLFLAG_GO_TO_BLOCK		2
#define	WLFLAG_GET_WEAP			4
#define WLFLAG_SELF_DEAD		8
#define WLFLAG_THING_DEAD		16

class gameWLFlags
{
private:
	bool IsAllDead();
	bool IsInBlock();
	bool GotWeap();
	bool IsSelfDead();
	bool IsThingDead();
public:
	unsigned int	flags;
	unsigned int	BlockID;
	gameBlockInfo*	Block;
	std::string		EnemyID;
	gameEnemy*		Enemy;
	unsigned int	WeapID;
	std::string		Text;
	float			Color[3];

	void VerifyFlags();
	bool CheckAllFlags();
	bool CheckOneFlag();
};

/*	KLASA gameBlockInfo
	Przechowuje dane potrzebne
	do stworzenia, inicjalizacji
	i dzia�ani poszczeg�lnych
	"blok�w" na poziomie.
*/
class gameBlockInfo
{
public:
	// Wektor trzymaj�cy rogi sektora
	Vector3f Corner[4];
	// Wektor trzyma rogi, o kt�re gracz ma si� zatrzymywa� (rogi �cian)
	Vector3f TCorner[4];
	// P�aszczyzna ka�dej �ciany (do wykrywania kolizji)
	Planef WPlane[4];
	// Flagi �cian jakie posiada dany blok
	unsigned int walls;
	// Liczba rog�w o kt�re mo�e zawadzi� gracz ( od 0 do 4, wi��e si� z TCorner )
	unsigned int CornerCount;

	// Dane dla broni i amunicji ( -1 jak nie ma �adnej w danym bloku )
	std::string StatObj;
};

/*	KLASA gameLevel
	Obiekty tej klasy �aduj�
	pliki txt z poziomami, i
	tworz� ich wizualn� wersj�.
*/
class gameLevel
{
private:
	GLModelManager& mdlManager;
	ioTexManager&	texManager;

	ioTexture *Tex[3];
	
	// Informacje o liczbie wierszy i kolumn w poziomie gry
	unsigned int rows;
	unsigned int cols;

	unsigned int PlayerStartBlock;
	unsigned int PlayerStartAngle;

	unsigned int WeapCount;
	unsigned int BonusCount;
	unsigned int StatObjCount;
	unsigned int EnemyCount;
	unsigned int EnemyTypeCount;
	std::string* EnemyType;

	unsigned int Version;

	// Tablica trzymaj�ca dane wyci�gni�te z pliku
	gameBlockInfo* block;

	int AllWin;
	int AllLose;

	// Listy wy�wietlania
	unsigned int Floor;
	unsigned int Top;
	unsigned int Wall;
	unsigned int Reflect;
	// Czy poziom ju� jest za�adowany
	bool loaded;

	gameWLFlags WinFlags;
	gameWLFlags LoseFlags;

	// Nazwa poziomu
	std::string LevName;
	// Plik ostatnio za�adowanego poziomu
	std::string file;

	// Metoda pobiera jedna linie z pliku
	std::string GetString( FILE* fp );
	std::string GetParamStr( const std::string &str );

	// Metody wykorzystywane przy wizualizacji
	void DrawWall( unsigned int wall );
	void DrawFloor();
	void DrawTop();

	// Metody inicjuj�ce
	void BuildVisual();
	void BuildPhisic();
public:
	// Konstruktor i destruktor
	gameLevel( GLModelManager& mdlManager, ioTexManager& texManager );
	~gameLevel();

	std::string GetLevelName();

	unsigned int GetBlockCount();
	gameBlockInfo* GetBlock( unsigned int i, unsigned int j );
	gameBlockInfo* GetBlock( Vector3f Pos );
	gameBlockInfo* GetBlock( unsigned int i );
	Vector3f GetBlockPos( unsigned int i );
	bool GetLoaded();
	unsigned int GetEnemyCount();
	unsigned int GetSObjCount();

	void CheckWLFlags();

	// Metoda �adujaca poziom z pliku txt
	bool LoadLevel( const std::string &filename );

	// Ko�cowa metoda inicjuj�ca
	void InitLevel();

	// Metoda rysuj�ca
	void DrawLevel();
	void DrawAllWall();
	void DrawAllFloor();
	void DrawAllTop();

	void DrawReflect();

	// Metoda czyszcz�ca pami��
	void Free();
};

extern bool TestCollBlock( Dummy* Dum, gameBlockInfo* Block, bool testthing = false );
extern Vector3f RayCast( Vector3f Pos, Vector3f Veloc, float Step, gameLevel* Level );
extern bool TestCollDum( Dummy* Dum, Dummy* Dum2 );
extern bool IsCollOnRay( Vector3f V1, Vector3f V2, int Steps = 10 );

#endif