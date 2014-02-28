/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	inifile.h
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Zawiera definicje klasy do odczytu pliku INI.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <string>
#include <fstream>

class IniFile
{
private:
	std::string file;
	bool opened;
	std::fstream fileStream;
	unsigned int readedValues;
	unsigned int errors;

public:
	IniFile();
	IniFile( std::string filename );
	~IniFile();

	bool Open( std::string filename );
	void Close();

	int		ReadInt( std::string Section, std::string Item, int DefInt );
	float	ReadFloat( std::string Section, std::string Item, float DefFloat );
	bool	ReadBool( std::string Section, std::string Item, bool DefBool );
	std::string ReadStr( std::string Section, std::string Item, std::string DefStr );

private:
	std::string	GetStr();
	std::string GetItemName( std::string str );
	std::string GetItemValue( std::string str );
};