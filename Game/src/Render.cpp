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
		RndInfo domyœlnie s¹ puste, bo informacje otrzymamy
		dopiero po inicjalizacji kontekstu renderuj¹cego (hRC).
		Klasa std::string automatycznie ju¿ ustawi³a puste
		³añcuchy znaków.
	*/
}

/*	DESTRUKTOR	*/
CRender::~CRender()
{
}

/*	INICJACJA OPENGL
	Tu siê odbywa w³aœciwa inicjacja OpenGL.
	Mo¿liwe jest przypisanie klasie zewnêtrznego
	uchwytu do okna i inicjalizacja na nim, poprzez
	rêczne wywo³anie tej metody.
*/

/*	FUNKCJE USTAWIANIA PERSPEKTYWY
	Napisa³em te metody, bo nie chce
	mi siê wklepywaæ ca³ej regu³ki, a
	poza tym, w taki sposób program
	bêdzie ³atwo pamiêta³ ustawienia.
	UWAGA: Po wywo³aniu tych funkcji
	zostaje zresetowany aktualny uk³ad
	wspó³rzêdnych!
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

	//Reset uk³adu wspó³rzêdnych
	glLoadIdentity();	

	//Ustawienie Perspektywy ( K¹t widzenia, Rozmiar ekranu, Min widocznoœci, Max widocznoœci )
	gluPerspective( fov, aspect, znear, zfar );

	//Znowu zmiana Matrix'a
	glMatrixMode( GL_MODELVIEW );	
}

/*	FUNKCJE USTAWIANIA RZUT. PROSTOK¥TNEGO
	To samo, jak przy perspektywie
	tutaj, dziêki tym funkcj¹, bêdzie
	³atwiej siê prze³¹czyæ w tryb
	2D ( by np. dobrze pisaæ po
	ekranie ). A co oznaczaj¹ parametry?
	Jest to tzw. bry³a przycinania i
	wszystko co jest rysowane poza ni¹
	jest w³aœnie przycinane. Dodatkowo
	ostatnie dwa maj¹ wartoœci domyœlne,
	bo praktycznie s¹ rzadko u¿ywane.
*/
void CRender::SetOrtho( float left, float right, float bottom, float top, float znear, float zfar )
{
	/*	Jak widaæ, ta funkcja niewiele siê ró¿ni
		od funkcji perspektywy. Dla czytelnoœci
		( i moich palców :P ) ominê niektóre 
		zbêdne komentarze.
	*/

	glMatrixMode( GL_PROJECTION );

	glLoadIdentity();	

	//Ustawienie Rzut. Prostok¹tnego ( dla rozszyfrowania parametrów patrz -> deklaracja tablicy OrthoConf )
	glOrtho( left, right, bottom, top, znear, zfar); 

	glMatrixMode( GL_MODELVIEW );	
}

/*	ZAMIANA BUFORÓW
	Ca³a idea podwójnego buforowania
	( Double Buffering ) polega na
	dwóch miejscach pamiêci na karcie
	graficznej - przedniej ( widocznej
	na ekranie ) i tylnej ( niewidocznej ).
	Ca³y trik polega na tym, ¿e tworzymy 
	ca³¹ klatkê animacji w buforze tylnym
	i zamieniamy bufory miejscami. Dziêki
	temu nie ma wra¿enia migania i animacja
	jest p³ynna. Ta funkcja jest "obudówk¹"
	dla w³aœciwej funkcji podmiany buforów
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