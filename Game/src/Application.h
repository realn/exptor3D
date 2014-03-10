#pragma once

#include "Render.h"
#include "Texture.h"

class CApplication
{
private:
	CRender GLRender;

	bool	active;
	bool	Keys[256];

public:
	CApplication();

	int		Run();
	int		ProcessMsg( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	void	InitGraphics( CTexManager& texManager );

	void	MainLoop();

	void	Mouse();
	void	Update( const float fTD );
	void	Render();
};