#include "GUIMenu.h"

#include "Render.h"

/*===========================
	KLASA guiMainMenu
	Steruje g³ównym menu gry.
===========================*/
guiMainMenu::guiMainMenu()
{
	Enabled = false;
	Clicked = false;
	IsSliding = false;
	file = "-";
	CurMenu = 0;
	ToMenu = 0;
	slide = 0.0f;
}

guiMainMenu::~guiMainMenu()
{
	Free();
}

void guiMainMenu::Init( CTextRenderer *text, CTexture* cursor )
{
	TText = text;
	Cursor = cursor;
	curX = 0.0f;
	curY = 0.0f;
}

void guiMainMenu::Update()
{
	if( !Enabled )
		return;

	if( List.size() <= 0 || CurMenu >= List.size() || CurMenu < 0 )
		return;

	if( IsSliding )
	{
		if( slide < 800.0f )
			slide += 10.0f;
		else
		{
			CurMenu = ToMenu;
			IsSliding = false;
			slide = 800.0f;
		}
	}
	else
	{
		if( slide > 0.0f )
			slide -= 10.0f;
		else slide = 0.0f;
	}

	guiMenu* Menu;
	Menu = List[CurMenu];
	Menu->slide = slide;

	Menu->Update( curX, curY, Clicked );
	Clicked = false;
}

void guiMainMenu::Render()
{
	if( !Enabled )
		return;

	if( List.size() <= 0 || CurMenu >= List.size() || CurMenu < 0 )
		return;

	guiMenu* Menu;
	Menu = List[CurMenu];

	Menu->Render( TText, Cursor );
}

void guiMainMenu::Click( unsigned int X, unsigned int Y, bool click )
{
	float ax = 1.0f;//800.0f / (float)GLRender.GetWidth();
	float ay = 1.0f;//600.0f / (float)GLRender.GetHeight();

	curX = ax * (float)X;
	curY = ay * (float)Y;

	Clicked = click;
}

void guiMainMenu::SetCursor( unsigned int X, unsigned int Y )
{
	float ax = 1.0f;//800.0f / (float)GLRender.GetWidth();
	float ay = 1.0f;//600.0f / (float)GLRender.GetHeight();

	curX = ax * (float)X;
	curY = ay * (float)Y;
}

void guiMainMenu::GoToMenu( unsigned int index )
{
	if( List.size() <= 0 || CurMenu >= List.size() || CurMenu < 0 )
		return;

	ToMenu = index;
	IsSliding = true;
}

void guiMainMenu::Disable()
{
	Enabled = false;
	CurMenu = 0;
}

void guiMainMenu::Enable()
{
	Enabled = true;
}

bool guiMainMenu::IsEnabled()
{
	return Enabled;
}

void guiMainMenu::AddMenu( guiMenu *Menu )
{
	List.push_back( Menu );
}

void guiMainMenu::DeleteMenu( unsigned int index )
{
	delete List[index];
	List.erase( List.begin() + index );
}

void guiMainMenu::Free()
{
	for( int i = List.size()-1; i >= 0; i-- )
	{
		DeleteMenu( i );
	}
}

std::string guiMainMenu::GetStr( FILE* fp )
{
	char buf[512];
	do fgets( buf, 512, fp );
	while( buf[0] == '\n' );
	return buf;
}

std::string guiMainMenu::GetParamStr( std::string str )
{
	int i;
	std::string zwrot = "";

	for( i = 0; i < str.length(); i++ )
	{
		if( str[i] == '=' )
			break;
	}

	if( str[i] != '=' )
	{
		Log.Error( "MAINMENU( " + file + " ): B³¹d: nie mo¿na znaleœæ znaku = w ³añcuchu: " + str );
		return "ERROR";
	}

	for( i++; i < str.length(); i++ )
	{
		if( str[i] == '\n' )
			continue;

		zwrot += str[i];
	}

	return zwrot;
}

float guiMainMenu::GetParamFloat( std::string str )
{
	return atof( GetParamStr( str ).c_str() );
}

int guiMainMenu::GetMenuType( std::string str )
{
	int i;
	std::string temp;
	for( i = 0; i < str.length(); i++ )
	{
		if( str[i] == '=' )
			break;

		temp += str[i];
	}

	if( str[i] != '=' )
	{
		Log.Error( "MAINMENU( " + file + " ): B³¹d: nie mo¿na znaleœæ znaku = w ³añcuchu: " + str );
		return MENU_ERROR;
	}

	if( temp == "MENU" )
		return MENU_NAME;
	if( temp == "MENUITEM" )
		return MENU_ITEM_NAME;
	if( temp == "MENUITEMPOSX" )
		return MENU_ITEM_POSX;
	if( temp == "MENUITEMPOSY" )
		return MENU_ITEM_POSY;
	if( temp == "MENUITEMACTION" )
		return MENU_ITEM_ACTION;
	if( temp == "MENUITEMGOTO" )
		return MENU_ITEM_GOTO;

	return MENU_ERROR;
}

bool guiMainMenu::LoadMenu( std::string filename )
{
	if( filename == "" )
	{
		Log.Error( "MAINMENU( " + file + " ): Pusty ci¹g znaków w œcie¿ce do pliku" );
		return false;
	}

	// Uchwyt do pliku
	FILE* fp = 0;

	fopen_s(&fp, filename.c_str(), "rt" );

	// sprawdzamy uchwyt
	if( !fp )
	{
		Log.Error( "MAINMENU( " + file + " ): B³êdna œcie¿ka" );
		return false;
	}

	this->Free();

	file = filename;
	std::string str = "";
	guiMenu* Menu = NULL;
	guiMenuItem* Item = NULL;

	while( !feof( fp )  )
	{
		str = GetStr( fp );

		switch( GetMenuType( str ) )
		{
		case MENU_NAME :
			Item = NULL;
			Menu = new guiMenu;
			this->AddMenu( Menu );
			Menu->MainCaption = GetParamStr( str );
			break;

		case MENU_ITEM_NAME :
			if( Menu == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma menu, by dodaæ pozycje, ³añcuch: " + str );
				break;
			}
			Item = new guiMenuItem;
			Menu->AddMenuItem( Item );
			Item->Caption = GetParamStr( str );
			break;

		case MENU_ITEM_POSX :
			if( Item == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
				break;
			}
			Item->X = GetParamFloat( str );
			break;

		case MENU_ITEM_POSY :
			if( Item == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
				break;
			}
			Item->Y = GetParamFloat( str );
			break;

		case MENU_ITEM_ACTION :
			if( Item == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
				break;
			}
			Item->Action = GetParamStr( str );
			break;

		case MENU_ITEM_GOTO :
			if( Item == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
				break;
			}
			Item->GoTo = (int)GetParamFloat( str );
			break;

		default:
		case MENU_ERROR :
			Log.Error( "MAINMENU( " + file + " ): Nierozpoznano typu: " + str );
			break;
		}
	}

	fclose( fp );

	return true;
}
