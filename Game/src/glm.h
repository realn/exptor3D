/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	glm.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klas do ³adowania i zarz¹dzania
		modelami z plików GLM.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#ifndef _GLM_H_
#define _GLM_H_

#include "Texture.h"	// Naglówek z klasa ioTexture
#include <fstream>

// Numer wersji loader'a
#define		GLM_VERSION		100

/*	G³ówna klasa modeli OpenGL
*/
class GLModel
{
private:
	ioTexManager& texManager;

	// Lista tekstur
	std::vector<ioTexture*>	Textures;
	// Lista obiektów
	unsigned int List;
	// Liczba obiektów
	unsigned int ListCount;
	// Lista klatek
	unsigned int Frame;
	// Liczba klatek
	unsigned int FrameCount;
	// Aktualna klatka
	unsigned int CurrFrame;
	// Animacja od klatki
	unsigned int FromFrame;
	// Animacja do Klatki
	unsigned int ToFrame;
	// Czy model jest za³adowany?
	bool loaded;
	// Czy ma animacje?
	bool animation;
	// Czy aktualnie trwa animacja
	bool playing;
	// Ostatni za³adowany plik
	std::string file;
	// Obiekt do rysowania kwadryk ( tymczasowy, ale jest tu, by by³ dostêpny w ca³ej klasie )
	GLUquadric* obj;


public:
	GLModel( ioTexManager& texManager );
	~GLModel();

	bool LoadModel( std::string filename );
	void Free();
	std::string GetFile();
	unsigned int GetObjCount();

	void CallObject( unsigned int index );
	void DoDrawAnim( unsigned int index );
	void DoEngineAnim();
	void PlayAnim( unsigned int fromframe, unsigned int toframe, bool canskip = false );

private:
	std::string GetString( std::fstream& fileStream );
	std::string NoSpace( const std::string &str );
	void ParseGLCommand( const std::string &fullstr );
	bool GetParams( const std::string &str, int from, std::vector<std::string>& param, const std::string &Com );
	int GetConst( const std::string &str, const std::string &Com );

	bool ReadHeader( std::fstream& fileStream, unsigned& texCount );
	bool ReadAnimHeader( std::fstream& fileStream );
};


class GLModelManager
{
private:
	ioTexManager&	texManager;
	std::vector<GLModel*> List;

public:
	GLModelManager( ioTexManager& texManager );
	~GLModelManager();

	GLModel* Get( std::string filename );
	void AddModel( GLModel* Model );
	GLModel* GetModel( unsigned int index );
	void DeleteModel( unsigned int index );
	void Clear();
};

#endif