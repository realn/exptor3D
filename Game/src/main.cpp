/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	glm.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Znajduj¹ tu siê g³ówne funkcje które program inicjalizuj¹
		, wprawiaj¹ w ruch i koñcz¹. Wu te¿ znajduje siê funkcja WinMain.
		Po wiêcej info na temat tworzenia okna zapraszam do Render.cpp/Render.h.
		Potrzebne biblioteki, które trzeba dodaæ w nowym projekcie: OpenGL32.lib i GLU32.lib

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "main.h"
bool CanDoWLScr = true;

bool Init()    //Inicjalizacja OpenGL
{
	glDepthFunc( GL_LEQUAL );				//Metoda testowania G³êbokoœci (ta jest lepsza)
	glEnable( GL_TEXTURE_2D );
	GUI.InitGUI();
	GUI.SetLoadLevelFunc( LoadLevel );
	GUI.SendConMsg( "=====EXPERT 3D TOURNAMENT ENGINE=====", false );
	GUI.SendConMsg( "STEROWNIK: " + GLRender.GetRndInfo( RENDER_RENDERER ), false );
	GUI.SendConMsg( "WERSJA: " + GLRender.GetRndInfo( RENDER_VERSION ), false );
	GUI.SendConMsg( "INICJALIZACJA GLOWNA", false );

	GUI.SendConMsg( "Ustawianie OpenGL...", false );
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

	double p1[] = { 0.0, 1.0, 0.0, -Vector3f( 0.0f, -5.01f, 0.0f ).Dot( Vector3f( 0.0f, 1.0f, 0.0f ) ) };
	double p2[] = { 0.0, -1.0, 0.0, -Vector3f( 0.0f, -5.01f, 0.0f ).Dot( Vector3f( 0.0f, -1.0f, 0.0f ) ) };
	glClipPlane( GL_CLIP_PLANE0, p1 );
	glClipPlane( GL_CLIP_PLANE1, p2 );

	glEnable( GL_CLIP_PLANE0 );
	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_CULL_FACE );

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
	Part.LoadTGA( "Data/Part.tga" );
	MenuModel = GLMManager.Get( "Data/menumodel.glm" );
	MainPlayer.SetArmor( 100.0f );
	GUI.Menu.LoadMenu( "Data/menu.mnu" );
	SEManager.MaxSpec = 100;
	srand( GetTickCount() );

	GUI.SendConMsg( "Zakonczono", false );
	GUI.SendConMsg( "INICJALIZACJA GLOWNA ZAKONCZONA", false );
	GUI.Menu.Enable();

	return true; //zwracamy, ¿e wszystko OK
}

void DoEngine()	// Logika gry
{
	if( GUI.IsShowingWLScr() )
		return;

	Mouse();
	GLevel.CheckWLFlags();
	MainPlayer.DoEngine( Keys );
	ThingManager.DoEngine();
	if( GUI.GetCliping() )
	{
		MainPlayer.ModHealth( -BManager.DoTest( &MainPlayer, MainPlayer.GetArmor() ) );
		WManager.DoEngine( &MainPlayer, 1 );
		BonusMan.DoEngine( &MainPlayer );
	}
	MainPlayer.ApplyNextPos();
	SEManager.DoEngine();
	SMBlur.DoEngine();
	BManager.DoEngine();

	GUI.PInfo.HEALTH = MainPlayer.GetHealth();
	GUI.PInfo.ARMOR = MainPlayer.GetArmor();
	GUI.PInfo.angle = MainPlayer.GetAng();
}

