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

CRender GLRender;

const std::string	WINDOW_CLASS_NAME = "OpenGLWindowClass";

/* KONSTRUKTOR
	Inicjalizacja zmienych, czyli
	ustalenie poczatkowych wartosci
*/
CRender::CRender() :
	hDC(0),
	hRC(0),
	hWnd(0),
	hInstance(0),
	fullScreen(false),
	isWindowCreated(false),
	colorBits(0)
{	
	// Domyœlne wartoœci dla rozmiaru okna ( patrz->deklaracja )
	screenSize[0] = 640;
	screenSize[1] = 480;

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
	DisableGL();
	GLDestroyWindow();
}

/*	TWORZENIE OKNA
	Maj¹c Uchwyt mo¿emu ju¿ stworzyæ okno... No prawie :)
	Potrzebne jeszcze dane to:
	Tytul	- Tytu³ okna ( ten napis na belce )
	Szerokosc, Wysokosc - Wysokoœæ i Szerokoœæ okna, jak¹ sobie ¿yczymy
	FullScr	- Czy ma byæ na pe³nym ekranie
	WndProc	- Funkcja obs³ugi komunikatów Windowsa ( musi byæ z zewn¹trz, takie zasady C++ :P )
*/
bool CRender::GLCreateWindow( std::string title, unsigned int width, unsigned int height, bool fullScreen, WNDPROC wndProc, bool initGL )
{
	if(isWindowCreated)
	{
		DisableGL();
		GLDestroyWindow();
	}

	Log.Log( "Tworzenie Okna" );
	/* Na pocz¹tku musimy ustawiæ zmienne pomocnicze */

	/*	By stworzyæ okno, potrzebna jest jego definicja,
		a tak¹ mo¿emy przekazaæ, przez strukture typu
		WNDCLASS.
	*/
	WNDCLASSEX	wc;				//Struktura klasy okna
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);

	/*	Dalej s¹ tzw. style okna. dwStyle trzyma
		bezwzglêdny styl ( taki jak, czy to okno jest wyskakuj¹ce, czy zewnêrzne )
		dwExStyle trzyma wygl¹d okna ( np. czy ma krawêdzie i belkê tytu³ow¹ ).
		Oba typy s¹ DWORD ( czyli unsigned int, jakby ktoœ pyta³ )
	*/
	DWORD		dwExStyle = 0;	//Zewnêtrzny styl okna
	DWORD		dwStyle = 0;	//Styl Okna

	/*	Potrzebny jest jeszcze tylko prostok¹t. Jest potrzebny
		By okreœliæ rozmiar i po³orzenie okna. Jest to po prostu
		struktura z 4 zmiennymi typu long int.
	*/
	RECT		WindowRect;		//Wielkoœæ okna (struktura czworoboku)

	/*	Na pocz¹tku ustawiamy prostok¹t okna. */
	WindowRect.left		= (long)0;		//Lewa krawêdŸ na 0
	WindowRect.right	= (long)width;	//Prawa krawêdŸ na wartoœæ Szerokosc
	WindowRect.top		= (long)0;		//Górna krawêdŸ na 0
	WindowRect.bottom	= (long)height;	//Dolna krawêdŸ na wartoœæ Wysokosc

	// Przy okazji ustawiamy tablice z rozmiarem, by potem by³ ³atwiejszy dostêp w ca³ej klasie
	screenSize[0] = width;
	screenSize[1] = height;

	// £apiemy instancje ( jest potrzebna do wiêkszoœci zadañ z oknem )
	hInstance	= GetModuleHandle(NULL);

	// Okreœlamy paramerty okna dziêki strukturze WNDCLASS

	/*	Styl Bezwglêdny okna
		Tu ustawiamy, ¿e okno mo¿e
		byæ odrysowywane, oraz, ¿e 
		posiada w³asny kontekst.
	*/	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	

	// Tu podajemy funkcje obs³ugi komunikatów tego okna.
	wc.lpfnWndProc		= wndProc;	

	// wtf? Lepiej zostawic w spokoju...
	wc.cbClsExtra		= 0;	// ?
	wc.cbWndExtra		= 0;	// ?		
	
	// Tu podajemy instancje
	wc.hInstance		= hInstance;	

	// £adujemy ikone aplikacji (widoczn¹ na belce okna) oraz kursor (bez zmia, bo po co?)
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			//Ustaw standardow¹ ikonê
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			//Ustaw zwyk³y wskaŸnik myszy

	// A po cholere nam podk³ad i menu, gdy i tak wszystko bêdzie rysowane na wierzchu?
	wc.hbrBackground	= NULL;	//Brak podk³adu
	wc.lpszMenuName		= NULL;	//Bez menu okna

	// Jeszcze potrzebna nazwa tej klasy okna
	wc.lpszClassName	= WINDOW_CLASS_NAME.c_str();	//Nazwa klasy okna

	/*	Teraz rejestrujemy klasê okna.
		Windows posiada swój w³asny rejestr klas okna
		i ¿eby stworzyæ jakieœ nowe okno, to trzeba
		podaæ jego klasê. Jako, ¿e my chcemy stworzyæ
		niestandardowe okno ( z OpenGL ) to musimy
		zarejestrowaæ w³asn¹ klasê okna.
	*/
	if (!RegisterClassEx(&wc)) //Próba rejestracji okna
	{
	    //Gdy nieudana, wyœwietlamy odpowieni komunikat
		Log.FatalError( "Nie mo¿na zarejestrowaæ klasy okna!" );
		return false;	//przerwij i zwróæ, ¿e inicjalizacja nie udana.
	}

	if(fullScreen)
		EnableFullScreen();
	
	//Ustawiamy styl okna w zale¿noœci, czy jest ustawiony pe³ny ekran
	if( fullScreen )	
	{
		dwExStyle = WS_EX_APPWINDOW;	// ? 
		dwStyle = WS_POPUP;				// Okienko bez niczego ( tylko œrodek okna )
		ShowCursor( false );			// Schowaj kursor myszy
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// ?
		dwStyle = WS_OVERLAPPEDWINDOW;	/*	Okno z belk¹ (która ma wszystkie przyciski typu
											Minimalizuj, itp ), z krawêdzi¹ i tytu³em
										*/
	}
	
	//Ustawiamy styl okna
	AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );		

	// Ostatecznie tworzymy okno
	if (!(hWnd = CreateWindowEx(	dwExStyle,						//Styl zewnêtrzny okna
								WINDOW_CLASS_NAME.c_str(),			//Nazwa klasy okna
								title.c_str(),						//Tytu³ okna (na belce)
								dwStyle |							//Definicja stylu
								WS_CLIPSIBLINGS |					
								WS_CLIPCHILDREN,					
								0, 0,								//Pozycja okna
								WindowRect.right - WindowRect.left,	//Szerokoœæ okna (z obliczeñ)
								WindowRect.bottom - WindowRect.top,	//Wysokoœæ okna (z obliczeñ)
								NULL,								//Brak okna-matki
								NULL,								//Bez Menu
								hInstance,							//Uchwyt pamiêci
								NULL)))								//Nic nie informuj przez WM_CREATE
	{
	 	//Je¿eli siê nie uda					
		GLDestroyWindow();		//Niszczymy okno
		Log.FatalError( "B³¹d tworzenia okna!" );
		return false;	//koñczymy dzia³anie
	}

	isWindowCreated = true;

	if(initGL)
	{
		if( !EnableGL() )
		{
			GLDestroyWindow();
			return false;
		}
	}

	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );
	
	Log.Log( "Tworzenie okna zakoñczone." );
	return true;
}

