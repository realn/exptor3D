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

#include <CBGL/Texture.h>

#include "core_object.h"

namespace gfx {
	class Texture : public core::Object {
	public:
		// Konstruktor i Destruktor
		Texture();
		Texture(std::string filePath);
		~Texture() override;

		bool load(std::string filePath);

		bool isLoaded() const { return loaded; }

		// Funkcja aktywuj�ca tekstur�
		virtual void Activate(unsigned int tex = 2);

		// Metoda zwraca nazwe aktualnie za�adowanego pliku
		virtual std::string GetFile();

	protected:
		virtual cb::string getLogName() const override;

	private:
		std::unique_ptr<cb::gl::Texture> texture;

		cb::string file = L"-";
		// zmienna dla ori�tacji czy tektrura jest gotowa
		bool loaded = false;
	};
}