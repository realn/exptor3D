/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Log.cpp
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Patrz -> Log.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "Log.h"
#include <wx/wx.h>
#include <wx/msgdlg.h>

Logger::Logger() :
	inited(false),
	firstTick(0)
{
}

Logger::~Logger(){
}

void Logger::Init( std::string filename, std::string str )
{
	fileName = filename;
	firstTick = GetTickCount();
	outStream.open(filename, std::ios::out | std::ios::trunc);

	SaveToFile( "======LOG START" + str + "======", firstTick );

	inited = true;
}

void Logger::SaveToFile( std::string str, unsigned int time )
{
	char temp[36];
	memset(temp, 0, sizeof(char) * 36);
	_itoa_s( time, temp, 36 * sizeof(char), 10 );

	outStream << "[" << temp << "]" << str << std::endl;
	outStream.flush();
}

void Logger::Log( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - firstTick;
	SaveToFile( str, time );
}

void Logger::Report( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - firstTick;
	SaveToFile( "[R]" + str, time );	
}

void Logger::Error( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - firstTick;
	SaveToFile( "[!]" + str, time );
}

void Logger::FatalError( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - firstTick;
	SaveToFile( "[!!!]" + str, time );
	wxMessageBox(str, _("FATAL ERROR"));
}

void Logger::Note( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - firstTick;
	SaveToFile( "[N]" + str, time );
}

Logger Log;