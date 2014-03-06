/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	main.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Znajduj� tu si� g��wne funkcje kt�re program inicjalizuj�
		, wprawiaj� w ruch i ko�cz�. Wu te� znajduje si� funkcja WinMain.
		Po wi�cej info na temat tworzenia okna zapraszam do Render.cpp/Render.h.
		Potrzebne biblioteki, kt�re trzeba doda� w nowym projekcie: OpenGL32.lib i GLU32.lib

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "main.h"

#include "ModelManager.h"
#include "GamePlayer.h"
#include "WeaponBulletManager.h"
#include "ItemManager.h"

bool CanDoWLScr = true;

bool Init( CTexManager& texManager, GLModelManager& modelManager )    //Inicjalizacja OpenGL
{
	glDepthFunc( GL_LEQUAL );				//Metoda testowania G��boko�ci (ta jest lepsza)
	glEnable( GL_TEXTURE_2D );
	GUI.InitGUI( &texManager );
	GUI.SetLoadLevelFunc( LoadLevel );
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

	double p1[] = { 0.0, 1.0, 0.0, -Vector3f( 0.0f, -5.01f, 0.0f ).Dot( Vector3f( 0.0f, 1.0f, 0.0f ) ) };
	double p2[] = { 0.0, -1.0, 0.0, -Vector3f( 0.0f, -5.01f, 0.0f ).Dot( Vector3f( 0.0f, -1.0f, 0.0f ) ) };
	glClipPlane( GL_CLIP_PLANE0, p1 );
	glClipPlane( GL_CLIP_PLANE1, p2 );

	glEnable( GL_CLIP_PLANE0 );
	glEnable( GL_COLOR_MATERIAL );
	glDisable( GL_CULL_FACE );
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
	SMBlur.Init();
	MenuModel = modelManager.Get( "menumodel.glm" );
	MainPlayer.SetArmor( 100.0f );
	GUI.Menu.LoadMenu( "Data/menu.mnu" );
	SEManager.MaxSpec = 100;
	srand( GetTickCount() );

	GUI.SendConMsg( "Zakonczono", false );
	GUI.SendConMsg( "INICJALIZACJA GLOWNA ZAKONCZONA", false );
	GUI.Menu.Enable();

	return true; //zwracamy, �e wszystko OK
}

float	menuModelRot = 0.0f;

void Update(const float fTD)	// Logika gry
{
	GUI.ParseKeys( Keys );
	GUI.DoGUIEngine(fTD);

	if( GUI.Menu.IsEnabled() && !pGLevel->GetLoaded() )
	{
		menuModelRot += fTD;
	}

	if( !GUI.CanDoMainEng() )
		return;

	if( GUI.IsShowingWLScr() )
		return;

	Mouse();

	pGLevel->CheckWLFlags();
	MainPlayer.Update( Keys, fTD );
	ThingManager.Update( fTD );

	if( GUI.GetCliping() )
	{
		MainPlayer.ModHealth( -BManager.DoTest( &MainPlayer, MainPlayer.GetArmor() ) );
		WManager.Update( &MainPlayer, 1, fTD );
		BonusMan.Update( &MainPlayer, fTD );
	}
	MainPlayer.ApplyNextPos();

	SEManager.Update( fTD );
	SMBlur.Update( fTD );
	BManager.Update( fTD );

	GUI.PInfo.HEALTH = MainPlayer.GetHealth();
	GUI.PInfo.ARMOR = MainPlayer.GetArmor();
	GUI.PInfo.angle = MainPlayer.GetAng();
}

