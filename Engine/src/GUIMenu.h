#pragma once

#include <glm/vec2.hpp>

#include "GUITextPrinter.h"
#include "GUIRenderContext.h"

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

	glm::vec2	Pos;
	glm::vec2	Size;
	glm::vec2	Scroll;
	glm::vec2	ScrollStep;
	float	Color;

public:
	CMenuItem( const std::string& id );

	void	Update( const float fTD );
	void	Render(gui::RenderContext& ctx, gui::TextPrinter& printer);

	void	SetTitle( const std::string& title );
	void	SetType( const MENU_ITEM_TYPE type );
	void	SetPos( const glm::vec2& pos );
	void	SetSize( const glm::vec2& size );
	void	SetHighlight( const bool set );
	void	SetScript( const std::string& script );

	const std::string	GetScript() const;	
	const bool	Contains( const glm::vec2& point ) const;
};

class CMenu
{
private:
	const std::string	ID;
	std::string	Title;

	std::vector<CMenuItem*> List;
	glm::vec2	TitlePos;
	glm::vec2	Size;

	CMenuItem*	pSelectedItem;

	bool	Visible;
	float	Scroll;

public:
	CMenu( const std::string& id );
	~CMenu();

	void	Update( const float fTD );
	void	Render(gui::RenderContext& ctx, gui::TextPrinter& printer);

	void	EventMouseMove( const glm::vec2& pos );
	void	EventMoveUp();
	void	EventMoveDown();
	const bool	EventEnter( std::string& outScript );

	void	AddMenuItem( CMenuItem* item );

	void	SetTitle( const std::string& title );
	void	SetTitlePos( const glm::vec2& pos );
	void	SetSize( const glm::vec2& size );
	void	SetVisible( const bool visible, const bool animate );

	const std::string&	GetID() const;
	const bool	IsVisible() const;
	const bool	IsAnimating() const;
};


class CMenuMain
{
private:
	std::vector<CMenu*> List;
	std::vector<CMenu*>	Stack;
	std::string file;
	CMenu*	MenuToShow;
	float	AspectRatio;

	const std::string GetParamStr( const std::string& str ) const;
	const float		GetParamFloat( const std::string& str ) const;
	const glm::vec2	GetParamVector2( const std::string& str ) const;
	const MENU_TAG GetMenuType( std::string str );

public:
	CMenuMain(const float aspectRatio );
	~CMenuMain();

	void	EventMouseMove( const glm::vec2& pos );
	void	EventMoveUp();
	void	EventMoveDown();
	const bool	EventEnter( std::string& outScript );

	void	Update( const float fTD );
	void	Render(gui::RenderContext& ctx, gui::TextPrinter& printer);

	bool	Load( const std::string& filename );
	void	Free();

	void	Push( const std::string& id );
	void	Pop();

	const bool	IsMenuAnimating() const;

private:
	CMenu*	FindMenu( const std::string& id );
};