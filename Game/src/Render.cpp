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
	hRC = NULL; // �adnego kontekstu, dlatego
	hWnd = NULL;// zerujemy te zmienne
	hInstance = NULL;
	
	IsPers = false; // Na poczatku nie potrzebujemy perspektywy
	fullS = true;	// Na pocz�tku lepiej ustawi� pe�ny ekran
	IsWin = false;	// Przecie� na pocz�tku nie mamy okna!
	ColBits = -1;	// Domy�lna warto�� dla bit�w kolor�w

	// Domy�lne warto�ci dla rozmiaru okna ( patrz->deklaracja )
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
		RndInfo domy�lnie s� puste, bo informacje otrzymamy
		dopiero po inicjalizacji kontekstu renderuj�cego (hRC).
		Klasa std::string automatycznie ju� ustawi�a puste
		�a�cuchy znak�w.
	*/
}

/*	DESTRUKTOR	*/
UIRender::~UIRender()
{

}

/*	TWORZENIE OKNA
	Maj�c Uchwyt mo�emu ju� stworzy� okno... No prawie :)
	Potrzebne jeszcze dane to:
	Tytul	- Tytu� okna ( ten napis na belce )
	Szerokosc, Wysokosc - Wysoko�� i Szeroko�� okna, jak� sobie �yczymy
	FullScr	- Czy ma by� na pe�nym ekranie
	WndProc	- Funkcja obs�ugi komunikat�w Windowsa ( musi by� z zewn�trz, takie zasady C++ :P )
*/
bool UIRender::GLCreateWindow( std::string Tytul, int Szerokosc, int Wysokosc, bool FullScr, WNDPROC WndProc )
{
	Log.Log( "Tworzenie Okna" );
	/* Na pocz�tku musimy ustawi� zmienne pomocnicze */

	/*	By stworzy� okno, potrzebna jest jego definicja,
		a tak� mo�emy przekaza�, przez strukture typu
		WNDCLASS.
	*/
	WNDCLASS	wc;				//Struktura klasy okna

	/*	Dalej s� tzw. style okna. dwStyle trzyma
		bezwzgl�dny styl ( taki jak, czy to okno jest wyskakuj�ce, czy zewn�rzne )
		dwExStyle trzyma wygl�d okna ( np. czy ma kraw�dzie i belk� tytu�ow� ).
		Oba typy s� DWORD ( czyli unsigned int, jakby kto� pyta� )
	*/
	DWORD		dwExStyle;				//Zewn�trzny styl okna
	DWORD		dwStyle;				//Styl Okna

	/*	Potrzebny jest jeszcze tylko prostok�t. Jest potrzebny
		By okre�li� rozmiar i po�orzenie okna. Jest to po prostu
		struktura z 4 zmiennymi typu long int.
	*/
	RECT		WindowRect;				//Wielko�� okna (struktura czworoboku)

	/*	Na pocz�tku ustawiamy prostok�t okna. */
	WindowRect.left		= (long)0;			//Lewa kraw�d� na 0
	WindowRect.right	= (long)Szerokosc;	//Prawa kraw�d� na warto�� Szerokosc
	WindowRect.top		= (long)0;			//G�rna kraw�d� na 0
	WindowRect.bottom	= (long)Wysokosc;	//Dolna kraw�d� na warto�� Wysokosc

	// Przy okazji ustawiamy tablice z rozmiarem, by potem by� �atwiejszy dost�p w ca�ej klasie
	ScrSize[0] = Szerokosc;
	ScrSize[1] = Wysokosc;

	// Oraz ustawiamy czy wszystko jest w pe�nym ekranie
	fullS = FullScr;

	// �apiemy instancje ( jest potrzebna do wi�kszo�ci zada� z oknem )
	hInstance			= GetModuleHandle(NULL);

	// Okre�lamy paramerty okna dzi�ki strukturze WNDCLASS

	/*	Styl Bezwgl�dny okna
		Tu ustawiamy, �e okno mo�e
		by� odrysowywane, oraz, �e 
		posiada w�asny kontekst.
	*/	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	

	// Tu podajemy funkcje obs�ugi komunikat�w tego okna.
	wc.lpfnWndProc		= WndProc;	

	// wtf? Lepiej zostawic w spokoju...
	wc.cbClsExtra		= 0;	// ?
	wc.cbWndExtra		= 0;	// ?		
	
	// Tu podajemy instancje
	wc.hInstance		= hInstance;	

	// �adujemy ikone aplikacji (widoczn� na belce okna) oraz kursor (bez zmia, bo po co?)
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			//Ustaw standardow� ikon�
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			//Ustaw zwyk�y wska�nik myszy

	// A po cholere nam podk�ad i menu, gdy i tak wszystko b�dzie rysowane na wierzchu?
	wc.hbrBackground	= NULL;	//Brak podk�adu
	wc.lpszMenuName		= NULL;	//Bez menu okna

	// Jeszcze potrzebna nazwa tej klasy okna
	wc.lpszClassName	= "OpenGL";	//Nazwa klasy okna

	/*	Teraz rejestrujemy klas� okna.
		Windows posiada sw�j w�asny rejestr klas okna
		i �eby stworzy� jakie� nowe okno, to trzeba
		poda� jego klas�. Jako, �e my chcemy stworzy�
		niestandardowe okno ( z OpenGL ) to musimy
		zarejestrowa� w�asn� klas� okna.
	*/
	if (!RegisterClass(&wc)) //Pr�ba rejestracji okna
	{
	    //Gdy nieudana, wy�wietlamy odpowieni komunikat
		Log.FatalError( "Nie mo�na zarejestrowa� klasy okna!" );
		return false;	//przerwij i zwr��, �e inicjalizacja nie udana.
	}
	
	/*	Teraz ustwiamy pe�ny ekran.
		Mamy ju� do tego wszystkie potrzebne informacje
		( oczywi�cie je�eli chcemy pe�ny ekran.
	*/
	if (fullS)
	{
		// struktura trzymaj�ca konfiguracje zmiany parametr�w ekranu			
		DEVMODE dmScreenSettings;

        // Musimy t� struktur� przeczy�ci� (nadpisa� zerami), by nie mie� �adnych niespodzianek					
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		
        //ustawiamy strukture	
		dmScreenSettings.dmPelsWidth	= ScrSize[0];	//Szeroko�� ekranu
		dmScreenSettings.dmPelsHeight	= ScrSize[1];	//Wysoko�� ekranu
		dmScreenSettings.dmBitsPerPel	= 32;		//G��bia kolor�w

		// Ostatnie ustawienie m�wi, co zrobi� z Pixelami
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		/*	Pr�ba zmiany rozdzielczo�ci (CDS_FULLSCREEN usuwa 
			panel startu, oraz przywraca star� konfiguracje ekranu
			przy kolejnej pr�bie zmiany rozdzielczo�� )
		*/
		Log.Log( "Zmiana rozdzielczo�ci" );
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			Log.Error( "Nie mo�na zmieni� rozdzielczo�ci!" );
			//Je�eli nieudana, proponujemy dzia�anie okienkowe
			if (MessageBox(NULL,"Nie mo�na zmieni� rozdzielczo�ci. Prawdopodobnie\ntwoja karta graficzna nie obs�uguje trybu Pe�noekranowego.\nU�y� trybu okienkowego?","OpenGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				Log.Report( "Zmiana trybu na Okienkowy" );
                //Je�eli tak to wykonujemy
				fullS = false;
			}
			else
			{
				//Je�eli nie to ko�czymy dzia�anie (dajemy komunikat, �e progam zostanie zamkni�ty)
				MessageBox(NULL,"Program zostanie zamkni�ty.","B��D",MB_OK|MB_ICONSTOP);
				Log.Report( "Wy��czenie aplikacji" );
				return false;	//przerywamy dzia�anie aplikacji
			}
		}
	}

	//Ustawiamy styl okna w zale�no�ci, czy jest ustawiony pe�ny ekran
	if( fullS )	
	{
		dwExStyle=WS_EX_APPWINDOW;	// ? 
		dwStyle=WS_POPUP;			// Okienko bez niczego ( tylko �rodek okna )
		ShowCursor( false );		// Schowaj kursor myszy
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// ?
		dwStyle=WS_OVERLAPPEDWINDOW;	/*	Okno z belk� (kt�ra ma wszystkie przyciski typu
											Minimalizuj, itp ), z kraw�dzi� i tytu�em
										*/
	}
	
	//Ustawiamy styl okna
	AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );		

	// Ostatecznie tworzymy okno
	if (!(hWnd=CreateWindowEx(	dwExStyle,							//Styl zewn�trzny okna
								"OpenGL",							//Nazwa klasy okna
								Tytul.c_str(),						//Tytu� okna (na belce)
								dwStyle |							//Definicja stylu
								WS_CLIPSIBLINGS |					
								WS_CLIPCHILDREN,					
								0, 0,								//Pozycja okna
								WindowRect.right-WindowRect.left,	//Szeroko�� okna (z oblicze�)
								WindowRect.bottom-WindowRect.top,	//Wysoko�� okna (z oblicze�)
								NULL,								//Brak okna-matki
								NULL,								//Bez Menu
								hInstance,							//Uchwyt pami�ci
								NULL)))								//Nic nie informuj przez WM_CREATE
	{
	 	//Je�eli si� nie uda					
		GLDestroyWindow();		//Niszczymy okno
		Log.FatalError( "B��d tworzenia okna!" );
		return false;	//ko�czymy dzia�anie
	}

	IsWin = true;

	if( !EnableGL() )
	{
		GLDestroyWindow();
		return false;
	}
	
	Log.Log( "Tworzenie okna zako�czone." );
	return true;
}

