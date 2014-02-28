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
#ifndef _GUI_H_
#define _GUI_H_

#include "defines.h"
#include "Log.h"
#include "Texture.h"
#include "Render.h"
#include <sstream>
#include <vector>
#include "Timer.h"

class guiTextureText
{
private:
	ioTexture *Tex;

	unsigned int base;
	unsigned int list;

	unsigned int Height;
	unsigned int Width;

	float C[4];

	bool loaded;
public:
	guiTextureText();
	~guiTextureText();

	void Init( ioTexture *font );
	void Free();
	void SetSize( unsigned int wid, unsigned int hei );
	void SetColor( float R, float G, float B, float Alpha = 1.0f );
	void StartPrint();
	void EndPrint();
	void Print( float x, float y, std::string text, float ScaleX = 1.0f, float ScaleY = 1.0f );
};

class guiIntro
{
private:
	ioTexture		*Tex;
	guiTextureText	TText;
	unsigned int Time;
	unsigned int TimePerChar;
	float Alpha;
	std::vector<std::string> Text;
	bool Started, Ended;
	int CurChar;
	int CurStr;
	std::vector<std::string> RenText;
public:
	guiIntro();
	~guiIntro();

	bool Init( ioTexture *Font );

	void AddStr( std::string str, int tpc );

	void Start();
	void DoEngine();
	void DoDraw();
	bool IsEnd();
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

class guiMenuItem
{
private:
	bool Enabled;
	bool Highlight;
	float scrollX;
	float scrollY;
public:
	std::string ID;
	std::string Caption;
	float X;
	float Y;
	float Color;
	float Alpha;
	unsigned int GoTo;
	std::string Action;

	guiMenuItem();

	void DoEngine();
	void DoDraw( guiTextureText* TText );

	void Enable();
	void Disable();

	void HighLight();
	void DontHighLight();

	void DoAction();
	void DoGoTo();
	
};

class guiMenu
{
private:
	float scroll;
	std::vector<guiMenuItem*> List;
	float x, y;
public:
	std::string ID;
	std::string MainCaption;
	float slide;

	guiMenu();
	~guiMenu();
	void Free();

	void DoEngine( float cX, float cY, bool click );
	void DoDraw( guiTextureText* TText );
	void AddMenuItem( guiMenuItem* item );
	void DeleteMenuItem( unsigned int index );
};


class guiMainMenu
{
private:
	guiTextureText* TText;
	std::vector<guiMenu*> List;

	bool Enabled;
	bool Clicked;
	bool WasClick;
	bool IsSliding;
	float curX;
	float curY;
	float slide;
	unsigned int CurMenu;
	unsigned int ToMenu;

	std::string file;

	void AddMenu( guiMenu* Menu );
	void DeleteMenu( unsigned int index );
	std::string GetStr( FILE* fp );
	std::string GetParamStr( std::string str );
	float GetParamFloat( std::string str );
	int GetMenuType( std::string str );
public:
	guiMainMenu();
	~guiMainMenu();

	void Init( guiTextureText* text );
	void Free();

	void Click( unsigned int X, unsigned int Y, bool click );
	void Cursor( unsigned int X, unsigned int Y );

	void GoToMenu( unsigned int index );

	void DoEngine();
	void DoDraw();

	void Enable();
	void Disable();
	bool IsEnabled();

	bool LoadMenu( std::string filename );
};

class guiMain
{
private:
	ioTexture font[1];
	ioTexture CH[1];
	guiTextureText TText;

	unsigned short MaxTexDLevel;
	unsigned short TexDetailLevel;
	unsigned short Second;
	unsigned short Minute;
	unsigned short Hour;

	float	TexPerWall;
	float	TexPerFloor;
	float	TexPerTop;
	float	Speed;
	float	RefLevel;
	float	ConsoleScroll;
	float	EyeAngle;
	float	FScrColor[4];
	float	ShowWLScrTime;
	float	ThisWLScrTime;
	float	WLScrColor[4];

