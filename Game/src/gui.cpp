/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	gui.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> gio.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "gui.h"

guiMain GUI;

const std::string ConFunc[] = { "Speed", "MotionBlur", "MBKeyFrames", "Reflection",
								"RefLevel", "WireFrame", "Note", "SaveRndInfo",
								"SearchExt", "GetRndInfo", "GetExtList", "LoadLevel",
								"EyeAngle", "TexDLevel", "UpTexDLevel", "DownTexDLevel",
								"Cliping", "EnableEngine", "DisableEngine", "Quit",
								"EnableGUI", "DisableGUI", "EnableFullScreen", "DisableFullScreen",
								"Smoke", "MaxSpecial", "ShowAITarget" };
const int ConFuncCount = 27;

/*	KLASA guiMain
	To mia³a byæ tylko ma³a klasa poœrednicz¹ca pomiêdzy
	Graczem a Gr¹, a zrobi³ siê z niej g³ówny system
	steruj¹cy wszystkim. Tutaj dochodz¹ wszystkie metody,
	tutaj zarz¹dza siê wszystkim i tutaj zakodowana jest konsola.
	Wiem, wiem, mo¿na to by³o inaczej zrobiæ :/
*/
guiMain::guiMain()
{
	MaxTexDLevel = 2;
	TexDetailLevel = 2;
	MotionBlur = false;
	HandPos = GAME_RIGHT_HAND;
	FrameRate = 0;
	Frame = 0;
	Second = 0;
	Minute = 0;
	Hour = 0;
	ThisTick = GetTickCount();
	LastTick = ThisTick;
	MiliSecPass = 0;
	EngPerFrame = 0;
	PInfo.HEALTH = 100.0f;
	PInfo.ARMOR = 100.0f;
	PInfo.FRAGS = 0;
	PInfo.AMMO = 0;
	PInfo.CLIPS = 0;
	Speed = 1.0f;
	MBKeyFrames = 1;
	Reflection = false;
	RefLevel = 0.5f;
	MainEngineEnabled = false;
	ConsoleOn = false;
	ConsoleScroll = 0.0f;
	TexPerWall = 2.0f;
	TexPerTop = 4.0f;
	TexPerFloor = 1.0f;
	EyeAngle = 45.0f;
	Cliping = true;
	Quit = false;
	DrawGGUI = true;
}

void guiMain::ParseKeys( bool *Keys )
{
	static bool sickview = false;
	if( !Menu.IsEnabled() )
	{
		if( Keys[VK_F8] )
		{
			GUI.UpTexDLevel();
			Keys[VK_F8] = false;
		}
		if( Keys[VK_F7] )
		{
			GUI.DownTexDLevel();
			Keys[VK_F7] = false;
		}
		if( Keys[VK_F4] )
		{
			this->Reflection = !this->Reflection;
			Keys[VK_F4] = false;
		}
		if( Keys[VK_F5] )
		{
			if( !sickview )
			{
				this->SetMBKeyFrames( 4 );
				this->EnableMotionBlur();
				this->SetEyeAngle( 100.0f );
				this->SetSpeed( 0.4f );
			}
			else
			{
				this->DisableMotionBlur();
				this->SetEyeAngle( 60.0f );
				this->SetSpeed( 1.0f );
			}
			sickview = !sickview;
			Keys[VK_F5] = false;
		}
		if( Keys[VK_F6] )
		{
			if( Speed == 1.0f )
				Speed = 0.01f;
			else Speed = 1.0f;
			Keys[VK_F6] = false;
		}
		if( Keys[VK_ESCAPE] )
		{
			Menu.Enable();
			this->DisableGGUI();
			Keys[VK_ESCAPE] = false;
		}
	}
	else
	{
		POINT mpos;
		GetCursorPos( &mpos );
		Menu.Cursor( mpos.x, mpos.y );
		if( Keys[VK_LBUTTON] )
		{
			Menu.Click( mpos.x, mpos.y, true );
			Keys[VK_LBUTTON] = false;
		}
		if( Keys[VK_ESCAPE] )
		{
			Menu.Disable();
			this->EnableGGUI();
			Keys[VK_ESCAPE] = false;
		}
	}
	if( Keys[192] )
	{
		ConsoleOn = !ConsoleOn;
		Keys[192] = false;
	}
}

bool guiMain::GetQuit()
{
	return Quit;
}

void guiMain::SetTexDLevel( unsigned short level )
{
	if( level < MaxTexDLevel && level >= 0 )
		TexDetailLevel = level;
}

void guiMain::UpTexDLevel()
{
	if( TexDetailLevel < MaxTexDLevel )
		TexDetailLevel++;
}

void guiMain::DownTexDLevel()
{
	if( TexDetailLevel > 0 )
		TexDetailLevel--;
}

unsigned short guiMain::GetTexDLevel()
{
	return TexDetailLevel;
}

void guiMain::EnableMotionBlur()
{
	MotionBlur = true;
}

void guiMain::DisableMotionBlur()
{
	MotionBlur = false;
}

unsigned int guiMain::GetMBKeyFrames()
{
	return MBKeyFrames;
}

void guiMain::SetMBKeyFrames( unsigned int keyf )
{
	if( keyf > 0 )
		MBKeyFrames = keyf;
}

bool guiMain::GetMotionBlur()
{
	return MotionBlur;
}

/*	Tu zaczynaj¹ siê metody pokazuj¹ce napis WYGRA£EŒ oraz napis
	PRZEGRA£EŒ. Jest to zbiór zaledwie kilku funkcji, i wcale nie
	s¹ przera¿aj¹ce. Po pokazaniu siê którego kolwiek  z nich
	gracz automatycznie wraca do Menu.
*/
void guiMain::ShowWinScr( float Time, std::string text )
{
	if( !CanDoWLScr )
		return;
	WLScrColor[0] = 0.5f;
	WLScrColor[1] = 1.0f;
	WLScrColor[2] = 0.5f;
	WLScrColor[3] = 0.0f;

	WinText = text;
	ShowWLScrTime = Time;
	ThisWLScrTime = 0.0f;
	CanShowWinScr = true;
	CanShowLoseScr = false;
	CanDoWLScr = false;
}

void guiMain::ShowLoseScr( float Time, std::string text )
{
	if( !CanDoWLScr )
		return;
	WLScrColor[0] = 0.1f;
	WLScrColor[1] = 0.1f;
	WLScrColor[2] = 0.1f;
	WLScrColor[3] = 0.0f;

	LoseText = text;
	ShowWLScrTime = Time;
	ThisWLScrTime = 0.0f;
	CanShowWinScr = false;
	CanShowLoseScr = true;
	CanDoWLScr = false;
}

void guiMain::ResetWLScr()
{
	CanShowWinScr = false;
	CanShowLoseScr = false;
}