/*	INICJACJA OPENGL
	Tu si� odbywa w�a�ciwa inicjacja OpenGL.
	Mo�liwe jest przypisanie klasie zewn�trznego
	uchwytu do okna i inicjalizacja na nim, poprzez
	r�czne wywo�anie tej metody.
*/
bool UIRender::EnableGL()
{
	Log.Log( "Inicjalizacja OpenGL" );
	/*	Na pocz�tku zmienna trzymaj�ca wynik
		wyszukiwania ustawie� dla danego
		formatu ustawie� (dziwnie brzmi, ale jest potrzebne :P )
	*/
	unsigned int   PixelFormat;			//Wynik wyszukiwania

	/*	Najperw musimy wyci�gn�� z Windows aktualn� palet� kolor�w.
		Dlaczego? Nie wiadomo, ale na niekt�reych windowsach na pe�nym
		ekranie, gdy ustawiamy inn� palete ni� jest na pulpicie
		to si� dziej� r�ne nieprzyjemne rzeczy :P. Najpro�ciej
		t� informacje wyci�gn�� przez PixelFormat. Do tego jest
		potrzebny kontekst (DC), z kt�rego wyci�gamy informacje, a
		do tego z kolei potrzebny jest uchwyt okna ( bo windows
		traktuje pulpit jak jedno ogromne okno ).
	*/
	if( ColBits == -1 ) // sprawdzamy, czy casem informacja nie zosta�a ju� wyci�gni�ta
	{
		HDC deskDC = GetWindowDC( GetDesktopWindow() );
		PixelFormat = GetPixelFormat( deskDC );

		// Teraz rozpisujemy otrzyman� informacje
		PIXELFORMATDESCRIPTOR tempPFD;
		DescribePixelFormat( deskDC, PixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &tempPFD );

		// Wreszcie pobieramy upragnion� informacje
		ColBits = tempPFD.cColorBits;
	}

	// Teraz ustwaiamy format pixeli ( jednorazowo, st�d static )
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Rozmiar
		1,								// Numer wersji
		PFD_DRAW_TO_WINDOW |			// Musi wspiera� zwyk�e okno
		PFD_SUPPORT_OPENGL |			// Musi wspiera� OpenGL
		PFD_DOUBLEBUFFER,				// Musi wspiera� podw�jne buforowanie
		PFD_TYPE_RGBA,					// Tryb kolor�w RGBA
		ColBits,						// G��gia kolor�w
		0, 0, 0, 0, 0, 0,				// Ignorowane bity kolor�w (�adne)
		0,								// Bez bufora kana�u Alfa
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,						// Accumulation Bits Ignored
		16,								// Wielko�� bufora g��boko�ci (16bit) 
		0,								// Bufor Stencil (W��czony, na wszelki wypadek)
		0,								// Bufor Aux'a
		PFD_MAIN_PLANE,					// Gdzie rysowa�
		0,								// Zarezerwowane
		0, 0, 0							// Maski Warstw (ignorowane)
	};
	
	// Pobieramy kontekst dla okna z uchwytu (* jest dodane, by uzyska� sam uchwyt, nie wska�nik )
	if (!(hDC=GetDC(hWnd)))
	{
		// Je�eli si� nie udaje
		Log.FatalError( "Nie mo�na stworzy� kontekstu GL!." );
		return false;								
	}

	// Teraz szukamy odpowiednich ustawie� w dost�pnych
	if (!(PixelFormat = ChoosePixelFormat( hDC, &pfd )))
	{
		Log.FatalError( "Nie mo�na znale�� odpowiedniego PixelFormat'u." );
		return false;								
	}

	// Spr�bujmy teraz wprowadzi� nasze ustawienia
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))
	{
		Log.FatalError( "Nie mo�na ustawic Pixel Formatu." );
		return false;								
	}

	/*	Teraz tworzymy kontekst renderuj�cy OpenGL
		Bez niego nie mo�na korzysta� z wi�kszo�ci
		funkcji w OpenGL ( w ka�dym razie - nie 
		ma sensu go nie tworzy� ).]
	*/
	if (!(hRC=wglCreateContext(hDC)))
	{
		Log.FatalError( "Nie mo�na stworzy� kontekstu renderuj�cego!." );
		return false;								
	}

	/*	A teraz trzeba uaktywni� nasz kontekst OpenGL
		Wed�ug specyfikacji mo�emy mie� kilka kontekst�w
		i si� prze��ncza� mi�dzy nimi, ale my mamy tylko
		jeden :P ( proste nie? )
	*/
	if(!wglMakeCurrent(hDC,hRC))
	{
		Log.FatalError( "Nie mo�na aktywowa� kontekstu." );
		return false;								
	}

	/*	Musimy jeszcze przestawi� pare rzeczy.
		Najpierw - pokaza� okno na ekranie,
		Potem - przestawi� priorytet w�tku (bo
		poszczeg�lne okna w Windows s� w w�tkach - 
		dziwne?? ) na troch� wy�szy od normalnego.
		Ustawi� okno, by reagowa�o na klawisze, oraz
		ostatecznie ustawi� viewport prze metode
		Resize().
	*/
	Log.Log( "Ko�czenie okna..." );
	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );
	Resize( ScrSize[0], ScrSize[1] );

	/*	I na ko�cu, kiedy kontekst jest ju� ustawiony
		wreszcie mo�na pobra� dane do RndInfo.
		S�u�y do tego funkcja glGetString(). Problem
		jest jedynie z tym, �e std::string nie akceptuje
		domy�lnie typu const unsigned char* zwracanego przez
		t� funkcje, ale konwersja pomaga :)
	*/
	Log.Log( "Pobieranie Informacji o sprz�cie..." );
	RndInfo[0] = (char*)glGetString( GL_VERSION );
	RndInfo[1] = (char*)glGetString( GL_RENDERER );
	RndInfo[2] = (char*)glGetString( GL_VENDOR );
	RndInfo[3] = (char*)glGetString( GL_EXTENSIONS );
	Log.Report( "Wersja OpenGL: " + RndInfo[0] );
	Log.Report( "Sterownik: " + RndInfo[1] );
	Log.Report( "Dostawca: " + RndInfo[2] );

	// Hura! Sukces, nic si� nie sypne�o :)
	return true;
}

