#pragma once

#include "TextureText.h"
#include "EventManager.h"

enum class MENU_TAG
{
	UNKNOWN,
	MENU_ID,
	MENU_NAME,
	MENU_SIZE,
	MENU_MARGIN,
	MENU_STEP,
	ITEM_ID,
	ITEM_NAME,
	ITEM_POS,
	ITEM_ACTION,
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
	const bool	EventEnter( std::string& outScript );

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
	CTextRenderer& TextRender;

	std::vector<CMenu*> List;
	std::vector<CMenu*>	Stack;
	std::string file;
	CMenu*	MenuToShow;
	float	AspectRatio;

	const std::string GetParamStr( const std::string& str ) const;
	const float		GetParamFloat( const std::string& str ) const;
	const Vector2f	GetParamVector2( const std::string& str ) const;
	const MENU_TAG GetMenuType( std::string str );

public:
	CMenuMain( CTextRenderer& textRender, const float aspectRatio );
	~CMenuMain();

	void	EventMouseMove( const Vector2f& pos );
	void	EventMoveUp();
	void	EventMoveDown();
	const bool	EventEnter( std::string& outScript );

	void	Update( const float fTD );
	void	Render();

	bool	Load( const std::string& filename );
	void	Free();

	void	Push( const std::string& id );
	void	Pop();

	const bool	IsMenuAnimating() const;

private:
	CMenu*	FindMenu( const std::string& id );
};