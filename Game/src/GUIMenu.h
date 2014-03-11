#pragma once

#include "TextureText.h"
#include "EventManager.h"

enum class MENU_TAG
{
	UNKNOWN			= -1,
	ID				= 0,
	NAME			= 1,
	ITEM_ID			= 2,
	ITEM_NAME		= 3,
	ITEM_POSX		= 4,
	ITEM_POSY		= 5,
	ITEM_ACTION		= 6,
};

enum class MENU_ITEM_TYPE
{
	UNKNOWN = 0,
	BUTTON = 1,
};

class CMenuItem
{
private:
	const std::string	ID;
	std::string	Title;
	std::string Script;

	MENU_ITEM_TYPE	Type;

	bool Highlight;

	Vector2f	Pos;
	Vector2f	Size;
	Vector2f	Scroll;
	Vector2f	ScrollStep;
	float	Color;

public:
	CMenuItem( const std::string& id );

	void	Update( const float fTD );
	void	Render( CTextRenderer& TText );

	void	SetTitle( const std::string& title );
	void	SetType( const MENU_ITEM_TYPE type );
	void	SetPos( const Vector2f& pos );
	void	SetSize( const Vector2f& size );
	void	SetHighlight( const bool set );
	void	SetScript( const std::string& script );

	const std::string	GetScript() const;	
	const bool	Contains( const Vector2f& point ) const;
};

class CMenu
{
private:
	const std::string	ID;
	std::string	Title;

	std::vector<CMenuItem*> List;
	Vector2f	TitlePos;
	Vector2f	Size;

	CMenuItem*	pSelectedItem;

	bool	Visible;
	float	Scroll;

public:
	CMenu( const std::string& id );
	~CMenu();

	void	Update( const float fTD );
	void	Render( CTextRenderer& TText );

	void	EventMouseMove( const Vector2f& pos );
	void	EventMoveUp();
	void	EventMoveDown();
	void	EvetnEnter();

	void	AddMenuItem( CMenuItem* item );

	void	SetTitle( const std::string& title );
	void	SetTitlePos( const Vector2f& pos );
	void	SetSize( const Vector2f& size );
	void	SetVisible( const bool visible, const bool animate );

	const std::string&	GetID() const;
	const bool	IsVisible() const;
	const bool	IsAnimating() const;
};


class CMenuMain
{
private:
	CTextRenderer* TText;
	CTexture* Cursor;

	std::vector<CMenu*> List;
	std::vector<CMenu*>	Stack;
	CMenu*	MenuToShow;

	bool Clicked;
	bool WasClick;
	bool IsSliding;
	float curX;
	float curY;
	float slide;
	unsigned int CurMenu;
	unsigned int ToMenu;

	std::string file;
	std::string GetParamStr( std::string str );
	const MENU_TAG GetMenuType( std::string str );

public:
	CMenuMain();
	~CMenuMain();

	void Init( CTextRenderer* text, CTexture* cursor );
	void Free();

	void	EventMouseMove( const Vector2f& pos );
	void	EventMoveUp();
	void	EventMoveDown();
	void	EventEnter();

	void Update( const float fTD );
	void Render();

	bool Load( const std::string& filename );

private:
	void	Push( const std::string& id );
	void	Pop();

	CMenu*	FindMenu( const std::string& id );
};
