#include "Application.h"
#include "EventInput.h"
#include "GamePlayerController.h"

#include <windowsx.h>

#include "gui.h"
#include "GamePlayer.h"
#include "Level.h"
#include "inifile.h"


CApplication::CApplication() :
	active( true ),
	State(GAME_STATE::MAINMENU),
	WindowWidth( 640 ),
	WindowHeight( 480 ),
	MouseX( 0 ),
	MouseY( 0 )
{
	memset( Keys, 0, sizeof(bool) * 256 );

	POINT cursorPos;
	if( GetCursorPos( &cursorPos ) )
	{
		MouseX = cursorPos.x;
		MouseY = cursorPos.y;
	}

	RegScript();
}

CApplication::~CApplication()
{

}

/*	FUNKCJA KOMUNIKAT�W
By okno sprawnie pracowa�o, potrzebna
jest funkcja komunikat�w, tzw. Window Processing.
Interpretujemy tutaj komunikaty by okno, np. Zmieni�o
szeroko��. Tu w�a�nie si� pojawia znany problem "Program
Nie Odpowiada", bo pewnie jest tak zaj�ty, �e nie ma czasu
uruchomi� funkcji komunikat�w.
*/
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Uchwyt do okna
						 UINT	uMsg,			// Komunikaty
						 WPARAM	wParam,			// Dodatkowe informacje
						 LPARAM	lParam)			// Dodatkowe informacje
{
	auto ptr = GetWindowLongPtr( hWnd, GWLP_USERDATA );
	if( ptr != 0 )
	{
		auto pApp = (CApplication*)ptr;
		if(pApp->ProcessMsg( hWnd, uMsg, wParam, lParam ))
			return 0;
	}

	// przeka� reszte komunikat�w do DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int	CApplication::Run()
{
	// Czytamy plik ini, by odpowiednio wszystko ustawi�
	IniFile ini;
	ini.Open( "config.ini" );
	WindowWidth = ini.ReadInt( "GRAPHIC", "WIDTH", 640 );
	WindowHeight = ini.ReadInt( "GRAPHIC", "HEIGHT", 480 );
	bool fullscreen = ini.ReadBool( "GRAPHIC", "FULLSCREEN", false );
	GUI.SetMaxSpecial( ini.ReadInt( "SPECIAL", "MAXCOUNT", 0 ) );
	GUI.SetCanSmoke( ini.ReadBool( "SPECIAL", "SMOKE", true ) );
	ini.Close();

	// Stw�rz okno
	if ( !GLRender.GLCreateWindow( "Expert 3D Tournament", WindowWidth, WindowHeight, fullscreen, (WNDPROC)WndProc, this ) )
		return 0;									// Wyjd� je�eli nie zosta�o stworzone

	CTexManager texManager( "Data/Textures/" );
	CModelManager modelManager( "Data/Models/", texManager );
	CLevel level( texManager, modelManager );

	pGLevel = &level;
	CLocalPlayerController* pController = new CLocalPlayerController( level.GetPlayer() );
	ControllerList.AddController( pController );
	EventManager.AddObserver( pController );

	Log.Log( "Inicjalizacja OpenGL" );

	InitGraphics( texManager );

	MainLoop();

	Log.Log( "Koniec pracy Aplikacji" );
	GLRender.GLDestroyWindow();// Zniszcz okno
	return 0;
}

const bool	CApplication::ProcessMsg( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)									// Sprawd� komunikaty
	{
	case WM_ACTIVATE:							// komunikar Aktywno�ci
		{
			if (!HIWORD(wParam))					// Sprawd� altywno��
			{
				active=true;						// Program jest aktywny
			}
			else
			{
				active=false;						// Program ju� nie jest aktywny
			}

			return true;								// Spowrotem do p�tli
		}

	case WM_SYSCOMMAND:							// Sprawd� komunikaty systemowe
		{
			switch (wParam)	
			{
			case SC_SCREENSAVE:					// Wygaszaczpr�buje si� w��czy�?
			case SC_MONITORPOWER:				// Monitor pr�buje w��czy� oszcz�dzanie pr�du?
				return true;							// Zapobiegaj
			}
			return false;									// wyjd�
		}

	case WM_CLOSE:								// Komunikat zamkni�cia
		{
			PostQuitMessage(0);						// Wysy�amy zamkni�cie
			return true;								
		}

	case WM_LBUTTONDOWN:
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYDOWN, VK_LBUTTON );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );
			
			Keys[VK_LBUTTON] = true;
			return true;
		}

	case WM_LBUTTONUP:
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYUP, VK_LBUTTON );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );

			Keys[VK_LBUTTON] = false;
			return true;
		}

	case WM_RBUTTONDOWN:
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYDOWN, VK_RBUTTON );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );

			Keys[VK_RBUTTON] = true;
			return true;
		}

	case WM_RBUTTONUP:
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYUP, VK_RBUTTON );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );

			Keys[VK_RBUTTON] = false;
			return true;
		}

	case WM_KEYDOWN:							// Klawisz jest naci�nieni�ty?
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYDOWN, (unsigned)wParam );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );			

			GUI.ParseKey( (char)wParam );
			Keys[wParam] = true;					// Zaznacz �e jest wci�ni�ty
			return true;								
		}

	case WM_KEYUP:								// Klawisz jest puszczony?
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYUP, (unsigned)wParam );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );			

			Keys[wParam] = false;					// Zaznacz �e jest wolny
			return true;								
		}

	case WM_SIZE:								// Zmiana rozmiar�w
		{
			GLRender.Resize(LOWORD(lParam),HIWORD(lParam));  // LoWord=szeroko��, HiWord=wysoko��
			return true;								
		}

	case WM_MOUSEMOVE:
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			CEventMouse MouseEvent( EVENT_INPUT_TYPE::MOUSEMOVE, x, y, x - MouseX, y - MouseY );
			
			MouseX = x;
			MouseY = y;

			EventManager.AddEvent( *((CEvent*)&MouseEvent) );

			return true;
		}
	}

	return false;
}