void UIRender::DisableGL()
{
	/*	Je�eli mamy kontekst OpenGL i nie jest
		on zerowy, to wypada�o by go skasowa�,
		ale najpierw trzeba zresetowa� aktywny
		kontekst.
	*/
	if (hRC)
	{
		/*	btw: mo�na by�o by przywr�ci� kontekst jaki by� poprzednio
			aktywny, ale to za du�o roboty :P	*/
		if (!wglMakeCurrent(NULL,NULL))
		{
			// wtf?
			Log.FatalError( "Nie mo�na od��czy� kontekstu!" );
		}

		// Usuwamy kontekst OpenGL
		if (!wglDeleteContext(hRC))
		{
		    // Chyba nie chce :P
			Log.FatalError( "Nie mo�na usun�� kontekstu OpenGL!" );
		}
		hRC=NULL; // Zerujemy kontekst ( wtedy if go nie wychwyci przy nast�pnym wywo�aniu metody )
	}

	// Teraz usuwamy konteks aplikacji - wystarczy go tylko "pu�ci�" i ju� b�dzie wolny.
	if (hDC && !ReleaseDC(hWnd,hDC))
	{
	    // Znowu???
		Log.FatalError( "Nie mo�na usun�� kontekstu aplikacji!" );
		hDC=NULL;	//Zerujemy kontekst
	}
}

