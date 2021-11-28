/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	gui.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Definicje klas do zarz¹dzania programem oraz komunikacj¹
		z graczem. 

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include "Log.h"
#include "Texture.h"

#include "TextureText.h"
#include "ScriptParser.h"

#include "GUIMenu.h"
#include "GUIConsole.h"
#include "GUIScreen.h"

#include "EventManager.h"

enum class GUI_MODE
{
	MENU,
	SCREEN,
};

enum class GUI_TIME
{
	MILISECONDS,
	SECONDS,
	MINUTES,
	HOUR,
};

struct guiPlayerInfo
{
	float HEALTH;
	float ARMOR;
	int AMMO;
	int CLIPS;
	int FRAGS;
	std::string Name;
	float angle;
	std::string WeapName;
};

struct guiScrMsg
{
	std::string msg;
	float Alpha;
	unsigned int Time;
	unsigned int ThisTime;
	float C[3];
	float S[2];
	float x, y;
};

class CGUIMain :
	public IEventObserver
{
private:
	CTexture *CH;
	CTextRenderer TextRender;
	CTexture *Cursor;

	GUI_MODE	Mode;
	CMenuMain	Menu;
	CGUIConsole	Console;
	CGUIScreen	Screen;
	CScriptParser&	ScriptParser;

	unsigned short Second;
	unsigned short Minute;
	unsigned short Hour;

	float	FrameTime;
	float	CurrentTD;
	float	AspectRatio;
	float	ConsoleScroll;
	float	FScrColor[4];
	float	ShowWLScrTime;
	float	ThisWLScrTime;
	float	WLScrColor[4];

	unsigned	Frame;
	unsigned	ScreenHeight;
	unsigned	cursorX, cursorY;

	bool	ConsoleOn;
	bool	Quit;

	std::vector< std::string >	ConsoleLMsg;
	std::string		ConsoleCMsg;
	std::vector< guiScrMsg >	ScrMsg;
	std::vector< std::string >	PFL;

public:
	CGUIMain( CTexManager& texManager, CScriptParser& scriptParser, const float aspectRation, const unsigned height );

	void	Update(const float fTD);
	void	Render();

	void	ShowMenu( const std::string& strID );
	void	HideMenu();
	const bool	IsMenuAnimating() const;
	
	void	SetMode( const GUI_MODE mode );

	bool	GetQuit();

	void	UpdateCounter( const float fTD );
	const unsigned	GetTime( const GUI_TIME type ) const;
	const unsigned	GetFrameRate() const;
	const unsigned	GetMiliSecPas() const;

	void	ActiveFScrColor( float R, float G, float B, float Alpha = 0.5f );

	void	SendMsg( std::string msg, unsigned int Time = 2000, float X = -1.0f, float Y = -1.0f, float SclX = 1.0f, float SclY = 1.0f, float R = 1.0f, float G = 1.0f, float B = 1.0f );
	void	SendMsg( guiScrMsg msg );
	void	Print( float x, float y, std::string text, float ScaleX = 1.0f, float ScaleY = 1.0f );
	void	PrintConsole( const std::string& text );

	void	ProcessEvent( const CEvent& event ) override;

private:
	void	ParseKeys( const unsigned key, const bool down );
	void	ParseMouseMove( const int x, const int y );

};