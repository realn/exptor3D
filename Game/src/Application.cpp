#include "Application.h"
#include "EventInput.h"
#include "GamePlayerController.h"

#include <windowsx.h>

#include "GUI.h"
#include "GamePlayer.h"
#include "Level.h"
#include "inifile.h"


CApplication::CApplication( HINSTANCE hInstance ) :
	active( true ),
	GLWindow( hInstance, "OPENGLWINDOWCLASS3232" ),
	State(GAME_STATE::MAINMENU),
	MouseMode(MOUSE_MODE::MENU),
	ScriptParser(EventManager),
	GUI( nullptr )
{
	RegScript();
}

CApplication::~CApplication()
{
	if( GUI != nullptr )
	{
		delete GUI;
		GUI = nullptr;
	}
	Log.Log( "Koniec pracy Aplikacji" );
}

/*	FUNKCJA KOMUNIKATÓW
By okno sprawnie pracowa³o, potrzebna
jest funkcja komunikatów, tzw. Window Processing.
Interpretujemy tutaj komunikaty by okno, np. Zmieni³o
szerokoœæ. Tu w³aœnie siê pojawia znany problem "Program
Nie Odpowiada", bo pewnie jest tak zajêty, ¿e nie ma czasu
uruchomiæ funkcji komunikatów.
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

	// przeka¿ reszte komunikatów do DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int	CApplication::Run()
{
	srand( GetTickCount() );

	// Czytamy plik ini, by odpowiednio wszystko ustawiæ
	IniFile ini;
	ini.Open( "config.ini" );
	int WindowWidth = ini.ReadInt( "GRAPHIC", "WIDTH", 640 );
	int WindowHeight = ini.ReadInt( "GRAPHIC", "HEIGHT", 480 );
	bool fullscreen = ini.ReadBool( "GRAPHIC", "FULLSCREEN", false );
	ini.Close();

	float aspectRatio = (float)WindowWidth / (float)WindowHeight;

	if( !GLWindow.Create( "Expert 3D Tournament", 0, 0, WindowWidth, WindowHeight, fullscreen, (WNDPROC)WndProc, this ) )
	{
		Log.FatalError( "Nie uda³o siê stworzyæ okna!" );
		return 0;
	}

	// Stwórz okno
	if ( !GLRender.CreateGLContext( GLWindow.GetHandle(), 32, 24, 8 ) )
	{
		Log.FatalError( "Nie uda³o siê stworzyæ kontekstu OpenGL!" );
		return 0;									// WyjdŸ je¿eli nie zosta³o stworzone
	}

	GLWindow.SetVisible( true );

	CTexManager texManager( "Data/Textures/" );
	CModelManager modelManager( "Data/Models/", texManager );
	CLevel level( texManager, modelManager );
	GUI = new CGUIMain( texManager, ScriptParser, aspectRatio, WindowHeight );

	pGLevel = &level;
	CLocalPlayerController* pController = new CLocalPlayerController( level.GetPlayer() );
	ControllerList.AddController( pController );

	EventManager.AddObserver( pController );
	EventManager.AddObserver( GUI );

	Log.Log( "Inicjalizacja OpenGL" );

	InitGraphics( texManager );

	GUI->ShowMenu( "MainMenu" );
	GUI->SetMode( GUI_MODE::MENU );

	MainLoop();
	return 0;
}

const bool	CApplication::ProcessMsg( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)									// SprawdŸ komunikaty
	{
	case WM_ACTIVATE:							// komunikar Aktywnoœci
		if (!HIWORD(wParam))					// SprawdŸ altywnoœæ
			active=true;						// Program jest aktywny
		else
			active=false;						// Program ju¿ nie jest aktywny
		return true;								// Spowrotem do pêtli

	case WM_SYSCOMMAND:							// SprawdŸ komunikaty systemowe
		switch (wParam)	
		{
		case SC_SCREENSAVE:					// Wygaszaczpróbuje siê w³¹czyæ?
		case SC_MONITORPOWER:				// Monitor próbuje w³¹czyæ oszczêdzanie pr¹du?
			return true;							// Zapobiegaj
		}
		return false;									// wyjdŸ

	case WM_CLOSE:								// Komunikat zamkniêcia
		PostQuitMessage(0);						// Wysy³amy zamkniêcie
		return true;								

	case WM_LBUTTONDOWN:
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYDOWN, VK_LBUTTON );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );
			return true;
		}

	case WM_LBUTTONUP:
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYUP, VK_LBUTTON );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );
			return true;
		}

	case WM_RBUTTONDOWN:
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYDOWN, VK_RBUTTON );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );
			return true;
		}

	case WM_RBUTTONUP:
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYUP, VK_RBUTTON );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );
			return true;
		}

	case WM_KEYDOWN:							// Klawisz jest naciœnieniêty?
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYDOWN, (unsigned)wParam );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );			
			return true;								
		}

	case WM_KEYUP:								// Klawisz jest puszczony?
		{
			CEventKey KeyEvent( EVENT_INPUT_TYPE::KEYUP, (unsigned)wParam );
			EventManager.AddEvent( *((CEvent*)&KeyEvent) );			
			return true;								
		}

	case WM_SIZE:								// Zmiana rozmiarów
		//GLRender.Resize(LOWORD(lParam),HIWORD(lParam));  // LoWord=szerokoœæ, HiWord=wysokoœæ
		return true;								

	case WM_MOUSEMOVE:
		{
			if( MouseMode == MOUSE_MODE::MENU )
			{
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);

				CEventMouse MouseEvent( EVENT_INPUT_TYPE::MOUSEMOVEABS, x, y );

				EventManager.AddEvent( *((CEvent*)&MouseEvent) );
			}

			return true;
		}

	case WM_CHAR:
		{
			CEventChar CharEvent( EVENT_INPUT_TYPE::CHARPRESS, (char)wParam, (wchar_t)wParam );
			EventManager.AddEvent( *((CEvent*)&CharEvent) );
			return true;
		}
	}


	return false;
}

void	CApplication::UpdateMouse()
{
	if( MouseMode != MOUSE_MODE::GAME )
		return;

	unsigned halfScreenX = GLWindow.GetWidth() / 2;
	unsigned halfScreenY = GLWindow.GetHeight() / 2;

	POINT mousePos;
	if( !GetCursorPos( &mousePos ) )
		return;

	int diffX = mousePos.x - (int)halfScreenX;
	int diffY = mousePos.y - (int)halfScreenY;

	if( diffX == 0 && diffY == 0 )
		return;

	CEventMouse MouseEvent( EVENT_INPUT_TYPE::MOUSEMOVEDIF, diffX, diffY );
	EventManager.AddEvent( *((CEvent*)&MouseEvent) );

	SetCursorPos( (int)halfScreenX, (int)halfScreenY );
}

void	CApplication::InitGraphics( CTexManager& texManager )
{
	glDepthFunc( GL_LEQUAL );				//Metoda testowania G³êbokoœci (ta jest lepsza)
	glEnable( GL_TEXTURE_2D );
	//GUI->SetLoadLevelFunc( LoadLevel );
	//GUI->SendConMsg( "=====EXPERT 3D TOURNAMENT ENGINE=====", false );
	//GUI->SendConMsg( "STEROWNIK: " + GLRender.GetRndInfo( RENDER_RENDERER ), false );
	//GUI->SendConMsg( "WERSJA: " + GLRender.GetRndInfo( RENDER_VERSION ), false );
	//GUI->SendConMsg( "INICJALIZACJA GLOWNA", false );

	//GUI->SendConMsg( "Ustawianie OpenGL...", false );
	glShadeModel( GL_SMOOTH );			    //Ustawienie £adnego cieniowania
	glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );	//Ustawienie koloru czyszczenia bufora kolorów
	glClearDepth( 1.0f );					//Ustwienie glêbokoœci czyszczenia bufora g³êbokoœci
	glEnable( GL_DEPTH_TEST );				//W³¹czenie test g³êbokoœci

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );//Standardowa funkcja przezroczystoœci 
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

	//GUI->SendConMsg( "Zakonczono", false );

	//GUI->SendConMsg( "Inicjalizacja silnika...", false );
	////SMBlur.Init();
	////MenuModel = modelManager.Get( "menumodel.glm" );
	////MainPlayer.SetArmor( 100.0f );
	////SEManager.MaxSpec = 100;

	//GUI->SendConMsg( "Zakonczono", false );
	//GUI->SendConMsg( "INICJALIZACJA GLOWNA ZAKONCZONA", false );

}

void	CApplication::MainLoop()
{
	const float	TIME_STEP = 0.005f;

	MSG		msg;									// Struktura komunikatów windowsa
	bool	done = false;
	float	frameTime = 0.0f;
	CTimer	timer;

	while(!done)									// Pêtla g³ówna (dopuki done nie jest true)
	{
		frameTime += timer.GetDT();

		for( unsigned i = 0; i < 20 && PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ); i++ )	// Czy otrzymano komunikat?
		{
			if ( msg.message == WM_QUIT )				// Czy to komunikat wyjœcia?
			{
				done = true;							// Je¿eli tak to wychodzimy z pêtli
				break;
			}
			else									// Je¿eli nie to zajmij siê komunikatami
			{
				TranslateMessage( &msg );				// T³umacz komunikat
				DispatchMessage( &msg );				// Wykonaj komunikat
			}
		}


		// Rysujemy scene
		if (active)								// Program jest aktywny?
		{
			GUI->UpdateCounter( timer.GetDT() );
			UpdateMouse();

			Render();						// Rysujemy scene

			EventManager.ProcessEvents();

			for( unsigned i = 0; i < 100 && frameTime > TIME_STEP; i++ )
			{
				Update(TIME_STEP);
				frameTime -= TIME_STEP;
			}

			GLRender.SwapBuffers();				// Prze³anczamy bufory
		}
		else
			WaitMessage();

		timer.Update();
	}
}


void	CApplication::Update( const float fTD )
{
	GUI->Update( fTD );

	if( State != GAME_STATE::LEVEL )
		return;

	if( GUI->IsMenuAnimating() )
		return;
	else 
	{
		GUI->SetMode( GUI_MODE::SCREEN );
		MouseMode = MOUSE_MODE::GAME;
	}

	ControllerList.Update();

	pGLevel->Update( fTD );
}

void	CApplication::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if( State == GAME_STATE::LEVEL && !GUI->IsMenuAnimating() )
	{
		GLRender.SetPerspective( 60.0f, 4, 3, 1.0f, 100.0f );
		glLoadIdentity();

		glRotatef( pGLevel->GetPlayer().GetAngle(), 0.0f, 1.0f, 0.0f );
		glTranslatef( -pGLevel->GetPlayer().Pos.X, 0, pGLevel->GetPlayer().Pos.Z );

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		pGLevel->Render();

		GLRender.SetPerspective( 45.0f, 4, 3, 1.0f, 10.0f );
		glClear( GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();
		pGLevel->GetPlayer().Render();
	}

	GUI->Render();
}

void	CApplication::LoadLevel( const std::string& filename )
{
	if( pGLevel->LoadLevel( filename ) )
	{
		GUI->HideMenu();
		State = GAME_STATE::LEVEL;
	}
}

void	CApplication::Print( const std::string& text )
{
	GUI->PrintConsole( text );
}