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
	/* Na pocz¹tku musimy ustawiæ zmienne pomocnicze */

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

	/*	By stworzyæ okno, potrzebna jest jego definicja,
	a tak¹ mo¿emy przekazaæ, przez strukture typu
	WNDCLASS.
	*/
	// Okreœlamy paramerty okna dziêki strukturze WNDCLASS
	WNDCLASSEX	wc;				//Struktura klasy okna
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);

	/*	Styl Bezwglêdny okna
	Tu ustawiamy, ¿e okno mo¿e
	byæ odrysowywane, oraz, ¿e 
	posiada w³asny kontekst.
	*/	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	

	// Tu podajemy funkcje obs³ugi komunikatów tego okna.
	wc.lpfnWndProc		= windowProc;	

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
	wc.lpszClassName	= WindowClassName.c_str();	//Nazwa klasy okna

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

	//Ustawiamy styl okna w zale¿noœci, czy jest ustawiony pe³ny ekran
	if( fullscreen )	
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

	if( client )
		AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );		

	// Ostatecznie tworzymy okno
	if (!(hWindow = CreateWindowEx(	dwExStyle,						//Styl zewnêtrzny okna
		WindowClassName.c_str(),			//Nazwa klasy okna
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
		UnregisterClass( WindowClassName.c_str(), hInstance );
		Log.FatalError( "B³¹d tworzenia okna!" );
		return false;	//koñczymy dzia³anie
	}

	SetWindowLongPtr( hWindow, GWLP_USERDATA, (LONG_PTR)pUserData );
	Title = title;

	if( fullscreen )
		EnableFullScreen();

	return true;
}

void	CRenderWindow::Destroy()
{
	// Teraz potrzeba usun¹æ okno
	if (hWindow != nullptr)
	{
		SetWindowLongPtr( hWindow, GWLP_USERDATA, 0 );
		if(!DestroyWindow(hWindow))
			Log.FatalError( "Nie mo¿na usun¹æ okna!" );

		hWindow = nullptr;	//Zerujemy uchwyt okna

		// wypada³oby jeszcze odrejestrowaæ klasê okna
		if (!UnregisterClass( WindowClassName.c_str(), hInstance ))
		{
			Log.FatalError( "Nie mo¿na odrejestrowaæ klasy!" );
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

	// struktura trzymaj¹ca konfiguracje zmiany parametrów ekranu			
	DEVMODE dmScreenSettings;

    // Musimy t¹ strukturê przeczyœciæ (nadpisaæ zerami), by nie mieæ ¿adnych niespodzianek					
	memset( &dmScreenSettings, 0, sizeof(dmScreenSettings) );	
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);

	RECT rect;
	GetWindowRect( hWindow, &rect );
	
    //ustawiamy strukture	
	dmScreenSettings.dmPelsWidth	= rect.right - rect.left;	//Szerokoœæ ekranu
	dmScreenSettings.dmPelsHeight	= rect.bottom - rect.top;	//Wysokoœæ ekranu

	unsigned bits = colorBits;
	if( bits == 0 )
		bits = ColorBits;

	dmScreenSettings.dmBitsPerPel	= bits;		//G³êbia kolorów
	dmScreenSettings.dmFields = (bits != 0 ? DM_BITSPERPEL : 0) | DM_PELSWIDTH | DM_PELSHEIGHT;

	/*	Próba zmiany rozdzielczoœci (CDS_FULLSCREEN usuwa 
		panel startu, oraz przywraca star¹ konfiguracje ekranu
		przy kolejnej próbie zmiany rozdzielczoœæ )
	*/
	Log.Log( "Zmiana rozdzielczoœci" );
	if (ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL)
	{
		Log.Error( "Nie mo¿na zmieniæ rozdzielczoœci!" );
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

	Log.Report( "Przywracanie rozdzielczoœci..." );
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
		Log.Error( "Nie uda³o siê zmieniæ nazwy okna " + Title + " na " + title );
	Title = title;
}

void	CRenderWindow::SetPos( const int x, const int y )
{
	if( !SetWindowPos( hWindow, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE ) )
		Log.Error( "Nie uda³o siê zmieniæ pozycji okna " + Title );
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