bool guiMain::IsShowingWLScr()
{
	if( !CanShowWinScr && !CanShowLoseScr )
		return false;
	return true;
}
void guiMain::DrawWLScr()
{
	if( !CanShowWinScr && !CanShowLoseScr )
		return;

	TText.StartPrint();
	//glEnable( GL_BLEND );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_TEXTURE_2D );
	glColor4fv( WLScrColor );
	glBegin( GL_TRIANGLE_STRIP );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, 600.0f, 0.0f );
		glVertex3f( 800.0f, 0.0f, 0.0f );
		glVertex3f( 800.0f, 600.0f, 0.0f );
	glEnd();
	glEnable( GL_TEXTURE_2D );
	//glDisable( GL_BLEND );
	
	if( CanShowWinScr )
	{
		TText.Print( ( 800.0f - ( 8.0f * ( 16.0f * 1.4f ) ) ) / 2.0f , 100.0f, "WYGRALES", 1.4, 1.4 );
		if( WinText != "" )
			TText.Print( ( 800.0f - ( float( WinText.length() ) * 16.0f ) ) / 2.0f, 200.0f, WinText );
	}
	if( CanShowLoseScr )
	{
		TText.Print( ( 800.0f - ( 8.0f * ( 16.0f * 1.4f ) ) ) / 2.0f, 100.0f, "PRZEGRALES", 1.4, 1.4 );
		if( LoseText != "" )
			TText.Print( ( 800.0f - ( float( LoseText.length() ) * 16.0f ) ) / 2.0f, 200.0f, LoseText );
	}
	TText.EndPrint();
}

void guiMain::EngineWLScr( const float fTD )
{
	if( !CanShowWinScr && !CanShowLoseScr )
		return;

	this->MainEngineEnabled = false;

	this->ThisWLScrTime += fTD * Speed;

	this->WLScrColor[3] = ( 1.0f / ShowWLScrTime ) * this->ThisWLScrTime;

	if( ThisWLScrTime >= ShowWLScrTime )
	{
		this->ResetWLScr();
		this->MainEngineEnabled = false;
		this->Menu.Enable();
	}
}

/*	3 metody zarz¹daj¹ce mo¿liwoœci¹ wykrywania koolizji.
*/
void guiMain::EnableCliping()
{
	Cliping = true;
}

void guiMain::DisableCliping()
{
	Cliping = false;
}

bool guiMain::GetCliping()
{
	return Cliping;
}


/*	Dwie nastêpne metody s³u¿¹ do zezwolenia
	Pokazywania celów przez AI.
*/
void guiMain::SetShowAITarget( bool set )
{
	ShowAITarget = set;
}

bool guiMain::GetShowAITarget()
{
	return ShowAITarget;
}

/*	4 nastêpne metody zarz¹dzaj¹ mo¿liwoœci¹ lustra w 
	pod³odze. 2 ostatnie okreœlaj¹ stopieñ odbicia:
	1 - idealny ( nie widaæ pod³ogi, tylko samo odbicie )
	0 - niewidoczny ( widaæ pod³oge, a nie odbicie :/ )
*/
bool guiMain::GetReflection()
{
	return Reflection;
}

void guiMain::SetReflection( bool is )
{
	Reflection = is;
}

float guiMain::GetRefLevel()
{
	return RefLevel;
}

void guiMain::SetRefLevel( float set )
{
	RefLevel = 1.0f - set;
}

/*	Teraz 4 metody okreœlaj¹c¹, czy sprite'y powinny byæ
	rysowany i w jakiej maksymalnej iloœci ( -1 by nie
	by³o limitu ).
*/
void guiMain::SetCanSmoke( bool set )
{
	CanSmoke = set;
}

bool guiMain::GetCanSmoke()
{
	return CanSmoke;
}

void guiMain::SetMaxSpecial( int set )
{
	MaxSpecial = set;
}

int guiMain::GetMaxSpecial()
{
	return MaxSpecial;
}

/*	Ta metoda okreœla funkcjê, która bêdzie wywo³ywana
	przy komendzie LoadLevel. A dlaczego wskaŸnik funkcji?
	Bo inaczej siê nie da³o :/. ( no dobra, mog³em wsadziæ zapowiedŸ
	funkcji na górze, ale to by³o by nieeleganckie )
*/
void guiMain::SetLoadLevelFunc( void (*func)( std::string str ) )
{
	LoadLevel = func;
}

/*	Ta funkca zwraca iloœæ tekstur na dan¹ powieszchnie ( czyli œciany, sufit, itp )
	Nie mo¿na jej zmieniæ w trakcie dzia³ania programu.
*/
float guiMain::GetTexPerPlane( unsigned int type )
{
	switch( type )
	{
	case GAME_TEX_WALL :
		return TexPerWall;
		break;
	case GAME_TEX_FLOOR :
		return TexPerFloor;
		break;
	case GAME_TEX_TOP :
		return TexPerTop;
		break;
	default:
		return TexPerWall;
	}
}

/*	Te dwie metody s³u¿¹ do trzymania kontroli nad g³ównymi
	funkcjami gry od Silnika i Renderer'a. Dziêki temu mo¿na
	prosto zarz¹daæ nimi.
*/
bool guiMain::CanDoMainDraw()
{
	return DoMainDraw;
}

bool guiMain::CanDoMainEng()
{
	return DoMainEngine;
}

/*	Raczej œmieszna metoda: pocz¹tkowo mia³ byc w grze wybór
	rêki, w której gracz trzyma d³oñ ( baa, s¹ nawet odpowiednie metody
	w broniach ), ale jakoœ nie chcia³o mi siê tego dokoñczyæ :/. W ka¿dym
	razie - metoda zwraca ID rêki, w której gracz trzyma broñ.
*/
unsigned int guiMain::GetHandPos()
{
	return HandPos;
}

/*	A tu siê pojawia ca³y Matrix ( Reloaded :) ). Wartoœæ Speed
	jest mno¿ona przez ka¿d¹ wartoœæ, jaka ma wp³yw na szybkoœæ gry. Gdy
	wynosi 1, to wszystko chodzi normalnie, a gdy jest inna to wszystko porusza
	siê wolniej, lub szybciej... :)
*/
void guiMain::SetSpeed( float speed )
{
	Speed = speed;
}

float guiMain::GetSpeed()
{
	return Speed;// * Timer->GetDT();
}

/*	Jakby to powiedzieæ - tu mo¿na uzyskaæ efekt rybiego oka, jak i 
	okular snajperki. Jest to k¹t widzenia okna. Normalny wynosi 60.
*/
float guiMain::GetEyeAngle()
{
	return EyeAngle;
}

void guiMain::SetEyeAngle( float set )
{
	EyeAngle = set;
}

/*	Dwie metody w³¹czaj¹ce i wy³¹czaj¹ce g³ówny silnik
	gry ( ale nie GUI )
*/
void guiMain::EnableMainEngine()
{
	MainEngineEnabled = true;
	//ConsoleOn = false;
	Menu.Disable();
}

void guiMain::DisableMainEngine()
{
	MainEngineEnabled = false;
	//ConsoleOn = true;
	Menu.Enable();
}

// W³¹cza i wy³¹cza GUI gracza
void guiMain::EnableGGUI()
{
	DrawGGUI = true;
}

void guiMain::DisableGGUI()
{
	DrawGGUI = false;
}

// Zwraca ile razy trzeba wykonaæ w³aœciwy silnik, by gra toczy³a siê w dobrym tempie
unsigned int guiMain::GetEPFTimes()
{
	if( FrameRate < 1 )
		FrameRate = 1;
	if( FrameRate < 60 )
		return 60 / FrameRate;
	else return 1;
}

