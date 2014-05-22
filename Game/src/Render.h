/*--------------------------------------------
  Nazwa:	Nag��wek Render.h
  Autor:	Real_Noname (real_noname@wp.pl)
  WWW:      www.coderulers.prv.pl
  Data:		07-05-2005 21:21
  Opis:		Funkcje i klasy potrzebne do sterowania
			OpenGL.
--------------------------------------------*/

#pragma once

//#include <windows.h>	// Nag��wek funkcji Windows
//#include <gl/gl.h>		// Nag��wek funkcji OpenGL
//#include <gl/glu.h>		// Nag��wek funkcji pomocniczych do OpenGL
#include <string>		// Nag�owek do mainpulacji �a�cuchem znak�w

#define GLEW_STATIC
#include <gl/glew.h>

#include "RenderWindow.h"

enum RENDER_INFO{
	RENDER_VERSION	= 0,
	RENDER_RENDERER	= 1,
	RENDER_VENDOR	= 2,
	RENDER_EXTLIST	= 3,
};

class CRender
{
private:
	int		colorBits;			// Bity kolor�w
	int		depthBits;
	int		stencilBits;

	std::string rndInfo[4];	// Trzyma informacje o danej Maszynie
	/*	RndInfo[0] - Wersja OpenGL
		RndInfo[1] - Sterownik Karty Grafiki
		RndInfo[2] - Firma, kt�ra wmontowa�a OpenGL do karty ( np. NVDIA, ATI )
		RndInfo[3] - Lista rozszerze� dost�pnych na danej karcie
	*/

public:
	CRender();		// Konstruktor Klasy
	~CRender();	// Destruktor Klasy

	// Ustawianie perspektywy
	static void SetPerspective( float fov, float width, float height, float znear, float zfar );
	static void SetPerspective( float fov, float aspect, float znear, float zfar );
	void	SetPerspective();

	// Ustawianie rzut. prostok�tnego
	static void SetOrtho( float left, float right, float bottom, float top, float blisko = -1.0f, float daleko = 1.0f );
	void	SetOrtho();

	// Zamiana bufor�w
	void SwapBuffers();

	void VSync( unsigned int set );

	std::string GetRndInfo( RENDER_INFO info );
};
