#include "GUIScreen.h"

CGUITextElement::CGUITextElement() :
	CGUIElement( SCREEN_ELEMENT_TYPE::TEXT )
{
}

CGUITextElement::~CGUITextElement()
{

}

void	CGUITextElement::Render(gui::RenderContext& ctx, gui::TextPrinter& printer, const glm::vec2& screenSize)
{
	textSize = printer.getTextSize( Text ) * Scale;
	glm::vec2 pos = CreatePos( textSize, screenSize );

	ctx.setColor( Color );
	ctx.pushMatrix();
	ctx.scale({ Scale, 1.0f });
	printer.print(ctx, pos, Text);
	ctx.popMatrix();
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
	return textSize;
}

const std::string	CGUITextElement::GetText() const
{
	return Text;
}