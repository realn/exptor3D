/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Render.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Render.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Render.h"
#include "Log.h"

/* KONSTRUKTOR
	Inicjalizacja zmienych, czyli
	ustalenie poczatkowych wartosci
*/
CRender::CRender() :
	hDC(0),
	hRC(0),
	hWnd(0),
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
	DestroyGLContext();
}

/*	INICJACJA OPENGL
	Tu si� odbywa w�a�ciwa inicjacja OpenGL.
	Mo�liwe jest przypisanie klasie zewn�trznego
	uchwytu do okna i inicjalizacja na nim, poprzez
	r�czne wywo�anie tej metody.
*/
bool CRender::CreateGLContext( HWND hWindow, const unsigned colorBits, const unsigned depthBits, const unsigned stencilBits)
{
	Log.Log( "Inicjalizacja OpenGL" );
	/*	Na pocz�tku zmienna trzymaj�ca wynik
		wyszukiwania ustawie� dla danego
		formatu ustawie� (dziwnie brzmi, ale jest potrzebne :P )
	*/
	unsigned	pixelFormat = 0;
	unsigned	setColorBits = 0;
	unsigned	setDepthBits = 0;
	unsigned	setStencilBits = 0;

	if(colorBits == 0)
	{
		if(this->colorBits == 0)
		{
			/*	Najperw musimy wyci�gn�� z Windows aktualn� palet� kolor�w.
				Dlaczego? Nie wiadomo, ale na niekt�reych windowsach na pe�nym
				ekranie, gdy ustawiamy inn� palete ni� jest na pulpicie
				to si� dziej� r�ne nieprzyjemne rzeczy :P. Najpro�ciej
				t� informacje wyci�gn�� przez PixelFormat. Do tego jest
				potrzebny kontekst (DC), z kt�rego wyci�gamy informacje, a
				do tego z kolei potrzebny jest uchwyt okna ( bo windows
				traktuje pulpit jak jedno ogromne okno ).
			*/
			HDC deskDC = GetWindowDC( GetDesktopWindow() );
			pixelFormat = GetPixelFormat( deskDC );

			// Teraz rozpisujemy otrzyman� informacje
			PIXELFORMATDESCRIPTOR tempPFD = { 0 };
			tempPFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			tempPFD.nVersion = 1;

			DescribePixelFormat( deskDC, pixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &tempPFD );

			// Wreszcie pobieramy upragnion� informacje
			this->colorBits = tempPFD.cColorBits;
		}
	}
	else
		this->colorBits = colorBits;

	if(depthBits == 0)
	{
		if(this->depthBits == 0)
			this->depthBits = 24;
	}
	else
	{
		this->depthBits = depthBits;
	}

	if(stencilBits == 0)
	{
		if(this->stencilBits == 0)
			this->stencilBits = 8;
	}
	else
	{
		this->stencilBits = 8;
	}

	// Teraz ustwaiamy format pixeli ( jednorazowo, st�d static )
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Rozmiar
		1,								// Numer wersji
		PFD_DRAW_TO_WINDOW |			// Musi wspiera� zwyk�e okno
		PFD_SUPPORT_OPENGL |			// Musi wspiera� OpenGL
		PFD_DOUBLEBUFFER,				// Musi wspiera� podw�jne buforowanie
		PFD_TYPE_RGBA,					// Tryb kolor�w RGBA
		this->colorBits,				// G��gia kolor�w
		0, 0, 0, 0, 0, 0,				// Ignorowane bity kolor�w (�adne)
		0,								// Bez bufora kana�u Alfa
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,						// Accumulation Bits Ignored
		this->depthBits,				// Wielko�� bufora g��boko�ci 
		this->stencilBits,				// Bufor Stencil
		0,								// Bufor Aux'a
		PFD_MAIN_PLANE,					// Gdzie rysowa�
		0,								// Zarezerwowane
		0, 0, 0							// Maski Warstw (ignorowane)
	};
	
	// Pobieramy kontekst dla okna z uchwytu (* jest dodane, by uzyska� sam uchwyt, nie wska�nik )
	if (!(hDC = GetDC(hWindow)))
	{
		// Je�eli si� nie udaje
		Log.FatalError( "Nie mo�na stworzy� kontekstu GL!." );
		return false;								
	}

	// Teraz szukamy odpowiednich ustawie� w dost�pnych
	if (!(pixelFormat = ChoosePixelFormat( hDC, &pfd )))
	{
		Log.FatalError( "Nie mo�na znale�� odpowiedniego PixelFormat'u." );
		return false;								
	}

	// Spr�bujmy teraz wprowadzi� nasze ustawienia
	if(!SetPixelFormat( hDC, pixelFormat, &pfd ))
	{
		Log.FatalError( "Nie mo�na ustawic Pixel Formatu." );
		return false;								
	}

	/*	Teraz tworzymy kontekst renderuj�cy OpenGL
		Bez niego nie mo�na korzysta� z wi�kszo�ci
		funkcji w OpenGL ( w ka�dym razie - nie 
		ma sensu go nie tworzy� ).]
	*/
	if (!(hRC = wglCreateContext( hDC )))
	{
		Log.FatalError( "Nie mo�na stworzy� kontekstu renderuj�cego!." );
		return false;								
	}

	/*	A teraz trzeba uaktywni� nasz kontekst OpenGL
		Wed�ug specyfikacji mo�emy mie� kilka kontekst�w
		i si� prze��ncza� mi�dzy nimi, ale my mamy tylko
		jeden :P ( proste nie? )
	*/
	if(!wglMakeCurrent( hDC, hRC ))
	{
		Log.FatalError( "Nie mo�na aktywowa� kontekstu." );
		return false;								
	}

	Log.Log( "Ko�czenie okna..." );

	/*	I na ko�cu, kiedy kontekst jest ju� ustawiony
		wreszcie mo�na pobra� dane do RndInfo.
		S�u�y do tego funkcja glGetString(). Problem
		jest jedynie z tym, �e std::string nie akceptuje
		domy�lnie typu const unsigned char* zwracanego przez
		t� funkcje, ale konwersja pomaga :)
	*/
	Log.Log( "Pobieranie Informacji o sprz�cie..." );
	rndInfo[0] = (char*)glGetString( GL_VERSION );
	rndInfo[1] = (char*)glGetString( GL_RENDERER );
	rndInfo[2] = (char*)glGetString( GL_VENDOR );
	rndInfo[3] = (char*)glGetString( GL_EXTENSIONS );

	Log.Report( "Wersja OpenGL: " + rndInfo[0] );
	Log.Report( "Sterownik: " + rndInfo[1] );
	Log.Report( "Dostawca: " + rndInfo[2] );

	// Hura! Sukces, nic si� nie sypne�o :)
	return true;
}

