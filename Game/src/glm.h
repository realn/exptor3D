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

#include "gui.h"		// Nag³ówek z dostêpem do klast Logger
#include "Texture.h"	// Naglówek z klasa ioTexture

// Numer wersji loader'a
#define		GLM_VERSION		100

/*	G³ówna klasa modeli OpenGL
*/
class GLModel
{
protected:
	// Lista tekstur
	ioTexture** Tex;
	// Liczba tekstur
	unsigned int TexCount;
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
	bool Animation;
	// Czy aktualnie trwa animacja
	bool playing;
	// Ostatni za³adowany plik
	std::string file;
	// Obiekt do rysowania kwadryk ( tymczasowy, ale jest tu, by by³ dostêpny w ca³ej klasie )
	GLUquadric* obj;

	std::string GetString( FILE* fp );
	std::string NoSpace( const std::string &str );
	void ParseGLCommand( const std::string &fullstr );
	bool GetParams( const std::string &str, int from, std::string* param, const int paramCount, const std::string &Com );
	int GetConst( const std::string &str, const std::string &Com );

	bool ReadHeader( FILE* fp );
	bool ReadAnimHeader( FILE* fp );
public:
	GLModel();
	~GLModel();

	bool LoadModel( std::string filename );
	virtual void Free();
	std::string GetFile();
	unsigned int GetObjCount();

	void CallObject( unsigned int index );
	void DoDrawAnim( unsigned int index );
	void DoEngineAnim();
	void PlayAnim( unsigned int fromframe, unsigned int toframe, bool canskip = false );
};


class GLModelManager
{
private:
	std::vector<GLModel*> List;
public:
	GLModelManager();
	~GLModelManager();
	GLModel* Get( std::string filename );
	void AddModel( GLModel* Model );
	GLModel* GetModel( unsigned int index );
	void DeleteModel( unsigned int index );
	void Clear();
};

extern GLModelManager GLMManager;

#endif