#include "GUIConsole.h"
#include "Render.h"

CGUIConsole::CGUIConsole( CScriptParser& scriptParser, CTextRenderer& textRender, const unsigned height, const float aspectRatio ) :
	ScriptParser( scriptParser ),
	TextRender( textRender ),
	ScreenHeight( height ),
	AspectRatio( aspectRatio ),
	Visible( false ),
	Scroll( 0.0f )
{

}

void	CGUIConsole::Render()
{
	if( !Visible && Scroll <= 0.0f )
		return;

	glPushAttrib( GL_ENABLE_BIT );
	glPushMatrix();

	CRender::SetOrtho( 0.0f, 1.0f, -1.0f, 1.0f );
	glLoadIdentity();

	glEnable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 0.0f, 0.0f, 0.0f, 0.8f );

	glBegin( GL_TRIANGLE_STRIP );
	glVertex2f( 0.0f, 1.0f - Scroll );
	glVertex2f( 1.0f, 1.0f - Scroll );
	glVertex2f( 0.0f, 2.0f - Scroll );
	glVertex2f( 1.0f, 2.0f - Scroll );
	glEnd();

	glPopMatrix();
	glPopAttrib();
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

}

void	CGUIConsole::ParseChar( const char Characted )
{

}

void	CGUIConsole::Print( const std::string& msg )
{

}