void CRender::DestroyGLContext()
{
	/*	Je�eli mamy kontekst OpenGL i nie jest
		on zerowy, to wypada�o by go skasowa�,
		ale najpierw trzeba zresetowa� aktywny
		kontekst.
	*/
	if (hRC != 0)
	{
		/*	btw: mo�na by�o by przywr�ci� kontekst jaki by� poprzednio
			aktywny, ale to za du�o roboty :P	*/
		if (!wglMakeCurrent( 0, 0 ))
		{
			// wtf?
			Log.FatalError( "Nie mo�na od��czy� kontekstu!" );
		}

		// Usuwamy kontekst OpenGL
		if (!wglDeleteContext( hRC ))
		{
		    // Chyba nie chce :P
			Log.FatalError( "Nie mo�na usun�� kontekstu OpenGL!" );
		}
		hRC = 0; // Zerujemy kontekst ( wtedy if go nie wychwyci przy nast�pnym wywo�aniu metody )
	}

	// Teraz usuwamy konteks aplikacji - wystarczy go tylko "pu�ci�" i ju� b�dzie wolny.
	if (hDC != 0)
	{
		if(!ReleaseDC( hWnd, hDC ))
		{
			// Znowu???
			Log.FatalError( "Nie mo�na usun�� kontekstu aplikacji!" );
		}
		hDC = 0;	//Zerujemy kontekst
	}
}

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
	::SwapBuffers( hDC );
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