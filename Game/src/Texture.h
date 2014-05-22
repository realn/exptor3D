/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Texture.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klas odpowiedzalnych za tekstury.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
/*	Po co zmieni³em tutaj nazwe?
	¯eby nie by³o konfliktów z
	nag³ówkiem ekspert'a :)
*/
#pragma once

#include <string>	// Nag³ówek od klasy ³añcucha znaków
#include <vector>

#define GLEW_STATIC
#include <gl/glew.h>

// Klasa do zarz¹dzania jedn¹ tekstur¹
class CTexture
{
private:
	// tablica trzymaj¹ca adresy pamiêci tekstury ( jest 5 poziomów jakoœci )
	unsigned int texture;

	std::string file;
	// zmienna dla oriêtacji czy tektrura jest gotowa
	bool loaded;

public:
	// Konstruktor i Destruktor
	CTexture();
	CTexture( std::string filename );
	virtual ~CTexture();

	// Funkcja ³aduj¹ca obraz z pliku TGA
	virtual bool LoadTGA( std::string filename );

	// Funkcja aktywuj¹ca teksturê
	virtual void Activate( unsigned int tex = 2 );

	// Metoda zwraca nazwe aktualnie za³adowanego pliku
	virtual std::string GetFile();
};

/*	Klasa zarz¹daj¹ca wieloma teksturami
*/
class CTexManager
{
private:
	const std::string	DataDir;
	std::vector<CTexture*> List;

public:
	CTexManager( const std::string& strDataDir );
	~CTexManager();

	CTexture* Get( std::string filename );
	void AddTexture( CTexture* Tex );
	void DeleteTexture( unsigned int index );
	CTexture* GetTexture( unsigned int index );
	void Clear();
};