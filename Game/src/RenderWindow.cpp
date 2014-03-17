#include "RenderWindow.h"
#include "Log.h"

CRenderWindow::CRenderWindow( HINSTANCE instance, const std::string& classname ) :
	hInstance( instance ),
	hWindow( nullptr ),
	WindowClassName(classname),
	FullScreen( false ),
	ColorBits( 0 )
{
}

CRenderWindow::~CRenderWindow()
{
	Destroy();
}

const bool	CRenderWindow::Create( const std::string& title, const int x, const int y, const unsigned width, const unsigned height, const bool fullscreen, WNDPROC windowProc, void* pUserData, const bool client )
{
	if(DoesExist())
		Destroy();

	Log.Log( "Tworzenie Okna" );
	/* Na pocz�tku musimy ustawi� zmienne pomocnicze */

	/*	Potrzebny jest jeszcze tylko prostok�t. Jest potrzebny
	By okre�li� rozmiar i po�orzenie okna. Jest to po prostu
	struktura z 4 zmiennymi typu long int.
	*/
	RECT		WindowRect;		//Wielko�� okna (struktura czworoboku)

	/*	Na pocz�tku ustawiamy prostok�t okna. */
	WindowRect.left		= (long)0;		//Lewa kraw�d� na 0
	WindowRect.right	= (long)width;	//Prawa kraw�d� na warto�� Szerokosc
	WindowRect.top		= (long)0;		//G�rna kraw�d� na 0
	WindowRect.bottom	= (long)height;	//Dolna kraw�d� na warto�� Wysokosc

	/*	By stworzy� okno, potrzebna jest jego definicja,
	a tak� mo�emy przekaza�, przez strukture typu
	WNDCLASS.
	*/
	// Okre�lamy paramerty okna dzi�ki strukturze WNDCLASS
	WNDCLASSEX	wc;				//Struktura klasy okna
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);

	/*	Styl Bezwgl�dny okna
	Tu ustawiamy, �e okno mo�e
	by� odrysowywane, oraz, �e 
	posiada w�asny kontekst.
	*/	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	

	// Tu podajemy funkcje obs�ugi komunikat�w tego okna.
	wc.lpfnWndProc		= windowProc;	

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
	wc.lpszClassName	= WindowClassName.c_str();	//Nazwa klasy okna

	/*	Teraz rejestrujemy klas� okna.
	Windows posiada sw�j w�asny rejestr klas okna
	i �eby stworzy� jakie� nowe okno, to trzeba
	poda� jego klas�. Jako, �e my chcemy stworzy�
	niestandardowe okno ( z OpenGL ) to musimy
	zarejestrowa� w�asn� klas� okna.
	*/
	if (!RegisterClassEx(&wc)) //Pr�ba rejestracji okna
	{
		//Gdy nieudana, wy�wietlamy odpowieni komunikat
		Log.FatalError( "Nie mo�na zarejestrowa� klasy okna!" );
		return false;	//przerwij i zwr��, �e inicjalizacja nie udana.
	}

	//Ustawiamy styl okna w zale�no�ci, czy jest ustawiony pe�ny ekran
	if( fullscreen )	
	{
		dwExStyle = WS_EX_APPWINDOW;	// ? 
		dwStyle = WS_POPUP;				// Okienko bez niczego ( tylko �rodek okna )
		ShowCursor( false );			// Schowaj kursor myszy
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// ?
		dwStyle = WS_OVERLAPPEDWINDOW;	/*	Okno z belk� (kt�ra ma wszystkie przyciski typu
										Minimalizuj, itp ), z kraw�dzi� i tytu�em
										*/
	}

	if( client )
		AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );		

	// Ostatecznie tworzymy okno
	if (!(hWindow = CreateWindowEx(	dwExStyle,						//Styl zewn�trzny okna
		WindowClassName.c_str(),			//Nazwa klasy okna
		title.c_str(),						//Tytu� okna (na belce)
		dwStyle |							//Definicja stylu
		WS_CLIPSIBLINGS |					
		WS_CLIPCHILDREN,					
		0, 0,								//Pozycja okna
		WindowRect.right - WindowRect.left,	//Szeroko�� okna (z oblicze�)
		WindowRect.bottom - WindowRect.top,	//Wysoko�� okna (z oblicze�)
		NULL,								//Brak okna-matki
		NULL,								//Bez Menu
		hInstance,							//Uchwyt pami�ci
		NULL)))								//Nic nie informuj przez WM_CREATE
	{
		//Je�eli si� nie uda					
		UnregisterClass( WindowClassName.c_str(), hInstance );
		Log.FatalError( "B��d tworzenia okna!" );
		return false;	//ko�czymy dzia�anie
	}

	SetWindowLongPtr( hWindow, GWLP_USERDATA, (LONG_PTR)pUserData );
	Title = title;

	if( fullscreen )
		EnableFullScreen();

	return true;
}

