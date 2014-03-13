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

		if( cmd == "TEXT" && params.size() >= 5 )
		{
			pElement = AddTextElement( params[0], GetElementAlignV( params[1] ), GetElementAlignH( params[2] ), Vector2f( StrToFloat( params[3] ), StrToFloat( params[4] ) ) );

		}
	}
}