void RenderLevel()	// Wizualizacja gry
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Czyszczenie bufor�w
	/*
	if( GUI.GetMotionBlur() )
	{
		GLRender.Resize( 512, 512 );
		GLRender.SetPerspective( GUI.GetEyeAngle(), 4, 3, 1.0f, 100.0f );
		glLoadIdentity();	//Reset uk�adu wsp�rz�dnych

		glRotatef( MainPlayer.GetAng(), 0.0f, 1.0f, 0.0f );
		glTranslatef( -MainPlayer.Pos.X, 0, -MainPlayer.Pos.Z );

		pGLevel->DrawLevel();

		ThingManager.Render();
		
		glColor4f( 1.0f, 1.0f, 1.0f ,1.0f );
		WManager.Render();
		BManager.Render();
		BonusMan.Render();
		pGLevel->Render();

		glDepthMask( 0 );
		SEManager.Render();
		pGLevel->DrawReflect();
		glDepthMask( 1 );

		GLRender.SetPerspective( 45.0f, 4, 3, 1.0f, 10.0f );
		glClear( GL_DEPTH_BUFFER_BIT );	//Czyszczenie bufor�w
		glLoadIdentity();
		MainPlayer.Render();
		
		SMBlur.CopyImage();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Czyszczenie bufor�w
		GLRender.Resize( GLRender.GetWidth(), GLRender.GetHeight() );
	}
	*/

	GLRender.SetPerspective( GUI.GetEyeAngle(), 4, 3, 1.0f, 100.0f );
	glLoadIdentity();	//Reset uk�adu wsp�rz�dnych

	glRotatef( MainPlayer.GetAng(), 0.0f, 1.0f, 0.0f );
	glTranslatef( -MainPlayer.Pos.X, 0, -MainPlayer.Pos.Z );

	pGLevel->DrawLevel();
	//ThingManager.Render();

	glColor4f( 1.0f, 1.0f, 1.0f ,1.0f );
	//WManager.Render();
	//BManager.Render();
	//BonusMan.Render();
	//pGLevel->Render();

	//glDepthMask( 0 );
	//SEManager.Render();
	//pGLevel->DrawReflect();
	//glDepthMask( 1 );

	//GLRender.SetPerspective( 45.0f, 4, 3, 1.0f, 10.0f );
	//glClear( GL_DEPTH_BUFFER_BIT );	//Czyszczenie bufor�w
	//glLoadIdentity();
	//MainPlayer.Render();

	//if( GUI.GetMotionBlur() )
	//	SMBlur.Render();
}

bool Render()		//G��wna funkcja renderuj�ca
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Czyszczenie bufor�w

	if( GUI.CanDoMainDraw() )
		RenderLevel();

	//if( GUI.Menu.IsEnabled() && !pGLevel->GetLoaded() )
	//{
	//	GLRender.SetPerspective( GUI.GetEyeAngle(), 4, 3, 1.0f, 100.0f );
	//	glLoadIdentity();	//Reset uk�adu wsp�rz�dnych

	//	glTranslatef( 0.0f, 0.0f, -10.0f );
	//	glRotatef( menuModelRot, 0.5f, 0.5f, 1.0f );
	//	MenuModel->CallObject( 0 );
	//}
	GUI.DoGUIDraw();

	return true;//zwracamy, �e wszystko OK
}

