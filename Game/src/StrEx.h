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

extern const std::string IntToStr(const int x);
extern const std::string FloatToStr(const float x);
extern const std::string DoubleToStr(const double x);
extern const std::string BoolToStr(const bool x);
extern const bool	IsWhiteSpace(const char Character);
extern const std::string GetLine( std::fstream& stream );