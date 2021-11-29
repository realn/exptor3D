#pragma once

#include <CBSDL/System.h>
#include <CBSDL/Window.h>
#include <CBSDL/GLContext.h>
#include <CBSDL/Events.h>

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
	std::unique_ptr<cb::sdl::System> system;
	std::shared_ptr<cb::sdl::Window> window;
	std::shared_ptr<cb::sdl::GLContext> glContext;

	CEventManager	EventManager;
	CControllerList	ControllerList;
	CScriptParser	ScriptParser;

	std::unique_ptr<CGUIMain>	GUI;

	GAME_STATE	State;
	MOUSE_MODE	MouseMode;

	bool active = true;
	bool runLoop = true;

public:
	CApplication();
	~CApplication();

	int		Run();
	const bool	ProcessEvent( cb::sdl::Event& event );
	bool ProcessWindowEvent(cb::sdl::WindowEvent& event);
	bool ProcessMouseButtonEvent(cb::sdl::MouseButtonEvent& event);
	bool ProcessKeyEvent(cb::sdl::KeyboardEvent& event);
	bool ProcessMouseMoveEvent(cb::sdl::MouseMotionEvent& event);
	bool ProcessTextInput(cb::sdl::TextInputEvent& event);

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