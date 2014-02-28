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

UIRender GLRender;
/* KONSTRUKTOR
	Inicjalizacja zmienych, czyli
	ustalenie poczatkowych wartosci
*/
UIRender::UIRender()
{
	hDC = NULL; // Przeciez na razie nia ma
	hRC = NULL; // ¿adnego kontekstu, dlatego
	hWnd = NULL;// zerujemy te zmienne
	hInstance = NULL;
	
	IsPers = false; // Na poczatku nie potrzebujemy perspektywy
	fullS = true;	// Na pocz¹tku lepiej ustawiæ pe³ny ekran
	IsWin = false;	// Przecie¿ na pocz¹tku nie mamy okna!
	ColBits = -1;	// Domyœlna wartoœæ dla bitów kolorów

	// Domyœlne wartoœci dla rozmiaru okna ( patrz->deklaracja )
	ScrSize[0] = 1;
	ScrSize[1] = 1;

	// Domyslne wartosci dla perspektywy ( patrz->deklaracja tablicy )
	PerConf[0] = 45.0f;
	PerConf[1] = 1.0f;
	PerConf[2] = 1.0f;
	PerConf[3] = 100.0f;

	// Domyslne wartosci dla rzutowania prostokatnego ( patrz->deklaracja tablicy )
	OrthoConf[0] = -1.0f;
	OrthoConf[1] = 1.0f;
	OrthoConf[2] = -1.0f;
	OrthoConf[3] = 1.0f;
	OrthoConf[4] = -1.0f;
	OrthoConf[5] = 1.0f;

	/*
		RndInfo domyœlnie s¹ puste, bo informacje otrzymamy
		dopiero po inicjalizacji kontekstu renderuj¹cego (hRC).
		Klasa std::string automatycznie ju¿ ustawi³a puste
		³añcuchy znaków.
	*/
}

/*	DESTRUKTOR	*/
UIRender::~UIRender()
{

}

