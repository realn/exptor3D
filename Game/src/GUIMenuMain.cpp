#include "GUIMenu.h"

#include "Render.h"

#include <fstream>

/*===========================
	KLASA CMenuMain
	Steruje g³ównym menu gry.
===========================*/
CMenuMain::CMenuMain() : 
	MenuToShow( nullptr )
{
	Clicked = false;
	IsSliding = false;
	file = "-";
	CurMenu = 0;
	ToMenu = 0;
	slide = 0.0f;
}

CMenuMain::~CMenuMain()
{
	Free();
}

void CMenuMain::Init( CTextRenderer *text, CTexture* cursor )
{
	TText = text;
	Cursor = cursor;
	curX = 0.0f;
	curY = 0.0f;
}

void CMenuMain::Update( const float fTD )
{
	if( Stack.empty() )
		return;

	auto pMenu = Stack.back();
	if( !pMenu->IsVisible() && !pMenu->IsAnimating() )
	{
		if( MenuToShow != nullptr )
		{
			Stack.push_back( MenuToShow );
			pMenu = MenuToShow;
			MenuToShow->SetVisible( true, true );
			MenuToShow = nullptr;
		}
		else
		{
			Stack.pop_back();
			if( !Stack.empty() )
			{
				pMenu = Stack.back();
				pMenu->SetVisible( true, true );
			}
			else
				return;
		}
	}
	pMenu->Update( fTD );
}

void CMenuMain::Render()
{
	if( Stack.empty() )
		return;

	Stack.back()->Render( *TText );
}

void	CMenuMain::Push( const std::string& id )
{
	auto menu = FindMenu( id );
	if( menu == nullptr )
		return;

	if( !Stack.empty() )
	{
		Stack.back()->SetVisible( false, true );
		MenuToShow = menu;
	}
	else
	{
		menu->SetVisible( true, true );
		Stack.push_back( menu );
	}
}

void	CMenuMain::Pop()
{
	if( Stack.empty() )
		return;

	Stack.back()->SetVisible( false, true );
}

CMenu*	CMenuMain::FindMenu( const std::string& id )
{
	for( unsigned i = 0; i < List.size(); i++ )
	{
		if( List[i]->GetID() == id )
			return List[i];
	}
	return nullptr;
}

void	CMenuMain::EventMouseMove( const Vector2f& pos )
{
	if( Stack.empty() )
		return;
	
	auto menu = Stack.back();
	if( menu->IsAnimating() )
		return;

	menu->EventMouseMove( pos );
}

void	CMenuMain::EventEnter()
{
	if( Stack.empty() )
		return;
	
	auto menu = Stack.back();
	if( menu->IsAnimating() )
		return;

	menu->EvetnEnter();
}

void	CMenuMain::EventMoveDown()
{
	if( Stack.empty() )
		return;
	
	auto menu = Stack.back();
	if( menu->IsAnimating() )
		return;

	menu->EventMoveDown();
}

void	CMenuMain::EventMoveUp()
{
	if( Stack.empty() )
		return;
	
	auto menu = Stack.back();
	if( menu->IsAnimating() )
		return;

	menu->EventMoveUp();
}

void CMenuMain::Free()
{
	for( unsigned i = 0; i < List.size(); i++ )
		delete List[i];
	List.clear();
	Stack.clear();
}

std::string CMenuMain::GetParamStr( std::string str )
{
	auto pos = str.find( "=" );
	if( pos == std::string::npos )
	{
		Log.Error( "MAINMENU( " + file + " ): B³¹d: nie mo¿na znaleœæ znaku = w ³añcuchu: " + str );
		return "ERROR";
	}

	return str.substr( pos + 1 );
}

const MENU_TAG CMenuMain::GetMenuType( std::string str )
{
	auto pos = str.find( "=" );
	if( pos == std::string::npos )
	{
		Log.Error( "MAINMENU( " + file + " ): B³¹d: nie mo¿na znaleœæ znaku = w ³añcuchu: " + str );
		return MENU_TAG::UNKNOWN;
	}

	std::string temp = str.substr( 0, pos );

	if( temp == "MENU" )
		return MENU_TAG::ID;
	if( temp == "MENUTITLE" )
		return MENU_TAG::NAME;
	if( temp == "MENUITEM" )
		return MENU_TAG::ITEM_ID;
	if( temp == "MENUITEMTITLE" )
		return MENU_TAG::ITEM_NAME;
	if( temp == "MENUITEMPOSX" )
		return MENU_TAG::ITEM_POSX;
	if( temp == "MENUITEMPOSY" )
		return MENU_TAG::ITEM_POSY;
	if( temp == "MENUITEMACTION" )
		return MENU_TAG::ITEM_ACTION;

	return MENU_TAG::UNKNOWN;
}

bool CMenuMain::Load( const std::string& filename )
{
	if( filename.empty() )
	{
		Log.Error( "MAINMENU( " + file + " ): Pusty ci¹g znaków w œcie¿ce do pliku" );
		return false;
	}

	std::fstream fileStream( filename, std::ios::in );

	// sprawdzamy uchwyt
	if( !fileStream )
	{
		Log.Error( "MAINMENU( " + file + " ): B³êdna œcie¿ka" );
		return false;
	}

	this->Free();

	file = filename;
	std::string str = "";
	CMenu*		Menu = nullptr;
	CMenuItem*	Item = nullptr;
	Vector2f	pos;

	while( fileStream  )
	{
		str = GetLine( fileStream );

		switch( GetMenuType( str ) )
		{
		case MENU_TAG::ID:
			Item = nullptr;
			Menu = new CMenu( GetParamStr( str ) );
			List.push_back( Menu );
			break;

		case MENU_TAG::NAME:
			if( Menu != nullptr )
				Menu->SetTitle( GetParamStr( str ) );
			break;

		case MENU_TAG::ITEM_ID:
			Item = nullptr;
			if( Menu != nullptr )
			{
				Item = new CMenuItem( GetParamStr( str ) );
				Menu->AddMenuItem( Item );
			}
			else
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma menu, by dodaæ pozycje, ³añcuch: " + str );
			break;

		case MENU_TAG::ITEM_NAME:
			if( Item != nullptr )
				Item->SetTitle( GetParamStr( str ) );
			break;

		case MENU_TAG::ITEM_POSX:
			if( Item != nullptr )
			{
				pos.X = StrToFloat( GetParamStr( str ) );
				Item->SetPos( pos );
				break;
			}
			else
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
			break;

		case MENU_TAG::ITEM_POSY:
			if( Item != nullptr )
			{
				pos.Y = StrToFloat( GetParamStr( str ) );
				Item->SetPos( pos );
				break;
			}
			else
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
			break;

		case MENU_TAG::ITEM_ACTION:
			if( Item != nullptr )
			{
				Item->SetScript( GetParamStr( str ) );
			}
			else
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
			break;

		default:
		case MENU_TAG::UNKNOWN :
			Log.Error( "MAINMENU( " + file + " ): Nierozpoznano typu: " + str );
			break;
		}
	}

	return true;
}
