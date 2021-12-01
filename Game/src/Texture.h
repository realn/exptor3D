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

#include <CBGL/Texture.h>

#include "core_object.h"

// Klasa do zarz¹dzania jedn¹ tekstur¹
class CTexture : public core::Object
{
public:
	// Konstruktor i Destruktor
	CTexture();
	CTexture( std::string filePath );
	~CTexture() override;

	bool load( std::string filePath );

	bool isLoaded() const { return loaded; }

	// Funkcja aktywuj¹ca teksturê
	virtual void Activate( unsigned int tex = 2 );

	// Metoda zwraca nazwe aktualnie za³adowanego pliku
	virtual std::string GetFile();

protected:
	virtual cb::string getLogName() const override;

private:
	std::unique_ptr<cb::gl::Texture> texture;

	cb::string file = L"-";
	// zmienna dla oriêtacji czy tektrura jest gotowa
	bool loaded = false;
};

/*	Klasa zarz¹daj¹ca wieloma teksturami
*/
class CTexManager : core::Object
{
public:
	CTexManager( const std::string& strDataDir );
	~CTexManager() override;

	CTexture* Get( std::string filename );

private:
	using textureptr_t = std::unique_ptr<CTexture>;
	using textures_t = std::vector<textureptr_t>;

	void addTexture( std::unique_ptr<CTexture> texture );

	const std::string	DataDir;
	textures_t textures;
};