void	CApplication::InitGraphics( CTexManager& texManager )
{
	glDepthFunc( GL_LEQUAL );				//Metoda testowania G��boko�ci (ta jest lepsza)
	glEnable( GL_TEXTURE_2D );
	GUI.InitGUI( &texManager );
	//GUI.SetLoadLevelFunc( LoadLevel );
	GUI.SendConMsg( "=====EXPERT 3D TOURNAMENT ENGINE=====", false );
	GUI.SendConMsg( "STEROWNIK: " + GLRender.GetRndInfo( RENDER_RENDERER ), false );
	GUI.SendConMsg( "WERSJA: " + GLRender.GetRndInfo( RENDER_VERSION ), false );
	GUI.SendConMsg( "INICJALIZACJA GLOWNA", false );

	GUI.SendConMsg( "Ustawianie OpenGL...", false );
	glShadeModel( GL_SMOOTH );			    //Ustawienie �adnego cieniowania
	glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );	//Ustawienie koloru czyszczenia bufora kolor�w
	glClearDepth( 1.0f );					//Ustwienie gl�boko�ci czyszczenia bufora g��boko�ci
	glEnable( GL_DEPTH_TEST );				//W��czenie test g��boko�ci

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );//Standardowa funkcja przezroczysto�ci 
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );//Najlepsze obliczenia perspektywy
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_FOG_HINT, GL_NICEST );
	glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

	glFogi( GL_FOG_MODE, GL_EXP2 );
	glFogf( GL_FOG_DENSITY, 0.023f );
	glFogf( GL_FOG_START, 70.0f );
	glFogf( GL_FOG_END, 100.0f );
	float fc[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glFogfv( GL_FOG_COLOR, fc );
	glEnable( GL_FOG );

	//double p1[] = { 0.0, 1.0, 0.0, -Vector3f( 0.0f, -5.01f, 0.0f ).Dot( Vector3f( 0.0f, 1.0f, 0.0f ) ) };
	//double p2[] = { 0.0, -1.0, 0.0, -Vector3f( 0.0f, -5.01f, 0.0f ).Dot( Vector3f( 0.0f, -1.0f, 0.0f ) ) };
	//glClipPlane( GL_CLIP_PLANE0, p1 );
	//glClipPlane( GL_CLIP_PLANE1, p2 );

	//glEnable( GL_CLIP_PLANE0 );
	glEnable( GL_COLOR_MATERIAL );
	//glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );

#ifdef LIGHT_TEST
	float lDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float lSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightfv( GL_LIGHT0, GL_DIFFUSE, lDiffuse );
	glLightfv( GL_LIGHT0, GL_AMBIENT, lAmbient );
	glLightfv( GL_LIGHT0, GL_SPECULAR, lSpecular );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, lDiffuse );
	glLightfv( GL_LIGHT1, GL_AMBIENT, lAmbient );
	glLightfv( GL_LIGHT1, GL_SPECULAR, lSpecular );
	glEnable( GL_LIGHTING );
#endif

	GUI.SendConMsg( "Zakonczono", false );

	GUI.SendConMsg( "Inicjalizacja silnika...", false );
	//SMBlur.Init();
	//MenuModel = modelManager.Get( "menumodel.glm" );
	//MainPlayer.SetArmor( 100.0f );
	GUI.Menu.Load( "Data/menu.mnu" );
	//SEManager.MaxSpec = 100;
	srand( GetTickCount() );

	GUI.SendConMsg( "Zakonczono", false );
	GUI.SendConMsg( "INICJALIZACJA GLOWNA ZAKONCZONA", false );

}

