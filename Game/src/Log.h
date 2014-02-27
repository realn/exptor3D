/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	Log.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Klasa odpowiedzialna za mechanizm "logowania" czyli
		zapisywaniu stanu programu.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#ifndef _LOG_H_
#define _LOG_H_

#include <windows.h>
#include <stdio.h>
#include <string>

class Logger
{
private:
	bool inited;
	unsigned int FirstTick;
	std::string FileName;
	void SaveToFile( std::string str, unsigned int time );
public:
	Logger();
	void Init( std::string filename, std::string str = "" );
	void Report( std::string str );
	void Log( std::string str );
	void Note( std::string str );
	void Error( std::string str );
	void FatalError( std::string str );
};

extern Logger Log;

#endif