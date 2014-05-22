#include "stdafx.h"
#include "GUIScreen.h"

CGUITextElement::CGUITextElement( CTextRenderer& textRender ) :
	CGUIElement( SCREEN_ELEMENT_TYPE::TEXT ),
	TextRender( textRender )
{
}

CGUITextElement::~CGUITextElement()
{

}

void	CGUITextElement::Render( const Vector2f& screenSize )
{
	Vector2f size = TextRender.GetTextSize( Text ) * Scale;
	Vector2f pos = CreatePos( size, screenSize );

	TextRender.SetColor( Color );
	TextRender.Print( pos, Text, Scale );
}

void	CGUITextElement::Update( const float fTD )
{

}

void	CGUITextElement::SetText( const std::string& text )
{
	Text = text;
}

const Vector2f	CGUITextElement::GetSize() const
{
	return TextRender.GetTextSize( Text ) * Scale;
}

const std::string	CGUITextElement::GetText() const
{
	return Text;
}