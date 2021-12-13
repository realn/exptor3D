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
#include <CBGL/State.h>

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
CGUIMain::CGUIMain( gfx::TextureRepository& texManager, std::shared_ptr<logic::ScriptParser> scriptParser, float aspectRatio, unsigned height ) :
	Mode( GUI_MODE::MENU ),
	textPrinter(texManager.Get("Font.tga")),
	ScriptParser( scriptParser ),
	menu(aspectRatio, scriptParser),
	Console( scriptParser, height, aspectRatio ),
	AspectRatio( aspectRatio ),
	ScreenHeight( height ),
	cursorX( 0 ),
	cursorY( 0 )
{
	//Log.Log( "Inicjalizacja GUI" );
	Cursor = texManager.Get( "Kursor.tga" );
	CH = texManager.Get( "cel.tga" );

	menu.load( L"Data/menu.mnu", textPrinter.getFontInfo() );
	screen.load( L"Data/screen.gui", textPrinter.getFontInfo() );

	ConsoleOn = false;
	ConsoleScroll = 0.0f;
	Quit = false;
}

void	CGUIMain::ShowMenu( const cb::string& menuID )
{
	menu.push( menuID );
}

void	CGUIMain::HideMenu()
{
	menu.pop();
}

const bool	CGUIMain::IsMenuAnimating() const
{
	return menu.isMenuAnimating();
}

void	CGUIMain::SetMode( const GUI_MODE mode )
{
	Mode = mode;
}

void	CGUIMain::processEvent( const event::Event& event )
{
	eventMapper.executeEvent(event);
	if (Mode != GUI_MODE::MENU)
		return;
	menu.processEvent(event);
}

/*	Pierwsza metoda wykonuje w³aœciwy silnik GUI, a druga renderuje GUI.
	W³aœciwego silnika nie da siê wy³¹czyæ
*/
void CGUIMain::Update(float timeDelta)
{
	switch (Mode)
	{
	case GUI_MODE::MENU:
		menu.update(timeDelta);
		break;

	case GUI_MODE::SCREEN:
		screen.update( timeDelta );
		if( FScrColor[3] > 0.0f )
			FScrColor[3] -= 0.1f * timeDelta;
		else FScrColor[3] = 0.0f;
		break;

	default:
		break;
	}
	
	Console.Update( timeDelta );
}

void CGUIMain::Render()
{
	auto blendState = cb::gl::getBlendState();
	auto cullState = cb::gl::getCullState();
	auto depthState = cb::gl::getDepthState();

	blendState.enabled = true;
	blendState.setFunc(cb::gl::BlendFactor::SRC_ALPHA, cb::gl::BlendFactor::ONE_MINUS_SRC_ALPHA);

	cullState.enabled = false;
	depthState.enabled = false;

	cb::gl::setState(blendState);
	cb::gl::setState(cullState);
	cb::gl::setState(depthState);

	switch (Mode)
	{
	case GUI_MODE::MENU:
		{
			auto ctx = menu.makeRender(textPrinter);

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(glm::value_ptr(ctx.getProjectionMatrix()));
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			ctx.render();

			//glPushAttrib( GL_ENABLE_BIT );
			//glPushMatrix();


			//glEnable( GL_TEXTURE_2D );
			//glEnable( GL_BLEND );
			//glDisable( GL_CULL_FACE );
			//glDisable( GL_DEPTH_TEST );
			//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			//glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			//Cursor->Activate();

			//glBegin( GL_TRIANGLE_STRIP );
			//	glNormal3f( 0.0f, 0.0f, 1.0f );

			//	glTexCoord2f( 0.0f, 0.0f );
			//	glVertex3f( (float)cursorX, (float)cursorY + 50.0f, -0.1f );

			//	glTexCoord2f( 0.0f, 1.0f );
			//	glVertex3f( (float)cursorX, (float)cursorY, -0.1f );

			//	glTexCoord2f( 1.0f, 0.0f );
			//	glVertex3f( (float)cursorX + 50.0f, (float)cursorY + 50.0f, -0.1f );

			//	glTexCoord2f( 1.0f, 1.0f );
			//	glVertex3f( (float)cursorX + 50.0f, (float)cursorY, -0.1f );
			//glEnd();

			//glPopMatrix();
			//glPopAttrib();
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

			auto ctx = gui::RenderContext();

			float height = (float)ScreenHeight;
			float width = height * AspectRatio;
			ctx.setProjectionMatrix(glm::ortho(0.0f, width, height, 0.0f));

			screen.render( ctx, textPrinter, glm::vec2( width, height ) );

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(glm::value_ptr(ctx.getProjectionMatrix()));
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			ctx.render();

			//float h = 32.0f;
			//float w = h * AspectRatio;

			//float x = w / 2.0f;
			//float y = h / 2.0f;

			//glPushAttrib( GL_ENABLE_BIT );
			//glPushMatrix();


			//{
			//	glMatrixMode(GL_PROJECTION);
			//	auto mat = glm::ortho(0.0f, w, 0.0f, h);
			//	glLoadMatrixf(glm::value_ptr(mat));
			//	glMatrixMode(GL_MODELVIEW);
			//}

			//glLoadIdentity();

			//CH->Activate();
			//
			//glEnable( GL_BLEND );
			//glEnable( GL_TEXTURE_2D );
			//glDisable( GL_DEPTH_TEST );
			//glDisable( GL_CULL_FACE );
			//glBlendFunc( GL_ONE, GL_ONE );

			//glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			//glBegin( GL_TRIANGLE_STRIP );
			//	glTexCoord2f( 1.0f, 0.0f );
			//	glVertex3f( x + 1.0f, y - 1.0f, 0.0f );

			//	glTexCoord2f( 0.0f, 0.0f );
			//	glVertex3f( x - 1.0f, y - 1.0f, 0.0f );

			//	glTexCoord2f( 1.0f, 1.0f );
			//	glVertex3f( x + 1.0f, y + 1.0f, 0.0f );

			//	glTexCoord2f( 0.0f, 1.0f );
			//	glVertex3f( x - 1.0f, y + 1.0f, 0.0f );
			//glEnd();

			//glPopMatrix();
			//glPopAttrib();
		}
		break;

	default:
		break;
	}

	//Console.Render(ctx, textPrinter);

	//glPushAttrib( GL_ENABLE_BIT );
	//glPushMatrix();

	//float height = (float)ScreenHeight;
	//float width = height * AspectRatio;

	//{
	//	glMatrixMode(GL_PROJECTION);
	//	auto mat = glm::ortho(0.0f, width, height, 0.0f);
	//	glLoadMatrixf(glm::value_ptr(mat));
	//	glMatrixMode(GL_MODELVIEW);
	//}
	//glLoadIdentity();

	//glEnable( GL_TEXTURE_2D );
	//glEnable( GL_BLEND );
	////glEnable(GL_COLOR_MATERIAL);
	//glDisable( GL_CULL_FACE );
	//glDisable( GL_DEPTH_TEST );
	//glDisable(GL_LIGHTING);
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	//ctx.render();

	//glPopMatrix();
	//glPopAttrib();
}


bool CGUIMain::GetQuit()
{
	return Quit;
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
	//TextRender.StartPrint(800.0f, 600.0f);
	//TextRender.SetColor( 1.0f, 1.0f, 1.0f );
	//TextRender.Print( x, y, text, ScaleX, ScaleY );
	//TextRender.EndPrint();
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