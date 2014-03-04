/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	StrEx.cpp
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Patrz -> StrEx.h.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include "StrEx.h"
#include <sstream>

const std::string IntToStr(const int x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

const std::string FloatToStr(const float x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

const std::string DoubleToStr(const double x) 
{
	std::ostringstream oss;
	oss << x;
	return oss.str();
}

const std::string BoolToStr(const bool x) 
{
	if(x) 
		return "TRUE";
	return "FALSE";
}

const bool	IsWhiteSpace( const char Character )
{
	return Character == ' ' || Character == '\t' || Character == '\r' || Character == '\n';
}

const std::string GetLine( std::fstream& stream )
{
	const unsigned BUFFER_SIZE = 1024;
	char	buf[BUFFER_SIZE + 1];
	int		len = 0;
	bool	repeat = false;
	bool	wfchar = false;

	unsigned pos = 0;
	std::string line;
	std::string result;

	do
	{
		repeat = false;
		wfchar = false;

		memset(buf, 0, BUFFER_SIZE + 1);
		stream.getline(buf, BUFFER_SIZE);
		line = buf;

		if(line.empty() && stream)
		{
			repeat = true;
			continue;
		}
	
		pos = 0;
		for( ; pos < line.length(); pos++ )
		{
			if(!IsWhiteSpace(line[pos]))
				break;
		}

		if( pos >= line.length() )
		{
			repeat = true;
			continue;
		}

		bool possibleComment = true;
		for( ; pos < line.length(); pos++ )
		{
			if( ( line[pos] == '#' || line[pos] == '/' ) && possibleComment )
			{
				repeat = true;
				break;
			}

			possibleComment = false;
			if(IsWhiteSpace(line[pos]))
			{
				if(!wfchar)
				{
					wfchar = true;
					result += ' ';
				}
				continue;
			}
			else
				wfchar = false;

			result += line[pos];
		}
	}
	while( repeat );

	return result;
}

const std::string ClearWhiteSpace( const std::string& str )
{
	std::string result;
	for( unsigned i = 0; i < str.length(); i++ )
	{
		if( !IsWhiteSpace( str[i] ) )
			result += str[i];
	}
	return result;
}

const std::string GetClearLine( std::fstream& stream )
{
	return ClearWhiteSpace( GetLine( stream ) );
}

const bool	ContainsString( const std::string& where, const std::string& what )
{
	return where.find( what ) != std::string::npos;
}

void SplitString( const std::string& str, const std::string& split, std::vector<std::string>& list )
{
	size_t pos = 0;
	size_t next = 0;
	while( ( next = str.find( split, pos ) ) != std::string::npos )
	{
		if( next - pos > 0 )
			list.push_back( str.substr( pos, next - pos ) );

		pos = next + 1;
	}
	if( pos < str.length() )
		list.push_back( str.substr( pos ) );
}

const std::string	CorrectDirSlashes( const std::string& str )
{
	std::string result( str );
	for( unsigned i = 0; i < result.length(); i++ )
	{
		if( result[i] == '\\' )
			result[i] = '/';
	}
	return result;
}