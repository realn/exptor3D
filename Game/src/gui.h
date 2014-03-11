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

#include "defines.h"
#include "Log.h"
#include "Texture.h"
#include "Render.h"
#include "Timer.h"

#include "TextureText.h"
#include "GUIMenu.h"
#include "ScriptParser.h"

#include "EventManager.h"

enum class GUI_MODE
{
	MENU,
	SCREEN,
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
	CTexture *font;
	CTexture *CH;
	CTextRenderer TText;
	CTexture *Cursor;

	GUI_MODE	Mode;

	unsigned short Second;
	unsigned short Minute;
	unsigned short Hour;

	float	ConsoleScroll;
	float	FScrColor[4];
	float	ShowWLScrTime;
	float	ThisWLScrTime;
	float	WLScrColor[4];

	unsigned int	LastTick;
	unsigned int	ThisTick;
	unsigned int	MiliSecPass;
	unsigned int	FrameRate;
	unsigned int	Frame;
	unsigned int	EngPerFrame;

	bool	DoMainEngine;
	bool	DoMainDraw;
	bool	EPFMode;
	bool	MainEngineEnabled;
	bool	ConsoleOn;
	bool	Quit;
	bool	DrawGGUI;
	bool	CanShowLoseScr;
	bool	CanShowWinScr;

	std::vector< std::string >	ConsoleLMsg;
	std::string		ConsoleCMsg;
	std::vector< guiScrMsg >	ScrMsg;
	std::vector< std::string >	PFL;
	std::string WinText;
	std::string LoseText;

	void	GetParams( std::string str, unsigned int from, std::string* param, unsigned int Count );
	void	ParseConMsg( std::string msg );
	void	ScrMsgEng( const float fTD );
	void	ScrMsgDraw();
	void	ConsoleEng( const float fTD );
	void	ConsoleDraw();
	void	ConExecute( bool hist = true );
	void	ConAddChar( char key );
	void	ConCreatePFL( bool AutoRep = true );
	void	DrawWLScr();
	void	EngineWLScr( const float fTD );

public:
	CMenuMain Menu;
	guiPlayerInfo PInfo;
	std::string LevName;
	CScriptParser	ScriptParser;

	CGUIMain();

	void InitGUI( CTexManager* texManager );

	void	ProcessEvent( const CEvent& event ) override;

	void	ParseKeys( const unsigned key, const bool down );
	void	ParseKey( char Key );

	void	Update(const float fTD);
	void	Render();

	void	ShowWinScr( float Time = 300.0f, std::string text = "" );
	void	ShowLoseScr( float Time = 300.0f, std::string text = "" );
	void	ResetWLScr();
	bool	IsShowingWLScr();

	void	EnableMainEngine();
	void	DisableMainEngine();

	bool	GetQuit();

	void	EnableGGUI();
	void	DisableGGUI();

	bool	CanDoMainEng();
	bool	CanDoMainDraw();
	unsigned int GetEPFTimes();

	void	UpdateCounter();
	unsigned int	GetTime( unsigned int time );
	unsigned int	GetFrameRate();
	unsigned int	GetMiliSecPas();

	void	ActiveFScrColor( float R, float G, float B, float Alpha = 0.5f );

	void	SendMsg( std::string msg, unsigned int Time = 2000, float X = -1.0f, float Y = -1.0f, float SclX = 1.0f, float SclY = 1.0f, float R = 1.0f, float G = 1.0f, float B = 1.0f );
	void	SendMsg( guiScrMsg msg );
	void	SendConMsg( std::string msg, bool parse = true, bool hist = true );
	void	Print( float x, float y, std::string text, float ScaleX = 1.0f, float ScaleY = 1.0f );
};

extern CGUIMain GUI;

extern bool guiIsExtSupported( const std::string findExt );
extern bool guiIsInStr( const std::string where, const std::string what );