/*	TWORZENIE OKNA
	Maj¹c Uchwyt mo¿emu ju¿ stworzyæ okno... No prawie :)
	Potrzebne jeszcze dane to:
	Tytul	- Tytu³ okna ( ten napis na belce )
	Szerokosc, Wysokosc - Wysokoœæ i Szerokoœæ okna, jak¹ sobie ¿yczymy
	FullScr	- Czy ma byæ na pe³nym ekranie
	WndProc	- Funkcja obs³ugi komunikatów Windowsa ( musi byæ z zewn¹trz, takie zasady C++ :P )
*/
bool UIRender::GLCreateWindow( std::string Tytul, int Szerokosc, int Wysokosc, bool FullScr, WNDPROC WndProc )
{
	Log.Log( "Tworzenie Okna" );
	/* Na pocz¹tku musimy ustawiæ zmienne pomocnicze */

	/*	By stworzyæ okno, potrzebna jest jego definicja,
		a tak¹ mo¿emy przekazaæ, przez strukture typu
		WNDCLASS.
	*/
	WNDCLASS	wc;				//Struktura klasy okna

	/*	Dalej s¹ tzw. style okna. dwStyle trzyma
		bezwzglêdny styl ( taki jak, czy to okno jest wyskakuj¹ce, czy zewnêrzne )
		dwExStyle trzyma wygl¹d okna ( np. czy ma krawêdzie i belkê tytu³ow¹ ).
		Oba typy s¹ DWORD ( czyli unsigned int, jakby ktoœ pyta³ )
	*/
	DWORD		dwExStyle;				//Zewnêtrzny styl okna
	DWORD		dwStyle;				//Styl Okna

	/*	Potrzebny jest jeszcze tylko prostok¹t. Jest potrzebny
		By okreœliæ rozmiar i po³orzenie okna. Jest to po prostu
		struktura z 4 zmiennymi typu long int.
	*/
	RECT		WindowRect;				//Wielkoœæ okna (struktura czworoboku)

	/*	Na pocz¹tku ustawiamy prostok¹t okna. */
	WindowRect.left		= (long)0;			//Lewa krawêdŸ na 0
	WindowRect.right	= (long)Szerokosc;	//Prawa krawêdŸ na wartoœæ Szerokosc
	WindowRect.top		= (long)0;			//Górna krawêdŸ na 0
	WindowRect.bottom	= (long)Wysokosc;	//Dolna krawêdŸ na wartoœæ Wysokosc

	// Przy okazji ustawiamy tablice z rozmiarem, by potem by³ ³atwiejszy dostêp w ca³ej klasie
	ScrSize[0] = Szerokosc;
	ScrSize[1] = Wysokosc;

	// Oraz ustawiamy czy wszystko jest w pe³nym ekranie
	fullS = FullScr;

	// £apiemy instancje ( jest potrzebna do wiêkszoœci zadañ z oknem )
	hInstance			= GetModuleHandle(NULL);

	// Okreœlamy paramerty okna dziêki strukturze WNDCLASS

	/*	Styl Bezwglêdny okna
		Tu ustawiamy, ¿e okno mo¿e
		byæ odrysowywane, oraz, ¿e 
		posiada w³asny kontekst.
	*/	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	

	// Tu podajemy funkcje obs³ugi komunikatów tego okna.
	wc.lpfnWndProc		= WndProc;	

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
	wc.lpszClassName	= "OpenGL";	//Nazwa klasy okna

	/*	Teraz rejestrujemy klasê okna.
		Windows posiada swój w³asny rejestr klas okna
		i ¿eby stworzyæ jakieœ nowe okno, to trzeba
		podaæ jego klasê. Jako, ¿e my chcemy stworzyæ
		niestandardowe okno ( z OpenGL ) to musimy
		zarejestrowaæ w³asn¹ klasê okna.
	*/
	if (!RegisterClass(&wc)) //Próba rejestracji okna
	{
	    //Gdy nieudana, wyœwietlamy odpowieni komunikat
		Log.FatalError( "Nie mo¿na zarejestrowaæ klasy okna!" );
		return false;	//przerwij i zwróæ, ¿e inicjalizacja nie udana.
	}
	
	/*	Teraz ustwiamy pe³ny ekran.
		Mamy ju¿ do tego wszystkie potrzebne informacje
		( oczywiœcie je¿eli chcemy pe³ny ekran.
	*/
	if (fullS)
	{
		// struktura trzymaj¹ca konfiguracje zmiany parametrów ekranu			
		DEVMODE dmScreenSettings;

        // Musimy t¹ strukturê przeczyœciæ (nadpisaæ zerami), by nie mieæ ¿adnych niespodzianek					
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		
        //ustawiamy strukture	
		dmScreenSettings.dmPelsWidth	= ScrSize[0];	//Szerokoœæ ekranu
		dmScreenSettings.dmPelsHeight	= ScrSize[1];	//Wysokoœæ ekranu
		dmScreenSettings.dmBitsPerPel	= 32;		//G³êbia kolorów

		// Ostatnie ustawienie mówi, co zrobiæ z Pixelami
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		/*	Próba zmiany rozdzielczoœci (CDS_FULLSCREEN usuwa 
			panel startu, oraz przywraca star¹ konfiguracje ekranu
			przy kolejnej próbie zmiany rozdzielczoœæ )
		*/
		Log.Log( "Zmiana rozdzielczoœci" );
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			Log.Error( "Nie mo¿na zmieniæ rozdzielczoœci!" );
			//Je¿eli nieudana, proponujemy dzia³anie okienkowe
			if (MessageBox(NULL,"Nie mo¿na zmieniæ rozdzielczoœci. Prawdopodobnie\ntwoja karta graficzna nie obs³uguje trybu Pe³noekranowego.\nU¿yæ trybu okienkowego?","OpenGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				Log.Report( "Zmiana trybu na Okienkowy" );
                //Je¿eli tak to wykonujemy
				fullS = false;
			}
			else
			{
				//Je¿eli nie to koñczymy dzia³anie (dajemy komunikat, ¿e progam zostanie zamkniêty)
				MessageBox(NULL,"Program zostanie zamkniêty.","B£¥D",MB_OK|MB_ICONSTOP);
				Log.Report( "Wy³¹czenie aplikacji" );
				return false;	//przerywamy dzia³anie aplikacji
			}
		}
	}

	//Ustawiamy styl okna w zale¿noœci, czy jest ustawiony pe³ny ekran
	if( fullS )	
	{
		dwExStyle=WS_EX_APPWINDOW;	// ? 
		dwStyle=WS_POPUP;			// Okienko bez niczego ( tylko œrodek okna )
		ShowCursor( false );		// Schowaj kursor myszy
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// ?
		dwStyle=WS_OVERLAPPEDWINDOW;	/*	Okno z belk¹ (która ma wszystkie przyciski typu
											Minimalizuj, itp ), z krawêdzi¹ i tytu³em
										*/
	}
	
	//Ustawiamy styl okna
	AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );		

	// Ostatecznie tworzymy okno
	if (!(hWnd=CreateWindowEx(	dwExStyle,							//Styl zewnêtrzny okna
								"OpenGL",							//Nazwa klasy okna
								Tytul.c_str(),						//Tytu³ okna (na belce)
								dwStyle |							//Definicja stylu
								WS_CLIPSIBLINGS |					
								WS_CLIPCHILDREN,					
								0, 0,								//Pozycja okna
								WindowRect.right-WindowRect.left,	//Szerokoœæ okna (z obliczeñ)
								WindowRect.bottom-WindowRect.top,	//Wysokoœæ okna (z obliczeñ)
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

	IsWin = true;

	if( !EnableGL() )
	{
		GLDestroyWindow();
		return false;
	}
	
	Log.Log( "Tworzenie okna zakoñczone." );
	return true;
}

/*	INICJACJA OPENGL
	Tu siê odbywa w³aœciwa inicjacja OpenGL.
	Mo¿liwe jest przypisanie klasie zewnêtrznego
	uchwytu do okna i inicjalizacja na nim, poprzez
	rêczne wywo³anie tej metody.
*/
bool UIRender::EnableGL()
{
	Log.Log( "Inicjalizacja OpenGL" );
	/*	Na pocz¹tku zmienna trzymaj¹ca wynik
		wyszukiwania ustawieñ dla danego
		formatu ustawieñ (dziwnie brzmi, ale jest potrzebne :P )
	*/
	unsigned int   PixelFormat;			//Wynik wyszukiwania

	/*	Najperw musimy wyci¹gn¹æ z Windows aktualn¹ paletê kolorów.
		Dlaczego? Nie wiadomo, ale na niektóreych windowsach na pe³nym
		ekranie, gdy ustawiamy inn¹ palete ni¿ jest na pulpicie
		to siê dziej¹ ró¿ne nieprzyjemne rzeczy :P. Najproœciej
		t¹ informacje wyci¹gn¹æ przez PixelFormat. Do tego jest
		potrzebny kontekst (DC), z którego wyci¹gamy informacje, a
		do tego z kolei potrzebny jest uchwyt okna ( bo windows
		traktuje pulpit jak jedno ogromne okno ).
	*/
	if( ColBits == -1 ) // sprawdzamy, czy casem informacja nie zosta³a ju¿ wyci¹gniêta
	{
		HDC deskDC = GetWindowDC( GetDesktopWindow() );
		PixelFormat = GetPixelFormat( deskDC );

		// Teraz rozpisujemy otrzyman¹ informacje
		PIXELFORMATDESCRIPTOR tempPFD;
		DescribePixelFormat( deskDC, PixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &tempPFD );

		// Wreszcie pobieramy upragnion¹ informacje
		ColBits = tempPFD.cColorBits;
	}

	// Teraz ustwaiamy format pixeli ( jednorazowo, st¹d static )
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Rozmiar
		1,								// Numer wersji
		PFD_DRAW_TO_WINDOW |			// Musi wspieraæ zwyk³e okno
		PFD_SUPPORT_OPENGL |			// Musi wspieraæ OpenGL
		PFD_DOUBLEBUFFER,				// Musi wspieraæ podwójne buforowanie
		PFD_TYPE_RGBA,					// Tryb kolorów RGBA
		ColBits,						// G³êgia kolorów
		0, 0, 0, 0, 0, 0,				// Ignorowane bity kolorów (¿adne)
		0,								// Bez bufora kana³u Alfa
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,						// Accumulation Bits Ignored
		16,								// Wielkoœæ bufora g³êbokoœci (16bit) 
		0,								// Bufor Stencil (W³¹czony, na wszelki wypadek)
		0,								// Bufor Aux'a
		PFD_MAIN_PLANE,					// Gdzie rysowaæ
		0,								// Zarezerwowane
		0, 0, 0							// Maski Warstw (ignorowane)
	};
	
	// Pobieramy kontekst dla okna z uchwytu (* jest dodane, by uzyskaæ sam uchwyt, nie wskaŸnik )
	if (!(hDC=GetDC(hWnd)))
	{
		// Je¿eli siê nie udaje
		Log.FatalError( "Nie mo¿na stworzyæ kontekstu GL!." );
		return false;								
	}

	// Teraz szukamy odpowiednich ustawieñ w dostêpnych
	if (!(PixelFormat = ChoosePixelFormat( hDC, &pfd )))
	{
		Log.FatalError( "Nie mo¿na znaleŸæ odpowiedniego PixelFormat'u." );
		return false;								
	}

	// Spróbujmy teraz wprowadziæ nasze ustawienia
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))
	{
		Log.FatalError( "Nie mo¿na ustawic Pixel Formatu." );
		return false;								
	}

	/*	Teraz tworzymy kontekst renderuj¹cy OpenGL
		Bez niego nie mo¿na korzystaæ z wiêkszoœci
		funkcji w OpenGL ( w ka¿dym razie - nie 
		ma sensu go nie tworzyæ ).]
	*/
	if (!(hRC=wglCreateContext(hDC)))
	{
		Log.FatalError( "Nie mo¿na stworzyæ kontekstu renderuj¹cego!." );
		return false;								
	}

	/*	A teraz trzeba uaktywniæ nasz kontekst OpenGL
		Wed³ug specyfikacji mo¿emy mieæ kilka kontekstów
		i siê prze³¹nczaæ miêdzy nimi, ale my mamy tylko
		jeden :P ( proste nie? )
	*/
	if(!wglMakeCurrent(hDC,hRC))
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
	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );
	Resize( ScrSize[0], ScrSize[1] );

	/*	I na koñcu, kiedy kontekst jest ju¿ ustawiony
		wreszcie mo¿na pobraæ dane do RndInfo.
		S³u¿y do tego funkcja glGetString(). Problem
		jest jedynie z tym, ¿e std::string nie akceptuje
		domyœlnie typu const unsigned char* zwracanego przez
		tê funkcje, ale konwersja pomaga :)
	*/
	Log.Log( "Pobieranie Informacji o sprzêcie..." );
	RndInfo[0] = (char*)glGetString( GL_VERSION );
	RndInfo[1] = (char*)glGetString( GL_RENDERER );
	RndInfo[2] = (char*)glGetString( GL_VENDOR );
	RndInfo[3] = (char*)glGetString( GL_EXTENSIONS );
	Log.Report( "Wersja OpenGL: " + RndInfo[0] );
	Log.Report( "Sterownik: " + RndInfo[1] );
	Log.Report( "Dostawca: " + RndInfo[2] );

	// Hura! Sukces, nic siê nie sypne³o :)
	return true;
}