void DoDraw()	// Wizualizacja gry
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Czyszczenie buforów
	if( GUI.GetMotionBlur() )
	{
		GLRender.Resize( 512, 512 );
		GLRender.SetPerspective( GUI.GetEyeAngle(), 4, 3, 1.0f, 100.0f );
		glLoadIdentity();	//Reset uk³adu wspó³rzêdnych

		glRotatef( MainPlayer.GetAng(), 0.0f, 1.0f, 0.0f );
		glTranslatef( -MainPlayer.Pos.X, 0, -MainPlayer.Pos.Z );

		GLevel.DrawLevel();
		ThingManager.DoDraw();
		glColor4f( 1.0f, 1.0f, 1.0f ,1.0f );
		WManager.DoDraw();
		BManager.DoDraw();
		BonusMan.DoDraw();
		glDepthMask( 0 );
		SEManager.DoDraw();
		GLevel.DrawReflect();
		glDepthMask( 1 );

		GLRender.SetPerspective( 45.0f, 4, 3, 1.0f, 10.0f );
		glClear( GL_DEPTH_BUFFER_BIT );	//Czyszczenie buforów
		glLoadIdentity();
		MainPlayer.DoDraw();
		
		glFlush();

		SMBlur.CopyImage();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Czyszczenie buforów
		GLRender.Resize( GLRender.GetWidth(), GLRender.GetHeight() );
	}

	GLRender.SetPerspective( GUI.GetEyeAngle(), 4, 3, 1.0f, 100.0f );
	glLoadIdentity();	//Reset uk³adu wspó³rzêdnych

	glRotatef( MainPlayer.GetAng(), 0.0f, 1.0f, 0.0f );
	glTranslatef( -MainPlayer.Pos.X, 0, -MainPlayer.Pos.Z );

	GLevel.DrawLevel();
	ThingManager.DoDraw();
	glColor4f( 1.0f, 1.0f, 1.0f ,1.0f );
	WManager.DoDraw();
	BManager.DoDraw();
	BonusMan.DoDraw();
	glDepthMask( 0 );
	SEManager.DoDraw();
	GLevel.DrawReflect();
	glDepthMask( 1 );

	GLRender.SetPerspective( 45.0f, 4, 3, 1.0f, 10.0f );
	glClear( GL_DEPTH_BUFFER_BIT );	//Czyszczenie buforów
	glLoadIdentity();
	MainPlayer.DoDraw();

	if( GUI.GetMotionBlur() )
		SMBlur.DoDraw();

	glFlush();
}

bool Render()		//G³ówna funkcja renderuj¹ca
{
	unsigned int i;
	static float rot = 0.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Czyszczenie buforów
	GUI.ParseKeys( Keys );
	GUI.DoGUIEngine();
	if( GUI.CanDoMainEng() )
		for( i = 0; i < GUI.GetEPFTimes(); i++ )
			DoEngine();

	if( GUI.CanDoMainDraw() )
		DoDraw();
	if( GUI.Menu.IsEnabled() && !GLevel.GetLoaded() )
	{
		GLRender.SetPerspective( GUI.GetEyeAngle(), 4, 3, 1.0f, 100.0f );
		glLoadIdentity();	//Reset uk³adu wspó³rzêdnych

		for( i = 0; i < GUI.GetEPFTimes(); i++ )
			rot += 0.01f;

		glTranslatef( 0.0f, 0.0f, -10.0f );
		MenuModel->CallObject( 1 );
		glRotatef( rot, 0.5f, 0.5f, 1.0f );
		MenuModel->CallObject( 0 );
	}
	GUI.DoGUIDraw();

	return true;//zwracamy, ¿e wszystko OK
}

