#pragma once

#include "Render.h"
#include "Texture.h"
#include "EventManager.h"
#include "GameController.h"
#include "gui.h"

enum class GAME_STATE
{
	MAINMENU,
	LEVEL,
};

enum class MOUSE_MODE
{
	MENU,
	GAME,
};

class CApplication
{
private:
	CRender	GLRender;
	CEventManager	EventManager;
	CControllerList	ControllerList;
	CScriptParser	ScriptParser;

	CGUIMain*	GUI;

	GAME_STATE	State;
	MOUSE_MODE	MouseMode;

	bool	active;
	bool	Keys[256];

public:
	CApplication();
	~CApplication();

	int		Run();
	const bool	ProcessMsg( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	void	RegScript();
	void	InitGraphics( CTexManager& texManager );

	void	MainLoop();

	void	UpdateMouse();
	void	Update( const float fTD );
	void	Render();

public:
	void	LoadLevel( const std::string& filename );
	void	Print( const std::string& text );
};