/*	NISZCZENIE OKNA
	Tu wystarczy par� prostych funkji, by posprz�ta�
	po metodzie tworzenia okna.
	UWAGA: Gdy tracimy kontekst, to tak�e tracimy
	wszytkie tekstury, list'y, itp. wi�c u�ywa� z
	rozwag�!
*/
void UIRender::GLDestroyWindow()
{
	Log.Log( "Niszczenie Okna" );
	/*	Je�eli ustawili�my pe�ny ekran, to teraz
		Wypada�o by go przywr�ci� do normalnych
		ustawie�. Dzi�ki wcze�niej wprowadzonej
		fladze CDS_FULLSCREEN teraz wystarczy
		wprowadzi� dowoln� warto�� by ekran
		wr�ci� do porz�dku.
	*/
	if (fullS)
	{
		Log.Report( "Przywracanie rozdzielczo�ci..." );
		ChangeDisplaySettings(NULL,0);	
		ShowCursor( true ); // Pokazujemy te� kursor
	}

	DisableGL();

	// Teraz potrzeba usun�� okno
	if (hWnd && !DestroyWindow(hWnd))
	{
	    // alala kotki dwa
		Log.FatalError( "Nie mo�na usun�� okna!" );
		hWnd=NULL;	//Zerujemy uchwyt okna
	}

	// wypada�oby jeszcze odrejestrowa� klas� okna
	if (!UnregisterClass("OpenGL",hInstance))
	{
	    // zgadnijnie co?
		Log.FatalError( "Nie mo�na odrejestrowa� klasy!" );
		hInstance=NULL;
	}

	IsWin = false;
	Log.Log( "Niszczenie okna zako�czone." );
}

