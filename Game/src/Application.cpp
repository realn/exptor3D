#include "stdafx.h"
#include "Application.h"

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/evtloop.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EventInput.h"
#include "GamePlayerController.h"

//#include <windowsx.h>

#include "GUI.h"
#include "GamePlayer.h"
#include "Level.h"
#include "inifile.h"

wxIMPLEMENT_APP(CApplication);

CApplication::CApplication() :
	active( true ),
	//GLWindow( hInstance, "OPENGLWINDOWCLASS3232" ),
	State(GAME_STATE::MAINMENU),
	MouseMode(MOUSE_MODE::MENU),
	ScriptParser(EventManager),
	GUI( nullptr )
{
	Log.Init( "main.log", " - Expert 3D Tournament Log" );
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

bool	CApplication::OnInit() {
	srand( GetTickCount() );

	// Czytamy plik ini, by odpowiednio wszystko ustawiæ
	IniFile ini;
	ini.Open( "config.ini" );
	int WindowWidth = ini.ReadInt( "GRAPHIC", "WIDTH", 640 );
	int WindowHeight = ini.ReadInt( "GRAPHIC", "HEIGHT", 480 );
	bool fullscreen = ini.ReadBool( "GRAPHIC", "FULLSCREEN", false );
	ini.Close();

	float aspectRatio = (float)WindowWidth / (float)WindowHeight;

	this->m_pRenderWindow = new CRenderWindow(this->EventManager);
	this->m_pRenderWindow->SetClientSize(WindowWidth, WindowHeight);

	CRenderViewConfig config;
	config.ColorBits = 32;
	config.DepthBits = 24;
	config.StencilBits = 8;

	this->m_pRenderView = new CRenderView(this->m_pRenderWindow, config);
	this->m_pRenderView->SetClientSize(WindowWidth, WindowHeight);

	this->m_pRenderWindow->Show();
	this->m_pRenderContext = new CRenderContext(this->m_pRenderView, 1, 1);
	this->m_pRenderContext->SetCurrent(*this->m_pRenderView);
	this->m_pRenderView->Initialize(wxShowEvent());

	this->m_pRenderView->Bind(wxEVT_MOTION, &CRenderWindow::OnMouseMove, this->m_pRenderWindow);
	this->m_pRenderView->Bind(wxEVT_KEY_DOWN, &CRenderWindow::OnKeyDown, this->m_pRenderWindow);
	this->m_pRenderView->Bind(wxEVT_KEY_UP, &CRenderWindow::OnKeyUp, this->m_pRenderWindow);
	this->m_pRenderView->Bind(wxEVT_MOTION, &CRenderWindow::OnMouseMove, this->m_pRenderWindow);
	this->m_pRenderView->Bind(wxEVT_LEFT_UP, &CRenderWindow::OnMouseKeyUp, this->m_pRenderWindow);
	this->m_pRenderView->Bind(wxEVT_RIGHT_UP, &CRenderWindow::OnMouseKeyUp, this->m_pRenderWindow);
	this->m_pRenderView->Bind(wxEVT_LEFT_DOWN, &CRenderWindow::OnMouseKeyDown, this->m_pRenderWindow);

	return true;
}

void	CApplication::UpdateMouse()
{
	if( MouseMode != MOUSE_MODE::GAME )
		return;

	auto clientSize = this->m_pRenderWindow->GetClientSize();
	unsigned halfScreenX = clientSize.GetWidth() / 2;
	unsigned halfScreenY = clientSize.GetHeight() / 2;

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

int	CApplication::MainLoop() {
	auto Size = this->m_pRenderWindow->GetClientSize();
	float aspectRatio = (float)Size.GetWidth() / (float)Size.GetHeight();

	CTexManager texManager( "Data/Textures/" );
	CModelManager modelManager( "Data/Models/", texManager );
	CLevel level( texManager, modelManager );
	GUI = new CGUIMain( texManager, ScriptParser, aspectRatio, Size.GetHeight() );

	pGLevel = &level;
	CLocalPlayerController* pController = new CLocalPlayerController( level.GetPlayer() );
	ControllerList.AddController( pController );

	EventManager.AddObserver( pController );
	EventManager.AddObserver( GUI );

	Log.Log( "Inicjalizacja OpenGL" );

	InitGraphics( texManager );

	GUI->ShowMenu( "MainMenu" );
	GUI->SetMode( GUI_MODE::MENU );

	const float	TIME_STEP = 0.005f;

	float	frameTime = 0.0f;
	CTimer	timer;

	wxEventLoop	winEventLoop;
	wxEventLoopActivator loopActivate(&winEventLoop);

	while(winEventLoop.IsRunning()) {
		frameTime += timer.GetDT();

		while(winEventLoop.Pending())
			winEventLoop.Dispatch();

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

			this->m_pRenderView->SwapBuffers();
		}
		else
			winEventLoop.Yield();

		timer.Update();
	}

	return 0;
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
		glm::mat4 mat = glm::perspectiveFov( 60.0f, 4.0f, 3.0f, 1.0f, 100.0f );
		glMatrixMode( GL_PROJECTION );
		glLoadMatrixf( glm::value_ptr(mat) );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glRotatef( pGLevel->GetPlayer().GetAngle(), 0.0f, 1.0f, 0.0f );
		glTranslatef( -pGLevel->GetPlayer().Pos.X, 0, pGLevel->GetPlayer().Pos.Z );

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		pGLevel->Render();

		mat = glm::perspectiveFov( 45.0f, 4.0f, 3.0f, 1.0f, 10.0f );
		glMatrixMode( GL_PROJECTION );
		glLoadMatrixf( glm::value_ptr(mat) );
		glMatrixMode( GL_MODELVIEW );
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