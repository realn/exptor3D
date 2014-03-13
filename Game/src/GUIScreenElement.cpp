#include "GUIScreen.h"

CGUIElement::CGUIElement( const SCREEN_ELEMENT_TYPE type, const ELEMENT_HALIGN alignH, const ELEMENT_VALIGN alignV, const Vector2f& margin ) :
	Type( type ),
	AlignH( alignH ),
	AlignV( alignV ),
	Margin( margin )
{
}

CGUIElement::~CGUIElement()
{
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

const Vector2f	CGUIElement::GetMargin() const
{
	return Margin;
}

const Vector2f	CGUIElement::CreatePos( const Vector2f& size, const Vector2f& screenSize ) const
{
	Vector2f pos;

	switch (AlignH)
	{
	case ELEMENT_HALIGN::LEFT:
		pos.X = Margin.X;
		break;

	case ELEMENT_HALIGN::CENTER:
		pos.X = ( screenSize.X - size.X ) / 2.0f;
		break;

	case ELEMENT_HALIGN::RIGHT:
		pos.X = screenSize.X - size.X - Margin.X;
		break;

	default:
		break;
	}

	switch (AlignV)
	{
	case ELEMENT_VALIGN::TOP:
		pos.Y = Margin.Y;
		break;

	case ELEMENT_VALIGN::CENTER:
		pos.Y = ( screenSize.Y - size.Y ) / 2.0f;
		break;

	case ELEMENT_VALIGN::BOTTOM:
		pos.Y = screenSize.Y - size.Y - Margin.Y;
		break;

	default:
		break;
	}

	return pos;
}