void UIRender::DisableGL()
{
	/*	Je¿eli mamy kontekst OpenGL i nie jest
		on zerowy, to wypada³o by go skasowaæ,
		ale najpierw trzeba zresetowaæ aktywny
		kontekst.
	*/
	if (hRC)
	{
		/*	btw: mo¿na by³o by przywróciæ kontekst jaki by³ poprzednio
			aktywny, ale to za du¿o roboty :P	*/
		if (!wglMakeCurrent(NULL,NULL))
		{
			// wtf?
			Log.FatalError( "Nie mo¿na od³¹czyæ kontekstu!" );
		}

		// Usuwamy kontekst OpenGL
		if (!wglDeleteContext(hRC))
		{
		    // Chyba nie chce :P
			Log.FatalError( "Nie mo¿na usun¹æ kontekstu OpenGL!" );
		}
		hRC=NULL; // Zerujemy kontekst ( wtedy if go nie wychwyci przy nastêpnym wywo³aniu metody )
	}

	// Teraz usuwamy konteks aplikacji - wystarczy go tylko "puœciæ" i ju¿ bêdzie wolny.
	if (hDC && !ReleaseDC(hWnd,hDC))
	{
	    // Znowu???
		Log.FatalError( "Nie mo¿na usun¹æ kontekstu aplikacji!" );
		hDC=NULL;	//Zerujemy kontekst
	}
}

