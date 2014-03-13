#include "GUIScreen.h"

CValueSync::CValueSync( CGUIElement* element, const std::string& name, const std::string& prefix, const std::string& sufix ) :
	Element( element ),
	VarName( name ),
	Prefix( prefix ),
	Sufix( sufix )
{
}

const std::string	CValueSync::GetVarName() const
{
	return VarName;
}

void	CValueSync::OnValueChange( const std::string& value )
{
	switch (Element->GetType())
	{
	case SCREEN_ELEMENT_TYPE::TEXT:
		((CGUITextElement*)Element)->SetText( Prefix + value + Sufix );
		break;

	default:
		break;
	}
}