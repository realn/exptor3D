/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	gui.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Definicje klas do zarządzania programem oraz komunikacją
		z graczem. 

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include "gfx_Texture.h"
#include "gfx_TextureRepository.h"

#include "event_Observer.h"
#include "event_Mapper.h"

#include "Log.h"

#include "logic_ScriptParser.h"

#include "gui_MainMenu.h"
#include "gui_Screen.h"

#include "GUITextPrinter.h"
#include "GUIConsole.h"

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

class CGUIMain : public event::IObserver
{
private:
	event::Mapper eventMapper;
	gui::TextPrinter textPrinter;

	std::shared_ptr<gfx::Texture> CH;
	std::shared_ptr<gfx::Texture> Cursor;

	GUI_MODE	Mode;
	gui::MenuMain	menu;
	gui::Screen screen;
	CGUIConsole	Console;
	std::shared_ptr<logic::ScriptParser>	ScriptParser;

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

	glm::vec2 currentMousePos = glm::vec2(0.0f);

public:
	CGUIMain( gfx::TextureRepository& texManager, std::shared_ptr<logic::ScriptParser> scriptParser, float aspectRation, unsigned height );
	~CGUIMain() override = default;

	void	Update(float timeDelta);
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

	void	processEvent(const event::Event& event) override;

	gui::Screen& getScreen() { return screen; }

private:
};