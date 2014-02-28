/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	inifile.cpp
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Patrz -> inifile.h.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "inifile.h"
#include "Log.h"
#include "StrEx.h"

const unsigned MAX_BUFFER_SIZE = 512;

IniFile::IniFile() :
	file("-"),
	readedValues(0),
	errors(0),
	opened(false)
{
}

IniFile::IniFile( std::string filename ) :
	file(filename),
	readedValues(0),
	errors(0),
	opened(false)
{
	Open( filename );
}

IniFile::~IniFile()
{
	Close();
}

bool IniFile::Open( std::string filename )
{
	if( filename.empty() )
	{
		Log.Error( "IniFile( " + file + " ): Pusty ci¹g znaków." );
		return false;
	}
	Close();

	fileStream.open(filename, std::ios::in);
	if( !fileStream.is_open() )
	{
		Log.Error( "IniFile( " + file + " ): Œcie¿ka niew³aœciwa lub plik nie istnieje: " + filename );
		return false;
	}

	file = filename;
	opened = true;
	readedValues = 0;
	errors = 0;

	return true;
}

void IniFile::Close()
{
	if( !opened )
		return;

	Log.Report( "IniFile( " + file + " ): Zakoñczono parsowanie pliku: " + file + ", odczytano " + IntToStr( readedValues ) + " wartoœci, przy " + IntToStr( errors ) + " b³êdach." );

	fileStream.close();
	opened = false;
}

std::string IniFile::GetStr()
{
	if( !opened )
		return "";

	char buf[MAX_BUFFER_SIZE + 1];
	memset(buf, 0, MAX_BUFFER_SIZE + 1);
	fileStream.getline(buf, MAX_BUFFER_SIZE);

	return buf;
}

std::string IniFile::GetItemName( std::string str )
{
	auto i = str.find('=');
	if( i == std::string::npos )
	{
		Log.Error( "IniFile( " + file + " ): B³¹d, brak znaku równoœci: " + str );
		errors++;
		return "ERROR";
	}
	return str.substr( 0, i );
}

std::string IniFile::GetItemValue( std::string str )
{
	auto i = str.find('=');
	if( i == std::string::npos )
	{
		Log.Error( "IniFile( " + file + " ): B³¹d, brak znaku równoœci: " + str );
		errors++;
		return "ERROR";
	}
	return str.substr( i+1 );
}

std::string IniFile::ReadStr( std::string Section, std::string Item, std::string DefStr )
{
	if( !opened )
		return DefStr;

	fileStream.seekg(0);
	std::string str;

	while( !fileStream.eof() )
	{
		str = GetStr();
		
		if( str == "[" + Section + "]" )
		{
			do
			{
				str = GetStr();

				if( GetItemName( str ) == Item )
				{
					readedValues++;
					return GetItemValue( str );
				}
			}
			while( !fileStream.eof() && str[0] != '[' );
		}
	}

	return DefStr;
}

int IniFile::ReadInt( std::string Section, std::string Item, int DefInt )
{
	if( !opened )
		return DefInt;

	auto str = ReadStr(Section, Item, IntToStr(DefInt));

	return atoi(str.c_str());
}

float IniFile::ReadFloat( std::string Section, std::string Item, float DefFloat )
{
	if( !opened )
		return DefFloat;

	auto str = ReadStr(Section, Item, FloatToStr(DefFloat));
	return (float)atof(str.c_str());
}

bool IniFile::ReadBool( std::string Section, std::string Item, bool DefBool )
{
	if( !opened )
		return DefBool;

	auto str = ReadStr(Section, Item, BoolToStr(DefBool));
	
	if(str == "TRUE" || str == "1")
		return true;
	if(str == "FALSE" || str == "0")
		return false;

	return DefBool;
}