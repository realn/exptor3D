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
#ifndef	_TEXTURE_SPC_H
#define	_TEXTURE_SPC_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>	// Nag³ówek od operacji na plikach
#include <string>	// Nag³ówek od klasy ³añcucha znaków
#include <vector>
#include "Log.h"

// Klasa do zarz¹dzania jedn¹ tekstur¹
class ioTexture
{
private:
	// tablica trzymaj¹ca adresy pamiêci tekstury ( jest 5 poziomów jakoœci )
	unsigned int texture[3];
	std::string file;

	// zmienna dla oriêtacji czy tektrura jest gotowa
	bool loaded;
public:
	// Konstruktor i Destruktor
	ioTexture();
	ioTexture( std::string filename );
	virtual ~ioTexture();

	// Funkcja ³aduj¹ca obraz z pliku TGA
	virtual bool LoadTGA( std::string filename );

	// Funkcja aktywuj¹ca teksturê
	virtual void Activate( unsigned int tex = 2 );

	// Funkcja zwalniaj¹ca pamiêæ
	virtual void Free();

	// Metoda zwraca nazwe aktualnie za³adowanego pliku
	virtual std::string GetFile();
};

/*	Klasa zarz¹daj¹ca wieloma teksturami
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