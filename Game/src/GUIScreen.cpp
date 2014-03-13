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

void	CGUIScreen::Render( const Vector2f& screenSize )
{
	TextRender.StartPrint( screenSize.X, screenSize.Y );

	for( unsigned i = 0; i < ElementList.size(); i++ )
		ElementList[i]->Render( screenSize );

	TextRender.EndPrint();
}

void	CGUIScreen::Update( const float fTD )
{
	for( unsigned i = 0; i < ElementList.size(); i++ )
		ElementList[i]->Update( fTD );
}

void	CGUIScreen::SetMargin( const Vector2f& margin )
{
	Margin = margin;
}

CGUITextElement*	CGUIScreen::AddTextElement( const std::string& text, const ELEMENT_HALIGN alignH, const ELEMENT_VALIGN alignV, const Vector2f& margin )
{
	auto pElement = new CGUITextElement( alignH, alignV, margin, TextRender );

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

		if( cmd == "TEXT" && params.size() == 5 )
		{
			pElement = AddTextElement( params[0], GetElementAlignH( params[1] ), GetElementAlignV( params[2] ), Vector2f( StrToFloat( params[3] ), StrToFloat( params[4] ) ) );
		}
		if( cmd == "SCALE" && params.size() == 2 )
		{
			if( pElement != nullptr )
				pElement->SetScale( Vector2f( StrToFloat( params[0] ), StrToFloat( params[1] ) ) );
		}
		if( cmd == "COLOR" && params.size() == 4 )
		{
			if( pElement != nullptr )
				pElement->SetColor( StrToFloat( params[0] ), StrToFloat( params[1] ), StrToFloat( params[2] ), StrToFloat( params[3] ) );
		}
		if( cmd == "SYNCVALUE" && params.size() >= 1 )
		{
			if( pElement != nullptr )
				AddValueSync( pElement, ClearWhiteSpace( params[0] ), params[1], params[2] );
		}
	}

	return true;
}

const ELEMENT_HALIGN			GetElementAlignH( const std::string& align )
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

const ELEMENT_VALIGN			GetElementAlignV( const std::string& align )
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