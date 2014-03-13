#include "GUIScreen.h"

CGUITextElement::CGUITextElement( const ELEMENT_HALIGN alignH, const ELEMENT_VALIGN alignV, const Vector2f& margin, CTextRenderer& textRender ) :
	CGUIElement( SCREEN_ELEMENT_TYPE::TEXT, alignH, alignV, margin ),
	TextRender( textRender ),
	Scale( 1.0f, 1.0f )
{
	Color[0] = 1.0f;
	Color[1] = 1.0f;
	Color[2] = 1.0f;
	Color[3] = 1.0f;
}

CGUITextElement::~CGUITextElement()
{

}

void	CGUITextElement::Render( const Vector2f& screenSize )
{
	Vector2f size = TextRender.GetTextSize( Text ) * Scale;
	Vector2f pos = CreatePos( size, screenSize );

	TextRender.SetColor( Color[0], Color[1], Color[2], Color[3] );
	TextRender.Print( pos, Text, Scale );
}

void	CGUITextElement::Update( const float fTD )
{

}

const Vector2f	CGUITextElement::GetSize() const
{
	return TextRender.GetTextSize( Text ) * Scale;
}

void	CGUITextElement::SetText( const std::string& text )
{
	Text = text;
}

void	CGUITextElement::SetScale( const Vector2f& scale )
{
	Scale = scale;
}

void	CGUITextElement::SetColor( const float R, const float G, const float B, const float Alpha )
{
	Color[0] = R;
	Color[1] = G;
	Color[2] = B;
	Color[3] = Alpha;
}

const std::string	CGUITextElement::GetText() const
{
	return Text;
}