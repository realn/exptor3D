/*--------------------------------------------
  Nazwa:	Nag³ówek Render.h
  Autor:	Real_Noname (real_noname@wp.pl)
  WWW:      www.coderulers.prv.pl
  Data:		07-05-2005 21:21
  Opis:		Funkcje i klasy potrzebne do sterowania
			OpenGL.
--------------------------------------------*/

#ifndef _RENDER_H_
#define _RENDER_H_

#include <windows.h>	// Nag³ówek funkcji Windows
#include <gl/gl.h>		// Nag³ówek funkcji OpenGL
#include <gl/glu.h>		// Nag³ówek funkcji pomocniczych do OpenGL
#include <string>		// Nag³owek do mainpulacji ³añcuchem znaków
#include "Log.h"

#define RENDER_VERSION	0
#define RENDER_RENDERER	1
#define RENDER_VENDOR	2
#define RENDER_EXTLIST	3

class UIRender
{
private:
	HGLRC	hRC;	// Kontekst Renderuj¹cy
	HDC		hDC;	// Kontekst Urz¹dzenia
	HWND	hWnd;	// Wskaznik na uchwyt okna	
	HINSTANCE	hInstance;	// Instancja

	bool IsPers; // Do sprawdzania, czy jest ustawiona perspektywa
	bool fullS; // Do sprawdzania, czy jest w³¹czony pe³ny Ekran
	bool IsWin;	// Do sprawdzenia, czy okno ju¿ jest postawione
	int	 ScrSize[2]; // Trzyma rozmiar okna. ( 0 - szerokoœæ, 1 - wysokoœæ )
	int	 ColBits; // Bity kolorów

	float PerConf[4]; // Trzyma konfiguracje perspektywy
	/*	PerConf[0] - Kat
		PerConf[1] - Wspó³czynik pomiêdzy wysokoscia/szerokoscia
		PerConf[2] - Minimalna odleglosc
		PerConf[3] - Maksymalna odleglosc*/

	float OrthoConf[6]; // Trzyma konfiguracje rzutowania prostok¹tnego
	/*	OrthoConf[0] - krawedz lewa bryly obcinania
		OrthoConf[1] - prawa
		OrthoConf[2] - dolna
		OrthoConf[3] - górna
		OrthoConf[4] - bliska ( do ciebie od ekranu )
		OrthoConf[5] - daleka ( za ekranem )
	*/

	std::string RndInfo[4];	// Trzyma informacje o danej Maszynie
	/*	RndInfo[0] - Wersja OpenGL
		RndInfo[1] - Sterownik Karty Grafiki
		RndInfo[2] - Firma, która wmontowa³a OpenGL do karty ( np. NVDIA, ATI )
		RndInfo[3] - Lista rozszerzeñ dostêpnych na danej karcie
	*/

public:
	UIRender();		// Konstruktor Klasy
	~UIRender();	// Destruktor Klasy

	// Skalowanie ViewPortu ( tego kwadratu, wktórym wyœwietlana jest grafika ).
	void Resize( unsigned int szerokosc, unsigned int wysokosc );

	// Tworzenie Okna ( zwraca true, jezeli udane lub false, jezeli sie nie powiod³o )
	bool GLCreateWindow( std::string Tytul, int Szerokosc, int Wysokosc, bool FullScr, WNDPROC WndProc );
	bool EnableGL();

	// Niszczenie Okna ( nic nie zwraca, bo i tak jak siê coœ zwali, to nic nie zaszkodzi )
	void GLDestroyWindow();
	void DisableGL();

	// Ustawianie perspektywy
	void SetPerspective( float kat, int szerokosc, int wysokosc, float blisko, float daleko );
	void SetPerspective( float kat, float wspl, float blisko, float daleko );
	void SetPerspective();

	// Ustawianie rzut. prostok¹tnego
	void SetOrtho( float lewo, float prawo, float dol, float gora, float blisko = -1, float daleko = 1 );
	void SetOrtho();

	// Zamiana buforów
	void SwitchBuf();

	// Funkcje by pobraæ szerokoœæ i wysokoœæ okna
	const int GetWidth();
	const int GetHeight();

	bool GetFullScreen();
	void EnableFullScreen();
	void DisableFullScreen();

	void VSync( unsigned int set );

	std::string GetRndInfo( unsigned int info );
};

extern UIRender GLRender;

#endif