/*	Pierwsza metoda wykonuje w³aœciwy silnik GUI, a druga renderuje GUI.
	W³aœciwego silnika nie da siê wy³¹czyæ
*/
void guiMain::DoGUIEngine(const float fTD)
{
	Timer->Update();

	if( !ConsoleOn && !Menu.IsEnabled() )
	{
		DoMainEngine = true;
	}
	else DoMainEngine = false;

	DoMainDraw = true;

	this->ScrMsgEng( fTD );
	this->ConsoleEng( fTD );
	this->Menu.Update();
	this->EngineWLScr( fTD );

	if( FScrColor[3] > 0.0f )
		FScrColor[3] -= 0.1f * fTD;
	else FScrColor[3] = 0.0f;
}

void guiMain::DoGUIDraw()
{
	if( WireFrame )
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	// Gdy g³ówny silnik dzia³a, rysujemy
	if( MainEngineEnabled && DrawGGUI )
	{
		// zaczynamy pisaæ
		TText.StartPrint();

		// Informacje po górnej lewej stronie ekranu
		TText.SetColor( 1.0f, 1.0f, 1.0f );
		TText.Print( 5.0f, 5.0f, "FPS: " + guiIntToStr( FrameRate ) );
		TText.Print( 5.0f, 25.0f, "CZAS: " + guiIntToStr( Hour ) + ":" + guiIntToStr( Minute ) + ":" + guiIntToStr( Second ) );
		TText.Print( 400.0f - (float)(LevName.length()*14 / 2), 5.0f, LevName );

		TText.SetColor( 0.8f, 1.0f, 0.2f );
		TText.Print( 600.0f, 5.0f, "PUNKTY: " + guiIntToStr( PInfo.FRAGS ), 1.3f, 1.3f );

		TText.SetColor( 1.0f, 0.3f, 0.3f );
		TText.Print( 5.0f, 570.0f, "Z: " + guiIntToStr( PInfo.HEALTH ), 1.5f, 1.5f );

		TText.SetColor( 0.0f, 1.0f, 1.0f );
		TText.Print( 150.0f, 570.0f, "P: " + guiIntToStr( PInfo.ARMOR ), 1.5f, 1.5f );

		TText.SetColor( 0.2f, 1.0f, 0.2f );
		TText.Print( 530.0f, 540.0f, "BRON: " + PInfo.WeapName, 1.3f, 1.3f );
		if( PInfo.AMMO != -1 )
			TText.Print( 500.0f, 570.0f, "AMUNICJA: " + guiIntToStr( PInfo.AMMO ), 1.3f, 1.3f );
		else
			TText.Print( 500.0f, 570.0f, "AMUNICJA: INF", 1.3f, 1.3f );

		if( PInfo.CLIPS != -1 )
			TText.Print( 710.0f, 570.0f, "/" + guiIntToStr( PInfo.CLIPS ), 1.3f, 1.3f );

		this->ScrMsgDraw();

		TText.EndPrint();

		GLRender.SetOrtho( -2.0f, 2.0f, -2.0f, 2.0f );
		CH[0].Activate( TexDetailLevel );
		{
			glBlendFunc( GL_ONE, GL_ONE );
			glEnable( GL_BLEND );
			glBegin( GL_TRIANGLE_STRIP );
			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f( -0.05f, 0.06f, 0.0f );
			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( -0.05f, -0.06f, 0.0f );
			glTexCoord2f( 0.0f, 1.0f );
			glVertex3f( 0.05f, 0.06f, 0.0f );
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( 0.05f, -0.06f, 0.0f );
			glEnd();
			if( FScrColor[3] > 0.0f )
			{
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				glDisable( GL_TEXTURE_2D );
				glColor4fv( FScrColor );
				glBegin( GL_TRIANGLE_STRIP );
					glVertex3f( -1.0f, 1.0f, 0.0f );
					glVertex3f( -1.0f, -1.0f, 0.0f );
					glVertex3f( 1.0f, 1.0f, 0.0f );
					glVertex3f( 1.0f, -1.0f, 0.0f );
				glEnd();
				glEnable( GL_TEXTURE_2D );
			}
			glDisable( GL_BLEND );
		}
	}

	if( this->IsShowingWLScr() )
		this->DrawWLScr();
	else
	{
		this->ConsoleDraw();
		this->Menu.Render();
	}

	if( WireFrame )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

// Wylicza czas, klatki na sekunde, minuty, godziny, milisekundy, itp.
void guiMain::UpdateCounter()
{
	unsigned int temp = GetTickCount();
	Frame++;
	MiliSecPass = temp - ThisTick;
	ThisTick = temp;

	if( ThisTick - LastTick > 1000 )
	{
		LastTick = ThisTick;
		FrameRate = Frame;
		Frame = 0;
		EngPerFrame = FrameRate / 60;
		if( EngPerFrame <= 0 )
			EngPerFrame = 1;

		Second++;
		if( Second >= 60 )
		{
			Second = 0;
			Minute++;
			if( Minute >= 60 )
			{
				Minute = 0;
				Hour++;
			}
		}

	}
}

// Dalsze trzy metody zwracaj¹ czas poziomu, klatki na sekunde i milisekundy
unsigned int guiMain::GetTime( unsigned int time )
{
	switch( time )
	{
	case GAME_SECOND :
		return Second;
	case GAME_MINUTE :
		return Minute;
	case GAME_HOUR :
		return Hour;
	default:
		return EngPerFrame;
	}
}
unsigned int guiMain::GetFrameRate()
{
	return FrameRate;
}

unsigned int guiMain::GetMiliSecPas()
{
	if( MiliSecPass <= 0 )
		return 1;
	else
		return MiliSecPass;
}

// Metoda inicjalizuje GUI
void guiMain::InitGUI()
{
	Timer = new CTimer();
	Timer->Init();
	Log.Log( "Inicjalizacja koñcowa GUI" );
	font[0].LoadTGA( "Data/Font.tga" );
	Cursor = TManager.Get( "Data/Kursor.tga" );
	TText.Init( &font[0] );
	CH[0].LoadTGA( "Data/cel.tga" );
	Menu.Init( &TText );
}

// Silnik widomoœci ekranowych wysy³anych przez silnik gry
void guiMain::ScrMsgEng( const float fTD )
{
	guiScrMsg* msg;
	for( int i = ScrMsg.size()-1; i >= 0; i-- )
	{
		msg = &ScrMsg[i];
		if( ThisTick < msg->ThisTime + msg->Time )
		{
			msg->Alpha = ( 2.0f / (float)msg->Time ) * (float)( ( msg->ThisTime + msg->Time ) - ThisTick );  
		}
		else
		{
			ScrMsg.erase( ScrMsg.begin() + i );
		}
	}
}

// Rendering owych wiadomoœci
void guiMain::ScrMsgDraw()
{
	guiScrMsg* msg;
	float x, y;
	int Cur = 0;
	for( unsigned int i = 0; i < ScrMsg.size(); i++ )
	{
		msg = &ScrMsg[i];
		TText.SetColor( msg->C[0], msg->C[1], msg->C[2], msg->Alpha );
		if( msg->x < 0.0f )
			x = 400 - ( ( ( 16.0f * msg->S[0] ) * (float)msg->msg.length() ) / 2.0f );
		else x = msg->x;
		if( msg->y < 0.0f )
			y = 540.0f - ( (float)Cur++ * ( 16.0f * msg->S[1] ) );
		else y = msg->y;

		TText.Print( x, y, msg->msg, msg->S[0], msg->S[1] );
	}
}


// Wys³anie wiadomoœci
void guiMain::SendMsg( std::string msg, unsigned int Time, float X, float Y, float SclX, float SclY, float R, float G, float B ) 
{
	guiScrMsg Msg;
	Msg.msg = msg;
	Msg.Time = Time;
	Msg.x = X;
	Msg.y = Y;
	Msg.C[0] = R;
	Msg.C[1] = G;
	Msg.C[2] = B;
	Msg.S[0] = SclX;
	Msg.S[1] = SclY;
	SendMsg( Msg );
}

void guiMain::SendMsg( guiScrMsg msg )
{
	msg.ThisTime = ThisTick;
	ScrMsg.push_back( msg );
}

// DO TESTÓW: metoda bezpoœrednio rysuje na ekranie tekst: NIE PRZEZNACZONA DO U¯YWANIA POZA DEBUG'iem
void guiMain::Print( float x, float y, std::string text, float ScaleX, float ScaleY )
{
	TText.StartPrint();
	TText.SetColor( 1.0f, 1.0f, 1.0f );
	TText.Print( x, y, text, ScaleX, ScaleY );
	TText.EndPrint();
}

// Tutaj aktywujemy "Full Screen Color", czyli np. Ca³y czerwony ekran od dostania kulk¹, zielony po podniesieniu apteczyki, itp.
// Naraz mo¿e byæ tylko jeden kolor ekranu
void guiMain::ActiveFScrColor( float R, float G, float B, float Alpha )
{
	FScrColor[0] = R;
	FScrColor[1] = G;
	FScrColor[2] = B;
	FScrColor[3] = Alpha;
}

// Metoda prywatna - pobiera argumenty polecenia wys³anego przez konsole
void guiMain::GetParams( std::string str, unsigned int from, std::string *param, unsigned int Count )
{
	if( Count <= 0 )
		return;

	unsigned int i, curparam = 0;
	for( i = from; i < str.length(); i++ )
	{
		if( str[i] == ' ' )
		{
			curparam++;
			if( curparam >= Count )
				break;
		}

		param[curparam] += str[i];
	}
}

// Metoda prywatna - parsuje ³añcuch znaków przes³any przez konsole
void guiMain::ParseConMsg( std::string msg )
{
	int i, j;
	std::string func;
	std::string param[5];
	/*	Najpierw wyodrêbniam same polecenie  */
	for( i = 0; i < msg.length(); i++ )
	{
		if( msg[i] == ' ' || msg[i] == '\n' || msg[i] == '\t')
			break;
		func += msg[i];
		j = i+2;
	}

	/*	Teraz sprawdzamy co to za funkcja, i wykonujemy
		odpowiedni¹ medode	
	*/
	if( func == "Speed" )
	{
		GetParams( msg, j, param, 1 );
		this->SendConMsg( "Szybkosc ustawiona na " + param[0], false );
		SetSpeed( atof( param[0].c_str() ) );
		return;
	}
	if( func == "MotionBlur" )
	{
		GetParams( msg, j, param, 1 );
		if( param[0] == "" )
			this->MotionBlur = !this->MotionBlur;
		else if( param[0] == "1" )
		{
			this->SendConMsg( "Motion Blur wlaczony", false );
			this->EnableMotionBlur();
		}
		else
		{
			this->SendConMsg( "Motion Blur wylaczony", false );
			this->DisableMotionBlur();
		}
		return;
	}
	if( func == "MBKeyFrames" )
	{
		GetParams( msg, j, param, 1 );
		this->SendConMsg( "MBKeyFrames ustawiony na " + param[0], false );
		this->SetMBKeyFrames( atoi( param[0].c_str() ) );
		return;
	}
	if( func == "Reflection" )
	{
		GetParams( msg, j, param, 1 );
		if( param[0] == "" )
			Reflection = !Reflection;
		else if( param[0] == "1" )
		{
			this->SendConMsg( "Odbicia wlaczone", false );
			this->SetReflection( true );
		}
		else
		{
			this->SendConMsg( "Odbicia wylaczone", false );
			this->SetReflection( false );
		}
		return;
	}
	if( func == "RefLevel" )
	{
		GetParams( msg, j, param, 1 );
		this->SendConMsg( "Stopien Odbicia ustawiony na " + param[0], false );
		SetRefLevel( atof( param[0].c_str() ) );
		return;
	}
	if( func == "WireFrame" )
	{
		GetParams( msg, j, param, 1 );
		if( param[0] == "" )
			WireFrame = !WireFrame;
		else if( param[0] == "1" )
		{
			this->SendConMsg( "Siatka widoczna", false );
			WireFrame = true;
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		else
		{
			this->SendConMsg( "Siatka niewidoczna", false );
			WireFrame = false;
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
		return;
	}
	if( func == "Note" )
	{
		GetParams( msg, j, param, 1 );
		this->SendConMsg( "Notatka zapisana", false );
		Log.Note( param[0] );
		return;
	}
	if( func == "SaveRndInfo" )
	{
		GetParams( msg, j, param, 1 );
		FILE* fp = 0;
		fopen_s(&fp, param[0].c_str(), "wt" );
		if( !fp )
		{
			this->SendConMsg( "Nieprawidlowa sciezka", false );
			return;
		}
		std::string save = GLRender.GetRndInfo( RENDER_RENDERER ) + "\n";
		fputs( save.c_str(), fp );
		save = GLRender.GetRndInfo( RENDER_VERSION ) + "\n";
		fputs( save.c_str(), fp );
		save = GLRender.GetRndInfo( RENDER_VENDOR ) + "\n";
		fputs( save.c_str(), fp );
		save = GLRender.GetRndInfo( RENDER_EXTLIST ) + "\n";
		fputs( save.c_str(), fp );
		this->SendConMsg( "Informacje zapisane...", false );
		Log.Report( "Zapis inforamcji do pliku: " + param[0] );
		fclose( fp );
		return;
	}
	if( func == "SearchExt" )
	{
		GetParams( msg, j, param, 1 );
		if( guiIsInStr( GLRender.GetRndInfo( RENDER_EXTLIST ), param[0] ) )
		{
			this->SendConMsg( "Rozszerzenie " + param[0] + " jest obslugiwane", false );
		}
		else
		{
			this->SendConMsg( "Rozszerzenie " + param[0] + " nie jest obslugiwane", false );
		}
		return;
	}
	if( func == "GetRndInfo" )
	{
		this->SendConMsg( "Wersja OpenGL: " + GLRender.GetRndInfo( RENDER_VERSION ), false );
		this->SendConMsg( "Sterownik OpenGL: " + GLRender.GetRndInfo( RENDER_RENDERER ), false );
		this->SendConMsg( "Dostawca OpenGL: " + GLRender.GetRndInfo( RENDER_VENDOR ), false );
		return;
	}
	if( func == "GetExtList" )
	{
		int i = 0, j = 0;
		std::string tostr;
		for( i = 0; i < GLRender.GetRndInfo( RENDER_EXTLIST ).length(); i++ )
		{
			tostr += GLRender.GetRndInfo( RENDER_EXTLIST )[i];
			if( GLRender.GetRndInfo( RENDER_EXTLIST )[i] == ' ' )
			{
				if( j >= 2 )
				{
					j = 0;
					this->SendConMsg( tostr, false );
					tostr = "";
				}
				else j++;
			}
		}
		return;
	}
	if( func == "LoadLevel" )
	{
		GetParams( msg, j, param, 1 );
		bool temp = MainEngineEnabled;
		this->DisableMainEngine();
		LoadLevel( param[0] );
		if( temp )
			this->EnableMainEngine();
		return;
	}
	if( func == "EyeAngle" )
	{
		GetParams( msg, j, param, 1 );
		this->SendConMsg( "Kat oka ustawiony na " + param[0], false );
		SetEyeAngle( atof( param[0].c_str() ) );
		return;
	}
	if( func == "TexDLevel" )
	{
		GetParams( msg, j, param, 1 );
		this->SendConMsg( "Poziom detali textur ustawiony na " + param[0], false );
		this->SetTexDLevel( atoi( param[0].c_str() ) );
		return;
	}
	if( func == "UpTexDLevel" )
	{
		this->UpTexDLevel();
		this->SendConMsg( "Poziom deteli textur zwiekszony do " + guiIntToStr( GUI.GetTexDLevel() ), false );
		return;
	}
	if( func == "DownTexDLevel" )
	{
		this->DownTexDLevel();
		this->SendConMsg( "Poziom deteli textur zmniejszony do " + guiIntToStr( GUI.GetTexDLevel() ), false );
		return;
	}
	if( func == "Cliping" )
	{
		GetParams( msg, j, param, 1 );
		if( param[0] == "1" )
		{
			this->SendConMsg( "Kolizje wlaczone", false );
			this->EnableCliping();
		}
		else
		{
			this->SendConMsg( "Kolizje wylaczone", false );
			this->DisableCliping();
		}
		return;
	}
	if( func == "EnableEngine" )
	{
		this->SendConMsg( "Glowny silnik wlaczony", false );
		this->EnableMainEngine();
		return;
	}
	if( func == "DisableEngine" )
	{
		this->SendConMsg( "Glowny silnik wylaczony", false );
		this->DisableMainEngine();
		return;
	}
	if( func == "Quit" )
	{
		Quit = true;
		return;
	}
	if( func == "EnableGUI" )
	{
		DrawGGUI = true;
		return;
	}
	if( func == "DisableGUI" )
	{
		DrawGGUI = false;
		return;
	}
	if( func == "EnableFullScreen" )
	{
		GLRender.EnableFullScreen();
		return;
	}
	if( func == "DisableFullScreen" )
	{
		GLRender.DisableFullScreen();
		return;
	}
	if( func == "Smoke" )
	{
		GetParams( msg, j, param, 1 );
		if( param[0] == "" )
			CanSmoke = !CanSmoke;
		else if( param[0] == "1" )
		{
			this->SendConMsg( "Sprite'y w³¹czone", false );
			CanSmoke = true;
		}
		else
		{
			this->SendConMsg( "Sprite'y wy³¹czone", false );
			CanSmoke = false;
		}
		return;
	}
	if( func == "MaxSpecial" )
	{
		GetParams( msg, j, param, 1 );
		this->SendConMsg( "Maksimum efektów ustawiony na " + param[0], false );
		this->SetMaxSpecial( atoi( param[0].c_str() ) );
		return;
	}
	if( func == "ShowAITarget" )
	{
		GetParams( msg, j, param, 1 );
		if( param[0] == "" )
			ShowAITarget = !ShowAITarget;
		else if( param[0] == "1" )
		{
			this->SendConMsg( "Cele AI widoczne", false );
			ShowAITarget = true;
		}
		else
		{
			this->SendConMsg( "Cele AI niewidoczne", false );
			ShowAITarget = false;
		}
		return;
	}
	this->SendConMsg( "Bledne polecenie...", false );
}

// Wysy³a wiadomoœæ do konsoli, mo¿e byæ przeparsowana, lub dodana do listy
void guiMain::SendConMsg( std::string msg, bool parse, bool hist )
{
	if( parse )
	{
		ConsoleCMsg = msg;
		ConExecute( hist );
	}
	else
	{
		ConsoleLMsg.push_back( msg );
	}

	if( !MainEngineEnabled && !Menu.IsEnabled() )
	{
		GLRender.SetOrtho();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		//this->ConsoleEng();
		this->ConsoleDraw();
		GLRender.SwapBuffers();
	}
}

// Wykonanie komendy aktualnie wpisanej przez u¿ytkownika
void guiMain::ConExecute( bool hist )
{
	if( hist )
		ConsoleLMsg.push_back( ConsoleCMsg );
	ParseConMsg( ConsoleCMsg );
	ConsoleCMsg = "";
}

// Dodanie jednej litery do konsoli
void guiMain::ConAddChar( char Key )
{
	this->ConsoleCMsg += Key;
	this->ConCreatePFL();
}

// Przeparsowanie przez GUI wciœniêtego klawisza
void guiMain::ParseKey( char Key )
{
	if( MainEngineEnabled )
	{
		if( !ConsoleOn )
			return;
	}

	if( Key == VK_BACK )
	{
		if( ConsoleCMsg.length() != 0 )
			ConsoleCMsg = ConsoleCMsg.substr( 0, ConsoleCMsg.length()-1 );
		this->ConCreatePFL( false );
		return;
	}
	if( Key == VK_RETURN )
	{
		this->ConExecute();
		this->ConCreatePFL( false );
		return;
	}
	if( Key == 192 || Key == '`' )
		return;

	unsigned short key = 0;
	unsigned char KeybState[256] = { 0 };
	
	GetKeyboardState( KeybState );
	if( ToAscii( Key, MapVirtualKey( Key, 0 ), KeybState, &key, 0 ) == 1 )
	{
		this->ConAddChar( key );
	}
}

// Stwa¿a "Liste Prawdopodobnych Funkcji" ( "Possible Function List" ) do konsoli
void guiMain::ConCreatePFL( bool AutoRep )
{
	PFL.clear();

	if( ConsoleCMsg == "" )
		return;

	for( int i = 0; i < ConFuncCount; i++ )
	{
		if( guiIsInStr( ConFunc[i], ConsoleCMsg ) )
		{
			PFL.push_back( ConFunc[i] );
		}
	}
	if( PFL.size() == 1 && AutoRep )
	{
		ConsoleCMsg = PFL[0] + " ";
		PFL.clear();
	}
}

// Silnik w³aœciwy konsoli
void	guiMain::ConsoleEng( const float fTD )
{
	if( ConsoleOn )
	{
		if( this->ConsoleScroll < 1.0f )
			ConsoleScroll += 0.1f * fTD;
		else ConsoleScroll = 1.0f;
	}
	else
	{
		if( this->ConsoleScroll > 0.0f )
			ConsoleScroll -= 0.1f * fTD;
		else ConsoleScroll = 0.0f;
	}
}

// Rendering konsoli
void guiMain::ConsoleDraw()
{
	if( MainEngineEnabled && !ConsoleOn && ConsoleScroll == 0.0f && Menu.IsEnabled() )
		return;

	int i = 0;
	float PrintFrom = 0.0f, y;

	glPushMatrix();
	GLRender.SetOrtho();
	glDisable( GL_TEXTURE_2D );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	if( MainEngineEnabled || Menu.IsEnabled() )
	{

		glColor4f( 0.0f, 0.0f, 0.0f, 0.5f );
		glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -1.0f, 2.0f - ConsoleScroll, 0.0f );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -1.0f, 1.0f - ConsoleScroll, 0.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 2.0f - ConsoleScroll, 0.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, 1.0f - ConsoleScroll, 0.0f );
		glEnd();


		PrintFrom = 300.0f;
		y = PrintFrom * ConsoleScroll;
	}
	else
	{
		glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
		glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -1.0f, 1.0f, -1.0f );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -1.0f, -1.0f, -1.0f );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 1.0f, -1.0f );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, -1.0f, -1.0f );
		glEnd();

		PrintFrom = 600.0f;
		y = PrintFrom;
	}

	glColor3f( 1.0f, 1.0f, 1.0f );
	glDisable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	TText.StartPrint();

	TText.SetColor( 1.0f, 1.0f, 1.0f );
	for( i = ConsoleLMsg.size()-1; i >= 0; i-- )
	{
		float fromy = y - 16.0f - (float)(16.0f * ( ConsoleLMsg.size()-i ));
		if( fromy < 0.0f )
			continue;
		TText.Print( 5.0f, fromy, "/" + ConsoleLMsg[i] );
	}
	TText.Print( 5.0f, y - 16.0f, ">" + this->ConsoleCMsg );

	if( PFL.size() && MainEngineEnabled )
	{
		float x = 0.0f;
		for( i = 0; i < PFL.size(); i++ )
		{
			if( x < (float)PFL[i].length() )
				x = (float)PFL[i].length();

			TText.Print( 20.0f, PrintFrom + ( (float)i * 16.0f ), PFL[i] );
		}
	}
	TText.EndPrint();

	glPopMatrix();
}