/*	INICJACJA OPENGL
	Tu siê odbywa w³aœciwa inicjacja OpenGL.
	Mo¿liwe jest przypisanie klasie zewnêtrznego
	uchwytu do okna i inicjalizacja na nim, poprzez
	rêczne wywo³anie tej metody.
*/
bool CRender::EnableGL(unsigned colorBits, unsigned depthBits, unsigned stencilBits)
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
	{
		this->colorBits = colorBits;
	}

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
	if (!(hDC = GetDC(hWnd)))
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

	/*	Musimy jeszcze przestawiæ pare rzeczy.
		Najpierw - pokazaæ okno na ekranie,
		Potem - przestawiæ priorytet w¹tku (bo
		poszczególne okna w Windows s¹ w w¹tkach - 
		dziwne?? ) na trochê wy¿szy od normalnego.
		Ustawiæ okno, by reagowa³o na klawisze, oraz
		ostatecznie ustawiæ viewport prze metode
		Resize().
	*/
	Log.Log( "Koñczenie okna..." );

	Resize( screenSize[0], screenSize[1] );

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

void CRender::DisableGL()
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

/*	NISZCZENIE OKNA
	Tu wystarczy parê prostych funkji, by posprz¹taæ
	po metodzie tworzenia okna.
	UWAGA: Gdy tracimy kontekst, to tak¿e tracimy
	wszytkie tekstury, list'y, itp. wiêc u¿ywaæ z
	rozwag¹!
*/
void CRender::GLDestroyWindow(bool deinitGL)
{
	Log.Log( "Niszczenie Okna" );

	if(deinitGL)
	{
		DisableGL();
	}

	// Teraz potrzeba usun¹æ okno
	if (hWnd != 0)
	{
		if(!DestroyWindow(hWnd))
		{
			// alala kotki dwa
			Log.FatalError( "Nie mo¿na usun¹æ okna!" );
		}
		hWnd = 0;	//Zerujemy uchwyt okna

		// wypada³oby jeszcze odrejestrowaæ klasê okna
		if (!UnregisterClass(WINDOW_CLASS_NAME.c_str(), hInstance))
		{
			// zgadnijnie co?
			Log.FatalError( "Nie mo¿na odrejestrowaæ klasy!" );
			hInstance = 0;
		}
	}


	isWindowCreated = false;
	Log.Log( "Niszczenie okna zakoñczone." );

	/*	Je¿eli ustawiliœmy pe³ny ekran, to teraz
		Wypada³o by go przywróciæ do normalnych
		ustawieñ. Dziêki wczeœniej wprowadzonej
		fladze CDS_FULLSCREEN teraz wystarczy
		wprowadziæ dowoln¹ wartoœæ by ekran
		wróci³ do porz¹dku.
	*/
	if (fullScreen)
	{
		Log.Report( "Przywracanie rozdzielczoœci..." );
		ChangeDisplaySettings(0, 0);	
		ShowCursor( true ); // Pokazujemy te¿ kursor
	}
}

