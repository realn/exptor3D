/*--------------------------------------------
  Nazwa:	Nag³ówek Render.h
  Autor:	Real_Noname (real_noname@wp.pl)
  WWW:      www.coderulers.prv.pl
  Data:		07-05-2005 21:21
  Opis:		Funkcje i klasy potrzebne do sterowania
			OpenGL.
--------------------------------------------*/

#pragma once

#include <windows.h>	// Nag³ówek funkcji Windows
#include <gl/gl.h>		// Nag³ówek funkcji OpenGL
#include <gl/glu.h>		// Nag³ówek funkcji pomocniczych do OpenGL
#include <string>		// Nag³owek do mainpulacji ³añcuchem znaków

#include "RenderWindow.h"

enum class RENDER_INFO{
	RENDER_VERSION	= 0,
	RENDER_RENDERER	= 1,
	RENDER_VENDOR	= 2,
	RENDER_EXTLIST	= 3,
};

class CRender
{
private:
	HGLRC		hRC = nullptr;	// Kontekst Renderuj¹cy
	HDC			hDC = nullptr;	// Kontekst Urz¹dzenia
	HWND		hWnd = nullptr;	// Wskaznik na uchwyt okna	

	int		colorBits = 0;			// Bity kolorów
	int		depthBits = 0;
	int		stencilBits = 0;

	std::string rndInfo[4];	// Trzyma informacje o danej Maszynie
	/*	RndInfo[0] - Wersja OpenGL
		RndInfo[1] - Sterownik Karty Grafiki
		RndInfo[2] - Firma, która wmontowa³a OpenGL do karty ( np. NVDIA, ATI )
		RndInfo[3] - Lista rozszerzeñ dostêpnych na danej karcie
	*/

public:
	CRender();		// Konstruktor Klasy
	~CRender();	// Destruktor Klasy

	bool CreateGLContext( HWND hWindow, const unsigned colorBits = 0, const unsigned depthBits = 0, const unsigned stencilBits = 0);
	void DestroyGLContext();

	// Ustawianie perspektywy
	static void SetPerspective( float fov, float width, float height, float znear, float zfar );
	static void SetPerspective( float fov, float aspect, float znear, float zfar );

	// Ustawianie rzut. prostok¹tnego
	static void SetOrtho( float left, float right, float bottom, float top, float blisko = -1.0f, float daleko = 1.0f );

	// Zamiana buforów
	void SwapBuffers();

	void VSync( unsigned int set );

	std::string GetRndInfo( RENDER_INFO info );
};
