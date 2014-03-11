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
	ScrollStep( 10.0f, 0.0f ),
	Color( 1.0f )
{
}

void CMenuItem::Update( const float fTD )
{
	if( !Highlight )
	{
		if( Color > 0.5f )
			Color -= 0.5f * fTD;
		else 
			Color = 0.5f;
		
		if( Scroll.X > 0.0f )
			Scroll.X -= 0.5f * fTD;
		else 
			Scroll.X = 0.0f;
	}
	else
	{
		if( Color < 1.0f )
			Color += fTD;
		else
			Color = 1.0f;

		if( Scroll.X < 1.0f )
			Scroll.X += fTD;
		else
			Scroll.X = 1.0f;
	}
}

void CMenuItem::Render( CTextRenderer &TText )
{
	if( Size.X == 0.0f || Size.Y == 0.0f )
		Size = TText.GetTextSize( Title );

	TText.SetColor( Color, Color, Color );
	TText.Print( Pos.X + Scroll.X * ScrollStep.X, Pos.Y + Scroll.Y * ScrollStep.Y, this->Title, 1.5f, 1.5f );
}

void	CMenuItem::SetTitle( const std::string& title )
{
	Title = title;
	Size.X = 0.0f;
	Size.Y = 0.0f;
}

void	CMenuItem::SetPos( const Vector2f& pos )
{
	Pos = pos;
}

void	CMenuItem::SetSize( const Vector2f& size )
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

const bool	CMenuItem::Contains( const Vector2f& point ) const
{
	return point.X > Pos.X && point.X < Pos.X + Size.X && point.Y > Pos.Y && point.Y < Pos.Y + Size.Y;
}