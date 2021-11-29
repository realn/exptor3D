#include "GUIScreen.h"

CGUITextElement::CGUITextElement( CTextRenderer& textRender ) :
	CGUIElement( SCREEN_ELEMENT_TYPE::TEXT ),
	TextRender( textRender )
{
}

CGUITextElement::~CGUITextElement()
{

}

void	CGUITextElement::Render( const glm::vec2& screenSize )
{
	glm::vec2 size = TextRender.GetTextSize( Text ) * Scale;
	glm::vec2 pos = CreatePos( size, screenSize );

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

const glm::vec2	CGUITextElement::GetSize() const
{
	return TextRender.GetTextSize( Text ) * Scale;
}

const std::string	CGUITextElement::GetText() const
{
	return Text;
}