void	CApplication::MainLoop()
{
	MSG		msg;									// Struktura komunikat�w windowsa
	bool done = false;
	const float	TIME_STEP = 0.005f;
	float	frameTime = 0.0f;
	CTimer	timer;

	while(!done)									// P�tla g��wna (dopuki done nie jest true)
	{
		frameTime += timer.GetDT();

		for( unsigned i = 0; i < 20 && PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ); i++ )	// Czy otrzymano komunikat?
		{
			if ( msg.message == WM_QUIT )				// Czy to komunikat wyj�cia?
			{
				done = true;							// Je�eli tak to wychodzimy z p�tli
				break;
			}
			else									// Je�eli nie to zajmij si� komunikatami
			{
				TranslateMessage( &msg );				// T�umacz komunikat
				DispatchMessage( &msg );				// Wykonaj komunikat
			}
		}

		GUI.UpdateCounter();

		// Rysujemy scene
		if (active)								// Program jest aktywny?
		{
			if ( GUI.GetQuit() )				// Czy by� wci�ni�ty ESC?
			{
				done = true;						// Je�eli tak to wychodzimy z p�tli
			}
			else								// Je�eli nie to rysujemy
			{
				Render();						// Rysujemy scene

				EventManager.ProcessEvents();
				for(unsigned i = 0; i < 20 && frameTime > TIME_STEP; i++)
				{
					Update(TIME_STEP);
					frameTime -= TIME_STEP;
				}

				GLRender.SwapBuffers();				// Prze�anczamy bufory
			}

		}
		else
			WaitMessage();

		timer.Update();
	}
}

void	CApplication::Mouse()
{
	/*	Najpierw standardowa strukturka windows.
	Jest to eee... PUNKT :) kt�ry zawiera A�
	2 sk�adowe ( x, y ) :P
	*/
	POINT mpos;
	
	/*	Teraz musimy mie� punk oparcia o ile ruszono
	mysz�. Ja wybra�em �rodek ekranu, z t�d
	korzystam z klasy UIRender by uzyska� szeroko��
	i wysoko�� oraz dziele je na p�.
	*/
	int m_wid = GLRender.GetWidth() / 2;
	int m_hei = GLRender.GetHeight() / 2;

	// Dwie zmienne pomocnicze, trzymaj�ce stopie� wychylenia
	float mov_x;// mov_y,

	/*	U�ywamy pi�knej funkcji Windows, by wychwyci� po�orzenie
	kursora wzgl�dem okna.
	*/
	GetCursorPos( &mpos );

	/*	Wyliczamy r�znice mi�dzy po�orzeniem 
	punktu na ekranie, a �rodkiem ekranu
	i zmniejszamy j� dziesi�ciokrotnie.
	( by�aby za du�a ).
	*/
	mov_x = (float)(mpos.x - m_wid) / 10.0f;

	if( pGLevel != nullptr )
	{
		/*	Modyfikujemy stopie� obrotu gracza.	*/
		//pGLevel->GetPlayer().ModAngle( mov_x );
	}

	// W ko�cu ustawiamy kursor na �rodek ekranu.
	SetCursorPos( m_wid, m_hei );
}

void	CApplication::Update( const float fTD )
{
	GUI.ParseKeys( Keys );
	GUI.DoGUIEngine(fTD);

	if( State != GAME_STATE::LEVEL )
		return;

	ControllerList.Update();

	Mouse();

	pGLevel->Update( fTD );
	pGLevel->GetPlayer().ParseKeys( Keys );

	GUI.PInfo.HEALTH = pGLevel->GetPlayer().GetHealth();
	GUI.PInfo.ARMOR = pGLevel->GetPlayer().GetArmor();
	GUI.PInfo.angle = pGLevel->GetPlayer().GetAngle();
}

void	CApplication::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Czyszczenie bufor�w

	if( State == GAME_STATE::LEVEL )
	{
		GLRender.SetPerspective( GUI.GetEyeAngle(), 4, 3, 1.0f, 100.0f );
		glLoadIdentity();	//Reset uk�adu wsp�rz�dnych

		glRotatef( pGLevel->GetPlayer().GetAngle(), 0.0f, 1.0f, 0.0f );
		glTranslatef( -pGLevel->GetPlayer().Pos.X, 0, pGLevel->GetPlayer().Pos.Z );

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		pGLevel->Render();

		glDepthMask( 0 );
		//SEManager.Render();
		//pGLevel->DrawReflect();
		glDepthMask( 1 );

		GLRender.SetPerspective( 45.0f, 4, 3, 1.0f, 10.0f );
		glClear( GL_DEPTH_BUFFER_BIT );	//Czyszczenie bufor�w
		glLoadIdentity();
		pGLevel->GetPlayer().Render();
	}

	//if( GUI.Menu.IsEnabled() && !pGLevel->GetLoaded() )
	//{
	//	GLRender.SetPerspective( GUI.GetEyeAngle(), 4, 3, 1.0f, 100.0f );
	//	glLoadIdentity();	//Reset uk�adu wsp�rz�dnych

	//	glTranslatef( 0.0f, 0.0f, -10.0f );
	//	glRotatef( menuModelRot, 0.5f, 0.5f, 1.0f );
	//	MenuModel->CallObject( 0 );
	//}

	GUI.DoGUIDraw();
}

void	CApplication::LoadLevel( const std::string& filename )
{
	if( pGLevel->LoadLevel( filename ) )
	{
		pGLevel->InitLevel();

		GUI.LevName = pGLevel->GetLevelName();
		GUI.EnableMainEngine();
		GUI.EnableGGUI();
		State = GAME_STATE::LEVEL;
	}
	else
	{
		GUI.SendConMsg( "Nie mozna znalesc pliku", false );
	}
}