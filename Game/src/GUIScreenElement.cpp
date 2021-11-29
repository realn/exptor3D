#include "GUIScreen.h"

CGUIElement::CGUIElement( const SCREEN_ELEMENT_TYPE type ) :
	Type( type ),
	AlignH( ELEMENT_HALIGN::CENTER ),
	AlignV( ELEMENT_VALIGN::CENTER ),
	Scale( 1.0f, 1.0f ),
	Color( 1.0f, 1.0f, 1.0f, 1.0f )
{
	
}

CGUIElement::~CGUIElement()
{
}

void	CGUIElement::SetAlign( const ELEMENT_HALIGN alignH, const ELEMENT_VALIGN alignV )
{
	AlignH = alignH;
	AlignV = alignV;
}

void	CGUIElement::SetMargin( const glm::vec2& margin )
{
	Margin = margin;
}

void	CGUIElement::SetScale( const glm::vec2& scale )
{
	Scale = scale;
}

void	CGUIElement::SetColor( const glm::vec4& color )
{
	Color = color;
}

const SCREEN_ELEMENT_TYPE	CGUIElement::GetType() const
{
	return Type;
}

const ELEMENT_HALIGN	CGUIElement::GetAlignH() const
{
	return AlignH;
}

const ELEMENT_VALIGN	CGUIElement::GetAlignV() const
{
	return AlignV;
}

const glm::vec2	CGUIElement::GetMargin() const
{
	return Margin;
}

const glm::vec2	CGUIElement::GetScale() const
{
	return Scale;
}

const glm::vec2	CGUIElement::CreatePos( const glm::vec2& size, const glm::vec2& screenSize ) const
{
	glm::vec2 pos;

	switch (AlignH)
	{
	case ELEMENT_HALIGN::LEFT:
		pos.x = Margin.x;
		break;

	case ELEMENT_HALIGN::CENTER:
		pos.x = ( screenSize.x - size.x ) / 2.0f;
		break;

	case ELEMENT_HALIGN::RIGHT:
		pos.x = screenSize.x - size.x - Margin.x;
		break;

	default:
		break;
	}

	switch (AlignV)
	{
	case ELEMENT_VALIGN::TOP:
		pos.y = Margin.y;
		break;

	case ELEMENT_VALIGN::CENTER:
		pos.y = ( screenSize.y - size.y ) / 2.0f;
		break;

	case ELEMENT_VALIGN::BOTTOM:
		pos.y = screenSize.y - size.y - Margin.y;
		break;

	default:
		break;
	}

	return pos;
}