void UIRender::Resize( unsigned int szerokosc, unsigned int wysokosc )
{
	if( wysokosc == 0 )	// Ma�e zabezpieczenie 
		wysokosc = 1;	// przed dzieleniem przez zero

	// Ustawiamy Viewport ( taki kwadrat, przez kt�ry b�dziemy widzieli co OpenGL wykombinuje )
	glViewport( 0, 0, szerokosc, wysokosc);

	// W zale�no�ci czy mamy widok perspektywy, czy prostok�tny, to musimy zresetowa� matrix
	if( IsPers )
		SetPerspective( PerConf[0], szerokosc, wysokosc, PerConf[2], PerConf[3] );
	else SetOrtho();
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
void UIRender::SetPerspective( float kat, int szerokosc, int wysokosc, float blisko, float daleko )
{
	if( wysokosc == 0 )	// Zabezpieczenie, przed 
		wysokosc = 1;	// dzieleniem przez zero

	SetPerspective( kat, ( (float)szerokosc / (float)wysokosc ), blisko, daleko );
}

void UIRender::SetPerspective( float kat, float wspl, float blisko, float daleko )
{
	// Zapisanie do warto�ci domy�lnych, by �atwo mo�na by�o
	// Prze�ancza� si� mi�dzy typami rzut�w
	PerConf[0] = kat;
	PerConf[1] = wspl;
	PerConf[2] = blisko;
	PerConf[3] = daleko;

	SetPerspective();
}

void UIRender::SetPerspective()
{
	// A tu jest ta regu�ka ;)

	// Przestawienie trybu matrix'a
	glMatrixMode( GL_PROJECTION );

	//Reset uk�adu wsp�rz�dnych
	glLoadIdentity();	

	//Ustawienie Perspektywy ( K�t widzenia, Rozmiar ekranu, Min widoczno�ci, Max widoczno�ci )
	gluPerspective( PerConf[0], PerConf[1], PerConf[2], PerConf[3] );

	//Znowu zmiana Matrix'a
	glMatrixMode(GL_MODELVIEW);	
	
	//I reset uk�adu
	glLoadIdentity();
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
void UIRender::SetOrtho( float lewo, float prawo, float dol, float gora, float blisko, float daleko )
{
	// Przypisanie nowych warto�ci do tablicy
	OrthoConf[0] = lewo;
	OrthoConf[1] = prawo;
	OrthoConf[2] = dol;
	OrthoConf[3] = gora;
	OrthoConf[4] = blisko;
	OrthoConf[5] = daleko;

	// Wywo�anie w�a�ciwej funkcji
	SetOrtho();
}

void UIRender::SetOrtho()
{
	/*	Jak wida�, ta funkcja niewiele si� r�ni
		od funkcji perspektywy. Dla czytelno�ci
		( i moich palc�w :P ) omin� niekt�re 
		zb�dne komentarze.
	*/

	glMatrixMode( GL_PROJECTION );

	glLoadIdentity();	

	//Ustawienie Rzut. Prostok�tnego ( dla rozszyfrowania parametr�w patrz -> deklaracja tablicy OrthoConf )
	glOrtho( OrthoConf[0], OrthoConf[1], OrthoConf[2], OrthoConf[3], OrthoConf[4], OrthoConf[5] ); 

	glMatrixMode(GL_MODELVIEW);	
	
	glLoadIdentity();
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
void UIRender::SwitchBuf()
{
	SwapBuffers(hDC);
}

/*	FUNKCJE DO KOMUNIKACJI POZA KLAS�	*/
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
	// struktura trzymaj�ca konfiguracje zmiany parametr�w ekranu			
	DEVMODE dmScreenSettings;

    // Musimy t� struktur� przeczy�ci� (nadpisa� zerami), by nie mie� �adnych niespodzianek					
	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);
	
    //ustawiamy strukture	
	dmScreenSettings.dmPelsWidth	= ScrSize[0];	//Szeroko�� ekranu
	dmScreenSettings.dmPelsHeight	= ScrSize[1];	//Wysoko�� ekranu
	dmScreenSettings.dmBitsPerPel	= 32;		//G��bia kolor�w

	// Ostatnie ustawienie m�wi, co zrobi� z Pixelami
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	/*	Pr�ba zmiany rozdzielczo�ci (CDS_FULLSCREEN usuwa 
		panel startu, oraz przywraca star� konfiguracje ekranu
		przy kolejnej pr�bie zmiany rozdzielczo�� )
	*/
	Log.Log( "Zmiana rozdzielczo�ci" );
	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		Log.Error( "Nie mo�na zmieni� rozdzielczo�ci!" );
		//Je�eli nieudana, proponujemy dzia�anie okienkowe
		if (MessageBox(NULL,"Nie mo�na zmieni� rozdzielczo�ci. Prawdopodobnie\ntwoja karta graficzna nie obs�uguje trybu Pe�noekranowego.\nU�y� trybu okienkowego?","OpenGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
		{
			Log.Report( "Zmiana trybu na Okienkowy" );
            //Je�eli tak to wykonujemy
			fullS = false;
		}
		else
		{
			//Je�eli nie to ko�czymy dzia�anie (dajemy komunikat, �e progam zostanie zamkni�ty)
			MessageBox(NULL,"Program zostanie zamkni�ty.","B��D",MB_OK|MB_ICONSTOP);
			Log.Report( "Wy��czenie aplikacji" );
			return;	//przerywamy dzia�anie aplikacji
		}
	}

	RECT		WindowRect;				//Wielko�� okna (struktura czworoboku)
	WindowRect.left		= (long)0;			//Lewa kraw�d� na 0
	WindowRect.right	= (long)ScrSize[0];	//Prawa kraw�d� na warto�� Szerokosc
	WindowRect.top		= (long)0;			//G�rna kraw�d� na 0
	WindowRect.bottom	= (long)ScrSize[1];	//Dolna kraw�d� na warto�� Wysokosc

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
	RECT		WindowRect;				//Wielko�� okna (struktura czworoboku)
	WindowRect.left		= (long)0;			//Lewa kraw�d� na 0
	WindowRect.right	= (long)ScrSize[0];	//Prawa kraw�d� na warto�� Szerokosc
	WindowRect.top		= (long)0;			//G�rna kraw�d� na 0
	WindowRect.bottom	= (long)ScrSize[1];	//Dolna kraw�d� na warto�� Wysokosc

	AdjustWindowRectEx( &WindowRect, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );		

	SetWindowPos( hWnd, HWND_TOP, 0, 0, WindowRect.right, WindowRect.bottom, SWP_SHOWWINDOW );

	Log.Report( "Przywracanie rozdzielczo�ci..." );
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