void CRender::Resize( unsigned int width, unsigned int height )
{
	if( height == 0 )	// Ma³e zabezpieczenie 
		height = 1;		// przed dzieleniem przez zero

	// Ustawiamy Viewport ( taki kwadrat, przez który bêdziemy widzieli co OpenGL wykombinuje )
	glViewport( 0, 0, width, height );
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

void	CRender::SetPerspective()
{
	SetPerspective(60.0f, (float)screenSize[0], (float)screenSize[1], 0.1f, 1000.0f);
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

void	CRender::SetOrtho()
{
	SetOrtho(0.0f, (float)screenSize[0], 0.0f, (float)screenSize[1]);
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

/*	FUNKCJE DO KOMUNIKACJI POZA KLAS¥	*/
const unsigned CRender::GetWidth()
{
	return screenSize[0];
}

const unsigned CRender::GetHeight()
{
	return screenSize[1];
}

std::string CRender::GetRndInfo( RENDER_INFO info )
{
	return rndInfo[(unsigned)info];
}

bool CRender::IsFullScreen()
{
	return fullScreen;
}

void CRender::EnableFullScreen()
{
	if( fullScreen )
		return;

	ShowCursor( false );

	// struktura trzymaj¹ca konfiguracje zmiany parametrów ekranu			
	DEVMODE dmScreenSettings;

    // Musimy t¹ strukturê przeczyœciæ (nadpisaæ zerami), by nie mieæ ¿adnych niespodzianek					
	memset( &dmScreenSettings, 0, sizeof(dmScreenSettings) );	
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	
    //ustawiamy strukture	
	dmScreenSettings.dmPelsWidth	= screenSize[0];	//Szerokoœæ ekranu
	dmScreenSettings.dmPelsHeight	= screenSize[1];	//Wysokoœæ ekranu
	dmScreenSettings.dmBitsPerPel	= this->colorBits;		//G³êbia kolorów

	// Ostatnie ustawienie mówi, co zrobiæ z Pixelami
	dmScreenSettings.dmFields = (this->colorBits != 0 ? DM_BITSPERPEL : 0) | DM_PELSWIDTH | DM_PELSHEIGHT;

	/*	Próba zmiany rozdzielczoœci (CDS_FULLSCREEN usuwa 
		panel startu, oraz przywraca star¹ konfiguracje ekranu
		przy kolejnej próbie zmiany rozdzielczoœæ )
	*/
	Log.Log( "Zmiana rozdzielczoœci" );
	if (ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL)
	{
		Log.Error( "Nie mo¿na zmieniæ rozdzielczoœci!" );
		//Je¿eli nieudana, proponujemy dzia³anie okienkowe
		if (MessageBox(NULL,"Nie mo¿na zmieniæ rozdzielczoœci. Prawdopodobnie\ntwoja karta graficzna nie obs³uguje trybu Pe³noekranowego.\nU¿yæ trybu okienkowego?","OpenGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
		{
			Log.Report( "Zmiana trybu na Okienkowy" );
            //Je¿eli tak to wykonujemy
			fullScreen = false;
		}
		else
		{
			//Je¿eli nie to koñczymy dzia³anie (dajemy komunikat, ¿e progam zostanie zamkniêty)
			MessageBox( NULL, "Program zostanie zamkniêty.", "B£¥D", MB_OK | MB_ICONSTOP);
			Log.Report( "Wy³¹czenie aplikacji" );
			return;	//przerywamy dzia³anie aplikacji
		}
	}

	if(isWindowCreated)
	{
		SetWindowLong( hWnd, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
		SetWindowLong( hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW );

		RECT		WindowRect;				//Wielkoœæ okna (struktura czworoboku)
		WindowRect.left		= (long)0;				//Lewa krawêdŸ na 0
		WindowRect.right	= (long)screenSize[0];	//Prawa krawêdŸ na wartoœæ Szerokosc
		WindowRect.top		= (long)0;				//Górna krawêdŸ na 0
		WindowRect.bottom	= (long)screenSize[1];	//Dolna krawêdŸ na wartoœæ Wysokosc

		AdjustWindowRectEx( &WindowRect, WS_POPUP, false, WS_EX_APPWINDOW );		

		SetWindowPos( hWnd, HWND_TOP, WindowRect.left, WindowRect.top, WindowRect.right, WindowRect.bottom, SWP_SHOWWINDOW );
		ShowWindow( hWnd, SW_SHOW );
	}

	fullScreen = true;
}

void CRender::DisableFullScreen()
{
	if( !fullScreen )
		return;

	Log.Report( "Przywracanie rozdzielczoœci..." );
	ShowCursor( true );
	ChangeDisplaySettings(NULL,0);	

	if(isWindowCreated)
	{
		SetWindowLong( hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
		SetWindowLong( hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );

		RECT		WindowRect;				//Wielkoœæ okna (struktura czworoboku)
		WindowRect.left		= (long)0;				//Lewa krawêdŸ na 0
		WindowRect.right	= (long)screenSize[0];	//Prawa krawêdŸ na wartoœæ Szerokosc
		WindowRect.top		= (long)0;				//Górna krawêdŸ na 0
		WindowRect.bottom	= (long)screenSize[1];	//Dolna krawêdŸ na wartoœæ Wysokosc

		AdjustWindowRectEx( &WindowRect, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );		

		SetWindowPos( hWnd, HWND_TOP, 0, 0, WindowRect.right, WindowRect.bottom, SWP_SHOWWINDOW );
		ShowWindow( hWnd, SW_SHOW );
	}

	fullScreen = false;
}

void CRender::VSync( unsigned int set )
{
/*	PFNWGLSWAPINTERVALPROC   wglSwapIntervalEXT; 
	wglSwapIntervalEXT    = (PFNWGLSWAPINTERVALPROC) wglGetProcAddress("wglSwapIntervalEXT"); 
	if( wglSwapIntervalEXT )wglSwapIntervalEXT(set);*/
}