/*======================
	KLASA guiMenuItem
	Jest to klasa reprezentuj¹ca
	wybór w menu.
======================*/
guiMenuItem::guiMenuItem()
{
	Enabled = true;
	Highlight = false;
	Action = "";
	GoTo = -1;
	Color = 0.0f;
	scrollX = 0.0f;
	scrollY = 0.0f;
}

void guiMenuItem::Enable()
{
	Enabled = true;
}

void guiMenuItem::Disable()
{
	Enabled = false;
}

void guiMenuItem::HighLight()
{
	Highlight = true;
}

void guiMenuItem::DontHighLight()
{
	Highlight = false;
}

void guiMenuItem::Update()
{
	if( !Highlight )
	{
		if( Color > 0.0f )
			Color -= 0.01f;
		
		if( scrollX > 0.0f )
			scrollX -= 0.1f;
	}
	else
	{
		if( Color < 0.6f )
			Color += 0.01f;

		if( scrollX < 10.0f )
			scrollX += 0.1f;
	}
}

void guiMenuItem::Render( guiTextureText *TText )
{
	if( Enabled )
		TText->SetColor( 0.4f + Color, 0.4f + Color, 0.4f + Color );
	else TText->SetColor( 0.2f, 0.2f, 0.2f, 0.5f );
	TText->Print( X + scrollX, Y, this->Caption, 1.5f, 1.5f );
}