/*	PORUSZANIE MYSZ¥
	By wychwyciæ ruch myszy pos³u¿ymy siê WinApi.
	Zrezygnowa³em z Input'sów, bo tylko namiesza³y
	by w kodzie :P.
*/
void Mouse()
{
	/*	Najpierw standardowa strukturka windows.
		Jest to eee... PUNKT :) który zawiera A¯
		2 sk³adowe ( x, y ) :P
	*/
	POINT mpos;

	/*	Teraz musimy mieæ punk oparcia o ile ruszono
		mysz¹. Ja wybra³em œrodek ekranu, z t¹d
		korzystam z klasy UIRender by uzyskaæ szerokoœæ
		i wysokoœæ oraz dziele je na pó³.
	*/
	int m_wid = GLRender.GetWidth() / 2;
	int m_hei = GLRender.GetHeight() / 2;

	// Dwie zmienne pomocnicze, trzymaj¹ce stopieñ wychylenia
	float mov_x;// mov_y,

	/*	U¿ywamy piêknej funkcji Windows, by wychwyciæ po³orzenie
		kursora wzglêdem okna.
	*/
	GetCursorPos( &mpos );

	/*	Wyliczamy róznice miêdzy po³orzeniem 
		punktu na ekranie, a œrodkiem ekranu
		i zmniejszamy j¹ dziesiêciokrotnie.
		( by³aby za du¿a ).
	*/
	mov_x = (float)(mpos.x - m_wid) / 10.0f;

	/*	Modyfikujemy stopieñ obrotu gracza.	*/
	MainPlayer.ModAngle( mov_x );

	// W koñcu ustawiamy kursor na œrodek ekranu.
	SetCursorPos( m_wid, m_hei );
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
	switch (uMsg)									// SprawdŸ komunikaty
	{
		case WM_ACTIVATE:							// komunikar Aktywnoœci
		{
			if (!HIWORD(wParam))					// SprawdŸ altywnoœæ
			{
				active=true;						// Program jest aktywny
			}
			else
			{
				active=false;						// Program ju¿ nie jest aktywny
			}

			return 0;								// Spowrotem do pêtli
		}

		case WM_SYSCOMMAND:							// SprawdŸ komunikaty systemowe
		{
			switch (wParam)	
			{
				case SC_SCREENSAVE:					// Wygaszaczpróbuje siê w³¹czyæ?
				case SC_MONITORPOWER:				// Monitor próbuje w³¹czyæ oszczêdzanie pr¹du?
				return 0;							// Zapobiegaj
			}
			break;									// wyjdŸ
		}

		case WM_CLOSE:								// Komunikat zamkniêcia
		{
			PostQuitMessage(0);						// Wysy³amy zamkniêcie
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

		case WM_KEYDOWN:							// Klawisz jest naciœnieniêty?
		{
			GUI.ParseKey( (char)wParam );
			Keys[wParam] = true;					// Zaznacz ¿e jest wciœniêty
			return 0;								
		}

		case WM_KEYUP:								// Klawisz jest puszczony?
		{
			Keys[wParam] = false;					// Zaznacz ¿e jest wolny
			return 0;								
		}

		case WM_SIZE:								// Zmiana rozmiarów
		{
			GLRender.Resize(LOWORD(lParam),HIWORD(lParam));  // LoWord=szerokoœæ, HiWord=wysokoœæ
			return 0;								
		}
	}

	// przeka¿ reszte komunikatów do DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancja
					HINSTANCE	hPrevInstance,		// Poprzednia instancja
					LPSTR		lpCmdLine,			// Parametry
					int			nCmdShow)			// Tryb widocznoœci okna
{
	MSG		msg;									// Struktura komunikatów windowsa
	bool	done = false;	// bool do wyjœcia z pêtli

	Log.Init( "main.log", " - Expert 3D Tournament Log" );

	// Czytamy plik ini, by odpowiednio wszystko ustawiæ
	MyIniFile ini;
	ini.Open( "config.ini" );
	int Szer = ini.ReadInt( "GRAPHIC", "WIDTH", 640 );
	int Wys = ini.ReadInt( "GRAPHIC", "HEIGHT", 480 );
	fullscreen = ini.ReadBool( "GRAPHIC", "FULLSCREEN", false );
	GUI.SetMaxSpecial( ini.ReadInt( "SPECIAL", "MAXCOUNT", 0 ) );
	GUI.SetCanSmoke( ini.ReadBool( "SPECIAL", "SMOKE", true ) );
	ini.Close();

	GLRender.SetWinHandle( &hWnd );

	// Stwórz okno
	if ( !GLRender.GLCreateWindow( "Expert 3D Tournament", Szer, Wys, fullscreen, (WNDPROC)WndProc ) )
	{
		return 0;									// WyjdŸ je¿eli nie zosta³o stworzone
	}

	Log.Log( "Inicjalizacja OpenGL" );
	if( !Init() )
	{
		Log.Error( "B³¹d inicjalizacji" );
		return 0;
	}

	while(!done)									// Pêtla g³ówna (dopuki done nie jest true)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Czy otrzymano komunikat?
		{
			if (msg.message==WM_QUIT)				// Czy to komunikat wyjœcia?
			{
				done = true;							// Je¿eli tak to wychodzimy z pêtli
			}
			else									// Je¿eli nie to zajmij siê komunikatami
			{
				TranslateMessage(&msg);				// T³umacz komunikat
				DispatchMessage(&msg);				// Wykonaj komunikat
			}
		}
		//else										// Je¿eli nie ma komunikatów
		//{
			GUI.UpdateCounter();
			// Rysujemy scene
			if (active)								// Program jest aktywny?
			{
				if ( GUI.GetQuit() )				// Czy by³ wciœniêty ESC?
				{
					done = true;						// Je¿eli tak to wychodzimy z pêtli
				}
				else								// Je¿eli nie to rysujemy
				{
					Render();						// Rysujemy scene
					GLRender.SwitchBuf();				// Prze³anczamy bufory
				}

			}
			else
			{
				//WaitMessage();
			}
		//}
	}

	// Wy³anczanie
	Log.Log( "Koniec pracy Aplikacji" );
	GLRender.GLDestroyWindow();// Zniszcz okno
	if( Timer ) delete Timer;
	return (msg.wParam);							// WyjdŸ z programu
}

// Oddzielna funkcja ³aduj¹ca poziom - nie da³o siê inaczej zrobiæ :/
void LoadLevel( std::string filename )
{
	FILE* fp = 0;
	fopen_s(&fp, filename.c_str(), "r" );
	if( !fp )
	{
		GUI.SendConMsg( "Nie mozna znalesc pliku", false );
		return;
	}
	fclose( fp );

	GLevel.LoadLevel( filename );
	GLevel.InitLevel();
	GUI.LevName = GLevel.GetLevelName();
	WManager.LoadFromLevel();
	GUI.EnableMainEngine();
	GUI.EnableGGUI();
	CanDoWLScr = true;
}
