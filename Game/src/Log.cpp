/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Log.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> Log.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Log.h"

Logger::Logger()
{
	inited = false;
}

void Logger::Init( std::string filename, std::string str )
{
	FileName = filename;
	FirstTick = GetTickCount();
	FILE* fp = 0;
	fopen_s(&fp, FileName.c_str(), "wt" );
	fputs( "", fp );
	fclose( fp );
	SaveToFile( "======LOG START" + str + "======", FirstTick );
	inited = true;
}

void Logger::SaveToFile( std::string str, unsigned int time )
{
	char temp[36];
	_itoa_s( time, temp, 36 * sizeof(char), 10 );
	std::string save = "[";
	save += temp;
	save += "]" + str + "\n";

	FILE* fp = 0;
	fopen_s( &fp, FileName.c_str(), "at" );

	if( !fp )
	{
		MessageBox( NULL, "NIE MO¯NA ZAPISAÆ DO PLIKU!", "LOG - ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;
	}

	fputs( save.c_str(), fp );

	fclose( fp );
}

void Logger::Log( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - FirstTick;
	SaveToFile( str, time );
}

void Logger::Report( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - FirstTick;
	SaveToFile( "[R]" + str, time );	
}

void Logger::Error( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - FirstTick;
	SaveToFile( "[!]" + str, time );
}

void Logger::FatalError( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - FirstTick;
	SaveToFile( "[!!!]" + str, time );
	MessageBox( NULL, str.c_str(), "FATAL ERROR", MB_OK | MB_ICONEXCLAMATION );
}

void Logger::Note( std::string str )
{
	if( !inited )
		return;

	unsigned int time = GetTickCount() - FirstTick;
	SaveToFile( "[N]" + str, time );
}

Logger Log;