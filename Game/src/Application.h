#pragma once

#include "Render.h"
#include "Texture.h"
#include "EventManager.h"

enum class GAME_STATE
{
	MAINMENU,
	LEVEL,
};

class CApplication
{
private:
	CRender	GLRender;
	CEventManager	EventManager;

	GAME_STATE	State;
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

	void	Mouse();
	void	Update( const float fTD );
	void	Render();

public:
	void	LoadLevel( const std::string& filename );
};