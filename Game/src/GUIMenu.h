#pragma once

#include "TextureText.h"

#define MENU_ERROR			-1
#define MENU_NAME			0
#define MENU_ITEM_NAME		1
#define MENU_ITEM_POSX		2
#define MENU_ITEM_POSY		3
#define MENU_ITEM_ACTION	4
#define MENU_ITEM_GOTO		5

class guiMenuItem
{
private:
	bool Enabled;
	bool Highlight;
	float scrollX;
	float scrollY;
public:
	std::string ID;
	std::string Caption;
	float X;
	float Y;
	float Color;
	float Alpha;
	unsigned int GoTo;
	std::string Action;

	guiMenuItem();

	void Update();
	void Render( CTextRenderer* TText );

	void Enable();
	void Disable();

	void HighLight();
	void DontHighLight();

	void DoAction();
	void DoGoTo();
	
};

class guiMenu
{
private:
	float scroll;
	std::vector<guiMenuItem*> List;
	float x, y;
public:
	std::string ID;
	std::string MainCaption;
	float slide;

	guiMenu();
	~guiMenu();
	void Free();

	void Update( float cX, float cY, bool click );
	void Render( CTextRenderer* TText, CTexture* cursor );
	void AddMenuItem( guiMenuItem* item );
	void DeleteMenuItem( unsigned int index );
};


class guiMainMenu
{
private:
	CTextRenderer* TText;
	CTexture* Cursor;
	std::vector<guiMenu*> List;

	bool Enabled;
	bool Clicked;
	bool WasClick;
	bool IsSliding;
	float curX;
	float curY;
	float slide;
	unsigned int CurMenu;
	unsigned int ToMenu;

	std::string file;

	void AddMenu( guiMenu* Menu );
	void DeleteMenu( unsigned int index );
	std::string GetStr( FILE* fp );
	std::string GetParamStr( std::string str );
	float GetParamFloat( std::string str );
	int GetMenuType( std::string str );
public:
	guiMainMenu();
	~guiMainMenu();

	void Init( CTextRenderer* text, CTexture* cursor );
	void Free();

	void Click( unsigned int X, unsigned int Y, bool click );
	void SetCursor( unsigned int X, unsigned int Y );

	void GoToMenu( unsigned int index );

	void Update();
	void Render();

	void Enable();
	void Disable();
	bool IsEnabled();

	bool LoadMenu( std::string filename );
};