void	CRenderWindow::Destroy()
{
	// Teraz potrzeba usun�� okno
	if (hWindow != nullptr)
	{
		SetWindowLongPtr( hWindow, GWLP_USERDATA, 0 );
		if(!DestroyWindow(hWindow))
			Log.FatalError( "Nie mo�na usun�� okna!" );

		hWindow = nullptr;	//Zerujemy uchwyt okna

		// wypada�oby jeszcze odrejestrowa� klas� okna
		if (!UnregisterClass( WindowClassName.c_str(), hInstance ))
		{
			Log.FatalError( "Nie mo�na odrejestrowa� klasy!" );
			hInstance = 0;
		}
	}

	if( FullScreen )
		DisableFullScreen();
}

const bool	CRenderWindow::IsFullScreen() const
{
	return FullScreen;
}

void	CRenderWindow::EnableFullScreen( const unsigned colorBits )
{
	if( FullScreen || hWindow == nullptr )
		return;

	ShowCursor( false );

	// struktura trzymaj�ca konfiguracje zmiany parametr�w ekranu			
	DEVMODE dmScreenSettings;

    // Musimy t� struktur� przeczy�ci� (nadpisa� zerami), by nie mie� �adnych niespodzianek					
	memset( &dmScreenSettings, 0, sizeof(dmScreenSettings) );	
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);

	RECT rect;
	GetWindowRect( hWindow, &rect );
	
    //ustawiamy strukture	
	dmScreenSettings.dmPelsWidth	= rect.right - rect.left;	//Szeroko�� ekranu
	dmScreenSettings.dmPelsHeight	= rect.bottom - rect.top;	//Wysoko�� ekranu

	unsigned bits = colorBits;
	if( bits == 0 )
		bits = ColorBits;

	dmScreenSettings.dmBitsPerPel	= bits;		//G��bia kolor�w
	dmScreenSettings.dmFields = (bits != 0 ? DM_BITSPERPEL : 0) | DM_PELSWIDTH | DM_PELSHEIGHT;

	/*	Pr�ba zmiany rozdzielczo�ci (CDS_FULLSCREEN usuwa 
		panel startu, oraz przywraca star� konfiguracje ekranu
		przy kolejnej pr�bie zmiany rozdzielczo�� )
	*/
	Log.Log( "Zmiana rozdzielczo�ci" );
	if (ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL)
	{
		Log.Error( "Nie mo�na zmieni� rozdzielczo�ci!" );
		FullScreen = false;
		return;
	}

	SetWindowLong( hWindow, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
	SetWindowLong( hWindow, GWL_EXSTYLE, WS_EX_APPWINDOW );

	ColorBits = bits;
	FullScreen = true;
}

void CRenderWindow::DisableFullScreen()
{
	if( !FullScreen )
		return;

	Log.Report( "Przywracanie rozdzielczo�ci..." );
	ShowCursor( true );
	ChangeDisplaySettings(NULL,0);	

	if(hWindow != nullptr)
	{
		SetWindowLong( hWindow, GWL_STYLE, dwStyle );
		SetWindowLong( hWindow, GWL_EXSTYLE, dwExStyle );
	}

	FullScreen = false;
}


void	CRenderWindow::SetTitle( const std::string& title )
{
	if( !SetWindowText( hWindow, title.c_str() ) )
		Log.Error( "Nie uda�o si� zmieni� nazwy okna " + Title + " na " + title );
	Title = title;
}

void	CRenderWindow::SetPos( const int x, const int y )
{
	if( !SetWindowPos( hWindow, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE ) )
		Log.Error( "Nie uda�o si� zmieni� pozycji okna " + Title );
}

void	CRenderWindow::SetSize( const unsigned width, const unsigned height, const bool client )
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	if( client )
		AdjustWindowRectEx( &rect, dwStyle, FALSE, dwExStyle );

	SetWindowPos( hWindow, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE );
}

void	CRenderWindow::SetVisible( const bool visible )
{
	ShowWindow( hWindow, (visible ? SW_SHOW : SW_HIDE ) );
}

const unsigned	CRenderWindow::GetWidth( const bool client ) const
{
	RECT rect = { 0 };
	if( client )
		GetClientRect( hWindow, &rect );
	else
		GetWindowRect( hWindow, &rect );

	return rect.right - rect.left;
}

const unsigned	CRenderWindow::GetHeight( const bool client ) const
{
	RECT rect = { 0 };
	if( client )
		GetClientRect( hWindow, &rect );
	else
		GetWindowRect( hWindow, &rect );

	return rect.bottom - rect.top;
}

const int	CRenderWindow::GetX() const
{
	RECT rect = { 0 };
	GetWindowRect( hWindow, &rect );

	return rect.left;
}

const int	CRenderWindow::GetY() const
{
	RECT rect = { 0 };
	GetWindowRect( hWindow, &rect );

	return rect.top;
}

const bool	CRenderWindow::IsVisible() const
{
	return ShowWindow( hWindow, 0 ) != 0 ? true : false;
}

const bool	CRenderWindow::DoesExist() const
{
	return hWindow != nullptr;
}

HWND	CRenderWindow::GetHandle() const
{
	return hWindow;
}