#include <CBGL/COpenGL.h>

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
			Scroll -= fTD * 2.0f;
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
			Scroll += fTD * 2.0f;
		else
			Scroll = 1.0f;
	}
}

void CMenu::Render(gui::RenderContext& ctx, gui::TextPrinter& printer)
{
	ctx.pushMatrix();
	ctx.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	ctx.translate({Scroll * Size.x, 0.0f, 0.0f});

	ctx.pushMatrix();
	ctx.scale({ 2.5f, 2.5f, 1.0f });
	printer.print(ctx, TitlePos, Title);
	ctx.popMatrix();

	for (unsigned i = 0; i < List.size(); i++)
		List[i]->Render(ctx, printer);

	ctx.popMatrix();
}

void CMenu::EventMouseMove( const glm::vec2& pos )
{
	auto screenPos = pos * Size;
	CMenuItem* Item;
	pSelectedItem = nullptr;
	for( unsigned i = 0; i < List.size(); i++ )
	{
		Item = List[i];

		bool contains = Item->Contains( screenPos );
		Item->SetHighlight( contains );
		if( contains )
			pSelectedItem = Item;
	}
}

const bool	CMenu::EventEnter( std::string& outScript )
{
	if( pSelectedItem != nullptr )
	{
		outScript = pSelectedItem->GetScript();
		return true;
	}
	return false;
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
			auto index = std::distance( List.cbegin(), it );
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
			auto index = std::distance( List.cbegin(), it );
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

void	CMenu::SetTitlePos( const glm::vec2& pos )
{
	TitlePos = pos;
}

void	CMenu::SetSize( const glm::vec2& size )
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