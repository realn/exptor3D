#include "GUIMenu.h"

/*======================
	KLASA CMenuItem
	Jest to klasa reprezentuj¹ca
	wybór w menu.
======================*/
CMenuItem::CMenuItem( const std::string& id ) :
	ID(id),
	Type( MENU_ITEM_TYPE::UNKNOWN ),
	Highlight( false ),
	ScrollStep( 20.0f, 0.0f ),
	Color( 1.0f )
{
}

void CMenuItem::Update( const float fTD )
{
	if( !Highlight )
	{
		if( Color > 0.5f )
			Color -= 1.0f * fTD;
		else 
			Color = 0.5f;
		
		if( Scroll.x > 0.0f )
			Scroll.x -= 1.0f * fTD;
		else 
			Scroll.x = 0.0f;
	}
	else
	{
		if( Color < 1.0f )
			Color += 2.0f * fTD;
		else
			Color = 1.0f;

		if( Scroll.x < 1.0f )
			Scroll.x += 2.0f * fTD;
		else
			Scroll.x = 1.0f;
	}
}

void CMenuItem::Render( CTextRenderer &TText )
{
	if( Size.x == 0.0f || Size.y == 0.0f )
		Size = TText.GetTextSize( Title );

	TText.SetColor( Color, Color, Color );
	TText.Print( Pos.x + Scroll.x * ScrollStep.x, Pos.y + Scroll.y * ScrollStep.y, this->Title, 1.0f, 1.0f );
}

void	CMenuItem::SetTitle( const std::string& title )
{
	Title = title;
	Size.x = 0.0f;
	Size.y = 0.0f;
}

void	CMenuItem::SetPos( const glm::vec2& pos )
{
	Pos = pos;
}

void	CMenuItem::SetSize( const glm::vec2& size )
{
	Size = size;
}

void	CMenuItem::SetType( const MENU_ITEM_TYPE type )
{
	Type = type;
}

void	CMenuItem::SetHighlight( const bool set )
{
	Highlight = set;
}

void	CMenuItem::SetScript( const std::string& script )
{
	Script = script;
}

const std::string	CMenuItem::GetScript() const
{
	return Script;
}

const bool	CMenuItem::Contains( const glm::vec2& point ) const
{
	return point.x > Pos.x && point.x < Pos.x + Size.x && point.y > Pos.y && point.y < Pos.y + Size.y;
}