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

extern const std::string IntToStr(const int x);
extern const std::string FloatToStr(const float x);
extern const std::string DoubleToStr(const double x);
extern const std::string BoolToStr(const bool x);
extern const bool	IsWhiteSpace(const char Character);
extern const std::string GetLine( std::fstream& stream );
extern const std::string ClearWhiteSpace( const std::string& str );
extern const std::string GetClearLine( std::fstream& stream );
extern const bool	ContainsString( const std::string& where, const std::string& what );
extern void SplitString( const std::string& str, const std::string& split, std::vector<std::string>& list );
extern const std::string CorrectDirSlashes( const std::string& str );