void guiMenuItem::DoAction()
{
	GUI.SendConMsg( Action, true, false );
}

void guiMenuItem::DoGoTo()
{
	GUI.Menu.GoToMenu( GoTo );
}


/*======================
	KLASA guiMenu
	Klasa reprezentuj¹ca pojedyñcze menu.
	Zarz¹dza jednoczeœnie swoimi przyciskami
======================*/
guiMenu::guiMenu()
{
	
}

guiMenu::~guiMenu()
{
	Free();
}

void guiMenu::Update( float cX, float cY, bool click )
{
	x=cX;
	y=cY;
	guiMenuItem* Item;
	for( int i = 0; i < List.size(); i++ )
	{
		Item = List[i];

		float itemL = Item->X;
		float itemR = Item->X + (float)Item->Caption.length() * ( 16.0f * 1.5f );
		float itemT = Item->Y;
		float itemB = Item->Y + 16.0f * 1.5f;

		Item->DontHighLight();
		if( cX >= itemL && cX <= itemR )
		{
			if( cY >= itemT && cY <= itemB )
			{
				Item->HighLight();
				if( click )
				{
					if( Item->Action != "" )
						Item->DoAction();

					if( Item->GoTo != -1 )
						Item->DoGoTo();
				}
			}
		}
		else Item->DontHighLight();

		Item->Update();
	}
}

