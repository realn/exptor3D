/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	gui.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> gio.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CBSDL/Consts.h>
#include <CBGL/COpenGL.h>

#include "event_Event.h"

#include "gui.h"
#include "StrEx.h"

const std::string ConFunc[] = { "Speed", "MotionBlur", "MBKeyFrames", "Reflection",
								"RefLevel", "WireFrame", "Note", "SaveRndInfo",
								"SearchExt", "GetRndInfo", "GetExtList", "LoadLevel",
								"EyeAngle", "TexDLevel", "UpTexDLevel", "DownTexDLevel",
								"Cliping", "EnableEngine", "DisableEngine", "Quit",
								"EnableGUI", "DisableGUI", "EnableFullScreen", "DisableFullScreen",
								"Smoke", "MaxSpecial", "ShowAITarget" };
const int ConFuncCount = 27;

/*	KLASA CGUIMain
	To mia³a byæ tylko ma³a klasa poœrednicz¹ca pomiêdzy
	Graczem a Gr¹, a zrobi³ siê z niej g³ówny system
	steruj¹cy wszystkim. Tutaj dochodz¹ wszystkie metody,
	tutaj zarz¹dza siê wszystkim i tutaj zakodowana jest konsola.
	Wiem, wiem, mo¿na to by³o inaczej zrobiæ :/
*/
CGUIMain::CGUIMain( gfx::TextureRepository& texManager, CScriptParser& scriptParser, const float aspectRatio, const unsigned height ) :
	Mode( GUI_MODE::MENU ),
	TextRender( texManager ),
	ScriptParser( scriptParser ),
	Menu( TextRender, aspectRatio ),
	Console( scriptParser, TextRender, height, aspectRatio ),
	Screen( TextRender ),
	AspectRatio( aspectRatio ),
	ScreenHeight( height ),
	FrameTime( 0.0f ),
	cursorX( 0 ),
	cursorY( 0 )
{
	Log.Log( "Inicjalizacja GUI" );
	Cursor = texManager.Get( "Kursor.tga" );
	CH = texManager.Get( "cel.tga" );

	Menu.Load( "Data/menu.mnu" );
	Screen.Load( "Data/screen.gui" );

	Frame = 0;
	Second = 0;
	Minute = 0;
	Hour = 0;
	ConsoleOn = false;
	ConsoleScroll = 0.0f;
	Quit = false;

	eventMapper.addAction(L"gui_move_up", [&](const event::EventAction&) { eventMoveUp(); });
	eventMapper.addAction(L"gui_move_down", [&](const event::EventAction&) { eventMoveDown(); });
	eventMapper.addAction(L"gui_enter", [&](const event::EventAction&) {eventEnter(); });
}

void	CGUIMain::ShowMenu( const std::string& menuID )
{
	Menu.Push( menuID );
}

void	CGUIMain::HideMenu()
{
	Menu.Pop();
}

const bool	CGUIMain::IsMenuAnimating() const
{
	return Menu.IsMenuAnimating();
}

void	CGUIMain::SetMode( const GUI_MODE mode )
{
	Mode = mode;
}

void	CGUIMain::processEvent( const event::Event& event )
{
	eventMapper.executeEvent(event);
}

void	CGUIMain::ParseMouseMove( const int x, const int y )
{
	if( Mode != GUI_MODE::MENU )
		return;

	cursorX = x;
	cursorY = y;

	unsigned width = (unsigned)( (float)ScreenHeight * AspectRatio );

	glm::vec2 pos( (float)x / (float)width, (float)y / (float)ScreenHeight );

	Menu.EventMouseMove( pos );
}

void CGUIMain::eventMoveUp() {
	if (Mode != GUI_MODE::MENU)
		return;

	Menu.EventMoveUp();
}

void CGUIMain::eventMoveDown() {
	if (Mode != GUI_MODE::MENU)
		return;

	Menu.EventMoveDown();
}

void CGUIMain::eventEnter() {
	if (Mode != GUI_MODE::MENU)
		return;

	std::string script;
	if (Menu.EventEnter(script))
		ScriptParser.Execute(script);
}

void CGUIMain::eventBack() {
}

void CGUIMain::eventPointerX(float value) {
}

void CGUIMain::eventPointerY(float value) {
}

