/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	inifile.h
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Zawiera definicje klasy do odczytu pliku INI.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#ifndef _INIFILE_H_
#define _INIFILE_H_

#include "Log.h"
#include <string>
#include <stdio.h>

class MyIniFile
{
private:
	std::string file;
	FILE* fp;
	bool opened;
	unsigned int ReadedValues;
	unsigned int Errors;

	std::string GetStr();
	std::string GetItemName( std::string str );
	std::string GetItemValue( std::string str );
public:
	MyIniFile();
	MyIniFile( std::string filename );
	~MyIniFile();

	bool Open( std::string filename );

	int ReadInt( std::string Section, std::string Item, int DefInt );
	float ReadFloat( std::string Section, std::string Item, float DefFloat );
	bool ReadBool( std::string Section, std::string Item, bool DefBool );
	std::string ReadStr( std::string Section, std::string Item, std::string DefStr );

	void Close();
};

#endif