	unsigned int	HandPos;
	unsigned int	LastTick;
	unsigned int	ThisTick;
	unsigned int	MiliSecPass;
	unsigned int	FrameRate;
	unsigned int	Frame;
	unsigned int	EngPerFrame;
	unsigned int	MBKeyFrames;

	int	MaxSpecial;

	bool	DoMainEngine;
	bool	DoMainDraw;
	bool	EPFMode;
	bool	MotionBlur;
	bool	Reflection;
	bool	WireFrame;
	bool	MainEngineEnabled;
	bool	ConsoleOn;
	bool	Cliping;
	bool	Quit;
	bool	DrawGGUI;
	bool	CanSmoke;
	bool	ShowAITarget;
	bool	CanShowLoseScr;
	bool	CanShowWinScr;

	std::vector< std::string >	ConsoleLMsg;
	std::string		ConsoleCMsg;
	std::vector< guiScrMsg >	ScrMsg;
	std::vector< std::string >	PFL;
	std::string WinText;
	std::string LoseText;

	void	(*LoadLevel)( std::string filename );

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
	ioTexture *Cursor;
	guiMainMenu Menu;
	guiPlayerInfo PInfo;
	std::string LevName;

	guiMain();
	void InitGUI();

	unsigned short	GetTexDLevel();
	void	SetTexDLevel( unsigned short level );
	void	UpTexDLevel();
	void	DownTexDLevel();
	float	GetTexPerPlane( unsigned int type );

	void	EnableMotionBlur();
	void	DisableMotionBlur();
	unsigned int	GetMBKeyFrames();
	void	SetMBKeyFrames( unsigned int keyf );
	bool	GetMotionBlur();

	bool	GetReflection();
	void	SetReflection( bool is );
	float	GetRefLevel();
	void	SetRefLevel( float set );

	void	SetCanSmoke( bool set );
	bool	GetCanSmoke();

	void	SetShowAITarget( bool set );
	bool	GetShowAITarget();

	void	ShowWinScr( float Time = 300.0f, std::string text = "" );
	void	ShowLoseScr( float Time = 300.0f, std::string text = "" );
	void	ResetWLScr();
	bool	IsShowingWLScr();

	void	SetMaxSpecial( int set );
	int		GetMaxSpecial();

	void	EnableCliping();
	void	DisableCliping();
	bool	GetCliping();

	void	EnableMainEngine();
	void	DisableMainEngine();

	bool	GetQuit();

	void	EnableGGUI();
	void	DisableGGUI();

	bool	CanDoMainEng();
	bool	CanDoMainDraw();
	unsigned int GetEPFTimes();

	void	ParseKeys( bool *Keys );
	void	ParseKey( char Key );
	unsigned int	GetHandPos();

	void	DoGUIEngine(const float fTD);
	void	DoGUIDraw();

	void	UpdateCounter();
	unsigned int	GetTime( unsigned int time );
	unsigned int	GetFrameRate();
	unsigned int	GetMiliSecPas();

	float	GetSpeed();
	void	SetSpeed( float speed );

	float	GetEyeAngle();
	void	SetEyeAngle( float set );

	void	ActiveFScrColor( float R, float G, float B, float Alpha = 0.5f );

	void	SendMsg( std::string msg, unsigned int Time = 2000, float X = -1.0f, float Y = -1.0f, float SclX = 1.0f, float SclY = 1.0f, float R = 1.0f, float G = 1.0f, float B = 1.0f );
	void	SendMsg( guiScrMsg msg );
	void	SendConMsg( std::string msg, bool parse = true, bool hist = true );
	void	Print( float x, float y, std::string text, float ScaleX = 1.0f, float ScaleY = 1.0f );

	void	SetLoadLevelFunc(  void (*func)( std::string str )  );
};

extern guiMain GUI;

extern std::string guiIntToStr(const long int x);
extern std::string guiFloatToStr(const float x);
extern std::string guiDoubleToStr(const long double x);
extern std::string guiBoolToStr(const bool x);
extern bool guiIsExtSupported( const std::string findExt );
extern bool guiIsInStr( const std::string where, const std::string what );

#endif