/*	NISZCZENIE OKNA
	Tu wystarczy parê prostych funkji, by posprz¹taæ
	po metodzie tworzenia okna.
	UWAGA: Gdy tracimy kontekst, to tak¿e tracimy
	wszytkie tekstury, list'y, itp. wiêc u¿ywaæ z
	rozwag¹!
*/
void UIRender::GLDestroyWindow()
{
	Log.Log( "Niszczenie Okna" );
	/*	Je¿eli ustawiliœmy pe³ny ekran, to teraz
		Wypada³o by go przywróciæ do normalnych
		ustawieñ. Dziêki wczeœniej wprowadzonej
		fladze CDS_FULLSCREEN teraz wystarczy
		wprowadziæ dowoln¹ wartoœæ by ekran
		wróci³ do porz¹dku.
	*/
	if (fullS)
	{
		Log.Report( "Przywracanie rozdzielczoœci..." );
		ChangeDisplaySettings(NULL,0);	
		ShowCursor( true ); // Pokazujemy te¿ kursor
	}

	DisableGL();

	// Teraz potrzeba usun¹æ okno
	if (hWnd && !DestroyWindow(hWnd))
	{
	    // alala kotki dwa
		Log.FatalError( "Nie mo¿na usun¹æ okna!" );
		hWnd=NULL;	//Zerujemy uchwyt okna
	}

	// wypada³oby jeszcze odrejestrowaæ klasê okna
	if (!UnregisterClass("OpenGL",hInstance))
	{
	    // zgadnijnie co?
		Log.FatalError( "Nie mo¿na odrejestrowaæ klasy!" );
		hInstance=NULL;
	}

	IsWin = false;
	Log.Log( "Niszczenie okna zakoñczone." );
}

