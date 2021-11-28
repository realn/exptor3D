/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Log.h
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Klasa odpowiedzialna za mechanizm "logowania" czyli
		zapisywaniu stanu programu.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <chrono>
#include <string>
#include <fstream>

/// <summary>
/// 
/// </summary>
class Logger
{
private:
	bool			inited;
	std::chrono::milliseconds	firstTick;
	std::string		fileName;
	std::ofstream	outStream;

public:
	Logger();
	~Logger();

	void	Init( std::string filename, std::string str = "" );
	void	Report( std::string str );
	void	Log( std::string str );
	void	Note( std::string str );
	void	Error( std::string str );
	void	FatalError( std::string str );

private:
	void SaveToFile( std::string str, std::chrono::milliseconds time );
};

extern Logger Log;