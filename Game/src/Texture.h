/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Texture.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klas odpowiedzalnych za tekstury.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
/*	Po co zmieni�em tutaj nazwe?
	�eby nie by�o konflikt�w z
	nag��wkiem ekspert'a :)
*/
#pragma once

#include <string>	// Nag��wek od klasy �a�cucha znak�w
#include <vector>

#define GLEW_STATIC
#include <gl/glew.h>

// Klasa do zarz�dzania jedn� tekstur�
class CTexture
{
private:
	// tablica trzymaj�ca adresy pami�ci tekstury ( jest 5 poziom�w jako�ci )
	unsigned int texture;

	std::string file;
	// zmienna dla ori�tacji czy tektrura jest gotowa
	bool loaded;

public:
	// Konstruktor i Destruktor
	CTexture();
	CTexture( std::string filename );
	virtual ~CTexture();

	// Funkcja �aduj�ca obraz z pliku TGA
	virtual bool LoadTGA( std::string filename );

	// Funkcja aktywuj�ca tekstur�
	virtual void Activate( unsigned int tex = 2 );

	// Metoda zwraca nazwe aktualnie za�adowanego pliku
	virtual std::string GetFile();
};

/*	Klasa zarz�daj�ca wieloma teksturami
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