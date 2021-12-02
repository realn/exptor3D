#include "GUIScreen.h"
#include "StrEx.h"
#include <fstream>

CGUIScreen::CGUIScreen( CTextRenderer& textRender ) :
	TextRender( textRender )
{

}

CGUIScreen::~CGUIScreen()
{
	for( unsigned i = 0; i < ElementList.size(); i++ )
		delete ElementList[i];
	ElementList.clear();
}

void	CGUIScreen::Render( const glm::vec2& screenSize )
{
	TextRender.StartPrint( screenSize.x, screenSize.y );

	for( unsigned i = 0; i < ElementList.size(); i++ )
		ElementList[i]->Render( screenSize );

	TextRender.EndPrint();
}

void	CGUIScreen::Update( const float fTD )
{
	for( unsigned i = 0; i < ElementList.size(); i++ )
		ElementList[i]->Update( fTD );
}

void	CGUIScreen::SetMargin( const glm::vec2& margin )
{
	Margin = margin;
}

CGUITextElement*	CGUIScreen::AddTextElement( const std::string& text )
{
	auto pElement = new CGUITextElement( TextRender );

	if( !text.empty() )
		pElement->SetText( text );
	
	ElementList.push_back( pElement );

	return pElement;
}

void	CGUIScreen::AddValueSync( CGUIElement* pElement, const std::string& valueName, const std::string& prefix, const std::string& sufix )
{
	SyncList.push_back( CValueSync( pElement, valueName, prefix, sufix ) );
}

const bool	CGUIScreen::IsVarMonitored( const std::string& name ) const
{
	for( unsigned i = 0; i < SyncList.size(); i++ )
	{
		if( SyncList[i].GetVarName() == name )
			return true;
	}

	return false;
}

void	CGUIScreen::OnVarChanged( const std::string& name, const std::string& value )
{
	for( unsigned i = 0; i < SyncList.size(); i++ )
	{
		if( SyncList[i].GetVarName() == name )
			SyncList[i].OnValueChange( value );
	}
}

const bool	CGUIScreen::Load( const std::string& filename )
{
	if( filename.empty() )
		return false;

	std::fstream	file( filename, std::ios::in );

	CGUIElement* pElement = nullptr;

	while( file )
	{
		auto str = GetLine( file );

		auto pos = str.find( "(" );
		if( pos == std::string::npos )
			continue;

		auto endpos = str.find( ")", pos );
		if( endpos == std::string::npos )
			continue;

		auto cmd = ClearWhiteSpace( str.substr( 0, pos ) );
		auto paramline = str.substr( pos + 1, endpos - pos - 1 );
		std::vector<std::string> params;
		SplitString( paramline, ",", params );

		if( cmd == "TEXT" && params.size() >= 0 && params.size() <= 1 )
		{
			switch (params.size())
			{
			case 0:	pElement = AddTextElement(); break;
			case 1:	pElement = AddTextElement( params[0] ); break;
			}
		}
		else if( cmd == "ALIGN" && params.size() == 2 )
		{
			if( pElement != nullptr )
				pElement->SetAlign( GetElementAlignH( params[0] ), GetElementAlignV( params[1] ) );
		}
		else if( cmd == "MARGIN" && params.size() == 2 )
		{
			if( pElement != nullptr )
				pElement->SetMargin( glm::vec2( StrToFloat( params[0] ), StrToFloat( params[1] ) ) );
		}
		else if( cmd == "SCALE" && params.size() == 2 )
		{
			if( pElement != nullptr )
				pElement->SetScale( glm::vec2( StrToFloat( params[0] ), StrToFloat( params[1] ) ) );
		}
		else if( cmd == "COLOR" && params.size() >= 3 && params.size() <= 4 )
		{
			if( pElement != nullptr )
			{
				switch (params.size())
				{
				case 3:	pElement->SetColor( glm::vec4( StrToFloat( params[0] ), StrToFloat( params[1] ), StrToFloat( params[2] ), 1.0f ) );	break;
				case 4:	pElement->SetColor( glm::vec4( StrToFloat( params[0] ), StrToFloat( params[1] ), StrToFloat( params[2] ), StrToFloat( params[3] ) ));	break;
				}
			}
		}
		else if( cmd == "SYNCVALUE" && params.size() >= 1 && params.size() <= 3 )
		{
			if( pElement != nullptr )
			{
				switch (params.size())
				{
				case 1:	AddValueSync( pElement, ClearWhiteSpace( params[0] ) );	break;
				case 2:	AddValueSync( pElement, ClearWhiteSpace( params[0] ), params[1] );	break;
				case 3:	AddValueSync( pElement, ClearWhiteSpace( params[0] ), params[1], params[2] );	break;
				}
			}
		}
	}

	return true;
}

const ELEMENT_HALIGN	GetElementAlignH( const std::string& align )
{
	auto al = ClearWhiteSpace( align );
	if( al == "LEFT" )
		return ELEMENT_HALIGN::LEFT;
	if( al == "CENTER" )
		return ELEMENT_HALIGN::CENTER;
	if( al == "RIGHT" )
		return ELEMENT_HALIGN::RIGHT;

	return ELEMENT_HALIGN::CENTER;
}

const ELEMENT_VALIGN	GetElementAlignV( const std::string& align )
{
	auto al = ClearWhiteSpace( align );
	if( al == "TOP" )
		return ELEMENT_VALIGN::TOP;
	if( al == "CENTER" )
		return ELEMENT_VALIGN::CENTER;
	if( al == "BOTTOM" )
		return ELEMENT_VALIGN::BOTTOM;

	return ELEMENT_VALIGN::CENTER;
}