void guiMenu::Render( guiTextureText *TText )
{
	TText->StartPrint();
	GUI.Cursor->Activate();
	glDisable( GL_CULL_FACE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBegin( GL_TRIANGLE_STRIP );
	glNormal3f( 0.0f, 0.0f, 1.0f );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( x, y+50.0f, -0.1f );
	glTexCoord2f( 0.0f, 1.0f );
	glVertex3f( x, y, -0.1f );
	glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( 50.0f+x, y+50.0f, -0.1f );
	glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( 50.0f+x, y, -0.1f );
	glEnd();

	TText->EndPrint();
	TText->StartPrint();
	glTranslatef( slide, 0.0f, 0.0f );
	TText->SetColor( 1.0f, 1.0f, 1.0f );
	TText->Print( 10.0f, 20.0f, this->MainCaption, 2.5f, 2.5f );
	guiMenuItem* Item;
	for( int i = 0; i < List.size(); i++ )
	{
		Item = List[i];
		Item->Render( TText );
	}
	TText->EndPrint();
}

void guiMenu::AddMenuItem( guiMenuItem* item )
{
	List.push_back( item );
}

void guiMenu::DeleteMenuItem( unsigned int index )
{
	delete List[index];
	List.erase( List.begin() + index );
}

void guiMenu::Free()
{
	for( int i = List.size()-1; i >= 0; i-- )
	{
		DeleteMenuItem( i );
	}
}


/*===========================
	KLASA guiMainMenu
	Steruje g³ównym menu gry.
===========================*/
guiMainMenu::guiMainMenu()
{
	Enabled = false;
	Clicked = false;
	IsSliding = false;
	file = "-";
	CurMenu = 0;
	ToMenu = 0;
	slide = 0.0f;
}

guiMainMenu::~guiMainMenu()
{
	Free();
}

void guiMainMenu::Init( guiTextureText *text )
{
	TText = text;
	curX = 0.0f;
	curY = 0.0f;
}

void guiMainMenu::Update()
{
	if( !Enabled )
		return;

	if( List.size() <= 0 || CurMenu >= List.size() || CurMenu < 0 )
		return;

	if( IsSliding )
	{
		if( slide < 800.0f )
			slide += 10.0f;
		else
		{
			CurMenu = ToMenu;
			IsSliding = false;
			slide = 800.0f;
		}
	}
	else
	{
		if( slide > 0.0f )
			slide -= 10.0f;
		else slide = 0.0f;
	}

	guiMenu* Menu;
	Menu = List[CurMenu];
	Menu->slide = slide;

	Menu->Update( curX, curY, Clicked );
	Clicked = false;
}

void guiMainMenu::Render()
{
	if( !Enabled )
		return;

	if( List.size() <= 0 || CurMenu >= List.size() || CurMenu < 0 )
		return;

	guiMenu* Menu;
	Menu = List[CurMenu];

	Menu->Render( TText );
}

void guiMainMenu::Click( unsigned int X, unsigned int Y, bool click )
{
	float ax = 800.0f / (float)GLRender.GetWidth();
	float ay = 600.0f / (float)GLRender.GetHeight();

	curX = ax * (float)X;
	curY = ay * (float)Y;

	Clicked = click;
}

void guiMainMenu::Cursor( unsigned int X, unsigned int Y )
{
	float ax = 800.0f / (float)GLRender.GetWidth();
	float ay = 600.0f / (float)GLRender.GetHeight();

	curX = ax * (float)X;
	curY = ay * (float)Y;
}

void guiMainMenu::GoToMenu( unsigned int index )
{
	if( List.size() <= 0 || CurMenu >= List.size() || CurMenu < 0 )
		return;

	ToMenu = index;
	IsSliding = true;
}

void guiMainMenu::Disable()
{
	Enabled = false;
	CurMenu = 0;
}

void guiMainMenu::Enable()
{
	Enabled = true;
}

bool guiMainMenu::IsEnabled()
{
	return Enabled;
}

void guiMainMenu::AddMenu( guiMenu *Menu )
{
	List.push_back( Menu );
}

void guiMainMenu::DeleteMenu( unsigned int index )
{
	delete List[index];
	List.erase( List.begin() + index );
}

void guiMainMenu::Free()
{
	for( int i = List.size()-1; i >= 0; i-- )
	{
		DeleteMenu( i );
	}
}

std::string guiMainMenu::GetStr( FILE* fp )
{
	char buf[512];
	do fgets( buf, 512, fp );
	while( buf[0] == '\n' );
	return buf;
}

std::string guiMainMenu::GetParamStr( std::string str )
{
	int i;
	std::string zwrot = "";

	for( i = 0; i < str.length(); i++ )
	{
		if( str[i] == '=' )
			break;
	}

	if( str[i] != '=' )
	{
		Log.Error( "MAINMENU( " + file + " ): B³¹d: nie mo¿na znaleœæ znaku = w ³añcuchu: " + str );
		return "ERROR";
	}

	for( i++; i < str.length(); i++ )
	{
		if( str[i] == '\n' )
			continue;

		zwrot += str[i];
	}

	return zwrot;
}

float guiMainMenu::GetParamFloat( std::string str )
{
	return atof( GetParamStr( str ).c_str() );
}

int guiMainMenu::GetMenuType( std::string str )
{
	int i;
	std::string temp;
	for( i = 0; i < str.length(); i++ )
	{
		if( str[i] == '=' )
			break;

		temp += str[i];
	}

	if( str[i] != '=' )
	{
		Log.Error( "MAINMENU( " + file + " ): B³¹d: nie mo¿na znaleœæ znaku = w ³añcuchu: " + str );
		return MENU_ERROR;
	}

	if( temp == "MENU" )
		return MENU_NAME;
	if( temp == "MENUITEM" )
		return MENU_ITEM_NAME;
	if( temp == "MENUITEMPOSX" )
		return MENU_ITEM_POSX;
	if( temp == "MENUITEMPOSY" )
		return MENU_ITEM_POSY;
	if( temp == "MENUITEMACTION" )
		return MENU_ITEM_ACTION;
	if( temp == "MENUITEMGOTO" )
		return MENU_ITEM_GOTO;

	return MENU_ERROR;
}

bool guiMainMenu::LoadMenu( std::string filename )
{
	if( filename == "" )
	{
		Log.Error( "MAINMENU( " + file + " ): Pusty ci¹g znaków w œcie¿ce do pliku" );
		return false;
	}

	// Uchwyt do pliku
	FILE* fp = 0;

	fopen_s(&fp, filename.c_str(), "rt" );

	// sprawdzamy uchwyt
	if( !fp )
	{
		Log.Error( "MAINMENU( " + file + " ): B³êdna œcie¿ka" );
		return false;
	}

	this->Free();

	file = filename;
	std::string str = "";
	guiMenu* Menu = NULL;
	guiMenuItem* Item = NULL;

	while( !feof( fp )  )
	{
		str = GetStr( fp );

		switch( GetMenuType( str ) )
		{
		case MENU_NAME :
			Item = NULL;
			Menu = new guiMenu;
			this->AddMenu( Menu );
			Menu->MainCaption = GetParamStr( str );
			break;

		case MENU_ITEM_NAME :
			if( Menu == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma menu, by dodaæ pozycje, ³añcuch: " + str );
				break;
			}
			Item = new guiMenuItem;
			Menu->AddMenuItem( Item );
			Item->Caption = GetParamStr( str );
			break;

		case MENU_ITEM_POSX :
			if( Item == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
				break;
			}
			Item->X = GetParamFloat( str );
			break;

		case MENU_ITEM_POSY :
			if( Item == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
				break;
			}
			Item->Y = GetParamFloat( str );
			break;

		case MENU_ITEM_ACTION :
			if( Item == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
				break;
			}
			Item->Action = GetParamStr( str );
			break;

		case MENU_ITEM_GOTO :
			if( Item == NULL )
			{
				Log.Error( "MAINMENU( " + file + " ): B³¹d: nie ma pozycji, by dodaæ w³aœciwoœæ, ³añcuch: " + str );
				break;
			}
			Item->GoTo = (int)GetParamFloat( str );
			break;

		default:
		case MENU_ERROR :
			Log.Error( "MAINMENU( " + file + " ): Nierozpoznano typu: " + str );
			break;
		}
	}

	fclose( fp );

	return true;
}

/*========================
	KLASA guiTextureText
	Jest odpowiedzialna za rysowanie tekstów na ekranie.
=========================*/
guiTextureText::guiTextureText()
{
	loaded = false;
	Tex = NULL;
	Width = 800;
	Height = 600;
	C[0] = 1.0f;
	C[1] = 1.0f;
	C[2] = 1.0f;
}

guiTextureText::~guiTextureText()
{
	Free();
}

void guiTextureText::Init( CTexture* font )
{
	if( !font )
		return;
	
	Tex = font;

	base = glGenLists(256);	// Creating 256 display lists
	
	Tex->Activate();

	for (int loop1=0; loop1 < 256; loop1++)			// Loop through all 256 lists
	{
		float cx = float( loop1%16 ) / 16.0f;		// X position of current character
		float cy = float( loop1/16 ) / 16.0f;		// Y position of current character

		glNewList(base+loop1,GL_COMPILE);	// Start building a list
			glBegin(GL_QUADS);					// Use a quad for each character
				glTexCoord2f( cx, 1.0f - cy - 0.0625f );		// Texture coord (Bottom left)
				glVertex2d( 0, 16 );				// Vertex coord (Bottom left)
				glTexCoord2f( cx + 0.0625f, 1.0f - cy - 0.0625f );	// Texture coord (Bottom right)
				glVertex2i( 16, 16);				// Vertex coord (Bottom right)
				glTexCoord2f( cx + 0.0625f, 1.0f - cy - 0.001f );	// Texture coord (Top right)
				glVertex2i( 16, 0 );				// Vertex coord (Top right)
				glTexCoord2f( cx, 1.0f - cy - 0.001f );		// Texture coord (Top left)
				glVertex2i( 0, 0 );				// Vertex coord (Top left)
			glEnd();			// Done building our quad (Character)
			glTranslated( 14, 0, 0);		// Move to the right of the character
		glEndList();				// Done building the display list
	}	

	loaded = true;
}

void guiTextureText::Free()
{
	if( !loaded )
		return;

	glDeleteLists( base, 256 );
	glDeleteLists( list, 2 );
	loaded = false;
}

void guiTextureText::StartPrint()
{
	glLoadIdentity();

	glListBase( base - 32 );

	glPushAttrib( GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
	glPushMatrix();

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glDisable( GL_LIGHTING );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	Tex->Activate();

	glMatrixMode(GL_PROJECTION);				
	glLoadIdentity();		
	glOrtho( 0, (double)Width, (double)Height, 0, -1.0, 1.0 );
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}

void guiTextureText::Print( float x, float y, std::string text, float ScaleX, float ScaleY )
{
	char stext[255];
	strcpy_s( stext, 255 * sizeof(char), text.c_str() );
	glPushMatrix();
	glTranslatef( x, y, 0 );
	glScalef( ScaleX, ScaleY, 1.0f );
	glColor4f( C[0], C[1], C[2], C[3] );
	glPushMatrix();
	glCallLists( strlen(stext), GL_UNSIGNED_BYTE, stext );
	glPopMatrix();
	glCallLists( strlen(stext), GL_UNSIGNED_BYTE, stext );
	glPopMatrix();
}

void guiTextureText::EndPrint()
{
	glPopMatrix();
	glPopAttrib();

	glColor3f( 1.0f, 1.0f ,1.0f );
}

void guiTextureText::SetSize( unsigned int wid, unsigned int hei )
{
	Width = wid;
	Height = hei;
}

void guiTextureText::SetColor( float R, float G, float B, float Alpha )
{
	C[0] = R;
	C[1] = G;
	C[2] = B;
	C[3] = Alpha;
}

/*==========================
	KLASA guiIntro - >>PRACE WSTRZYMANE<<
	Jest odpowiedzialna za filmik wprowadzaj¹cy do gry
==========================*/
guiIntro::guiIntro()
{
	Time = 0;
	TimePerChar = 0;
	Started = false;
	Ended = true;
	Text.clear();
	RenText.clear();
}

guiIntro::~guiIntro()
{

}

bool guiIntro::Init( CTexture* Font )
{
	Tex = Font;
	TText.Init( Tex );
	return true;
}

void guiIntro::AddStr( std::string str, int tpc )
{
	Text.push_back( str );
	TimePerChar = tpc;
}

void guiIntro::Start()
{
	Started = true;
	Ended = false;
	CurChar = 0;
	CurStr = -1;
	//std::string str;
	//RenText.push_back( str );
	Alpha = 1.0f;
}

void guiIntro::Update()
{
	if( !Started )
		return;

	unsigned int LastTime = GetTickCount();

	if( LastTime - Time > TimePerChar )
	{
		Time = LastTime;
		if( RenText.size() > 0 && CurChar < Text[CurStr].length() )
		{
			RenText[CurStr] += Text[CurStr][CurChar];
			CurChar++;
		}
		else
		{
			if( ( CurStr + 1) < Text.size() )
			{
				CurStr++;
				CurChar = 0;
				std::string str;
				RenText.push_back( str );
			}
			else
			{
				if( Alpha > 0.0f )
					Alpha -= 0.04f;
				else
					Ended = true;
			}
		}
	}
}

void guiIntro::Render()
{
	if( Ended )
		return;
	
	glPushMatrix();
	glPushAttrib( GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );

	glMatrixMode(GL_PROJECTION);				
	glLoadIdentity();		
	glOrtho( -1, 1, -1, 1, -1.0, 1.0 );
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	//glClear( GL_DEPTH_BUFFER_BIT);	//Czyszczenie buforów
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_2D );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );

	glBegin( GL_QUADS );
	glColor4f( 0.0f, 0.0f, 0.0f, Alpha );
	glVertex3f( 1.0f, 1.0f, -0.5f );
	glVertex3f( -1.0f, 1.0f, -0.5f );
	glVertex3f( -1.0f, -1.0f, -0.5f );
	glVertex3f( 1.0f, -1.0f, -0.5f );
	glEnd();

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	for( int i = 0; i < RenText.size(); i++ )
	{
		float w = 420.0f - ( (float)Text[i].length() * 16.0f ) / 2.0f;
		float h = 300.0f - ( (float)Text.size() * 18.0f ) / 2.0f + (float)i * 18.0f;
		TText.Print( w, h, RenText[i] );
	}

	glPopMatrix();
	glPopAttrib();
}

