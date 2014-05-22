#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <wx/wx.h>

#include "../../Engine/src/RenderView.h"
#include "../../Engine/src/RenderContext.h"

//#include "Render.h"
#include "RenderWindow.h"

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

class CApplication :
	public wxApp
{
private:
	//CRenderWindow	GLWindow;
	//CRender			GLRender;

	CRenderWindow*	m_pRenderWindow;
	CRenderView*	m_pRenderView;
	CRenderContext*	m_pRenderContext;

	CEventManager	EventManager;
	CControllerList	ControllerList;
	CScriptParser	ScriptParser;

	CGUIMain*	GUI;

	GAME_STATE	State;
	MOUSE_MODE	MouseMode;

	bool	active;

public:
	CApplication();
	virtual ~CApplication();

	virtual bool	OnInit() override;

private:
	void	RegScript();
	void	InitGraphics( CTexManager& texManager );

	virtual int	MainLoop() override;

	void	UpdateMouse();
	void	Update( const float fTD );
	void	Render();

public:
	void	LoadLevel( const std::string& filename );
	void	Print( const std::string& text );
};