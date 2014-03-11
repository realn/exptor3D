#include "GUIMenu.h"

/*======================
KLASA CMenu
Klasa reprezentuj¹ca pojedyñcze menu.
Zarz¹dza jednoczeœnie swoimi przyciskami
======================*/
CMenu::CMenu( const std::string& id ) :
	ID( id ),
	Visible( true ),
	Scroll( 0.0f ),
	TitlePos( 10.0f, 20.0f ),
	pSelectedItem( nullptr )
{

}

CMenu::~CMenu()
{
	for( unsigned i = 0; i < List.size(); i++ )
		delete List[i];
	List.clear();
}

void	CMenu::Update( const float fTD )
{
	if( Visible )
	{
		if( Scroll > 0.0f )
			Scroll -= fTD;
		else
		{
			Scroll = 0.0f;
			for( unsigned i = 0; i < List.size(); i++ )
				List[i]->Update( fTD );
		}
	}
	else
	{
		if( Scroll < 1.0f )
			Scroll += fTD;
		else
			Scroll = 1.0f;
	}
}

void CMenu::Render( CTextRenderer& TText )
{
	//TText.StartPrint();
	//cursor->Activate();
	//glDisable( GL_CULL_FACE );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	//glBegin( GL_TRIANGLE_STRIP );
	//glNormal3f( 0.0f, 0.0f, 1.0f );
	//glTexCoord2f( 0.0f, 0.0f );
	//glVertex3f( x, y+50.0f, -0.1f );
	//glTexCoord2f( 0.0f, 1.0f );
	//glVertex3f( x, y, -0.1f );
	//glTexCoord2f( 1.0f, 0.0f );
	//glVertex3f( 50.0f+x, y+50.0f, -0.1f );
	//glTexCoord2f( 1.0f, 1.0f );
	//glVertex3f( 50.0f+x, y, -0.1f );
	//glEnd();
	//TText.EndPrint();

	TText.StartPrint( Size.X, Size.Y );
	TText.SetColor( 1.0f, 1.0f, 1.0f );

	glTranslatef( Scroll * Size.X, 0.0f, 0.0f );
	TText.Print( TitlePos.X, TitlePos.Y, Title, 2.5f, 2.5f );

	for( int i = 0; i < List.size(); i++ )
	{
		List[i]->Render( TText );
	}

	TText.EndPrint();
}

void CMenu::EventMouseMove( const Vector2f& pos )
{
	auto screenPos = pos * Size;
	CMenuItem* Item;
	pSelectedItem = nullptr;
	for( int i = 0; i < List.size(); i++ )
	{
		Item = List[i];

		bool contains = Item->Contains( screenPos );
		Item->SetHighlight( contains );
		if( contains )
			pSelectedItem = Item;
	}
}

void	CMenu::EvetnEnter()
{
	if( pSelectedItem != nullptr )
	{
		if( pSelectedItem->GetScript() == "ReturnMenu" )
			this->SetVisible( false, true );
	}
}

void	CMenu::EventMoveUp()
{
	if( pSelectedItem == nullptr )
	{
		if( !List.empty() )
		{
			pSelectedItem = List.back();
			pSelectedItem->SetHighlight( true );
		}
	}
	else
	{
		pSelectedItem->SetHighlight( false );
		auto it = std::find( List.cbegin(), List.cend(), pSelectedItem );
		if( it != List.cend() )
		{
			unsigned index = std::distance( List.cbegin(), it );
			index = (index + 1) % List.size();

			pSelectedItem = List[index];
			pSelectedItem->SetHighlight( true );
		}
	}
}

void	CMenu::EventMoveDown()
{
	if( pSelectedItem == nullptr )
	{
		if( !List.empty() )
		{
			pSelectedItem = List.front();
			pSelectedItem->SetHighlight( true );
		}
	}
	else
	{
		pSelectedItem->SetHighlight( false );
		auto it = std::find( List.cbegin(), List.cend(), pSelectedItem );
		if( it != List.cend() )
		{
			unsigned index = std::distance( List.cbegin(), it );
			if( index == 0 )
				pSelectedItem = List.back();
			else
				pSelectedItem = List[index - 1];

			pSelectedItem->SetHighlight( true );
		}
	}
}

void CMenu::AddMenuItem( CMenuItem* item )
{
	List.push_back( item );
}

void	CMenu::SetTitle( const std::string& title )
{
	Title = title;
}

void	CMenu::SetTitlePos( const Vector2f& pos )
{
	TitlePos = pos;
}

void	CMenu::SetSize( const Vector2f& size )
{
	Size = size;
}

void	CMenu::SetVisible( const bool visible, const bool animate )
{
	if( visible )
	{
		Visible = true;
		if( !animate )
			Scroll = 0.0f;
	}
	else
	{
		Visible = false;
		if( !animate )
			Scroll = 1.0f;
	}
}

const std::string&	CMenu::GetID() const
{
	return ID;
}

const bool	CMenu::IsVisible() const
{
	return Visible;
}

const bool	CMenu::IsAnimating() const
{
	if( Visible )
		return Scroll > 0.0f;
	else 
		return Scroll < 1.0f;
}