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
#ifndef	_TEXTURE_SPC_H
#define	_TEXTURE_SPC_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>	// Nag��wek od operacji na plikach
#include <string>	// Nag��wek od klasy �a�cucha znak�w
#include <vector>
#include "Log.h"

// Klasa do zarz�dzania jedn� tekstur�
class ioTexture
{
private:
	// tablica trzymaj�ca adresy pami�ci tekstury ( jest 5 poziom�w jako�ci )
	unsigned int texture[3];
	std::string file;

	// zmienna dla ori�tacji czy tektrura jest gotowa
	bool loaded;
public:
	// Konstruktor i Destruktor
	ioTexture();
	ioTexture( std::string filename );
	virtual ~ioTexture();

	// Funkcja �aduj�ca obraz z pliku TGA
	virtual bool LoadTGA( std::string filename );

	// Funkcja aktywuj�ca tekstur�
	virtual void Activate( unsigned int tex = 2 );

	// Funkcja zwalniaj�ca pami��
	virtual void Free();

	// Metoda zwraca nazwe aktualnie za�adowanego pliku
	virtual std::string GetFile();
};

/*	Klasa zarz�daj�ca wieloma teksturami
*/
class ioTexManager
{
private:
	std::vector<ioTexture*> List;
public:
	ioTexManager();
	~ioTexManager();
	ioTexture* Get( std::string filename );
	void AddTexture( ioTexture* Tex );
	void DeleteTexture( unsigned int index );
	ioTexture* GetTexture( unsigned int index );
	void Clear();
};

extern ioTexManager TManager;

#endif