/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Render.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Render.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "Render.h"
#include "Log.h"

/* KONSTRUKTOR
	Inicjalizacja zmienych, czyli
	ustalenie poczatkowych wartosci
*/
CRender::CRender() :
	colorBits(0)
{	
	/*
		RndInfo domy�lnie s� puste, bo informacje otrzymamy
		dopiero po inicjalizacji kontekstu renderuj�cego (hRC).
		Klasa std::string automatycznie ju� ustawi�a puste
		�a�cuchy znak�w.
	*/
}

/*	DESTRUKTOR	*/
CRender::~CRender()
{
}

/*	INICJACJA OPENGL
	Tu si� odbywa w�a�ciwa inicjacja OpenGL.
	Mo�liwe jest przypisanie klasie zewn�trznego
	uchwytu do okna i inicjalizacja na nim, poprzez
	r�czne wywo�anie tej metody.
*/

/*	FUNKCJE USTAWIANIA PERSPEKTYWY
	Napisa�em te metody, bo nie chce
	mi si� wklepywa� ca�ej regu�ki, a
	poza tym, w taki spos�b program
	b�dzie �atwo pami�ta� ustawienia.
	UWAGA: Po wywo�aniu tych funkcji
	zostaje zresetowany aktualny uk�ad
	wsp�rz�dnych!
*/
void CRender::SetPerspective( float fov, float width, float height, float znear, float zfar )
{
	if( height == 0 )	// Zabezpieczenie, przed 
		height = 1;	// dzieleniem przez zero

	SetPerspective( fov, ( width / height ), znear, zfar );
}

void CRender::SetPerspective( float fov, float aspect, float znear, float zfar )
{
	// Przestawienie trybu matrix'a
	glMatrixMode( GL_PROJECTION );

	//Reset uk�adu wsp�rz�dnych
	glLoadIdentity();	

	//Ustawienie Perspektywy ( K�t widzenia, Rozmiar ekranu, Min widoczno�ci, Max widoczno�ci )
	gluPerspective( fov, aspect, znear, zfar );

	//Znowu zmiana Matrix'a
	glMatrixMode( GL_MODELVIEW );	
}

/*	FUNKCJE USTAWIANIA RZUT. PROSTOK�TNEGO
	To samo, jak przy perspektywie
	tutaj, dzi�ki tym funkcj�, b�dzie
	�atwiej si� prze��czy� w tryb
	2D ( by np. dobrze pisa� po
	ekranie ). A co oznaczaj� parametry?
	Jest to tzw. bry�a przycinania i
	wszystko co jest rysowane poza ni�
	jest w�a�nie przycinane. Dodatkowo
	ostatnie dwa maj� warto�ci domy�lne,
	bo praktycznie s� rzadko u�ywane.
*/
void CRender::SetOrtho( float left, float right, float bottom, float top, float znear, float zfar )
{
	/*	Jak wida�, ta funkcja niewiele si� r�ni
		od funkcji perspektywy. Dla czytelno�ci
		( i moich palc�w :P ) omin� niekt�re 
		zb�dne komentarze.
	*/

	glMatrixMode( GL_PROJECTION );

	glLoadIdentity();	

	//Ustawienie Rzut. Prostok�tnego ( dla rozszyfrowania parametr�w patrz -> deklaracja tablicy OrthoConf )
	glOrtho( left, right, bottom, top, znear, zfar); 

	glMatrixMode( GL_MODELVIEW );	
}

/*	ZAMIANA BUFOR�W
	Ca�a idea podw�jnego buforowania
	( Double Buffering ) polega na
	dw�ch miejscach pami�ci na karcie
	graficznej - przedniej ( widocznej
	na ekranie ) i tylnej ( niewidocznej ).
	Ca�y trik polega na tym, �e tworzymy 
	ca�� klatk� animacji w buforze tylnym
	i zamieniamy bufory miejscami. Dzi�ki
	temu nie ma wra�enia migania i animacja
	jest p�ynna. Ta funkcja jest "obud�wk�"
	dla w�a�ciwej funkcji podmiany bufor�w
	( dla prostoty ).
*/
void CRender::SwapBuffers()
{
	//::SwapBuffers( hDC );
}

std::string CRender::GetRndInfo( RENDER_INFO info )
{
	return rndInfo[(unsigned)info];
}

void CRender::VSync( unsigned int set )
{
/*	PFNWGLSWAPINTERVALPROC   wglSwapIntervalEXT; 
	wglSwapIntervalEXT    = (PFNWGLSWAPINTERVALPROC) wglGetProcAddress("wglSwapIntervalEXT"); 
	if( wglSwapIntervalEXT )wglSwapIntervalEXT(set);*/
}