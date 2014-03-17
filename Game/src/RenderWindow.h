#pragma once

#include <string>
#include <Windows.h>

class CRenderWindow
{
private:
	const std::string	WindowClassName;
	HINSTANCE	hInstance;
	HWND		hWindow;
	std::string	Title;
	DWORD		dwStyle;
	DWORD		dwExStyle;
	bool		FullScreen;
	unsigned	ColorBits;

public:
	CRenderWindow( HINSTANCE instance, const std::string& classname );
	~CRenderWindow();

	const bool	Create( const std::string& title, const int x, const int y, const unsigned width, const unsigned height, const bool fullscreen, WNDPROC windowProc, void* pUserData = nullptr, const bool client = true );
	void	Destroy();

	void	EnableFullScreen( const unsigned colorBits = 32 );
	void	DisableFullScreen();

	void	SetTitle( const std::string& title );
	void	SetPos( const int x, const int y );
	void	SetSize( const unsigned width, const unsigned height, const bool client = false );
	void	SetVisible( const bool visible );

	const unsigned	GetWidth( const bool client = false ) const;
	const unsigned	GetHeight( const bool client = false ) const;
	const int	GetX() const;
	const int	GetY() const;
	const bool	IsVisible() const;
	const bool	DoesExist() const;
	const bool IsFullScreen() const;
	HWND	GetHandle() const;
};