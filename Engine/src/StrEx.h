/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	StrEx.h
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Dodatkowe funkcje konwertuj¹ce.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <string>
#include <fstream>
#include <vector>

extern const std::string	IntToStr(const int x);
extern const std::string	FloatToStr(const float x);
extern const std::string	DoubleToStr(const double x);
extern const std::string	BoolToStr(const bool x);

extern const int		StrToInt(const std::string& str);
extern const unsigned	StrToUInt(const std::string& str);
extern const float		StrToFloat(const std::string& str);
extern const bool		StrToBool(const std::string& str);

extern const bool			IsWhiteSpace(const char Character);
extern const std::string	GetLine( std::fstream& stream );
extern const std::string	ClearWhiteSpace( const std::string& str );
extern const std::string	ClearWhiteSpaceFront( const std::string& str );
extern const std::string	GetClearLine( std::fstream& stream );
extern const bool			ContainsString( const std::string& where, const std::string& what );
extern void					SplitString( const std::string& str, const std::string& split, std::vector<std::string>& list );
extern const std::string	JoinString( const std::vector<std::string>& list, const std::string& glue = "" );
extern const std::string	CorrectDirSlashes( const std::string& str );
extern const bool			ParseNameValue( const std::string& str, std::string& name, std::string& value );