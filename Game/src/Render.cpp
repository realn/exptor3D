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
		RndInfo domyœlnie s¹ puste, bo informacje otrzymamy
		dopiero po inicjalizacji kontekstu renderuj¹cego (hRC).
		Klasa std::string automatycznie ju¿ ustawi³a puste
		³añcuchy znaków.
	*/
}

/*	DESTRUKTOR	*/
CRender::~CRender()
{
	DestroyGLContext();
}

/*	INICJACJA OPENGL
	Tu siê odbywa w³aœciwa inicjacja OpenGL.
	Mo¿liwe jest przypisanie klasie zewnêtrznego
	uchwytu do okna i inicjalizacja na nim, poprzez
	rêczne wywo³anie tej metody.
*/
bool CRender::CreateGLContext( HWND hWindow, const unsigned colorBits, const unsigned depthBits, const unsigned stencilBits)
{
	Log.Log( "Inicjalizacja OpenGL" );
	/*	Na pocz¹tku zmienna trzymaj¹ca wynik
		wyszukiwania ustawieñ dla danego
		formatu ustawieñ (dziwnie brzmi, ale jest potrzebne :P )
	*/
	unsigned	pixelFormat = 0;
	unsigned	setColorBits = 0;
	unsigned	setDepthBits = 0;
	unsigned	setStencilBits = 0;

	if(colorBits == 0)
	{
		if(this->colorBits == 0)
		{
			/*	Najperw musimy wyci¹gn¹æ z Windows aktualn¹ paletê kolorów.
				Dlaczego? Nie wiadomo, ale na niektóreych windowsach na pe³nym
				ekranie, gdy ustawiamy inn¹ palete ni¿ jest na pulpicie
				to siê dziej¹ ró¿ne nieprzyjemne rzeczy :P. Najproœciej
				t¹ informacje wyci¹gn¹æ przez PixelFormat. Do tego jest
				potrzebny kontekst (DC), z którego wyci¹gamy informacje, a
				do tego z kolei potrzebny jest uchwyt okna ( bo windows
				traktuje pulpit jak jedno ogromne okno ).
			*/
			HDC deskDC = GetWindowDC( GetDesktopWindow() );
			pixelFormat = GetPixelFormat( deskDC );

			// Teraz rozpisujemy otrzyman¹ informacje
			PIXELFORMATDESCRIPTOR tempPFD = { 0 };
			tempPFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			tempPFD.nVersion = 1;

			DescribePixelFormat( deskDC, pixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &tempPFD );

			// Wreszcie pobieramy upragnion¹ informacje
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

	// Teraz ustwaiamy format pixeli ( jednorazowo, st¹d static )
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Rozmiar
		1,								// Numer wersji
		PFD_DRAW_TO_WINDOW |			// Musi wspieraæ zwyk³e okno
		PFD_SUPPORT_OPENGL |			// Musi wspieraæ OpenGL
		PFD_DOUBLEBUFFER,				// Musi wspieraæ podwójne buforowanie
		PFD_TYPE_RGBA,					// Tryb kolorów RGBA
		this->colorBits,				// G³êgia kolorów
		0, 0, 0, 0, 0, 0,				// Ignorowane bity kolorów (¿adne)
		0,								// Bez bufora kana³u Alfa
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,						// Accumulation Bits Ignored
		this->depthBits,				// Wielkoœæ bufora g³êbokoœci 
		this->stencilBits,				// Bufor Stencil
		0,								// Bufor Aux'a
		PFD_MAIN_PLANE,					// Gdzie rysowaæ
		0,								// Zarezerwowane
		0, 0, 0							// Maski Warstw (ignorowane)
	};
	
	// Pobieramy kontekst dla okna z uchwytu (* jest dodane, by uzyskaæ sam uchwyt, nie wskaŸnik )
	if (!(hDC = GetDC(hWindow)))
	{
		// Je¿eli siê nie udaje
		Log.FatalError( "Nie mo¿na stworzyæ kontekstu GL!." );
		return false;								
	}

	// Teraz szukamy odpowiednich ustawieñ w dostêpnych
	if (!(pixelFormat = ChoosePixelFormat( hDC, &pfd )))
	{
		Log.FatalError( "Nie mo¿na znaleŸæ odpowiedniego PixelFormat'u." );
		return false;								
	}

	// Spróbujmy teraz wprowadziæ nasze ustawienia
	if(!SetPixelFormat( hDC, pixelFormat, &pfd ))
	{
		Log.FatalError( "Nie mo¿na ustawic Pixel Formatu." );
		return false;								
	}

	/*	Teraz tworzymy kontekst renderuj¹cy OpenGL
		Bez niego nie mo¿na korzystaæ z wiêkszoœci
		funkcji w OpenGL ( w ka¿dym razie - nie 
		ma sensu go nie tworzyæ ).]
	*/
	if (!(hRC = wglCreateContext( hDC )))
	{
		Log.FatalError( "Nie mo¿na stworzyæ kontekstu renderuj¹cego!." );
		return false;								
	}

	/*	A teraz trzeba uaktywniæ nasz kontekst OpenGL
		Wed³ug specyfikacji mo¿emy mieæ kilka kontekstów
		i siê prze³¹nczaæ miêdzy nimi, ale my mamy tylko
		jeden :P ( proste nie? )
	*/
	if(!wglMakeCurrent( hDC, hRC ))
	{
		Log.FatalError( "Nie mo¿na aktywowaæ kontekstu." );
		return false;								
	}

	Log.Log( "Koñczenie okna..." );

	/*	I na koñcu, kiedy kontekst jest ju¿ ustawiony
		wreszcie mo¿na pobraæ dane do RndInfo.
		S³u¿y do tego funkcja glGetString(). Problem
		jest jedynie z tym, ¿e std::string nie akceptuje
		domyœlnie typu const unsigned char* zwracanego przez
		tê funkcje, ale konwersja pomaga :)
	*/
	Log.Log( "Pobieranie Informacji o sprzêcie..." );
	rndInfo[0] = (char*)glGetString( GL_VERSION );
	rndInfo[1] = (char*)glGetString( GL_RENDERER );
	rndInfo[2] = (char*)glGetString( GL_VENDOR );
	rndInfo[3] = (char*)glGetString( GL_EXTENSIONS );

	Log.Report( "Wersja OpenGL: " + rndInfo[0] );
	Log.Report( "Sterownik: " + rndInfo[1] );
	Log.Report( "Dostawca: " + rndInfo[2] );

	// Hura! Sukces, nic siê nie sypne³o :)
	return true;
}

void CRender::DestroyGLContext()
{
	/*	Je¿eli mamy kontekst OpenGL i nie jest
		on zerowy, to wypada³o by go skasowaæ,
		ale najpierw trzeba zresetowaæ aktywny
		kontekst.
	*/
	if (hRC != 0)
	{
		/*	btw: mo¿na by³o by przywróciæ kontekst jaki by³ poprzednio
			aktywny, ale to za du¿o roboty :P	*/
		if (!wglMakeCurrent( 0, 0 ))
		{
			// wtf?
			Log.FatalError( "Nie mo¿na od³¹czyæ kontekstu!" );
		}

		// Usuwamy kontekst OpenGL
		if (!wglDeleteContext( hRC ))
		{
		    // Chyba nie chce :P
			Log.FatalError( "Nie mo¿na usun¹æ kontekstu OpenGL!" );
		}
		hRC = 0; // Zerujemy kontekst ( wtedy if go nie wychwyci przy nastêpnym wywo³aniu metody )
	}

	// Teraz usuwamy konteks aplikacji - wystarczy go tylko "puœciæ" i ju¿ bêdzie wolny.
	if (hDC != 0)
	{
		if(!ReleaseDC( hWnd, hDC ))
		{
			// Znowu???
			Log.FatalError( "Nie mo¿na usun¹æ kontekstu aplikacji!" );
		}
		hDC = 0;	//Zerujemy kontekst
	}
}

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