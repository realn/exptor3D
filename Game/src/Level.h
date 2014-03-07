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
#pragma once

#include <string>	// Nag��wek od klasy �a�cucha znak�w
#include <fstream>	// Nag��wek od operacji we/wy.

#include "StrEx.h"
#include "ModelManager.h"

#include "defines.h"
#include "Log.h"
#include "gui.h"
#include "Render.h"	// patrz-> nag��wek Render.h i plik Render.cpp
#include "Texture.h" // patrz-> nag��wek Texture.h i plik Texture.cpp
#include "3dMath.h"	// patrz-> nag��wek 3dMath.h i plik 3dMath.cpp

#include "GameEnemy.h"
#include "Item.h"
#include "WeaponBullet.h"
#include "RenderList.h"
#include "UpdateList.h"
#include "CollisionManager.h"
#include "Scene.h"

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

class CLvlBlock;

class gameWLFlags
{
private:
	bool IsAllDead();
	bool IsInBlock();
	bool GotWeap();
	bool IsSelfDead();
	bool IsThingDead();
public:
	gameWLFlags();

	unsigned int	flags;
	unsigned int	BlockID;
	CLvlBlock*		Block;
	std::string		EnemyID;
	//CEnemy*			Enemy;
	unsigned int	WeapID;
	std::string		Text;
	float			Color[3];

	void VerifyFlags();
	bool CheckAllFlags();
	bool CheckOneFlag();
};

/*	KLASA CLvlBlock
	Przechowuje dane potrzebne
	do stworzenia, inicjalizacji
	i dzia�ani poszczeg�lnych
	"blok�w" na poziomie.
*/
class CLvlBlock
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

public:
	CLvlBlock();

	void	LoadWalls( const std::string& str );
};

/*	KLASA CLevel
	Obiekty tej klasy �aduj�
	pliki txt z poziomami, i
	tworz� ich wizualn� wersj�.
*/
class CLevel :
	public CScene
{
private:
	CModelManager&	ModelManager;
	CTexManager&	TexManager;

	CRenderList			RenderList;
	CUpdateList			UpdateList;
	CCollisionManager	CollisionMng;

	CTexture*		Tex[3];
	
	float blockWidth;
	float blockHeight;
	float blockDepth;

	// Informacje o liczbie wierszy i kolumn w poziomie gry
	unsigned int Rows;
	unsigned int Cols;

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
	std::vector<CLvlBlock> block;

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

	// Metody wykorzystywane przy wizualizacji
	void DrawWall( unsigned int wall );
	void DrawFloor();
	void DrawTop();

	// Metody inicjuj�ce
	void BuildVisual();
	void BuildPhysic();

public:
	// Konstruktor i destruktor
	CLevel( CTexManager& texManager, CModelManager& modelManager );
	~CLevel();

	void	Update( const float fTD );
	void	Render();

	const bool	AddEntity( ISceneEntity* pEntity ) override;
	const bool	RemoveEntity( ISceneEntity* pEntity ) override;

	std::string GetLevelName();

	unsigned int GetBlockCount();
	CLvlBlock* GetBlock( unsigned int i, unsigned int j ) const;
	CLvlBlock* GetBlock( Vector3f Pos ) const;
	CLvlBlock* GetBlock( unsigned int i ) const;

	const Vector3f GetBlockPos( const unsigned i ) const;
	const Vector3f GetBlockPos( const int x, const int y ) const;

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

private:
	void	OnDeleteEntity( ISceneEntity* pEntity ) override;

	const bool	ParseCoords(const std::string& str, int& x, int& y);
	const bool	ParseItem(const std::string& str, int& x, int& y, ITEM_TYPE& type, std::vector<std::string>& params);
	const bool	LoadHeader( std::fstream& stream );
	const bool	LoadTextures( std::fstream& stream );
	const bool	LoadWalls( std::fstream& stream );
	const bool	LoadItemList( std::fstream& stream );
};

extern bool TestCollBlock( CDynamic* Dum, CLvlBlock* Block, bool testthing = false );
extern const Vector3f RayCast( const Vector3f& pos, const Vector3f& vector, const float step, const CLevel& level );
extern bool TestCollDum( CDynamic* Dum, CDynamic* Dum2 );
extern bool IsCollOnRay( Vector3f V1, Vector3f V2, int Steps = 10 );

extern CLevel* pGLevel;
