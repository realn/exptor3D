/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Log.cpp
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Patrz -> Log.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "Log.h"
#include <chrono>

namespace detail {
	constexpr auto getTimeNow() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
	}
}

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
	firstTick = detail::getTimeNow();
	outStream.open(filename, std::ios::out | std::ios::trunc);

	SaveToFile( "======LOG START" + str + "======", firstTick );

	inited = true;
}

void Logger::SaveToFile( std::string str, std::chrono::milliseconds time )
{
	char temp[36];
	memset(temp, 0, sizeof(char) * 36);
	_itoa_s( static_cast<int>(time.count()), temp, 36 * sizeof(char), 10 );

	outStream << "[" << temp << "]" << str << std::endl;
	outStream.flush();
}

void Logger::Log( std::string str )
{
	if( !inited )
		return;

	auto time = detail::getTimeNow() - firstTick;
	SaveToFile( str, time );
}

void Logger::Report( std::string str )
{
	if( !inited )
		return;

	auto time = detail::getTimeNow() - firstTick;
	SaveToFile( "[R]" + str, time );	
}

void Logger::Error( std::string str )
{
	if( !inited )
		return;

	auto time = detail::getTimeNow() - firstTick;
	SaveToFile( "[!]" + str, time );
}

void Logger::FatalError( std::string str )
{
	if( !inited )
		return;

	auto time = detail::getTimeNow() - firstTick;
	SaveToFile( "[!!!]" + str, time );
	//MessageBox( NULL, str.c_str(), "FATAL ERROR", MB_OK | MB_ICONEXCLAMATION );
}

void Logger::Note( std::string str )
{
	if( !inited )
		return;

	auto time = detail::getTimeNow() - firstTick;
	SaveToFile( "[N]" + str, time );
}

Logger Log;