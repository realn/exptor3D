#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CBGL/COpenGL.h>

#include "GUIConsole.h"

CGUIConsole::CGUIConsole( CScriptParser& scriptParser, const unsigned height, const float aspectRatio ) :
	ScriptParser( scriptParser ),
	ScreenHeight( height ),
	AspectRatio( aspectRatio ),
	Visible( false ),
	Scroll( 0.0f )
{

}

void	CGUIConsole::Render(gui::RenderContext& ctx, gui::TextPrinter& printer)
{
	//if( !Visible && Scroll <= 0.0f )
	//	return;

	//glPushAttrib( GL_ENABLE_BIT );
	//glPushMatrix();

	//{
	//	glMatrixMode(GL_PROJECTION);
	//	auto mat = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f);
	//	glLoadMatrixf(glm::value_ptr(mat));
	//	glMatrixMode(GL_MODELVIEW);
	//}
	//glLoadIdentity();

	//glEnable( GL_BLEND );
	//glDisable( GL_TEXTURE_2D );
	//glDisable( GL_DEPTH_TEST );

	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glColor4f( 0.0f, 0.0f, 0.0f, 0.8f );

	//glBegin( GL_TRIANGLE_STRIP );
	//glVertex2f( 0.0f, -1.0f + Scroll );
	//glVertex2f( 1.0f, -1.0f + Scroll );
	//glVertex2f( 0.0f, -0.5f + Scroll );
	//glVertex2f( 1.0f, -0.5f + Scroll );
	//glEnd();

	//glPopMatrix();
	//glPopAttrib();

	//float height = (float)ScreenHeight;
	//float width = height * AspectRatio;

	//float halfY = height / 2.0f;

	//TextRender.StartPrint( width, height );
	//TextRender.SetColor( 1.0f, 1.0f, 1.0f );

	//glm::vec2 pos( 0.0f, -halfY + Scroll * height - TextRender.GetTextSize( CurrentText ).y );
	//TextRender.Print( pos, CurrentText );
	//pos.y -= TextRender.GetTextSize( CurrentText ).y;

	//for( auto it = TextLog.rbegin(); it != TextLog.rend() && pos.y > -halfY; it++ )
	//{
	//	TextRender.Print( pos, *it );
	//	pos.y -= TextRender.GetTextSize( *it ).y;
	//}

	//TextRender.EndPrint();
}

void	CGUIConsole::Update( const float fTD )
{
	if( Visible )
	{
		if( Scroll < 1.0f )
			Scroll += 2.0f * fTD;
		else
			Scroll = 1.0f;
	}
	else
	{
		if( Scroll > 0.0f )
			Scroll -= 2.0f * fTD;
		else 
			Scroll = 0.0f;
	}
}

void	CGUIConsole::SetVisible( const bool set, const bool animate )
{
	Visible = set;
	if( !animate )
	{
		if( Visible )
			Scroll = 1.0f;
		else
			Scroll = 0.0f;
	}
}

const bool	CGUIConsole::IsVisible() const
{
	return Visible;
}

const bool	CGUIConsole::IsAnimating() const
{
	if( Visible )
		return Scroll < 1.0f;
	else
		return Scroll > 0.0f;
}

void	CGUIConsole::ParseKey( const unsigned key, const bool down )
{
	//if( down )
	//{
	//	switch (key)
	//	{
	//	case VK_BACK:
	//		if( !CurrentText.empty() )
	//			CurrentText = CurrentText.substr( 0, CurrentText.length() - 1 );
	//		break;

	//	case VK_RETURN:
	//		if( !CurrentText.empty() )
	//		{
	//			ScriptParser.Execute( CurrentText, true );
	//			CurrentText.clear();
	//		}
	//		break;

	//	default:
	//		break;
	//	}
	//}
}

void	CGUIConsole::ParseChar( const char Characted )
{
	if( Characted < 32 )
		return;

	CurrentText += Characted;
}

void	CGUIConsole::Print( const std::string& msg )
{
	TextLog.push_back( msg );
}