bool guiIntro::IsEnd()
{
	return Ended;
}


/*============================
	FUNKCJE POMOCNICZE
	G³ównie funkcje wspieraj¹ce edycje i przeszukiwanie tekstu.
=============================*/
std::string guiIntToStr(const long int x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

std::string guiFloatToStr(const float x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

std::string guiDoubleToStr(const long double x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

std::string guiBoolToStr(const bool x) 
{
	std::string temp;
	if(x) temp = "TRUE";
	else temp = "FALSE";
	return temp;
}

bool guiIsExtSupported( const std::string findExt )
{
	const unsigned char *ExtList = NULL;
	ExtList = glGetString( GL_EXTENSIONS );

	for( int i = 0; i < strlen((const char*)ExtList) - findExt.length(); i++ )
	{
		if( ExtList[i] == findExt[0] )
		{
			bool ok = true;
			for( int j = 0; j < findExt.length(); j++ )
			{
				if( ExtList[i+j] == findExt[j] )
					ok = true;
				else
				{
					ok = false;
					break;
				}
			}
			if( ok ) return true;
		}
	}
	return false;
}

bool guiIsInStr( const std::string where, const std::string what )
{
	for( int i = 0; i < where.length(); i++ )
	{
		if( where[i] == what[0] )
		{
			bool ok = true;
			for( int j = 0; j < what.length(); j++ )
			{
				if( where[i+j] == what[j] )
					ok = true;
				else
				{
					ok = false;
					break;
				}
			}
			if( ok ) return true;
		}
	}
	return false;
}
