/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Level.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Definicja klas i struktur do zarz¹dzania poziomem
		gry. Zawiera tak¿e funkcje odpowiedzialne za fizyke
		oraz logike gry.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <string>	// Nag³ówek od klasy ³añcucha znaków
#include <fstream>	// Nag³ówek od operacji we/wy.

#include "StrEx.h"
#include "ModelManager.h"

#include "Log.h"
#include "Render.h"	// patrz-> nag³ówek Render.h i plik Render.cpp
#include "Texture.h" // patrz-> nag³ówek Texture.h i plik Texture.cpp
#include "3dMath.h"	// patrz-> nag³ówek 3dMath.h i plik 3dMath.cpp

#include "Item.h"
#include "WeaponProjectile.h"
#include "RenderList.h"
#include "UpdateList.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "GamePlayer.h"

/*	DEFINICJE MAKROWE
	Te synonimy s¹ dla u³atwienia.
	Dziêki nim poprawia siê czytelnoœæ
	kodu.
*/

enum class LEV_SURFACE : unsigned
{
	NONE		= 0x00000000,
	WALL_FAR	= 0x00000001,
	WALL_NEAR	= 0x00000002,
	WALL_LEFT	= 0x00000004,
	WALL_RIGHT	= 0x00000008,
	CEILING		= 0x00000010,
	FLOOR		= 0x00000020,
};

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

class CLvlMesh
{
public:
	std::vector<Vector3f>		Verts;
	std::vector<Vector2f>		TexCoords;
	std::vector<unsigned short>	Indices;

	void	AddVertex( const Vector3f& vert, const Vector2f& texCoord )
	{
		for( unsigned i = 0; i < Verts.size(); i++ )
		{
			if( Verts[i] == vert && TexCoords[i] == texCoord )
			{
				Indices.push_back( i );
				return;
			}
		}

		Indices.push_back( static_cast<unsigned short>(Verts.size()) );
		Verts.push_back( vert );
		TexCoords.push_back( texCoord );
	}
	void	Clear()
	{
		Verts.clear();
		TexCoords.clear();
		Indices.clear();
	}

	void	Render()
	{
		if( Indices.empty() )
			return;

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		glVertexPointer( 3, GL_FLOAT, 0, &Verts[0] );
		glTexCoordPointer( 2, GL_FLOAT, 0, &TexCoords[0] );

		glDrawElements( GL_QUADS, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_SHORT, &Indices[0] );

		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
};

/*	KLASA CLvlBlock
	Przechowuje dane potrzebne
	do stworzenia, inicjalizacji
	i dzia³ani poszczególnych
	"bloków" na poziomie.
*/
class CLvlBlock
{
public:
	unsigned Row;
	unsigned Col;

	Vector3f	Origin;

	// Wektor trzymaj¹cy rogi sektora
	Vector3f Corner[4];
	// Wektor trzyma rogi, o które gracz ma siê zatrzymywaæ (rogi œcian)
	Vector3f TCorner[4];
	// P³aszczyzna ka¿dej œciany (do wykrywania kolizji)
	Planef WPlane[4];
	// Flagi œcian jakie posiada dany blok
	unsigned int walls;
	// Liczba rogów o które mo¿e zawadziæ gracz ( od 0 do 4, wi¹¿e siê z TCorner )
	unsigned int CornerCount;

	// Dane dla broni i amunicji ( -1 jak nie ma ¿adnej w danym bloku )
	std::string StatObj;

public:
	CLvlBlock();

	void	Set( const unsigned row, const unsigned col, const float width, const float height, const float depth );

	void	LoadWalls( const std::string& str );

	void	GetVerts( std::vector<Vector3f>& verts );
};

/*	KLASA CLevel
	Obiekty tej klasy ³aduj¹
	pliki txt z poziomami, i
	tworz¹ ich wizualn¹ wersjê.
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

	CPlayer	Player;
	std::vector<CItem*>	Items;
	
	float blockWidth;
	float blockHeight;
	float blockDepth;

	// Informacje o liczbie wierszy i kolumn w poziomie gry
	unsigned int Rows;
	unsigned int Cols;

	unsigned int PlayerStartBlock;
	unsigned int PlayerStartAngle;

	unsigned int Version;

	// Tablica trzymaj¹ca dane wyci¹gniête z pliku
	std::vector<CLvlBlock> block;

	CLvlMesh	Floor;
	CLvlMesh	Ceiling;
	CLvlMesh	Wall;

	// Czy poziom ju¿ jest za³adowany
	bool loaded;

	// Nazwa poziomu
	std::string LevName;

	// Plik ostatnio za³adowanego poziomu
	std::string file;

public:
	// Konstruktor i destruktor
	CLevel( CTexManager& texManager, CModelManager& modelManager );
	~CLevel();

	void	Update( const float fTD );
	void	Render();

	const bool	AddEntity( ISceneEntity* pEntity ) override;
	const bool	RemoveEntity( ISceneEntity* pEntity ) override;

	const CCollisionManager&	GetCollisionManager() const;
	CPlayer&	GetPlayer();

	const std::string GetName() const;

	const unsigned int GetBlockCount() const;
	CLvlBlock* GetBlock( const unsigned col, const unsigned row ) const;
	CLvlBlock* GetBlock( Vector3f Pos ) const;
	CLvlBlock* GetBlock( unsigned int i ) const;

	const Vector3f GetBlockPos( const unsigned i ) const;
	const Vector3f GetBlockPos( const int x, const int y ) const;

	const unsigned	GetBlockIndex( const unsigned col, const unsigned row ) const;

	const bool IsLoaded() const;

	// Metoda ³adujaca poziom z pliku txt
	const bool LoadLevel( const std::string &filename );

	// Metoda rysuj¹ca
	void RenderLevel();
	void RenderWalls();
	void RenderFloors();
	void RenderCeilings();

	// Metoda czyszcz¹ca pamiêæ
	void Free();

private:
	void BuildVisual();
	void BuildPhysic();

	void	OnDeleteEntity( ISceneEntity* pEntity ) override;

	const bool	ParseCoords(const std::string& str, int& x, int& y);
	const bool	ParseItem(const std::string& str, int& x, int& y, ITEM_TYPE& type, std::vector<std::string>& params);
	const bool	LoadHeader( std::fstream& stream );
	const bool	LoadTextures( std::fstream& stream );
	const bool	LoadWalls( std::fstream& stream );
	const bool	LoadItemList( std::fstream& stream );

	void	GenSurfaceVerts( const LEV_SURFACE surf, std::vector<Vector3f>& verts );
};

extern CLevel* pGLevel;
