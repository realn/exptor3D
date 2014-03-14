/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Model.h
Autor:	Real_Noname (real_noname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Zawiera definicje klas do �adowania i zarz�dzania
		modelami z plik�w GLM.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include "Texture.h"	// Nagl�wek z klasa CTexture
#include <fstream>

// Numer wersji loader'a
#define		GLM_VERSION		100

/*	G��wna klasa modeli OpenGL
*/
class CModel
{
private:
	// Lista tekstur
	std::vector<CTexture*>	Textures;
	// Lista obiekt�w
	unsigned int List;
	// Liczba obiekt�w
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
	// Czy model jest za�adowany?
	bool loaded;
	// Czy ma animacje?
	bool animation;
	// Czy aktualnie trwa animacja
	bool playing;
	// Ostatni za�adowany plik
	std::string file;
	// Obiekt do rysowania kwadryk ( tymczasowy, ale jest tu, by by� dost�pny w ca�ej klasie )
	GLUquadric* obj;

public:
	CModel();
	~CModel();

	bool LoadModel( CTexManager& texManager, const std::string& filename );
	void Free();
	const std::string GetFile() const;
	unsigned int GetObjCount();

	void RenderObject( unsigned int index );
	void RenderAnim( unsigned int index );
	void UpdateAnim();
	void PlayAnim( unsigned int fromframe, unsigned int toframe, bool canskip = false );

private:
	std::string NoSpace( const std::string &str );
	void ParseGLCommand( const std::string &fullstr );
	bool GetParams( const std::string &str, int from, std::vector<std::string>& param, const std::string &Com );
	int GetConst( const std::string &str, const std::string &Com );

	const bool	ReadHeader( std::fstream& fileStream );
	const bool	ReadAnimHeader( std::fstream& fileStream );
	const bool	ReadTextures( std::fstream& fileStream, CTexManager& texManager );
	const bool	ReadModel( std::fstream& fileStream, const unsigned modelIndex );

	CModel( const CModel& ){
		throw std::exception( "COPY ERROR" );
	}
	void operator=(const CModel&)
	{
		throw std::exception( "ASSIGN ERROR" );
	}

};