/*	Pierwsza metoda wykonuje w³aœciwy silnik GUI, a druga renderuje GUI.
	W³aœciwego silnika nie da siê wy³¹czyæ
*/
void CGUIMain::Update(const float fTD)
{
	switch (Mode)
	{
	case GUI_MODE::MENU:
		Menu.Update( fTD );
		break;

	case GUI_MODE::SCREEN:
		Screen.Update( fTD );
		if( FScrColor[3] > 0.0f )
			FScrColor[3] -= 0.1f * fTD;
		else FScrColor[3] = 0.0f;
		break;

	default:
		break;
	}
	
	Console.Update( fTD );
}

void CGUIMain::Render()
{
	switch (Mode)
	{
	case GUI_MODE::MENU:
		{
			this->Menu.Render();

			float height = (float)ScreenHeight;
			float width = height * AspectRatio;

			glPushAttrib( GL_ENABLE_BIT );
			glPushMatrix();

			{
				glMatrixMode(GL_PROJECTION);
				auto mat = glm::ortho(0.0f, width, height, 0.0f);
				glLoadMatrixf(glm::value_ptr(mat));
				glMatrixMode(GL_MODELVIEW);
			}

			glEnable( GL_TEXTURE_2D );
			glEnable( GL_BLEND );
			glDisable( GL_CULL_FACE );
			glDisable( GL_DEPTH_TEST );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			Cursor->Activate();

			glBegin( GL_TRIANGLE_STRIP );
				glNormal3f( 0.0f, 0.0f, 1.0f );

				glTexCoord2f( 0.0f, 0.0f );
				glVertex3f( (float)cursorX, (float)cursorY + 50.0f, -0.1f );

				glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( (float)cursorX, (float)cursorY, -0.1f );

				glTexCoord2f( 1.0f, 0.0f );
				glVertex3f( (float)cursorX + 50.0f, (float)cursorY + 50.0f, -0.1f );

				glTexCoord2f( 1.0f, 1.0f );
				glVertex3f( (float)cursorX + 50.0f, (float)cursorY, -0.1f );
			glEnd();

			glPopMatrix();
			glPopAttrib();
		}
		break;

	case GUI_MODE::SCREEN:
		{
			//// zaczynamy pisaæ
			//TextRender.StartPrint(800.0f, 600.0f);

			//// Informacje po górnej lewej stronie ekranu
			//TextRender.SetColor( 1.0f, 1.0f, 1.0f );
			//TextRender.Print( 5.0f, 5.0f, "FPS: " + IntToStr( GetFrameRate() ) + ", TD: " + IntToStr( GetMiliSecPas() ) );
			//TextRender.Print( 5.0f, 25.0f, "CZAS: " + IntToStr( Hour ) + ":" + IntToStr( Minute ) + ":" + IntToStr( Second ) );
			//TextRender.Print( 400.0f - (float)(LevName.length()*14 / 2), 5.0f, LevName );

			//TextRender.SetColor( 0.8f, 1.0f, 0.2f );
			//TextRender.Print( 600.0f, 5.0f, "PUNKTY: " + IntToStr( PInfo.FRAGS ), 1.3f, 1.3f );

			//TextRender.SetColor( 1.0f, 0.3f, 0.3f );
			//TextRender.Print( 5.0f, 570.0f, "Z: " + IntToStr( (int)PInfo.HEALTH ), 1.5f, 1.5f );

			//TextRender.SetColor( 0.0f, 1.0f, 1.0f );
			//TextRender.Print( 150.0f, 570.0f, "P: " + IntToStr( (int)PInfo.ARMOR ), 1.5f, 1.5f );

			//TextRender.SetColor( 0.2f, 1.0f, 0.2f );
			//TextRender.Print( 530.0f, 540.0f, "BRON: " + PInfo.WeapName, 1.3f, 1.3f );
			//if( PInfo.AMMO != -1 )
			//	TextRender.Print( 500.0f, 570.0f, "AMUNICJA: " + IntToStr( PInfo.AMMO ), 1.3f, 1.3f );
			//else
			//	TextRender.Print( 500.0f, 570.0f, "AMUNICJA: INF", 1.3f, 1.3f );

			//if( PInfo.CLIPS != -1 )
			//	TextRender.Print( 710.0f, 570.0f, "/" + IntToStr( PInfo.CLIPS ), 1.3f, 1.3f );

			//TextRender.EndPrint();

			float height = (float)ScreenHeight;
			float width = height * AspectRatio;

			Screen.Render( glm::vec2( width, height ) );

			float h = 32.0f;
			float w = h * AspectRatio;

			float x = w / 2.0f;
			float y = h / 2.0f;

			glPushAttrib( GL_ENABLE_BIT );
			glPushMatrix();


			{
				glMatrixMode(GL_PROJECTION);
				auto mat = glm::ortho(0.0f, w, 0.0f, h);
				glLoadMatrixf(glm::value_ptr(mat));
				glMatrixMode(GL_MODELVIEW);
			}

			glLoadIdentity();

			CH->Activate();
			
			glEnable( GL_BLEND );
			glEnable( GL_TEXTURE_2D );
			glDisable( GL_DEPTH_TEST );
			glDisable( GL_CULL_FACE );
			glBlendFunc( GL_ONE, GL_ONE );

			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			glBegin( GL_TRIANGLE_STRIP );
				glTexCoord2f( 1.0f, 0.0f );
				glVertex3f( x + 1.0f, y - 1.0f, 0.0f );

				glTexCoord2f( 0.0f, 0.0f );
				glVertex3f( x - 1.0f, y - 1.0f, 0.0f );

				glTexCoord2f( 1.0f, 1.0f );
				glVertex3f( x + 1.0f, y + 1.0f, 0.0f );

				glTexCoord2f( 0.0f, 1.0f );
				glVertex3f( x - 1.0f, y + 1.0f, 0.0f );
			glEnd();

			glPopMatrix();
			glPopAttrib();
		}
		break;

	default:
		break;
	}

	Console.Render();
}


