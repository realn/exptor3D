/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	inifile.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> inifile.h.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "inifile.h"
#include "gui.h"

MyIniFile::MyIniFile()
{
	file = "-";
	ReadedValues = 0;
	Errors = 0;
	fp = NULL;
	opened = false;
}

MyIniFile::MyIniFile( std::string filename )
{
	MyIniFile();
	Open( filename );
}

MyIniFile::~MyIniFile()
{
	if( opened )
		Close();
}

bool MyIniFile::Open( std::string filename )
{
	if( filename.empty() )
	{
		Log.Error( "MYINIFILE( " + file + " ): Pusty ci¹g znaków." );
		return false;
	}

	if( opened )
		Close();

	fopen_s(&fp, filename.c_str(), "r+t" );

	if( !fp )
	{
		Log.Error( "MYINIFILE( " + file + " ): Œcie¿ka niew³aœciwa lub plik nie istnieje: " + filename );
		return false;
	}

	file = filename;
	opened = true;
	ReadedValues = 0;
	Errors = 0;

	return true;
}

void MyIniFile::Close()
{
	if( !opened )
		return;

	Log.Report( "MYINIFILE( " + file + " ): Zakoñczono parsowanie pliku: " + file + ", odczytano " + guiIntToStr( ReadedValues ) + " wartoœci, przy " + guiIntToStr( Errors ) + " b³êdach." );

	fclose( fp );
	opened = true;
}

std::string MyIniFile::GetStr()
{
	if( !opened )
		return "";

	char buf[512];
	fgets( buf, 512, fp );
	if( buf[strlen( buf )-1] == '\n' ) buf[strlen( buf )-1] = 0;
	return buf;
}

std::string MyIniFile::GetItemName( std::string str )
{
	unsigned int i;
	for( i = 0; i < str.length(); i++ )
	{
		if( str[i] == '=' )
			break;
	}

	if( i >= str.length() )
	{
		Log.Error( "MYINIFILE( " + file + " ): B³¹d, brak znaku równoœci: " + str );
		Errors++;
		return "ERROR";
	}

	return str.substr( 0, i );
}

std::string MyIniFile::GetItemValue( std::string str )
{
	unsigned int i;
	for( i = 0; i < str.length(); i++ )
	{
		if( str[i] == '=' )
			break;
	}

	if( i >= str.length() )
	{
		Log.Error( "MYINIFILE( " + file + " ): B³¹d, brak znaku równoœci: " + str );
		Errors++;
		return "ERROR";
	}

	return str.substr( i+1, str.length() - i );
}

std::string MyIniFile::ReadStr( std::string Section, std::string Item, std::string DefStr )
{
	if( !opened )
		return DefStr;

	fseek( fp, 0, 0 );
	std::string str;

	while( !feof( fp ) )
	{
		str = GetStr();
		
		if( str == "[" + Section + "]" )
		{
			do
			{
				str = GetStr();

				if( GetItemName( str ) == Item )
				{
					ReadedValues++;
					return GetItemValue( str );
				}
			}
			while( !feof( fp ) && str[0] != '[' );
		}
	}

	return DefStr;
}

int MyIniFile::ReadInt( std::string Section, std::string Item, int DefInt )
{
	if( !opened )
		return DefInt;

	fseek( fp, 0, 0 );
	std::string str;

	while( !feof( fp ) )
	{
		str = GetStr();
		
		if( str == "[" + Section + "]" )
		{
			do
			{
				str = GetStr();

				if( GetItemName( str ) == Item )
				{
					ReadedValues++;
					return atoi( GetItemValue( str ).c_str() );
				}
			}
			while( !feof( fp ) && str[0] != '[' );
		}
	}

	return DefInt;
}

float MyIniFile::ReadFloat( std::string Section, std::string Item, float DefFloat )
{
	if( !opened )
		return DefFloat;

	fseek( fp, 0, 0 );
	std::string str;

	while( !feof( fp ) )
	{
		str = GetStr();
		
		if( str == "[" + Section + "]" )
		{
			do
			{
				str = GetStr();

				if( GetItemName( str ) == Item )
				{
					ReadedValues++;
					return (float)atof( GetItemValue( str ).c_str() );
				}
			}
			while( !feof( fp ) && str[0] != '[' );
		}
	}

	return DefFloat;
}

bool MyIniFile::ReadBool( std::string Section, std::string Item, bool DefBool )
{
	if( !opened )
		return DefBool;

	fseek( fp, 0, 0 );
	std::string str;

	while( !feof( fp ) )
	{
		str = GetStr();
		
		if( str == "[" + Section + "]" )
		{
			do
			{
				str = GetStr();

				if( GetItemName( str ) == Item )
				{
					ReadedValues++;
					return (bool)atoi( GetItemValue( str ).c_str() );
				}
			}
			while( !feof( fp ) && str[0] != '[' );
		}
	}

	return DefBool;
}