/*	PORUSZANIE MYSZ�
	By wychwyci� ruch myszy pos�u�ymy si� WinApi.
	Zrezygnowa�em z Input's�w, bo tylko namiesza�y
	by w kodzie :P.
*/
void Mouse()
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

	/*	Modyfikujemy stopie� obrotu gracza.	*/
	MainPlayer.ModAngle( mov_x );

	// W ko�cu ustawiamy kursor na �rodek ekranu.
	SetCursorPos( m_wid, m_hei );
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

			return 0;								// Spowrotem do p�tli
		}

		case WM_SYSCOMMAND:							// Sprawd� komunikaty systemowe
		{
			switch (wParam)	
			{
				case SC_SCREENSAVE:					// Wygaszaczpr�buje si� w��czy�?
				case SC_MONITORPOWER:				// Monitor pr�buje w��czy� oszcz�dzanie pr�du?
				return 0;							// Zapobiegaj
			}
			break;									// wyjd�
		}

		case WM_CLOSE:								// Komunikat zamkni�cia
		{
			PostQuitMessage(0);						// Wysy�amy zamkni�cie
			return 0;								
		}

		case WM_LBUTTONDOWN:
		{
			Keys[VK_LBUTTON] = true;
			return 0;
		}

		case WM_LBUTTONUP:
		{
			Keys[VK_LBUTTON] = false;
			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			Keys[VK_RBUTTON] = true;
			return 0;
		}

		case WM_RBUTTONUP:
		{
			Keys[VK_RBUTTON] = false;
			return 0;
		}

		case WM_KEYDOWN:							// Klawisz jest naci�nieni�ty?
		{
			GUI.ParseKey( (char)wParam );
			Keys[wParam] = true;					// Zaznacz �e jest wci�ni�ty
			return 0;								
		}

		case WM_KEYUP:								// Klawisz jest puszczony?
		{
			Keys[wParam] = false;					// Zaznacz �e jest wolny
			return 0;								
		}

		case WM_SIZE:								// Zmiana rozmiar�w
		{
			GLRender.Resize(LOWORD(lParam),HIWORD(lParam));  // LoWord=szeroko��, HiWord=wysoko��
			return 0;								
		}
	}

	// przeka� reszte komunikat�w do DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancja
					HINSTANCE	hPrevInstance,		// Poprzednia instancja
					LPSTR		lpCmdLine,			// Parametry
					int			nCmdShow)			// Tryb widoczno�ci okna
{
	MSG		msg;									// Struktura komunikat�w windowsa
	bool	done = false;	// bool do wyj�cia z p�tli

	Log.Init( "main.log", " - Expert 3D Tournament Log" );

	// Czytamy plik ini, by odpowiednio wszystko ustawi�
	IniFile ini;
	ini.Open( "config.ini" );
	int Szer = ini.ReadInt( "GRAPHIC", "WIDTH", 640 );
	int Wys = ini.ReadInt( "GRAPHIC", "HEIGHT", 480 );
	fullscreen = ini.ReadBool( "GRAPHIC", "FULLSCREEN", false );
	GUI.SetMaxSpecial( ini.ReadInt( "SPECIAL", "MAXCOUNT", 0 ) );
	GUI.SetCanSmoke( ini.ReadBool( "SPECIAL", "SMOKE", true ) );
	ini.Close();

	// Stw�rz okno
	if ( !GLRender.GLCreateWindow( "Expert 3D Tournament", Szer, Wys, fullscreen, (WNDPROC)WndProc ) )
	{
		return 0;									// Wyjd� je�eli nie zosta�o stworzone
	}

	CTexManager texManager( "Data/Textures/" );
	GLModelManager modelManager( "Data/Models/", texManager );
	CLevel level( texManager, modelManager );

	pGLevel = &level;

	SEManager.Init( texManager );
	MainPlayer.Init( modelManager );

	Log.Log( "Inicjalizacja OpenGL" );
	if( !Init( texManager, modelManager ) )
	{
		Log.Error( "B��d inicjalizacji" );
		return 0;
	}

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

	// Deinicjalizacja
	Log.Log( "Koniec pracy Aplikacji" );
	GLRender.GLDestroyWindow();// Zniszcz okno
	return (msg.wParam);							// Wyjd� z programu
}

// Oddzielna funkcja �aduj�ca poziom - nie da�o si� inaczej zrobi� :/
void LoadLevel( std::string filename )
{
	if( pGLevel->LoadLevel( filename ) )
	{
		pGLevel->InitLevel();
		
		GUI.LevName = pGLevel->GetLevelName();
		WManager.LoadFromLevel();
		GUI.EnableMainEngine();
		GUI.EnableGGUI();

		CanDoWLScr = true;
	}
	else
	{
		GUI.SendConMsg( "Nie mozna znalesc pliku", false );
	}
}