bool CGUIMain::GetQuit()
{
	return Quit;
}

// Wylicza czas, klatki na sekunde, minuty, godziny, milisekundy, itp.
void CGUIMain::UpdateCounter( const float fTD )
{
	FrameTime += fTD;
	CurrentTD = fTD;
	Frame++;

	if( FrameTime > 1.0f )
	{
		unsigned sec = (unsigned)FrameTime;
		FrameTime -= (float)sec;

		Second += sec;
		if( Second >= 60 )
		{
			Minute += Second / 60;
			Second -= 60;
			if( Minute >= 60 )
			{
				Hour += Minute / 60;
				Minute -= 60;
			}
		}
	}
}

// Dalsze trzy metody zwracaj¹ czas poziomu, klatki na sekunde i milisekundy
const unsigned CGUIMain::GetTime( const GUI_TIME type ) const
{
	switch( type )
	{
	case GUI_TIME::SECONDS :
		return Second;
	case GUI_TIME::MINUTES :
		return Minute;
	case GUI_TIME::HOUR :
		return Hour;

	case GUI_TIME::MILISECONDS:
	default:
		return GetMiliSecPas();
	}
}

const unsigned  CGUIMain::GetFrameRate() const
{
	if( CurrentTD != 0.0f )
		return (unsigned)( 1.0f / CurrentTD );
	return 1;
}

const unsigned CGUIMain::GetMiliSecPas() const
{
	return (unsigned)( CurrentTD * 1000.0f );
}

// Wys³anie wiadomoœci
void CGUIMain::SendMsg( std::string msg, unsigned int Time, float X, float Y, float SclX, float SclY, float R, float G, float B ) 
{
	guiScrMsg Msg;
	Msg.msg = msg;
	Msg.Time = Time;
	Msg.x = X;
	Msg.y = Y;
	Msg.C[0] = R;
	Msg.C[1] = G;
	Msg.C[2] = B;
	Msg.S[0] = SclX;
	Msg.S[1] = SclY;
	SendMsg( Msg );
}

void CGUIMain::SendMsg( guiScrMsg msg )
{
	//msg.ThisTime = ThisTick;
	ScrMsg.push_back( msg );
}

// DO TESTÓW: metoda bezpoœrednio rysuje na ekranie tekst: NIE PRZEZNACZONA DO U¯YWANIA POZA DEBUG'iem
void CGUIMain::Print( float x, float y, std::string text, float ScaleX, float ScaleY )
{
	TextRender.StartPrint(800.0f, 600.0f);
	TextRender.SetColor( 1.0f, 1.0f, 1.0f );
	TextRender.Print( x, y, text, ScaleX, ScaleY );
	TextRender.EndPrint();
}

void	CGUIMain::PrintConsole( const std::string& text )
{
	Console.Print( text );
}

// Tutaj aktywujemy "Full Screen Color", czyli np. Ca³y czerwony ekran od dostania kulk¹, zielony po podniesieniu apteczyki, itp.
// Naraz mo¿e byæ tylko jeden kolor ekranu
void CGUIMain::ActiveFScrColor( float R, float G, float B, float Alpha )
{
	FScrColor[0] = R;
	FScrColor[1] = G;
	FScrColor[2] = B;
	FScrColor[3] = Alpha;
}