void UIRender::Resize( unsigned int szerokosc, unsigned int wysokosc )
{
	if( wysokosc == 0 )	// Ma³e zabezpieczenie 
		wysokosc = 1;	// przed dzieleniem przez zero

	// Ustawiamy Viewport ( taki kwadrat, przez który bêdziemy widzieli co OpenGL wykombinuje )
	glViewport( 0, 0, szerokosc, wysokosc);

	// W zale¿noœci czy mamy widok perspektywy, czy prostok¹tny, to musimy zresetowaæ matrix
	if( IsPers )
		SetPerspective( PerConf[0], szerokosc, wysokosc, PerConf[2], PerConf[3] );
	else SetOrtho();
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
void UIRender::SetPerspective( float kat, int szerokosc, int wysokosc, float blisko, float daleko )
{
	if( wysokosc == 0 )	// Zabezpieczenie, przed 
		wysokosc = 1;	// dzieleniem przez zero

	SetPerspective( kat, ( (float)szerokosc / (float)wysokosc ), blisko, daleko );
}

void UIRender::SetPerspective( float kat, float wspl, float blisko, float daleko )
{
	// Zapisanie do wartoœci domyœlnych, by ³atwo mo¿na by³o
	// Prze³anczaæ siê miêdzy typami rzutów
	PerConf[0] = kat;
	PerConf[1] = wspl;
	PerConf[2] = blisko;
	PerConf[3] = daleko;

	SetPerspective();
}

void UIRender::SetPerspective()
{
	// A tu jest ta regu³ka ;)

	// Przestawienie trybu matrix'a
	glMatrixMode( GL_PROJECTION );

	//Reset uk³adu wspó³rzêdnych
	glLoadIdentity();	

	//Ustawienie Perspektywy ( K¹t widzenia, Rozmiar ekranu, Min widocznoœci, Max widocznoœci )
	gluPerspective( PerConf[0], PerConf[1], PerConf[2], PerConf[3] );

	//Znowu zmiana Matrix'a
	glMatrixMode(GL_MODELVIEW);	
	
	//I reset uk³adu
	glLoadIdentity();
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
void UIRender::SetOrtho( float lewo, float prawo, float dol, float gora, float blisko, float daleko )
{
	// Przypisanie nowych wartoœci do tablicy
	OrthoConf[0] = lewo;
	OrthoConf[1] = prawo;
	OrthoConf[2] = dol;
	OrthoConf[3] = gora;
	OrthoConf[4] = blisko;
	OrthoConf[5] = daleko;

	// Wywo³anie w³aœciwej funkcji
	SetOrtho();
}

void UIRender::SetOrtho()
{
	/*	Jak widaæ, ta funkcja niewiele siê ró¿ni
		od funkcji perspektywy. Dla czytelnoœci
		( i moich palców :P ) ominê niektóre 
		zbêdne komentarze.
	*/

	glMatrixMode( GL_PROJECTION );

	glLoadIdentity();	

	//Ustawienie Rzut. Prostok¹tnego ( dla rozszyfrowania parametrów patrz -> deklaracja tablicy OrthoConf )
	glOrtho( OrthoConf[0], OrthoConf[1], OrthoConf[2], OrthoConf[3], OrthoConf[4], OrthoConf[5] ); 

	glMatrixMode(GL_MODELVIEW);	
	
	glLoadIdentity();
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
void UIRender::SwitchBuf()
{
	SwapBuffers(hDC);
}

/*	FUNKCJE DO KOMUNIKACJI POZA KLAS¥	*/
const int UIRender::GetWidth()
{
	return ScrSize[0];
}

const int UIRender::GetHeight()
{
	return ScrSize[1];
}

std::string UIRender::GetRndInfo( unsigned int info )
{
	if( info > 3 )
		return "INVALID INFO";

	return RndInfo[info];
}

bool UIRender::GetFullScreen()
{
	return fullS;
}

void UIRender::EnableFullScreen()
{
	if( fullS )
		return;

	SetWindowLong( hWnd, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
	SetWindowLong( hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW );
	ShowCursor( false );
	// struktura trzymaj¹ca konfiguracje zmiany parametrów ekranu			
	DEVMODE dmScreenSettings;

    // Musimy t¹ strukturê przeczyœciæ (nadpisaæ zerami), by nie mieæ ¿adnych niespodzianek					
	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);
	
    //ustawiamy strukture	
	dmScreenSettings.dmPelsWidth	= ScrSize[0];	//Szerokoœæ ekranu
	dmScreenSettings.dmPelsHeight	= ScrSize[1];	//Wysokoœæ ekranu
	dmScreenSettings.dmBitsPerPel	= 32;		//G³êbia kolorów

	// Ostatnie ustawienie mówi, co zrobiæ z Pixelami
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	/*	Próba zmiany rozdzielczoœci (CDS_FULLSCREEN usuwa 
		panel startu, oraz przywraca star¹ konfiguracje ekranu
		przy kolejnej próbie zmiany rozdzielczoœæ )
	*/
	Log.Log( "Zmiana rozdzielczoœci" );
	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		Log.Error( "Nie mo¿na zmieniæ rozdzielczoœci!" );
		//Je¿eli nieudana, proponujemy dzia³anie okienkowe
		if (MessageBox(NULL,"Nie mo¿na zmieniæ rozdzielczoœci. Prawdopodobnie\ntwoja karta graficzna nie obs³uguje trybu Pe³noekranowego.\nU¿yæ trybu okienkowego?","OpenGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
		{
			Log.Report( "Zmiana trybu na Okienkowy" );
            //Je¿eli tak to wykonujemy
			fullS = false;
		}
		else
		{
			//Je¿eli nie to koñczymy dzia³anie (dajemy komunikat, ¿e progam zostanie zamkniêty)
			MessageBox(NULL,"Program zostanie zamkniêty.","B£¥D",MB_OK|MB_ICONSTOP);
			Log.Report( "Wy³¹czenie aplikacji" );
			return;	//przerywamy dzia³anie aplikacji
		}
	}

	RECT		WindowRect;				//Wielkoœæ okna (struktura czworoboku)
	WindowRect.left		= (long)0;			//Lewa krawêdŸ na 0
	WindowRect.right	= (long)ScrSize[0];	//Prawa krawêdŸ na wartoœæ Szerokosc
	WindowRect.top		= (long)0;			//Górna krawêdŸ na 0
	WindowRect.bottom	= (long)ScrSize[1];	//Dolna krawêdŸ na wartoœæ Wysokosc

	AdjustWindowRectEx( &WindowRect, WS_POPUP, false, WS_EX_APPWINDOW );		

	SetWindowPos( hWnd, HWND_TOP, WindowRect.left, WindowRect.top, WindowRect.right, WindowRect.bottom, SWP_SHOWWINDOW );
	ShowWindow( hWnd, SW_SHOW );

	fullS = true;
}

void UIRender::DisableFullScreen()
{
	if( !fullS )
		return;

	SetWindowLong( hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
	SetWindowLong( hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );
	ShowCursor( true );
	RECT		WindowRect;				//Wielkoœæ okna (struktura czworoboku)
	WindowRect.left		= (long)0;			//Lewa krawêdŸ na 0
	WindowRect.right	= (long)ScrSize[0];	//Prawa krawêdŸ na wartoœæ Szerokosc
	WindowRect.top		= (long)0;			//Górna krawêdŸ na 0
	WindowRect.bottom	= (long)ScrSize[1];	//Dolna krawêdŸ na wartoœæ Wysokosc

	AdjustWindowRectEx( &WindowRect, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );		

	SetWindowPos( hWnd, HWND_TOP, 0, 0, WindowRect.right, WindowRect.bottom, SWP_SHOWWINDOW );

	Log.Report( "Przywracanie rozdzielczoœci..." );
	ChangeDisplaySettings(NULL,0);	
	ShowWindow( hWnd, SW_SHOW );

	fullS = false;
}

void UIRender::VSync( unsigned int set )
{
/*	PFNWGLSWAPINTERVALPROC   wglSwapIntervalEXT; 
	wglSwapIntervalEXT    = (PFNWGLSWAPINTERVALPROC) wglGetProcAddress("wglSwapIntervalEXT"); 
	if( wglSwapIntervalEXT )wglSwapIntervalEXT(set);*/
}